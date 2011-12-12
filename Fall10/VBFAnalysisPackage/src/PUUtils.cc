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
    {
      histoFunc* templateHistoFunc = new histoFunc(distrPU_DATA);
      float n = 1.;
      
      int nBins = 35;
      float xWidth = 1.;
      float xMin = 0. - 0.5*xWidth;
      float xMax = xMin + nBins * xWidth;

      TH1F* distrPU_DATA_scaled = new TH1F("distrPU_DATA_scaled","",nBins,xMin,xMax);      
      TF1* f_scaled = new TF1("f_scaled",templateHistoFunc,xMin,xMax,3,"histoFunc");
      f_scaled -> FixParameter(0,1.);
      f_scaled -> SetParameter(1,1./(1.+PUScale*n/distrPU_DATA->GetMean()));
      f_scaled -> FixParameter(2,0.);
      
      for(float x = xMin + 0.5*xWidth; x < xMax; x+=xWidth)
	distrPU_DATA_scaled -> Fill(x,f_scaled->Eval(x));
      
      double val = 1. * distrPU_DATA_scaled->GetBinContent(bin_DATA) / distrPU_MC->GetBinContent(bin_MC);
      
      delete f_scaled;
      delete templateHistoFunc;
      delete distrPU_DATA_scaled;
      
      return val;
    }
  }
}

//  ------------------------------------------------------------
