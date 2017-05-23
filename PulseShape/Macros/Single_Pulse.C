{ 
  /*
  
  The purpose of this macro is to plot a single event in a TB XTAL from a TB file: /eos/cms/store/group/dpg_ecal/comm_ecal/upgrade/testbeam/ECALTB_H4_Jul2016/raw/DataTree/analysis_*.root
  Currently requires hard coding of run number.

  5-16-17 

  Abe Tishelman-Charny

  */ 

  TString run;
  run.Form("Data_Files/analysis_5196.root"); // Testing with run 5196 
  TFile *file = TFile::Open(run.Data()); // Open data file.
 
  h4 = (TTree*) file->Get("h4"); // Get tree from current file
  
  TCanvas *c1 = new TCanvas("c1","XTAL Plot",200,20,1000,715); // First argument is what -> used with. Last four arguments: wtopx, wtopy, ww, wh
 
  TH1F *histone = new TH1F("histone", "histone", 100, 0, 100);
  TH2F *histtwo = new TH2F("histtwo", "Amplitude vs. Time", 75, 0, 75, 100, 0, 100); // May need to make sure axis covers range of data.

  vector<int> good;

  // Find Spill that likely has clear pulse shapes
  TString cut;
  Int_t spill = 0;
  Int_t local_spill_max = 0; 
  Int_t global_max = 0;
 
  for (Int_t i =0; i < 6; i++) // i < max spill number. Need to obtain this.
	{
       
         cut.Form("WF_ch == XTAL_C3 && spill == %d",i);
         h4->Draw("WF_val >> histone", cut.Data() ,"colz");
	 local_spill_max = histone->GetMaximum();	 
 	 printf("Spill %d most common WF_val entry = %d\n",i,local_spill_max);

	 if (local_spill_max > global_max)
		{
 		
		global_max = local_spill_max; // Change new max to max found.
		spill = i;  // The spill number with the greatest valued most common WF_val entry. 

		}	 

	}  

  // Find Event within Spill that's likely a clear pulse shape based on widest range of amplitude values.
  
  TString event_cut;
  Int_t event = 0;
  Int_t local_event_max = 0; 
  Int_t global_event_max = 0;
 
  for (Int_t i =0; i < 50; i++) // i < max event number. Need to obtain this. Since this loop will take a long time, might need to think of a way to make search faster. Might require less thouroughness (checking every 3rd or 5th event?).
	{
       
         event_cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d",spill,i);
         h4->Draw("WF_val >> histone", event_cut.Data() ,"colz");
	 local_event_max = histone->GetMaximum(); // Event with broadest amplitude range (most likely, since this would contain maximum value.)	
 	 printf("Event %d max amplitude = %d\n",i,local_event_max);

	 if (local_event_max > global_event_max)
		{
 		
		global_event_max = local_event_max;
		event = i; // Event with max amplitude so far

		}	 

	} 

  // Plot with spill and waveform cut
  
  TString final_cut;
  final_cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d",spill,event);
  TGraph *gr = new TGraph();
  h4->Draw("WF_val:WF_time >> gr",final_cut.Data(),"colz");
  // h4->Draw("WF_val:WF_time >> histtwo",final_cut.Data(),"colz");

  TString Plot_Title;  
  Plot_Title.Form("Outputs/analysis_5196_spill%d_event%dAmpVsTime.png",spill,event); // Can also be useful to save as .root file to be accessed with a macro.
  c1->SaveAs(Plot_Title.Data());   

  // Make Graph Look Better
 
  // Want to average many waveforms to extract single average pulse from event. 

}
