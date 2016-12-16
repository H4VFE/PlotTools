/*

This was subsumed by resolution.cpp, which in turn was taken
in by resolutionChain.cpp, so look at resolutionChain for
comments and explanations.

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

int main(){
  
  float meanAmp1[5], meanAmp2[5], eX1[5] = {0, 0, 0, 0, 0}, eX2[5] = {0, 0, 0, 0, 0}, eY1[5], eY2[5];
  float energy[5] = {20, 50, 100, 150, 200};
  int runs[5] = {5651, 5654, 5655, 5659, 5661};  
  
  TFile *file = new TFile("myfile.root", "RECREATE");
  
  TH1F *hist1 = new TH1F("hist1", "hist1", 10000, -3000, 3000);
  TH1F *hist2 = new TH1F("hist2", "hist2", 10000, -3000, 3000);
  
  TFile *f;
  TTree *h4;
  TString filename;
  
  for(int i = 0; i < 5; i++){
    filename.Form("./ntuples_v1/analysis_%d.root", runs[i]);
    f = TFile::Open(filename.Data());
    
    h4 = (TTree*) f->Get("h4");
    float entries = h4->GetEntries();
    
    file->cd();
    
    h4->Draw("fit_ampl[XTAL_C0_APD1] >> hist1");
    meanAmp1[i] = hist1->GetMean();
    eY1[i] = (hist1->GetRMS())/sqrt(entries);
    hist1->Reset();
    
    h4->Draw("fit_ampl[XTAL_C0_APD2] >> hist2");
    meanAmp2[i] = hist2->GetMean();
    
    
    eY2[i] = (hist2->GetRMS())/sqrt(entries);
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
  
  TMultiGraph *mg = new TMultiGraph("mg", "Energy Resolution Calibration");

  mg->Add(gr1);
  mg->Add(gr2);
  
  mg->Draw("apl");
  
  c->SaveAs("./calibration.png");
  c->SaveAs("./calibration.root");
  
  ////////////////////////////////////////////////////////////////////////////
  
//   float hodoX[2], hodoY[2], fitAmp[26], meanE[5], errE[5], divided[5];
// 
//   for(int i = 0; i < 5; i++){
//     filename.Form("./ntuples_v1/analysis_%d.root", runs[i]);
//     f = TFile::Open(filename.Data());
//     
//     h4 = (TTree*) f->Get("h4");
//     
//     h4->SetBranchAddress("X", hodoX);
//     h4->SetBranchAddress("Y", hodoY);
//     h4->SetBranchAddress("fit_ampl", fitAmp);
//     
//     cout << i << endl;
//     
//     TH1F *eHist = new TH1F("eHist", "eHist", 10000, -3000, 3000);
//     
//     for(int iEvt = 0; iEvt < h4->GetEntries(); iEvt++){
//       h4->GetEntry(iEvt);
// 
//       if(abs(hodoX[0]) < 5 && abs(hodoY[0]) < 5 && abs(hodoX[1]) < 5 && abs(hodoY[1]) < 5)
// 	eHist->Fill(fitAmp[9]/slope2 - fitAmp[8]/slope1);
//     }
//     
//     meanE[i] = eHist->GetMean();
//     errE[i] = eHist->GetRMS();
//     
//     divided[i] = errE[i]/meanE[i];
//     
//     eHist->Reset();
//   }
//   
//   TCanvas *c2 = new TCanvas("c2", "c2", 600, 600);
// 
//   TGraphErrors *eGr = new TGraphErrors(5, energy, divided, eX1, eX1);
//   
//   eGr->Draw("apl");
//   
//   c2->SaveAs("./test.png");
//   c2->SaveAs("./test.root");
  
  return 1;
}