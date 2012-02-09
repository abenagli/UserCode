void chopPileup(const std::string& inFileName)
{
  TFile* f = TFile::Open(inFileName.c_str(),"READ");
  TFile* o = TFile::Open("pileup_chopped.root","RECREATE");
  
  TH1F* pileup = (TH1F*)( f->Get("pileup") );
  TH1F* pileup_chopped = (TH1F*)( pileup->Clone() );
  
  for(int bin = 1; bin <= pileup->GetNbinsX(); ++bin)
  {
    if( pileup_chopped->GetBinContent(bin) >= 1. )   pileup_chopped -> SetBinContent(bin,pileup->GetBinContent(bin));
    if( pileup_chopped->GetBinContent(bin) < 0.001 ) pileup_chopped -> SetBinContent(bin,0.);
    if( bin == pileup->GetNbinsX() )                 pileup_chopped -> SetBinContent(bin,0.);
  }
  
  o -> cd();
  pileup_chopped -> Write();
  o -> Close();
}
