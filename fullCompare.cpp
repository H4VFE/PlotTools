#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TLegend.h"
#include "TMultiGraph.h"

void compare(int grp, int ch, char* run1, char* run2);

int main(int argc, char *argv[]){
  for(int grp = 0; grp < 4; grp++){
    for(int ch = 0; ch < 9; ch++){
      compare(grp, ch, argv[1], argv[2]);
    }
  }
  return 1;
}

void compare(int grp, int ch, char* run1, char* run2){
  TString filename1;
  filename1.Form("./%s/1/rms_%d_%d.root", run1, grp, ch);
  
  TString filename2;
  filename2.Form("./%s/1/rms_%d_%d.root", run2, grp, ch);
  
  TFile *f1 = TFile::Open(filename1.Data());
  TFile *f2 = TFile::Open(filename2.Data());

  TH1F* h1 = (TH1F*) f1->Get("h");
  TH1F* h2 = (TH1F*) f2->Get("h");
  
  TH2F* pulse1 = (TH2F*) f1->Get("pulse");
  TH2F* pulse2 = (TH2F*) f2->Get("pulse");
  
  TMultiGraph* mg1 = (TMultiGraph*) f1->Get("mg");
  TMultiGraph* mg2 = (TMultiGraph*) f2->Get("mg");
  
  h1->SetAxisRange(0, 14, "X");
  h2->SetLineColor(kRed);
  
  h1->SetStats(0);
  pulse1->SetStats(0);
  pulse2->SetStats(0);
  
  TCanvas *c = new TCanvas("c", "c", 2400, 2400);
  c->Divide(3,2);
  
  c->cd(1);
  h1->Draw();
  h2->Draw("same");
  
  TLegend* leg = new TLegend(0.7,0.8,0.9,0.9);
  leg->AddEntry(h1,"Colder","l");
  leg->AddEntry(h2,"Warmer","l");
  leg->Draw();
  
  c->cd(2);
  pulse1->SetMarkerColor(kBlue);
  pulse1->SetTitle("Colder");
  pulse1->DrawClone("colz");
  
  c->cd(3);
  pulse2->SetMarkerColor(kRed);
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
  
  if(atoi(run1) < atoi(run2)) canvasName.Form("./%svs%s/cmp_%d_%d.png", run1, run2, grp, ch);
  else canvasName.Form("./%svs%s/cmp_%d_%d.png", run2, run1, grp, ch);
  c->SaveAs(canvasName.Data());
  
  if(atoi(run1) < atoi(run2)) canvasName.Form("./%svs%s/cmp_%d_%d.root", run1, run2, grp, ch);
  else canvasName.Form("./%svs%s/cmp_%d_%d.root", run2, run1, grp, ch);
  c->SaveAs(canvasName.Data());
}