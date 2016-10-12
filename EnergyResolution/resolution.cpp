#include "TTree.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TLegend.h"
#include "TMath.h"
#include <vector>
#include <string.h>
#include <algorithm>
#include <iostream>

using namespace std;

vector<float> getSlopeRatio(char slice, const float MAX1, const float MAX2, const float STEP, float xmin, float xmax, float ymin, float ymax); 

int main(int argc, char *argv[]){

  TGraphErrors *gr = new TGraphErrors();
  
  bool x = true;
  const float MAX1 = atof(argv[1]), MAX2 = atof(argv[2]), STEP = atof(argv[3]);
  
  TCanvas *can = new TCanvas("can", "can", 600, 600);
  
  float xmin = -1*MAX1, xmax = MAX1, ymin = -1*MAX2, ymax = MAX2, varmin, varmax;
  
  for(int i = 0; i < 2; i++){
    
    char slice;
    
    if(i == 0) slice = 'X';
    else{
      slice = 'Y';
      swap(xmin, ymin);
      swap(xmax, ymax);
    }
    
    char buffer[100];
    sprintf(buffer,"mkdir -p /afs/cern.ch/user/w/wbenoit/work/EnergyResolution/Resolution%c/%g_%g_%g", slice, MAX1, MAX2, STEP);
    system(buffer);
    
    int num = 0;
    for(float pos = -1*MAX1; pos <= MAX1-STEP; pos+=STEP){
      varmin = pos;
      varmax = pos+STEP;
      
      vector<float> point;
      
      if(x){
	point = getSlopeRatio(slice, MAX1, MAX2, STEP, varmin, varmax, ymin, ymax);
	if(pos == MAX1-STEP) x = false;
      }
      else point = getSlopeRatio(slice, MAX1, MAX2, STEP, xmin, xmax, varmin, varmax);
      
      gr->SetPoint(num, (varmin+varmax)/4, point.at(0));
      gr->SetPointError(num, (float) STEP/4, point.at(1));
      num++;
    }
    
    TString grTitle;
    grTitle.Form("Hodoscope %c Position (mm)", slice);
    
    gr->SetMarkerStyle(21);
    gr->GetXaxis()->SetTitle(grTitle.Data());
    gr->GetYaxis()->SetTitle("m2+m1");
    gr->GetYaxis()->SetTitleOffset(2);
    
    gr->Draw("apl");
    
    can->SetLeftMargin(0.15);
    
    TString title;
    title.Form("./Resolution%c/%g_%g_%g/resolution%c_%g_%g_%g.png", slice, MAX1, MAX2, STEP, slice, MAX1, MAX2, STEP);
    can->SaveAs(title.Data());
    
    title.Form("./Resolution%c/%g_%g_%g/resolution%c_%g_%g_%g.root", slice, MAX1, MAX2, STEP, slice, MAX1, MAX2, STEP);
    TFile *resFile = new TFile(title.Data(), "RECREATE");
    gr->Write("gr");
  }
  
  return 1;
}

vector<float> getSlopeRatio(char slice, const float MAX1, const float MAX2, const float STEP, float xmin, float xmax, float ymin, float ymax){
  
  float meanAmp1[5], meanAmp2[5], eX1[5] = {0, 0, 0, 0, 0}, eX2[5] = {0, 0, 0, 0, 0}, eY1[5], eY2[5];
  float energy[5] = {20, 50, 100, 150, 200};
  int runs[5] = {5651, 5654, 5655, 5659, 5661};
  
  vector<float> point(2);

  point.at(0) = 0;
  point.at(1) = 0;
  
  TFile *file = new TFile("myfile.root", "RECREATE");
  
  TH1F *hist1 = new TH1F("hist1", "hist1", 10000, -3000, 3000);
  TH1F *hist2 = new TH1F("hist2", "hist2", 10000, -3000, 3000);
  
  TFile *f;
  TTree *h4;
  TString filename, cut, histname;
  
  TString newName;
  newName.Form("Resolution%c/%g_%g_%g/%c_%g_%g_%g_%g.root", slice, MAX1, MAX2, STEP, slice, xmin, xmax, ymin, ymax);
    
  TFile *newFile = new TFile(newName.Data(), "RECREATE");
  
  for(int i = 0; i < 5; i++){
    filename.Form("./ntuples_v1/analysis_%d.root", runs[i]);
    f = TFile::Open(filename.Data());
    
    h4 = (TTree*) f->Get("h4");
    
    file->cd();
    
    float xOffset = 2.5;
    
    cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f)", xmin, xmin + xOffset, xmax, xmax + xOffset, ymin, ymin, ymax, ymax);
    h4->Draw("fit_ampl[XTAL_C0_APD1] >> hist1", cut.Data());
    h4->Draw("fit_ampl[XTAL_C0_APD2] >> hist2", cut.Data());
    
    float cutoff = hist1->GetMean() - 3*hist1->GetRMS();
    cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f) && (fit_ampl[XTAL_C0_APD1] > %f)", xmin, xmin + xOffset, xmax, xmax + xOffset, ymin, ymin, ymax, ymax);
    long int entries1 = h4->Draw("fit_ampl[XTAL_C0_APD1] >> hist1", cut.Data());

    TF1 *g1 = new TF1("g1", "gaus", hist1->GetMean() - 2*hist1->GetRMS(), hist1->GetMean() + 2*hist1->GetRMS());
    hist1->Fit(g1, "RQME");
    TF1 *f1 = new TF1("f1","crystalball", hist1->GetMean() - 2*hist1->GetRMS(), hist1->GetMean() + 2*hist1->GetRMS());
    f1->SetParameters(1, g1->GetParameter(1), g1->GetParameter(2)/3, 1, .5);
    f1->SetParLimits(1, g1->GetParameter(1) - 2*g1->GetParameter(2), g1->GetParameter(1) + 2*g1->GetParameter(2));
    f1->SetParLimits(4, 0.1, 4);
    f1->SetParLimits(3, 0.1, 10);
    f1->SetParLimits(2, (g1->GetParameter(2))/10, g1->GetParameter(2)/2);
    hist1->Fit(f1, "RQME");
    
    if(abs(f1->GetParameter(1)-hist1->GetMean()) > hist1->GetRMS() || f1->GetParameter(2) < hist1->GetRMS()/10){
      f1->SetParameter(1, hist1->GetMean());
      f1->SetParLimits(1, hist1->GetMean() - hist1->GetRMS(), hist1->GetMean() + hist1->GetRMS());
      f1->SetParameter(2, hist1->GetRMS()/3);
      f1->SetParLimits(2, hist1->GetRMS()/10, hist1->GetRMS()/2);
      hist1->Fit(f1, "RQME");
    }
    
    newFile->cd();
    histname.Form("APD1_%d", runs[i]);
    hist1->Write(histname.Data());
    file->cd();
    
    cutoff = hist2->GetMean() - 3*hist2->GetRMS();
    cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f) && (fit_ampl[XTAL_C0_APD2] > %f)", xmin, xmin + xOffset, xmax, xmax + xOffset, ymin, ymin, ymax, ymax);
    long int entries2 = h4->Draw("fit_ampl[XTAL_C0_APD2] >> hist2", cut.Data());
    
    TF1 *g2 = new TF1("g2", "gaus", hist2->GetMean() - 2*hist2->GetRMS(), hist2->GetMean() + 2*hist2->GetRMS());
    hist2->Fit(g2, "RQME");
    TF1 *f2 = new TF1("f2","crystalball", hist2->GetMean() - 2*hist2->GetRMS(), hist2->GetMean() + 2*hist2->GetRMS());
    f2->SetParameters(1, g2->GetParameter(1), g2->GetParameter(2)/3, 1, .5);
    f2->SetParLimits(1, g2->GetParameter(1) - 2*g2->GetParameter(2), g2->GetParameter(1) + 2*g2->GetParameter(2));
    f2->SetParLimits(4, 0.1, 4);
    f2->SetParLimits(3, 0.1, 10);
    f2->SetParLimits(2, (g2->GetParameter(2))/10, g2->GetParameter(2)/2);
    hist2->Fit(f2, "RQME");
    
    if(abs(f2->GetParameter(1)-hist2->GetMean()) > hist2->GetRMS() || f2->GetParameter(2) < hist2->GetRMS()/10){
      f2->SetParameter(1, hist2->GetMean());
      f2->SetParLimits(1, hist2->GetMean() - hist2->GetRMS(), hist2->GetMean() + hist2->GetRMS());
      f2->SetParameter(2, hist2->GetRMS()/3);
      f2->SetParLimits(2, hist2->GetRMS()/10, hist2->GetRMS()/2);
      hist2->Fit(f2, "RQME");
    }
       
    newFile->cd();
    histname.Form("APD2_%d", runs[i]);
    hist2->Write(histname.Data());

    meanAmp1[i] = f1->GetParameter(1);
    eY1[i] = f1->GetParError(1);
    hist1->Reset();
    
    meanAmp2[i] = f2->GetParameter(1);
    eY2[i] = f2->GetParError(1);
    hist2->Reset();
  }

  TGraphErrors *gr1 = new TGraphErrors(5, energy, meanAmp1, eX1, eY1);
  TGraphErrors *gr2 = new TGraphErrors(5, energy, meanAmp2, eX2, eY2);
  
  gr1->GetXaxis()->SetTitle("Beam Energy");
  gr1->GetYaxis()->SetTitle("Mean Fit Amplitude");
  
  gr1->SetMarkerStyle(21);
  gr2->SetMarkerStyle(20);
  gr2->SetMarkerColor(kBlue);
  gr2->SetLineColor(kBlue);
  
  TFitResultPtr r1 = gr1->Fit("pol1", "S MEQ");
  TFitResultPtr r2 = gr2->Fit("pol1", "S MEQ");
  
  float slope1 = r1->Value(1);
  float slope2 = r2->Value(1);
  
  float err1 = r1->ParError(1);
  float err2 = r2->ParError(1);
  
  float chi1 = r1->Chi2();
  float chi2 = r2->Chi2();
  
  //cout << chi1 << " " << chi2 << endl;
  
  TMultiGraph *mg = new TMultiGraph("mg", "Energy Resolution Calibration");

  mg->Add(gr1);
  mg->Add(gr2);
  
  mg->Draw("apl");
  mg->GetXaxis()->SetTitle("Beam Energy (GeV)");
  mg->GetYaxis()->SetTitle("Mean Fit Amplitude");
  
  TLegend* leg = new TLegend(0.1,0.7,0.5,0.9);
  leg->AddEntry(gr1,"APD1","lp");
  leg->AddEntry(gr2,"APD2","lp");
  leg->Draw();
  
  mg->Write();
  
//   point.at(0)= slope2+slope1;
//   point.at(1) = sqrt((err1*err1) + (err2*err2));
//   
  point.at(0)= slope2/slope1;
  point.at(1) = point.at(0)*sqrt((err1*err1)/(slope1*slope1) + (err2*err2)/(slope2*slope2));
  
  return point;
}