/*

Uses the files and plots created by rmsDist.cpp to make a
2x3 canvas comparing a warm run and a cold run. 

The upper-left plot has the RMS distributions
of both runs overlaid. Top-middle and top-right are the full
signals from each run for the specific channel. Bottom-left
has both pulses overlaid. The bottom-middle and bottom-right
are the sections of the pulse used for the histograms, and
correspond to the plot above them.

Will Benoit
December 7th 2016

*/

#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TMultiGraph.h"

void compare(int grp, int ch, char* run1, char* run2);

int main(int argc, char *argv[]){ //Takes two runs for comparison. The colder run must be given first, as there is nothing in the root file to indicate the temperature automatically
  for(int grp = 0; grp < 4; grp++){ //Do the comparison for every channel from the 2 given runs.
    for(int ch = 0; ch < 9; ch++){
      compare(grp, ch, argv[1], argv[2]);
    }
  }
  return 1;
}

void compare(int grp, int ch, char* run1, char* run2){
  
  //Opening the files created by rmsDist.cpp
  TString filename1;
  filename1.Form("./%s/1/rms_%d_%d.root", run1, grp, ch);
  
  TString filename2;
  filename2.Form("./%s/1/rms_%d_%d.root", run2, grp, ch);
  
  TFile *f1 = TFile::Open(filename1.Data());
  TFile *f2 = TFile::Open(filename2.Data());

  //Getting the objects from each file
  TH1F* h1 = (TH1F*) f1->Get("h");
  TH1F* h2 = (TH1F*) f2->Get("h");
  
  TH2F* pulse1 = (TH2F*) f1->Get("pulse");
  TH2F* pulse2 = (TH2F*) f2->Get("pulse");
  
  TMultiGraph* mg1 = (TMultiGraph*) f1->Get("mg");
  TMultiGraph* mg2 = (TMultiGraph*) f2->Get("mg");
  
  //Making things nice...
  h1->SetAxisRange(0, 14, "X");
  h2->SetLineColor(kRed);
  
  h1->SetStats(0);
  pulse1->SetStats(0);
  pulse2->SetStats(0);
  
  //Canvas creation and division
  TCanvas *c = new TCanvas("c", "c", 2400, 2400);
  c->Divide(3,2);
  
  //Drawing everything!
  c->cd(1);
  h1->Draw();
  h2->Draw("same");
  
  TLegend* leg = new TLegend(0.7,0.8,0.9,0.9);
  leg->AddEntry(h1,"Colder","l");
  leg->AddEntry(h2,"Warmer","l");
  leg->Draw();
  
  c->cd(2);
  pulse1->SetMarkerColor(kBlue); //There's not much point in changing color if you use the colz option, but it affects what happens in pad 4
  pulse1->SetTitle("Colder");
  pulse1->DrawClone("colz");
  
  c->cd(3);
  pulse2->SetMarkerColor(kRed); //Same here
  pulse2->SetTitle("Warmer");
  pulse2->Draw("colz");
  
  c->cd(4);
  pulse1->SetTitle("Colder vs Warmer");
  pulse1->Draw("p");
  pulse2->Draw("p same");
  
  c->cd(5);
  mg1->Draw("AP");
  
  c->cd(6);
  mg2->Draw("AP");
  
  TString canvasName;
  
  //I wanted the smaller run number to be listed first, so if I was searching I knew what to look for. Both png and root files are created.
  if(atoi(run1) < atoi(run2)) canvasName.Form("./%svs%s/cmp_%d_%d.png", run1, run2, grp, ch);
  else canvasName.Form("./%svs%s/cmp_%d_%d.png", run2, run1, grp, ch);
  c->SaveAs(canvasName.Data());
  
  if(atoi(run1) < atoi(run2)) canvasName.Form("./%svs%s/cmp_%d_%d.root", run1, run2, grp, ch);
  else canvasName.Form("./%svs%s/cmp_%d_%d.root", run2, run1, grp, ch);
  c->SaveAs(canvasName.Data());
}