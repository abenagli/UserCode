#include "PUUtils.h"



double PURescaleFactor(TH1F* distrPU_DATA, TH1F* distrPU_MC,
                       const double& nPU_MC, const int& PUScale)
{
  if(nPU_MC < 0.)
    return 1.;
  
  else if(nPU_MC > 34.)
    return 0.;
  
  else
  {
    int bin_DATA = distrPU_DATA -> FindBin(nPU_MC);
    int bin_MC   = distrPU_MC   -> FindBin(nPU_MC);
    
    //std::cout << "   distrPU_DATA[" << std::setw(2) << bin_DATA << "] = " << std::fixed << std::setprecision(6) << std::setw(8) << distrPU_DATA -> GetBinContent(bin_DATA)
    //          << "   distrPU_MC["   << std::setw(2) << bin_MC << "] = "   << std::fixed << std::setprecision(6) << std::setw(8) << distrPU_MC   -> GetBinContent(bin_MC)
    //          << "   ratio: " << 1. * distrPU_DATA->GetBinContent(bin_DATA) / distrPU_MC->GetBinContent(bin_MC)
    //          << std::endl;
    
    if( PUScale == 0 )
      return 1. * distrPU_DATA->GetBinContent(bin_DATA) / distrPU_MC->GetBinContent(bin_MC);
    
    else
      return 0.;
  }
}

//  ------------------------------------------------------------
