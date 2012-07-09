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
