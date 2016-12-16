/*

This macro compares the distributions of different
quantities stored in the ROOT file, such as b_slope, 
b_rms, or amp_max. Can change by using the replace 
command. Not super useful, but decent if you need to
compare a bunch of runs or something.


Will Benoit
December 14th 2016

*/


{
  TFile *g_file = new TFile("dummy.root", "RECREATE"); //Dummy file to make things work
  
  vector<TH1F*> hists(18); //Vector to hold all the histograms
  vector<int> runs; //Vector of run numbers
  runs.push_back(5270); runs.push_back(5307); runs.push_back(5308); runs.push_back(5309); runs.push_back(5305); runs.push_back(5651); runs.push_back(5654); runs.push_back(5655);  runs.push_back(5659); runs.push_back(5661);
 
  TH1F *temphist = new TH1F("temphist", "temphist", 50, 0, 10); //Temporary histogram to be added to the vector of histograms
  
  //Variable declarations
  TTree *h4;
  TFile *f;
  TString filename, plot, cut, legname, histname;
  float scale, cutoff1, cutoff2;
  
  //Used for tracking stuff
  int color = 1;
  int num = 0;
  
  //Canvas and legend creation
  TCanvas *c = new TCanvas("c", "c", 1000, 1000);
  c->Divide(1,2);
  
  TLegend *leg1 = new TLegend(0.5,0.7,0.9,0.9);
  TLegend *leg2 = new TLegend(0.5,0.7,0.9,0.9);
  leg1->SetNColumns(2);
  leg2->SetNColumns(2);
  
  for(auto i : runs){//Go through all runs
    
    if(i == 5308 || i == 5309 || i == 5270) continue; //Can look at only one cardinal direction. Comment out to look at all directions.
    
    //Initiazling each member of hists
    histname.Form("Run %d, APD1", i);
    hists.at(num) = new TH1F(histname.Data(), "APD1", 50, 0, 10);
    histname.Form("Run %d, APD2", i);
    hists.at(num+1) = new TH1F(histname.Data(), "APD2", 50, 0, 10);
    
    //Different hodoscope cuts for each run. These may need to be tweaked.
    switch (i){
      case 5270:
	legname.Form("North");
	cut.Form("abs(X[0]) < 6 && X[1] < 8.5 && X[1] > -3.5 && Y[0] > 0 && Y[1] > 0 && Y[0] < 10 && Y[1] < 10 && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1");
	break;
      case 5307:
	legname.Form("East");
	cut.Form("abs(Y[0]) < 6 && abs(Y[1]) < 6 && X[0] < 0 && X[1] < 2.5 && X[0] > -10 && X[1] > -7.5 && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1");
	break;
      case 5308:
	legname.Form("South");
	cut.Form("abs(X[0]) < 6 && X[1] < 8.5 && X[1] > -3.5 && Y[0] > -10 && Y[1] > -10 && Y[0] < 0 && Y[1] < 0 && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1");
	break;
      case 5309:
	legname.Form("West");
	cut.Form("abs(Y[0]) < 6 && abs(Y[1]) < 6 && X[0] < 10 && X[1] < 12.5 && X[0] > 0 && X[1] > 2.5 && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1");
	break;
      default:
	legname.Form("Center");
	cut.Form("abs(Y[0]) < 6 && abs(Y[1]) < 6 && X[0] < 0 && X[1] < 2.5 && X[0] > -10 && X[1] > -7.5 && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1");
	break;
    }
    
    filename.Form("./ntuples_v1/analysis_%d.root", i);
    f = TFile::Open(filename.Data());
    h4 = (TTree*) f->Get("h4");
    
    g_file->cd();
    //Calculating cutoff values for whatever quantity
    h4->Draw("b_rms[XTAL_C0_APD1] >> temphist", "abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5", "goff");
    cutoff1 = temphist->GetMean() - 3*temphist->GetRMS();
    if(cutoff1 < 0) cutoff1 = temphist->GetMean();
    
    temphist->Reset();
    
    h4->Draw("b_rms[XTAL_C0_APD2] >> temphist", "abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5", "goff");
    cutoff2 = temphist->GetMean() - 3*temphist->GetRMS();
    
    if(cutoff2 < 0) cutoff2 = temphist->GetMean();
    
    //cout << cutoff1 << " " << cutoff2 << endl;
    
    temphist->Reset();
    
    //Drawing final plots, adding them to the vector, normalizing, and making them look nice
    plot.Form("b_rms[XTAL_C0_APD1] >> temphist");
    
    h4->Draw(plot.Data(), cut.Data(), "goff");
    
    hists.at(num)->Add(temphist, 1);
    hists.at(num)->SetStats(0);
    hists.at(num)->SetLineColor(color);
    hists.at(num)->Sumw2();
    scale = hists.at(num)->Integral();
    hists.at(num)->Scale(1/scale);
    if(i < 5600) hists.at(num)->SetLineWidth(4);
    c->cd(1);
    hists.at(num)->GetYaxis()->SetRangeUser(0,0.25);
    hists.at(num)->Draw("same");
    leg1->AddEntry(hists.at(num), legname.Data(), "l");
    
    plot.Form("b_rms[XTAL_C0_APD2] >> temphist");

    h4->Draw(plot.Data(), cut.Data(), "goff");
    
    hists.at(num+1)->Add(temphist, 1);
    hists.at(num+1)->SetStats(0);
    hists.at(num+1)->SetLineColor(color);
    hists.at(num+1)->Sumw2();
    scale = hists.at(num+1)->Integral();
    hists.at(num+1)->Scale(1/scale);
    if(i < 5600) hists.at(num+1)->SetLineWidth(4);
    c->cd(2);
    hists.at(num+1)->GetYaxis()->SetRangeUser(0,0.25);
    hists.at(num+1)->Draw("same");
    leg2->AddEntry(hists.at(num+1), legname.Data(), "l");
    
    color++; //Changing color
    num += 2; //Tracking where the histogram should be placed
  }
  
  //Adding legends
  c->cd(1);
  leg1->Draw();
  c->cd(2);
  leg2->Draw();
  
}