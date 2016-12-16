/*

This macro was used to compare the amplitude ratio
of the two APDs on C0 when the beam was to C0 to
when the beam was on C1

Will Benoit
December 14th 2016

*/

{
  
  //Opening relevant files
  TFile *fileC0, *fileC1, *dummy;
  fileC0 = TFile::Open("ntuples_v1/analysis_5270.root");
  fileC1 = TFile::Open("ntuples_v1/analysis_5278.root");
  dummy = new TFile("dummy.root", "RECREATE"); //Dummy file for histograms to live in
  
  //Getting the trees
  TTree *treeC0, *treeC1;
  treeC0 = (TTree*) fileC0->Get("h4");
  treeC1 = (TTree*) fileC1->Get("h4");
  
  TGraphErrors *mean = new TGraphErrors();
  
  dummy->cd();
  TH1F *temphist = new TH1F("temphist", "temphist", 200, 0, 2);
  vector<TH1F*> hists(10); //Vector of TH1Fs to be filled with empty histograms
  TString histname;
  for(int i = 0; i < 10; i++){
    histname.Form("hist%d", i);
    hists.at(i) = new TH1F(histname.Data(), histname.Data(), 200, 0, 2);
  }
  
  //Cut to take out unwanted values
  TString cut;
  cut.Form("amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1] > 0 && amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1] < 5 && amp_max[XTAL_C0_APD1] > 40 && amp_max[XTAL_C0_APD2] > 40");
  
  //Drawing the ratio for the beam on C0 and making it look nice
  TCanvas *c = new TCanvas("c", "c", 800, 800);
  treeC0->Draw("amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1] >> temphist", cut.Data(), "goff");
  hists.at(0)->Add(temphist, 1);
  float scale = hists.at(0)->Integral(); //Normalizing
  hists.at(0)->Scale(1/scale);
  hists.at(0)->SetFillStyle(0);
  hists.at(0)->SetFillColor(1);
  hists.at(0)->SetLineColor(1);
  hists.at(0)->SetLineWidth(2);
  hists.at(0)->SetTitle("Normalized Amplitude Ratio Distribution");
  hists.at(0)->GetXaxis()->SetTitle("Amp2/Amp1");
  hists.at(0)->SetStats(0);
  hists.at(0)->Draw();
  
  //Drawing the ratio with the beam on C1 with various cut on the amplitude
  for(int i = 0; i < 9; i++){
    cut.Form("amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1] > 0 && amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1] < 5 && amp_max[XTAL_C0_APD1] > %d && amp_max[XTAL_C0_APD2] > %d", i*10, i*10);
    treeC1->Draw("amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1] >> temphist", cut.Data(), "goff");
    //Making it look nice
    hists.at(i+1)->Add(temphist, 1);
    hists.at(i+1)->SetFillColor(i+2);
    hists.at(i+1)->SetLineColor(i+2);
    if(i == 8) hists.at(i+1)->SetLineColor(7);
    hists.at(i+1)->SetLineWidth(2);
    scale = hists.at(i+1)->Integral(); //Normalizing
    hists.at(i+1)->Scale(1/scale);
    hists.at(i+1)->SetFillStyle(0);
    if(i%2 == 0) hists.at(i+1)->Draw("same");
    
    //Getting mean values for later plot
    mean->SetPoint(i, i*10, hists.at(i+1)->GetMean());
    mean->SetPointError(i, 0, hists.at(i+1)->GetMeanError());
    
  }
  
  //Making the legend
  TLegend *leg = new TLegend(0.1, 0.7, 0.4, 0.9);
  leg->SetNColumns(2);
  leg->AddEntry(hists.at(0), "Beam @C0", "l");
  leg->AddEntry(hists.at(1), "@C1, amp > 0", "l");
  leg->AddEntry(hists.at(3), "@C1, amp > 20", "l");
  leg->AddEntry(hists.at(5), "@C1, amp > 40", "l");
  leg->AddEntry(hists.at(7), "@C1, amp > 60", "l");
  leg->AddEntry(hists.at(9), "@C1, amp > 80", "l");
  leg->Draw();
  
  //Creating the canvas
  TCanvas *stats = new TCanvas("stats", "stats", 1200, 1200);
  stats->Divide(1,2);
  
  //Drawing the amplitude distribution of APD1 with the beam on C1
  stats->cd(1);
  TH1F *ampHist = new TH1F("ampHist", "Maxmimum Amplitude Distribution For APD1 on Crystal C0 With the Beam on C1", 100, -10, 100);
  treeC1->Draw("amp_max[XTAL_C0_APD1] >> ampHist", "", "goff");
  ampHist->Draw();
  ampHist->GetXaxis()->SetTitle("Maximum Amplitude");
  
  //Making the mean plot look nice
  mean->SetTitle("Mean of Amplitude Ratio Distribution");
  mean->SetMarkerStyle(20);
  mean->GetXaxis()->SetRangeUser(-10, 100);
  mean->GetXaxis()->SetTitle("Max Amplitude Cutoff");
  mean->GetYaxis()->SetTitle("Mean Amplitude Ratio");
  
  //Drawing the mean plot
  stats->cd(2);
  mean->Draw("ap");
  mean->GetXaxis()->SetRangeUser(-10, 100);
  mean->Draw("ap"); //Need to do this twice for the range to take effect, I think
  
  //Additional comparison of two histograms. The amplitude cut at 40 was decided to be the best for comparison
  TCanvas *can = new TCanvas("can", "can", 800, 800);
  hists.at(0)->Draw();
  hists.at(5)->Draw("same");
  
  //Legend for previous drawing
  TLegend *leg2 = new TLegend(0.1, 0.7, 0.3, 0.9);
  leg2->AddEntry(hists.at(0), "Beam @C0", "l");
  leg2->AddEntry(hists.at(5), "@C1, amp > 40", "l");
  leg2->Draw();
  
  //Comparing the means
  cout << hists.at(0)->GetMean() << endl << hists.at(5)->GetMean() << endl;
  
  //Could look at fit_chi2, but it wasn't that useful
//   TCanvas *chi2test = new TCanvas("chi2test", "chi2test", 1600, 800);
//   chi2test->Divide(2,1);
//   chi2test->cd(1);
//   treeC0->Draw("fit_chi2[XTAL_C0_APD1]:amp_max[XTAL_C0_APD1]", "", "colz");
//   chi2test->cd(2);
//   treeC0->Draw("fit_chi2[XTAL_C0_APD2]:amp_max[XTAL_C0_APD2]", "WF_ch == XTAL_C0_APD2", "colz");

  
}