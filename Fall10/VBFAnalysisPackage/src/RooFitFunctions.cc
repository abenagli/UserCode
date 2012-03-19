#include "RooFitFunctions.h"



int DefineRooFitFunction(RooRealVar* x, RooGenericPdf** pdf_bkg, std::vector<RooRealVar*>& pars, std::vector<std::string>& parNames,
                         const std::string& fitMethod,
                         const float& mH, const int& step, const std::string& flavour, const std::string& additionalCuts)
{
  int nPars;
  
  if( fitMethod == "attenuatedExponential" )       nPars = 3;
  if( fitMethod == "attenuatedDoubleExponential" ) nPars = 5;
  if( fitMethod == "attenuatedPowerLaw" )          nPars = 4;
  if( fitMethod == "attenuatedDoublePowerLaw" )    nPars = 5;
  if( fitMethod == "attenuatedExpPol2order" )      nPars = 4;
  if( fitMethod == "attenuatedExpPol3order" )      nPars = 5;
  if( fitMethod == "attenuatedExpPol4order" )      nPars = 6;
  
  std::vector<float> initPars(nPars);
  
  
  
  //-----------------------------
  // define attenuation parameters

  int iStart = 0;
  
  if( (fitMethod == "attenuatedExponential") ||
      (fitMethod == "attenuatedDoubleExponential") ||
      (fitMethod == "attenuatedPowerLaw") ||
      (fitMethod == "attenuatedCrystalBallHigh") ||
      (fitMethod == "attenuatedExpPol2order") ||
      (fitMethod == "attenuatedExpPol3order") ||
      (fitMethod == "attenuatedExpPol4order") )
  {
    GetTurnOnParameters(fitMethod,initPars[0],initPars[1],mH,step,flavour,additionalCuts);
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_mu").c_str(),"",initPars[0],initPars[0],initPars[0]) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_mu" );
    
    pars.push_back(new RooRealVar(("CMS_HWWlvjj_"+flavour+"_kT").c_str(),"",initPars[1],initPars[1],initPars[1]) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_kT" );
    
    //pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_mu").c_str(),"",initPars[0],0.,500.) );
    //parNames.push_back( "CMS_HWWlvjj_"+flavour+"_mu" );
    //
    //pars.push_back(new RooRealVar(("CMS_HWWlvjj_"+flavour+"_kT").c_str(),"",initPars[1],0.,500.) );
    //parNames.push_back( "CMS_HWWlvjj_"+flavour+"_kT" );
    
    iStart = 2;
  }  
  
  

  //-------------------------------------
  // define other parameters and function
  
  GetParameters(fitMethod,initPars,iStart,mH,step,flavour,additionalCuts);
  
  if( fitMethod == "attenuatedExponential" )
  {
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_L1").c_str(),"",initPars[2],0.,0.1) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_L1" );
    
    (*pdf_bkg) = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * exp(-1*@3*@0)",RooArgSet(*x,*pars[0],*pars[1],*pars[2]));
    //(*pdf_bkg) = new RooGenericPdf("bkg",""," 1./(1.+pow(abs(@1*(@0-160.)),-1.*@2)) * exp(-1*@3*@0)",RooArgSet(*x,*pars[0],*pars[1],*pars[2]));
  }
  
  if( fitMethod == "attenuatedDoubleExponential" )
  {
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_L1").c_str(),"",initPars[2],0.,0.1) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_L1" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_N").c_str(), "",initPars[3],0.,1.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_N" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_L2").c_str(),"",initPars[4],0.,0.1) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_L2" );
    
    (*pdf_bkg) = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * (@4*exp(-1*@3*@0) + (1.-@4)*exp(-1*@5*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3],*pars[4]));
    //(*pdf_bkg) = new RooGenericPdf("bkg","","1./(1.+pow(abs(@1*(@0-160.)),-1.*@2)) * (@4*exp(-1*@3*@0) + (1.-@4)*exp(-1*@5*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3],*pars[4]));
  }
  
  if( fitMethod == "attenuatedPowerLaw" )
  {
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_n").c_str(),"",initPars[2],0.,1000.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_n" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_a").c_str(),"",initPars[3],0.,100000.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_a" );
    
    (*pdf_bkg) = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * pow((500.+@4)/abs(@0+@4),@3)",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3]));
    //(*pdf_bkg) = new RooGenericPdf("bkg","","1./(1.+pow(abs(@1*(@0-160.)),-1.*@2)) * pow((500.+@4)/abs(@0+@4),@3)",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3]));
  }
  
  if( fitMethod == "attenuatedDoublePowerLaw" )
  {
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_n").c_str(),"",initPars[2],0.,1000.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_n" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_a").c_str(),"",initPars[3],0.,10000000.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_a" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_b").c_str(),"",initPars[4],0.,10000000.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_b" );
    
    (*pdf_bkg) = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * pow((500.*500.+@5*500.+@4)/(@0*@0+@5*@0+@4),@3)",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3],*pars[4]));
    //(*pdf_bkg) = new RooGenericPdf("bkg","","1./(1.+pow(abs(@1*(@0-160.)),-1.*@2)) * pow((500.*500.+@5*500.+@4)/(@0*@0+@5*@0+@4),@3)",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3],*pars[4]));
  }
  
  if( fitMethod == "attenuatedExpPol2order" )
  {
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_a").c_str(),"",initPars[2],-10.,10.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_a" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_b").c_str(),"",initPars[3],-10.,10.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_b" );
    
    (*pdf_bkg) = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * exp(-1.*(pow(@4/500.*@0,2)+@3/500.*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3]));
    //(*pdf_bkg) = new RooGenericPdf("bkg","","1./(1.+pow(abs(@1*(@0-160.)),-1.*@2)) * exp(-1.*(pow(@4/500.*@0,2)+@3/500.*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3]));
  }
  
  if( fitMethod == "attenuatedExpPol3order" )
  {
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_a").c_str(),"",initPars[2],-10.,10.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_a" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_b").c_str(),"",initPars[3],-10.,10.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_b" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_c").c_str(),"",initPars[4],-10.,10.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_c" );
    
    (*pdf_bkg) = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * exp(-1.*(pow(@5/500.*@0,3)+pow(@4/500.*@0,2)+@3/500.*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3],*pars[4]));
    //(*pdf_bkg) = new RooGenericPdf("bkg","","1./(1.+pow(abs(@1*(@0-160.)),-1.*@2)) * exp(-1.*(pow(@5/500.*@0,3)+pow(@4/500.*@0,2)+@3/500.*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3],*pars[4]));
  }
  
  if( fitMethod == "attenuatedExpPol4order" )
  {
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_a").c_str(),"",initPars[2],-10.,10.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_a" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_b").c_str(),"",initPars[3],-10.,10.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_b" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_c").c_str(),"",initPars[4],-10.,10.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_c" );
    
    pars.push_back( new RooRealVar(("CMS_HWWlvjj_"+flavour+"_d").c_str(),"",initPars[5],-10.,10.) );
    parNames.push_back( "CMS_HWWlvjj_"+flavour+"_d" );
    
    (*pdf_bkg) = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * exp(-1.*(pow(@6/500.*@0,4)+pow(@5/500.*@0,3)+pow(@4/500.*@0,2)+@3/500.*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3],*pars[4],*pars[5]));
    //(*pdf_bkg) = new RooGenericPdf("bkg","","1./(1.+pow(abs(@1*(@0-160.)),-1.*@2)) * exp(-1.*(pow(@6/500.*@0,4)+pow(@5/500.*@0,3)+pow(@4/500.*@0,2)+@3/500.*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3],*pars[4],*pars[5]));
  }
  
  return nPars;
}
