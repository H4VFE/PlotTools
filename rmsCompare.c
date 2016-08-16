{
  TH1F* h1 = (TH1F*) _file0->Get("h");
  TH1F* h2 = (TH1F*) _file1->Get("h");
  
  h1->SetAxisRange(5, 9, "X");
  h2->SetLineColor(kRed);

  h1->Draw();
  h2->Draw("same");
}