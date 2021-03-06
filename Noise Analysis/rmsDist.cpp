/*

Creates a distribution of the RMS values for each channel
of a given run using the raw data files. Generally uses
the same logic as RMS.C

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
int start = 350, end = 900;
int count = 0;

//Arrays used to temporarily hold the data for each entry in the TTree. These do need to be global
float vals[36864];
uint grps[36864];
uint chs[36864];
float x[550], y[550]; //Well, not these two

void RMS(int grp = -1, int ch = -1, int events = 0, TTree* H4tree = 0, TH1F* h = 0); //Function that craetes the RMS distribution
TString channelMap(int grp, int ch, TTree* H4tree); //Function to convert the channel number into a meaningful label

int main(int argc, char *argv[]){ //Takes the run number as an argument
  char* runNum = argv[1];
  
  //Opening the file
  TString t;
  t.Form("%s/1.root", runNum);
  
  TFile *file = TFile::Open(t.Data());
  TTree *H4tree = (TTree*) file->Get("H4tree");

  //Getting the number of entries, groups, and channels
  int events = H4tree->GetEntries(); //Probably could have done this in the RMS function so I wouldn't have to pass it
  int channels = H4tree->GetMaximum("digiChannel");
  int groups = H4tree->GetMaximum("digiGroup");
  
  //Assigning the branch addresses to the previously declared arrays
  H4tree->SetBranchAddress("digiSampleValue", vals);
  H4tree->SetBranchAddress("digiGroup", grps);
  H4tree->SetBranchAddress("digiChannel", chs);
  
  TH1F *h = new TH1F("h", "", sqrt(26712), 0, 20); //Histogram that will contain the RMS distribution. Pretty sure it was easier to declare it up here for some reason
  
  //Looping through all groups and channels
  for(int grp = 0; grp <= groups ; grp++){
    for(int ch = 0; ch <= channels; ch++){
      RMS(grp, ch, events, H4tree, h);
    }
  }
  
  return 1;
}

void RMS(int grp, int ch, int events, TTree* H4tree, TH1F* h){
  h->Reset(); //Resets the histogram so it can be filled with new data
  
  h->SetAxisRange(0, 20, "X"); //Setting the axis range after resestting
  
  TString title;
  title.Form("RMS Distribution for Group %d, Channel %d, ", grp, ch); //Making the first part of the histogram title
  
  TString channelLabel = channelMap(grp, ch, H4tree); //Calling the label function
  
  title = title + channelLabel; //Completing the title
  
  h->SetTitle(title.Data());
  
  TMultiGraph *mg = new TMultiGraph("mg", "Events used in RMS Distribution"); //A multigraph that will contain the section of the pulses used to create the RMS distribution
  
  float rms, max, min;
  
  for(int iEnt = 0; iEnt < events; iEnt++){ //Loop over all events
    H4tree->GetEntry(iEnt); //Gets the next entry, puts data from the entry into the arrays
    for(int jGrp = 0; jGrp < 36; jGrp++){ //Loop over the groups
      if(grps[jGrp*1024] != grp && grp != -1) continue; //Skip if group doesn't match given group
      min = vals[0]; //Initialize min and max
      max = 0;
      for(int kVal = start; kVal < end; kVal++){ //Loop over values in an event. kVal can be changed to look at a section, to easily exlude pulses
	if(chs[1024*jGrp+kVal] != ch && ch != -1) break; //Skip if channel doesn't match given channel
	if(vals[1024*jGrp+kVal] > max) max = vals[1024*jGrp+kVal]; //Reassign min and max depending on current value
	else if(vals[1024*jGrp+kVal] < min) min = vals[1024*jGrp+kVal];
	x[kVal-start] = kVal; //Store index and value in x and y
	y[kVal-start] = vals[1024*jGrp+kVal];
      }
    
      if(max - min < 50 && max - min > 0){ //For small amplitudes(events without pulses), make a graph from x and y, and store the rms in the histogram
	TGraph *gr = new TGraph(end - start, x, y);
	if(count%10 == 0) mg->Add(gr); //Add every 10th graph to the multigraph. Adding every graph is inefficient
	count++; //Counts the number of events that have small amplitudes
	rms = gr->GetRMS(2);
	h->Fill(rms);
      }
    }
    
    if(iEnt == events -1) std::cout<<grp<<" "<<ch<<std::endl; //Tracks the progress of the program
  }

  //Set the range of the histogram
  float r = h->GetRMS();
  float mean = h->GetMean();
  min = mean - 3*r;
  max = mean + 3*r;
  
  h->SetAxisRange(min, max, "X");
  
  int run = (int) H4tree->GetMaximum("runNumber"); //Get the run number
  
  //Make a filename to save the histogram as
  TString filename;
  filename.Form("./%d/1/rms_%d_%d.root", run, grp, ch);
  
  TFile fout(filename.Data(), "RECREATE");
  h->Write();
  
  //Make a title for the 2D histogram of the pulses
  TString plotName;
  plotName.Form("digiSampleValue:digiSampleIndex, digiGroup == %d && digiChannel == %d", grp, ch);
  
  TH2F* pulse = new TH2F("pulse", plotName.Data(), 1024, 0, 1024, 4096, 0, 4096);
  
  //Make histogram with given cut
  TString cut;
  cut.Form("digiGroup == %d && digiChannel == %d", grp, ch);
  H4tree->Draw("digiSampleValue:digiSampleIndex >> pulse", cut.Data(), "goff");
  
  //Save
  mg->Write();
  pulse->Write();
}

TString channelMap(int grp, int ch, TTree* H4tree){ //Translates the (group, channel) into a meaningful label. Might have been a better way to do this...
  int channel = 8*grp + ch; //Turn the pair into a single number
  float run = H4tree->GetMaximum("runNumber");
  
  //Channels were changed at certain times during the test beam, so different runs may have a different channel configuration
  if(run < 5400){
    switch(channel){ //Switch on the channel number. Everything is pretty explicit
      case 0:
	return "E5, APD";
      case 1:
	return "E4, APD";
      case 2:
	return "Not Used";
      case 3:
	return "Not Used";
      case 4:
	return "E2, APD, Low Ampl";
      case 5:
	return "E1, APD";
      case 6:
	return "Not Used";
      case 7:
	return "Not Used";
      case 8:
	return "A2, APD";
      case 9:
	return "Not Used";
      case 10:
	return "Not Used";
      case 11:
	return "Not Used";
      case 12:
	return "D5, APD";
      case 13:
	return "Not Used";
      case 14:
	return "Not Used";
      case 15:
	return "C1, APD";
      case 16:
	return "MCP";
      case 17:
	return "C2, APD";
      case 18:
	return "C3, APD";
      case 19:
	return "C4, APD";
      case 20:
	return "D3, APD";
      case 21:
	return "A0, Nano, SiPM";
      case 22:
	return "A0, Analog, SiPM";
      case 23:
	return "B2, APD, Low Ampl";
      case 24:
	return "D4, APD";
      case 25:
	return "D2, APD";
      case 26:
	return "Not Used";
      case 27:
	return "Not Used";
      case 28:
	return "Not Used";
      case 29:
	return "D0, APD, Irradiated 2";
      case 30:
	return "C0, Double APD";
      case 31:
	return "C0, Double APD";
      default:
	return "";
    }
    
  }
  else if(run < 5602){
    switch(channel){
      case 0:
	return "E5, APD";
      case 1:
	return "E4, APD";
      case 2:
	return "Not Used";
      case 3:
	return "B3, APD";
      case 4:
	return "E2, APD, Low Ampl";
      case 5:
	return "E1, APD";
      case 6:
	return "Not Used";
      case 7:
	return "Not Used";
      case 8:
	return "A1, APD";
      case 9:
	return "A2, APD";
      case 10:
	return "A4, APD";
      case 11:
	return "A5, APD";
      case 12:
	return "D5, APD";
      case 13:
	return "Not Used";
      case 14:
	return "C5, APD";
      case 15:
	return "C1, APD";
      case 16:
	return "MCP";
      case 17:
	return "C2, APD";
      case 18:
	return "C3, APD";
      case 19:
	return "C4, APD";
      case 20:
	return "D3, APD";
      case 21:
	return "A0, Nano, SiPM";
      case 22:
	return "A0, Analog, SiPM";
      case 23:
	return "B2, APD, Low Ampl";
      case 24:
	return "D4, APD";
      case 25:
	return "D2, APD";
      case 26:
	return "Not Used";
      case 27:
	return "B1, APD";
      case 28:
	return "Not Used";
      case 29:
	return "D0, APD, Irradiated 2";
      case 30:
	return "C0, Double APD";
      case 31:
	return "C0, Double APD";
      default:
	return "";
    }
  }
  else{
    switch(channel){
      case 0:
	return "E5, APD";
      case 1:
	return "E4, APD";
      case 2:
	return "Not Used";
      case 3:
	return "B3, APD";
      case 4:
	return "Not Used";
      case 5:
	return "Not Used";
      case 6:
	return "Not Used";
      case 7:
	return "Not Used";
      case 8:
	return "A1, APD";
      case 9:
	return "A2, APD";
      case 10:
	return "A4, APD";
      case 11:
	return "A5, APD";
      case 12:
	return "D5, APD";
      case 13:
	return "Not Used";
      case 14:
	return "C5, APD";
      case 15:
	return "C1, APD";
      case 16:
	return "MCP";
      case 17:
	return "C2, APD";
      case 18:
	return "C3, APD";
      case 19:
	return "C4, APD";
      case 20:
	return "D3, APD";
      case 21:
	return "A0, Nano, SiPM";
      case 22:
	return "C0, Double APD";
      case 23:
	return "C0, Double APD";
      case 24:
	return "D4, APD";
      case 25:
	return "D2, APD";
      case 26:
	return "Not Used";
      case 27:
	return "B1, APD";
      case 28:
	return "B0, Spike";
      case 29:
	return "E0, APD, Irradiated 1";
      case 30:
	return "A0, Analog, SiPM";
      case 31:
	return "Not Used";
      default:
	return "";
    }
  }
}