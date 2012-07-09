#include "HiggsMassFits.h"
#include <algorithm>


void FitHiggsMass(TF1** fitFunc, const std::string& funcName, const float& xMin, const float& xMax,
                  TH1F* h, const float& mH, const std::string& fitMethod)
{
  TVirtualFitter::SetDefaultFitter("Minuit2");
  double width = GetHiggsWidth(mH);
  
  
  
  //--------------------------
  // 1st pre-fit with gaussian
  
  TF1* preFitFunc = new TF1("preFitFunc","[0]*exp(-1.*(x-[1])*(x-[1])/(2.*[2]*[2]))",mH-100,mH+100);
  
  preFitFunc -> SetParLimits(0,0.5*h->GetMaximum(),1.5*h->GetMaximum());
  preFitFunc -> SetParLimits(1,mH-50.,mH+50.);
  preFitFunc -> SetParLimits(2,0.,200.);
  
  preFitFunc -> SetParameter(0,h->GetMaximum());
  preFitFunc -> SetParameter(1,mH);
  preFitFunc -> SetParameter(2,std::max(width,5.));
  
  preFitFunc -> SetNpx(10000);
  preFitFunc -> SetLineColor(kBlack);
  preFitFunc -> SetLineWidth(2);
  
  h -> Fit("preFitFunc","RQ+");
  
  double preN     = preFitFunc -> GetParameter(0);
  double preMu    = preFitFunc -> GetParameter(1);
  double preSigma = preFitFunc -> GetParameter(2);
  
  
  
  //--------------------------
  // 2nd pre-fit with gaussian
  
  TF1* preFitFunc2 = new TF1("preFitFunc2","[0]*exp(-1.*(x-[1])*(x-[1])/(2.*[2]*[2]))",preMu-0.5*preSigma,preMu+0.5*preSigma);
  
  preFitFunc2 -> SetParLimits(0,0.5*h->GetMaximum(),1.5*h->GetMaximum());
  preFitFunc2 -> SetParLimits(1,mH-50.,mH+50.);
  preFitFunc2 -> SetParLimits(2,0.,200.);
  
  preFitFunc2 -> SetParameter(0,preN);
  preFitFunc2 -> SetParameter(1,preMu);
  preFitFunc2 -> SetParameter(2,preSigma);
  
  preFitFunc2 -> SetNpx(10000);
  preFitFunc2 -> SetLineColor(kBlack);
  preFitFunc2 -> SetLineWidth(2);
  
  h -> Fit("preFitFunc2","NRQ+");
  
  double preN2     = preFitFunc2 -> GetParameter(0);
  double preMu2    = preFitFunc2 -> GetParameter(1);
  double preSigma2 = preFitFunc2 -> GetParameter(2);
  
  
  
  //--------------------------
  // 3rd pre-fit with gaussian
  
  TF1* preFitFunc3 = new TF1("preFitFunc3","[0]*exp(-1.*(x-[1])*(x-[1])/(2.*[2]*[2]))",preMu2-preSigma2,preMu2+preSigma2);
  
  preFitFunc3 -> SetParLimits(0,0.5*h->GetMaximum(),1.5*h->GetMaximum());
  preFitFunc3 -> SetParLimits(1,mH-50.,mH+50.);
  preFitFunc3 -> SetParLimits(2,0.,200.);
  
  preFitFunc3 -> SetParameter(0,preN2);
  preFitFunc3 -> SetParameter(1,preMu2);
  preFitFunc3 -> SetParameter(2,preSigma2);
  
  preFitFunc3 -> SetNpx(10000);
  preFitFunc3 -> SetLineColor(kGreen+1);
  preFitFunc3 -> SetLineWidth(2);
  
  h -> Fit("preFitFunc3","RQ+");
  
  double preN3     = preFitFunc3 -> GetParameter(0);
  double preMu3    = preFitFunc3 -> GetParameter(1);
  double preSigma3 = preFitFunc3 -> GetParameter(2);
  
  
  
  //---------------------------------
  // 4th pre-fit with cristalBallHigh
    
  TF1* preFitFunc4 = new TF1("preFitFunc4",crystalBallLowHigh,preMu3,1000.,7);
  
  preFitFunc4 -> SetParLimits(3,0.,10.);
  preFitFunc4 -> SetParLimits(4,0.,200.);
  preFitFunc4 -> SetParLimits(5,0.,10.);
  preFitFunc4 -> SetParLimits(6,0.,200.);
  
  preFitFunc4 -> FixParameter(0,preN3);
  preFitFunc4 -> FixParameter(1,preMu3);
  preFitFunc4 -> FixParameter(2,preSigma3);
  preFitFunc4 -> SetParameter(3,1.);
  preFitFunc4 -> SetParameter(4,10.);
  preFitFunc4 -> FixParameter(5,1.);
  preFitFunc4 -> FixParameter(6,10.);
  
  preFitFunc4 -> SetParName(0,"N");
  preFitFunc4 -> SetParName(1,"#mu");
  preFitFunc4 -> SetParName(2,"#sigma");
  preFitFunc4 -> SetParName(3,"#alpha_{high}");
  preFitFunc4 -> SetParName(4,"n_{high}");
  preFitFunc4 -> SetParName(5,"#alpha_{low}");
  preFitFunc4 -> SetParName(6,"n_{low}");
  
  preFitFunc4 -> SetNpx(10000);
  preFitFunc4 -> SetLineColor(kGreen+2);
  preFitFunc4 -> SetLineWidth(2);
  
  h -> Fit("preFitFunc4","QR+");
  preFitFunc4 -> Draw("same");
  
  double preAlphaHigh = preFitFunc4 -> GetParameter(3);  
  double preNHigh = preFitFunc4 -> GetParameter(4);
  
  
  
  //--------------------------------
  // 5th pre-fit with cristalBallLow
    
  TF1* preFitFunc5 = new TF1("preFitFunc5",attenuatedCrystalBallLowHigh,0.,preMu3,9);
  
  preFitFunc5 -> SetParLimits(4,0.,10.);
  preFitFunc5 -> SetParLimits(5,0.,200.);
  preFitFunc5 -> SetParLimits(6,0.,10.);
  preFitFunc5 -> SetParLimits(7,0.,200.);
  
  preFitFunc5 -> FixParameter(0,175.);
  preFitFunc5 -> FixParameter(1,GetHiggsMassTurnOnWidth(mH));
  preFitFunc5 -> FixParameter(2,preN3);
  preFitFunc5 -> FixParameter(3,preMu3);
  preFitFunc5 -> FixParameter(4,preSigma3);
  preFitFunc5 -> FixParameter(5,1.);
  preFitFunc5 -> FixParameter(6,10.);
  preFitFunc5 -> SetParameter(7,1.);
  preFitFunc5 -> SetParameter(8,10.);
  
  preFitFunc5 -> SetParName(0,"M_{0}");
  preFitFunc5 -> SetParName(1,"#sigma_{M}");
  preFitFunc5 -> SetParName(2,"N");
  preFitFunc5 -> SetParName(3,"#mu");
  preFitFunc5 -> SetParName(4,"#sigma");
  preFitFunc5 -> SetParName(5,"#alpha_{high}");
  preFitFunc5 -> SetParName(6,"n_{high}");
  preFitFunc5 -> SetParName(7,"#alpha_{low}");
  preFitFunc5 -> SetParName(8,"n_{low}");
  
  preFitFunc5 -> SetNpx(10000);
  preFitFunc5 -> SetLineColor(kGreen+2);
  preFitFunc5 -> SetLineWidth(2);
  
  h -> Fit("preFitFunc5","QR+","",0.,preMu3);
  preFitFunc5 -> Draw("same");

  double preAlphaLow = preFitFunc5 -> GetParameter(7);
  double preNLow = preFitFunc5 -> GetParameter(8);
  
  
  
  if( fitMethod == "crystalBallLowHigh" )
  {
    //-----------------------------
    // fit with double crystal-ball
        
    (*fitFunc) = new TF1(funcName.c_str(),attenuatedCrystalBallLowHigh,0.,1000.,9);
    
    (*fitFunc) -> SetParLimits(5,0.,10.);
    (*fitFunc) -> SetParLimits(6,0.,200.);
    (*fitFunc) -> SetParLimits(7,0.,10.);
    (*fitFunc) -> SetParLimits(8,0.,200.);
    
    (*fitFunc) -> FixParameter(0,175.);
    (*fitFunc) -> FixParameter(1,GetHiggsMassTurnOnWidth(mH));
    (*fitFunc) -> FixParameter(2,preN3);
    (*fitFunc) -> FixParameter(3,preMu3);
    (*fitFunc) -> FixParameter(4,preSigma3);
    (*fitFunc) -> FixParameter(5,preAlphaHigh);
    (*fitFunc) -> FixParameter(6,preNHigh);
    (*fitFunc) -> FixParameter(7,preAlphaLow);
    (*fitFunc) -> FixParameter(8,preNLow);
    
    (*fitFunc) -> SetParName(0,"M_{0}");
    (*fitFunc) -> SetParName(1,"#sigma_{M}");
    (*fitFunc) -> SetParName(2,"N");
    (*fitFunc) -> SetParName(3,"#mu");
    (*fitFunc) -> SetParName(4,"#sigma");
    (*fitFunc) -> SetParName(5,"#alpha_{high}");
    (*fitFunc) -> SetParName(6,"n_{high}");
    (*fitFunc) -> SetParName(7,"#alpha_{low}");
    (*fitFunc) -> SetParName(8,"n_{low}");
    
    (*fitFunc) -> SetNpx(10000);
    (*fitFunc) -> SetLineColor(kRed);
    (*fitFunc) -> SetLineWidth(2);
    
    int counter = 0;
    int fitStatus = -1;
    while( counter < 5 )
    {
      TFitResultPtr fitResultPtr = h -> Fit(funcName.c_str(),"QR+","",0.,1000.);
      fitStatus = (int)(fitResultPtr);
      if( fitStatus == 0 ) break;
      ++counter;
    }
  }
  
  
  if( fitMethod == "doubleSuperGausCumCauda" )
  {
    //-----------------------------
    // fit with double superGausCumCauda
        
    (*fitFunc) = new TF1(funcName.c_str(),doubleSuperGausCumCauda,0.,1000.,5);
    
    (*fitFunc) -> SetParLimits(3,0.25,5.);
    (*fitFunc) -> SetParLimits(4,0.25,5.);
    
    (*fitFunc) -> SetParameter(0,preN3);
    (*fitFunc) -> FixParameter(1,preMu3);
    (*fitFunc) -> FixParameter(2,preSigma3);
    (*fitFunc) -> SetParameter(3,0.75);
    (*fitFunc) -> SetParameter(4,0.75);

    (*fitFunc) -> SetParName(0,"N");
    (*fitFunc) -> SetParName(1,"#mu");
    (*fitFunc) -> SetParName(2,"#sigma");
    (*fitFunc) -> SetParName(3,"#alpha_{high}");
    (*fitFunc) -> SetParName(4,"#alpha_{low}");
    
    (*fitFunc) -> SetNpx(10000);
    (*fitFunc) -> SetLineColor(kBlue);
    (*fitFunc) -> SetLineWidth(2);
    
    int counter = 0;
    int fitStatus = -1;
    while( counter < 5 )
    {
      TFitResultPtr fitResultPtr = h -> Fit(funcName.c_str(),"QR+","",0.,1000.);
      fitStatus = (int)(fitResultPtr);
      if( fitStatus == 0 ) break;
      ++counter;
    }
  }
  
  
  delete preFitFunc;
  delete preFitFunc2;
}




float GetHiggsMassTurnOnWidth(const float& mH)
{
  if     ( mH == 200. ) return 6.;
  else if( mH == 250. ) return 8.53;
  else if( mH == 300. ) return 16.4;
  else if( mH == 350. ) return 28.4;
  else if( mH == 400. ) return 21.7;
  else if( mH == 450. ) return 29.2;
  else if( mH == 500. ) return 20.9;
  else if( mH == 550. ) return 18.5;
  else if( mH == 600. ) return 16.4;
  else                  return -1.;
}






TFitResultPtr 
FitData(TF1** fitFunc, const std::string& funcName,
        TH1F* h, const float& mH, const int& step, const std::string& flavour, const std::string& additionalCuts,
        const std::string& fitMethod, const bool& fixTurnOn, 
        double fitMin, double fitMax)
{
  std::cout << ">>> HiggsMassFits::FitData::fitting " << flavour << " data for mH " << mH << " with function " << fitMethod << std::endl;
  
  
  
  TVirtualFitter::SetDefaultFitter("Minuit2");
  
  
  
  //------------------------
  // define the fit function
  
  int nPar;
  
  if( fitMethod == "attenuatedExponential" )
  {
    nPar = 4;
    (*fitFunc) = new TF1(funcName.c_str(),attenuatedExponential,180.,800.,nPar);
  }
  if( fitMethod == "attenuatedDoubleExponential" )
  {
    nPar = 6;
    (*fitFunc) = new TF1(funcName.c_str(),attenuatedDoubleExponential,180.,800.,nPar);
  }
  if( fitMethod == "attenuatedPowerLaw" )
  {
    nPar = 5;
    (*fitFunc) = new TF1(funcName.c_str(),attenuatedPowerLaw,180.,800.,nPar);
  }
  if( fitMethod == "attenuatedExpPol2order" )
  {
    nPar = 5;
    (*fitFunc) = new TF1(funcName.c_str(),attenuatedExpPol2order,180.,800.,nPar);
  }
  if( fitMethod == "attenuatedExpPol3order" )
  {
    nPar = 6;
    (*fitFunc) = new TF1(funcName.c_str(),attenuatedExpPol3order,180.,800.,nPar);
  }
  if( fitMethod == "attenuatedExpPol4order" )
  {
    nPar = 7;
    (*fitFunc) = new TF1(funcName.c_str(),attenuatedExpPol4order,180.,800.,nPar);
  }  
  
  std::vector<float> initPars(nPar);
  
  
  //-----------------------
  // set turn-on parameters
  
  bool isAttenuated = false;
  int iStart = 0;
  
  if( (fitMethod == "attenuatedExponential") ||
      (fitMethod == "attenuatedDoubleExponential") ||
      (fitMethod == "attenuatedPowerLaw") ||
      (fitMethod == "attenuatedExpPol2order") ||
      (fitMethod == "attenuatedExpPol3order") ||
      (fitMethod == "attenuatedExpPol4order") )
  {
    isAttenuated = true;
    
    GetTurnOnParameters(fitMethod,initPars[0],initPars[1],mH,step,flavour,additionalCuts);
    
    if( fixTurnOn == true )
    {
      (*fitFunc) -> FixParameter(0,initPars[0]);
      (*fitFunc) -> FixParameter(1,initPars[1]); 
    }
    else
    {
      (*fitFunc) -> SetParameter(0,initPars[0]);
      (*fitFunc) -> SetParameter(1,initPars[1]);     
    }
    
    (*fitFunc) -> SetParName(0,"mu");
    (*fitFunc) -> SetParName(1,"kT");
  }
  
  if( isAttenuated == true ) iStart = 2;
  
  
  
  //---------------------
  // set other parameters
  
  GetParameters(fitMethod,initPars,iStart,mH,step,flavour,additionalCuts);
  
  if( fitMethod == "attenuatedExponential" )
  {
    (*fitFunc) -> SetParLimits(iStart+0,0.,1000000.);
    (*fitFunc) -> SetParLimits(iStart+1,0.,0.1);
    
    (*fitFunc) -> SetParameter(iStart+0,1000.);
    (*fitFunc) -> SetParameter(iStart+1,initPars[iStart+0]);
    
    (*fitFunc) -> SetParName(iStart+0,"N");
    (*fitFunc) -> SetParName(iStart+1,"#lambda");
    
    (*fitFunc) -> SetLineColor(kRed);
  }
  
  if( fitMethod == "attenuatedDoubleExponential" )
  {
    (*fitFunc) -> SetParLimits(iStart+0,0.,1000000.);
    (*fitFunc) -> SetParLimits(iStart+1,0.,0.1);
    (*fitFunc) -> SetParLimits(iStart+2,0.,1.);
    (*fitFunc) -> SetParLimits(iStart+3,0.,0.1);
    
    (*fitFunc) -> SetParameter(iStart+0,50000.);
    (*fitFunc) -> SetParameter(iStart+1,initPars[iStart+0]);
    (*fitFunc) -> SetParameter(iStart+2,initPars[iStart+1]);
    (*fitFunc) -> SetParameter(iStart+3,initPars[iStart+2]);
    
    (*fitFunc) -> SetParName(iStart+0,"N1");
    (*fitFunc) -> SetParName(iStart+1,"#lambda1");
    (*fitFunc) -> SetParName(iStart+2,"N2");
    (*fitFunc) -> SetParName(iStart+3,"#lambda2");
    
    (*fitFunc) -> SetLineColor(kMagenta);
  }
  
  if( fitMethod == "attenuatedPowerLaw" )
  {
    (*fitFunc) -> SetParLimits(iStart+0,0.,1000000.);
    (*fitFunc) -> SetParLimits(iStart+1,0.,1000.);
    (*fitFunc) -> SetParLimits(iStart+2,0.,10000.);
    
    (*fitFunc) -> SetParameter(iStart+0,100.);
    (*fitFunc) -> SetParameter(iStart+1,initPars[iStart+0]);
    (*fitFunc) -> SetParameter(iStart+2,initPars[iStart+1]);
    
    (*fitFunc) -> SetParName(iStart+0,"N");
    (*fitFunc) -> SetParName(iStart+1,"n");
    (*fitFunc) -> SetParName(iStart+2,"a");
    
    (*fitFunc) -> SetLineColor(kCyan);
  }
  
  if( fitMethod == "attenuatedExpPol2order" )
  {
    (*fitFunc) -> SetParLimits(iStart+0,0.,1000000.);
    (*fitFunc) -> SetParLimits(iStart+1,-10.,10.);
    (*fitFunc) -> SetParLimits(iStart+2,-10.,10.);
    
    (*fitFunc) -> SetParameter(iStart+0,10000.);
    (*fitFunc) -> SetParameter(iStart+1,initPars[iStart+0]);
    (*fitFunc) -> SetParameter(iStart+2,initPars[iStart+1]);
    
    (*fitFunc) -> SetParName(iStart+0,"N");
    (*fitFunc) -> SetParName(iStart+1,"a");
    (*fitFunc) -> SetParName(iStart+2,"b");
    
    (*fitFunc) -> SetLineColor(kOrange);
  }
  
  if( fitMethod == "attenuatedExpPol3order" )
  {
    (*fitFunc) -> SetParLimits(iStart+0,0.,1000000.);
    (*fitFunc) -> SetParLimits(iStart+1,-10.,10.);
    (*fitFunc) -> SetParLimits(iStart+2,-10.,10.);
    (*fitFunc) -> SetParLimits(iStart+3,-10.,10.);
    
    (*fitFunc) -> SetParameter(iStart+0,10000.);
    (*fitFunc) -> SetParameter(iStart+1,initPars[iStart+0]);
    (*fitFunc) -> SetParameter(iStart+2,initPars[iStart+1]);
    (*fitFunc) -> SetParameter(iStart+3,initPars[iStart+2]);
    
    (*fitFunc) -> SetParName(iStart+0,"N");
    (*fitFunc) -> SetParName(iStart+1,"a");
    (*fitFunc) -> SetParName(iStart+2,"b");
    (*fitFunc) -> SetParName(iStart+3,"c");
    
    (*fitFunc) -> SetLineColor(kOrange+1);
  }
  
  if( fitMethod == "attenuatedExpPol4order" )
  {
    (*fitFunc) -> SetParLimits(iStart+0,0.,1000000.);
    (*fitFunc) -> SetParLimits(iStart+1,-10.,10.);
    (*fitFunc) -> SetParLimits(iStart+2,-10.,10.);
    (*fitFunc) -> SetParLimits(iStart+3,-10.,10.);
    (*fitFunc) -> SetParLimits(iStart+4,-10.,10.);
    
    (*fitFunc) -> SetParameter(iStart+0,10000.);
    (*fitFunc) -> SetParameter(iStart+1,initPars[iStart+0]);
    (*fitFunc) -> SetParameter(iStart+2,initPars[iStart+1]);
    (*fitFunc) -> SetParameter(iStart+3,initPars[iStart+2]);
    (*fitFunc) -> SetParameter(iStart+4,initPars[iStart+3]);
    
    (*fitFunc) -> SetParName(iStart+0,"N");
    (*fitFunc) -> SetParName(iStart+1,"a");
    (*fitFunc) -> SetParName(iStart+2,"b");
    (*fitFunc) -> SetParName(iStart+3,"c");
    (*fitFunc) -> SetParName(iStart+4,"d");
    
    (*fitFunc) -> SetLineColor(kOrange+2);
  }
  
  
  
  //------------------
  // fit the histogram
  
  (*fitFunc) -> SetNpx(10000);
  (*fitFunc) -> SetLineWidth(2);
  
  int counter = 0;
  int fitStatus = -1;
  TFitResultPtr fitResultPtr ;
  while( counter < 5 )
  {
    fitResultPtr = h -> Fit(funcName.c_str(),"NQLRS+","",fitMin,fitMax);
    fitStatus = (int)(fitResultPtr);
    if( fitStatus == 0 ) break;
    ++counter;
  }
  return fitResultPtr ;
}












void GetTurnOnParameters(const std::string& fitMethod,
                         float& mu, float& kT,
                         const float& mH, const int& step, const std::string& flavour, const std::string& additionalCuts)
{
  if( fitMethod == "attenuatedExponential" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      mu = 178.;
      kT = 1.67;
      //mu = 24.04;
      //kT = 0.1067;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      mu = 88.0;
      kT = 26.7;
      //mu = 184.943;
      //kT = 0.02;
    }
  }
  
  
  if( fitMethod == "attenuatedDoubleExponential" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      mu = 148.;
      kT = 29.6;
      //mu = 0.347;
      //kT = 1.15;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      mu = 171.;
      kT = 75.7;
      //mu = 0.413;
      //kT = 2.59;
    }
  }
  
  
  if( fitMethod == "attenuatedPowerLaw" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      //mu = 160.;
      //kT = 8.95;
      //works
      mu = 24.04;
      kT = 0.1067;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      //mu = 108.;
      //kT = 70.2;
      //~works
      //mu = 24.04;
      //kT = 0.1067;
      //~works better
      mu = 160.04;
      kT = 10.;
    }
  }
  
  
  if( fitMethod == "attenuatedExpPol2order" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      mu = 178.;
      kT = 1.67;
      //mu = 0.376;
      //kT = 1.34;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      mu = 88.0;
      kT = 26.7;
      //mu = 0.415;
      //kT = 2.83;
    }
  }
  
  
  if( fitMethod == "attenuatedExpPol3order" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      mu = 165.;
      kT = 6.52;
      //mu = 0.341;
      //kT = 1.12;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      mu = 75.5;
      kT = 59.8;
      //mu = 0.416;
      //kT = 2.96;
    }
  }
  
  
  if( fitMethod == "attenuatedExpPol4order" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      mu = 182.;
      kT = 0.124;
      //mu = 0.341;
      //kT = 1.12;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      mu = 82.2;
      kT = 65.0;
      //mu = 0.418;
      //kT = 3.41;
    }
  }
}






void GetParameters(const std::string& fitMethod,
                   std::vector<float>& initPars, const int& iStart,
                   const float& mH, const int& step, const std::string& flavour, const std::string& additionalCuts)
{
  if( fitMethod == "attenuatedExponential" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 0.0116;
      //initPars[iStart+0] = 0.0116;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 0.0119;
      //initPars[iStart+0] = 0.0118;
    }
  }
  
  
  if( fitMethod == "attenuatedDoubleExponential" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 0.0287;
      initPars[iStart+1] = 0.901;
      initPars[iStart+2] = 0.0113;
      //initPars[iStart+0] = 0.0121;
      //initPars[iStart+1] = 0.996;
      //initPars[iStart+2] = 0.00535;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 0.0195;
      initPars[iStart+1] = 0.911;
      initPars[iStart+2] = 0.0106;
      //initPars[iStart+0] = 0.0120;
      //initPars[iStart+1] = 0.999;
      //initPars[iStart+2] = 0.00473;
    }
  }
  
  
  if( fitMethod == "attenuatedPowerLaw" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 74.6;
      initPars[iStart+1] = 6050.;
      //initPars[iStart+0] = 28.6;
      //initPars[iStart+1] = 2080.;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 19.9;
      initPars[iStart+1] = 1220.;
      //initPars[iStart+0] = 91.4;
      //initPars[iStart+1] = 7380.;
    }
  }
  
  
  if( fitMethod == "attenuatedExpPol2order" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 5.80;
      initPars[iStart+1] = 6.12e-08;
      //initPars[iStart+0] = 5.79;
      //initPars[iStart+1] = 4.64e-08;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 5.96;
      initPars[iStart+1] = -4.10e-06;
      //initPars[iStart+0] = 5.91;
      //initPars[iStart+1] = 0.0000530;
    }
  }
  
  
  if( fitMethod == "attenuatedExpPol3order" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 5.93;
      initPars[iStart+1] = 0.000526;
      initPars[iStart+2] = -0.412;
      //initPars[iStart+0] = 5.96;
      //initPars[iStart+1] = 0.532;
      //initPars[iStart+2] = -0.672;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 6.55;
      initPars[iStart+1] = 0.00551;
      initPars[iStart+2] = -0.631;
      //initPars[iStart+0] = 5.20;
      //initPars[iStart+1] = 1.06;
      //initPars[iStart+2] = -0.802;
    }
  }
  
  
  if( fitMethod == "attenuatedExpPol4order" )
  {
    if( (step <= 13) && (flavour == "e") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = -0.229;
      initPars[iStart+1] = -3.59;
      initPars[iStart+2] = -2.24;
      initPars[iStart+3] = -1.36;
      //initPars[iStart+0] = 5.96;
      //initPars[iStart+1] = -0.531;
      //initPars[iStart+2] = -0.672;
      //initPars[iStart+3] = -0.0133;
    }
    if( (step <= 13) && (flavour == "mu") && (additionalCuts == "none") )
    {
      initPars[iStart+0] = 6.77;
      initPars[iStart+1] = -0.0249;
      initPars[iStart+2] = -0.775;
      initPars[iStart+3] = 0.563;
      //initPars[iStart+0] = 3.36;
      //initPars[iStart+1] = 2.17;
      //initPars[iStart+2] = -1.49;
      //initPars[iStart+3] = -0.932;
    }
  }
}
