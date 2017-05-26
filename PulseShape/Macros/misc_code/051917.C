{

// http://www.cplusplus.com/reference/algorithm/max_element/


/*
  vector <TString> xdata(10);
  TString name;
  for(int i = 0; i < 10; i++)
        {

        name.Form("x_%d",i);
        xdata.at(i) = name.Data();

        }

  printf("xdata element 0 = %s", xdata[0].c_str());

*/

  // TCanvas *c1 = new TCanvas("c1","Canvas Title",200,10,700,500);

  file = TFile::Open("Data_Files/analysis_5196.root");
  // TTree *h4;
  // h4 = (TTree*) file->Get("h4");
  h4->Draw("WF_val:WF_time","WF_ch == XTAL_C3 && spill == 4 && event == 9", "goff"); // Need to draw before using GetV1(), GetV2



  // Try to save event data in vectors

  // vector <Double_t*> xdata(1);
  // vector <Double_t*> ydata(1);

  // Double_t* xdata[1];
  // Double_t* ydata[1];

  // xdata = h4->GetV2();
  // ydata = h4->GetV1();

  // TGraph *gr = new TGraph(h4->GetSelectedRows(), h4->GetV2(), h4->GetV1());
  // gr->Draw();

  Double_t* xdata = h4->GetV2(); // Storing x data in xdata[] with pointer *xdata
  Double_t* ydata = h4->GetV1(); // Storing y data in ydata[] array with pointer *ydata

  /*

  for (int i = 0; i < 5; i++)
	{

	printf("xdata[%d] = %f\n", i, xdata[i]);
	printf("ydata[%d] = %f\n", i, ydata[i]);

	// cout << "*(xdata + " << i << ") : ";
        // cout << *(xdata + i) << endl;
	
	}
  */

  printf("Largest element in ydata between 0 to 5 = %f ",*max_element(ydata,ydata+100)); 

}
