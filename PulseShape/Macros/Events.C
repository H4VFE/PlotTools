{
  /* The purpose of this macro is to take an analysis_*.root file and obtain the 'good' events and spills, where goodness is defined by some threshold of minimum peak WF_val. This should return events with some meaningful (non-zero) ampltiude. The data from these events can then be normalized, averaged and plotted.

Abe Tishelman-Charny
24-5-17
   */

  TString run; // Can change to user input run if desired
  run.Form("Data_Files/analysis_5196.root"); // Testing with run 5196 
  TFile *file = TFile::Open(run.Data()); // Open data file.
 
  h4 = (TTree*) file->Get("h4"); // Get h4 tree from current opened file
  
  TCanvas *c1 = new TCanvas("c1","XTAL Plot",200,20,1000,715); // First argument is what -> used with. Last four arguments: wtopx, wtopy, ww, wh
 
  // TH1F *histone = new TH1F("histone", "histone", 100, 0, 100);
  // TH2F *histtwo = new TH2F("histtwo", "Amplitude vs. Time", 75, 0, 75, 100, 0, 100); // May need to make sure axis covers range of data.

  TGraph *gr = new TGraph();
  TGraph *gr2 = new TGraph(); // For testing plots

  //int spill_dimension = 1;
  //int event_dimension = 1; 

  vector< vector < int > > good_spills_runs;//(spill_dimension, vector<int>(event_dimension)); // Vector of spill vectors, each spill vector contains good event numbers. First element of each spill vector is spill number. 

  // Find Spill that likely has clear pulse shapes
  TString cut;
  Int_t spill = 0;
  Int_t local_spill_max = 0; 
  Int_t global_max = 0;
 
  //  h4->Draw("WF_val >> gr", "WF_ch == XTAL_C3 && spill == 4");

  for (Int_t i =0; i < 6; i++) // i < max spill number. Need to obtain this.
	{

         cut.Form("WF_ch == XTAL_C3 && spill == %d",i);
         h4->Draw("WF_val:WF_time >> gr", cut.Data() ,"goff"); // goff = graphics off, colz = colors
	 // gr->SetMaximum(0); // Sets maximum to 0
	 Double_t* ydata = h4->GetV1();
	 Int_t range = h4->GetSelectedRows();       
	 local_spill_max = *max_element(ydata,ydata+range);	 
 	 printf("Spill %d maximum WF_val = %d\n",i,local_spill_max);

	 if (local_spill_max > 100) // Set threshold for spill's max WF_val. If spill's max value is less than this, spill number not saved. 
		{
		vector <int> NewColumn;
		good_spills_runs.push_back(NewColumn); // Create spill vector
		good_spills_runs.at(good_spills_runs.size()-1).push_back(i); // Enter spill number as last (currently also first) element of new spill vector. New spill vector pushed back. 0th spill vector (row) is first good spill.

		}


	}  


  // good_spills_runs.at(5).push_back(99);

  // Print vector elements.
  for (int i = 0; i < good_spills_runs.size(); i++)
	{
    	for (int j = 0; j < good_spills_runs[i].size(); j++)
   		{
       	        cout << "good_spills_runs[" << i << "][" << j << "] = "  << good_spills_runs[i][j] << endl;
	        }
	}

  // h4->Draw("WF_val:WF_time >> gr2", "WF_ch == XTAL_C3 && spill == 0");
  // Find Event within Spill that's likely a clear pulse shape based on widest range of amplitude values.
 
/* 

  TString event_cut;
  Int_t event = 0;
  Int_t local_event_max = 0; 
  Int_t global_event_max = 0;

 // h4->Draw("WF_val:event >> gr", "WF_ch == XTAL_C3 && spill == 0");

  for (Int_t i =0; i < 50; i++) // i < max event number. Need to obtain this. Since this loop will take a long time, might need to think of a way to make search faster. Might require less thouroughness (checking every 3rd or 5th event?).
	{
       
         event_cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d",spill,i);
         h4->Draw("WF_val:WF_time >> gr", event_cut.Data() ,"colz");
	 Double_t* ydata = h4->GetV1();
	 local_event_max = *max_element(ydata,ydata+400); // Event with broadest amplitude range (most likely, since this would contain maximum value.)	
 	 printf("Event %d max amplitude = %d\n",i,local_event_max);

		}	 

	}
*/

// Loop over each good spill. Maybe make separate spill vector then 
// loop over elements. inside loop, specify event cut with spill == spill number, obtained from vector.
//  for(vector<int>::iterator it = spills.begin();)

  for (Int_t i =0; i < 50; i++) // i < max spill number. Need to obtain this.
	{

         event_cut.Form("WF_ch == XTAL_C3 && spill == %d",i);
         h4->Draw("WF_val:WF_time >> gr", cut.Data() ,"goff"); // goff = graphics off, colz = colors
	 // gr->SetMaximum(0); // Sets maximum to 0
	 Double_t* ydata = h4->GetV1();
	 Int_t range = h4->GetSelectedRows();       
	 local_spill_max = *max_element(ydata,ydata+range);	 
 	 printf("Spill %d maximum WF_val = %d\n",i,local_spill_max);

	 if (local_spill_max > 100) // Set threshold for spill's max WF_val. If spill's max value is less than this, spill number not saved. 
		{
		vector <int> NewColumn;
		good_spills_runs.push_back(NewColumn); // Create spill vector
		good_spills_runs.at(good_spills_runs.size()-1).push_back(i); // Enter spill number as last (currently also first) element of new spill vector. New spill vector pushed back. 0th spill vector (row) is first good spill.

		}


	}  


 
}
