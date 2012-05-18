/*
.L RooGaussWithSkirt.cxx++
.L SuperCrystalBall.cxx++
.x testSCB.cpp 
*/

#include "/Users/govoni/private/job/cms/HIGGS/qqH_WW_lvjj/analysis/VBFAnalysisPackage/interface/Functions.h"
#include "RooPlot.h" 

int testSCB ()
{
//  TFile * f = new TFile ("shapes_fit_attenuatedDoubleExponentialNoHoles_500.root") ;
  TFile * f = new TFile ("shapes_fit_attenuatedDoubleExponentialNoHoles_250.root") ;
  TH1F * h = (TH1F *) f->Get ("ggH") ;

  h->Draw () ;
  TF1 * uGaus = new TF1 ("uGaus", gaussian, 0., 800., 3) ;
  uGaus->SetLineWidth (1.5) ;
  uGaus->SetLineColor (kBlack) ;
  uGaus->SetParameter (0, h->Integral ()) ;
  uGaus->SetParameter (1, h->GetMean ()) ;
  uGaus->SetParameter (2, h->GetRMS ()) ;
  h->Fit ("uGaus") ;

  uGaus->SetLineWidth (2) ;
  double gsigma = uGaus->GetParameter (2) ;
  double gmean = uGaus->GetParameter (1) ;
  h->Fit ("uGaus", "+", "", gmean-gsigma, gmean+gsigma) ;

  gsigma = uGaus->GetParameter (2) ;
  gmean = uGaus->GetParameter (1) ;

  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  RooRealVar * x      = new RooRealVar ("x",      "x",      0, 1000) ;

  RooRealVar * scb_norma  = new RooRealVar ("scb_norma",  "scb_norma",  h->Integral ()) ;
  RooRealVar * scb_mean   = new RooRealVar ("scb_mean",   "scb_mean",   gmean) ;
  RooRealVar * scb_sigma  = new RooRealVar ("scb_sigma",  "scb_sigma",  gsigma, 0.5 * gsigma, 2 * gsigma) ;
  RooRealVar * scb_alpha  = new RooRealVar ("scb_alpha",  "scb_alpha",  1, 0, 10) ;
  RooRealVar * scb_n      = new RooRealVar ("scb_n",      "scb_n",      50, 0, 200) ;
  RooRealVar * scb_alpha2 = new RooRealVar ("scb_alpha2", "scb_alpha2", 1, 0, 10) ;
  RooRealVar * scb_n2     = new RooRealVar ("scb_n2",     "scb_n2",     50, 0, 200) ;

  SuperCrystalBall * scb = new SuperCrystalBall ("scb", "scb", 
                                                 *x, *scb_norma, *scb_mean, *scb_sigma, *scb_alpha, *scb_n, *scb_alpha2, *scb_n2) ;

  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  RooRealVar * rgws_norma  = new RooRealVar ("rgws_norma",  "rgws_norma",  h->Integral ()) ;
  RooRealVar * rgws_mean   = new RooRealVar ("rgws_mean",   "rgws_mean",   gmean) ;
  RooRealVar * rgws_sigma  = new RooRealVar ("rgws_sigma",  "rgws_sigma",  gsigma, 0.5 * gsigma, 2 * gsigma) ;
  RooRealVar * rgws_alphaL = new RooRealVar ("rgws_alphaL", "rgws_alphaL", 1, 0, 10) ;
  RooRealVar * rgws_alphaR = new RooRealVar ("rgws_alphaR", "rgws_alphaR", 1, 0, 10) ;

  RooGaussWithSkirt * rgws = new RooGaussWithSkirt ("rgws", "rgws",
                                                    *x, *rgws_norma, *rgws_mean, *rgws_sigma, *rgws_alphaL, *rgws_alphaR) ;

  // ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  RooDataHist * datasetHist = new RooDataHist ("higgs", "higgs", RooArgList (*x), h) ;

  scb->fitTo (*datasetHist) ;   
  rgws->fitTo (*datasetHist) ;   
  
  RooPlot * xframe = x->frame () ;
  datasetHist->plotOn (xframe) ;
  scb->plotOn (xframe) ;
  rgws->plotOn (xframe) ;
  TCanvas * c2 = new TCanvas () ;
  xframe->Draw () ;
 
//  RooWorkspace w ("w") ;
//  w.import (*x     ) ;
//  w.import (*norma ) ;
//  w.import (*mean  ) ;
//  w.import (*sigma ) ;
//  w.import (*alphaL) ;
//  w.import (*alphaR) ;
//  w.import (* rgws) ;
//  
//  TFile fo ("test.root", "recreate") ;
//  fo.cd () ;
//  w.Write () ;
//  fo.Close () ;

  return 0 ;
}
  