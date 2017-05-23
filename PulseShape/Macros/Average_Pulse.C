{ 
  /*
  
  The purpose of this macro is to average many event pulses in order to plot an average pulse for a run from a TB XTAL file: /eos/cms/store/group/dpg_ecal/comm_ecal/upgrade/testbeam/ECALTB_H4_Jul2016/raw/DataTree/analysis_*.root
  Currently requires hard coding of run number.

  5-17-17 

  Abe Tishelman-Charny

  */ 

  TString run;
  run.Form("Data_Files/analysis_5196.root"); // Testing with run 5196 
  TFile *file = TFile::Open(run.Data()); // Open data file.
 
  h4 = (TTree*) file->Get("h4"); // Get tree from current file
  
  TCanvas *c1 = new TCanvas("c1","XTAL Plot",200,20,1000,715); // First argument is what -> used with. Last four arguments: wtopx, wtopy, ww, wh
 
  TH1F *histone = new TH1F("histone", "histone", 100, 0, 100);
  TH2F *histtwo = new TH2F("histtwo", "Amplitude vs. Time", 75, 0, 75, 100, 0, 100); // May need to make sure axis covers range of data.

  TGraph *gr = new TGraph();

  Double x[500] = h4->GetV1();
  Double y[500] = h4->GetV2();
  


  // Find Spill that likely has clear pulse shapes
  TString cut;
  Int_t spill = 0;
  Double_t local_spill_max = 0; 
  Double_t global_max = 0;
 
  vector<int> good_spills;

  for (Int_t i =0; i < 5; i++) // i < max spill number. Need to obtain this.
	{
       
         cut.Form("WF_ch == XTAL_C3 && spill == %d",i);
         h4->Draw("WF_val >> gr", cut.Data() ,"colz");
	 local_spill_max = gr->GetMaximum();	 
 	 printf("Spill %d maximum WF_val entries = %f\n",i,local_spill_max);

	 if (local_spill_max > 5) // Setting threshold to save as 'significant' spill (likely to have clear waveforms)
		{

		good_spills.push_back(i);   // append array to contain spill number

		}
	 
	}  

  // Find Event within Spill that's likely a clear pulse shape based on widest range of amplitude values.
  
  TString event_cut;
  Int_t event = 0;
  Double_t local_event_max = 0; 
  Double_t global_event_max = 0;


  vector <TString*> xdata(10);
  TString name;
  for(int i = 0; i < 10; i++)
	{

	name.Form("x_%d",i);
	xdata.at(i) = name.Data();

	}

  

 
  for (Int_t i =0; i < 25; i++) // i < max event number. Need to obtain this. Since this loop will take a long time, might need to think of a way to make search faster. Might require less thouroughness (checking every 3rd or 5th event?).
	{
       
         event_cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d",spill,i);
         h4->Draw("WF_val >> gr", event_cut.Data() ,"colz");
	 local_event_max = gr->GetMaximum(); // Event with broadest amplitude range (most likely, since this would contain maximum value.)	
 	 printf("Event %d max amplitude = %f\n",i,local_event_max);

	 if (local_event_max > global_event_max)
		{
 		
		global_event_max = local_event_max;
		event = i; // Event with max amplitude so far

		}	 


	 if (local_event_max > 10) // Setting threshold of max amplitude 
		{

		// Append event number to array
		
		}


	} 

  // Plot with spill and waveform cut
  
  TString final_cut;
  final_cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d",spill,event);
  h4->Draw("WF_val:WF_time >> histtwo",final_cut.Data(),"colz");

  TString Plot_Title;  
  //Plot_Title.Form("Outputs/analysis_5196_spill%f_event%fAmpVsTime.png",spill,event); // Can also be useful to save as .root file to be accessed with a macro.
  // c1->SaveAs(Plot_Title.Data());   

  // Make Graph Look Better
 
  // Want to average many waveforms to extract single average pulse from event. 

}
