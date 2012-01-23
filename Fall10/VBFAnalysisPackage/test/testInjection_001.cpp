
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TROOT.h"
#include "TPaveText.h" ;
#include "TStyle.h"
#include <fstream>
#include <vector>
#include <utility>
#include <cmath>

#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooGenericPdf.h"
#include "RooAddPdf.h"
#include "RooProdPdf.h"
#include "RooGaussian.h"
#include "RooPolynomial.h"
#include "RooFFTConvPdf.h"
#include "RooFitResult.h"
#ifndef __CINT__
#include "RooGlobalFunc.h"
#include "RooChi2Var.h"
#endif 

#include "SUperCrystalBall.h"
#include "RooGaussWithSkirt.h"

using namespace RooFit ;

double fixMe (RooRealVar & var) 
{
  var.setRange (var.getVal () , var.getVal ()) ;
  return var.getVal () ;
}


int main (int argc, char ** argv)
{
//  TFile *_file0 = TFile::Open("shapes_fitNoHoles_doubleExponential_350.root") ;
  TFile *_file0 = TFile::Open(argv[1]) ;
  RooWorkspace * w = (RooWorkspace *) _file0->Get ("workspace")               ;
  RooDataHist * d = (RooDataHist *) w->data ("data_obs")                      ;
  RooRealVar * x = w->var ("x")                                               ;
  RooRealVar * L1 = w->var ("L1") ;
  RooRealVar * L2 = w->var ("L2") ; 
  RooRealVar * N  = w->var ("N") ;
  RooRealVar * kT  = w->var ("kT") ;
  RooRealVar * mu  = w->var ("mu") ;
  RooAbsPdf * sig = w->pdf ("ggH")                                            ;
  RooAbsPdf * bkg = w->pdf ("bkg")                                            ;
  RooGenericPdf * sing_alone = (RooGenericPdf *) sig->clone ("sig_alone") ;
  RooGenericPdf * bkg_alone = (RooGenericPdf *) bkg->clone ("bkg_alone") ;
  RooGenericPdf * bkg_forGaus = (RooGenericPdf *) bkg->clone ("bkg_forGaus") ;
  RooDataHist * exp = (RooDataHist *) w->data ("dh_ggH")                      ;

  //PG prepare the signal function for the fit
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  cerr << "VARS LOADED" << endl ;

  //PG preliminary fit to initiate parameters
  RooRealVar gm ("gm", "gm", 0., 1000.) ; 
  RooRealVar gs ("gs", "gs", 0., 100.); 
  RooGaussian peak ("peak", "peak", *x, gm, gs) ;

  peak.fitTo (*exp) ;
  x->setRange ("R1", gm.getVal () - gs.getVal () , gm.getVal () + gs.getVal ()) ;    
  peak.fitTo (*exp, Range ("R1"), PrintLevel (-10)) ;
  x->setRange ("R2", gm.getVal () - gs.getVal () , gm.getVal () + gs.getVal ()) ;    
  peak.fitTo (*exp, Range ("R2"), PrintLevel (-10)) ;

  double gmean = gm.getVal () ;
  double gsigma = gs.getVal () ;

  cerr << "GAUS FITTED" << endl ;

  //PG gaus with exponential tails
  RooRealVar rgws_norma  ("rgws_norma",  "rgws_norma",  exp->sumEntries ()) ;
  RooRealVar rgws_mean   ("rgws_mean",   "rgws_mean",   gmean, 0.5 * gmean, 1.5 * gmean) ;
  RooRealVar rgws_sigma  ("rgws_sigma",  "rgws_sigma",  gsigma, 0.5 * gsigma, 2 * gsigma) ;
  RooRealVar rgws_alphaL ("rgws_alphaL", "rgws_alphaL", 1, 0, 10) ;
  RooRealVar rgws_alphaR ("rgws_alphaR", "rgws_alphaR", 1, 0, 10) ;

  RooGaussWithSkirt * rgws = new RooGaussWithSkirt ("rgws", "rgws",
                                *x, rgws_norma, rgws_mean, rgws_sigma, rgws_alphaL, rgws_alphaR) ;

  cerr << "SKIRT DEFINED" << endl ;

  RooFitResult * rgws_res = rgws->fitTo (*exp, Save ()) ;
  RooPlot * gausFrame = x->frame () ;
  exp->plotOn (gausFrame) ;
  rgws->plotOn (gausFrame) ;
  TCanvas c1 ;
  gausFrame->Draw () ;
  c1.Print ("signalModel.pdf", "pdf") ;
  

  //PG build injected dataset
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  double sigStartingNum = 1. ;
  sigStartingNum *= exp->sumEntries () ;

  //PG inject a Higgs boson
  RooDataHist injected (*d, "injected") ;
  RooDataSet * signal = sig->generate (*x, sigStartingNum) ;
  injected.add (*signal, "1==1", 1.) ;

//  RooAddPdf sb ("sb","bkg+ggH", RooArgList (*sig, *bkg), RooArgList (nSig, nBkg)) ;
//  
//  RooPlot * xframe = x->frame () ;
//  injected.plotOn (xframe) ;

  cerr << "SIGNAL INJECTED" << endl ;

  //PG fix the parameters of the turn-on
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  kT->setVal (55) ;
  mu->setVal (190) ;
  fixMe (*kT) ;
  fixMe (*mu) ;

  //PG build signal plus bkg function
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  fixMe (rgws_norma ) ;
  fixMe (rgws_mean  ) ;
  fixMe (rgws_sigma ) ;
  fixMe (rgws_alphaL) ;
  fixMe (rgws_alphaR) ;

//  RooRealVar nSig ("nSig", "nSig", sigStartingNum, 0.5 * sigStartingNum, 2 * sigStartingNum) ;
  RooRealVar nSig ("nSig", "nSig", sigStartingNum, 0., 4 * sigStartingNum) ;
  double numInit = d->sumEntries () ;
  RooRealVar nBkg ("nBkg", "nBkg", numInit, 0.5 * numInit, 2 * numInit) ;
  w->import (nSig) ;
  w->import (nBkg) ;  

  RooAddPdf sb ("sb","bkg+ggH", RooArgList (*rgws, *bkg), RooArgList (nSig, nBkg)) ;

  cerr << "BS DEFINED" << endl ;
  
  //PG perform the fit
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  RooFitResult * sbRes = sb.fitTo (injected, Extended (kTRUE), Save (), Strategy (2), Minos (1)) ;

  RooPlot * xframe = x->frame () ;
  injected.plotOn (xframe) ;

  sb.plotOn (xframe, VisualizeError (*sbRes, 1), FillColor (kOrange), FillStyle (3001)) ;
  sb.plotOn (xframe, LineColor (kRed)) ;
  sb.paramOn (xframe) ;
//  c1.SetLogy () ;
  xframe->Draw () ;
  c1.Print ("fitted.pdf", "pdf") ;
  c1.SaveAs ("fitted.root") ;
  
  cout << "INJECTED : " << sigStartingNum << endl ;
  cout << "MEASURED : " << nSig.getVal () 
       << " + " << nSig.getAsymErrorHi()
       << " - " << nSig.getAsymErrorLo() 
  << endl ;
}

