#include <iostream>
#include <iomanip>
#include <map>

#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"
#include "HiggsCrossSectionSyst.h"
#include "Functions.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TString.h"

#include "RooArgList.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooGaussian.h"
#include "RooWorkspace.h"
#include "RooAttenuatedCrystalBallLowHigh.h"
#include "RooGaussWithSkirt.h"
#include "RooFitResult.h"
#include "RooAbsPdf.h"
#include "RooDataSet.h"
#include "RooPlot.h"

using namespace std ;
using namespace RooFit ;


void fitRGWS (RooDataHist & datasetHist, RooAbsPdf * rgws, RooRealVar & x, RooArgList & params) ;
void fitSCB (RooDataHist & datasetHist, RooAbsPdf * rgws, RooRealVar & x, RooArgList & params) ;


int main (int argc, char ** argv) 
{
  TFile f (argv[1]) ;
  TH1F * h_ggH = (TH1F *) f.Get ("ggH") ;
  TH1F * h_qqH = (TH1F *) f.Get ("qqH") ;

  TH1F * h = (TH1F *) h_ggH->Clone ("h") ;
  h->Add (h_qqH) ;

  //PG the x variable for everything
  RooRealVar x ("x",      "x",      0, 1000) ;

  //PG the full histo to be fit
  RooDataHist datasetHist ("higgs", "higgs", RooArgList (x), h) ;

  RooAbsPdf * rgws ;
  RooArgList rgws_params ;
  fitRGWS (datasetHist, rgws, x, rgws_params) ;

  for (int i = 0 ; i < rgws_params.getSize () ; ++i)
    {
      RooRealVar * dummy = (RooRealVar *) rgws_params.at (i) ;
      cout << dummy->GetName () 
           << " = " 
           << dummy->getVal () 
           << endl ;    
    }

  RooAbsPdf * scb ;
  RooArgList scb_params ;
  fitSCB (datasetHist, scb, x, scb_params) ;

  for (int i = 0 ; i < scb_params.getSize () ; ++i)
    {
      RooRealVar * dummy = (RooRealVar *) scb_params.at (i) ;
      cout << dummy->GetName () 
           << " = " 
           << dummy->getVal () 
           << endl ;    
    }

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void fitRGWS (RooDataHist & datasetHist, RooAbsPdf * rgws, RooRealVar & x, RooArgList & params)
{
  //PG preliminary fit to initiate parameters
  RooRealVar gm ("gm", "gm", 0., 1000.) ; 
  RooRealVar gs ("gs", "gs", 0., 100.); 
  RooGaussian peak ("peak", "peak", x, gm, gs) ;

  peak.fitTo (datasetHist) ;
  x.setRange ("R1", gm.getVal () - gs.getVal () , gm.getVal () + gs.getVal ()) ;    
  peak.fitTo (datasetHist, Range ("R1"), PrintLevel (-10)) ;
  x.setRange ("R2", gm.getVal () - gs.getVal () , gm.getVal () + gs.getVal ()) ;    
  peak.fitTo (datasetHist,Range ("R2"), PrintLevel (-10)) ;

  double gmean = gm.getVal () ;
  double gsigma = gs.getVal () ;

  //PG gaus with exponential tails
  RooRealVar rgws_norma  ("rgws_norma",  "rgws_norma",  datasetHist.sumEntries ()) ;
  RooRealVar rgws_mean   ("rgws_mean",   "rgws_mean",   gmean, 0.5 * gmean, 1.5 * gmean) ;
  RooRealVar rgws_sigma  ("rgws_sigma",  "rgws_sigma",  gsigma, 0.5 * gsigma, 2 * gsigma) ;
  RooRealVar rgws_alphaL ("rgws_alphaL", "rgws_alphaL", 1, 0, 10) ;
  RooRealVar rgws_alphaR ("rgws_alphaR", "rgws_alphaR", 1, 0, 10) ;

  rgws = new RooGaussWithSkirt ("rgws", "rgws",
                                x, rgws_norma, rgws_mean, rgws_sigma, rgws_alphaL, rgws_alphaR) ;
  RooFitResult * rgws_res = rgws->fitTo (datasetHist, Save (), PrintLevel (-10)) ;
  
  params.add (rgws_norma ) ;
  params.add (rgws_mean  ) ;
  params.add (rgws_sigma ) ;
  params.add (rgws_alphaL) ;
  params.add (rgws_alphaR) ;

  cout << "RGWS fit status " << rgws_res->status () << endl ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void fitSCB (RooDataHist & datasetHist, RooAbsPdf * scb, RooRealVar & x, RooArgList & params)
{
  //PG preliminary fit to initiate parameters
  RooRealVar gm ("gm", "gm", 0., 1000.) ; 
  RooRealVar gs ("gs", "gs", 0., 100.); 
  RooGaussian peak ("peak", "peak", x, gm, gs) ;

  peak.fitTo (datasetHist) ;
  x.setRange ("R1", gm.getVal () - gs.getVal () , gm.getVal () + gs.getVal ()) ;    
  peak.fitTo (datasetHist, Range ("R1"), PrintLevel (-10)) ;
  x.setRange ("R2", gm.getVal () - gs.getVal () , gm.getVal () + gs.getVal ()) ;    
  peak.fitTo (datasetHist,Range ("R2"), PrintLevel (-10)) ;

  double gmean = gm.getVal () ;
  double gsigma = gs.getVal () ;

  //PG gaus with exponential tails
  RooRealVar scb_norma  ("scb_norma",  "scb_norma",  datasetHist.sumEntries ()) ;
  RooRealVar scb_mu     ("scb_mu",     "scb_mu",     gmean, 0.5 * gmean, 1.5 * gmean) ;
  RooRealVar scb_kT     ("scb_kT",     "scb_kT",     gmean, 0.5 * gmean, 1.5 * gmean) ;
  RooRealVar scb_mean   ("scb_mean",   "scb_mean",   gmean, 0.5 * gmean, 1.5 * gmean) ;
  RooRealVar scb_sigma  ("scb_sigma",  "scb_sigma",  gsigma, 0.5 * gsigma, 2 * gsigma) ;
  RooRealVar scb_alpha  ("scb_alpha",  "scb_alpha",  1, 0, 10) ;
  RooRealVar scb_n      ("scb_n",      "scb_n",      50, 0, 200) ;
  RooRealVar scb_alpha2 ("scb_alpha2", "scb_alpha2", 1, 0, 10) ;
  RooRealVar scb_n2     ("scb_n2",     "scb_n2",     50, 0, 200) ;

  scb = new RooAttenuatedCrystalBallLowHigh ("scb", "scb",
					     x, scb_mu, scb_kT, scb_mean, scb_sigma, scb_alpha, scb_n, scb_alpha2, scb_n2) ;
  RooFitResult * scb_res = scb->fitTo (datasetHist, Save (), PrintLevel (-10)) ;   

  params.add (scb_norma ) ;
  params.add (scb_mu    ) ;
  params.add (scb_kT    ) ;
  params.add (scb_mean  ) ;
  params.add (scb_sigma ) ;
  params.add (scb_alpha ) ;
  params.add (scb_n     ) ;
  params.add (scb_alpha2) ;
  params.add (scb_n2    ) ;

  cout << "SCB fit status " << scb_res->status () << endl ;
}
