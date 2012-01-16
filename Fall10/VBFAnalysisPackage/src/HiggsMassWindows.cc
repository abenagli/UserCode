#include "HiggsMassWindows.h"



float GetBinWidth()
{
  return 5.;
}



std::vector<int> GetMasses()
{
  std::vector<int> masses;
  
  masses.push_back(200);
  masses.push_back(250);
  masses.push_back(300);
  masses.push_back(350);
  masses.push_back(400);
  masses.push_back(450);
  masses.push_back(500);
  masses.push_back(550);
  masses.push_back(600);
  
  return masses;
}






float GetHiggsWidth(const float& mH)
{
  if     ( mH == 200. ) return 1.43;
  if     ( mH == 250. ) return 4.04;
  else if( mH == 300. ) return 8.43;
  else if( mH == 350. ) return 15.2;
  else if( mH == 400. ) return 29.2;
  else if( mH == 450. ) return 47.0;
  else if( mH == 500. ) return 68.0;
  else if( mH == 550. ) return 93.2;
  else if( mH == 600. ) return 123.;
  else return -1.;
}

float GetHiggsMassFitMIN(const float& mH)
{
  if     ( mH == 200. ) return 180.;
  if     ( mH == 250. ) return 225.;
  else if( mH == 300. ) return 265.;
  else if( mH == 350. ) return 305.;
  else if( mH == 400. ) return 345.;
  else if( mH == 450. ) return 365.;
  else if( mH == 500. ) return 395.;
  else if( mH == 550. ) return 455.;
  else if( mH == 600. ) return 475.;
  else return -1.;
}

float GetHiggsMassFitMAX(const float& mH)
{
  if     ( mH == 200. ) return 220.;
  if     ( mH == 250. ) return 285.;
  if     ( mH == 300. ) return 345.;
  else if( mH == 350. ) return 395.;
  else if( mH == 400. ) return 455.;
  else if( mH == 450. ) return 525.;
  else if( mH == 500. ) return 565.;
  else if( mH == 550. ) return 625.;
  else if( mH == 600. ) return 700.;
  else return -1.;
}




void FitHiggsMass(TF1** fitFunc, const std::string& funcName, const float& xMin, const float& xMax,
                  TH1F* h, const float& mH, const std::string& fitMethod)
{
  TVirtualFitter::SetDefaultFitter("Minuit2");
  double width = GetHiggsWidth(mH);
  
  /*
  TH1F* h = (TH1F*)( h_temp->Clone("h") );
  h -> Reset(); 
  for(int bin = 1; bin <= h->GetNbinsX(); ++bin)
  {
    float binContent = h_temp -> GetBinContent(bin);
    float binError   = h_temp -> GetBinError(bin);
    
    if( binContent/h_temp->Integral() > 0.001 )
    {
      h -> SetBinContent(bin,binContent);
      h -> SetBinError(bin,binError);
    }
  }
  */
  
  
  //--------------------------
  // 1st pre-fit with gaussian
  
  TF1* preFitFunc = new TF1("preFitFunc","gaus",mH-100,mH+100);
  
  preFitFunc -> SetParameter(0,h->GetMaximum());
  preFitFunc -> SetParameter(1,mH);
  preFitFunc -> SetParameter(2,width);
  
  preFitFunc -> SetNpx(10000);
  preFitFunc -> SetLineColor(kBlue);
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
  preFitFunc2 -> SetLineColor(kBlue);
  preFitFunc2 -> SetLineWidth(2);
  
  h -> Fit("preFitFunc2","NRQ+");
  
  double preN2     = preFitFunc2 -> GetParameter(0);
  double preMu2    = preFitFunc2 -> GetParameter(1);
  double preSigma2 = preFitFunc2 -> GetParameter(2);
  
  
  
  if( fitMethod == "crystalBallLowHigh" )
  {
    //-----------------------------
    // fit with double crystal-ball
        
    (*fitFunc) = new TF1(funcName.c_str(),crystalBallLowHigh,0.,1000.,7);
    
    (*fitFunc) -> SetParLimits(3,0.,10.);
    (*fitFunc) -> SetParLimits(4,0.,200.);
    (*fitFunc) -> SetParLimits(5,0.,10.);
    (*fitFunc) -> SetParLimits(6,0.,200.);
    
    (*fitFunc) -> SetParameter(0,preN2);
    (*fitFunc) -> FixParameter(1,preMu2);
    (*fitFunc) -> SetParameter(2,preSigma2);
    (*fitFunc) -> SetParameter(3,0.8);
    (*fitFunc) -> SetParameter(4,10.);
    (*fitFunc) -> SetParameter(5,0.8);
    (*fitFunc) -> SetParameter(6,10.);
    
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
  
  
  delete preFitFunc;
  delete preFitFunc2;
}






float GetLepNuWMMIN(const float& mH)
{
  if     ( mH == 200. ) return 180.;
  if     ( mH == 250. ) return 225.;
  else if( mH == 300. ) return 270.;
  else if( mH == 350. ) return 310.;
  else if( mH == 400. ) return 355.;
  else if( mH == 450. ) return 390.;
  else if( mH == 500. ) return 415.;
  else if( mH == 550. ) return 470.;
  else if( mH == 600. ) return 485.;
  else return -1.;
}

float GetLepNuWMMAX(const float& mH)
{
  if     ( mH == 200. ) return 220.;
  if     ( mH == 250. ) return 275.;
  else if( mH == 300. ) return 330.;
  else if( mH == 350. ) return 390.;
  else if( mH == 400. ) return 445.;
  else if( mH == 450. ) return 510.;
  else if( mH == 500. ) return 575.;
  else if( mH == 550. ) return 610.;
  else if( mH == 600. ) return 665.;
  else return -1.;
}






float GetXFitMIN1(const float& mH, const std::string& fitMethod)
{
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") )
  {
    if     ( mH == 200. ) return 225.;
    if     ( mH == 250. ) return 225.;
    else if( mH == 300. ) return 225.;
    else if( mH == 350. ) return 225.;
    else if( mH == 400. ) return 225.;
    else if( mH == 450. ) return 225.;
    else if( mH == 500. ) return 225.;
    else if( mH == 550. ) return 225.;
    else if( mH == 600. ) return 225.;
    else return 1.;
  }
  
  else if( (fitMethod == "attenuatedExponential") || (fitMethod == "attenuatedExponentialNoHoles") ||
           (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
  {
    if     ( mH == 200. ) return 175.;
    if     ( mH == 250. ) return 175.;
    else if( mH == 300. ) return 175.;
    else if( mH == 350. ) return 175.;
    else if( mH == 400. ) return 175.;
    else if( mH == 450. ) return 175.;
    else if( mH == 500. ) return 175.;
    else if( mH == 550. ) return 175.;
    else if( mH == 600. ) return 175.;
    else return 1.;
  }
  
  else return -1.;
}

float GetXFitMAX1(const float& mH, const std::string& fitMethod)
{
  if( (fitMethod == "doubleExponentialNoHoles") ||
      (fitMethod == "attenuatedExponentialNoHoles") ||
      (fitMethod == "attenuatedDoubleExponentialNoHoles") )
  {
    return -1.;
  }
  
  else
  {
    if     ( mH == 200. ) return GetLepNuWMMIN(200.);
    if     ( mH == 250. ) return GetLepNuWMMIN(250.);
    else if( mH == 300. ) return GetLepNuWMMIN(300.);
    else if( mH == 350. ) return GetLepNuWMMIN(350.);
    else if( mH == 400. ) return GetLepNuWMMIN(400.);
    else if( mH == 450. ) return GetLepNuWMMIN(450.);
    else if( mH == 500. ) return GetLepNuWMMIN(500.);
    else if( mH == 550. ) return GetLepNuWMMIN(550.);
    else if( mH == 600. ) return GetLepNuWMMIN(600.);
    else return -1.;
  }
}






float GetXFitMIN2(const float& mH, const std::string& fitMethod)
{
  if( (fitMethod == "doubleExponentialNoHoles") ||
      (fitMethod == "attenuatedExponentialNoHoles") ||
      (fitMethod == "attenuatedDoubleExponentialNoHoles") )
  {
    return -1.;
  }
  
  else
  {
    if     ( mH == 200. ) return GetLepNuWMMAX(200.);
    if     ( mH == 250. ) return GetLepNuWMMAX(250.);
    else if( mH == 300. ) return GetLepNuWMMAX(300.);
    else if( mH == 350. ) return GetLepNuWMMAX(350.);
    else if( mH == 400. ) return GetLepNuWMMAX(400.);
    else if( mH == 450. ) return GetLepNuWMMAX(450.);
    else if( mH == 500. ) return GetLepNuWMMAX(500.);
    else if( mH == 550. ) return GetLepNuWMMAX(550.);
    else if( mH == 600. ) return GetLepNuWMMAX(600.);
    else return -1.;
  }
}

float GetXFitMAX2(const float& mH, const std::string& fitMethod)
{
  if     ( mH == 200. ) return 800.;
  if     ( mH == 250. ) return 800.;
  else if( mH == 300. ) return 800.;
  else if( mH == 350. ) return 800.;
  else if( mH == 400. ) return 800.;
  else if( mH == 450. ) return 800.;
  else if( mH == 500. ) return 800.;
  else if( mH == 550. ) return 800.;
  else if( mH == 600. ) return 800.;
  else return -1.;
}
