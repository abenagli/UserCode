#ifndef RooFitFunctions_h
#define RooFitFunctions_h

#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooGenericPdf.h"



RooRealVar m ("m", "higgs mass", 0., 1000.) ;

RooRealVar N1 ("N1", "rel amplitude first exp",  0., 1.) ;
RooRealVar N2 ("N2", "rel amplitude second exp", 0., 1.) ;
RooRealVar L1 ("L1", "rel attentuation first exp",  0., 1.) ;
RooRealVar L2 ("L2", "rel attentuation first exp",  0., 1.) ;

RooGenericPdf RFdoubleExp ("RFdoubleExp", "double exponential", 
//                           "N1 * exp (-1 * L1 * x) + N2 * exp (-1 * L2 * x)",
                           "@1 * exp (-1 * @2 * @0) + @3 * exp (-1 * @4 * @0)",
                           RooArgSet (m, N1, L1, N2, L2)) ;
                           
#endif                           

