#include "TTree.h"
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraphErrors.h"
#include "TFitResultPtr.h"
#include "TFitResult.h"
#include "TLegend.h"
#include "TMath.h"
#include "TChain.h"
#include <vector>
#include <string.h>
#include <algorithm>
#include <iostream>
#include <stdlib.h>

using namespace std;

//chi2 used to generate a chi2 distribution to check accuracy of fits
TH1F *chi2 = new TH1F("chi2", "chi2", 50, 0, .5);

//Files to save good and bad fits. Enable once everything else works
// TFile *goodFits = new TFile("goodFits.root", "RECREATE");
// TFile *badFits = new TFile("badFits.root", "RECREATE");
// TFile *allFits = new TFile("allFits.root", "RECREATE");

//Function to return the slope ratio and its error
vector<float> getSlopeRatio(char slice, const float MAX1, const float MAX2, const float STEP, float xmin, float xmax, float ymin, float ymax); 

//Fucntion to fit amplitude distributions with a crystal ball fit
TF1* crystalFit(TH1F &hist, int entries);

int main(int argc, char *argv[]){

  TGraphErrors *gr = new TGraphErrors();
  
  //bool determines whether the scan is across x or y
  bool x = true;

  //assigning inputs
  const float MAX1 = atof(argv[1]), MAX2 = atof(argv[2]), STEP = atof(argv[3]);
  
  TCanvas *can = new TCanvas("can", "can", 600, 600);
  
  float xmin = -1*MAX1, xmax = MAX1, ymin = -1*MAX2, ymax = MAX2, varmin, varmax;
  
  //Iterate twice to get both x and y
  for(int i = 0; i < 2; i++){
    
    char slice;
    
    //variable assignment based on which directon we're scanning
    if(i == 0) slice = 'X';
    else{
      slice = 'Y';
      swap(xmin, ymin);
      swap(xmax, ymax);
    }
    
    //Create directory to store root files
    char buffer[100];
    sprintf(buffer,"mkdir -p /afs/cern.ch/user/w/wbenoit/work/PlotTools/EnergyResolution/Resolution%c/%g_%g_%g", slice, MAX1, MAX2, STEP);
    system(buffer);
    

    //Analyze each slice
    int num = 0;
    for(float pos = -1*MAX1; pos <= MAX1-STEP; pos+=STEP){
      varmin = pos;
      varmax = pos+STEP;
      
      vector<float> point;
      
      //Pass variables depending on x or y
      if(x){
	point = getSlopeRatio(slice, MAX1, MAX2, STEP, varmin, varmax, ymin, ymax);
	if(pos == MAX1-STEP) x = false;
      }
      else point = getSlopeRatio(slice, MAX1, MAX2, STEP, xmin, xmax, varmin, varmax);
      
      //Make graph using the results of each slice analysis 
      gr->SetPoint(num, (varmin+varmax)/4, point.at(0));
      gr->SetPointError(num, (float) STEP/4, point.at(1));
      num++;
    }
    
    //Make the graph look nice
    TString grTitle;
    grTitle.Form("Hodoscope %c Position (mm)", slice);
    
    gr->SetMarkerStyle(21);
    gr->GetXaxis()->SetTitle(grTitle.Data());
    gr->GetYaxis()->SetTitle("m2/m1");
    gr->GetYaxis()->SetTitleOffset(2);
    
    gr->Draw("apl");
    
    can->SetLeftMargin(0.15);
    
    //Save as png and as root file
    TString title;
    title.Form("./Resolution%c/%g_%g_%g/resolution%c_%g_%g_%g.png", slice, MAX1, MAX2, STEP, slice, MAX1, MAX2, STEP);
    can->SaveAs(title.Data());
    
    title.Form("./Resolution%c/%g_%g_%g/resolution%c_%g_%g_%g.root", slice, MAX1, MAX2, STEP, slice, MAX1, MAX2, STEP);
    TFile *resFile = new TFile(title.Data(), "RECREATE");
    gr->Write("gr");
  }
  
  //chi2->SaveAs("chi2.root");
  
  return 1;
}

vector<float> getSlopeRatio(char slice, const float MAX1, const float MAX2, const float STEP, float xmin, float xmax, float ymin, float ymax){
  
   TString fitFilename;
//   
//   fitFilename.Form("goodFits/Fit_%c_%g_%g_%g_%g.root", slice, xmin, xmax, ymin, ymax);
//   TFile *goodFits = new TFile(fitFilename.Data(), "RECREATE");
//   
//   fitFilename.Form("badFits/Fit_%c_%g_%g_%g_%g.root", slice, xmin, xmax, ymin, ymax);
//   TFile *badFits = new TFile(fitFilename.Data(), "RECREATE");
//   
  fitFilename.Form("allFits/Fit_%c_%g_%g_%g_%g.root", slice, xmin, xmax, ymin, ymax);
  TFile *allFits = new TFile(fitFilename.Data(), "RECREATE");
  
  vector<float> meanAmp, eX, eY, energy, slopes, errs, chi2s;
  vector<TGraphErrors*> grs;
  vector<vector<int>> runs(5);
  vector<vector<float>> indivMeans(5), meanRatios(5);
  
  for(int i = 0; i < 5; i++){
    eX.push_back(0);
    if(i == 0) energy.push_back(20);
    else energy.push_back(i*50);
  }
  
  (runs.at(0)).push_back(5650); (runs.at(0)).push_back(5651);
  (runs.at(1)).push_back(5611); (runs.at(1)).push_back(5653); (runs.at(1)).push_back(5654);
  (runs.at(2)).push_back(5619); (runs.at(2)).push_back(5656); (runs.at(2)).push_back(5655);
  (runs.at(3)).push_back(5649); (runs.at(3)).push_back(5658); (runs.at(3)).push_back(5659);
  (runs.at(4)).push_back(5630); (runs.at(4)).push_back(5660); (runs.at(4)).push_back(5661);
  
  vector<float> point(2); //Vector to be returned
  float cutoff;
  
  point.push_back(0);
  point.push_back(0);
  
  TFile *file = new TFile("myfile.root", "RECREATE");
  
  TH1F *hist = new TH1F("hist", "hist", 10000, -3000, 3000); //Histogram for the APDs
  TH1F *temphist = new TH1F("temphist", "temphist", 10000, -3000, 3000);
  
  TGraphErrors *gr = new TGraphErrors();
  TFile *f;
  TTree *h4;
  TF1 *f0, *fit;
  TString filename, plot, cut, histname, newName;
  
  newName.Form("Resolution%c/%g_%g_%g/%c_%g_%g_%g_%g.root", slice, MAX1, MAX2, STEP, slice, xmin, xmax, ymin, ymax);
  
  TFile *newFile = new TFile(newName.Data(), "RECREATE"); //File that stores the two histograms for each slice
  
  for(int apd = 1; apd < 3; apd++){
    //Nested loops to get the proportionality between runs with different gains
    for(int i = 0; i < runs.size(); i++){
      for(int j = 0; j < (runs.at(i)).size(); j++){
	
	filename.Form("./ntuples_v1/analysis_%d.root", runs[i][j]);
	f = TFile::Open(filename.Data());
	h4 = (TTree*) f->Get("h4");
	
	float xOffset = 2.5;
	
	plot.Form("fit_ampl[XTAL_C0_APD%d] >> temphist", apd);
	cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f)", xmin, xmin + xOffset, xmax, xmax + xOffset, ymin, ymin, ymax, ymax);
	
	file->cd();
	
	h4->Draw(plot.Data(), cut.Data());
	
	cutoff = temphist->GetMean() - 3*temphist->GetRMS(); //Designate a cutoff value for fit_ampl based on the previous plots
	cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f) && (fit_ampl[XTAL_C0_APD%d] > %f)", xmin, xmin + xOffset, xmax, 
		xmax + xOffset, ymin, ymin, ymax, ymax, apd, cutoff);
	long int entries = h4->Draw(plot.Data(), cut.Data()); //Replot with fit_ampl restriction
	
	cout << i << " " << j << endl;
	f0 = crystalFit(*temphist, entries);
	
	indivMeans[i].push_back(f0->GetParameter(1));
	temphist->Reset();
      }
      
      meanRatios[i].push_back((indivMeans[i].back())/(indivMeans[i].at(0)));
      
      //Resolves hole created by lack to 20 GeV, gain 50 run by filling the same number for the last two spots
      if(i == 0) meanRatios[i].push_back(1);
      
      else meanRatios[i].push_back((indivMeans[i].back())/(indivMeans[i].at(1)));
      
      meanRatios[i].push_back(1);
    }
    
    meanRatios[3][0] = 3.7; //Temp fix to make everything run, because run 5649 was processed incorrectly. Shouldn't affect enf results. May actually be permanent...
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    for(int i = 0; i < runs.size(); i++){
      for(int j = 0; j < (runs.at(i)).size(); j++){
	
	filename.Form("./ntuples_v1/analysis_%d.root", runs[i][j]);
	
	f = TFile::Open(filename.Data());
	h4 = (TTree*) f->Get("h4");
	
	float xOffset = 2.5;
	
	plot.Form("fit_ampl[XTAL_C0_APD%d] * %f >> temphist", apd,  meanRatios[i][j]);
	cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f)", xmin, xmin + xOffset, xmax, xmax + xOffset, ymin, ymin, ymax, ymax);
	
	file->cd();
	h4->Draw(plot.Data(), cut.Data());
	
	cutoff = temphist->GetMean() - 3*temphist->GetRMS();
	cut.Form("(X[0] > %f) && (X[1] > %f) && (X[0] < %f) && (X[1] < %f) && (Y[0] > %f) && (Y[1] > %f) && (Y[0] < %f) && (Y[1] < %f) && (fit_ampl[XTAL_C0_APD%d]*%f > %f)", xmin, xmin + xOffset, xmax, 
		xmax + xOffset, ymin, ymin, ymax, ymax, apd, meanRatios[i][j], cutoff);
		
	float temp = h4->Draw(plot.Data(), cut.Data());
	
	hist->Add(temphist, 1);
	
	//float entries = hist->GetEntries();
	//cout << "Hist entries: " << entries << endl;
	
	allFits->cd();
	histname.Form("hist_%d_%d_%d", apd, i, j);
	hist->Write(histname.Data());
	
	temphist->Reset();
      }
            
      float entries = hist->GetEntries();
      
      fit = crystalFit(*hist, entries);
      
      //Save histogram
      newFile->cd();
      histname.Form("APD%d_%d", apd, i); //tag
      hist->Write(histname.Data());
      file->cd();
      
      //Use the fit parameters as values to fill the arrays
      meanAmp.push_back(fit->GetParameter(1));
      eY.push_back(fit->GetParError(1));
      hist->Reset();
    }
    
    for(int i = 0; i < energy.size(); i++){
      gr->SetPoint(i, energy.at(i), meanAmp.at(i));
      gr->SetPointError(i, eX.at(i), eY.at(i));
    }

    //Making the graphs nice
    gr->GetXaxis()->SetTitle("Beam Energy");
    gr->GetYaxis()->SetTitle("Mean Fit Amplitude");
    
    if(apd == 1) gr->SetMarkerStyle(21);
    else{
      gr->SetMarkerStyle(20);
      gr->SetMarkerColor(kBlue);
      gr->SetLineColor(kBlue);
    }
    
    //Linear fit of the graph
    TFitResultPtr r = gr->Fit("pol1", "SMEQ");
    //Getting the slope and error of each fit
    slopes.push_back(r->Value(1));
    errs.push_back(r->ParError(1));
    chi2s.push_back(r->Chi2());
    
    grs.push_back(gr);
    
    for(int i = 0; i < meanRatios.size(); i++){
      (meanRatios.at(i)).clear();
      (indivMeans.at(i)).clear();
    }
    eY.clear();
    meanAmp.clear();
    
    cout << xmin << " " << xmax << " " << ymin << " " << ymax << " " << apd << endl;
  }  
  
  TMultiGraph *mg = new TMultiGraph("mg", "Energy Resolution Calibration");
  
  mg->Add(grs.at(0));
  mg->Add(grs.at(1));
  
  mg->Draw("apl");
  mg->GetXaxis()->SetTitle("Beam Energy (GeV)");
  mg->GetYaxis()->SetTitle("Mean Fit Amplitude");
  
  TLegend* leg = new TLegend(0.1,0.7,0.5,0.9);
  leg->AddEntry(grs.at(0),"APD1","lp");
  leg->AddEntry(grs.at(1),"APD2","lp");
  leg->Draw();
  
  newFile->cd();
  mg->Write();
  

  //Filling the vector with the relevant values
  point.at(0)= slopes.at(1)/slopes.at(0);
  point.at(1) = point.at(0)*sqrt((errs.at(0)*errs.at(0))/(slopes.at(0)*slopes.at(0)) + (errs.at(1)*errs.at(1))/(slopes.at(1)*slopes.at(1))); //Error propagation
  
  return point;
}

TF1* crystalFit(TH1F &hist, int entries){
  TString histname;
  
  TF1 *g, *f;
  
  g = new TF1("g", "gaus", hist.GetMean() - 2*hist.GetRMS(), hist.GetMean() + 2*hist.GetRMS());
  g->SetParameters(1, hist.GetMean(), hist.GetRMS());
  g->SetParLimits(1, hist.GetMean() - hist.GetRMS()/2, hist.GetMean() + hist.GetRMS()/2);
  g->SetParLimits(2, hist.GetRMS()/10, hist.GetRMS()/2);
  hist.Fit(g, "RQME");
  
  //Fit the plot with a crystalball based on the Gaussian
  f = new TF1("f","crystalball", hist.GetMean() - 2*hist.GetRMS(), hist.GetMean() + 2*hist.GetRMS());
  f->SetParameters(1, g->GetParameter(1), g->GetParameter(2)/3, 1, .5);
  f->SetParLimits(1, g->GetParameter(1) - g->GetParameter(2), g->GetParameter(1) + g->GetParameter(2));
  f->SetParLimits(4, 0.1, 4);
  f->SetParLimits(3, 0.1, 10);
  f->SetParLimits(2, (g->GetParameter(2))/10, g->GetParameter(2)/2);
  hist.Fit(f, "RQME");
  
  //Attempt to refit if the first failed for some reason, such as a bad Gaussian fit
  if((f->GetChisquare()/entries - 0.2132) > 1*0.03559){ //Using values from chi2 distribution on 6 6 2 run
    f->SetParameter(1, hist.GetMean());
    f->SetParameter(2, hist.GetRMS()/3);
    f->SetParLimits(1, hist.GetMean() - hist.GetRMS()/2, hist.GetMean() + hist.GetRMS()/2);
    f->SetParLimits(2, hist.GetRMS()/10, hist.GetRMS()/2);
    hist.Fit(f, "RQME");
    
//     if((f->GetChisquare()/entries1 - 0.2132) > 1*0.03559){
//       badFits->cd();
//       histname.Form("APD1_%d_%g_%g_%g_%g", i, xmin, xmax, ymin, ymax); //tag
//       hist.Write(histname.Data());
//     }
//     else{
//     goodFits->cd();
//     histname.Form("APD1_%d_%g_%g_%g_%g", i, xmin, xmax, ymin, ymax); //tag
//     hist.Write(histname.Data());
//     }
  }
  
  return f;
}
