#include "../interface/RooFitFunctions.h"

int makeWorkspace (/*TString mass*/) 
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

//  RooHistPdf * signal = (RooHistPdf *) sigIn->Get (TString ("PDF_") + mass) ;  w.import (*signal) ;
  
  RooHistPdf * signal_250 = (RooHistPdf *) sigIn->Get ("PDF_250") ;  w.import (*signal_250) ;
  RooHistPdf * signal_300 = (RooHistPdf *) sigIn->Get ("PDF_300") ;  w.import (*signal_300) ;
  RooHistPdf * signal_350 = (RooHistPdf *) sigIn->Get ("PDF_350") ;  w.import (*signal_350) ;
  RooHistPdf * signal_400 = (RooHistPdf *) sigIn->Get ("PDF_400") ;  w.import (*signal_400) ;
  RooHistPdf * signal_450 = (RooHistPdf *) sigIn->Get ("PDF_450") ;  w.import (*signal_450) ;
  RooHistPdf * signal_500 = (RooHistPdf *) sigIn->Get ("PDF_500") ;  w.import (*signal_500) ;
  RooHistPdf * signal_550 = (RooHistPdf *) sigIn->Get ("PDF_550") ;  w.import (*signal_550) ;
  RooHistPdf * signal_600 = (RooHistPdf *) sigIn->Get ("PDF_600") ;  w.import (*signal_600) ;

  //PG the data
  //PG --- --- --- --- --- --- --- --- --- ---

  TFile * bkgIn = new TFile ("../testRooFit_004.root") ;
  RooDataSet * data_DS = (RooDataSet *) bkgIn->Get ("data_unbinned") ;
  RooDataHist * data_obs = new RooDataHist ("data_obs", "data_obs", *data_DS->get (), *data_DS) ;
  w.import (*data_obs) ;
//  w.import (*data_DS) ;

  
  cout << " --- --- --- --- --- --- ---" << endl ;
  w.Print () ;
  
  TFile * output = new TFile (TString ("combineWS.root"), "recreate") ;
  output->cd () ;
  w.Write () ;
  output->Close () ;
  
}