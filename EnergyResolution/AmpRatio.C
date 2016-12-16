/*

This macro makes plots of the amplitude ratio of
the two APDs on C0 for different runs, and presents
these ratios separately and together.

Will Benoit
December 14th 2016

*/

{
  TFile *g_file = new TFile("dummy.root", "RECREATE"); //Dummy file for histograms
  
  vector<int> runs; //Vector to hold run numbers
  runs.push_back(5305); runs.push_back(5270); runs.push_back(5307); runs.push_back(5308); runs.push_back(5309);
  
  vector<vector<float>> cutoffs (5); //Vector of vectors to hold the amplitude cutoff values for each APD for each run, determined by FindCutoffs.C
  cutoffs.at(0).push_back(310.5); cutoffs.at(0).push_back(311.5); cutoffs.at(1).push_back(504.5); cutoffs.at(1).push_back(540); cutoffs.at(2).push_back(251); 
  cutoffs.at(2).push_back(322); cutoffs.at(3).push_back(327.5); cutoffs.at(3).push_back(342); cutoffs.at(4).push_back(246.5); cutoffs.at(4).push_back(308); 
  
  //Temporary histograms for holding data
  TH1F *temphist1D = new TH1F("temphist1D", "temphist1D", 10000, -3000, 3000);
  TH2F *temphist2D = new TH2F("temphist2D", "temphist2D", 60, -15, 15, 1000, 0.5, 1.5);
  temphist2D->GetXaxis()->SetTitle("Position (mm)");
  temphist2D->GetYaxis()->SetTitle("Amplitude Ratio");
  
  //Setting up histograms and canvases
  TH2F *xhist = new TH2F("x", "x", 60, -15, 15, 1000, 0.5, 1.5);
  TH2F *yhist = new TH2F("y", "y", 60, -15, 15, 1000, 0.5, 1.5);
  
  //Holds all plots from the X scan
  xhist->SetTitle("X Scan - All");
  xhist->GetXaxis()->SetTitle("Position (mm)");
  xhist->GetYaxis()->SetTitle("Amplitude Ratio");
  
  //Holds all plots from the Y scan
  yhist->SetTitle("Y Scan - All");
  yhist->GetXaxis()->SetTitle("Position (mm)");
  yhist->GetYaxis()->SetTitle("Amplitude Ratio");
  
  //Canvases hold either the ratio of the side runs, or of all the runs
  TCanvas *XsideC = new TCanvas("XsideC", "XsideC", 1000, 1000);
  TCanvas *XallC = new TCanvas("XallC", "XallC", 1000, 1000);
  
  XsideC->Divide(3,3);
  XallC->Divide(1,2);
  
  TCanvas *YsideC = new TCanvas("YsideC", "YsideC", 1000, 1000);
  TCanvas *YallC = new TCanvas("YallC", "YallC", 1000, 1000);
  
  YsideC->Divide(3,3);
  YallC->Divide(1,2);
  
  //Variable declaration
  TTree* h4;
  TFile *f;
  TString filename, plot, cut, histname;
  float cutoff1, cutoff2;
  int NthRun = 0;
  
  for(auto i : runs){ //Looping through the runs
    //Opening file and getting the TTree
    filename.Form("./ntuples_v1/analysis_%d.root", i);
    f = TFile::Open(filename.Data());
    h4 = (TTree*) f->Get("h4");
    
    g_file->cd();
    
    
    //No longer needed as cutoff values are hardcoded, taken from FindCutoffs.C
//     h4->Draw("fit_ampl[XTAL_C0_APD1] >> temphist1D", "abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5", "goff");
//     cutoff1 = temphist1D->GetMean() - 3*temphist1D->GetRMS();
//     if(cutoff1 < 0) cutoff1 = temphist1D->GetMean();
//     
//     temphist1D->Reset();
//     
//     h4->Draw("fit_ampl[XTAL_C0_APD2] >> temphist1D", "abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5", "goff");
//     cutoff2 = temphist1D->GetMean() - 3*temphist1D->GetRMS();
//     
//     if(cutoff2 < 0) cutoff2 = temphist1D->GetMean();
    
    //cout << i << " " << cutoff1 << " " << cutoff2 << endl;
    
    temphist1D->Reset();
    
    //Assigning cutoffs
    cutoff1 = cutoffs.at(NthRun).at(0);
    cutoff1 = cutoffs.at(NthRun).at(1);
    
    //Creating plot and cut strings. Not perfect, because the cut should change based on the run. The +-10 in the plot string accounts for the runs being in different locations
    plot.Form("(fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]):((X[0]*(run == 5270 || run == 5308 || run == 5305) + (X[0]-10)*(run == 5309) + (X[0]+10)*(run == 5307))/2) >> temphist2D");
    cut.Form("X[0] > -10 && X[0] < 10 && X[1] > -7.5 && X[1] < 12.5 && Y[0] > -4 && Y[0] < 8 && Y[1] > -4 && Y[1] < 8 && fit_ampl[XTAL_C0_APD1] > %f && fit_ampl[XTAL_C0_APD2] > %f", cutoff1, cutoff2);
    //cut.Form("X[0] > -200 && X[1] > -200 && abs(Y[0]) < 5 && abs(Y[1]) < 5");
    
    //Drawing, and adding the data to the histogram containing all runs
    h4->Draw(plot.Data(), cut.Data(), "colz goff");
    xhist->Add(temphist2D, 1);
    
    //Different actions for each run. Generally: set an appropriate name, create a profile with an appropriate name, DrawClone into the proper pad, and DrawClone the profile into the same pad, with a unique color
    switch (i){
      case 5270:
	XsideC->cd(2);
	temphist2D->SetTitle("X Scan - North");
	temphist2D->ProfileX("XNorthpfx");
	temphist2D->DrawClone("colz");
	XNorthpfx->SetLineColor(6);
	XNorthpfx->DrawClone("same");
	break;
      case 5307:
	XsideC->cd(6);
	temphist2D->SetTitle("X Scan - East");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XEastpfx");
	XEastpfx->SetLineColor(7);
	XEastpfx->DrawClone("same");
	break;
      case 5308:
	XsideC->cd(8);
	temphist2D->SetTitle("X Scan - South");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XSouthpfx");
	XSouthpfx->SetLineColor(8);
	XSouthpfx->DrawClone("same");
	break;
      case 5309:
	XsideC->cd(4);
	temphist2D->SetTitle("X Scan - West");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XWestpfx");
	XWestpfx->SetLineColor(9);
	XWestpfx->DrawClone("same");
	break;
      case 5305:
	XsideC->cd(5);
	temphist2D->SetTitle("X Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XCenterpfx");
	XCenterpfx->SetLineColor(5);
	XCenterpfx->DrawClone("same");
	break;
    }
    
    temphist2D->Reset();
    
    //The same as the previous section, except for Y instead of X
    plot.Form("(fit_ampl[XTAL_C0_APD2]/fit_ampl[XTAL_C0_APD1]):((Y[0]*(run == 5307 || run == 5309 || run == 5305) + (Y[0]-10)*(run == 5270) + (Y[0]+10)*(run == 5308))/2) >> temphist2D");
    cut.Form("Y[0] > -10 && Y[0] < 10 && Y[1] > -10 && Y[1] < 10 && X[0] > -8 && X[0] < 4 && X[1] > -5.5 && X[1] < 6.5 && fit_ampl[XTAL_C0_APD1] > %f && fit_ampl[XTAL_C0_APD2] > %f", cutoff1, cutoff2);
    //cut.Form("Y[0] > -200 && Y[1] > -200 && abs(X[0]) < 5 && X[1] < 7.5 && X[1] > -2.5");
    
    h4->Draw(plot.Data(), cut.Data(), "colz goff");
    yhist->Add(temphist2D, 1);
    
    switch (i){
      case 5270:
	YsideC->cd(2);
	temphist2D->SetTitle("Y Scan - North");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YNorthpfx");
	YNorthpfx->SetLineColor(6);
	YNorthpfx->DrawClone("same");
	break;
      case 5307:
	YsideC->cd(6);
	temphist2D->SetTitle("Y Scan - East");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YEastpfx");
	YEastpfx->SetLineColor(7);
	YEastpfx->DrawClone("same");
	break;
      case 5308:
	YsideC->cd(8);
	temphist2D->SetTitle("Y Scan - South");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YSouthpfx");
	YSouthpfx->SetLineColor(8);
	YSouthpfx->DrawClone("same");
	break;
      case 5309:
	YsideC->cd(4);
	temphist2D->SetTitle("Y Scan - West");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YWestpfx");
	YWestpfx->SetLineColor(9);
	YWestpfx->DrawClone("same");
	break;
      case 5305:
	YsideC->cd(5);
	temphist2D->SetTitle("Y Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YCenterpfx");
	YCenterpfx->SetLineColor(5);
	YCenterpfx->DrawClone("same");
	break;
    }
    
    temphist2D->Reset();
    NthRun++; //Track the number of runs in order to access the correct cutoff value
  }
  
  //Drawing things and making them look nice
  XallC->cd(1);
  xhist->Draw("colz");
  xhist->ProfileX("xpfx");
  xpfx->Draw("same");
  XallC->cd(2);
  xhist->Draw("AXIS");
  xpfx->SetLineWidth(2);
  xpfx->Draw("same");
  XNorthpfx->DrawClone("same");
  XEastpfx->DrawClone("same");
  XSouthpfx->DrawClone("same");
  XWestpfx->DrawClone("same");
  XCenterpfx->DrawClone("same");
  
  //Legend for X
  TLegend *legx = new TLegend(0.1, 0.1, 0.5, 0.3);
  legx->SetNColumns(2);
  legx->AddEntry(xpfx, "All runs", "l");
  legx->AddEntry(XNorthpfx, "North (150 GeV)", "l");
  legx->AddEntry(XEastpfx, "East (100 GeV)", "l");
  legx->AddEntry(XSouthpfx, "South (100 GeV)", "l");
  legx->AddEntry(XWestpfx, "West (100 GeV)", "l");
  legx->AddEntry(XCenterpfx, "Center (100 GeV)", "l");
  legx->Draw();
  
  YallC->cd(1);
  yhist->Draw("colz");
  yhist->ProfileX("ypfx");
  ypfx->Draw("same");
  YallC->cd(2);
  yhist->Draw("AXIS");
  ypfx->SetLineWidth(2);
  ypfx->Draw("same");
  YNorthpfx->DrawClone("same");
  YEastpfx->DrawClone("same");
  YSouthpfx->DrawClone("same");
  YWestpfx->DrawClone("same");
  YCenterpfx->DrawClone("same");
  
  //Legend for Y
  TLegend *legy = new TLegend(0.1, 0.1, 0.5, 0.3);
  legy->SetNColumns(2);
  legy->AddEntry(ypfx, "All runs", "l");
  legy->AddEntry(YNorthpfx, "North (150 GeV)", "l");
  legy->AddEntry(YEastpfx, "East (100 GeV)", "l");
  legy->AddEntry(YSouthpfx, "South (100 GeV)", "l");
  legy->AddEntry(YWestpfx, "West (100 GeV)", "l");
  legy->AddEntry(YCenterpfx, "Center (100 GeV)", "l");
  legy->Draw();

  TFile *file1 = new TFile("AmpRatio.root", "RECREATE");
  
  //c1->Write();
  //c2->Write();
}