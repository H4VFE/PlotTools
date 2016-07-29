{
#include <math.h>

TTree *H4tree = (TTree*) _file0->Get("H4tree");
int n = H4tree->GetEntries();
float rms, max = 0, min;

float vals[36864]; //1024*groups*channels
float x[1024], y[1024];

H4tree->SetBranchAddress("digiSampleValue", vals);

//float rms[n]; Future use

TH1F h("h", "RMS Distribution", sqrt(26712), 0, 20);

for(int i = 0; i < n; i++){
  H4tree->GetEntry(i);
  
  for(int j = 0; j < 36; j++){
    min = vals[0];
    for(int k = 0; k < 1024; k++){
      if(vals[1024*j+k] < min) min = vals[1024*j+k];
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
}

float r = h.GetRMS();
float mean = h.GetMean();
min = mean - 3*r;
max = mean + 3*r;

h.SetAxisRange(min, max, "X");

h.Draw();
}