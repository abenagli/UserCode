#include <iomanip>



void drawPileup(const std::string& fileName)
{
  TFile* f = TFile::Open(((fileName)+".root").c_str(),"READ");
  TH1F* h = (TH1F*)( f->Get("pileup") );
  
  TCanvas* c = new TCanvas("c","pileup");
  c -> cd();
  c -> SetGridx();
  c -> SetGridy();
  
  h -> GetXaxis() -> SetTitle("n_{PU}");
  h -> GetXaxis() -> SetLabelSize(0.04);
  h -> GetYaxis() -> SetLabelSize(0.04);
  h -> GetYaxis() -> SetTitle("event fraction");
  h -> Scale(1./h->Integral());
  h -> SetMarkerStyle(20);
  h -> Draw("P");
  
  c -> Update();
  
  for(int bin = 1; bin <= h->GetNbinsX(); ++bin)
  {
    if( int(h->GetBinCenter(bin)) < 10 )
      std::cout << "distrPU_DATA[" << int(h->GetBinCenter(bin)) <<"]  = ";
    else
      std::cout << "distrPU_DATA[" << int(h->GetBinCenter(bin)) <<"] = ";
    std::cout << std::fixed << std::setprecision(9) << std::setw(11) << h -> GetBinContent(bin);
    std::cout << ";" << std::endl;
  }
  
  
  
  TFile* o = new TFile(((fileName)+"_plot.root").c_str(),"RECREATE");
  o -> cd();
  
  h -> Write();
  
  o -> Close();
}
