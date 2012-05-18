/*
.L RooGaussWithSkirt.cxx++
.x testRGWS.cpp 
*/

#include "/Users/govoni/private/job/cms/HIGGS/qqH_WW_lvjj/analysis/VBFAnalysisPackage/interface/Functions.h"

int testRGWS ()
{
  TFile * f = new TFile ("shapes_fit_attenuatedDoubleExponentialNoHoles_500.root") ;
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

  RooRealVar * x      = new RooRealVar ("x",      "x",      0, 1000) ;
  RooRealVar * norma  = new RooRealVar ("norma",   "norma",   h->Integral ()) ;
  RooRealVar * mean   = new RooRealVar ("mean",   "mean",   uGaus->GetParameter (1)) ;
  RooRealVar * sigma  = new RooRealVar ("sigma",  "sigma",  uGaus->GetParameter (2)) ;
  RooRealVar * alphaL = new RooRealVar ("alphaL", "alphaL", 1, 0, 10) ;
  RooRealVar * alphaR = new RooRealVar ("alphaR", "alphaR", 1, 0, 10) ;

  RooGaussWithSkirt * rgws = new RooGaussWithSkirt ("rgws", "rgws",
                                                    *x, *norma, *mean, *sigma, *alphaL, *alphaR);

  RooDataHist * datasetHist = new RooDataHist ("higgs", "higgs", RooArgList (*x), h) ;

  rgws->fitTo (*datasetHist) ;   
  
  RooPlot * xframe = x->frame () ;
  datasetHist->plotOn (xframe) ;
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
  