#include "FitBiasSyst.h"



double FitBiasSyst(const float& mH, const std::string& flavour)
{
  if( flavour == "e" )
  {
    if     ( mH == 200 ) return 0.127;
    if     ( mH == 250 ) return 0.170;
    else if( mH == 300 ) return 0.135;
    else if( mH == 350 ) return 0.045;
    else if( mH == 400 ) return 0.045;
    else if( mH == 450 ) return 0.143;
    else if( mH == 500 ) return 0.132;
    else if( mH == 550 ) return 0.241;
    else if( mH == 600 ) return 0.383;
    else return 0.;
  }
  
  if( flavour == "mu" )
  {
    if     ( mH == 200 ) return 0.030;
    if     ( mH == 250 ) return 0.042;
    else if( mH == 300 ) return 0.026;
    else if( mH == 350 ) return 0.046;
    else if( mH == 400 ) return 0.032;
    else if( mH == 450 ) return 0.053;
    else if( mH == 500 ) return 0.021;
    else if( mH == 550 ) return 0.103;
    else if( mH == 600 ) return 0.180;
    else return 0.;
  }
  
  else return 0.;
}
