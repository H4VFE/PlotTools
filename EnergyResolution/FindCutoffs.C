/*

Calculates the amplitude cutoff values based on the
1D histrograms created by 1DAmpDist.C

These values were then inserted manually into both
QuickRatio.C and AmpRatio.C

Will Benoit
December 8th 2016

*/

{
  
  TFile *file;
  TH1F *hist;
  vector<TString> histnames; //List of histogram names contained in the root file
  histnames.push_back("Center 1 - 5305"); histnames.push_back("Center 2 - 5305"); histnames.push_back("North 1 - 5270"); histnames.push_back("North 2 - 5270"); histnames.push_back("East 1 - 5307"); 
  histnames.push_back("East 2 - 5307"); histnames.push_back("South 1 - 5308"); histnames.push_back("South 2 - 5308"); histnames.push_back("West 1 - 5309"); histnames.push_back("West 2 - 5309");
  TF1 *gaus = new TF1("gaus", "gaus", 100, 1000); //Function used for fitting the peak in the amplitude ditribution
  
  //Variable declarations
  int maxentries, mode, temp;
  float cutoff;
  
  file = TFile::Open("1DAmpDistributions.root"); //File containing histograms
  
  //Loops through all histograms
  for(auto name:histnames){
    maxentries = 0;
    hist = (TH1F*) file->Get(name.Data()); //Getting the histogram
    for(int i = 1; i < hist->GetXaxis()->GetNbins()-1; i++){ //Going through all bins, ignoring the underflow and overflow bins
      temp = hist->GetBinContent(i); //Get the number of entries in the bin
      if(temp > maxentries){
	maxentries = temp; //Track the bin with the most entries
	mode = i*hist->GetBinWidth(i); //Calculate the value of the mode based on bin number and width
      }
    }
    
    //Fit the peak with a gaussian after setting parameters and parameter limits
    gaus->SetParLimits(1, mode-150, mode+150);
    gaus->SetParLimits(2, 1, 100);
    gaus->SetParameter(1, mode);
    gaus->SetParameter(2, 50);
    hist->Fit(gaus, "QR");
    
    cutoff = gaus->GetParameter(1) - 5*gaus->GetParameter(2); //cutoff = (mean) - 5(sigma)
    cout << name << " " << cutoff << endl; //Output the cutoff value for each APD on each run
  }
}