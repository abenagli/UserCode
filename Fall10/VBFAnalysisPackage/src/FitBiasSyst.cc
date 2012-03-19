#include "FitBiasSyst.h"



double FitBiasSyst(const float& mH, const std::string& flavour)
{
  if( flavour == "e" )
  {
    if     ( mH == 250 ) return 0.126;
    else if( mH == 300 ) return 0.101;
    else if( mH == 350 ) return 0.098;
    else if( mH == 400 ) return 0.091;
    else if( mH == 450 ) return 0.080;
    else if( mH == 500 ) return 0.070;
    else if( mH == 550 ) return 0.149;
    else if( mH == 600 ) return 0.277;
    else return 0.;
  }
  
  if( flavour == "mu" )
  {
    if     ( mH == 250 ) return 0.467;
    else if( mH == 300 ) return 0.174;
    else if( mH == 350 ) return 0.105;
    else if( mH == 400 ) return 0.225;
    else if( mH == 450 ) return 0.291;
    else if( mH == 500 ) return 0.323;
    else if( mH == 550 ) return 0.193;
    else if( mH == 600 ) return 0.101;
    else return 0.;
  }
  
  else return 0.;
}
