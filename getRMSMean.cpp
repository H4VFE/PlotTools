/*

Entirely identical to rmsDist.cpp, except it writes the mean of each
RMS distribution to a text file, for later use by graphAll.cpp

A script was used to run this program from run 5200 to 5601

*/

#include "TTree.h"
#include <cmath>
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH2F.h"
#include "TMultiGraph.h"
#include <iostream>
#include <fstream>

int start = 350, end = 900;
int count = 0;

float vals[36864];
uint grps[36864];
uint chs[36864];
float x[550], y[550];

void RMS(int grp = -1, int ch = -1, int events = 0, TTree* H4tree = 0, TH1F* h = 0);
TString channelMap(int grp, int ch, TTree* H4tree);

int main(int argc, char *argv[]){
  char* runNum = argv[1];
  
  TString t;
  t.Form("./eos/cms/store/group/dpg_ecal/comm_ecal/upgrade/testbeam/ECALTB_H4_Jul2016/raw/DataTree/%s/1.root", runNum);
  
  TFile *file = TFile::Open(t.Data());
  TTree *H4tree = (TTree*) file->Get("H4tree");

  int events = H4tree->GetEntries();
  int channels = H4tree->GetMaximum("digiChannel");
  int groups = H4tree->GetMaximum("digiGroup");
  
  H4tree->SetBranchAddress("digiSampleValue", vals);
  H4tree->SetBranchAddress("digiGroup", grps);
  H4tree->SetBranchAddress("digiChannel", chs);
  
  TH1F *h = new TH1F("h", "", sqrt(26712), 0, 20);
  
  for(int grp = 0; grp <= groups ; grp++){
    for(int ch = 0; ch <= channels; ch++){
      RMS(grp, ch, events, H4tree, h);
    }
  }
  
  return 1;
}

void RMS(int grp, int ch, int events, TTree* H4tree, TH1F* h){
  h->Reset();
  
  h->SetAxisRange(0, 20, "X");
  
  TString title;
  title.Form("RMS Distribution for Group %d, Channel %d, ", grp, ch);
  
  TString channelLabel = channelMap(grp, ch, H4tree);
  
  title = title + channelLabel;
  
  h->SetTitle(title.Data());
  
  TMultiGraph *mg = new TMultiGraph("mg", "Events used in RMS Distribution");
  
  float rms, max, min;
  
  for(int iEnt = 0; iEnt < events; iEnt++){
    H4tree->GetEntry(iEnt);
    for(int jGrp = 0; jGrp < 36; jGrp++){
      if(grps[jGrp*1024] != grp && grp != -1) continue;
      min = vals[0];
      max = 0;
      for(int kVal = start; kVal < end; kVal++){ //kVal can be changed to look at a section, to easily exlude pulses
	if(chs[1024*jGrp+kVal] != ch && ch != -1) break;
	if(vals[1024*jGrp+kVal] > max) max = vals[1024*jGrp+kVal];
	else if(vals[1024*jGrp+kVal] < min) min = vals[1024*jGrp+kVal];
	x[kVal-start] = kVal;
	y[kVal-start] = vals[1024*jGrp+kVal];
      }
    
      if(max - min < 50 && max - min > 0){
	TGraph *gr = new TGraph(end - start, x, y);
	if(count%10 == 0) mg->Add(gr);
	count++;
	rms = gr->GetRMS(2);
	h->Fill(rms);
      }
    }
    
    if(iEnt == events -1) std::cout<<grp<<" "<<ch<<std::endl;
  }

  //This is all that's different
  float mean = h->GetMean();
  
  std::ofstream myfile;
  myfile.open("./work/PlotTools/RMS Means.txt", std::ios::app);
  myfile << mean << std::endl;
  myfile.close();
  
  
}

TString channelMap(int grp, int ch, TTree* H4tree){
  int channel = 8*grp + ch;
  float run = H4tree->GetMaximum("runNumber");
  
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