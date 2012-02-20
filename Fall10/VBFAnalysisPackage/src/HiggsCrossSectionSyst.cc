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



double HiggsQCDScaleSyst(const float& mH, const std::string& productionMode, const std::string& updown)
{
  if( productionMode == "gg" )
  {
    if     ( (mH == 200) && (updown == "up") )   return 0.094;
    else if( (mH == 200) && (updown == "down") ) return 0.072;
    
    if     ( (mH == 250) && (updown == "up") )   return 0.086;
    else if( (mH == 250) && (updown == "down") ) return 0.074;
    
    else if( (mH == 300) && (updown == "up") )   return 0.086;
    else if( (mH == 300) && (updown == "down") ) return 0.071;
    
    else if( (mH == 350) && (updown == "up") )   return 0.111;
    else if( (mH == 350) && (updown == "down") ) return 0.062;
   
    else if( (mH == 400) && (updown == "up") )   return 0.068;
    else if( (mH == 400) && (updown == "down") ) return 0.077;
    
    else if( (mH == 450) && (updown == "up") )   return 0.067;
    else if( (mH == 450) && (updown == "down") ) return 0.084;
    
    else if( (mH == 500) && (updown == "up") )   return 0.080;
    else if( (mH == 500) && (updown == "down") ) return 0.086;
    
    else if( (mH == 550) && (updown == "up") )   return 0.086;
    else if( (mH == 550) && (updown == "down") ) return 0.085;
    
    else if( (mH == 600) && (updown == "up") )   return 0.090;
    else if( (mH == 600) && (updown == "down") ) return 0.083;
    
    else return 0.;
  }
  
  else if( productionMode == "qq" )
  {
    if     ( (mH == 200) && (updown == "up") )   return 0.001;
    else if( (mH == 200) && (updown == "down") ) return 0.002;
    
    if     ( (mH == 250) && (updown == "up") )   return 0.001;
    else if( (mH == 250) && (updown == "down") ) return 0.006;
    
    else if( (mH == 300) && (updown == "up") )   return 0.002;
    else if( (mH == 300) && (updown == "down") ) return 0.008;
    
    else if( (mH == 350) && (updown == "up") )   return 0.003;
    else if( (mH == 350) && (updown == "down") ) return 0.010;
   
    else if( (mH == 400) && (updown == "up") )   return 0.004;
    else if( (mH == 400) && (updown == "down") ) return 0.012;
    
    else if( (mH == 450) && (updown == "up") )   return 0.006;
    else if( (mH == 450) && (updown == "down") ) return 0.013;
    
    else if( (mH == 500) && (updown == "up") )   return 0.007;
    else if( (mH == 500) && (updown == "down") ) return 0.016;
    
    else if( (mH == 550) && (updown == "up") )   return 0.008;
    else if( (mH == 550) && (updown == "down") ) return 0.017;
    
    else if( (mH == 600) && (updown == "up") )   return 0.010;
    else if( (mH == 600) && (updown == "down") ) return 0.020;
    
    else return 0.;
  }
  
  else return 0.;
}



double HiggsPDFSyst(const float& mH, const std::string& productionMode, const std::string& updown)
{
  if( productionMode == "gg" )
  {
    if     ( (mH == 200) && (updown == "up") )   return 0.078;
    else if( (mH == 200) && (updown == "down") ) return 0.080;
    
    if     ( (mH == 250) && (updown == "up") )   return 0.079;
    else if( (mH == 250) && (updown == "down") ) return 0.082;
    
    else if( (mH == 300) && (updown == "up") )   return 0.082;
    else if( (mH == 300) && (updown == "down") ) return 0.084;
    
    else if( (mH == 350) && (updown == "up") )   return 0.086;
    else if( (mH == 350) && (updown == "down") ) return 0.087;
   
    else if( (mH == 400) && (updown == "up") )   return 0.089;
    else if( (mH == 400) && (updown == "down") ) return 0.087;
    
    else if( (mH == 450) && (updown == "up") )   return 0.093;
    else if( (mH == 450) && (updown == "down") ) return 0.088;
    
    else if( (mH == 500) && (updown == "up") )   return 0.097;
    else if( (mH == 500) && (updown == "down") ) return 0.090;
    
    else if( (mH == 550) && (updown == "up") )   return 0.099;
    else if( (mH == 550) && (updown == "down") ) return 0.092;
    
    else if( (mH == 600) && (updown == "up") )   return 0.104;
    else if( (mH == 600) && (updown == "down") ) return 0.096;
    
    else return 0.;
  }
  
  else if( productionMode == "qq" )
  {
    if     ( (mH == 200) && (updown == "up") )   return 0.033;
    else if( (mH == 200) && (updown == "down") ) return 0.020;
    
    if     ( (mH == 250) && (updown == "up") )   return 0.038;
    else if( (mH == 250) && (updown == "down") ) return 0.020;
    
    else if( (mH == 300) && (updown == "up") )   return 0.044;
    else if( (mH == 300) && (updown == "down") ) return 0.020;
    
    else if( (mH == 350) && (updown == "up") )   return 0.049;
    else if( (mH == 350) && (updown == "down") ) return 0.019;
   
    else if( (mH == 400) && (updown == "up") )   return 0.055;
    else if( (mH == 400) && (updown == "down") ) return 0.019;
    
    else if( (mH == 450) && (updown == "up") )   return 0.060;
    else if( (mH == 450) && (updown == "down") ) return 0.018;
    
    else if( (mH == 500) && (updown == "up") )   return 0.066;
    else if( (mH == 500) && (updown == "down") ) return 0.018;
    
    else if( (mH == 550) && (updown == "up") )   return 0.071;
    else if( (mH == 550) && (updown == "down") ) return 0.018;
    
    else if( (mH == 600) && (updown == "up") )   return 0.076;
    else if( (mH == 600) && (updown == "down") ) return 0.017;
    
    else return 0.;
  }
  
  else return 0.;
}



double HiggsHighMassSyst(const float& mH, const std::string& productionMode, const std::string& updown)
{
  if( productionMode == "gg" )
  {
    return 1.5 * pow(mH/1000,3.);
  }
  
  else if( productionMode == "qq" )
  {
    return 0.;
  }
  
  else return 0.;
}
