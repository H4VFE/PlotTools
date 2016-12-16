/*

Plots the average RMS vs the run number for a selection of runs.
Not really done in an efficient way, but it was good enough for
what I needed.

Will Benoit
December 6th 2016

*/

#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"

void makeGraph(int grp, int ch);

int main(){
  //Loop through all groups and channels
  for(int grp = 0; grp < 4 ; grp++){
    for(int ch = 0; ch < 9; ch++){
      makeGraph(grp, ch);
    }
  }
  
  return 1;
}

void makeGraph(int grp, int ch){
  
  //For each file, it takes the RMS histogram and extracts the mean
  
  TString file;
  file.Form("/1/rms_%d_%d.root", grp, ch);
  
  TString temp;
  
  temp = "5200" + file;
  TFile *f0 = TFile::Open(temp.Data());
  TH1F* h0 = (TH1F*) f0->Get("h");
  
  temp = "5218" + file;
  TFile *f1 = TFile::Open(temp.Data());
  TH1F* h1 = (TH1F*) f1->Get("h");
  
  temp = "5240" + file;
  TFile *f2 = TFile::Open(temp.Data());
  TH1F* h2 = (TH1F*) f2->Get("h");
  
  temp = "5245" + file;
  TFile *f3 = TFile::Open(temp.Data());
  TH1F* h3 = (TH1F*) f3->Get("h");
  
  temp = "5297" + file;
  TFile *f4 = TFile::Open(temp.Data());
  TH1F* h4 = (TH1F*) f4->Get("h");
  
  temp = "5387" + file;
  TFile *f5 = TFile::Open(temp.Data());
  TH1F* h5 = (TH1F*) f5->Get("h");
  
  temp = "5400" + file;
  TFile *f6 = TFile::Open(temp.Data());
  TH1F* h6 = (TH1F*) f6->Get("h");
  
  temp = "5407" + file;
  TFile *f7 = TFile::Open(temp.Data());
  TH1F* h7 = (TH1F*) f7->Get("h");
  
  temp = "5415" + file;
  TFile *f8 = TFile::Open(temp.Data());
  TH1F* h8 = (TH1F*) f8->Get("h");
  
  temp = "5419" + file;
  TFile *f9 = TFile::Open(temp.Data());
  TH1F* h9 = (TH1F*) f9->Get("h");
  
  temp = "5448" + file;
  TFile *f10 = TFile::Open(temp.Data());
  TH1F* h10 = (TH1F*) f10->Get("h");
  
  temp = "5471" + file;
  TFile *f11 = TFile::Open(temp.Data());
  TH1F* h11 = (TH1F*) f11->Get("h");
  
  temp = "5494" + file;
  TFile *f12 = TFile::Open(temp.Data());
  TH1F* h12 = (TH1F*) f12->Get("h");
  
  temp = "5521" + file;
  TFile *f13 = TFile::Open(temp.Data());
  TH1F* h13 = (TH1F*) f13->Get("h");
  
  temp = "5526" + file;
  TFile *f14 = TFile::Open(temp.Data());
  TH1F* h14 = (TH1F*) f14->Get("h");
  
  temp = "5527" + file;
  TFile *f15 = TFile::Open(temp.Data());
  TH1F* h15 = (TH1F*) f15->Get("h");
  
  //Taking the mean and filling the arrays
  const int n = 16;
  double x[n] = {5200, 5218, 5240, 5245, 5297, 5387, 5400, 5407, 5415, 5419, 5448, 5471, 5494, 5521, 5526, 5527};
  double y[n] = {h0->GetMean(), h1->GetMean(), h2->GetMean(), h3->GetMean(), h4->GetMean(), h5->GetMean(), h6->GetMean(), h7->GetMean(), 
		 h8->GetMean(), h9->GetMean(), h10->GetMean(), h11->GetMean(), h12->GetMean(), h13->GetMean(), h14->GetMean(), h15->GetMean()};

  TCanvas *c = new TCanvas("c", "c", 600, 600);

  //Making the graph
  TGraph* gr = new TGraph(n,x,y);
  gr->SetMarkerStyle(21);
  
  int channel = grp*8+ch;
  
  //Title based on channel organization within a group
  if(ch != 8) temp.Form("Mean RMS of Channel %d Before and After Run 5400", channel);
  else temp.Form("Mean RMS of Trigger Channel %d Before and After Run 5400", grp);
  
  gr->SetTitle(temp.Data());
  gr->Draw("AP");
  
  temp.Form("./Run Comparison/%d_%d.png", grp, ch);
  c->SaveAs(temp.Data());
}