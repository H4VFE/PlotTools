/*

This macro contains the basis for rmsDist.cpp and recoDist.cpp
For a specified raw data file, it will create a distribution of
the RMS values for each event in a given digiGroup and digiChannel

The description of the code here will match the comments in rmsDist
and recoDist, so reading those will likely give a more complete
picture of the function

Will Benoit
December 5th 2016

*/

#include <math.h>

void RMS(Int_t grp = -1, Int_t ch = -1){//Can look at all of one group, all of one channel, or all of both by using the default parameters
  
  TFile *file = TFile::Open("5663/1.root"); //Raw data file
  TTree *H4tree = (TTree*) file->Get("H4tree"); //Name of tree in root file

  int start = 360, end = 900; //Affects the range of values analyzed. Values are from 0-1024. Cuts out the signal at the beginning, and the drop at the end.
  
  Float_t vals[36864]; //4*9*1024 = 36864. #groups * #channels * #points per plot = #values per event 
  UInt_t grps[36864];
  UInt_t chs[36864];
  Float_t x[540], y[540]; //start - end. Temporary storage for the signal of each entry
  
  Int_t events = H4tree->GetEntries(); //Number of entries
  Int_t channels = H4tree->GetMaximum("digiChannel"); //Number of channels. Always 9
  Int_t groups = H4tree->GetMaximum("digiGroup"); //Number of groups. Always 4
  
  //Setting the branch addresses to the arrays so data is flushed into them
  H4tree->SetBranchAddress("digiSampleValue", vals);
  H4tree->SetBranchAddress("digiGroup", grps);
  H4tree->SetBranchAddress("digiChannel", chs);
  
  TString title;
  title.Form("RMS Distribution for Group %d, Channel %d", grp, ch); //Title of the histogram
  
  TH1F *h = new TH1F("h", title.Data(), sqrt(26712), 0, 20); //Creation of RMS distribution histogram
  //TH2F* drift = new TH2F("drift", "", 1024, 0, 1024, 100, 0, 4096); //Not used. Originally designated to plot the drift of the pedestal over time

  Float_t rms, max, min;
  
  for(int iEvt = 0; iEvt < events; iEvt++){ //Loop over all events
    H4tree->GetEntry(iEvt); //Gets the next entry, puts data from the entry into the arrays
    for(int jGrp = 0; jGrp < 36; jGrp++){ //Loop over the groups
      if(grps[jGrp*1024] != grp && grp != -1) continue; //Skip if group doesn't match given group
      min = vals[0]; //Initialize min and max
      max = 0;
      for(int kVal = start; kVal < end; kVal++){ //Loop over values in an event
	if(chs[1024*jGrp+kVal] != ch && ch != -1) break; //Skip if channel doesn't match given channel
	if(vals[1024*jGrp+kVal] > max) max = vals[1024*jGrp+kVal]; //Reassign min and max depending on current value
	else if(vals[1024*jGrp+kVal] < min) min = vals[1024*jGrp+kVal];
	x[kVal - start] = kVal; //Store index and value in x and y
	y[kVal - start] = vals[1024*jGrp+kVal];
      }
    
      if(max - min < 100){ //For small amplitudes(events without pulses), make a graph from x and y, and store the rms in the histogram
	TGraph *gr = new TGraph(end - start, x, y);
	rms = gr->GetRMS(2);
	h->Fill(rms);
      }
    }
  }
  
  //Set the boudaries for the histogram
  float r = h->GetRMS();
  float mean = h->GetMean();
  min = mean - 3*r;
  max = mean + 3*r;
  
  h->SetAxisRange(min, max, "X");
  
  //Make a filename to save the histogram as
  TString filename;
  filename.Form("./5663/1/rms_%d_%d.root", grp, ch);
  
  TFile fout(filename.Data(), "RECREATE");
  h->Write();
  
  //Make a title for the 2D histogram of the pulses
  TString plotName;
  plotName.Form("digiSampleValue:digiSampleIndex, digiGroup == %d && digiChannel == %d", grp, ch);
  
  TH2F* pulse = new TH2F("pulse", plotName.Data(), 1024, 0, 1024, 100, 0, 4096);
  
  //Make histogram with given cut
  TString cut;
  cut.Form("digiGroup == %d && digiChannel == %d", grp, ch);
  H4tree->Draw("digiSampleValue:digiSampleIndex >> pulse", cut.Data());

  //Save
  pulse->Write();
  //h->SaveAs(filename.Data());
}
