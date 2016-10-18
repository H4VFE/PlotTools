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
#include "TChain.h"
#include <vector>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>

using namespace std;

TH1F *chi2 = new TH1F("chi2", "chi2", 50, 0, .5);
TFile *goodFits = new TFile("goodFits.root", "RECREATE");
TFile *badFits = new TFile("badFits.root", "RECREATE");
TFile *allFits = new TFile("allFits.root", "RECREATE");

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
    sprintf(buffer,"mkdir -p /afs/cern.ch/user/w/wbenoit/work/PlotTools/EnergyResolution/Resolution%c/%g_%g_%g", slice, MAX1, MAX2, STEP);
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
    gr->GetYaxis()->SetTitle("m2/m1");
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
  
  //chi2->SaveAs("chi2.root");
  
  return 1;
}

vector<float> getSlopeRatio(char slice, const float MAX1, const float MAX2, const float STEP, float xmin, float xmax, float ymin, float ymax){
  
//   TString fitFilename;
//   
//   fitFilename.Form("goodFits/Fit_%c_%g_%g_%g_%g.root", slice, xmin, xmax, ymin, ymax);
//   TFile *goodFits = new TFile(fitFilename.Data(), "RECREATE");
//   
//   fitFilename.Form("badFits/Fit_%c_%g_%g_%g_%g.root", slice, xmin, xmax, ymin, ymax);
//   TFile *badFits = new TFile(fitFilename.Data(), "RECREATE");
//   
//   fitFilename.Form("allFits/Fit_%c_%g_%g_%g_%g.root", slice, xmin, xmax, ymin, ymax);
//   TFile *allFits = new TFile(fitFilename.Data(), "RECREATE");
  
  float meanAmp1[5] = {0}, meanAmp2[5] = {0}, eX1[5] = {0}, eX2[5] = {0}, eY1[5] = {0}, eY2[5] = {0};
  float energy[5] = {20, 50, 100, 150, 200}; //Different beam energies
  int runs[5][3] = {{0, 5650, 5651}, {5611, 5653, 5654}, {5619, 5656, 5655}, {5649, 5658, 5659}, {5630, 5660, 5661}}; //tag Runs used at different gains
  
  vector<float> point(2); //Vector to be returned

  point.at(0) = 0;
  point.at(1) = 0;
  
  TFile *file = new TFile("myfile.root", "RECREATE");
  
  TH1F *hist1 = new TH1F("hist1", "hist1", 10000, -3000, 3000); //Histograms for APDs 1 and 2
  TH1F *hist2 = new TH1F("hist2", "hist2", 10000, -3000, 3000);
  
  TTree *h4;
  TF1 *g1, *g2, *f1, *f2;
  TString filename, plot1, plot2, cut, histname, newName;
  float scaleFactor;
  vector<TChain*> chains(5); //Vector of TChains, each one containing all runs at a particular energy
  chains.at(0) = new TChain("h4");
  chains.at(1) = new TChain("h4");
  chains.at(2) = new TChain("h4");
  chains.at(3) = new TChain("h4");
  chains.at(4) = new TChain("h4");
  
  newName.Form("Resolution%c/%g_%g_%g/%c_%g_%g_%g_%g.root", slice, MAX1, MAX2, STEP, slice, xmin, xmax, ymin, ymax);
  
  TFile *newFile = new TFile(newName.Data(), "RECREATE"); //File that stores the two histograms for each slice
  
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 3; j++){ //Add the root files to each TChain in the vector
      if(i == 0 && j == 0) continue;
      
      filename.Form("./ntuples_v1/analysis_%d.root", runs[i][j]); //tag
      chains.at(i)->Add(filename.Data());
    }
      
    file->cd(); //Need this so the histograms have a place to "live"
    
    float xOffset = 2.5; //Difference between X[0] and X[1]. Determined by Draw("X[0]:X[1]"). There is no Y offset
    
    //Plot the fit_ampl of all the runs, multiplied by the appropriate factor to account for differences in gain
    plot1.Form("fit_ampl[XTAL_C0_APD1] * (4*((run == 5611) + (run == 5619) + (run == 5649) + (run == 5630)) + 2*((run == 5650) + (run == 5653) + (run == 5656) + (run == 5658) + (run == 5660)) + "
	       "((run == 5651) + (run == 5654) + (run == 5655) + (run == 5659) + (run == 5661))) >> hist1");
    plot2.Form("fit_ampl[XTAL_C0_APD2] * (4*((run == 5611) + (run == 5619) + (run == 5649) + (run == 5630)) + 2*((run == 5650) + (run == 5653) + (run == 5656) + (run == 5658) + (run == 5660)) + "
	       "((run == 5651) + (run == 5654) + (run == 5655) + (run == 5659) + (run == 5661))) >> hist2"); //tag
    
    //Cut to look only within the specified slice 
    cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f)", xmin, xmin + xOffset, xmax, xmax + xOffset, ymin, ymin, ymax, ymax);
    
    chains.at(i)->Draw(plot1.Data(), cut.Data());
    chains.at(i)->Draw(plot2.Data(), cut.Data());
    
    allFits->cd();
    hist1->Write("hist1");
    hist2->Write("hist2");
    
    float cutoff = hist1->GetMean() - 3*hist1->GetRMS(); //Designate a cutoff value for fit_ampl based on the previous plot
    cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f) && (fit_ampl[XTAL_C0_APD1] > %f)", xmin, xmin + xOffset, xmax, xmax + xOffset, ymin, ymin, ymax, ymax);
    long int entries1 = chains.at(i)->Draw(plot1.Data(), cut.Data()); //Replot with fit_ampl restriction
    
    //Fit the plot with a Gaussian
    g1 = new TF1("g1", "gaus", hist1->GetMean() - 2*hist1->GetRMS(), hist1->GetMean() + 2*hist1->GetRMS());
    g1->SetParameters(1, hist1->GetMean(), hist1->GetRMS());
    g1->SetParLimits(1, hist1->GetMean() - hist1->GetRMS()/2, hist1->GetMean() + hist1->GetRMS()/2);
    g1->SetParLimits(2, hist1->GetRMS()/10, hist1->GetRMS()/2);
    hist1->Fit(g1, "RQME");
    
    //Fit the plot with a crystalball based on the Gaussian
    f1 = new TF1("f1","crystalball", hist1->GetMean() - 2*hist1->GetRMS(), hist1->GetMean() + 2*hist1->GetRMS());
    f1->SetParameters(1, g1->GetParameter(1), g1->GetParameter(2)/3, 1, .5);
    f1->SetParLimits(1, g1->GetParameter(1) - g1->GetParameter(2), g1->GetParameter(1) + g1->GetParameter(2));
    f1->SetParLimits(4, 0.1, 4);
    f1->SetParLimits(3, 0.1, 10);
    f1->SetParLimits(2, (g1->GetParameter(2))/10, g1->GetParameter(2)/2);
    hist1->Fit(f1, "RQME");
    
    //Attempt to refit if the first failed for some reason, such as a bad Gaussian fit
    if((f1->GetChisquare()/entries1 - 0.2132) > 1*0.03559){ //Using values from chi2 distribution on 6 6 2 run
      
      f1->SetParameter(1, hist1->GetMean());
      f1->SetParameter(2, hist1->GetRMS()/3);
      f1->SetParLimits(1, hist1->GetMean() - hist1->GetRMS()/2, hist1->GetMean() + hist1->GetRMS()/2);
      f1->SetParLimits(2, hist1->GetRMS()/10, hist1->GetRMS()/2);
      hist1->Fit(f1, "RQME");
      
      if((f1->GetChisquare()/entries1 - 0.2132) > 1*0.03559){
	badFits->cd();
	histname.Form("APD1_%d_%g_%g_%g_%g", i, xmin, xmax, ymin, ymax); //tag
	hist1->Write(histname.Data());
      }
      else{
      goodFits->cd();
      histname.Form("APD1_%d_%g_%g_%g_%g", i, xmin, xmax, ymin, ymax); //tag
      hist1->Write(histname.Data());
      }
    }
    
    allFits->cd();
    histname.Form("APD1_%d_%g_%g_%g_%g", i, xmin, xmax, ymin, ymax); //tag
    hist1->Write(histname.Data());
    
    chi2->Fill(f1->GetChisquare()/entries1);
    
    //Save histogram
    newFile->cd();
    histname.Form("APD1_%d", i); //tag
    hist1->Write(histname.Data());
    file->cd();
    
    //Repeat of everything with APD2
    cutoff = hist2->GetMean() - 3*hist2->GetRMS();
    cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f) && (fit_ampl[XTAL_C0_APD2] > %f)", xmin, xmin + xOffset, xmax, xmax + xOffset, ymin, ymin, ymax, ymax);
    long int entries2 = chains.at(i)->Draw(plot2.Data(), cut.Data());
    
    g2 = new TF1("g2", "gaus", hist2->GetMean() - 2*hist2->GetRMS(), hist2->GetMean() + 2*hist2->GetRMS());
    g2->SetParameters(1, hist2->GetMean(), hist2->GetRMS());
    g2->SetParLimits(1, hist2->GetMean() - hist2->GetRMS()/2, hist2->GetMean() + hist2->GetRMS()/2);
    g2->SetParLimits(2, hist2->GetRMS()/10, hist2->GetRMS()/2);
    hist2->Fit(g2, "RQME");
    
    f2 = new TF1("f2","crystalball", hist2->GetMean() - 2*hist2->GetRMS(), hist2->GetMean() + 2*hist2->GetRMS());
    f2->SetParameters(1, g2->GetParameter(1), g2->GetParameter(2)/3, 1, .5);
    f2->SetParLimits(1, g2->GetParameter(1) - g2->GetParameter(2), g2->GetParameter(1) + g2->GetParameter(2));
    f2->SetParLimits(4, 0.1, 4);
    f2->SetParLimits(3, 0.1, 10);
    f2->SetParLimits(2, (g2->GetParameter(2))/10, g2->GetParameter(2)/2);
    hist2->Fit(f2, "RQME");
    
    if((f2->GetChisquare()/entries2 - 0.2132) > 1*0.03559){ //Using values from chi2 distribution on 6 6 2 run
      
      f2->SetParameter(1, hist2->GetMean());
      f2->SetParameter(2, hist2->GetRMS()/3);
      f2->SetParLimits(1, hist2->GetMean() - hist2->GetRMS()/2, hist2->GetMean() + hist2->GetRMS()/2);
      f2->SetParLimits(2, hist2->GetRMS()/10, hist2->GetRMS()/2);
      hist2->Fit(f2, "RQME");
      
      if((f2->GetChisquare()/entries2 - 0.2132) > 1*0.03559){
	badFits->cd();
	histname.Form("APD2_%d_%g_%g_%g_%g", i, xmin, xmax, ymin, ymax); //tag
	hist2->Write(histname.Data());
      }
      else{
      goodFits->cd();
      histname.Form("APD2_%d_%g_%g_%g_%g", i, xmin, xmax, ymin, ymax); //tag
      hist2->Write(histname.Data());
      }
    }
    
    allFits->cd();
    histname.Form("APD2_%d_%g_%g_%g_%g", i, xmin, xmax, ymin, ymax); //tag
    hist1->Write(histname.Data());
    
    chi2->Fill(f2->GetChisquare()/entries2);
      
    newFile->cd();
    histname.Form("APD2_%d", i); //tag
    hist2->Write(histname.Data());
    
    //Use the fit parameters as values to fill the arrays
    meanAmp1[i] = f1->GetParameter(1);
    eY1[i] = f1->GetParError(1);
    hist1->Reset();
    
    meanAmp2[i] = f2->GetParameter(1);
    eY2[i] = f2->GetParError(1);
    hist2->Reset();
  }

  //Make graphs from arrays
  TGraphErrors *gr1 = new TGraphErrors(5, energy, meanAmp1, eX1, eY1);
  TGraphErrors *gr2 = new TGraphErrors(5, energy, meanAmp2, eX2, eY2);
  
  //Making the graphs nice
  gr1->GetXaxis()->SetTitle("Beam Energy");
  gr1->GetYaxis()->SetTitle("Mean Fit Amplitude");
  
  gr1->SetMarkerStyle(21);
  gr2->SetMarkerStyle(20);
  gr2->SetMarkerColor(kBlue);
  gr2->SetLineColor(kBlue);
  
  //Linear fit of both graphs
  TFitResultPtr r1 = gr1->Fit("pol1", "S MEQ");
  TFitResultPtr r2 = gr2->Fit("pol1", "S MEQ");
  
  //Getting the slope and error of each fit
  float slope1 = r1->Value(1);
  float slope2 = r2->Value(1);
  
  float err1 = r1->ParError(1);
  float err2 = r2->ParError(1);
  
  float chi1 = r1->Chi2();
  float chi2 = r2->Chi2();
  
  //cout << chi1 << " " << chi2 << endl;
  
  //Plotting both graphs together
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

  //Filling the vector with the relevant values
  point.at(0)= slope2/slope1;
  point.at(1) = point.at(0)*sqrt((err1*err1)/(slope1*slope1) + (err2*err2)/(slope2*slope2)); //Error propagation
  
  return point;
}