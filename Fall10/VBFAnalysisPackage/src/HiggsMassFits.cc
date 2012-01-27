#include "HiggsMassFits.h"



void FitHiggsMass(TF1** fitFunc, const std::string& funcName, const float& xMin, const float& xMax,
                  TH1F* h, const float& mH, const std::string& fitMethod)
{
  TVirtualFitter::SetDefaultFitter("Minuit2");
  double width = GetHiggsWidth(mH);
  
  
  
  //--------------------------
  // 1st pre-fit with gaussian
  
  TF1* preFitFunc = new TF1("preFitFunc","gaus",mH-100,mH+100);
  
  preFitFunc -> SetParameter(0,h->GetMaximum());
  preFitFunc -> SetParameter(1,mH);
  preFitFunc -> SetParameter(2,width);
  
  preFitFunc -> SetNpx(10000);
  preFitFunc -> SetLineColor(kBlack);
  preFitFunc -> SetLineWidth(2);
  
  h -> Fit("preFitFunc","NRQ+");
  
  double preN     = preFitFunc -> GetParameter(0);
  double preMu    = preFitFunc -> GetParameter(1);
  double preSigma = preFitFunc -> GetParameter(2);
  
  
  
  //--------------------------
  // 2nd pre-fit with gaussian
  
  TF1* preFitFunc2 = new TF1("preFitFunc2","gaus",preMu-0.5*preSigma,preMu+0.5*preSigma);
  
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
  
  TF1* preFitFunc3 = new TF1("preFitFunc3","gaus",preMu2-preSigma2,preMu2+preSigma2);
  
  preFitFunc3 -> SetParameter(0,preN2);
  preFitFunc3 -> SetParameter(1,preMu2);
  preFitFunc3 -> SetParameter(2,preSigma2);
  
  preFitFunc3 -> SetNpx(10000);
  preFitFunc3 -> SetLineColor(kGreen+1);
  preFitFunc3 -> SetLineWidth(2);
  
  h -> Fit("preFitFunc3","NRQ+");
  
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
  
  h -> Fit("preFitFunc4","QNR+");
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
  
  h -> Fit("preFitFunc5","QNR+","",0.,preMu3);
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
