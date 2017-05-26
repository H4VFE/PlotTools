{
  /* The purpose of this macro is to start with spill/event vector(s) containing 'good' (determined by set threshold) events, normalize data, average it, then plot average waveform on same plot as all event waveforms that make up average. Next step is to compare Russian and Chinese average waveforms qualitatively, and quantitatively.

  25-5-17
  Abe Tishelman-Charny

  */ 
  
  // Some links I used
  //
  // https://root.cern.ch/root/roottalk/roottalk03/4741.html
  // http://www.cplusplus.com/reference/algorithm/max_element/
  // http://www.cartotalk.com/index.php?showtopic=2943

  //-------------------------If good_spills_events previously defined, comment below

  /*

  // Access tree
  file = TFile::Open("Data_Files/analysis_5196.root");
  TTree *h4;
  h4 = (TTree*) file->Get("h4");

  // Hardcode vector with good events
  vector < vector <int>  > good_spills_events;

  for (int i = 0; i < 5; i++ )
        {
        
        vector <int> spillvector;
        good_spills_events.push_back(spillvector);

        } 
  
  good_spills_events[0].push_back(4);
  good_spills_events[0].push_back(9);
  good_spills_events[1].push_back(6);
  good_spills_events[1].push_back(15);
  good_spills_events[2].push_back(9);
  good_spills_events[2].push_back(21);
  good_spills_events[3].push_back(11);
  good_spills_events[3].push_back(39);
  good_spills_events[4].push_back(14);
  good_spills_events[4].push_back(29);
  
  TGraph *gr = new TGraph(); 

  */

  //----------------------------------If good_spills_events previously defined, comment above
  
  // Find number of events
  Int_t events = 0;
  
  for (int i = 0; i < good_spills_events.size(); i++)
	{

	for (int j = 1; j < good_spills_events[i].size(); j++) // start at j=1 so not to count spill numbers
		{

		events += 1;	

		}

	}
  cout << endl; 
  cout << "Number of events = " << events << endl;
  cout << endl;

  // int *g[];
  // size_t size;

  Double_t yavg[1024] = {0}; // Currently hardcoding knowing 1024 y values per event. Should eventually dynamically allocate size.
  Double_t denom = 0; // Number of good, centered events
  Int_t event_number = -1; // used for graphing on multigraph
  TString full_cut; // Used for (spill, event) cut

  // TGraph *g = new TGraph[events];
  // TGraph *g[events] = new TGraph(); // Hardcoding 10. Want *g[events] 
  
  TGraph *g[25]; // For now hardcoding many graphs. Needs to be >= number of events plotted. Should eventually be dynamically allocated
  TMultiGraph *mg = new TMultiGraph("mg","Run 5196 Waveform"); // Put all graphs here

  // Loop over first dimension entries (spills). i = spill index. [i][0] = spill number.
  for (int i = 0; i < good_spills_events.size(); i++)
          {
	  cout << "Spill " << i << " of " << good_spills_events.size()-1 << endl;
	  int spill_number = good_spills_events[i][0];         
	  	  
	  // Loop over second dimension entries (j = 0 is spill number, j > 0 are event numbers)
          for (int j = 1; j < good_spills_events[i].size(); j++)
  	  	{
		event_number += 1; // define first event as 0th event

		cout << "Event " << j << " of " << good_spills_events[i].size()-1 << endl; // -1 because first is spill information

	  	full_cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d",spill_number,good_spills_events[i][j]);		
		gr->Set(0); // Set number of points on gr to zero. (refresh)
                h4->Draw("WF_val:WF_time >> gr", full_cut.Data(), "goff"); // goff = graphics off, colz = colors
               	Double_t* ydata = h4->GetV1();

		/*   // Get x data one time
		if (i == 0 && j == 1)
			{
	
			Double_t* all_xdata = h4->GetV2(); // Assuming all events have same x data

			}
		*/			

                Int_t range = h4->GetSelectedRows(); // Number of data points       
                Int_t event_max = *max_element(ydata,ydata+range); // Max WF_val range over range of data    			

		// Normalize
		for (int k = 0; k < range ; k++)
			{

			ydata[k] = ydata[k] / event_max;

			}
		  
		// Filter off-centered waveforms
		for (int k = 512; k < range ; k++) // Start searching at halfway point (512 for 1024 points). Eventually generalize size
			{
			if (ydata[k] > 0.25) // Will skip if normalized value > 0.25 exists past halfway point. 
				{
				cout << "Off-centered wave detected" << endl;
				goto end;
				// return; // Might want to use this instead of goto end.
				}
			
			}

		// Rest of loop only runs for centered events

		// Add to multigraph		
		g[event_number] = new TGraph(range,h4->GetV2(),ydata); // event_number is just an index, not number from root file
		mg->Add(g[event_number]);		  

		// Begin averaging process by adding y values
		for (int l = 0; l < range ; l++)
			{

			yavg[l] += ydata[l];		
	
  			}
		
		denom += 1; // Event added to average (dividing by this instead of events because want Double_t, events needs to be Int_t)
			  		  
		// On final event, complete averaging process
		if (event_number == (events-1)) // -1 to account for different starting values
  			{

			cout << endl;	
			cout << "Number of averaged events: " << denom << endl;
			cout << "Number of off-centered events: " << (events - denom) << endl;	

			for (int l = 0; l < range ; l++)
				{

				yavg[l] = yavg[l] / denom;

				}

			// Create average waveform graph
			TGraph *avg = new TGraph(range,h4->GetV2(),yavg); // assumes all events have same number of values
			avg->SetName("avg"); // To put TGraph in legend
			avg->SetLineColor(3); // Green
			avg->SetLineWidth(3); // Make stand out from singular events
			mg->Add(avg);

			}

		end: // Skip to here if non-centered event

		cout << " " << endl; // It seems something is necessary after the 'end' goto, so I put this here. In future may be cleaner to use 'return' rather than goto in off-centered wave loop.
                }
          }     

  // Create canvas
  TCanvas *c1 = new TCanvas("c1","Canvas Title",200,10,700,500);  

  // Draw multigraph
  mg->Draw("AL"); // try ("a") or ("AL")
  mg->GetXaxis()->SetTitle("Time (ns)");
  mg->GetYaxis()->SetTitle("Normalized Amplitude");
  gPad->Modified();

  // Create Legend
  TLegend *leg = new TLegend(0.7,0.7,0.9,0.9); // (x1, y1, x2, y2) new TLegend(0.1,0.7,0.2,0.8)
  // leg->SetHeader("Legend","C"); // C centers legend header  
  leg->AddEntry("avg","Average","l"); // Options: L P F E
  leg->AddEntry(g[0],"Individual Runs","l");
  leg->Draw();

  // Save plot  
  TString Plot_Title; 
  Plot_Title.Form("Images/analysis_5196_AverageWaveform.png"); // Can also be useful to save as .root file to be accessed with a macro.
  c1->SaveAs(Plot_Title.Data());

}
