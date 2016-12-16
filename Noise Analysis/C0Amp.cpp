#include "TTree.h"
#include <cmath>
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){
  
  char* runNum = argv[1];
  
  TString t;
  t.Form("eos/cms/store/group/dpg_ecal/comm_ecal/upgrade/testbeam/ECALTB_H4_Jul2016/raw/DataTree/%s/1.root", runNum);
  
  int count = 0;
  TFile *file = TFile::Open(t.Data());
  TTree *H4tree = (TTree*) file->Get("H4tree");
  
  float vals[36864];
  H4tree->SetBranchAddress("digiSampleValue", vals);

  int events = H4tree->GetEntries();

  float max1, max2, min1, min2, amp1, amp2, ampMax1, ampMax2;
  
  for(int iEvt = 0; iEvt < events; iEvt++){
    H4tree->GetEntry(iEvt);
    
    max1 = 0;
    max2 = 0;
    min1 = vals[1024*24];
    min2 = vals[1024*25];
    
    for(int pt = 1; pt < 900; pt++){
      if(vals[1024*24+pt] > max1) max1 = vals[1024*24+pt];
      else if(vals[1024*24+pt] < min1) min1 = vals[1024*24+pt];
      
      if(vals[1024*25+pt] > max2) max2 = vals[1024*25+pt];
      else if(vals[1024*25+pt] < min2) min2 = vals[1024*25+pt];
    }
    
    amp1 = max1 - min1;
    amp2 = max2 - min2;
    
    if(amp1 > ampMax1 && amp2 > ampMax2){ 
      ampMax1 = amp1;
      ampMax2 = amp2;
    }
    
    if(amp1 > 100 && amp2 > 100) count++;
  }
  
  ofstream myfile;
  myfile.open ("amps.txt", ios::app);
  myfile << runNum << " " << count << " " << ampMax1 << " " << ampMax2 << endl;
  myfile.close();
  
  return 1;
}