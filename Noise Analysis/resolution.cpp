/*

This code is made obsolete by resolutionChain.cpp
Please refer to that file for comments and explanations

Will Benoit
December 8th 2016

*/

#include "TTree.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include <iostream>

using namespace std;

float getSlopeRatio(float xmin, float xmax, float ymin, float y max); 

int main(){

  getSlopeRatio(-5, 5, -5, 5);
  
  return 1;
}

float getSlopeRatio(float xmin, float xmax, float ymin, float y max){
  
  float meanAmp1[5], meanAmp2[5], eX1[5] = {0, 0, 0, 0, 0}, eX2[5] = {0, 0, 0, 0, 0}, eY1[5], eY2[5];
  float energy[5] = {20, 50, 100, 150, 200};
  int runs[5] = {5651, 5654, 5655, 5659, 5661};  
  
  TFile *file = new TFile("myfile.root", "RECREATE");
  
  TH1F *hist1 = new TH1F("hist1", "hist1", 10000, -3000, 3000);
  TH1F *hist2 = new TH1F("hist2", "hist2", 10000, -3000, 3000);
  
  TFile *f;
  TTree *h4;
  TString filename, cut;
  
  for(int i = 0; i < 5; i++){
    filename.Form("./ntuples_v1/analysis_%d.root", runs[i]);
    f = TFile::Open(filename.Data());
    
    h4 = (TTree*) f->Get("h4");
    
    file->cd();
    
    cut1.Form("X > %d && X < %d && Y > %d && Y < %d", xmin, xmax, ymin, ymax);
    
    long int entries1 = h4->Draw("fit_ampl[XTAL_C0_APD1] >> hist1", cut.Data());
    long int entries2 = h4->Draw("fit_ampl[XTAL_C0_APD2] >> hist2", cut.Data());
    
    if(entries1 < 1000 || entries2 < 1000) return 0;
    
    meanAmp1[i] = hist1->GetMean();
    eY1[i] = (hist1->GetRMS())/sqrt(entries1);
    hist1->Reset();
    
    meanAmp2[i] = hist2->GetMean();
    eY2[i] = (hist2->GetRMS())/sqrt(entries2);
    hist2->Reset();
  }
  
  TCanvas *c = new TCanvas("c", "c", 600, 600);

  TGraphErrors *gr1 = new TGraphErrors(5, energy, meanAmp1, eX1, eY1);
  TGraphErrors *gr2 = new TGraphErrors(5, energy, meanAmp2, eX2, eY2);
  
  gr2->SetMarkerColor(kBlue);
  gr2->SetLineColor(kBlue);
  
  TFitResultPtr r1 = gr1->Fit("pol1", "S");
  TFitResultPtr r2 = gr2->Fit("pol1", "S");
  
  float slope1 = r1->Value(0);
  float slope2 = r2->Value(0);
  
  return slope2/slope1;
  
//   TMultiGraph *mg = new TMultiGraph("mg", "Energy Resolution Calibration");
// 
//   mg->Add(gr1);
//   mg->Add(gr2);
//   
//   mg->Draw("apl");
//   
//   c->SaveAs("./calibration.png");
//   c->SaveAs("./calibration.root");
}