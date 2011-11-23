void plotJECUncertainty(const std::string& inFileName)
{
  std::ifstream inFile((inFileName+".txt").c_str(),std::ios::in);
  
  TFile* outFile = new TFile((inFileName+".root").c_str(),"RECREATE");
  TH2F* histo = new TH2F("histo","",102,-5.1,5.1,1001,-0.5,1000.5);
  
  double eta;
  double pt;
  double unc;
  
  while(1)
  {
    inFile >> eta >> pt >> unc;
    if( !inFile.good() ) break;
    
    histo -> Fill(eta,pt,unc);
  }
  
  histo -> Draw("COLZ");
  histo -> Write();
  outFile -> Close();
}
