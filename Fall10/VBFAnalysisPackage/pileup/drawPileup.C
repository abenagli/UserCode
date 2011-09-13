#include <iomanip>



void drawPileup(const std::string& fileName)
{
  TFile* f = TFile::Open(fileName.c_str(),"READ");
  TH1F* h = (TH1F*)(f -> Get("pileup") );
  
  TCanvas* c = new TCanvas("c","pileup");
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  
  h -> Scale(1./h->GetEntries());
  h -> SetMarkerStyle(20);
  h -> Draw("P");
  
  c -> Update();
  
  for(int bin = 1; bin <= h->GetNbinsX(); ++bin)
  {
    std::cout << std::fixed << std::setprecision(9) << std::setw(11) << h -> GetBinContent(bin);

    if( bin < h->GetNbinsX() )
      std::cout << "," << " // " << h->GetBinCenter(bin) << std::endl;
    else
      std::cout << " " << " // " << h->GetBinCenter(bin) << std::endl;
  }
}
