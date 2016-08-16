#include <math.h>

void RMS(Int_t grp = -1, Int_t ch = -1){
  TTree *H4tree = (TTree*) _file0->Get("H4tree");
  
  Int_t events = H4tree->GetEntries();
  Int_t channels = H4tree->GetMaximum("digiChannel");
  Int_t groups = H4tree->GetMaximum("digiGroup");
  Int_t values = events*channels*groups*1024;
  
  Float_t vals[36864];
  UInt_t grps[36864];
  UInt_t chs[36864];
  Float_t x[1024], y[1024];
  
  H4tree->SetBranchAddress("digiSampleValue", vals);
  H4tree->SetBranchAddress("digiGroup", grps);
  H4tree->SetBranchAddress("digiChannel", chs);
  
  //TString title;
  //title.Form("RMS Distribution for Channel %i", ch);
  
  TH1F h("h", ""/*title.Data()*/, sqrt(values/1024), 0, 20);
  
  Float_t rms, max = 0, min;
  
  for(int i = 0; i < events; i++){
    H4tree->GetEntry(i);
    for(int j = 0; j < 36; j++){
      NEXT:if(grps[j*1024] != grp && grp != -1) continue;
      min = vals[0];
      for(int k = 0; k < 1024; k++){
	if(chs[1024*j+k] != ch && ch != -1){
	  j++;
	  goto NEXT;
	}
	if(vals[1024*j+k] > max) max = vals[1024*j+k];
	x[k] = k;
	y[k] = vals[1024*j+k];
      }
    
      if(max - min < 100){
	gr = new TGraph(1024, x, y);
	rms = gr->GetRMS(2);
	h.Fill(rms);
      }
      max = 0;
    }
    cout<<i<<endl;
  }

  float r = h.GetRMS();
  float mean = h.GetMean();
  min = mean - 3*r;
  max = mean + 3*r;
  
  h.SetAxisRange(min, max, "X");
  
  TCanvas *canvas = new TCanvas("c", "c", 600, 600);
  
  h.Draw();
  
  c->SaveAs("./test.root");
}
