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
#include "RooFitFunctions.h"

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

  N1.setVal (10.) ;
  L1.setVal (0.012) ;
  N2.setVal (5.) ;
  L2.setVal (0.005) ;
  
  
  
  // plot the distributions
  RooPlot *xplot = x.frame () ;
  RFdoubleExp.plotOn (xplot) ;
  TCanvas c1 ;
  xplot->Draw () ;
  c1.Print ("testDoubleExp.pdf","pdf") ;

  return 0 ;
}

