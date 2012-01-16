#include "HiggsCrossSectionSyst.h"



double HiggsCrossSectionSyst(const float& mH, const std::string& productionMode, const std::string& updown)
{
  if( productionMode == "gg" )
  {
    if     ( (mH == 200) && (updown == "up") )   return 0.172;
    else if( (mH == 200) && (updown == "down") ) return 0.152;
    
    if     ( (mH == 250) && (updown == "up") )   return 0.165;
    else if( (mH == 250) && (updown == "down") ) return 0.156;
    
    else if( (mH == 300) && (updown == "up") )   return 0.168;
    else if( (mH == 300) && (updown == "down") ) return 0.155;
    
    else if( (mH == 350) && (updown == "up") )   return 0.197;
    else if( (mH == 350) && (updown == "down") ) return 0.148;
   
    else if( (mH == 400) && (updown == "up") )   return 0.157;
    else if( (mH == 400) && (updown == "down") ) return 0.163;
    
    else if( (mH == 450) && (updown == "up") )   return 0.160;
    else if( (mH == 450) && (updown == "down") ) return 0.172;
    
    else if( (mH == 500) && (updown == "up") )   return 0.177;
    else if( (mH == 500) && (updown == "down") ) return 0.176;
    
    else if( (mH == 550) && (updown == "up") )   return 0.186;
    else if( (mH == 550) && (updown == "down") ) return 0.178;
    
    else if( (mH == 600) && (updown == "up") )   return 0.194;
    else if( (mH == 600) && (updown == "down") ) return 0.179;
    
    else return 0.;
  }
  
  else if( productionMode == "qq" )
  {
    if     ( (mH == 200) && (updown == "up") )   return 0.034;
    else if( (mH == 200) && (updown == "down") ) return 0.023;
    
    if     ( (mH == 250) && (updown == "up") )   return 0.040;
    else if( (mH == 250) && (updown == "down") ) return 0.026;
    
    else if( (mH == 300) && (updown == "up") )   return 0.046;
    else if( (mH == 300) && (updown == "down") ) return 0.027;
    
    else if( (mH == 350) && (updown == "up") )   return 0.052;
    else if( (mH == 350) && (updown == "down") ) return 0.029;
   
    else if( (mH == 400) && (updown == "up") )   return 0.059;
    else if( (mH == 400) && (updown == "down") ) return 0.030;
    
    else if( (mH == 450) && (updown == "up") )   return 0.066;
    else if( (mH == 450) && (updown == "down") ) return 0.032;
    
    else if( (mH == 500) && (updown == "up") )   return 0.072;
    else if( (mH == 500) && (updown == "down") ) return 0.034;
    
    else if( (mH == 550) && (updown == "up") )   return 0.079;
    else if( (mH == 550) && (updown == "down") ) return 0.035;
    
    else if( (mH == 600) && (updown == "up") )   return 0.086;
    else if( (mH == 600) && (updown == "down") ) return 0.038;
    
    else return 0.;
  }
  
  else return 0.;
}
