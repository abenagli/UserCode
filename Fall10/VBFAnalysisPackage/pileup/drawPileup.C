#include <iomanip>



void drawPileup(const std::string& fileName)
{
  TFile* f = TFile::Open(fileName.c_str(),"READ");
  TH1F* h = (TH1F*)(f -> Get("pileup") );
  
  TCanvas* c = new TCanvas("c","pileup");
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  
  h -> GetXaxis() -> SetTitle("n_{PU}");
  h -> GetXaxis() -> SetLabelSize(0.04);
  h -> GetYaxis() -> SetLabelSize(0.04);
  h -> GetYaxis() -> SetTitle("event fraction");
  h -> Scale(1./h->GetEntries());
  h -> SetMarkerStyle(20);
  h -> Draw("P");
  
  c -> Update();
  
  for(int bin = 1; bin <= h->GetNbinsX(); ++bin)
  {
    std::cout << std::fixed << std::setprecision(9) << std::setw(11) << h -> GetBinContent(bin);

    if( bin < h->GetNbinsX() )
      std::cout << "," << " // " << std::setprecision(0) << int(h->GetBinCenter(bin)) << std::endl;
    else
      std::cout << " " << " // " << std::setprecision(0) << int(h->GetBinCenter(bin)) << std::endl;
  }
}
