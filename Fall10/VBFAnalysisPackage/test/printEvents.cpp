#include <iostream>
#include <string>

#include "TH1F.h"
#include "TFile.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> printEvents::usage: " << argv[0] << " rootFileName" << std::endl ;
    return 1;
  }
  
  
  
  TFile* f = TFile::Open(argv[1],"READ");
  TH1F* events = (TH1F*)f->Get("events");
  
  for(int bin = 1; bin <= events->GetNbinsX(); ++bin)
  {
    std::cout << events->GetXaxis()->GetBinLabel(bin) << "   : "  << events->GetBinContent(bin) << std::endl;
  }
  
  
  
  return 0;
}
