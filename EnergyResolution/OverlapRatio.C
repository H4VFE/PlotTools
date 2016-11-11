{
  TFile *g_file = new TFile("dummy.root", "RECREATE");
  
  TChain *chain  = new TChain("h4");
  
  vector<int> runs;
  runs.push_back(5270); runs.push_back(5307); runs.push_back(5308); runs.push_back(5309); runs.push_back(5651); runs.push_back(5654); runs.push_back(5655);  runs.push_back(5659); runs.push_back(5661);
  
  TH1F *temphist1D = new TH1F("temphist1D", "temphist1D", 10000, -3000, 3000);
  TH2F *temphist2D = new TH2F("temphist2D", "temphist2D", 60, -15, 15, 1000, 0.5, 1.5);
  temphist2D->GetXaxis()->SetTitle("Position (mm)");
  temphist2D->GetYaxis()->SetTitle("Amplitude Ratio");
  TH2F *XCenterHist = new TH2F("XCenterHist", "XCenterHist", 60, -15, 15, 1000, 0.5, 1.5);
  TH2F *YCenterHist = new TH2F("YCenterHist", "YCenterHist", 60, -15, 15, 1000, 0.5, 1.5);
  TH2F *xhist = new TH2F("x", "x", 60, -15, 15, 1000, 0.5, 1.5);
  TH2F *yhist = new TH2F("y", "y", 60, -15, 15, 1000, 0.5, 1.5);
  
  XCenterHist->SetTitle("X Scan - All Center");
  XCenterHist->GetXaxis()->SetTitle("Position (mm)");
  XCenterHist->GetYaxis()->SetTitle("Amplitude Ratio");
  
  YCenterHist->SetTitle("Y Scan - All Center");
  YCenterHist->GetXaxis()->SetTitle("Position (mm)");
  YCenterHist->GetYaxis()->SetTitle("Amplitude Ratio");
  
  xhist->SetTitle("X Scan - All");
  xhist->GetXaxis()->SetTitle("Position (mm)");
  xhist->GetYaxis()->SetTitle("Amplitude Ratio");
  
  yhist->SetTitle("Y Scan - All");
  yhist->GetXaxis()->SetTitle("Position (mm)");
  yhist->GetYaxis()->SetTitle("Amplitude Ratio");
  
  TCanvas *XcenterC = new TCanvas("XcenterC", "XcenterC", 1000, 1000);
  TCanvas *XsideC = new TCanvas("XsideC", "XsideC", 1000, 1000);
  TCanvas *XallC = new TCanvas("XallC", "XallC", 1000, 1000);
  
  XcenterC->Divide(2,3);
  XsideC->Divide(3,3);
  XallC->Divide(1,2);
  
  TCanvas *YcenterC = new TCanvas("YcenterC", "YcenterC", 1000, 1000);
  TCanvas *YsideC = new TCanvas("YsideC", "YsideC", 1000, 1000);
  TCanvas *YallC = new TCanvas("YallC", "YallC", 1000, 1000);
  
  YcenterC->Divide(2,3);
  YsideC->Divide(3,3);
  YallC->Divide(1,2);
  
  TCanvas *CenterProfiles = new TCanvas("CenterProfiles", "CenterProfiles", 1000, 1000);
  CenterProfiles->Divide(1,2);
  
  TFile *pfile = TFile::Open("IndivProjections.root", "UPDATE");
  TCanvas *Projections = new TCanvas("Projections", "Projections", 800, 800);/*(TCanvas*) pfile->Get("projections");*/
  //Projections->Divide(2,1);
  
  TTree* h4;
  TFile *f;
  TString filename, plot, cut, histname;
  float cutoff1, cutoff2, scale;
  
  XcenterC->cd(6);
  XCenterHist->Draw("AXIS");
  YcenterC->cd(6);
  YCenterHist->Draw("AXIS");

  XsideC->cd(5);
  XCenterHist->Draw("AXIS");
  YsideC->cd(5);
  YCenterHist->Draw("AXIS");

  CenterProfiles->cd(1);
  XCenterHist->Draw("AXIS");
  CenterProfiles->cd(2);
  YCenterHist->Draw("AXIS");
  
  for(auto i : runs){
    if(i == 5661) continue;
    filename.Form("./ntuples_v1/analysis_%d.root", i);
    f = TFile::Open(filename.Data());
    h4 = (TTree*) f->Get("h4");
    
    g_file->cd();
    
    h4->Draw("amp_max[XTAL_C0_APD1] >> temphist1D", "abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5", "goff");
    cutoff1 = temphist1D->GetMean() - 3*temphist1D->GetRMS();
    if(cutoff1 < 0) cutoff1 = temphist1D->GetMean();
    
    temphist1D->Reset();
    
    h4->Draw("amp_max[XTAL_C0_APD2] >> temphist1D", "abs(X[0]) < 5 && abs(X[1]) < 5 && abs(Y[0]) < 5 && abs(Y[1]) < 5", "goff");
    cutoff2 = temphist1D->GetMean() - 3*temphist1D->GetRMS();
    
    if(cutoff2 < 0) cutoff2 = temphist1D->GetMean();
    
    //cout << cutoff1 << " " << cutoff2 << endl;
    
    temphist1D->Reset();
    
    plot.Form("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):((X[0]*(run > 5600 || run == 5270 || run == 5308) + (X[0]-10)*(run == 5309) + (X[0]+10)*(run == 5307))/2) >> temphist2D");
    //cut.Form("X[0] > -200 && X[1] > -200 && abs(Y[0]) < 5 && abs(Y[1]) < 5 && amp_max[XTAL_C0_APD1] > %f && amp_max[XTAL_C0_APD2] > %f", cutoff1, cutoff2); && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1
    //cut.Form("X[0] > -200 && X[1] > -200 && abs(Y[0]) < 5 && abs(Y[1]) < 5");
    if(i == 5270 || i == 5655) cut.Form("abs(X[0]) < 6 && X[1] < 8.5 && X[1] > -3.5 && Y[0] > 0 && Y[1] > 0 && Y[0] < 10 && Y[1] < 10 && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1");
    else if(i == 5307 || i == 5659) cut.Form("abs(Y[0]) < 6 && abs(Y[1]) < 6 && X[0] < 0 && X[1] < 2.5 && X[0] > -10 && X[1] > -7.5 && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1");
    else if(i == 5308 || i == 5651) cut.Form("abs(X[0]) < 6 && X[1] < 8.5 && X[1] > -3.5 && Y[0] > -10 && Y[1] > -10 && Y[0] < 0 && Y[1] < 0 && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1");
    else if(i == 5309 || i == 5654) cut.Form("abs(Y[0]) < 6 && abs(Y[1]) < 6 && X[0] < 10 && X[1] < 12.5 && X[0] > 0 && X[1] > 2.5 && abs(X[1]-X[0]) < 3.5 && abs(Y[1]-Y[0]) < 1");
    else continue;
    
    
    h4->Draw(plot.Data(), cut.Data(), "colz goff");
    xhist->Add(temphist2D, 1);
    if(i > 5600) XCenterHist->Add(temphist2D, 1);
    
    switch (i){
      case 5270:
	XsideC->cd(2);
	temphist2D->SetTitle("X Scan - North");
	temphist2D->ProfileX("XNorthpfx");
	temphist2D->DrawClone("colz");
	XNorthpfx->SetLineColor(6);
	XNorthpfx->DrawClone("same");
	break;
      case 5307:
	XsideC->cd(6);
	temphist2D->SetTitle("X Scan - East");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XEastpfx");
	XEastpfx->SetLineColor(7);
	XEastpfx->DrawClone("same");
	break;
      case 5308:
	XsideC->cd(8);
	temphist2D->SetTitle("X Scan - South");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XSouthpfx");
	XSouthpfx->SetLineColor(8);
	XSouthpfx->DrawClone("same");
	break;
      case 5309:
	XsideC->cd(4);
	temphist2D->SetTitle("X Scan - West");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XWestpfx");
	XWestpfx->SetLineColor(9);
	XWestpfx->DrawClone("same");
	//pfile->cd();
	temphist2D->ProjectionY("XWestpy");
	XWestpy->Sumw2();
	scale = XWestpy->Integral();
	XWestpy->Scale(1/scale);
	XWestpy->SetLineColor(1);
	XWestpy->SetStats(0);
	XWestpy->Rebin(2);
	XWestpy->SetTitle("With Restriction");
	Projections->cd();
	XWestpy->Draw();
	break;
      case 5651:
	XcenterC->cd(1);
	temphist2D->SetTitle("X Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XCenter1pfx");
	XCenter1pfx->SetLineColor(1);
	XCenter1pfx->DrawClone("same");
	CenterProfiles->cd(1);
	XCenter1pfx->Draw("same");
	break;
      case 5654:
	XcenterC->cd(2);
	temphist2D->SetTitle("X Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XCenter2pfx");
	XCenter2pfx->SetLineColor(2);
	XCenter2pfx->DrawClone("same");
	CenterProfiles->cd(1);
	XCenter2pfx->Draw("same");
	break;
      case 5655:
	XcenterC->cd(3);
	temphist2D->SetTitle("X Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XCenter3pfx");
	XCenter3pfx->SetLineColor(3);
	XCenter3pfx->DrawClone("same");
	CenterProfiles->cd(1);
	XCenter3pfx->Draw("same");
	break;
      case 5659:
	XcenterC->cd(4);
	temphist2D->SetTitle("X Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XCenter4pfx");
	XCenter4pfx->SetLineColor(4);
	XCenter4pfx->DrawClone("same");
	CenterProfiles->cd(1);
	XCenter4pfx->Draw("same");
	//pfile->cd();
	temphist2D->ProjectionY("XCenterpy");
	XCenterpy->Sumw2();
	scale = XCenterpy->Integral();
	XCenterpy->Scale(1/scale);
	XCenterpy->SetStats(0);
	XCenterpy->Rebin(2);
	Projections->cd();
	XCenterpy->Draw("same");
	break;
      case 5661:
	XcenterC->cd(5);
	temphist2D->SetTitle("X Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("XCenter5pfx");
	XCenter5pfx->SetLineColor(5);
	XCenter5pfx->DrawClone("same");
	CenterProfiles->cd(1);
	XCenter5pfx->Draw("same");
	break;
    }
    
    temphist2D->Reset();
    
    plot.Form("(amp_max[XTAL_C0_APD2]/amp_max[XTAL_C0_APD1]):((Y[0]*(run > 5600 || run == 5307 || run == 5309) + (Y[0]-10)*(run == 5270) + (Y[0]+10)*(run == 5308))/2) >> temphist2D");
    //cut.Form("Y[0] > -200 && Y[1] > -200 && abs(X[0]) < 5 && X[1] < 7.5 && X[1] > -2.5 && amp_max[XTAL_C0_APD1] > %f && amp_max[XTAL_C0_APD2] > %f", cutoff1, cutoff2);
    //cut.Form("Y[0] > -200 && Y[1] > -200 && abs(X[0]) < 5 && X[1] < 7.5 && X[1] > -2.5");
    
    h4->Draw(plot.Data(), cut.Data(), "colz goff");
    yhist->Add(temphist2D, 1);
    if(i > 5600) YCenterHist->Add(temphist2D, 1);
    
    switch (i){
      case 5270:
	YsideC->cd(2);
	temphist2D->SetTitle("Y Scan - North");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YNorthpfx");
	YNorthpfx->SetLineColor(6);
	YNorthpfx->DrawClone("same");
	break;
      case 5307:
	YsideC->cd(6);
	temphist2D->SetTitle("Y Scan - East");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YEastpfx");
	YEastpfx->SetLineColor(7);
	YEastpfx->DrawClone("same");
	break;
      case 5308:
	YsideC->cd(8);
	temphist2D->SetTitle("Y Scan - South");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YSouthpfx");
	YSouthpfx->SetLineColor(8);
	YSouthpfx->DrawClone("same");
	break;
      case 5309:
	YsideC->cd(4);
	temphist2D->SetTitle("Y Scan - West");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YWestpfx");
	YWestpfx->SetLineColor(9);
	YWestpfx->DrawClone("same");
	break;
      case 5651:
	YcenterC->cd(1);
	temphist2D->SetTitle("Y Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YCenter1pfx");
	YCenter1pfx->SetLineColor(1);
	YCenter1pfx->DrawClone("same");
	CenterProfiles->cd(2);
	YCenter1pfx->Draw("same");
	break;
      case 5654:
	YcenterC->cd(2);
	temphist2D->SetTitle("Y Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YCenter2pfx");
	YCenter2pfx->SetLineColor(2);
	YCenter2pfx->DrawClone("same");
	CenterProfiles->cd(2);
	YCenter2pfx->Draw("same");
	break;
      case 5655:
	YcenterC->cd(3);
	temphist2D->SetTitle("Y Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YCenter3pfx");
	YCenter3pfx->SetLineColor(3);
	YCenter3pfx->DrawClone("same");
	CenterProfiles->cd(2);
	YCenter3pfx->Draw("same");
	break;
      case 5659:
	YcenterC->cd(4);
	temphist2D->SetTitle("Y Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YCenter4pfx");
	YCenter4pfx->SetLineColor(4);
	YCenter4pfx->DrawClone("same");
	CenterProfiles->cd(2);
	YCenter4pfx->Draw("same");
	break;
      case 5661:
	YcenterC->cd(5);
	temphist2D->SetTitle("Y Scan - Center");
	temphist2D->DrawClone("colz");
	temphist2D->ProfileX("YCenter5pfx");
	YCenter5pfx->SetLineColor(5);
	YCenter5pfx->DrawClone("same");
	CenterProfiles->cd(2);
	YCenter5pfx->Draw("same");
	break;
    }
    
    temphist2D->Reset();
  }
  
  XcenterC->cd(6);
  XCenterHist->Draw("colz");
  XCenterHist->ProfileX("XCenterAllpfx");
  XCenterAllpfx->DrawClone("same");
  XsideC->cd(5);
  XCenterHist->Draw("colz");
  XCenterAllpfx->DrawClone("same");
  
  YcenterC->cd(6);
  YCenterHist->Draw("colz");
  YCenterHist->ProfileX("YCenterAllpfx");
  YCenterAllpfx->DrawClone("same");
  YsideC->cd(5);
  YCenterHist->Draw("colz");
  YCenterAllpfx->DrawClone("same");
  
  XallC->cd(1);
  xhist->Draw("colz");
  xhist->ProfileX("xpfx");
  xpfx->Draw("same");
  XallC->cd(2);
  xhist->Draw("AXIS");
  xpfx->SetLineWidth(2);
  xpfx->Draw("same");
  XNorthpfx->DrawClone("same");
  XEastpfx->DrawClone("same");
  XSouthpfx->DrawClone("same");
  XWestpfx->DrawClone("same");
  XCenterAllpfx->DrawClone("same");
  
  YallC->cd(1);
  yhist->Draw("colz");
  yhist->ProfileX("ypfx");
  ypfx->Draw("same");
  YallC->cd(2);
  yhist->Draw("AXIS");
  ypfx->SetLineWidth(2);
  ypfx->Draw("same");
  YNorthpfx->DrawClone("same");
  YEastpfx->DrawClone("same");
  YSouthpfx->DrawClone("same");
  YWestpfx->DrawClone("same");
  YCenterAllpfx->DrawClone("same");
  

  TFile *file1 = new TFile("AmpRatio.root", "RECREATE");
  pfile->cd();
  Projections->Write("with");
  
}