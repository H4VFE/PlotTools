/*

This macro creates two plots to show the beam position
on crystals C0, D0, and C1

Note that this was used as a rough check, and should not
be used for anything more than that

Will Benoit
December 8th 2016

*/

{
  TChain* chain = new TChain("h4");
  
  chain->Add("analysis_5494.root"); //Adding D0 run
  chain->Add("analysis_5661.root"); //Adding C0 run
  chain->Add("analysis_5534.root"); //Adding C1 run
  
  TString cut;
  float C0cutoff, D0cutoff, C1cutoff; //Cutoff values for the amplitude to exclude leakage events
  TH1F *h = new TH1F("h", "h", 1000, -100, 5000);
  
  chain->Draw("fit_ampl[XTAL_C0_APD1] >> h", "run == 5661");
  C0cutoff = h->GetMean();0; //Cutoffs are calculated very roughly
  h->Reset();
  chain->Draw("fit_ampl[XTAL_D0] >> h", "run == 5494");
  D0cutoff = h->GetMean();0;
  h->Reset();
  chain->Draw("fit_ampl[XTAL_C1] >> h", "run == 5534");
  C1cutoff = h->GetMean();0;
  
  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  TH2F *hframe = new TH2F("hframe", "", 100, -10, 40, 100, -40, 10);
  hframe->Draw();
  
  //Drawing X vs Y for each run into the same histogram
  cut.Form("run == 5661 && X > -200 && Y > -200 && (fit_ampl[XTAL_C0_APD1] > %f  || fit_ampl[XTAL_D0] > %f || fit_ampl[XTAL_C1] > %f)", C0cutoff, D0cutoff, C1cutoff);
  chain->Draw("(-Y/2):X/2 >> hframe", cut.Data());
  
  cut.Form("run == 5494 && X > -200 && Y > -200 && (fit_ampl[XTAL_C0_APD1] > %f  || fit_ampl[XTAL_D0] > %f || fit_ampl[XTAL_C1] > %f)", C0cutoff, D0cutoff, C1cutoff);
  chain->Draw("(-(Y/2)):(X+50)/2 >>+ hframe", cut.Data());
  
  cut.Form("run == 5534 && X > -200 && Y > -200 && (fit_ampl[XTAL_C0_APD1] > %f  || fit_ampl[XTAL_D0] > %f || fit_ampl[XTAL_C1] > %f)", C0cutoff, D0cutoff, C1cutoff);
  chain->Draw("(-(Y+50))/2:X/2 >>+ hframe", cut.Data());
  
  hframe->Draw("colz");
  
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  //Drawing amplitude vs X vs Y
  TCanvas *c2 = new TCanvas("c2", "c2", 600, 600);
//   TH2F *hframe2 = new TH2F("hframe2", "", 100, -10, 40, 100, -40, 10);
  chain->Draw("(amp_max[XTAL_C0_APD1] + amp_max[XTAL_D0] + amp_max[XTAL_C1]):((run == 5661)*(-Y) + (run == 5494)*(-Y) + (run == 5534)*(-(Y+50)))/2:((run == 5494)*(X+50) + (run == 5661)*(X) + (run == 5534)*(X))/2", "X > -200 && Y > -200", "prof colz");

  
}
