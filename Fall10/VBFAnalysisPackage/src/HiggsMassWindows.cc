#include "HiggsMassWindows.h"



float GetLepNuWMMIN(const float& mH)
{
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






float GetXFitMIN1(const float& mH, const std::string& method)
{
  if( method == "highMass" )
  {
    if     ( mH == 250. ) return 275.;
    else if( mH == 300. ) return 275.;
    else if( mH == 350. ) return 275.;
    else if( mH == 400. ) return 275.;
    else if( mH == 450. ) return 275.;
    else if( mH == 500. ) return 275.;
    else if( mH == 550. ) return 275.;
    else if( mH == 600. ) return 275.;
    else return -1.;    
  }
  
  else
  {
    if     ( mH == 250. ) return 200.;
    else if( mH == 300. ) return 200.;
    else if( mH == 350. ) return 200.;
    else if( mH == 400. ) return 200.;
    else if( mH == 450. ) return 200.;
    else if( mH == 500. ) return 200.;
    else if( mH == 550. ) return 200.;
    else if( mH == 600. ) return 200.;
    else return -1.;
  }
}

float GetXFitMAX1(const float& mH, const std::string& method)
{
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






float GetXFitMIN2(const float& mH, const std::string& method)
{
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

float GetXFitMAX2(const float& mH, const std::string& method)
{
  if     ( mH == 250. ) return 1000.;
  else if( mH == 300. ) return 1000.;
  else if( mH == 350. ) return 1000.;
  else if( mH == 400. ) return 1000.;
  else if( mH == 450. ) return 1000.;
  else if( mH == 500. ) return 1000.;
  else if( mH == 550. ) return 1000.;
  else if( mH == 600. ) return 1000.;
  else return -1.;
}
