/*
testInjection_003.exe ./step13/shapes_fitNoHoles_attenuatedDoubleExponential_200_emu.root 425 1
testInjection_003.exe ./step13/shapes_fitNoHoles_attenuatedDoubleExponential_250_emu.root 433 1
testInjection_003.exe ./step13/shapes_fitNoHoles_attenuatedDoubleExponential_300_emu.root 420 1
testInjection_003.exe ./step13/shapes_fitNoHoles_attenuatedDoubleExponential_350_emu.root 450 1
testInjection_003.exe ./step13/shapes_fitNoHoles_attenuatedDoubleExponential_400_emu.root 371 1
testInjection_003.exe ./step13/shapes_fitNoHoles_attenuatedDoubleExponential_450_emu.root 250 1
testInjection_003.exe ./step13/shapes_fitNoHoles_attenuatedDoubleExponential_500_emu.root 161 1
testInjection_003.exe ./step13/shapes_fitNoHoles_attenuatedDoubleExponential_550_emu.root 103 1
testInjection_003.exe ./step13/shapes_fitNoHoles_attenuatedDoubleExponential_600_emu.root  65 1

*/


#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TROOT.h"
#include "TPaveText.h"
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

#include "RooSuperCrystalBall.h"
#include "RooAttenuatedCrystalBallLowHigh.h"
#include "RooGaussWithSkirt.h"

using namespace RooFit ;

double fixMe (RooRealVar & var) 
{
  var.setRange (var.getVal () , var.getVal ()) ;
  return var.getVal () ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int main (int argc, char ** argv)
{
  if (argc < 2) 
    {
      cerr << "usage: " << argv[0] << " filename [toys] [NsmXS]\n" ;
      exit (1) ;
    }
  int expnum = 100 ;
  if (argc>=2) expnum = atoi (argv[2]) ;
  double nSMXS = 1. ;
  if (argc>=3) nSMXS = atof (argv[3]) ;

  //PG prendo le info dal workspace
  //PG --- --- --- --- --- --- --- ---

  TFile *_file0 = TFile::Open(argv[1]) ;
  RooWorkspace * w = (RooWorkspace *) _file0->Get ("workspace")               ;
  RooDataHist * d = (RooDataHist *) w->data ("data_obs")                      ;
  RooRealVar * x = w->var ("x")                                               ;
  RooRealVar * L1 = w->var ("L1") ;
  RooRealVar * L2 = w->var ("L2") ; 
  RooRealVar * N  = w->var ("N") ;
  RooRealVar * kT  = w->var ("kT") ;
  RooRealVar * mu  = w->var ("mu") ;
  RooAbsPdf * sig_ggH = w->pdf ("ggH")                                            ;
  RooAbsPdf * sig_qqH = w->pdf ("qqH")                                            ;
  RooAbsPdf * bkg = w->pdf ("bkg")                                            ;
  RooGenericPdf * sing_alone = (RooGenericPdf *) sig_ggH->clone ("sig_alone") ;
  RooGenericPdf * bkg_alone = (RooGenericPdf *) bkg->clone ("bkg_alone") ;
//  RooGenericPdf * bkg_forGaus = (RooGenericPdf *) bkg->clone ("bkg_forGaus") ;
//  RooDataHist * exp = (RooDataHist *) w->data ("dh_ggH")                      ;

  //PG fitto i dati con solo fondo
  //PG --- --- --- --- --- --- --- ---
  
  fixMe (*kT) ;
  fixMe (*mu) ;
  RooFitResult * bkg_res = bkg->fitTo (*d, Save ()) ;

  RooPlot * bkgFrame = x->frame () ;
  d->plotOn (bkgFrame, MarkerStyle (4)) ;
  bkg->plotOn (bkgFrame, LineWidth (1)) ;
  TCanvas c1 ;
  bkgFrame->Draw () ;
  c1.Print ("tI3_bkg.pdf", "pdf") ;

  //PG fitto i dati con segnale piu' fondo
  //PG --- --- --- --- --- --- --- ---

  double sigStartingNum = nSMXS ;
  sigStartingNum *= expnum ;

//  RooRealVar nSig ("nSig", "nSig", sigStartingNum, -4. * sigStartingNum, 4 * sigStartingNum) ;
//  RooRealVar nSig ("nSig", "nSig", sigStartingNum, 0. * sigStartingNum, 4 * sigStartingNum) ; //PG only positive
  RooRealVar nSig ("nSig", "nSig", sigStartingNum, sigStartingNum, sigStartingNum) ; //PG fixed value
  double numInit = d->sumEntries () ;
  RooRealVar nBkg ("nBkg", "nBkg", numInit, 0.5 * numInit, 2 * numInit) ;
  w->import (nSig) ;
  w->import (nBkg) ;  

  //PG signal plus background
  RooAddPdf sb ("sb","ggH+bkg", RooArgList (*sig_ggH, *bkg), RooArgList (nSig, nBkg)) ;

  RooFitResult * sb_res = sb.fitTo (*d, Save ()) ;

  RooPlot * sbFrame = x->frame () ;
  d->plotOn (sbFrame, MarkerStyle (4)) ;
  sb.plotOn (sbFrame, LineWidth (1)) ;
  sb.plotOn (sbFrame, Components (*bkg), LineColor (kGreen+2), LineWidth (1)) ;
  sb.plotOn (sbFrame, Components (*sig_ggH), LineColor (kRed), LineWidth (1)) ;
  bkg->plotOn (sbFrame, LineColor (kBlack), LineWidth (1)) ;

//  sig_ggH->plotOn (sbFrame, LineColor (kRed)) ;

  sbFrame->Draw () ;
  c1.SetLogy () ;
  c1.Print ("tI3_sb.pdf", "pdf") ;

//  TFile ("tI3_sb.root", "recreate") ;
  c1.SaveAs ("tI3_sb.root") ;

  return 0 ;
}

/*
- il fit con il segnale forzato allo SM sta nei punti come il fit senza SM
- se non forzo lo SM, il fit non trova segnale (del resto non ne ho messo)
- la componente del fondo del fit forzato e' molto simile al fit di solo fondo

Quindi, non c'e' la risoluzione per vedere un segnale?
Devo iniettare per vedere se e' cosi'.
*/
