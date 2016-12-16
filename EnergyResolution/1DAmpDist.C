/*

This macro creates 1D distributions of the maximum amplitudes
for 5 runs that cover crystal C0. Cuts on the hodoscope restrict
the beam away from the edges of the crystal and the hodoscope.

Creates files for use by FindCutoffs.C

Will Benoit
December 8th 2016

*/

{
  
  TFile *f, *dists;
  dists = new TFile("1DAmpDistributions.root", "RECREATE"); //File to store histograms
  TTree *h4;
  TFile *g_file = new TFile("dummy.root", "RECREATE"); //Dummy file to make things work
  TH1F *hist = new TH1F("hist", "Amplitude Distribution", 50, 0, 900); //Histogram
  
  //Each paragraph does the following: opens a root file, gets the TTree, fills the histogram with a certain cut, and writes the histogram to file. This is done for each APD
  f = TFile::Open("ntuples_v1/analysis_5305.root");
  h4 = (TTree*) f->Get("h4");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD1] >> hist", "X[0] > -5 && X[0] < 5 && X[1] > -2.5 && X[1] < 7.5 && Y[0] > -5 && Y[0] < 5 && Y[1] > -5 && Y[1] < 3");
  dists->cd();
  hist->Write("Center 1 - 5305");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD2] >> hist", "X[0] > -5 && X[0] < 5 && X[1] > -2.5 && X[1] < 7.5 && Y[0] > -5 && Y[0] < 5 && Y[1] > -5 && Y[1] < 3");
  dists->cd();
  hist->Write("Center 2 - 5305");
  
  f = TFile::Open("ntuples_v1/analysis_5270.root");
  h4 = (TTree*) f->Get("h4");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD1] >> hist", "X[0] > -5 && X[0] < 5 && X[1] > -2.5 && X[1] < 7.5 && Y[0] > -5 && Y[0] < 5 && Y[1] > -5 && Y[1] < 5");
  dists->cd();
  hist->Write("North 1 - 5270");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD2] >> hist", "X[0] > -5 && X[0] < 5 && X[1] > -2.5 && X[1] < 7.5 && Y[0] > -5 && Y[0] < 5 && Y[1] > -5 && Y[1] < 5");
  dists->cd();
  hist->Write("North 2 - 5270");
  
  f = TFile::Open("ntuples_v1/analysis_5307.root");
  h4 = (TTree*) f->Get("h4");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD1] >> hist", "X[0] > -5 && X[0] < 5 && X[1] > -2.5 && X[1] < 7.5 && Y[0] > -5 && Y[0] < 5 && Y[1] > -5 && Y[1] < 3");
  dists->cd();
  hist->Write("East 1 - 5307");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD2] >> hist", "X[0] > -5 && X[0] < 5 && X[1] > -2.5 && X[1] < 7.5 && Y[0] > -5 && Y[0] < 5 && Y[1] > -5 && Y[1] < 3");
  dists->cd();
  hist->Write("East 2 - 5307");
  
  f = TFile::Open("ntuples_v1/analysis_5308.root");
  h4 = (TTree*) f->Get("h4");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD1] >> hist", "X[0] > -5 && X[0] < 5 && X[1] > -2.5 && X[1] < 7.5 && Y[0] > -10 && Y[0] < -4 && Y[1] > -10 && Y[1] < -4");
  dists->cd();
  hist->Write("South 1 - 5308");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD2] >> hist", "X[0] > -5 && X[0] < 5 && X[1] > -2.5 && X[1] < 7.5 && Y[0] > -10 && Y[0] < -4 && Y[1] > -10 && Y[1] < -4");
  dists->cd();
  hist->Write("South 2 - 5308");
  
  f = TFile::Open("ntuples_v1/analysis_5309.root");
  h4 = (TTree*) f->Get("h4");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD1] >> hist", "X[0] > 0 && X[0] < 8 && X[1] > 2.5 && X[1] < 10.5 && Y[0] > -5 && Y[0] < 5 && Y[1] > -5 && Y[1] < 3");
  dists->cd();
  hist->Write("West 1 - 5309");
  g_file->cd();
  h4->Draw("amp_max[XTAL_C0_APD2] >> hist", "X[0] > 0 && X[0] < 8 && X[1] > 2.5 && X[1] < 10.5 && Y[0] > -5 && Y[0] < 5 && Y[1] > -5 && Y[1] < 3");
  dists->cd();
  hist->Write("West 2 - 5309");
  
}