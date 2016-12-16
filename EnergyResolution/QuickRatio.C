/*

This macro uses the position scan runs on crystal C0
to determine the bias in Y due to the APD asymmetry.
The east, center, and west runs have their amplitude
ratio plotted as a function of X, and the north, center, 
and south runs have their amplitude ratio plotted as a
function of Y.

The E, C, and W ratios are plotted in one histogram, as
are the N, C, and S ratios. For both of these composite
ratio plots, Profiles and FitSlices are taken, and are
fit with pol0 and pol2 functions.

From the Profile plots, histograms are made. The first
is a distribution of the Profile values, and the second
is a distribution of the errors of the Profile values.
From statistics in these two histograms, the bias in Y
is calculated and output.

Will Benoit
December 15th 2016

*/

{
  
  TFile *dists; //File to hold final results
  //With TChains, multiple runs at each section can be used. In this case, it was not helpful, so the cuts used when drawing removed the additional runs
  TChain *center = new TChain("h4"), *north = new TChain("h4"), *east = new TChain("h4"), *south = new TChain("h4"), *west = new TChain("h4");
  
  //Adding the relevant runs
  center->Add("ntuples_v1/analysis_5282.root");
  center->Add("ntuples_v1/analysis_5305.root");
  
  north->Add("ntuples_v1/analysis_5270.root");
  
  east->Add("ntuples_v1/analysis_5272.root");
  east->Add("ntuples_v1/analysis_5287.root");
  east->Add("ntuples_v1/analysis_5307.root");
  
  south->Add("ntuples_v1/analysis_5271.root");
  south->Add("ntuples_v1/analysis_5284.root");
  south->Add("ntuples_v1/analysis_5285.root");
  south->Add("ntuples_v1/analysis_5308.root");
  
  west->Add("ntuples_v1/analysis_5273.root");
  west->Add("ntuples_v1/analysis_5286.root");
  west->Add("ntuples_v1/analysis_5309.root");
  
  //Testing to see how far the Y axis of the histograms should extend. Overly convoluted, but useful in remembering that there was a method of selection
  float half = .6;
  float ymin = 1.06-half, ymax = 1.06+half;
  int bins = (int) round((ymax-ymin)*333.33333);
  
  //Initializations
  dists = new TFile("AmpRatios.root", "RECREATE");
  TFile *g_file = new TFile("dummy.root", "RECREATE"); //Dummy file
  TH2F *xhist = new TH2F("xhist", "X Amplitude Ratios", 80, -15, 15, bins, ymin, ymax);
  TH2F *yhist = new TH2F("yhist", "Y Amplitude Ratios", 80, -15, 15, bins, ymin, ymax);
  TH2F *temphist = new TH2F("temphist", "temphist", 80, -15, 15, bins, ymin, ymax);
  
  //Drawing from each run and adding to the proper histogram. Also, making profiles of each for optional use.
  //Hodoscope cuts were selected to stay as far away from the hodoscope and crystal edges as possible, and also to make the slices symmetric between X and Y.
  //Max amplitude cutoffs are hard-coded in, and taken from FindCutoffs.C. Additional runs in the chain can be used by modifying the second line of each cut.
  g_file->cd();
  center->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):X[0]/2 >> temphist", "X[0] > -10 && X[0] < 10 && X[1] > -7.5 && X[1] < 12.5 && Y[0] > -4 && Y[0] < 8 && Y[1] > -4 && Y[1] < 8"
               "&& ((run == 5305 && amp_max[XTAL_C0_APD1] > 310.5 && amp_max[XTAL_C0_APD2] > 311.5)) && run != 5282", "goff");
  xhist->Add(temphist, 1);
  temphist->ProfileX("centerXpfx");
  temphist->Reset();
  center->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):Y[0]/2 >> temphist", "Y[0] > -10 && Y[0] < 10 && Y[1] > -10 && Y[1] < 10 && X[0] > -8 && X[0] < 4 && X[1] > -5.5 && X[1] < 6.5"
               "&& ((run == 5305 && amp_max[XTAL_C0_APD1] > 310.5 && amp_max[XTAL_C0_APD2] > 311.5)) && run != 5282", "goff");
  yhist->Add(temphist, 1);
  temphist->ProfileX("centerYpfx");
  temphist->Reset();
  
  north->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(Y[0]-10)/2 >> temphist", "Y[0] > -10 && Y[0] < 10 && Y[1] > -10 && Y[1] < 10 && X[0] > -8 && X[0] < 4 && X[1] > -5.5 && X[1] < 6.5"
              "&& amp_max[XTAL_C0_APD1] > 504.5 && amp_max[XTAL_C0_APD2] > 540", "goff");
  yhist->Add(temphist, 1);
  temphist->ProfileX("northpfx");
  temphist->Reset();
  
  east->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(X[0]+10)/2 >> temphist", "X[0] > -10 && X[0] < 10 && X[1] > -7.5 && X[1] < 12.5 && Y[0] > -4 && Y[0] < 8 && Y[1] > -4 && Y[1] < 8"
             "&& ((run == 5307 && amp_max[XTAL_C0_APD1] > 251 && amp_max[XTAL_C0_APD2] > 322)) && run != 5272 && run != 5287", "goff");
  xhist->Add(temphist, 1);
  temphist->ProfileX("eastpfx");
  temphist->Reset();
  
  south->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(Y[0]+10)/2 >> temphist", "Y[0] > -10 && Y[0] < 10 && Y[1] > -10 && Y[1] < 10 && X[0] > -8 && X[0] < 4 && X[1] > -5.5 && X[1] < 6.5"
              "&& ((run == 5308 && amp_max[XTAL_C0_APD1] > 327.5 && amp_max[XTAL_C0_APD2] > 342)) && run != 5271 && run != 5284 && run != 5285", "goff");
  yhist->Add(temphist, 1);
  temphist->ProfileX("southpfx");
  temphist->Reset();
  
  west->Draw("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):(X[0]-10)/2 >> temphist", "X[0] > -10 && X[0] < 10 && X[1] > -7.5 && X[1] < 12.5 && Y[0] > -4 && Y[0] < 8 && Y[1] > -4 && Y[1] < 8"
             "&& ((run == 5309 && amp_max[XTAL_C0_APD1] > 346.5 && amp_max[XTAL_C0_APD2] > 308)) && run != 5273 && run != 5286", "goff");
  xhist->Add(temphist, 1);
  temphist->ProfileX("westpfx");
  temphist->Reset();
  
  //Making the X and Y ratio plots look nice, and making profiles from each of them
  TCanvas *c = new TCanvas("c", "c", 1600, 800);
  c->Divide(2,2);
  c->cd(1);
  xhist->GetXaxis()->SetTitle("Position (mm)");
  xhist->GetYaxis()->SetTitle("Amp2/Amp1");
  xhist->Draw("colz");
  xhist->SetStats(0);
  xhist->ProfileX();
//   xhist_pfx->Draw("same");
//   xhist_pfx->SetStats(0);
  c->cd(2);
  yhist->GetXaxis()->SetTitle("Position (mm)");
  yhist->GetYaxis()->SetTitle("Amp2/Amp1");
  yhist->Draw("colz");
  yhist->SetStats(0);
  yhist->ProfileX();
//   yhist_pfx->Draw("same");
//   yhist_pfx->SetStats(0);
  
  //Making the Profiles look nice
  c->cd(3);
  xhist_pfx->SetTitle("Profiles");
  xhist_pfx->GetXaxis()->SetTitle("Position (mm)");
  xhist_pfx->GetYaxis()->SetTitle("Amp2/Amp1");
  xhist_pfx->SetMarkerStyle(22);
  xhist_pfx->SetMarkerSize(.8);
  xhist_pfx->SetMarkerColor(4);
  xhist_pfx->Draw();
  xhist_pfx->SetStats(0);
  xhist_pfx->GetYaxis()->SetRangeUser(1.01, 1.08);
  yhist_pfx->SetLineColor(3);
  yhist_pfx->SetMarkerColor(3);
  yhist_pfx->SetMarkerColor(3);
  yhist_pfx->SetMarkerSize(.8);
  yhist_pfx->SetMarkerStyle(21);
  yhist_pfx->Draw("same");
  
  //Fitting both Profiles with a pol0 and a pol2
  xhist_pfx->Fit("pol0", "Q", "", -7, 7);
  yhist_pfx->Fit("pol0", "Q", "", -10, 5);
  xhist_pfx->GetFunction("pol0")->SetLineColor(4);
  yhist_pfx->GetFunction("pol0")->SetLineColor(3);
  xhist_pfx->Fit("pol2", "+Q", "", -7, 7);
  yhist_pfx->Fit("pol2", "+Q", "", -10, 5);
  xhist_pfx->GetFunction("pol2")->SetLineColor(4);
  yhist_pfx->GetFunction("pol2")->SetLineColor(3);
  xhist_pfx->GetFunction("pol2")->SetLineStyle(2);
  yhist_pfx->GetFunction("pol2")->SetLineStyle(2);
  
  //Making a legend for the Profile plot
  TLegend *legpfx = new TLegend(.1, .1, .5, .3);
  legpfx->SetNColumns(3);
  legpfx->AddEntry(xhist_pfx, "X Scan", "lp");
  legpfx->AddEntry(xhist_pfx->GetFunction("pol0"), "X Pol0 Fit", "l");
  legpfx->AddEntry(xhist_pfx->GetFunction("pol2"), "X Pol2 Fit", "l");
  legpfx->AddEntry(yhist_pfx, "Y Scan", "lp");
  legpfx->AddEntry(yhist_pfx->GetFunction("pol0"), "Y Pol0 Fit", "l");
  legpfx->AddEntry(yhist_pfx->GetFunction("pol2"), "Y Pol2 Fit", "l");
  legpfx->Draw();
  
  //Making a gaussian function, and making FitSlice plots using the function
  TF1 *gaus = new TF1("gaus", "gaus", .5, 1.5);
  xhist->FitSlicesY(gaus, 0, -1, 0, "QNR");
  yhist->FitSlicesY(gaus, 0, -1, 0, "QNR");
  
  //Making the FitSlice plots look nice
  c->cd(4);
  xhist_1->SetTitle("Fit Slices (Gaussian)");
  xhist_1->GetXaxis()->SetTitle("Position (mm)");
  xhist_1->GetYaxis()->SetTitle("Amp2/Amp1");
  xhist_1->SetMarkerStyle(22);
  xhist_1->SetMarkerSize(.8);
  xhist_1->SetMarkerColor(4);
  xhist_1->Draw();
  xhist_1->SetStats(0);
  xhist_1->GetYaxis()->SetRangeUser(1.01, 1.08);
  yhist_1->SetLineColor(3);
  yhist_1->SetMarkerColor(3);
  yhist_1->SetMarkerSize(.8);
  yhist_1->SetMarkerStyle(21);
  yhist_1->Draw("same");
  
  //Fitting both FitSlice plots with a pol0 and a pol2
  xhist_1->Fit("pol0", "Q", "", -7, 7);
  yhist_1->Fit("pol0", "Q", "", -10, 5);
  xhist_1->GetFunction("pol0")->SetLineColor(4);
  yhist_1->GetFunction("pol0")->SetLineColor(3);
  xhist_1->Fit("pol2", "+Q", "", -7, 7);
  yhist_1->Fit("pol2", "+Q", "", -10, 5);
  xhist_1->GetFunction("pol2")->SetLineColor(4);
  yhist_1->GetFunction("pol2")->SetLineColor(3);
  xhist_1->GetFunction("pol2")->SetLineStyle(2);
  yhist_1->GetFunction("pol2")->SetLineStyle(2);
  
  //Making a legend for the FitSlice plots
  TLegend *legfit = new TLegend(.1, .1, .5, .3);
  legfit->SetNColumns(3);
  legfit->AddEntry(xhist_1, "X Scan", "lp");
  legfit->AddEntry(xhist_1->GetFunction("pol0"), "X Pol0 Fit", "l");
  legfit->AddEntry(xhist_1->GetFunction("pol2"), "X Pol2 Fit", "l");
  legfit->AddEntry(yhist_1, "Y Scan", "lp");
  legfit->AddEntry(yhist_1->GetFunction("pol0"), "Y Pol0 Fit", "l");
  legfit->AddEntry(yhist_1->GetFunction("pol2"), "Y Pol2 Fit", "l");
  legfit->Draw();
  
  /////////////////////////// Making Histograms From Profiles /////////////////////
  
  
  //Initializing the two histograms
  TH1F *profx = new TH1F("profx", "profx", 20, 1.049, 1.07);
  TH1F *profy = new TH1F("profy", "profy", 20, 1.049, 1.07);
  TH1F *profxErr = new TH1F("profxErr", "profxErr", 40, 0, .01);
  TH1F *profyErr = new TH1F("profyErr", "profyErr", 40, 0, .01);
  
  //Filling the two histograms from a reasonable range of data from the Profiles
  for(int i = 24; i < 61; i++){
    profx->Fill(xhist_pfx->GetBinContent(i));
    profxErr->Fill(xhist_pfx->GetBinError(i));
  }
  
  for(int i = 17; i < 54; i++){
    profy->Fill(yhist_pfx->GetBinContent(i));
    profyErr->Fill(yhist_pfx->GetBinError(i));
  }
  
  //Drawing, labeling, and coloring the histograms
  TCanvas *profhists = new TCanvas("profhists", "profhists", 1600, 1600);
  profhists->Divide(2,2);
  profhists->cd(1);
  profx->Draw();
  profx->GetXaxis()->SetTitle("Amp2/Amp1");
  profhists->cd(2);
  profy->Draw();
  profy->SetLineColor(3);
  profy->GetXaxis()->SetTitle("Amp2/Amp1");
  profhists->cd(3);
  profxErr->Draw();
  profxErr->GetXaxis()->SetTitle("Absolute Uncertainty on Amplitude Ratio");
  profhists->cd(4);
  profyErr->Draw();
  profyErr->SetLineColor(3);
  profyErr->GetXaxis()->SetTitle("Absolute Uncertainty on Amplitude Ratio");
  
  //Calculating the X and Y bias
  float xBias, yBias, xErr, yErr;
  if((profx->GetRMS()-profxErr->GetMean()) < 0) xBias = 0;
  else xBias = sqrt((profx->GetRMS()*profx->GetRMS()) - (profxErr->GetMean()*profxErr->GetMean()));
  
  yBias = sqrt((profy->GetRMS()*profy->GetRMS()) - (profyErr->GetMean()*profyErr->GetMean()));
  
  //Calculating the error in the X and Y bias
  xErr = sqrt((profx->GetRMSError()*profx->GetRMSError()) - (profxErr->GetMeanError()*profxErr->GetMeanError()));
  yErr = sqrt((profy->GetRMSError()*profy->GetRMSError()) - (profyErr->GetMeanError()*profyErr->GetMeanError()));
  
  //Outputting final results
  cout << "X Error: " << 1000*xBias << "±" << 1000*xErr << " per mil" << endl;
  cout << "Y Error: " << 1000*yBias << "±" << 1000*yErr << " per mil" << endl;
  
////////////////////////// Drawing Separate Profiles ////////////////////////

//This section was used when there was a question about whether a different beam energy would cause a different
//amplitude ratio. To show that the ratio was the same, I plotted the profiles for the north, center, and south runs
//on one pad, and the profiles for the east, center, and west runs on another pad. The north run was the only one with
//a different energy, so showing it matched the other 4, whether vertical or horizontal, demonstrated the beam energy
//did not have an effect, at least in the conditions we used.


//   TCanvas *indivProfs = new TCanvas("indivProfs", "indivProfs", 1600, 800);
//   indivProfs->Divide(2,1);
//   
//   centerXpfx->SetMarkerColor(1);
//   centerXpfx->SetLineColor(1);
//   centerXpfx->SetStats(0);
//   centerXpfx->SetTitle("Profiles of Runs in X Scan");
//   centerXpfx->GetXaxis()->SetTitle("Position (mm)");
//   centerXpfx->GetYaxis()->SetTitle("Amp2/Amp1");
//   centerXpfx->GetYaxis()->SetRangeUser(0.85, 1.15);
//   
//   centerYpfx->SetMarkerColor(1);
//   centerYpfx->SetLineColor(1);
//   centerYpfx->SetStats(0);
//   centerYpfx->SetTitle("Profiles of Runs in Y Scan");
//   centerYpfx->GetXaxis()->SetTitle("Position (mm)");
//   centerYpfx->GetYaxis()->SetTitle("Amp2/Amp1");
//   centerYpfx->GetYaxis()->SetRangeUser(0.85, 1.15);
//   
//   northpfx->SetMarkerColor(2);
//   northpfx->SetLineColor(2);
//   eastpfx->SetMarkerColor(3);
//   eastpfx->SetLineColor(3);
//   southpfx->SetMarkerColor(4);
//   southpfx->SetLineColor(4);
//   westpfx->SetMarkerColor(6);
//   westpfx->SetLineColor(6);
// 
//   
//   indivProfs->cd(1);
//   centerXpfx->Draw();
//   eastpfx->Draw("same");
//   westpfx->Draw("same");
//   
//   TLegend *legprofx = new TLegend(0.1, 0.1, 0.3, 0.3);
//   legprofx->AddEntry(centerXpfx, "Center (100 GeV)", "l");
//   legprofx->AddEntry(eastpfx, "East (100 GeV)", "l");
//   legprofx->AddEntry(westpfx, "West (100 GeV)", "l");
//   legprofx->Draw();
//   
//   indivProfs->cd(2);
//   centerYpfx->Draw();
//   northpfx->Draw("same");
//   southpfx->Draw("same");
//   
//   TLegend *legprofy = new TLegend(0.1, 0.1, 0.3, 0.3);
//   legprofy->AddEntry(centerYpfx, "Center (100 GeV)", "l");
//   legprofy->AddEntry(northpfx, "North (150 GeV)", "l");
//   legprofy->AddEntry(southpfx, "South (100 GeV)", "l");
//   legprofy->Draw();
//   
//


////////////////////// Saving /////////////////////
  dists->cd();
  indivProfs->Write();
//   c->Write();
  profhists->Write();
}