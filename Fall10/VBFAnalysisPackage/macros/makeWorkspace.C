#include "../interface/RooFitFunctions.h"

int makeWorkspace () 
{
  RooWorkspace w ("w") ;

  //PG the fit variable
  //PG --- --- --- --- --- --- --- --- --- ---

  x.setBins (80) ;
  w.import (x) ;
  
  //PG the background parameters and shape
  //PG --- --- --- --- --- --- --- --- --- ---

  w.import (N) ;
  w.import (L1) ;
  w.import (L2) ;
  w.import (RFdoubleExp) ;

  //PG the signal shape
  //PG --- --- --- --- --- --- --- --- --- ---

  TFile * sigIn = new TFile ("../signalShapes.root") ;
  RooHistPdf * signal = (RooHistPdf *) sigIn->Get ("PDF_400") ;
  w.import (*signal) ;

  //PG the data
  //PG --- --- --- --- --- --- --- --- --- ---

  TFile * bkgIn = new TFile ("../testRooFit_004.root") ;
  RooDataSet * data_DS = (RooDataSet *) bkgIn->Get ("data_obs") ;
  RooDataHist * data_binned = new RooDataHist ("data_binned", "data_binned", *data_DS->get (), *data_DS) ;
  w.import (*data_binned) ;
  w.import (*data_DS) ;

  
  cout << " --- --- --- --- --- --- ---" << endl ;
  w.Print () ;
  
  TFile * output = new TFile ("combineWS_400.root", "recreate") ;
  output->cd () ;
  w.Write () ;
  output->Close () ;
  
}