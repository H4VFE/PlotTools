#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include <vector>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
  
  const float FIXMAX = atof(argv[1]), VARMIN = atof(argv[2]), VARMAX = atof(argv[3]), STEP = atof(argv[4]);
  
  TCanvas *c = new TCanvas("c", "c", 1800, 600);
  c->Divide(3,1);
  
  TMultiGraph *mgX = new TMultiGraph("mgX", "Normalized Calibration Constant on X");
  TMultiGraph *mgY = new TMultiGraph("mgY", "Normalized Calibration Constant on Y");
  TMultiGraph *mgXY = new TMultiGraph("mgXY", "Ratio of Normalized Calibration Constants for Y/X");
  
  TString filename;
  TFile *file;
  
  int range = (int) (VARMAX - VARMIN);
  range++;
  vector<TGraphErrors*> graphs(range*3);
  
  for(int i = 0; i < range; i++){
    
    filename.Form("./ResolutionX/%g_%g_%g/resolutionX_%g_%g_%g.root", FIXMAX, VARMIN+i, STEP, FIXMAX, VARMIN+i, STEP);
    file = TFile::Open(filename.Data());
    graphs.at(i) = (TGraphErrors*) file->Get("gr");
    graphs.at(i)->SetLineColor(kBlue+3*(i-3));
    graphs.at(i)->SetMarkerColor(kBlue+3*(i-3));
    graphs.at(i)->SetMarkerStyle(20+i);
    
    filename.Form("./ResolutionY/%g_%g_%g/resolutionY_%g_%g_%g.root", FIXMAX, VARMIN+i, STEP, FIXMAX, VARMIN+i, STEP);
    file = TFile::Open(filename.Data());
    graphs.at(i+range) = (TGraphErrors*) file->Get("gr");
    graphs.at(i+range)->SetLineColor(kBlue+3*(i-3));
    graphs.at(i+range)->SetMarkerColor(kBlue+3*(i-3));
    graphs.at(i+range)->SetMarkerStyle(20+i);
  }
  
  /////////////////////////  Normalizing  /////////////////////////
  
  float max1 = 0, max2 = 0;
  
  int numPts = (2*FIXMAX)/STEP; //Number of points between -2 and 2
  
  for(int pt = 0; pt < numPts; pt++){
    for(int grs = 0; grs < range; grs++){
      if(max1 < graphs.at(grs)->GetY()[pt])
	max1 = graphs.at(grs)->GetY()[pt];
      
      if(max2 < graphs.at(grs+range)->GetY()[pt])
	max2 = graphs.at(grs+range)->GetY()[pt];
    }
  }
   
  for(int grs = 0; grs < range; grs++){
    int N = (int) 2 * graphs.at(grs)->GetN();
    double ratio[N];
    double errRatioX[N];
    double errRatioY[N];
    double eX1, eX2, eY1, eY2, ptX, ptY;
    
    
    for(int i = 0; i < graphs.at(grs)->GetN(); i++){
      graphs.at(grs)->GetY()[i] = (graphs.at(grs)->GetY()[i])/max1;
      graphs.at(grs+range)->GetY()[i] = (graphs.at(grs+range)->GetY()[i])/max2;
      
      graphs.at(grs)->GetEY()[i] = (graphs.at(grs)->GetEY()[i])/max1;
      graphs.at(grs+range)->GetEY()[i] = (graphs.at(grs+range)->GetEY()[i])/max2;
      
      ptX = graphs.at(grs)->GetY()[i];
      ptY = graphs.at(grs+range)->GetY()[i];
      ratio[i] = ptY/ptX;
      
      eX1 = graphs.at(grs)->GetEX()[i];
      eY1 = graphs.at(grs)->GetEY()[i];
      eX2 = graphs.at(grs+range)->GetEX()[i];
      eY2 = graphs.at(grs+range)->GetEY()[i];
      errRatioX[i] = eX1;
      errRatioY[i] = ratio[i]*sqrt((eY1*eY1)/(ptY*ptY) + (eY2*eY2)/(ptY*ptY));
    }
    
    mgX->Add(graphs.at(grs));
    mgY->Add(graphs.at(grs+range));
    
    graphs.at(grs+2*range) = new TGraphErrors(graphs.at(grs)->GetN(), graphs.at(grs)->GetX(), ratio, errRatioX, errRatioY);
    graphs.at(grs+2*range)->SetLineColor(kBlue+3*(grs-3));
    graphs.at(grs+2*range)->SetMarkerColor(kBlue+3*(grs-3));
    graphs.at(grs+2*range)->SetMarkerStyle(20+grs);
    mgXY->Add(graphs.at(grs+2*range));
  }
    
    
//   TF1 *p1 = new TF1("p1", "[0]*(x-[1])*(x-[2]) + [3]", -1*FIXMAX, FIXMAX);
//   TF1 *p2 = new TF1("p2", "[0]*(x-[1])*(x-[2]) + [3]", -1*FIXMAX, FIXMAX);
//   
//   p1->SetParameters(-1, 0, 0, 1);
//   p2->SetParameters(-1, 0, 0, 1);
//   
//   graphs.at(range-1)->Fit(p1, "RQME");
//   graphs.at(2*range-1)->Fit(p2, "RQME");
//   
//   float fitmax1 = p1->GetParameter(1);
//   float fitmax2 = p2->GetParameter(1);
//   
//   float fitDiff = fitmax1 - fitmax2;
//   
//   cout << "X Center: " << fitmax1 << " Y Center: " << fitmax2 << " Difference: " << fitDiff << endl;
  /////////////////////////  Drawing the X Graph  /////////////////////////
  c->cd(1);
  mgX->Draw("apl");
  mgX->GetXaxis()->SetTitle("Hodoscope X Position (mm)");
  //mgX->GetYaxis()->SetTitle("m1+m2");
  mgX->GetYaxis()->SetTitle("m2/m1");
  mgX->GetYaxis()->SetTitleOffset(1.75);
  mgX->GetYaxis()->SetRangeUser(.9, 1.05);
  
  c->SetLeftMargin(0.15);
  
  TLegend* legX = new TLegend(0.1,0.1,0.5,0.3);
  legX->SetNColumns(2);
  TString entryName;
  for(int i = 0; i < range; i++){
    entryName.Form("Y in [-%g,%g]", (VARMIN+i)/2, (VARMIN+i)/2);
    legX->AddEntry(graphs.at(i),entryName.Data(),"lp");
  }
  legX->Draw();
  
  //p1->Draw("same");
  
  /////////////////////////  Drawing the Y Graph  /////////////////////////
  c->cd(2);
  mgY->Draw("apl");
  mgY->GetXaxis()->SetTitle("Hodoscope Y Position (mm)");
  //mgY->GetYaxis()->SetTitle("m1+m2");
  mgY->GetYaxis()->SetTitle("m2/m1");
  mgY->GetYaxis()->SetTitleOffset(1.75);
  mgY->GetYaxis()->SetRangeUser(.9, 1.05);
  
  c->SetLeftMargin(0.15);
  
  TLegend* legY = new TLegend(0.1,0.1,0.5,0.3);
  legY->SetNColumns(2);
  for(int i = 0; i < range; i++){
    entryName.Form("X in [-%g,%g]", (VARMIN+i)/2, (VARMIN+i)/2);
    legY->AddEntry(graphs.at(i+range),entryName.Data(),"lp");
  }
  legY->Draw();
  
  //p2->Draw("same");
  
  /////////////////////////  Drawing the Ratio  /////////////////////////
  c->cd(3);
  mgXY->Draw("apl");
  mgXY->GetXaxis()->SetTitle("Hodoscope Position (mm)");
  mgXY->GetYaxis()->SetTitle("(my2/my1)/(mx2/mx1)");
  //mgXY->GetYaxis()->SetTitle("(my1+my2)/(mx1+mx2)");
  mgXY->GetYaxis()->SetTitleOffset(1.75);
  mgXY->GetYaxis()->SetRangeUser(.95, 1.05);
  
  c->SetLeftMargin(0.15);
  
  TLegend* legXY = new TLegend(0.1,0.1,0.5,0.3);
  legXY->SetNColumns(2);
  for(int i = 0; i < range; i++){
    entryName.Form("Range in [-%g,%g]", (VARMIN+i)/2, (VARMIN+i)/2);
    legXY->AddEntry(graphs.at(i+2*range),entryName.Data(),"lp");
  }
  legXY->Draw();
  
  /////////////////////////  Saving  /////////////////////////
  TString saveName;
  saveName.Form("Resolution Comparison %g_%g-%g_%g.png", FIXMAX, VARMIN, VARMAX, STEP);
  c->SaveAs(saveName.Data());
  
  saveName.Form("Resolution Comparison %g_%g-%g_%g.root", FIXMAX, VARMIN, VARMAX, STEP);
  c->SaveAs(saveName.Data());
  
  return 1;
}