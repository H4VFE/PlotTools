{
  
  // The purpose of this macro is the compare average waveform plots from different crystals.
  // Want to plot multiple Average waveforms on same plot, 

  ifstream file1("Text_Files/5522_XTAL_C3_Data.txt");
  ifstream file2("Text_Files/5360_XTAL_C2_Data.txt");

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
  TMultiGraph *mg = new TMultiGraph("mg","Average Waveforms");
  TGraph *gr1 = new TGraph(400,xarray1,yarray1);
  gr1->SetName("gr1");
  gr1->SetLineColor(4);      
  TGraph *gr2 = new TGraph(400,xarray2,yarray2);
  gr2->SetName("gr2");
  gr2->SetLineColor(6);  

  mg->Add(gr1);
  mg->Add(gr2);
  mg->Draw("AL");
  mg->GetXaxis()->SetTitle("Centered Time (ns)");
  mg->GetYaxis()->SetTitle("Averaged, Normalized Amplitude");
  gPad->Modified();

  TLegend *leg = new TLegend(0.7,0.7,0.9,0.9); // (x1, y1, x2, y2) new TLegend(0.1,0.7,0.2,0.8)
  // leg->SetHeader("Legend","C"); // C centers legend header  
  leg->AddEntry("gr1","C3","l"); // Options: L P F E
  leg->AddEntry("gr2","C2","l");
  // gStyle->SetLegendBorderSize(3);
  leg->Draw();

  c1->SaveAs("C3_C2_Comparison.root"); 

}
