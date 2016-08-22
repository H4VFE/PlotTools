#include "TTree.h"
#include <cmath>
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH2F.h"
#include "TMultiGraph.h"

int start = 350, end = 900;
int count = 0;

Float_t vals[36864];
UInt_t grps[36864];
UInt_t chs[36864];
Float_t x[550], y[550];

void RMS(Int_t grp = -1, Int_t ch = -1, Int_t events = 0, TTree* H4tree = 0);
TString channelMap(Int_t grp, Int_t ch, TTree* H4tree);

int main(int argc, char *argv[]){
  char* runNum = argv[1];
  
  TString t;
  t.Form("%s/1.root", runNum);
  
  TFile *file = TFile::Open(t.Data());
  TTree *H4tree = (TTree*) file->Get("H4tree");

  Int_t events = H4tree->GetEntries();
  Int_t channels = H4tree->GetMaximum("digiChannel");
  Int_t groups = H4tree->GetMaximum("digiGroup");
  
  H4tree->SetBranchAddress("digiSampleValue", vals);
  H4tree->SetBranchAddress("digiGroup", grps);
  H4tree->SetBranchAddress("digiChannel", chs);
  
  for(int grp = 0; grp <= groups ; grp++){
    for(int ch = 0; ch <= channels; ch++){
      RMS(grp, ch, events, H4tree);
    }
  }
  
  return 1;
}

void RMS(Int_t grp, Int_t ch, Int_t events, TTree* H4tree){
  TString title;
  title.Form("RMS Distribution for Group %d, Channel %d, ", grp, ch);
  
  TString channelLabel = channelMap(grp, ch, H4tree);
  
  title = title + channelLabel;
  
  TH1F *h = new TH1F("h", title.Data(), sqrt(26712), 0, 20);
  
  TMultiGraph *mg = new TMultiGraph("mg", "Events used in RMS Distribution");
  
  Float_t rms, max, min;
  
  for(int iEnt = 0; iEnt < events; iEnt++){
    H4tree->GetEntry(iEnt);
    for(int jGrp = 0; jGrp < 36; jGrp++){
      if(grps[jGrp*1024] != grp && grp != -1) continue;
      min = vals[0];
      max = 0;
      for(int kVal = start; kVal < end; kVal++){ //kVal can be changed to look at a section points, to easily exlude pulses
	if(chs[1024*jGrp+kVal] != ch && ch != -1) break;
	if(vals[1024*jGrp+kVal] > max) max = vals[1024*jGrp+kVal];
	else if(vals[1024*jGrp+kVal] < min) min = vals[1024*jGrp+kVal];
	x[kVal-start] = kVal;
	y[kVal-start] = vals[1024*jGrp+kVal];
      }
    
      if(max - min < 100 && max - min > 0){
	count++;
	TGraph *gr = new TGraph(end - start, x, y);
	if(count%10 == 0){
	  mg->Add(gr);
	  //std::cout << max << " " << min << std::endl;
	}
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
  
  Int_t run = (Int_t) H4tree->GetMaximum("runNumber");
  
  TString filename;
  filename.Form("./%d/1/rms_%d_%d.root", run, grp, ch);
  
  TFile fout(filename.Data(), "RECREATE");
  h->Write();
  
  TString plotName;
  plotName.Form("digiSampleValue:digiSampleIndex, digiGroup == %d && digiChannel == %d", grp, ch);
  
  TH2F* pulse = new TH2F("pulse", plotName.Data(), 1024, 0, 1024, 4096, 0, 4096);
  
  TString cut;
  cut.Form("digiGroup == %d && digiChannel == %d", grp, ch);
  H4tree->Draw("digiSampleValue:digiSampleIndex >> pulse", cut.Data());
  
  mg->Write();
  pulse->Write();
}

TString channelMap(Int_t grp, Int_t ch, TTree* H4tree){
  Int_t channel = 8*grp + ch;
  Float_t run = H4tree->GetMaximum("runNumber");
  
  if(run < 5400){
    switch(channel){
      case 0:
	return "E5, APD";
      case 1:
	return "E4, APD";
      case 2:
	return "Not Used";
      case 3:
	return "Not Used";
      case 4:
	return "E2, APD, Low Ampl";
      case 5:
	return "E1, APD";
      case 6:
	return "Not Used";
      case 7:
	return "Not Used";
      case 8:
	return "A2, APD";
      case 9:
	return "Not Used";
      case 10:
	return "Not Used";
      case 11:
	return "Not Used";
      case 12:
	return "D5, APD";
      case 13:
	return "Not Used";
      case 14:
	return "Not Used";
      case 15:
	return "C1, APD";
      case 16:
	return "MCP";
      case 17:
	return "C2, APD";
      case 18:
	return "C3, APD";
      case 19:
	return "C4, APD";
      case 20:
	return "D3, APD";
      case 21:
	return "A0, Nano, SiPM";
      case 22:
	return "A0, Analog, SiPM";
      case 23:
	return "B2, APD, Low Ampl";
      case 24:
	return "D4, APD";
      case 25:
	return "D2, APD";
      case 26:
	return "Not Used";
      case 27:
	return "Not Used";
      case 28:
	return "Not Used";
      case 29:
	return "D0, APD, Irradiated 2";
      case 30:
	return "C0, Double APD";
      case 31:
	return "C0, Double APD";
      default:
	return "";
    }
    
  }
  else if(run < 5602){
    switch(channel){
      case 0:
	return "E5, APD";
      case 1:
	return "E4, APD";
      case 2:
	return "Not Used";
      case 3:
	return "B3, APD";
      case 4:
	return "E2, APD, Low Ampl";
      case 5:
	return "E1, APD";
      case 6:
	return "Not Used";
      case 7:
	return "Not Used";
      case 8:
	return "A1, APD";
      case 9:
	return "A2, APD";
      case 10:
	return "A4, APD";
      case 11:
	return "A5, APD";
      case 12:
	return "D5, APD";
      case 13:
	return "Not Used";
      case 14:
	return "C5, APD";
      case 15:
	return "C1, APD";
      case 16:
	return "MCP";
      case 17:
	return "C2, APD";
      case 18:
	return "C3, APD";
      case 19:
	return "C4, APD";
      case 20:
	return "D3, APD";
      case 21:
	return "A0, Nano, SiPM";
      case 22:
	return "A0, Analog, SiPM";
      case 23:
	return "B2, APD, Low Ampl";
      case 24:
	return "D4, APD";
      case 25:
	return "D2, APD";
      case 26:
	return "Not Used";
      case 27:
	return "B1, APD";
      case 28:
	return "Not Used";
      case 29:
	return "D0, APD, Irradiated 2";
      case 30:
	return "C0, Double APD";
      case 31:
	return "C0, Double APD";
      default:
	return "";
    }
  }
  else{
    switch(channel){
      case 0:
	return "E5, APD";
      case 1:
	return "E4, APD";
      case 2:
	return "Not Used";
      case 3:
	return "B3, APD";
      case 4:
	return "Not Used";
      case 5:
	return "Not Used";
      case 6:
	return "Not Used";
      case 7:
	return "Not Used";
      case 8:
	return "A1, APD";
      case 9:
	return "A2, APD";
      case 10:
	return "A4, APD";
      case 11:
	return "A5, APD";
      case 12:
	return "D5, APD";
      case 13:
	return "Not Used";
      case 14:
	return "C5, APD";
      case 15:
	return "C1, APD";
      case 16:
	return "MCP";
      case 17:
	return "C2, APD";
      case 18:
	return "C3, APD";
      case 19:
	return "C4, APD";
      case 20:
	return "D3, APD";
      case 21:
	return "A0, Nano, SiPM";
      case 22:
	return "C0, Double APD";
      case 23:
	return "C0, Double APD";
      case 24:
	return "D4, APD";
      case 25:
	return "D2, APD";
      case 26:
	return "Not Used";
      case 27:
	return "B1, APD";
      case 28:
	return "B0, Spike";
      case 29:
	return "E0, APD, Irradiated 1";
      case 30:
	return "A0, Analog, SiPM";
      case 31:
	return "Not Used";
      default:
	return "";
    }
  }
}