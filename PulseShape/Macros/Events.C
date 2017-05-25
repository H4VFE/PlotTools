{
  /* The purpose of this macro is to take an analysis_*.root file and obtain the 'good' events and spills, where goodness is defined by some threshold of minimum peak WF_val. This should return events that are unlikely to be noise. The data from these events can then be normalized, averaged and plotted.

Abe Tishelman-Charny
24-5-17
   */

  // Rename file Event_Filter.C or Filter.C ?

  TString run; // Can change to user input run if desired
  run.Form("Data_Files/analysis_5196.root"); // Testing with run 5196 
  TFile *file = TFile::Open(run.Data()); // Open data file.
 
  h4 = (TTree*) file->Get("h4"); // Get h4 tree from current opened file
  
  // TCanvas *c1 = new TCanvas("c1","Plot",200,20,1000,715); // First argument is what -> used with. Last four arguments: wtopx, wtopy, ww, wh

  TGraph *gr = new TGraph();
  TGraph *gr2 = new TGraph(); // For testing plots

  //int spill_dimension = 1;
  //int event_dimension = 1; 

  vector< vector < int > > good_spills_events;//(spill_dimension, vector<int>(event_dimension)); // Vector of spill vectors, each spill vector contains good event numbers. First element of each spill vector is spill number. 

  vector <int> spills; // Vector with just spill numbers

  // Find spills that likely has clear pulse shapes
  TString cut;
  Int_t spill = 0;
  Int_t spill_max = 0; 
 
  //  h4->Draw("WF_val >> gr", "WF_ch == XTAL_C3 && spill == 4");

  for (Int_t i =0; i < 5; i++) // i < max spill number. Need to obtain this.
	{

         cut.Form("WF_ch == XTAL_C3 && spill == %d",i);
	 // c1->MakeDefCanvas();
 	 gr->Set(0); // Set number of points to zero.
         h4->Draw("WF_val:WF_time >> gr", cut.Data(), "goff"); // goff = graphics off, colz = colors
	 // gr->SetMaximum(0); // Sets maximum to 0
	 Double_t* ydata = h4->GetV1();
	 Int_t range = h4->GetSelectedRows();       
	 spill_max = *max_element(ydata,ydata+range); // Max WF_val range over range of data	 
 	 printf("Spill %d maximum WF_val = %d\n",i,spill_max);

	 if (spill_max > 100) // Set threshold for spill's max WF_val. If spill's max value is less than this, spill number not saved. 
		{
		vector <int> Spill_Vector;
		good_spills_events.push_back(Spill_Vector); // Create spill vector
		good_spills_events.at(good_spills_events.size()-1).push_back(i); // Enter spill number as last (currently also first) element of new spill vector. New spill vector pushed back. 0th spill vector is first good spill.
		spills.push_back(i);

		}

	}  

  // Print vector elements.
  for (int i = 0; i < good_spills_events.size(); i++)
	{
    	for (int j = 0; j < good_spills_events[i].size(); j++)
   		{
       	        cout << "good_spills_events[" << i << "][" << j << "] = "  << good_spills_events[i][j] << endl;
	        }
	}

  
  for (int i = 0; i < spills.size(); i++)
	{

        cout << "spills[" << i << "] = "  << spills[i] << endl;

	}


  // h4->Draw("WF_val:WF_time >> gr2", "WF_ch == XTAL_C3 && spill == 0");

  TString event_cut;
  Int_t event = 0;
  Int_t event_max = 0; 

  // Loop over all good spills
  // Probably need faster way to scan events for non zero max amplitude.  // Alternative is check every event which will take very long time (> 1 hour ? )

 
  for (vector<int>::size_type i = 0; i != spills.size(); i++) // Checking each good spill for good events
	{
	
	// cout << "spills ["<< i << "] =  " << spills[i] << endl;
 
  	for (Int_t j =0; j < 10; j++) // i < max event number. Need to obtain this.
		{
		// cout << "i = " << i << endl;
		// cout << "spills[" << i << "] = " << spills[i] << endl;
         	event_cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d",spills[i],j);
		
		// Alternative way to clear graph
		/*for (Int_t k = 0; k < h4->GetSelectedRows(); k++)
			{

			gr->SetPoint(k,0,0);

			}
		*/
 		gr->Set(0); // Set number of points to zero.
	        // c1->MakeDefCanvas();
         	h4->Draw("WF_val:WF_time >> gr", event_cut.Data(), "goff"); // goff = graphics off, colz = colors
	 	Double_t* eventydata = h4->GetV1();
		Int_t range = h4->GetSelectedRows();       
		event_max = *max_element(eventydata,eventydata+range);	 
 	 	printf("Spill %d event %d maximum WF_val = %d\n",spills[i],j,event_max);

	 	if (event_max > 50) // Set threshold for event's max WF_val. If max value is less than this, event number not saved. 
			{

			good_spills_events.at(i).push_back(j); // Enter good event numbers for spill number

			}

		}  

	} 

 //
 //
 //
 // Might be easier for data analysis if there is a separate event vector corresponding to each element in the spill vector. That way you can loop over spills and events more easily, but need to match event vectors with correct spill elements.
 //
 //
 //

  // Print final vector

  for (int i = 0; i < good_spills_events.size(); i++)
        {

        for (int j = 0; j < good_spills_events[i].size(); j++)
  		{

	      cout << "good_spills_events[" << i << "][" << j << "] = "  << good_spills_events[i][j] << endl;

	        }
        }

}
