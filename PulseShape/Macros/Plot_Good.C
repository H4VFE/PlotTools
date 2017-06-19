{

  // The purpose of this macro is to plot average wave using good spills/events text file.

  ifstream spills_events("Text_Files/5360_C2_Good_Events.txt");

  vector < vector < int >  > good_spills_events;
  vector < vector < string > > good_spills_events_string;
  /*
  	for (int i = 0; i < 5; i++ ) // i < 5  
        	{
        
        	vector <int> spillvector;
       		good_spills_events.push_back(spillvector);

        	} 
  */  

  TString channel;
  char X_let, X_num;
  
  int run_num;
  int events;

  spills_events.ignore(5,'\n');
  spills_events >> run_num;
  spills_events.ignore(500,'\n');
  spills_events.ignore(23,'\n');
  spills_events >> events;
  spills_events.ignore(500,'\n');
  spills_events.ignore(500,'\n');
  spills_events.ignore(500,'\n');
  spills_events.ignore(6,'\n');
  spills_events >> X_let;
  spills_events >> X_num;
  spills_events.ignore(500,'\n');
  spills_events.ignore(500,'\n');
  spills_events.ignore(500,'\n');
  //spills_events.ignore(500,'\n');

  // Should now be up to first spill.

  int total_spills = 0;
  int spill_num;
  int event_num;

  string temp;
  
  string s;

  while ( getline(spills_events,s) )
	{

		spills_events >> s;
		cout << "Line = '" << s << "'\n";
		vector <string> spillvector;
		good_spills_events_string.push_back(spillvector);
		good_spills_events_string[total_spills].push_back(s);

		spills_events >> s;

		if (s == "End")
			{

			break;

			}

		else if (s == "-")
			{

			total_spills +=1;

			}		

		else 
			{
			while (s != "-")
				{
				good_spills_events_string[total_spills].push_back(s);		
				cout << "In While" << endl;		
				spills_events >> s;
				cout << "s = " << s << endl;
			
				if (s == "-")
					{
					
					total_spills +=1;

					}	
			
				if (s == "End")
					{
					goto after;
					}

				}
			}

	}
  after:
  cout << "Out of Loop." << endl;

  spills_events.close();

  for (int i = 0; i < good_spills_events_string.size(); i++)
        {
        for (int j = 0; j < good_spills_events_string[i].size(); j++)
  		{

       	        cout << "good_spills_events_string[" << i << "][" << j << "] = "  << good_spills_events_string[i][j] << endl;
	        
		}
        }
  
  for (int i = 0; i < good_spills_events_string.size(); i++)
        {
	vector <int> newspillvector;
	good_spills_events.push_back(newspillvector);	

        for (int j = 0; j < good_spills_events_string[i].size(); j++)
  		{
		good_spills_events[i].push_back(atoi(good_spills_events_string[i][j].c_str()));
       	        cout << "good_spills_events[" << i << "][" << j << "] = "  << good_spills_events[i][j] << endl;
	        
		}
        }

/*
  // Loop that reads events for each spill
  //while ( getline(spills_events,temp) )
  //while ( spills_events.good() )  
  for (int j = 0; j < 500; j++) // for now hardcoding max of 500 lines.
	{

	// Create spill vector for each spill
	vector <int> spillvector;
  	good_spills_events.push_back(spillvector);
	
	spills_events >> spill_num;
  	cout << "Spill_num = " << spill_num << endl;
	good_spills_events[total_spills].push_back(spill_num);
	spills_events.ignore(500,'\n');	
	spills_events >> temp;
	cout << "Temp = '" << temp << "'\n";
	//spills_events.ignore(500,'\n');

	if ( temp != "-")
		{
	
		spills_events >> event_num;
		cout << "event_num = " << event_num << endl;
		good_spills_events[total_spills].push_back(event_num);
		spills_events.ignore(500,'\n');
		spills_events >> temp;
		cout << "Temp = '" << temp << "'\n";
		}	

	spills_events.ignore(500,'\n');
	total_spills += 1;
	//spills_events >> spill_num;
	cout << "temp = " << temp << endl;
  	//cout << "Temp = '" << temp  << "'\n";

	if ( temp == "")
	break;
 
	}

  //spills_events.close();


  for (int i = 0; i < good_spills_events.size(); i++)
        {
        for (int j = 0; j < good_spills_events[i].size(); j++)
  		{

       	        cout << "good_spills_events[" << i << "][" << j << "] = "  << good_spills_events[i][j] << endl;
	        
		}
        } 
 */


  cout << "run_num = " << run_num << endl;
  channel.Form("XTAL_%c%c",X_let,X_num);
  cout << "channel.Data() = " << channel.Data() << endl;
  
  TGraph *gr = new TGraph(); 
 

  // Access tree
  
  TString Data_Name;
  Data_Name.Form("Data_Files/analysis_%d.root",run_num);
  file = TFile::Open(Data_Name.Data());
  TTree *h4;
  h4 = (TTree*) file->Get("h4");

  cout << endl; 
  cout << "Number of events = " << events << endl;
  cout << endl;

  Double_t denom = 0; // Number of good, centered events
  Int_t event_number = -1; // Index, not value. Used for graphing on multigraph
  TString full_cut; // Used for (spill, event) cut

  // Peak information
  Double_t x_peak = 0;
  Double_t x_peaks[150] = {0}; // array of all x values at y maximum
  Double_t weights[150] = {0};
  Int_t x_peak_index = 0;
  int avg_start_index; 

  // For filtering multipeak events
  double before = 0;
  double after = 0;
  double localmax = 0;
  int localmaxcounter = 0;

  // Create average wave x and y arrays

  // average x array
  int xrange = 400;  // Avg waveform size. Should be set to ( x axis range * 5 ) b/c 5 data points per second
  Double_t xavg[400] = {0}; // Avg waveform xdata array initialization
  xavg[0] = -20; // Avg Waveform starting value. Should be set to x axis min.

  for (int i = 1; i < xrange; i++)
	{

	xavg[i] = xavg[i-1] + 0.2; // Currently known 0.2 ns intervals between data points.

	}

  // average y array
  Double_t yavg[1024] = {0}; // Currently hardcoding knowing 1024 y values per event. Should eventually dynamically allocate size.
  
  // Create Graphs, Multigraph and Histogram
  TGraph *g[500]; // For now hardcoding many graphs. Needs to be >= number of events plotted. Should eventually be dynamically allocated
  TString Multigraph_Title;
  Multigraph_Title.Form("Run %d, Channel %s Waveform",run_num,channel.Data());
  TMultiGraph *mg = new TMultiGraph("mg",Multigraph_Title.Data()); // Put all graphs here
  TString Histo;
  Histo.Form("Run %d, Channel %s Delta t counts",run_num,channel.Data());
  TH1F *del_t = new TH1F("del_t",Histo.Data(),100,0,100); // Historam for x_peaks[]


  TCanvas *c2 = new TCanvas("c2","Histo",200,10,700,500);
 
  // Loop over first dimension entries (spills). i = spill index. [i][0] = spill number.
  for (int i = 0; i < good_spills_events.size(); i++)
          {
	  cout << "Spill " << i << " of " << good_spills_events.size()-1 << endl; // -1 b/c .size() starts from 1
	  int spill_number = good_spills_events[i][0];         
	  	  
	  // Loop over second dimension entries (j = 0 is spill number, j > 0 are event numbers)
          for (int j = 1; j < good_spills_events[i].size(); j++)
  	  	{
		event_number += 1; // define first event as 0th event

		cout << "Event " << j << " of " << good_spills_events[i].size()-1 << endl; // -1 because first is spill information

	  	full_cut.Form("WF_ch == %s && spill == %d && event == %d",channel.Data(),spill_number,good_spills_events[i][j]);		
		gr->Set(0); // Set number of points on gr to zero. (refresh)
                h4->Draw("WF_val:WF_time >> gr", full_cut.Data(), "goff"); // goff = graphics off, colz = colors

		Double_t* xdata = h4->GetV2();
               	Double_t* ydata = h4->GetV1();
                Int_t range = h4->GetSelectedRows(); // Number of data points       
                Double_t event_max = *max_element(ydata,ydata+range); // Max WF_val range over range of data    			
		
		// Normalize
		for (int k = 0; k < range ; k++)
			{

			ydata[k] = ydata[k] / event_max;
			
			// Record x position of event_max
			if (ydata[k] == 1) 
				{
				
				cout << "Index of x position at peak = " << k << endl;
				x_peak_index = k;

			        // Obtain index of first average point. Different per event because different x_peak values  
				avg_start_index = x_peak_index - 100; // Avg start index is x_peak_index - ( abs(x axis lower bound) * 5)
				
				// Set x value of y peak
				x_peak = xdata[k];
				cout << "Value of x position at peak = " << x_peak << " ns " << endl;
				del_t->Fill(x_peak);
				//x_peaks[event_number] = x_peak; // add to array of all x_peak's
				// Above line, can also push back to x_peaks vector then set xpeaks array value equal to xpeaks vector element.
	
				}
			
			}
		
		// Filter Multiple Peak waves, as these are likely result of non isolated particle event (not sure if worded correctly).
		
		for (int k = 300; k < 1024 ; k++) // start and end one off from limits to access k-1 and k+1 elements
			{

			// before = ydata[k-1];
			// after = ydata[k+1];
	
			// Start looking past where first pulse expected. Might not be the best way.

			// only filter above certain values to avoid counting local noise maxes as peaks.
			//if (ydata[k] >=0.5 && ydata[k] >= before && ydata[k] >= after)
			if (ydata[k] >= 0.5) // && ( ydata[k] - ydata[k-50] ) > 0.2 && ( ydata[k] - ydata[k+50] ) > 0.2)
				{
				
				// Might want to count local maxes by integrating peaks	
				localmaxcounter += 1;
				if (localmaxcounter >= 2)

					{

					if (event_number == ( events - 1 ) )
						{

						cout << "Final event has multiple peaks." << endl;

						goto last; // If final event has multiple peaks, go to averaging.

						}

					cout << "Event has multiple peaks." << endl;

					goto end; // If there are at least two peaks, skip event

					}				

				}

			}

		// Rest of loop only runs for single peak events (unless final event is multi-peaked, then skips to averaging)

		// Move event peak to x = 0 ns
		for (int k = 0; k < range; k++)
			{

			xdata[k] = xdata[k] - x_peak; 

			}

		// Add event to multigraph		
		g[event_number] = new TGraph(range,xdata,ydata); // event_number is just an index, not number from root file
		mg->Add(g[event_number]);

		for(int l = 0; l < xrange; l++) 

			{

			yavg[l] += ydata[avg_start_index];
			avg_start_index += 1;

  			}
		
		denom += 1; // Event added to average (dividing by this instead of events because want Double_t, events needs to be Int_t)
		
		// On final event, complete averaging process
			
		last: // Skip to here if final event is off-center
	
		if (event_number == (events-1)) // -1 to account for different starting values
  			{

			//del_t->FillN(1,x_peaks,weights,1); // add x_peaks to histogram (delta_t's) to visualize spread. Remove deviation from average
			
			del_t->Draw();
			// Maybe loop through events again after determining average delta_t, THEN add and divide for average.

			for (int event_number = 0; event_number < events; event_number++)
				{

				// use event_number for peak and plotting in loop.
				// Add event to multigraph              
				g[event_number] = new TGraph(range,xdata,ydata); // event_number is just an index, not number from root file
			        mg->Add(g[event_number]);
												
				}

			cout << endl;	
			cout << "Number of averaged events: " << denom << endl;
			cout << "Number of off-centered events: " << (events - denom) << endl;	

			seconds = time(0);
  			TString Data_File_Name;
  			Data_File_Name.Form("Text_Files/%d_%s_Data_%d.txt",run_num,channel.Data(),(int)seconds); 
			ofstream avg_file;
			avg_file.open(Data_File_Name.Data());
			
			avg_file << "Run " << run_num << ", " << "XTAL " << channel.Data() << endl;
			avg_file << denom << " Averaged Events\n\n";
			avg_file << "Time (ns)" << " " << "Normalized Amplitude\n";

			for (int l = 0; l < xrange; l++) // l < range // range may be too long or short 
				{

				yavg[l] = yavg[l] / denom;

				// save average waveform data 
				avg_file << xavg[l] << " "; // space separated values
				avg_file << yavg[l] << endl;
				}

			avg_file.close();

			// Create average waveform graph
			TGraph *avg = new TGraph(xrange,xavg,yavg); // assumes all events have same number of values
			avg->SetName("avg"); // To put TGraph in legend
			avg->SetLineColor(3); // Green
			avg->SetLineWidth(3); // Make stand out from singular events
			mg->Add(avg);

			goto outer;

			}

		end: // Skip to here if multipeak event, not final event. 

		cout << " " << endl; // It seems something is necessary after the 'end' goto, so I put this here. In future may be cleaner to use 'return' rather than goto in multipeak event loop.
                } // event loop

          } // spill loop

  outer: // Skip to here if all events read

  cout << "Finished Scanning. " << endl;

  // Create canvas
  TCanvas *c1 = new TCanvas("c1","Canvas Title",200,10,700,500);  

  // Draw multigraph
  mg->Draw("AL"); // try ("a") or ("AL")
  mg->GetXaxis()->SetRangeUser(-20,60); // -20 to 60 Manual range to view wave. Lower bound should be avg start value. 1 ns = 5 data points
  mg->GetXaxis()->SetTitle("Peak Centered Time (ns)");
  mg->GetYaxis()->SetTitle("Normalized Amplitude");
  //Multigraph_Title.Form("Run 5196, Channel %s Waveform: %d Events",channel.Data(),(int) denom);
  //mg->SetTitle(Multigraph_Title.Data());
  gPad->Modified();

  // Create Legend
  TLegend *leg = new TLegend(0.7,0.7,0.9,0.9); // (x1, y1, x2, y2) new TLegend(0.1,0.7,0.2,0.8)
  // leg->SetHeader("Legend","C"); // C centers legend header  
  leg->AddEntry("avg","Average","l"); // Options: L P F E
  leg->AddEntry(g[0],"Individual Event","l");
  // gStyle->SetLegendBorderSize(3);
  leg->Draw();
  
  // Save plot and histogram as .png and .root 
  TString Plot_Title;
  TString File_Title;
  TString Histo_Title;
  TString Histo_File_Title;
  Plot_Title.Form("Images/%d_%s_AvgWave_test.png",run_num,channel.Data()); 
  File_Title.Form("Root_Files/%d_%s_AvgWave_test.root",run_num,channel.Data());
  Histo_Title.Form("Images/%d_%s_Delta_t_test.png",run_num,channel.Data());
  Histo_File_Title.Form("Root_Files/%d_%s_Delta_t_test.root",run_num,channel.Data());
  c1->SaveAs(Plot_Title.Data());
  c1->SaveAs(File_Title.Data());
  c2->SaveAs(Histo_Title.Data()); 
  c2->SaveAs(Histo_File_Title.Data());

  // Next step may be to obtain average waveform data for each channel and plot averages on same graph to compare.


}
