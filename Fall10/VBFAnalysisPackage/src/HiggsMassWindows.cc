#include "HiggsMassWindows.h"



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






float GetXFitMIN1(const float& mH, const std::string& fitMethod, const int& step)
{
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") )
  {
    if     ( mH == 200. ) return 220.;
    if     ( mH == 250. ) return 220.;
    else if( mH == 300. ) return 220.;
    else if( mH == 350. ) return 220.;
    else if( mH == 400. ) return 220.;
    else if( mH == 450. ) return 220.;
    else if( mH == 500. ) return 220.;
    else if( mH == 550. ) return 220.;
    else if( mH == 600. ) return 220.;
    else return 1.;
  }
  
  else if( (fitMethod == "attenuatedExponential") || (fitMethod == "attenuatedExponentialNoHoles") ||
           (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
  {
    if( step < 14 )
    {
      if     ( mH == 200. ) return 180.;
      if     ( mH == 250. ) return 180.;
      else if( mH == 300. ) return 180.;
      else if( mH == 350. ) return 180.;
      else if( mH == 400. ) return 180.;
      else if( mH == 450. ) return 180.;
      else if( mH == 500. ) return 180.;
      else if( mH == 550. ) return 180.;
      else if( mH == 600. ) return 180.;
      else return 1.;
    }
    else if( step == 14 )
    {
      if     ( mH == 200. ) return 200.;
      if     ( mH == 250. ) return 200.;
      else if( mH == 300. ) return 200.;
      else if( mH == 350. ) return 200.;
      else if( mH == 400. ) return 200.;
      else if( mH == 450. ) return 200.;
      else if( mH == 500. ) return 200.;
      else if( mH == 550. ) return 200.;
      else if( mH == 600. ) return 200.;
      else return 1.;
    }
    else
    {
      if     ( mH == 200. ) return 220.;
      if     ( mH == 250. ) return 220.;
      else if( mH == 300. ) return 220.;
      else if( mH == 350. ) return 220.;
      else if( mH == 400. ) return 220.;
      else if( mH == 450. ) return 220.;
      else if( mH == 500. ) return 220.;
      else if( mH == 550. ) return 220.;
      else if( mH == 600. ) return 220.;
      else return 1.;
    }
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
