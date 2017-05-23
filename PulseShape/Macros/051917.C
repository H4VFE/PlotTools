{





  vector <TString> xdata(10);
  TString name;
  for(int i = 0; i < 10; i++)
        {

        name.Form("x_%d",i);
        xdata.at(i) = name.Data();

        }

  printf("xdata element 0 = %s", xdata[0].c_str());



  TCanvas *c1 = new TCanvas("c1","Canvas Title",200,10,700,500);

  file = TFile::Open("Data_Files/analysis_5196.root");
  TTree *h4;
  h4 = (TTree*) file->Get("h4");



  // Try to save event data in vectors

  vector <double> xdata(10);
  vector <double> ydata(10);

  for(int i = 0; i < 5; i++)
	{

	xdata.at(i) = h4->GetV2(i);
	


	}



}
