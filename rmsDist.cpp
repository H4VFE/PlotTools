#include "TTree.h"
#include <cmath>
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"

TFile *file = TFile::Open("5387/1.root");
TTree *H4tree = (TTree*) file->Get("H4tree");

Float_t vals[36864];
UInt_t grps[36864];
UInt_t chs[36864];
Float_t x[1024], y[1024];

void RMS(Int_t grp = -1, Int_t ch = -1, Int_t events = 0);

int main(){
  Int_t events = H4tree->GetEntries();
  Int_t channels = H4tree->GetMaximum("digiChannel");
  Int_t groups = H4tree->GetMaximum("digiGroup");
  
  H4tree->SetBranchAddress("digiSampleValue", vals);
  H4tree->SetBranchAddress("digiGroup", grps);
  H4tree->SetBranchAddress("digiChannel", chs);
  
  for(int grp = 0; grp < 4; grp++){
    for(int ch = 0; ch < 9; ch++){
      RMS(grp, ch, events);
    }
  }
  
  return 1;
}

void RMS(Int_t grp, Int_t ch, Int_t events){
  TH1F *h = new TH1F("h", "RMS Distribution", sqrt(26712), 0, 20);
  
  Float_t rms, max, min;
  
  for(int iEnt = 0; iEnt < events; iEnt++){
    H4tree->GetEntry(iEnt);
    for(int jGrp = 0; jGrp < 36; jGrp++){
      if(grps[jGrp*1024] != grp && grp != -1) continue;
      min = vals[0];
      max = min;
      for(int kVal = 0; kVal < 1024; kVal++){
	if(chs[1024*jGrp+kVal] != ch && ch != -1) break;
	if(vals[1024*jGrp+kVal] > max) max = vals[1024*jGrp+kVal];
	else if(vals[1024*jGrp+kVal] < min) min = vals[1024*jGrp+kVal];
	x[kVal] = kVal;
	y[kVal] = vals[1024*jGrp+kVal];
      }
    
      if(max - min < 100){
	TGraph *gr = new TGraph(1024, x, y);
	rms = gr->GetRMS(2);
	h->Fill(rms);
      }
    }
  }

  float r = h->GetRMS();
  float mean = h->GetMean();
  min = mean - 3*r;
  max = mean + 3*r;
  
  h->SetAxisRange(min, max, "X");
  
  TString filename;
  filename.Form("./5387/1/rms_%d_%d.root", grp, ch);

  h->SaveAs(filename.Data());
}