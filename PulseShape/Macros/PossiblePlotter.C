#include <vector>

{

  TFile *file = new TFile("Data_Files/analysis_5196.root"); // Open data file.
  TTree *tree = (TTree*) file->Get("h4"); 

  //vector<float> *WF_val;
  vector<int> *WF_ch;

  tree->SetBranchAddress("WF_val", &WF_val);
  tree->SetBranchAddress("WF_ch", &WF_ch);

  // tree->GetEntry(nEvent);
  
  TCanvas *c1 = new TCanvas("c1","A Simple Graph Example",200,10,700,500);
  
  TGraph *gr = new TGraph();
     for(int i=0; i<WF_ch->size(); i++){

        gr->SetPoint(i,WF_ch->at(i),WF_val->at(i));
  
        }
  
  gr->Draw(); //A=Axis, L=Line graph
  gr->SetTitle("Title");
  //gr->SetLineColor(kMagenta);
  //gr->SetLineWidth);
  gr->GetXaxis()->SetTitle("X axis title");
  gr->GetYaxis()->SetTitle("Y axis title");
  
 }
