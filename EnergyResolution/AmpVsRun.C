/*

This macro uses the data files on lxplus to create
a plot of max amplitude vs run number for the two
APDs on crystal C0. The commented section is incomplete,
and I wouldn't recommend using it as it is now because
the methodology could be improved. Also, the initial
data wasn't that useful.

However, the upper part was repurposed to find runs that
weren't processed properly by checking whether they could
be opened. This functionality could be quite useful.

Will Benoit
December 8th 2016

*/

{
  //Root file object declarations
  TFile *file = new TFile("dummy.root", "RECREATE");
  TTree *h4;
  TFile *f;
  TString filename, cut;
  
  TFile *plots = new TFile("AmpPlots.root", "RECREATE"); //File to store output of lower section
  
  //Variables used for the lower section
  TH1F *hist = new TH1F("hist", "hist", 1000, -1000, 3000);
  vector<float> means1, means2;
  vector<int> runs;
  
  ofstream outFile;
  outFile.open("BadRuns.txt", ios::app); //File to record runs that couldn't be opened
  
  for(int i = 5166; i < 5551; i++){
    
    cout << i << endl;
    
    //Some files crashed the program when attempting to open them, and so had to be excluded manually
    if(i == 5172 || i == 5183 || i == 5301 || i == 5307 || i == 5342 || i == 5343 || i == 5365 || i == 5373 || i == 5384 || i == 5420 || i == 5456 || i == 5458 || i == 5472 || i == 5494 || i == 5518) continue;
    if(i == 5189 || i == 5199 || (i >= 5211 && i <= 5214) || i == 5224 || i == 5247 || i == 5248 || i == 5250 || i == 5262 || i == 5270 || i == 5271 || i == 5276 || i == 5277 || i == 5282 || i == 5298) continue;
    
    filename.Form("/afs/cern.ch/user/w/wbenoit/eos/cms/store/group/dpg_ecal/comm_ecal/upgrade/testbeam/ECALTB_H4_Jul2016/ntuples_V2/ntuples/analysis_%d.root", i);
    f = TFile::Open(filename.Data()); //Opening the file
    if(!(f->IsOpen())){ //One check of if the file opened
      outFile << i << endl; //Write to file and skip this run
      continue;
    }
    if(!(f->GetListOfKeys()->Contains("h4"))){ //Even if the file opened correctly, it may not have been processed correctly. This checks if the h4 TTree was created
      outFile << ">>> " << i << endl; //Write to file and skip this run
      f->Close();
      continue;
    }
    
    outFile.close();
    
    h4 = (TTree*) f->Get("h4");
    
    file->cd();} //Ending the loop here. Remove the bracket if you want to mess with the commented section below
   /* 
    h4->Draw("((amp_max[XTAL_C0_APD1]*(run > 5580)) + (amp_max[XTAL_A0_SIPM1]*(run < 5580))) >> hist", "abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5", "goff");
    float cutoff1 = hist->GetMean() - 3*hist->GetRMS();
    cut.Form("abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5 && ((amp_max[XTAL_C0_APD1]*(run > 5580)) + (amp_max[XTAL_A0_SIPM1]*(run < 5580))) > %f", cutoff1);
    h4->Draw("((amp_max[XTAL_C0_APD1]*(run > 5580)) + (amp_max[XTAL_A0_SIPM1]*(run < 5580))) >> hist", cut.Data(), "goff");
	     
    plots->cd();
    hist->Write();
    file->cd();

    runs.push_back(i);
    means1.push_back(hist->GetMean());
    
    hist->Reset();
    
    h4->Draw("((amp_max[XTAL_C0_APD2]*(run > 5580)) + (amp_max[XTAL_B2]*(run < 5580))) >> hist", "abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5", "goff");
    float cutoff2 = hist->GetMean() - 3*hist->GetRMS();
    cut.Form("abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5 && ((amp_max[XTAL_C0_APD2]*(run > 5580)) + (amp_max[XTAL_B2]*(run < 5580))) > %f", cutoff2);
    h4->Draw("((amp_max[XTAL_C0_APD2]*(run > 5580)) + (amp_max[XTAL_B2]*(run < 5580))) >> hist", cut.Data(), "goff");
	     
    plots->cd();
    hist->Write();
    
    means2.push_back(hist->GetMean());
    
    hist->Reset();
  }
  
  TGraph *gr1 = new TGraph();
  TGraph *gr2 = new TGraph();
  
  for(int i = 0; i < runs.size(); i++){
    gr1->SetPoint(i, runs.at(i), means1.at(i));
    gr2->SetPoint(i, runs.at(i), means2.at(i));
  }
  
  TCanvas *c = new TCanvas("c", "c", 2400, 1200);
  
  c->Divide(2,2);
  
  c->cd(1);
  gr1->Draw("ap");
  gr1->SetTitle("APD 1");
  gr1->GetXaxis()->SetTitle("Run Number");
  gr1->GetYaxis()->SetTitle("Mean Max Amplitude");
  gr1->SetMarkerStyle(20);
  
  c->cd(2);
  gr2->Draw("ap");
  gr2->SetTitle("APD 2");
  gr2->GetXaxis()->SetTitle("Run Number");
  gr2->GetYaxis()->SetTitle("Mean Max Amplitude");
  gr2->SetMarkerStyle(21);
  
  c->SaveAs("AmpVsRun.root");*/
}