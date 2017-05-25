{
  /* The purpose of this macro is to start with spill/event vector(s) containing 'good' (determined by set threshold) events, normalize data, average it, then plot average waveform on same plot as all event waveforms that make up average. Next step is to compare russian and Chinese average waveforms qualitatively, and quantitatively.

  25-5-17
  Abe Tishelman-Charny

  */ 

  // http://www.cplusplus.com/reference/algorithm/max_element/


  // file = TFile::Open("Data_Files/analysis_5196.root");
  // TTree *h4;
  // h4 = (TTree*) file->Get("h4");

  TString full_cut;

  // Hardcoding spill event vector to test graphing
   
  /*

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

  */
  
  // Number of events, need for creating graphs for each event
  Int_t events = 0;
  
  for (int i = 0; i < good_spills_events.size(); i++)
	{

	for (int j = 1; j < good_spills_events[i].size(); j++) // start at j=1 so not to count spill numbers
		{

		events += 1;	

		}

	}
  
  cout << "number of events = " << events << endl;

  // int *g[];
  // size_t size;

  TGraph *g[10]; // Hardcoding 10. Want *g[events] 
  TMultiGraph *mg = new TMultiGraph();

  int event_number = -1; // used for graphing on multigraph

  // Loop over first dimension entries (spill vectors)
  for (int i = 0; i < good_spills_events.size(); i++)
          {
		  cout << "Spill vector = " << i << endl;
		  cout << "Spill number = " << good_spills_events[i][0] << endl; // Number used in cut spill == #
		  int spill_number = good_spills_events[i][0];         
  
	  // Loop over second dimension entries (j = 0 is spill number, j > 0 are event numbers)
          for (int j = 1; j < good_spills_events[i].size(); j++)
                  {
		  event_number += 1; // define first event as 0th event

		  full_cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d",spill_number,good_spills_events[i][j]);		
		  gr->Set(0); // Set number of points on gr to zero. (refresh)
                  h4->Draw("WF_val:WF_time >> gr", full_cut.Data(), "goff"); // goff = graphics off, colz = colors
                  Double_t* ydata = h4->GetV1();

		  /*// Get x data one time
		  if (i == 0 && j == 1)
			{
	
			Double_t* all_xdata = h4->GetV2(); // Assuming all events have same x data

			}
		  */			

                  Int_t range = h4->GetSelectedRows();       
                  Int_t event_max = *max_element(ydata,ydata+range); // Max WF_val range over range of data    			

		  // Normalize
		  for (int k = 0; k < range ; k++)
			{

			ydata[k] = ydata[k] / event_max;

			}

		  // Add to multigraph		
		  g[event_number] = new TGraph(range,h4->GetV2(),ydata);
		  mg->Add(g[event_number]);		  

    		  // TString graph_name;
		  // graph_name.Form("gr%d_%d",spill_number,j); // gr(spill)_(event)
		  // TGraph graph_name.Data() = new TGraph(range,xdata,ydata); // or TGraph graph.Data() = new TGraph()
		  // mg->Add(graph_name.Data());
		
                  }
          }

  TCanvas *c1 = new TCanvas("c1","Canvas Title",200,10,700,500);
  mg->Draw("AL"); // try ("a") or ("AL")
  mg->GetXaxis()->SetTitle("Time (ns)");
  mg->GetYaxis()->SetTitle("Normalized Amplitude");
  gPad->Modified();

  /*
  
  cout << "Inside second Macro!" << endl;

  // Loop over first dimension entries
  for (int i = 0; i < good_spills_events.size(); i++)
          {
	  // Loop over second dimension entries
          for (int j = 0; j < good_spills_events[i].size(); j++)
                  {

                  cout << "good_spills_events[" << i << "][" << j << "] = "  << good_spills_events[i][j] << endl;

                  }
          }

  */
}
