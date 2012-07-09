#include "FitBiasSyst.h"

double FitBiasSyst(const float& mH, const std::string& flavour)
{
  if( flavour == "e" )
  {
    if     ( mH == 200 ) return 0.825014;
//  else if     ( mH == 250 ) return 0.124;
    else if( mH == 300 ) return 0.271848;
//    else if( mH == 350 ) return 0.293307;
    else if( mH == 400 ) return 0.293307;
    else if( mH == 450 ) return 0.218353;
    else if( mH == 500 ) return 0.0547397;
//    else if( mH == 550 ) return 0.165;
    else if( mH == 600 ) return 0.715767;
    else return 0.;
  }

  
  if( flavour == "mu" )
  {
    if     ( mH == 200 ) return 0.0726281;
//  else if     ( mH == 250 ) return 0.254;
    else if( mH == 300 ) return 0.108896;
//    else if( mH == 350 ) return 0.123063;
    else if( mH == 400 ) return 0.123063;
    else if( mH == 450 ) return 0.0367779;
    else if( mH == 500 ) return 0.215872;
//    else if( mH == 550 ) return 0.215872;
    else if( mH == 600 ) return 0.608116;
    else return 0.;
  }
  
  else return 0.;
}
