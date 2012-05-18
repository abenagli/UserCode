void plotHiggsMass(const std::string& inFileName)
{
  TFile* inFile = TFile::Open(inFileName.c_str(),"READ");
  TTree* t;
  
  std::string cut = "(WJJ_m > 65. && WJJ_m < 95.)";
  TCanvas* c1 = new TCanvas("c1","Higgs mass");
  c1 -> cd();
  c1 -> SetGridx();
  c1 -> SetGridy();
  
  float xMin = 100;
  float xMax = 600;
  int nBins = 100;
  
  TH1F* h = new TH1F("h","",nBins,xMin,xMax);
  h -> GetXaxis() -> SetTitle("m(l#nujj)   (GeV/c^{2})");
  h -> GetYaxis() -> SetTitle("event fraction");
  h -> SetFillStyle(3001);
  h -> SetFillColor(41);
  h -> SetLineColor(41);
  h -> SetLineStyle(1);
  h -> SetLineWidth(2);
  t = (TTree*)(inFile->Get("ntu_16"));
  t -> Draw("lepNuW_m >>+ h",cut.c_str());
  //h -> Scale(1./h->GetEntries());
  
  
  
  TH1F* h_KF1 = new TH1F("h_KF1","",nBins,xMin,xMax);
  h_KF1 -> GetXaxis() -> SetTitle("m(l#nujj)   (GeV/c^{2})");
  h_KF1 -> GetYaxis() -> SetTitle("event fraction");
  h_KF1 -> SetLineColor(kRed);
  h_KF1 -> SetLineStyle(2);
  h_KF1 -> SetLineWidth(2);
  t = (TTree*)(inFile->Get("ntu_16"));
  t -> Draw("lepNuW_m_KF >>+ h_KF1",cut.c_str());
  //h_KF1 -> Scale(1./h_KF->GetEntries());  
  
  TH1F* h_KF2 = new TH1F("h_KF2","",nBins,xMin,xMax);
  h_KF2 -> GetXaxis() -> SetTitle("m(l#nujj)   (GeV/c^{2})");
  h_KF2 -> GetYaxis() -> SetTitle("event fraction");
  h_KF2 -> SetLineColor(kBlue);
  h_KF2 -> SetLineStyle(2);
  h_KF2 -> SetLineWidth(2);
  t = (TTree*)(inFile->Get("ntu_17"));
  t -> Draw("lepNuW_m_KF >>+ h_KF2",cut.c_str());
  //h_KF2 -> Scale(1./h_KF->GetEntries());  
  
  
  
  TH1F* h_KF3 = new TH1F("h_KF3","",nBins,xMin,xMax);
  h_KF3 -> GetXaxis() -> SetTitle("m(l#nujj)   (GeV/c^{2})");
  h_KF3 -> GetYaxis() -> SetTitle("event fraction");
  h_KF3 -> SetLineColor(kRed);
  h_KF3 -> SetLineStyle(1);
  h_KF3 -> SetLineWidth(2);
  t = (TTree*)(inFile->Get("ntu_18"));
  t -> Draw("lepNuW_m_KF >>+ h_KF3",cut.c_str());
  //h_KF3 -> Scale(1./h_KF->GetEntries());  
  
  TH1F* h_KF4 = new TH1F("h_KF4","",nBins,xMin,xMax);
  h_KF4 -> GetXaxis() -> SetTitle("m(l#nujj)   (GeV/c^{2})");
  h_KF4 -> GetYaxis() -> SetTitle("event fraction");
  h_KF4 -> SetLineColor(kBlue);
  h_KF4 -> SetLineStyle(1);
  h_KF4 -> SetLineWidth(2);
  t = (TTree*)(inFile->Get("ntu_19"));
  t -> Draw("lepNuW_m_KF >>+ h_KF4",cut.c_str());
  //h_KF4 -> Scale(1./h_KF->GetEntries());  
  
  
  
  double maximum = -1;
  if( h->GetMaximum() > maximum ) maximum = h->GetMaximum();
  if( h_KF1->GetMaximum() > maximum ) maximum = h_KF1->GetMaximum();
  if( h_KF2->GetMaximum() > maximum ) maximum = h_KF2->GetMaximum();
  if( h_KF3->GetMaximum() > maximum ) maximum = h_KF3->GetMaximum();
  if( h_KF4->GetMaximum() > maximum ) maximum = h_KF4->GetMaximum();
  h -> SetMaximum(1.10*maximum);
  
  h -> Draw("");
  h_KF1 -> Draw("same");
  h_KF2 -> Draw("same");
  h_KF3 -> Draw("same");
  h_KF4 -> Draw("same");
  
  
  
  
  
  
  TCanvas* c2 = new TCanvas("c2","KF status");
  c2 -> cd();
  c2 -> SetGridx();
  c2 -> SetGridy();
  c2 -> SetLogy();
  
  nBins = 2;
  xMin = 0;
  xMax = 2;
  
  TH1F* h_status_KF1 = new TH1F("h_status_KF1","",nBins,xMin,xMax);
  h_status_KF1 -> GetXaxis() -> SetTitle("KF status");
  h_status_KF1 -> GetYaxis() -> SetTitle("event fraction");
  h_status_KF1 -> SetLineColor(kRed);
  h_status_KF1 -> SetLineStyle(2);
  h_status_KF1 -> SetLineWidth(2);
  t = (TTree*)(inFile->Get("ntu_16"));
  t -> Draw("status_KF >>+ h_status_KF1",cut.c_str());
  h_status_KF1 -> Scale(1./h_status_KF1->GetEntries());  
  
  TH1F* h_status_KF2 = new TH1F("h_status_KF2","",nBins,xMin,xMax);
  h_status_KF2 -> GetXaxis() -> SetTitle("KF status");
  h_status_KF2 -> GetYaxis() -> SetTitle("event fraction");
  h_status_KF2 -> SetLineColor(kBlue);
  h_status_KF2 -> SetLineStyle(2);
  h_status_KF2 -> SetLineWidth(2);
  t = (TTree*)(inFile->Get("ntu_17"));
  t -> Draw("status_KF >>+ h_status_KF2",cut.c_str());
  h_status_KF2 -> Scale(1./h_status_KF2->GetEntries());  
  
  
  
  TH1F* h_status_KF3 = new TH1F("h_status_KF3","",nBins,xMin,xMax);
  h_status_KF3 -> GetXaxis() -> SetTitle("KF status");
  h_status_KF3 -> GetYaxis() -> SetTitle("event fraction");
  h_status_KF3 -> SetLineColor(kRed);
  h_status_KF3 -> SetLineStyle(1);
  h_status_KF3 -> SetLineWidth(2);
  t = (TTree*)(inFile->Get("ntu_18"));
  t -> Draw("status_KF >>+ h_status_KF3",cut.c_str());
  h_status_KF3 -> Scale(1./h_status_KF3->GetEntries());  
  
  TH1F* h_status_KF4 = new TH1F("h_status_KF4","",nBins,xMin,xMax);
  h_status_KF4 -> GetXaxis() -> SetTitle("KF status");
  h_status_KF4 -> GetYaxis() -> SetTitle("event fraction");
  h_status_KF4 -> SetLineColor(kBlue);
  h_status_KF4 -> SetLineStyle(1);
  h_status_KF4 -> SetLineWidth(2);
  t = (TTree*)(inFile->Get("ntu_19"));
  t -> Draw("status_KF >>+ h_status_KF4",cut.c_str());
  h_status_KF4 -> Scale(1./h_status_KF4->GetEntries());  
  
  
  
  h_status_KF1 -> SetMinimum(0.00001);
  h_status_KF1 -> SetMaximum(1.);
  
  h_status_KF1 -> Draw("");
  h_status_KF2 -> Draw("same");
  h_status_KF3 -> Draw("same");
  h_status_KF4 -> Draw("same");
}
