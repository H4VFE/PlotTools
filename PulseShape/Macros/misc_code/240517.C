{

  // http://www.cplusplus.com/reference/algorithm/max_element/

  TCanvas *c1 = new TCanvas("c1","Canvas Title",200,10,700,500);

  file = TFile::Open("Data_Files/analysis_5196.root");
  // TTree *h4;
  // h4 = (TTree*) file->Get("h4");
  h4->Draw("WF_val:WF_time","WF_ch == XTAL_C3 && spill == 4 && event == 9", "colz"); // Need to draw before using GetV1(), GetV2

  Double_t* xdata = h4->GetV2(); // Storing x data in xdata[] with pointer *xdata
  Double_t* ydata = h4->GetV1(); // Storing y data in ydata[] array with pointer *ydata

  printf("Largest element in ydata between 0 to 400 = %f\n",*max_element(ydata,ydata+400)); 

}
