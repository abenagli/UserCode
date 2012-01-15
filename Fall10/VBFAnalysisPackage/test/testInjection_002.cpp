
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

  double initVal_L1 = L1->getVal () ;
  double initVal_L2 = L2->getVal () ;
  double initVal_N  = N ->getVal () ;
  double initVal_kT = kT->getVal () ;
  double initVal_mu = mu->getVal () ;

  double sigStartingNum = 1. ;
  sigStartingNum *= exp->sumEntries () ;

  RooRealVar nSig ("nSig", "nSig", sigStartingNum, -3. * sigStartingNum, 3 * sigStartingNum) ;
  double numInit = d->sumEntries () ;
  RooRealVar nBkg ("nBkg", "nBkg", numInit, 0.5 * numInit, 2 * numInit) ;
  w->import (nSig) ;
  w->import (nBkg) ;  

  //PG prepare the signal function for the fit
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

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

  //PG gaus with exponential tails
  RooRealVar rgws_norma  ("rgws_norma",  "rgws_norma",  exp->sumEntries ()) ;
  RooRealVar rgws_mean   ("rgws_mean",   "rgws_mean",   gmean, 0.5 * gmean, 1.5 * gmean) ;
  RooRealVar rgws_sigma  ("rgws_sigma",  "rgws_sigma",  gsigma, 0.5 * gsigma, 2 * gsigma) ;
  RooRealVar rgws_alphaL ("rgws_alphaL", "rgws_alphaL", 1, 0, 10) ;
  RooRealVar rgws_alphaR ("rgws_alphaR", "rgws_alphaR", 1, 0, 10) ;

  RooGaussWithSkirt * rgws = new RooGaussWithSkirt ("rgws", "rgws",
                                *x, rgws_norma, rgws_mean, rgws_sigma, rgws_alphaL, rgws_alphaR) ;

  fixMe (rgws_norma ) ;
  fixMe (rgws_mean  ) ;
  fixMe (rgws_sigma ) ;
  fixMe (rgws_alphaL) ;
  fixMe (rgws_alphaR) ;

  RooFitResult * rgws_res = rgws->fitTo (*exp, Save ()) ;
  RooPlot * gausFrame = x->frame () ;
  exp->plotOn (gausFrame) ;
  rgws->plotOn (gausFrame) ;
  TCanvas c1 ;
  gausFrame->Draw () ;
  c1.Print ("signalModel.pdf", "pdf") ;

  //PG signal plus background
  RooAddPdf sb ("sb","bkg+ggH", RooArgList (*rgws, *bkg), RooArgList (nSig, nBkg)) ;
  
  //PG fix the parameters of the turn-on
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  kT->setVal (55) ;
  mu->setVal (190) ;
  fixMe (*kT) ;
  fixMe (*mu) ;

  //PG fit the bkg only to data
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  //PG variables
  RooRealVar init_mu ("init_mu","init_mu",160.,0.,500.); 
  RooRealVar init_kT ("init_kT","init_kT", 20.,0.,100.); 
  RooRealVar init_N  ("init_N", "init_N", 0.007,0.,1.0);
  RooRealVar init_L1 ("init_L1","init_L1",0.015,0.,0.1);
  RooRealVar init_L2 ("init_L2","init_L2",0.005,0.,0.1);
  //PG function
  RooGenericPdf init_pdf_bkg ("init_pdf_bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * (exp(-1*@4*@0) + @3*exp(-1*@5*@0))",
                              RooArgSet(*x,init_mu,init_kT,init_N,init_L1,init_L2));
  init_pdf_bkg.fitTo (*d) ;
  
  TH1F result ("result", "result", 100, -5, 5) ;
  
  int nToys = 1000 ;

  //PG loop on toys
  for (int iToy = 0 ; iToy < nToys ; ++iToy)
    {
      //PG generate bkg
      RooDataSet * background = init_pdf_bkg.generate (*x, d->sumEntries ()) ;
      RooDataHist injected ("h_back", "h_back", *x, *background) ; 
      //PG generate signal
      RooDataSet * signal     = rgws->generate (*x, sigStartingNum) ;
      injected.add (*signal, "1==1", 1.) ;

      //PG prepare the fitting function initial params
      nSig.setVal (sigStartingNum) ;
      nBkg.setVal (numInit) ;
      L1->setVal (initVal_L1) ;
      L2->setVal (initVal_L2) ;
      N ->setVal (initVal_N) ;
      kT->setVal (initVal_kT) ;
      mu->setVal (initVal_mu) ;

      RooFitResult * sbRes = sb.fitTo (injected, Extended (kTRUE), Save (), Strategy (2), Minos (1)) ;

      double obtained = nSig.getVal () ;
      
      result.Fill ((sigStartingNum - obtained) / sigStartingNum) ;
      
      delete background ;
      delete signal ;
    } //PG loop on toys

  c1.cd () ;
  result.Draw () ;
  c1.Print ("bias.pdf", "pdf") ;

  TFile out ("bias.root", "recreate") ;
  out.cd () ;
  result.Write () ;
  out.Close () ;

  return 0 ;
}

