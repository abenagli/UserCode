/*
testRooFit_001.exe


*/


#include "VBFAnalysisVariables.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "kalanand.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>


#include "TH1.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"
#include "RooPlot.h"
#include "Math/Vector4D.h"

#include "TMVA/Reader.h"
#include "hColl.h"

// #include "PURescaleFactor.C"
//#pragma link C++ function PURescaleFactor (const int&);

using namespace std ;



int main (int argc, char** argv)
{

//  mu = new RooRealVar("mu","",tempMu,tempMu,tempMu);
//  kT = new RooRealVar("kT","",tempKT,tempKT,tempKT);
//  N  = new RooRealVar("N", "",tempN,0.,1000000.0);
//  L1 = new RooRealVar("L1","",tempL1,0.,0.1);
//  L2 = new RooRealVar("L2","",tempL2,0.,0.1);
//
//  pdf_bkg = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * (exp(-1*@4*@0) + @3*exp(-1*@5*@0))",RooArgSet(*x,*mu,*kT,*N,*L1,*L2));



  //RooRealVar x ("x", "higgs mass", 200., 1000.) ;
  //int xBinning = 160 ; //PG 5 GeV binning
  //int xBinning = 80 ; //PG 10 GeV binning

  //RooRealVar N ("N", "rel amplitude second exp", 0., 1.) ;
  //RooRealVar L1 ("L1", "rel attentuation first exp",  0., 1.) ;
  //RooRealVar L2 ("L2", "rel attentuation first exp",  0., 1.) ;
  //
  //RooGenericPdf RFdoubleExp ("RFdoubleExp", "double exponential",
  //                           //"N1 * exp (-1 * L1 * x) + N2 * exp (-1 * L2 * x)",
  //                           "exp (-1 * @1 * @0) + @2 * exp (-1 * @3 * @0)",
  //                           RooArgSet (x, L1, N, L2)) ;
    
//  N1.setVal (10.) ;
//  L1.setVal (0.012) ;
//  N2.setVal (5.) ;
//  L2.setVal (0.005) ;
//  
//  
//  
//  // plot the distributions
//  RooPlot *xplot = x.frame () ;
//  RFdoubleExp.plotOn (xplot) ;
//  TCanvas c1 ;
//  xplot->Draw () ;
//  c1.Print ("testDoubleExp.pdf","pdf") ;

  return 0 ;
}
