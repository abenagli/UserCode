#include <vector>

int macro_009 ()
  {
    TFile input ("../testBkg_009.root") ;                  

    RooDataSet * mc_DY   = (RooDataSet *) input.Get ("mc_DY") ;
    RooDataSet * mc_QCD  = (RooDataSet *) input.Get ("mc_QCD") ;
    RooDataSet * mc_TT   = (RooDataSet *) input.Get ("mc_TT") ;
    RooDataSet * mc_WW   = (RooDataSet *) input.Get ("mc_WW") ;
    RooDataSet * mc_WZ   = (RooDataSet *) input.Get ("mc_WZ") ;
    RooDataSet * mc_Wjet = (RooDataSet *) input.Get ("mc_Wjet") ;
    RooDataSet * mc_ZZ   = (RooDataSet *) input.Get ("mc_ZZ") ;
    RooDataSet * mc_top  = (RooDataSet *) input.Get ("mc_top") ;
    RooDataSet * data    = (RooDataSet *) input.Get ("data") ;


  }