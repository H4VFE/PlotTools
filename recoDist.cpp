/*

Creates a distribution of the RMS values for each channel
of a given run using the reconstructed data files. Generally
uses the same logic as RMS.C

Saves a histogram of the RMS distribution, a 2D histogram
of all the pulses in the run for that channel, and a multigraph
of the part of the pulses that were used to make the distribution

Will Benoit
December 7th 2016

*/

#include "TTree.h"
#include <cmath>
#include "TH1F.h"
#include "TGraph.h"
#include "TFile.h"
#include "TH2F.h"
#include "TMultiGraph.h"

//Global variables. start, end, and count don't need to be global, so I'm not sure why they're here, but whatever
int start = 500, end = 900;
int count = 0;

//Arrays used to temporarily hold the data for each entry in the TTree. These do need to be global
float vals[26624];
int chs[26624];
float slope[26];
float x[400], y[400]; //Well, not these two

void RMS(int ch = -1, int events = 0, TTree* h4 = 0, TH1F* h = 0); //Function that creates the RMS distribution
TString channelMap(int ch, TTree* h4); //Function to convert the channel number into a meaningful label

int main(int argc, char *argv[]){ //Takes the run number as an argument
  char* runNum = argv[1];
  
  //Opening the file
  TString t;
  t.Form("analysis_%s.root", runNum);
  
  TFile *file = TFile::Open(t.Data());
  TTree *h4 = (TTree*) file->Get("h4");

  //Getting the number of entries, groups, and channels
  int events = h4->GetEntries(); //Probably could have done this in the RMS function so I wouldn't have to pass it
  int channels = h4->GetMaximum("WF_ch");
  
  //Assigning the branch addresses to the previously declared arrays
  h4->SetBranchAddress("WF_val", vals);
  h4->SetBranchAddress("WF_ch", chs);
  h4->SetBranchAddress("b_slope", slope); //Uses the slope of the pedestal to cut out bad events. Doesn't have an effect on the end results
  
  TH1F *h = new TH1F("h", "", sqrt(26712), 0, 20); //Histogram that will contain the RMS distribution. Pretty sure it was easier to declare it up here for some reason
  
  //Looping through all channels
  for(int ch = 0; ch <= channels; ch++){
    RMS(ch, events, h4, h);
  }
  
  return 1;
}

void RMS(int ch, int events, TTree* h4, TH1F* h){
  h->Reset(); //Resets the histogram so it can be filled with new data
  
  h->SetAxisRange(0, 20, "X"); //Setting the axis range after resestting
  
  TString title;
  title.Form("RMS Distribution for Channel %d, ", ch); //Making the first part of the histogram title
  
  TString channelLabel = channelMap(ch, h4); //Calling the label function
  
  title = title + channelLabel; //Completing the title
  
  h->SetTitle(title.Data());
  
  TMultiGraph *mg = new TMultiGraph("mg", "Events used in RMS Distribution"); //A multigraph that will contain the section of the pulses used to create the RMS distribution
  
  float rms, max, min;
  
  for(int iEvt = 0; iEvt < events; iEvt++){ //Loop over all events
    h4->GetEntry(iEvt); //Gets the next entry, puts data from the entry into the arrays
    for(int jCh = 0; jCh < 26; jCh++){ //Loop over the channels
      if(chs[jCh*1024] != ch && ch != -1) continue; //Skip if channel doesn't match given channel
      min = vals[0]; //Initialize min and max
      max = 0;
      for(int kVal = start; kVal < end; kVal++){ //Loop over values in an event. kVal can be changed to look at a section, to easily exlude pulses
	if(vals[1024*jCh+kVal] > max) max = vals[1024*jCh+kVal]; //Reassign min and max depending on current value
	else if(vals[1024*jCh+kVal] < min) min = vals[1024*jCh+kVal];
	x[kVal-start] = kVal; //Store index and value in x and y
	y[kVal-start] = vals[1024*jCh+kVal];
      }
    
      if(max - min < 50 && max - min > 0 && abs(slope[ch]) < 4){ //For small amplitudes(events without pulses) and slopes, make a graph from x and y, and store the rms in the histogram
	TGraph *gr = new TGraph(end - start, x, y);
	if(count%10 == 0) mg->Add(gr); //Add every 10th graph to the multigraph. Adding every graph is inefficient
	count++; //Counts the number of events that have small amplitudes
	rms = gr->GetRMS(2);
	h->Fill(rms);
      }
    }
  }

  //Set the range of the histogram
  float r = h->GetRMS();
  float mean = h->GetMean();
  min = mean - 3*r;
  max = mean + 3*r;
  
  h->SetAxisRange(min, max, "X");
  
  int run = (int) h4->GetMaximum("run"); //Get the run number
  
  //Make a filename to save the histogram as
  TString filename;
  filename.Form("./%d/rms_%d.root", run, ch);
  
  TFile fout(filename.Data(), "RECREATE");
  h->Write();
  
  //Make a title for the 2D histogram of the pulses
  TString plotName;
  plotName.Form("WF_val:WF_time, WF_ch == %d",ch);
  
  TH2F* pulse = new TH2F("pulse", plotName.Data(), 1024, 0, 204.8, 4096, -100, 924);
  
  //Make histogram with given cut
  TString cut;
  cut.Form("WF_ch == %d", ch);
  h4->Draw("WF_val:WF_time >> pulse", cut.Data(), "goff");
  
  //Save
  mg->Write();
  pulse->Write();
  
  std::cout << ch << std::endl; //Tracks the progress of the program
  
}

TString channelMap(int ch, TTree* h4){ //Translates the channel into a meaningful label. Might have been a better way to do this...
  //With the reconstructed data, the channel reconfiguration was accounted for. Therefore, there no need to check the run number as there was in rmsDist.cpp
  switch(ch){ //Switch on the channel number. Everything is pretty explicit
    case 0:
      return "MCP1";
    case 1:
      return "XTAL_A0_NINO1";
    case 2:
      return "XTAL_A0_SIPM1";
    case 3:
      return "XTAL_C1";
    case 4:
      return "XTAL_C2";
    case 5:
      return "XTAL_C3";
    case 6:
      return "XTAL_C4";
    case 7:
      return "XTAL_C5";
    case 8:
      return "XTAL_C0_APD1";
    case 9:
      return "XTAL_C0_APD2";
    case 10:
      return "XTAL_D0";
    case 11:
      return "XTAL_A1";
    case 12:
      return "XTAL_A2";
    case 13:
      return "XTAL_A4";
    case 14:
      return "XTAL_A5";
    case 15:
      return "XTAL_B1";
    case 16:
      return "XTAL_B2";
    case 17:
      return "XTAL_B3";
    case 18:
      return "XTAL_D2";
    case 19:
      return "XTAL_D3";
    case 20:
      return "XTAL_D4";
    case 21:
      return "XTAL_D5";
    case 22:
      return "XTAL_E1";
    case 23:
      return "XTAL_E2";
    case 24:
      return "XTAL_E4";
    case 25:
      return "XTAL_E5";
    default:
      return "";
  }
  
}