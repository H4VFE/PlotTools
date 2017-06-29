{
  /* The purpose of this macro is to take an analysis_*.root file from /eos/cms/store/group/dpg_ecal/comm_ecal/upgrade/testbeam/ECALTB_H4_Jul2016/ntuples_V1/ntuples/, created by the H4Analysis repository: https://github.com/simonepigazzini/H4Analysis ,  and obtain the 'good' events and spills, where goodness is defined by a user input threshold of peak WF_val. This should return events that are unlikely to be noise. The data from these events can then be normalized, averaged and plotted.

Abe Tishelman-Charny
24-5-17
   */

  // Prompt user for run number (maybe later make terminal argument)
  int run_num, max_spills, max_events;
  char column;
  int row;
  int event_thres; // Threshold in ADC counts

  cout << "Please enter a run number: " << endl;
  cin >> run_num;
  cout << "Run number = " << run_num << endl;

  cout << "Please enter a crystal column (ex: A, B, ...) : " << endl;
  cin >> column;
  cout << "Column: " << column << endl;

  cout << "Please enter a row (ex: 1, 2, ...) : " << endl;
  cin >> row;
  cout << "Row: " << row << endl;
  
  cout << "Please enter max number of spills to scan: " << endl;
  cin >> max_spills;
  cout << "Max spills = " << max_spills << endl;

  cout << "Please enter max number of events to scan per spill: " << endl;
  cin >> max_events;
  cout << "Max events = " << max_events << endl;
  
  cout << "Please enter a WF_val threshold: " << endl;
  cin >> event_thres;
  cout << "event_thres = " << event_thres << endl;

  // Open File, obtain tree
  TString run; 
  run.Form("Data_Files/analysis_%d.root",run_num);  
  TFile *file = TFile::Open(run.Data()); // Open data file.
  h4 = (TTree*) file->Get("h4"); // Get h4 tree from current opened file

  // Initializing vector of spill vectors, each spill vector contains good event numbers. First element of each spill vector is spill number. 
  vector< vector < int > > good_spills_events;   
  
  // Vector with just spill numbers. Use in second loop to find good events for each spill.
  vector <int> spills;

  // Initializing some more variables
  TString cut;
  TString channel;
  TString event_cut;
  channel.Form("XTAL_%c%d",column,row); // ex: "XTAL_C3"
  Int_t spill = 0;
  Int_t spill_max = 0; 

  // Graph for drawing so data can be obtained
  TGraph *gr = new TGraph();

  // Find good spills 
  for (Int_t i =0; i < max_spills; i++) // i < max spill number. Can be user set or generalized to max spill number.
	{

         cut.Form("WF_ch == %s && spill == %d",channel.Data(),i);
 	 gr->Set(0); // Set number of points to zero.
         h4->Draw("WF_val:WF_time >> gr", cut.Data(), "goff"); // goff = graphics off, colz = colors
	 // gr->SetMaximum(0); // Sets maximum to 0
	 Double_t* ydata = h4->GetV1();
	 Int_t range = h4->GetSelectedRows();       
	 spill_max = *max_element(ydata,ydata+range); // Max WF_val range over range of data	 
 	 printf("Spill %d maximum WF_val = %d\n",i,spill_max);

	 if (spill_max > event_thres) // Set threshold for spill's max WF_val. If spill's max value is less than this, spill number not saved. 
		{

		vector <int> Spill_Vector;
		good_spills_events.push_back(Spill_Vector); // Create spill vector
		good_spills_events.at(good_spills_events.size()-1).push_back(i); // Enter spill number as last (currently also first) element of new spill vector. New spill vector pushed back. 0th spill vector is first good spill.
		spills.push_back(i); // Add good spill number to spills vector for later finding events

		}

	}  

  Int_t event_max = 0; 
  bool first_good = false; // True when first good event is found.
 
  // Find good events by scanning good spills
  for (vector<int>::size_type i = 0; i != spills.size(); i++) 
	{

	first_good = false;
 
  	for (Int_t j =0; j < max_events; j++) // i < max event number. Need to obtain this.
		{

         	event_cut.Form("WF_ch == %s && spill == %d && event == %d",channel.Data(),spills[i],j);
		
		// Set number of points to zero.
		// Need to do this because when iteration is on empty event, nothing is overwritten on gr, and without clearing the graph this will associate data with an empty event.  
 		gr->Set(0);
 		
		// Alternative way to clear graph. Might be better, not sure.
		/*for (Int_t k = 0; k < h4->GetSelectedRows(); k++)
			{

			gr->SetPoint(k,0,0);

			}
		*/
         	
		h4->Draw("WF_val:WF_time >> gr", event_cut.Data(), "goff"); // goff = graphics off, colz = colors
	 	Double_t* eventydata = h4->GetV1();
		Int_t range = h4->GetSelectedRows();       
		event_max = *max_element(eventydata,eventydata+range);	 
 	 	printf("Spill %d event %d maximum WF_val = %d\n",spills[i],j,event_max);

		if (event_max > 100)
			{

			first_good = true;

			}

		// Save event number if it exceeds threshold
	 	if (event_max > event_thres)  
			{
			good_spills_events.at(i).push_back(j); 					
			}

		// add 49, another one added at next loop iteration. Do this because know events should be spaced by 50.
		// This should be generalized since the spacing between events may be different in the future.
		if (first_good) 
			{

			j += 49;
 
			}
		}  
	} 

  // Find number of events
  int num_events = 0;

  for (int i = 0; i < good_spills_events.size(); i++)
        {

        for (int j = 1; j < good_spills_events[i].size(); j++) // start at j=1 so not to count spill numbers
                {

                num_events += 1;

                }

        }

  cout << endl;
  cout << "Number of events = " << num_events << endl;
  cout << endl;

  // Save good events in text file
  ofstream Event_File;
  TString File_Name;
  seconds = time(0);
  File_Name.Form("Text_Files/%d_%c%d_Good_Events_%d.txt",run_num,column,row,(int)seconds); // Save with time in seconds since 1/1/1970 so not to overwrite any files. Can change names and relocate later.
  Event_File.open(File_Name.Data());
  Event_File << "Run: " << run_num << "\n";
  Event_File << "Number of Good Events: " << num_events << endl; 
  Event_File << "Spills scanned: " << max_spills << "\n";
  Event_File << "Events scanned per Spill: " << max_events << endl;
  Event_File << "XTAL: " << column << "" << row << endl;
  // Event_File << "Row: " << row << endl;
  // Event_File << "Column: " << column << endl;
  Event_File << "Event Threshold = " << event_thres << "\n\n"; 
  Event_File << "Top Value is Spill Number, Subsequent Values are Event Numbers. Spills separated by ";
  
  for (int i = 0; i < good_spills_events.size(); i++)
        {
	
	Event_File << "-\n";
	Event_File << good_spills_events[i][0] << "\n"; 
        
	for (int j = 1; j < good_spills_events[i].size(); j++) // Start at j = 1 b/c j = 0 is spill number
  		{

		Event_File << good_spills_events[i][j] << "\n"; 

	        }
        }

  Event_File << "End\n";
  Event_File.close(); 
 
}
