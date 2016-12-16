/*

Used to check if there was a correlation between the RMS and
the slope of the pedestal, as removing events with a high
slope could have improved results

Actually, now that I've read through all this, I don't think
this does anything. The for-loop doesn't really make any sense,
and the output file is nothing. I probably wrote this, decided
it was easier just to do in-terminal, and never completed it.
Don't bother, it's not important, and cutting on the slope
didn't improve things anyway.

Will Benoit
December 6th 2016

*/

#include "TTree.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH2F.h"
#include "TCanvas.h"

#include <iostream>

int main(int argc, char *argv[]){ //Takes a run number as an argument
 
  char* runNum = argv[1];
  
  //Open file, get TTree
  TString t;
  t.Form("analysis_%s.root", runNum);
  
  TFile *file = TFile::Open(t.Data());
  TTree *h4 = (TTree*) file->Get("h4");
  
  int events = h4->GetEntries();
  
  //std::cout << events << std::endl; //Probably just used as a check
  
  float tempSlope[26], tempRms[26]; //Arrays to contain the slope and RMS for each channel while... Hold on, does this code even make sense?
  
  float slope[events], rms[events];
  
  h4->SetBranchAddress("b_slope", slope);
  h4->SetBranchAddress("b_rms", rms);
  
  //Garbage loop
  for(int iEvt = 0; iEvt < events; iEvt++){
    h4->GetEntry(iEvt);
    
    //Like, what? Filling slope and rms with junk? And why 5? We may never know...
    slope[iEvt] = tempSlope[5];
    rms[iEvt] = tempRms[5];
  }
  
  TCanvas *c = new TCanvas("c", "c", 600, 600);
  
  TGraph *gr = new TGraph(events, rms, slope);
  
  gr->Draw("ap");
  
  c->SaveAs("slopeVSrms.png");
  
  
  return 1;
}