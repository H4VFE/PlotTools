{
  TChain *chain  = new TChain("h4");
  
  chain->Add("ntuples_v1/analysis_5651.root");
  chain->Add("ntuples_v1/analysis_5654.root");
  chain->Add("ntuples_v1/analysis_5655.root");
  chain->Add("ntuples_v1/analysis_5659.root");
  chain->Add("ntuples_v1/analysis_5661.root");
  
  TH2F *x = new TH2F("x", "x", 100, -8, 8, 100, .5, 2);
  TH2F *y = new TH2F("y", "y", 100, -8, 8, 100, .5, 2);
  
  TCanvas *c1 = new TCanvas("c1", "c1", 600, 600);
  TCanvas *c2 = new TCanvas("c2", "c2", 600, 600);
  
  c1->Divide(1,2);
  c2->Divide(1,2);

  TF1 *p1 = new TF1("p1", "[0]*(x-[1])*(x-[2]) + [3]", -4, 8);
  TF1 *p2 = new TF1("p2", "[0]*(x-[1])*(x-[2]) + [3]", -4, 8);
  TF1 *p3 = new TF1("p3", "[0]*(x-[1])*(x-[2]) + [3]", -4, 8);
  TF1 *p4 = new TF1("p4", "[0]*(x-[1])*(x-[2]) + [3]", -4, 8);
  
  p1->SetParameters(-.01, 0, 0, 1);
  p2->SetParameters(-.01, 0, 0, 1);
  p3->SetParameters(-.01, 0, 0, 1);
  p4->SetParameters(-.01, 0, 0, 1);
  
  //////////////////////////////////////////// Drawing X with fit_ampl ////////////////////////////////////////////
  
  c1->cd(1);
  
  chain->Draw("(fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]):(X[0]/2) >> x", "X[0] > -200 && X[1] > -200 && abs(Y[0]) < 5 && abs(Y[1]) < 5 && (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) < 2"
		"&& (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) > 0.5", "colz");

  chain->Draw("(fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]):(X[0]/2)", "X[0] > -200 && X[1] > -200 && abs(Y[0]) < 5 && abs(Y[1]) < 5 && (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) < 2"
		"&& (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) > 0.5", "prof same");

  chain->Fit("p1", "(fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]):(X[0]/2)", "X[0] > -200 && X[1] > -200 && abs(Y[0]) < 5 && abs(Y[1]) < 5 && (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) < 2"
		"&& (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) > 0.5", "prof same");

  //////////////////////////////////////////// Drawing Y with fit_ampl ////////////////////////////////////////////
  
  c1->cd(2);
  
  chain->Draw("(fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]):(Y[0]/2) >> y", "Y[0] > -200 && Y[1] > -200 && abs(X[0]) < 5 && abs(X[1]) < 5 && (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) < 2"
		"&& (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) > 0.5", "colz");

  chain->Draw("(fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]):(Y[0]/2)", "Y[0] > -200 && Y[1] > -200 && abs(X[0]) < 5 && abs(X[1]) < 5 && (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) < 2"
		"&& (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) > 0.5", "prof same");

  chain->Fit("p2", "(fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]):(Y[0]/2)", "Y[0] > -200 && Y[1] > -200 && abs(X[0]) < 5 && abs(X[1]) < 5 && (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) < 2"
		"&& (fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]) > 0.5", "prof same");

  //////////////////////////////////////////// Drawing X with amp_max ////////////////////////////////////////////
  
  c2->cd(1);
  
  chain->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(X[0]/2) >> x", "X[0] > -200 && X[1] > -200 && abs(Y[0]) < 5 && abs(Y[1]) < 5 && (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) < 2"
		"&& (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) > 0.5", "colz");

  chain->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(X[0]/2)", "X[0] > -200 && X[1] > -200 && abs(Y[0]) < 5 && abs(Y[1]) < 5 && (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) < 2"
		"&& (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) > 0.5", "prof same");

  chain->Fit("p3", "(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(X[0]/2)", "X[0] > -200 && X[1] > -200 && abs(Y[0]) < 5 && abs(Y[1]) < 5 && (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) < 2"
		"&& (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) > 0.5", "prof same");
  
  //////////////////////////////////////////// Drawing Y with amp_max ////////////////////////////////////////////
  
  c2->cd(2);
  
  chain->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(Y[0]/2) >> y", "Y[0] > -200 && Y[1] > -200 && abs(X[0]) < 5 && abs(X[1]) < 5 && (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) < 2"
		"&& (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) > 0.5", "colz");

  chain->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(Y[0]/2)", "Y[0] > -200 && Y[1] > -200 && abs(X[0]) < 5 && abs(X[1]) < 5 && (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) < 2"
		"&& (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) > 0.5", "prof same");

  chain->Fit("p4", "(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(Y[0]/2)", "Y[0] > -200 && Y[1] > -200 && abs(X[0]) < 5 && abs(X[1]) < 5 && (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) < 2"
		"&& (amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]) > 0.5", "prof same");

  //////////////////////////////////////////////// Saving Canvases ////////////////////////////////////////////////
  
  TFile *file1 = new TFile("AmpRatios.root", "RECREATE");
  
  c1->Write();
  c2->Write();
}