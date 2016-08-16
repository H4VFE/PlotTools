#include "TFile.h"
#include "TH1F.h"
#include "TCanvas.h"

void compare(Int_t grp, Int_t ch);

int main(){
  for(int grp = 0; grp < 4; grp++){
    for(int ch = 0; ch < 9; ch++){
      compare(grp, ch);
    }
  }
  return 1;
}

void compare(Int_t grp, Int_t ch){
  TString filename1;
  filename1.Form("./5526/1/rms_%d_%d.root", grp, ch);
  
  TString filename2;
  filename2.Form("./5297/1/rms_%d_%d.root", grp, ch);
  
  TFile *f1 = TFile::Open(filename1.Data());
  TFile *f2 = TFile::Open(filename2.Data());

  TH1F* h1 = (TH1F*) f1->Get("h");
  TH1F* h2 = (TH1F*) f2->Get("h");
  
  h1->SetAxisRange(5, 9, "X");
  h2->SetLineColor(kRed);

  TCanvas *c = new TCanvas("c", "c", 600, 600);
  
  h1->Draw();
  h2->Draw("same");
  
  TString canvasName;
  canvasName.Form("./5297vs5526/cmp_%d_%d.png", grp, ch);
  
  c->SaveAs(canvasName.Data());
}