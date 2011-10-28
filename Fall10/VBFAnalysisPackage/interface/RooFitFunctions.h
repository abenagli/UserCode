#ifndef RooFitFunctions_h
#define RooFitFunctions_h

#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooGenericPdf.h"



RooRealVar x ("x", "higgs mass", 200., 1000.) ;


RooRealVar N ("N", "rel amplitude second exp", 0., 1.) ;
RooRealVar L1 ("L1", "rel attentuation first exp",  0., 1.) ;
RooRealVar L2 ("L2", "rel attentuation first exp",  0., 1.) ;

RooGenericPdf RFdoubleExp ("RFdoubleExp", "double exponential", 
//                           "N1 * exp (-1 * L1 * x) + N2 * exp (-1 * L2 * x)",
                           "exp (-1 * @1 * @0) + @2 * exp (-1 * @3 * @0)",
                           RooArgSet (x, L1, N, L2)) ;
                           
#endif                           

