{
  
  // The purpose of this macro is the compare average waveform plots from different crystals.
  // Want to plot multiple Average waveforms on same plot, 

  ifstream file1("Text_Files/5522_XTAL_C3_Data.txt");
  ifstream file2("Text_Files/5291_XTAL_C2_Data.txt");

  //string xarray_string[400];
  //string yarray_string[400];

  double xarray1[400];
  double yarray1[400];
  double ydiff1[400];

  double xarray2[400];
  double yarray2[400];
  double ydiff2[400];

  double xaverage[400];
  double yaverage[400];
 
  double ratio[400];
  double difference[400];
 
  // skip four lines (header)

  int run1;
  int run2;
  int events1;
  int events2;
  char crystal11;
  int crystal12;
  char crystal21;
  int crystal22;

  TString crystal1;
  TString crystal2;

  file1.ignore(4,'\n'); // Stop ignoring after 500 characters, or an '\n'
  file1 >> run1;
  file1.ignore(12,'\n');
  file1 >> crystal11;
  file1 >> crystal12;
  //file1.ignore(500,'\n');
  file1 >> events1; 
  file1.ignore(500,'\n');
  file1.ignore(500,'\n');
  file1.ignore(500,'\n');
  
  file2.ignore(4,'\n');
  file2 >> run2;
  file2.ignore(12,'\n');
  file2 >> crystal21;
  file2 >> crystal22;
  //file2.ignore(500,'\n');
  file2 >> events2;
  file2.ignore(500,'\n');
  file2.ignore(500,'\n');
  file2.ignore(500,'\n');

  crystal1.Form("%c%d",crystal11,crystal12);
  crystal2.Form("%c%d",crystal21,crystal22);
  
  cout << "run1 = " << run1 << endl;
  cout << "run2 = " << run2 << endl;
  cout << "events1 = " << events1 << endl;
  cout << "events2 = " << events2 << endl;
  cout << "crystal1 = " << crystal1.Data() << endl;
  cout << "crystal2 = " << crystal2.Data() << endl;

  // currently i < 400 because know number of data points in averaged files.
  for (int i = 0; i < 400; i++)
  	{
  
	file1 >> xarray1[i];
	file1 >> yarray1[i];

	file2 >> xarray2[i];
	file2 >> yarray2[i];

	xaverage[i] = ( ( xarray1[i] + xarray2[i] ) / 2.0 ); 
	yaverage[i] = ( ( yarray1[i] + yarray2[i] ) / 2.0 );

	//ydiff1[i] = ( yarray1[i] - yaverage[i] );
	//ydiff2[i] = ( yarray2[i] - yaverage[i] );

  	difference[i] = ( yarray1[i] - yarray2[i]);

  	ratio[i] = ( yarray1[i] / yarray2[i] );

	//cout << "xarray[ " << i << " ] = " << xarray[i] << endl;
	//cout << "yarray[ " << i << " ] = " << yarray[i] << endl;

  	//getline(file,xarray_string[i]," "); // " " deliminator
  	//getline(file,yarray_string[i]); // Don't think this needs deliminator since end of line

	//xarray[i] = stod(xarray_string[i]);
	//yarray[i] = stod(yarray_string[i]);

  	}

  file1.close();
  file2.close();

  TString Graph_Title;
  Graph_Title.Form("%s, Run %d, %d Events. %s, Run %d, %d Events.",crystal1.Data(),run1,events1,crystal2.Data(),run2,events2);

  // Plot average waveforms on same multigraph
  TCanvas *c1 = new TCanvas("c1","Average Wave", 600, 800); // width, height

  TMultiGraph *mg = new TMultiGraph("mg",Graph_Title.Data());

  //TGraph *gr1 = new TGraph(400,xarray1,difference);
  //TGraph *gr1 = new TGraph(400,xarray1,ydiff1);
  TGraph *gr1 = new TGraph(400,xarray1,yarray1);
  gr1->SetName("gr1");
  gr1->SetLineColor(4);      

  //TGraph *gr3 = new TGraph(400,xarray1,difference);
  
  //TGraph *gr4 = new TGraph(400,xarray1,difference);
  //TGraph *gr2 = new TGraph(400,xarray2,ydiff2);
  TGraph *gr2 = new TGraph(400,xarray2,yarray2);
  gr2->SetName("gr2");
  gr2->SetLineColor(6);  

  TGraph *gr3 = new TGraph(400,xarray1,difference);
  gr3->SetName("gr3");
  gr3->SetLineColor(1); // black
  gr3->SetLineStyle(2);

  //TGraph *avg = new TGraph(400,xaverage,yaverage);
  //avg->SetName("avg");
  //avg->SetLineColor(1);

  mg->Add(gr1);
  mg->Add(gr2);
  mg->Add(gr3);
  //mg->Add(avg);
  mg->Draw("AL");
  mg->GetXaxis()->SetTitle("Centered Time (ns)");
  //mg->GetYaxis()->SetTitle("Averaged, Normalized Amplitude");
  mg->GetYaxis()->SetTitle("Normalized Amplitude");
  //mg->GetYaxis()->SetTitle("Difference From Average in Normalized Ampltiude");
  mg->GetYaxis()->SetTitleOffset(1.3); // Increase distance between title and axis.
  gPad->Modified();

  TLegend *leg = new TLegend(0.7,0.7,0.9,0.9); // (x1, y1, x2, y2) new TLegend(0.1,0.7,0.2,0.8)
  //leg->SetHeader("Legend","C"); // C centers legend header  
  //leg->AddEntry("gr1","C3 - C2","l");
  leg->AddEntry("gr1",crystal1.Data(),"l"); // Options: L P F E
  leg->AddEntry("gr2",crystal2.Data(),"l");
  leg->AddEntry("gr3","C3-C2","l");
  //leg->AddEntry("avg","Normalized Average","l");
  // gStyle->SetLegendBorderSize(3);
  leg->Draw();

  seconds = time(0);
  TString OutTitle;
  OutTitle.Form("Root_Files/Compare_Average/%s_%s_Comparison_%d.root",crystal1.Data(),crystal2.Data(),(int)seconds);

  c1->SaveAs(OutTitle.Data()); 

}
