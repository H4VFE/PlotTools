{ //Testing with run 5196

  /*
  The purpose of this macro is to plot an event of a Russian Xtal from a TB data file. 
  Currently requires hard coding of run number. 

  Abe Tishelman-Charny
 
  Useful Links: 
  https://root-forum.cern.ch/t/finding-x-y-coordinates-to-max-value-in-2d-histogram/20915

  */ 

  TFile *file = TFile::Open("Data_Files/analysis_5196.root"); // Open data file.

  TCanvas *c1 = new TCanvas("c1","Amplitude vs Time",200,10,700,500); // Create Canvas
  TH2F *h = new TH2F("h", "Amplitude vs. Time, Russian Crystal", 100, 0, 100, 750, 0, 750); // bins, min, max, first for x then y as last 6 arguments.              
  h4->Draw("WF_val:WF_time >> h", "WF_ch == XTAL_C3", "colz");
  //h->GetMaximum(double a);
  //h->GetXaxis()->SetRange(0,a);
  //h->GetBinXYZ(h->GetMaximumBin(), x, y, z); // sets x as x bin number with max entries, same for y and z
  //h->GetXaxis()->SetRange(0,x);	
  h->GetXaxis()->SetTitle("Time");
  h->GetYaxis()->SetTitle("Amplitude");
 
  TCanvas *c2 = new TCanvas("c2","Amplitude Entries vs. Spill Number",200,10,700,500);
  TH2F *h2 = new TH2F("h2", "Amplitude Entires vs. Spill Number", 50, 0, 50, 1000, 0, 1000);
  h4->Draw("WF_val:spill >> h2", "WF_ch == XTAL_C3","colz");
  h2->GetXaxis()->SetTitle("Spill");
  h2->GetYaxis()->SetTitle("Amplitude Entries");
  Int_t x, y, z;
  h2->GetBinXYZ(h2->GetMaximumBin(), x, y, z); // sets x as x bin number with max entries, same for y and z
   
  TCanvas *c3 = new TCanvas("c3","Max Spill",200,10,700,500);
  TH2F *h3 = new TH2F("h3", "Max Spill", 50, 0, 50, 1000, 0, 1000);
  //printf("Max bin %d,%d\n",x,y);
  TString cut; 
  cut.Form("WF_ch == XTAL_C3 && spill == %d",x); // Formats a string using a printf style format descriptor.
  h4->Draw("WF_val:WF_time >> h3", cut.Data(), "colz");
  Int_t x2, y2, z2;
  h3->Reset();
  h4->Draw("WF_val:event >> h3", "WF_ch == XTAL_C3", "colz");
  h3->GetBinXYZ(h3->GetMaximumBin(), x2, y2, z2);
  printf("Max bin events(%d,%d)\n",x2,y2);
  h3->Reset();
  cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d", x, x2); // Formats a string using a printf style format descriptor.
  h4->Draw("WF_val:WF_time >> h3", cut.Data(), "colz");
  h3->GetXaxis()->SetTitle("Time");
  h3->GetYaxis()->SetTitle("Amplitude");

  

  // temphist->Reset();
  // h4->Draw(..newDraw..);


  // Saving code below just so it's not lost. ---------------------------------------------------- 
  


  // Want spill number with greatest WF_val, cut plot to this spill. Then want event number with greatest WF_val, cur plot to this event.


  //Int_t max = amp_max[XTAL_C3];
  //printf("Max y value = %d\n",max);
  //Int_t x, y, z, x2, y2, z2;
  //hist->GetBinXYZ(hist->GetMaximumBin(), x, y, z); // sets x as x bin number with max entries, same for y and z
  //printf("x and y bin maximum: (%d,%d)",x,y);
  
  //TCanvas *c2 = new TCanvas("c2", "Graph", 200, 10, 700, 500);
  //TGraph *gr = new TGraph(hist);
  //gr->Draw();
  
  //hist->Reset();
  //TString cut; 
  //cut.Form("WF_ch == XTAL_C3 && spill == %d",x); // Formats a string using a printf style format descriptor.
  //h4->Draw("WF_val:event >> hist", cut.Data(), "colz");
  //hist->GetBinXYZ(hist->GetMaximumBin(), x2, y2, z2);
  //printf("Even x and y bin maximum: (%d,%d)",x2,y2);
  //hist->Reset();
  //TString cut2;
  //cut2.Form("WF_ch == XTAL_C3 && spill == %d && event == %d", x, x2);
  //h4->Draw("WF_val:WF_time >> hist", cut2.Data(), "colz");
  //hist->GetXaxis()->SetTitle("Time");
  //hist->GetYaxis()->SetTitle("Amplitude");


/*
  TCanvas *c3 = new TCanvas("c3","Max Spill",200,30,500,500);
  TH2F *h3 = new TH2F("h3", "Max Spill", 50, 0, 50, 1000, 0, 1000);
  //printf("Max bin %d,%d\n",x,y);
  TString cut; 
  cut.Form("WF_ch == XTAL_C3 && spill == %d",x); // Formats a string using a printf style format descriptor.
  h4->Draw("WF_val:WF_time >> h3", cut.Data(), "colz");
  Int_t x2, y2, z2;
  h3->Reset();
  h4->Draw("WF_val:event >> h3", "WF_ch == XTAL_C3", "colz");
  h3->GetBinXYZ(h3->GetMaximumBin(), x2, y2, z2);
  printf("Max bin events(%d,%d)\n",x2,y2);
  h3->Reset();
  cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d", x, x2); // Formats a string using a printf style format descriptor.
  h4->Draw("WF_val:WF_time >> h3", cut.Data(), "colz");
  h3->GetXaxis()->SetTitle("Time");
  h3->GetYaxis()->SetTitle("Amplitude");
*/


}
