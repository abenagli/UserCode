#include "FitBiasSyst.h"



double FitBiasSyst(const float& mH, const std::string& flavour)
{
  if( flavour == "e" )
  {
    if     ( mH == 250 ) return 0.083;
    else if( mH == 300 ) return 0.242;
    else if( mH == 350 ) return 0.202;
    else if( mH == 400 ) return 0.108;
    else if( mH == 450 ) return 0.061;
    else if( mH == 500 ) return 0.132;
    else if( mH == 550 ) return 0.341;
    else if( mH == 600 ) return 0.656;
    else return 0.;
  }
  
  if( flavour == "mu" )
  {
    if     ( mH == 250 ) return 0.098;
    else if( mH == 300 ) return 0.088;
    else if( mH == 350 ) return 0.075;
    else if( mH == 400 ) return 0.046;
    else if( mH == 450 ) return 0.053;
    else if( mH == 500 ) return 0.100;
    else if( mH == 550 ) return 0.193;
    else if( mH == 600 ) return 0.281;
    else return 0.;
  }
  
  else return 0.;
}
