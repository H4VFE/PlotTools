{
  
  TFile *file = new TFile("Data_Files/analysis_5196.root"); // Open data file.
  TTree *h4 = (TTree*) file->Get("h4"); 

  // http://stackoverflow.com/questions/10767043/convert-subselection-of-ttree-to-array-of-floats-in-cerns-root 
  //TGraph *gr = new TGraph(h4->GetSelectedRows(),y,x);
  
  // Double_t *x;
  // Double_t *y;

  // TString cut;

  // Int_t i, j;
  // TString x, y;
  // x.Form("*x_%d_%d",i,j);
  // y.Form("*y_%d_%d",i,j);
  
  // Below is a loop through the values in the vector 'runs'
  
  for(auto i : spills) // Loop through all saved spills
	{ 
	// Figure out order
	printf("i = %d\n",i);

	for(auto j : events) // 
		{




		}


	}
  // Try to setup first with averaging two events.


  h4->Draw("WF_val:WF_time", "WF_ch == XTAL_C3 && spill == 15 && event == 101", "goff");
  Double_t *x = h4->GetV2();
  Double_t *y = h4->GetV1();

  for(i == 5196 || i == 5195)

	{
	
	h4->Draw("WF_val:WF_time", "WF_ch == XTAL_C3 && spill == 
	Double_t *x_i = h4
	cout << x[i] << endl; 
	cout << y[i] << endl;	

	}


  /*


  // int sz = h4->Draw();

  // Given a run, need to go into each spill and obtain (x,y) data for each event. Loop through all spills. Then need to normalize y values, and average y values for each x, then plot final (x,y_norm_avg).  

  for (Int_t i = 0; i < 2; i++) // i spill loops
	{

	for (Int_t j = 0; j < 2; j++) // j event loops
		
		{

	 printf("Spill %d event %d\n",i,j);
 	 
	 cut.Form("WF_ch == XTAL_C3 && spill == %d && event == %d",i,j);
	 h4->Draw("WF_val:WF_time",cut.Data(),"goff");
	 //Double_t y.Data();  // WF_val data for spill i, event j
	 //Double_t x.Data();  // WF_time data for spill i, event j
	 //y.Data() = h4->GetV1();
	 //x.Data() = h4->GetV2();
	 Double_t *y_i_j = h4->GetV1();
	 Double_t *x_i_j = h4->GetV2();

		}
	}
  */
   
  // TCanvas *c1 = new TCanvas("c1","XTAL Plot",200,10,700,500);
  // h4->Draw("WF_val:WF_time","WF_ch == XTAL_C3 && spill == 1 && event == 1");
  // TGraph *gr = new TGraph(h4->GetSelectedRows(), y_1_1, x_1_1);
  // gr->Draw();
 // Test that it works
/*
  for (int i = 0; i<(h4->GetSelectedRows()), i++)
	{
  	printf("x_1_1 array value %d: %f",i,x_1_1[i]);
 	}
*/

/*  
  
  gr->Draw("ap"); //A=Axis, L=Line graph
  gr->SetTitle("Title");
  //gr->SetLineColor(kMagenta);
  //gr->SetLineWidth();
  gr->GetXaxis()->SetTitle("X axis title");
  gr->GetYaxis()->SetTitle("Y axis title");
 

*/
 
 }
