{
  
  // The purpose of this macro is the compare average waveform plots from different crystals.
  // Want to plot multiple Average waveforms on same plot, 

  ifstream file1("Text_Files/5196_XTAL_C3_Data.txt");
  ifstream file2("Text_Files/name.txt");

  //string xarray_string[400];
  //string yarray_string[400];

  double xarray1[400];
  double yarray1[400];

  double xarray2[400];
  double yarray2[400];

  // skip four lines (header)

  file1.ignore(500,'\n'); // Stop ignoring after 500 characters, or an '\n'
  file1.ignore(500,'\n');
  file1.ignore(500,'\n');
  file1.ignore(500,'\n');
  
  file2.ignore(500,'\n');
  file2.ignore(500,'\n');
  file2.ignore(500,'\n');
  file2.ignore(500,'\n');

  for (int i = 0; i < 400; i++)
  	{
  
	file1 >> xarray1[i];
	file1 >> yarray1[i];

	file2 >> xarray2[i];
	file2 >> yarray2[i];

	//cout << "xarray[ " << i << " ] = " << xarray[i] << endl;
	//cout << "yarray[ " << i << " ] = " << yarray[i] << endl;

  	//getline(file,xarray_string[i]," "); // " " deliminator
  	//getline(file,yarray_string[i]); // Don't think this needs deliminator since end of line

	//xarray[i] = stod(xarray_string[i]);
	//yarray[i] = stod(yarray_string[i]);

  	}

  file1.close();
  file2.close();

  // Plot average waveforms on same multigraph
  TCanvas *c1 = new TCanvas("c1","Average Wave", 600, 800); // width, height
  TMultigraph *mg = new TMultigraph("mg","Average Waveforms");
  TGraph *gr1 = new TGraph(400,xarray1,yarray1);      
  TGraph *gr2 = new TGraph(400,xarray2,yarray2);
  
  mg->Add(gr1);
  mg->Add(gr2);
  mg->Draw("AL");
  mg->GetXaxis()->SetTitle("Centered Time (ns)");
  mg->GetYaxis()->SetTitle("Averaged, Normalized Amplitude");
  gPad->Modified();
}
