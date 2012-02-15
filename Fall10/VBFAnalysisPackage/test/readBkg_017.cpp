/*
bin/readBkg_017.exe /gwpool/users/ldimatt/scratch0/PHD_MiBi/Fall11_ForDataVsMC_SideBand/VBFAnalysisPackage 400
*/

using namespace std;

#include <algorithm>
#include <cmath>
#include "ConfigParser.h"
#include <fstream>
#include <functional>
#include "h2Factory.h"
#include "hColl.h"
#include "hFactory.h"
#include <iomanip>
#include "kalanand.h"
#include <map>
#include "Math/Vector4D.h"
#include "ntpleUtils.h"
#include "plotUtils.h"
#include "Functions.h"

#include "TVirtualFitter.h"

#include <sstream>
#include "stdHisto.h"
#include <string>
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1.h"
#include "TH2.h"
#include "TMVA/Reader.h"
#include "TObject.h"
#include "TPaveText.h"
#include "TProfile.h"
#include "TRandom3.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include <utility>
#include "VBFAnalysisVariables.h"
#include <vector>

#include "../macros/plotUtils.C"


void setSqrtErrors (TH1F * input)
{
  for (int iBin = 1 ; iBin <= input->GetNbinsX () ; ++iBin)
    {
      input->SetBinError (iBin, sqrt (input->GetBinContent (iBin))) ;
    }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/**
  dump a tprofile in a TH1F, so that the errors are easily modifiable afterwards
*/
TH1F * dumpProfile (TString outputName, TProfile * input)
{
  TH1F * output = new TH1F (outputName, outputName, input->GetNbinsX (), input->GetXaxis ()->GetXmin (), input->GetXaxis ()->GetXmax ()) ;
  for (int iBin = 1 ; iBin <= input->GetNbinsX () ; ++iBin)
    {
      double val = input->GetBinContent (iBin) ;
      output->SetBinContent (iBin, val) ;
      double err = input->GetBinError (iBin) ;
      output->SetBinError (iBin, err) ;
    }
  return output ;  
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void setDoubleExpPars (TF1 * func)
{
  //PG numbers from Andrea
  func->SetParameter (0, 10.) ;
  func->SetParameter (1, 0.013) ;
  func->SetParameter (2, 5.) ;
  func->SetParameter (3, 0.005) ;
  
  func->SetParName (0, "N1") ;
  func->SetParName (1, "#lambda1") ;
  func->SetParName (2, "N2") ;
  func->SetParName (3, "#lambda2") ;
  func->SetLineWidth (1) ;
  func->SetLineColor (kBlue+2) ;
  func->SetNpx (10000) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



void setAttenuatedDoubleExpPars (TF1 * func)
{
  func->SetParameter (0, 190) ;  
//   func->SetParameter (0, 160) ;
  
//   func->SetParLimits (0, 100, 250) ;
  func->SetParameter (1, 20) ;
  func->SetParameter (2, 10.) ;
  func->SetParameter (3, 0.013) ;
  func->SetParameter (4, 5.) ;
  func->SetParameter (5, 0.005) ;
  
  func->SetParName (0, "#mu") ;
  func->SetParName (1, "kT") ;
  func->SetParName (2, "N1") ;
  func->SetParName (3, "#lambda1") ;
  func->SetParName (4, "N2") ;
  func->SetParName (5, "#lambda2") ;
  func->SetLineWidth (1) ;
  func->SetLineColor (kBlue+2) ;
  func->SetNpx (10000) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

int main (int argc, char** argv)
{
  // Set style options
  gROOT->Reset();
  gROOT->SetStyle("Plain");

  gStyle->SetOptTitle(1);
  gStyle->SetOptStat(1110); 
  gStyle->SetOptFit(1111);
  gStyle->SetFitFormat("6.3g"); 
  gStyle->SetPalette(1);
  gROOT->ForceStyle(1);  //If uncommented removes the color from the THStack

  bool histosRatio = false ; //PG calculate the correction factor as the simple ratio of MC histograms, 
                             //PG and the error comes is due to MC statistics
  bool makeToys = false ; //PG calculate the error with a set of toys generated within the error band of the fits
                          //PG to the numerator and denominator
  int nToys = 10000 ;
  bool scaleBand = false ; //PG scale the band according to the pool wrt the MC histo
  
  //bkg to subtract  
  bool subtractWW   = false;
  bool subtractTT   = false; 
  bool subtracttop  = false;
  bool subtractDY   = false;
  bool subtractWZ   = false;
  bool subtractWjet = false;
  bool subtractZZ   = false;
  
  //fit the sideband *before* multiplying for the correction factor OR the extrapolated bkg *after* the multiplication
  bool fitsideband = false; // fit sideband dati prima di ratio
  bool fitbkg      = true;  // fit extrapolated bkg in signal region
  
  //closure on data - closure on MC - final analysis
  bool martijn = false;
  bool closure = false;
  bool final   = true;

  //PG type of fit
  bool attenuated = false ;
  //PG change this further on : double startFit = 225; //GeV, bin from where to start the num & den fit for corr factor

  TString inputFile =  argv[1] ;
  inputFile += "/testBkg_017_S" ;
  inputFile += argv[2] ;

//  inputFile += "_LI_2011AB.root" ; //PG likelihood discriminant
  inputFile += "_2011AB.root" ;
  cout << inputFile << endl ;
  TFile input (inputFile) ;
  
  
  //FC get the single histograms
  //FC ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  //LS get the bkg samples

  //EvenHigher
  TH1F * m4_EvenHigher_DY   = (TH1F *) input.Get ("m4_EvenHigher_DY") ;
  TH1F * m4_EvenHigher_TT   = (TH1F *) input.Get ("m4_EvenHigher_TT") ;
  TH1F * m4_EvenHigher_WW   = (TH1F *) input.Get ("m4_EvenHigher_WW") ;
  TH1F * m4_EvenHigher_WZ   = (TH1F *) input.Get ("m4_EvenHigher_WZ") ;
  TH1F * m4_EvenHigher_Wjet = (TH1F *) input.Get ("m4_EvenHigher_Wjet") ;
  TH1F * m4_EvenHigher_ZZ   = (TH1F *) input.Get ("m4_EvenHigher_ZZ") ;
  TH1F * m4_EvenHigher_top  = (TH1F *) input.Get ("m4_EvenHigher_top") ;
  
  TH1F * m4_EvenHigher_total = (TH1F*) m4_EvenHigher_DY->Clone ("m4_EvenHigher_total");
  if(subtractDY   == true)  m4_EvenHigher_total->Reset();
  if(subtractTT   == false) m4_EvenHigher_total->Add(m4_EvenHigher_TT);
  if(subtractWW   == false) m4_EvenHigher_total->Add(m4_EvenHigher_WW);
  if(subtractWZ   == false) m4_EvenHigher_total->Add(m4_EvenHigher_WZ);
  if(subtractWjet == false) m4_EvenHigher_total->Add(m4_EvenHigher_Wjet);
  if(subtractZZ   == false) m4_EvenHigher_total->Add(m4_EvenHigher_ZZ);
  if(subtracttop  == false) m4_EvenHigher_total->Add(m4_EvenHigher_top); 
  m4_EvenHigher_total->SetTitle ("");

  //upper
  TH1F * m4_upper_DY   = (TH1F *) input.Get ("m4_upper_DY") ;
  TH1F * m4_upper_TT   = (TH1F *) input.Get ("m4_upper_TT") ;
  TH1F * m4_upper_WW   = (TH1F *) input.Get ("m4_upper_WW") ;
  TH1F * m4_upper_WZ   = (TH1F *) input.Get ("m4_upper_WZ") ;
  TH1F * m4_upper_Wjet = (TH1F *) input.Get ("m4_upper_Wjet") ;
  TH1F * m4_upper_ZZ   = (TH1F *) input.Get ("m4_upper_ZZ") ;
  TH1F * m4_upper_top  = (TH1F *) input.Get ("m4_upper_top") ;

  TH1F * m4_upper_total = (TH1F*) m4_upper_DY->Clone ("m4_upper_total");
  if(subtractDY   == true)  m4_upper_total->Reset();
  if(subtractTT   == false) m4_upper_total->Add(m4_upper_TT);
  if(subtractWW   == false) m4_upper_total->Add(m4_upper_WW);
  if(subtractWZ   == false) m4_upper_total->Add(m4_upper_WZ);
  if(subtractWjet == false) m4_upper_total->Add(m4_upper_Wjet);
  if(subtractZZ   == false) m4_upper_total->Add(m4_upper_ZZ);
  if(subtracttop  == false) m4_upper_total->Add(m4_upper_top);     
  m4_upper_total->SetTitle ("");

  //upper_c
  TH1F * m4_upper_c_DY   = (TH1F *) input.Get ("m4_upper_c_DY") ;
  TH1F * m4_upper_c_TT   = (TH1F *) input.Get ("m4_upper_c_TT") ;
  TH1F * m4_upper_c_WW   = (TH1F *) input.Get ("m4_upper_c_WW") ;
  TH1F * m4_upper_c_WZ   = (TH1F *) input.Get ("m4_upper_c_WZ") ;
  TH1F * m4_upper_c_Wjet = (TH1F *) input.Get ("m4_upper_c_Wjet") ;
  TH1F * m4_upper_c_ZZ   = (TH1F *) input.Get ("m4_upper_c_ZZ") ;
  TH1F * m4_upper_c_top  = (TH1F *) input.Get ("m4_upper_c_top") ;
  
  TH1F * m4_upper_c_total = (TH1F*) m4_upper_c_DY->Clone ("m4_upper_c_total");
  if(subtractDY   == true)  m4_upper_c_total->Reset();
  if(subtractTT   == false) m4_upper_c_total->Add(m4_upper_c_TT);
  if(subtractWW   == false) m4_upper_c_total->Add(m4_upper_c_WW);
  if(subtractWZ   == false) m4_upper_c_total->Add(m4_upper_c_WZ);
  if(subtractWjet == false) m4_upper_c_total->Add(m4_upper_c_Wjet);
  if(subtractZZ   == false) m4_upper_c_total->Add(m4_upper_c_ZZ);
  if(subtracttop  == false) m4_upper_c_total->Add(m4_upper_c_top);     
  m4_upper_c_total->SetTitle ("");
  
  //upper_a
  TH1F * m4_upper_a_DY   = (TH1F *) input.Get ("m4_upper_a_DY") ;
  TH1F * m4_upper_a_TT   = (TH1F *) input.Get ("m4_upper_a_TT") ;
  TH1F * m4_upper_a_WW   = (TH1F *) input.Get ("m4_upper_a_WW") ;
  TH1F * m4_upper_a_WZ   = (TH1F *) input.Get ("m4_upper_a_WZ") ;
  TH1F * m4_upper_a_Wjet = (TH1F *) input.Get ("m4_upper_a_Wjet") ;
  TH1F * m4_upper_a_ZZ   = (TH1F *) input.Get ("m4_upper_a_ZZ") ;
  TH1F * m4_upper_a_top  = (TH1F *) input.Get ("m4_upper_a_top") ;
 
  TH1F * m4_upper_a_total = (TH1F*) m4_upper_a_DY->Clone ("m4_upper_a_total");
  if(subtractDY   == true)  m4_upper_a_total->Reset();
  if(subtractTT   == false) m4_upper_a_total->Add(m4_upper_a_TT);
  if(subtractWW   == false) m4_upper_a_total->Add(m4_upper_a_WW);
  if(subtractWZ   == false) m4_upper_a_total->Add(m4_upper_a_WZ);
  if(subtractWjet == false) m4_upper_a_total->Add(m4_upper_a_Wjet);
  if(subtractZZ   == false) m4_upper_a_total->Add(m4_upper_a_ZZ);
  if(subtracttop  == false) m4_upper_a_total->Add(m4_upper_a_top);     
  m4_upper_a_total->SetTitle ("");
  
  //signal
  TH1F * m4_signal_DY   = (TH1F *) input.Get ("m4_signal_DY") ;
  TH1F * m4_signal_TT   = (TH1F *) input.Get ("m4_signal_TT") ;
  TH1F * m4_signal_WW   = (TH1F *) input.Get ("m4_signal_WW") ;
  TH1F * m4_signal_WZ   = (TH1F *) input.Get ("m4_signal_WZ") ;
  TH1F * m4_signal_Wjet = (TH1F *) input.Get ("m4_signal_Wjet") ;
  TH1F * m4_signal_ZZ   = (TH1F *) input.Get ("m4_signal_ZZ") ;
  TH1F * m4_signal_top  = (TH1F *) input.Get ("m4_signal_top") ;
  
  TH1F * m4_signal_total = (TH1F*) m4_signal_DY->Clone ("m4_signal_total");
  if(subtractDY   == true)  m4_signal_total->Reset();
  if(subtractTT   == false) m4_signal_total->Add(m4_signal_TT);
  if(subtractWW   == false) m4_signal_total->Add(m4_signal_WW);
  if(subtractWZ   == false) m4_signal_total->Add(m4_signal_WZ);
  if(subtractWjet == false) m4_signal_total->Add(m4_signal_Wjet);
  if(subtractZZ   == false) m4_signal_total->Add(m4_signal_ZZ);
  if(subtracttop  == false) m4_signal_total->Add(m4_signal_top);
  m4_signal_total->SetTitle ("");
  THStack * stack_m4_signal = new THStack("stack_m4_signal","");
  if(subtractDY   == false) stack_m4_signal->Add(m4_signal_DY);
  if(subtractTT   == false) stack_m4_signal->Add(m4_signal_TT);
  if(subtractWW   == false) stack_m4_signal->Add(m4_signal_WW);
  if(subtractWZ   == false) stack_m4_signal->Add(m4_signal_WZ);
  if(subtractWjet == false) stack_m4_signal->Add(m4_signal_Wjet);
  if(subtractZZ   == false) stack_m4_signal->Add(m4_signal_ZZ);
  if(subtracttop  == false) stack_m4_signal->Add(m4_signal_top);

  //lower
  TH1F * m4_lower_DY   = (TH1F *) input.Get ("m4_lower_DY") ;
  TH1F * m4_lower_TT   = (TH1F *) input.Get ("m4_lower_TT") ;
  TH1F * m4_lower_WW   = (TH1F *) input.Get ("m4_lower_WW") ;
  TH1F * m4_lower_WZ   = (TH1F *) input.Get ("m4_lower_WZ") ;
  TH1F * m4_lower_Wjet = (TH1F *) input.Get ("m4_lower_Wjet") ;
  TH1F * m4_lower_ZZ   = (TH1F *) input.Get ("m4_lower_ZZ") ;
  TH1F * m4_lower_top  = (TH1F *) input.Get ("m4_lower_top") ;
  
  TH1F * m4_lower_total = (TH1F*) m4_lower_DY->Clone ("m4_lower_total");
  if(subtractDY   == true)  m4_lower_total->Reset();
  if(subtractTT   == false) m4_lower_total->Add(m4_lower_TT);
  if(subtractWW   == false) m4_lower_total->Add(m4_lower_WW);
  if(subtractWZ   == false) m4_lower_total->Add(m4_lower_WZ);
  if(subtractWjet == false) m4_lower_total->Add(m4_lower_Wjet);
  if(subtractZZ   == false) m4_lower_total->Add(m4_lower_ZZ);
  if(subtracttop  == false) m4_lower_total->Add(m4_lower_top);     
  m4_lower_total->SetTitle ("");
  
  //lower_c
  TH1F * m4_lower_c_DY   = (TH1F *) input.Get ("m4_lower_c_DY") ;
  TH1F * m4_lower_c_TT   = (TH1F *) input.Get ("m4_lower_c_TT") ;
  TH1F * m4_lower_c_WW   = (TH1F *) input.Get ("m4_lower_c_WW") ;
  TH1F * m4_lower_c_WZ   = (TH1F *) input.Get ("m4_lower_c_WZ") ;
  TH1F * m4_lower_c_Wjet = (TH1F *) input.Get ("m4_lower_c_Wjet") ;
  TH1F * m4_lower_c_ZZ   = (TH1F *) input.Get ("m4_lower_c_ZZ") ;
  TH1F * m4_lower_c_top  = (TH1F *) input.Get ("m4_lower_c_top") ;
  
  TH1F * m4_lower_c_total = (TH1F*) m4_lower_c_DY->Clone ("m4_lower_c_total");
  if(subtractDY   == true)  m4_lower_c_total->Reset();
  if(subtractTT   == false) m4_lower_c_total->Add(m4_lower_c_TT);
  if(subtractWW   == false) m4_lower_c_total->Add(m4_lower_c_WW);
  if(subtractWZ   == false) m4_lower_c_total->Add(m4_lower_c_WZ);
  if(subtractWjet == false) m4_lower_c_total->Add(m4_lower_c_Wjet);
  if(subtractZZ   == false) m4_lower_c_total->Add(m4_lower_c_ZZ);
  if(subtracttop  == false) m4_lower_c_total->Add(m4_lower_c_top);     
  m4_lower_c_total->SetTitle ("");

  //lower_a
  TH1F * m4_lower_a_DY   = (TH1F *) input.Get ("m4_lower_a_DY") ;
  TH1F * m4_lower_a_TT   = (TH1F *) input.Get ("m4_lower_a_TT") ;
  TH1F * m4_lower_a_WW   = (TH1F *) input.Get ("m4_lower_a_WW") ;
  TH1F * m4_lower_a_WZ   = (TH1F *) input.Get ("m4_lower_a_WZ") ;
  TH1F * m4_lower_a_Wjet = (TH1F *) input.Get ("m4_lower_a_Wjet") ;
  TH1F * m4_lower_a_ZZ   = (TH1F *) input.Get ("m4_lower_a_ZZ") ;
  TH1F * m4_lower_a_top  = (TH1F *) input.Get ("m4_lower_a_top") ;
  
  TH1F * m4_lower_a_total = (TH1F*) m4_lower_a_DY->Clone ("m4_lower_a_total");
  if(subtractDY   == true)  m4_lower_a_total->Reset();
  if(subtractTT   == false) m4_lower_a_total->Add(m4_lower_a_TT);
  if(subtractWW   == false) m4_lower_a_total->Add(m4_lower_a_WW);
  if(subtractWZ   == false) m4_lower_a_total->Add(m4_lower_a_WZ);
  if(subtractWjet == false) m4_lower_a_total->Add(m4_lower_a_Wjet);
  if(subtractZZ   == false) m4_lower_a_total->Add(m4_lower_a_ZZ);
  if(subtracttop  == false) m4_lower_a_total->Add(m4_lower_a_top);     
  m4_lower_a_total->SetTitle ("");
  
  //sideband_total
  TH1F * m4_sideband_DY   = (TH1F *) input.Get ("m4_sideband_DY") ;
  TH1F * m4_sideband_TT   = (TH1F *) input.Get ("m4_sideband_TT") ;
  TH1F * m4_sideband_WW   = (TH1F *) input.Get ("m4_sideband_WW") ;
  TH1F * m4_sideband_WZ   = (TH1F *) input.Get ("m4_sideband_WZ") ;
  TH1F * m4_sideband_Wjet = (TH1F *) input.Get ("m4_sideband_Wjet") ;
  TH1F * m4_sideband_ZZ   = (TH1F *) input.Get ("m4_sideband_ZZ") ;
  TH1F * m4_sideband_top  = (TH1F *) input.Get ("m4_sideband_top") ;
  
  TH1F * m4_sideband_total = (TH1F*) m4_sideband_DY->Clone ("m4_sideband_total");
  if(subtractDY   == true)  m4_sideband_total->Reset();
  if(subtractTT   == false) m4_sideband_total->Add(m4_sideband_TT);
  if(subtractWW   == false) m4_sideband_total->Add(m4_sideband_WW);
  if(subtractWZ   == false) m4_sideband_total->Add(m4_sideband_WZ);
  if(subtractWjet == false) m4_sideband_total->Add(m4_sideband_Wjet);
  if(subtractZZ   == false) m4_sideband_total->Add(m4_sideband_ZZ);
  if(subtracttop  == false) m4_sideband_total->Add(m4_sideband_top);
  m4_sideband_total->SetTitle ("");
  THStack * stack_m4_sideband = new THStack("stack_m4_sideband","");
  if(subtractDY   == false) stack_m4_sideband->Add(m4_sideband_DY);
  if(subtractTT   == false) stack_m4_sideband->Add(m4_sideband_TT);
  if(subtractWW   == false) stack_m4_sideband->Add(m4_sideband_WW);
  if(subtractWZ   == false) stack_m4_sideband->Add(m4_sideband_WZ);
  if(subtractWjet == false) stack_m4_sideband->Add(m4_sideband_Wjet);
  if(subtractZZ   == false) stack_m4_sideband->Add(m4_sideband_ZZ);
  if(subtracttop  == false) stack_m4_sideband->Add(m4_sideband_top);
  
  //FC get the signal samples
  THStack * stack_m4_EvenHigher_SIG = (THStack *) input.Get ("stack_m4_EvenHigher_SIG") ;
  TH1F * m4_EvenHigher_total_SIG = (TH1F*) stack_m4_EvenHigher_SIG->GetStack ()->Last () ;    
  m4_EvenHigher_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_upper_SIG = (THStack *) input.Get ("stack_m4_upper_SIG") ;
  TH1F * m4_upper_total_SIG = (TH1F*) stack_m4_upper_SIG->GetStack ()->Last () ;
  m4_upper_total_SIG->SetTitle ("") ;
      
  THStack * stack_m4_upper_a_SIG = (THStack *) input.Get ("stack_m4_upper_a_SIG") ;
  TH1F * m4_upper_a_total_SIG = (TH1F*) stack_m4_upper_a_SIG->GetStack ()->Last () ;
  m4_upper_a_total_SIG->SetTitle ("") ;
      
  THStack * stack_m4_upper_c_SIG = (THStack *) input.Get ("stack_m4_upper_c_SIG") ;
  TH1F * m4_upper_c_total_SIG = (TH1F*) stack_m4_upper_c_SIG->GetStack ()->Last () ;
  m4_upper_c_total_SIG->SetTitle ("") ;
      
  THStack * stack_m4_lower_SIG = (THStack *) input.Get ("stack_m4_lower_SIG") ;
  TH1F * m4_lower_total_SIG = (TH1F *) stack_m4_lower_SIG->GetStack ()->Last () ;
  m4_lower_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_lower_a_SIG = (THStack *) input.Get ("stack_m4_lower_a_SIG") ;
  TH1F * m4_lower_a_total_SIG = (TH1F *) stack_m4_lower_a_SIG->GetStack ()->Last () ;
  m4_lower_a_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_lower_c_SIG = (THStack *) input.Get ("stack_m4_lower_c_SIG") ;
  TH1F * m4_lower_c_total_SIG = (TH1F *) stack_m4_lower_c_SIG->GetStack ()->Last () ;
  m4_lower_c_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_signal_SIG = (THStack *) input.Get ("stack_m4_signal_SIG") ;
  TH1F * m4_signal_total_SIG = (TH1F *) stack_m4_signal_SIG->GetStack ()->Last () ;
  m4_signal_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_sideband_SIG = (THStack *) input.Get ("stack_m4_sideband_SIG") ;
  TH1F * m4_sideband_total_SIG = (TH1F *) stack_m4_sideband_SIG->GetStack ()->Last () ;
  m4_sideband_total_SIG->SetTitle ("") ;
  
  //FC get the data
  TH1F * m4_EvenHigher_DATA = (TH1F *) input.Get ("m4_EvenHigher_DATA") ;      
  m4_EvenHigher_DATA->SetTitle ("") ;
  TH1F * m4_upper_DATA = (TH1F *) input.Get ("m4_upper_DATA") ;      
  m4_upper_DATA->SetTitle ("") ;
  TH1F * m4_upper_a_DATA = (TH1F *) input.Get ("m4_upper_a_DATA") ;      
  m4_upper_a_DATA->SetTitle ("") ;
  TH1F * m4_upper_c_DATA = (TH1F *) input.Get ("m4_upper_c_DATA") ;      
  m4_upper_c_DATA->SetTitle ("") ;
  TH1F * m4_signal_DATA = (TH1F *) input.Get ("m4_signal_DATA") ;    
  m4_signal_DATA->SetTitle ("") ;
  TH1F * m4_lower_DATA = (TH1F *) input.Get ("m4_lower_DATA") ;      
  m4_lower_DATA->SetTitle ("") ;
  TH1F * m4_lower_a_DATA = (TH1F *) input.Get ("m4_lower_a_DATA") ;      
  m4_lower_a_DATA->SetTitle ("") ;
  TH1F * m4_lower_c_DATA = (TH1F *) input.Get ("m4_lower_c_DATA") ;      
  m4_lower_c_DATA->SetTitle ("") ;
  TH1F * m4_sideband_DATA = (TH1F *) input.Get ("m4_sideband_DATA") ;
  m4_sideband_DATA->SetTitle ("") ;

  
  //PG which histograms I use = possible analysis configurations
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  TH1F* signalRegionWW;
  TH1F* signalRegionDY;
  TH1F* signalRegionTT;
  TH1F* signalRegiontop;
  TH1F* signalRegionWZ;
  TH1F* signalRegionWjet;
  TH1F* signalRegionZZ;
  TH1F* signalRegion;
  
 if(martijn)
 { 
   cout << "Martijn\'s test" << endl ;
   TH1F * sidebaRegionMC = (TH1F *) m4_upper_a_total->Clone ("sidebaRegionMC") ; 
   sidebaRegionMC->Add (m4_lower_a_total) ;
   TH1F * signalRegionMC = (TH1F *) m4_upper_c_total->Clone ("signalRegionMC") ; 
   signalRegionMC->Add (m4_lower_c_total) ;
  
   //Subtract the MC shape from data in the sideband region 
   if(subtractWW   == true) m4_upper_a_DATA->Add (m4_upper_a_WW, -1);   
   if(subtractWW   == true) m4_lower_a_DATA->Add (m4_lower_a_WW, -1);
   if(subtractDY   == true) m4_upper_a_DATA->Add (m4_upper_a_DY, -1);   
   if(subtractDY   == true) m4_lower_a_DATA->Add (m4_lower_a_DY, -1);
   if(subtractTT   == true) m4_upper_a_DATA->Add (m4_upper_a_TT, -1);   
   if(subtractTT   == true) m4_lower_a_DATA->Add (m4_lower_a_TT, -1);
   if(subtracttop  == true) m4_upper_a_DATA->Add (m4_upper_a_top, -1);   
   if(subtracttop  == true) m4_lower_a_DATA->Add (m4_lower_a_top, -1);
   if(subtractWZ   == true) m4_upper_a_DATA->Add (m4_upper_a_WZ, -1);   
   if(subtractWZ   == true) m4_lower_a_DATA->Add (m4_lower_a_WZ, -1);
   if(subtractWjet == true) m4_upper_a_DATA->Add (m4_upper_a_Wjet, -1);   
   if(subtractWjet == true) m4_lower_a_DATA->Add (m4_lower_a_Wjet, -1);
   if(subtractZZ   == true) m4_upper_a_DATA->Add (m4_upper_a_ZZ, -1);   
   if(subtractZZ   == true) m4_lower_a_DATA->Add (m4_lower_a_ZZ, -1);

   
   TH1F * sidebaRegion = (TH1F *) m4_upper_a_DATA->Clone ("sidebaRegion") ; 
   sidebaRegion->Add (m4_lower_a_DATA) ;

   //FC FIXME: not really rigorous; check for other solutions!!!
   for (int i = 0; i <= sidebaRegion->GetNbinsX(); i++) {
     if (sidebaRegion->GetBinContent(i) < 0) { sidebaRegion->SetBinContent(i,0.) ;  sidebaRegion->SetBinError(i,0.) ; }
   }  
 
   if(subtractWW   == true) { 
     signalRegionWW = (TH1F *) m4_upper_c_WW->Clone ("signalRegionWW") ;  
     signalRegionWW->Add (m4_lower_c_WW) ;
   }
   if(subtractDY   == true) {
     signalRegionDY = (TH1F *) m4_upper_c_DY->Clone ("signalRegionDY") ;  
     signalRegionDY->Add (m4_lower_c_DY) ;
   }
   if(subtractTT   == true) { 
     signalRegionTT = (TH1F *) m4_upper_c_TT->Clone ("signalRegionTT") ;  
     signalRegionTT->Add (m4_lower_c_TT) ;
   }
   if(subtracttop  == true) {
     signalRegiontop = (TH1F *) m4_upper_c_top->Clone ("signalRegiontop") ;  
     signalRegiontop->Add (m4_lower_c_top) ;
   }
   if(subtractWZ   == true) { 
     signalRegionWZ = (TH1F *) m4_upper_c_WZ->Clone ("signalRegionWZ") ;  
     signalRegionWZ->Add (m4_lower_c_WZ) ;
   }
   if(subtractWjet == true) {
     signalRegionWjet = (TH1F *) m4_upper_c_Wjet->Clone ("signalRegionWjet") ;  
     signalRegionWjet->Add (m4_lower_c_Wjet) ;
   }
   if(subtractZZ   == true) {
     signalRegionZZ = (TH1F *) m4_upper_c_ZZ->Clone ("signalRegionZZ") ;  
     signalRegionZZ->Add (m4_lower_c_ZZ) ;
   }


   signalRegion = (TH1F *) m4_upper_c_DATA->Clone ("signalRegion") ; 
   signalRegion->Add (m4_lower_c_DATA) ; 
 }

 if(final)
 {
   cout << "final analysis" << endl ;
   
   TH1F * sidebaRegionMC = (TH1F *) m4_sideband_total->Clone ("sidebaRegionMC") ; 
   
   TH1F * signalRegionMC = (TH1F *) m4_signal_total->Clone ("signalRegionMC") ;
  
   //Subtract the shape from data in the sideband region
   if(subtractWW   == true) m4_sideband_DATA->Add (m4_sideband_WW, -1) ;
   if(subtractDY   == true) m4_sideband_DATA->Add (m4_sideband_DY, -1) ;
   if(subtractTT   == true) m4_sideband_DATA->Add (m4_sideband_TT, -1) ;
   if(subtracttop  == true) m4_sideband_DATA->Add (m4_sideband_top, -1) ;
   if(subtractWZ   == true) m4_sideband_DATA->Add (m4_sideband_WZ, -1) ;
   if(subtractWjet == true) m4_sideband_DATA->Add (m4_sideband_Wjet, -1) ;
   if(subtractZZ   == true) m4_sideband_DATA->Add (m4_sideband_ZZ, -1) ;

   TH1F * sidebaRegion   = (TH1F *) m4_sideband_DATA->Clone ("sidebaRegion") ;
   
   TH1F * signalRegion   = (TH1F *) m4_signal_DATA->Clone ("signalRegion") ;
  
   if(subtractWW   == true) TH1F * signalRegionWW = (TH1F *) m4_signal_WW->Clone ("signalRegionWW") ;
   if(subtractDY   == true) TH1F * signalRegionDY = (TH1F *) m4_signal_DY->Clone ("signalRegionDY") ;
   if(subtractTT   == true) TH1F * signalRegionTT = (TH1F *) m4_signal_TT->Clone ("signalRegionTT") ;
   if(subtracttop  == true) TH1F * signalRegiontop = (TH1F *) m4_signal_top->Clone ("signalRegiontop") ;
   if(subtractWZ   == true) TH1F * signalRegionWZ = (TH1F *) m4_signal_WZ->Clone ("signalRegionWZ") ;
   if(subtractWjet == true) TH1F * signalRegionWjet = (TH1F *) m4_signal_Wjet->Clone ("signalRegionWjet") ;
   if(subtractZZ   == true) TH1F * signalRegionZZ = (TH1F *) m4_signal_ZZ->Clone ("signalRegionZZ") ;
 }

 TH1F * sidebaRegionMC = (TH1F *) m4_sideband_total->Clone ("sidebaRegionMC") ; 
 TH1F * signalRegionMC = (TH1F *) m4_signal_total->Clone ("signalRegionMC") ;  
 TH1F * sidebaRegion   = (TH1F *) sidebaRegionMC->Clone ("sidebaRegion") ; 
 signalRegion   = (TH1F *) signalRegionMC->Clone ("signalRegion") ; 
 if(closure)
 {
   cout << "final analysis closure test" << endl ;
   if(subtractWW   == true) TH1F * signalRegionWW = (TH1F *) m4_signal_WW->Clone ("signalRegionWW") ; 
   if(subtractDY   == true) TH1F * signalRegionDY = (TH1F *) m4_signal_DY->Clone ("signalRegionDY") ; 
   if(subtractTT   == true) TH1F * signalRegionTT = (TH1F *) m4_signal_TT->Clone ("signalRegionTT") ; 
   if(subtracttop  == true) TH1F * signalRegiontop = (TH1F *) m4_signal_top->Clone ("signalRegiontop") ; 
   if(subtractWZ   == true) TH1F * signalRegionWZ = (TH1F *) m4_signal_WZ->Clone ("signalRegionWZ") ; 
   if(subtractWjet == true) TH1F * signalRegionWjet = (TH1F *) m4_signal_Wjet->Clone ("signalRegionWjet") ; 
   if(subtractZZ   == true) TH1F * signalRegionZZ = (TH1F *) m4_signal_ZZ->Clone ("signalRegionZZ") ; 
 }
 
  //PG plot MC signal and background region 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  int nBins =  m4_signal_DATA->GetNbinsX () ;
  double m4_min = m4_signal_DATA->GetXaxis ()->GetXmin () ;
  double m4_max = m4_signal_DATA->GetXaxis ()->GetXmax () ;
 
  double binSize = (m4_max - m4_min) / nBins ;
  double startFit = 225; //GeV, bin from where to start the num & den fit for corr factor
  double endFit = 800;   //GeV, bin from where to end the num & den fit for corr factor
  
  int fitBins = (endFit-startFit) / binSize ;

  cout << nBins << " " << m4_min << " " << m4_max << " " << binSize << endl ;
  

  //PG correction factor from the ratio of histograms 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * ratio_total = (TH1F *) signalRegionMC->Clone ("ratio") ;
  ratio_total->Divide (sidebaRegionMC) ;
  ratio_total->SetMarkerColor (kOrange) ;

  
  //PG fit separately numerator and denominator of MC 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  TF1 * numFitFunc ;
  if (!attenuated)
    {
      numFitFunc = new TF1 ("numFitFunc", doubleExponential, 0., 1000., 4);
      setDoubleExpPars (numFitFunc);            
    } else
    {
      numFitFunc = new TF1 ("numFitFunc", attenuatedDoubleExponential, 0., 1000., 6);
      setAttenuatedDoubleExpPars (numFitFunc);        
    }
  TFitResultPtr fitResultPtr = signalRegionMC->Fit (numFitFunc, "L", "", startFit, endFit) ;
  int fitStatus = (int)(fitResultPtr) ;
  int loops = 0 ; 
  while (fitStatus != 0 && loops < 30)
    {
      fitResultPtr = signalRegionMC->Fit (numFitFunc, "L", "", startFit, endFit) ;
      fitStatus = (int)(fitResultPtr) ;
      ++loops ;
    }
  cout << "`--> " << fitStatus << " @ " << loops << "\n" ;

  TH1F * num_fit_error = new TH1F ("num_fit_error", "", fitBins, startFit, endFit) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (num_fit_error, 0.68) ;

  TCanvas* c1 = new TCanvas();
  num_fit_error->SetFillColor (kGray+2) ;
  signalRegionMC->Draw () ;
  num_fit_error->Draw ("E3same") ;
  signalRegionMC->Draw ("sames") ;
  c1->Print ("01_numerator_MC_fit.png", "png") ;
  c1->SetLogy () ;
  signalRegionMC->Draw () ;
  num_fit_error->Draw ("E3same") ;
  signalRegionMC->Draw ("sames") ;
  c1->Print ("02_numerator_MC_fit_log.png", "png") ;
  c1->SetLogy (0) ;

  signalRegion->Draw () ;
  c1->Print ("03_signal.png", "png") ;
  c1->SetLogy () ;
  signalRegion->Draw () ;
  c1->Print ("04_signal_log.png", "png") ;
  c1->SetLogy (0) ;

  TF1 * denFitFunc ;
  if (!attenuated)
    {  
      denFitFunc = new TF1 ("denFitFunc", doubleExponential, 0., 1000., 4);
      setDoubleExpPars (denFitFunc);                
    } else
    {
      denFitFunc = new TF1 ("denFitFunc", attenuatedDoubleExponential, 0., 1000., 6);
      setAttenuatedDoubleExpPars (denFitFunc);        
    }
  
  fitResultPtr = sidebaRegionMC->Fit (denFitFunc, "L", "", startFit, endFit) ;
  fitStatus = (int)(fitResultPtr) ;
  loops = 0 ; 
  while (fitStatus != 0 && loops < 30)
    {
      fitResultPtr = sidebaRegionMC->Fit (denFitFunc, "L", "", startFit, endFit) ;
      fitStatus = (int)(fitResultPtr) ;
      ++loops ;
    }
  cout << "`--> " << fitStatus << " @ " << loops << "\n" ;

  TH1F * den_fit_error = new TH1F ("den_fit_error", "", fitBins, startFit, endFit) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (den_fit_error, 0.68) ;

  den_fit_error->SetFillColor (kGray+2) ;
  sidebaRegionMC->Draw () ;
  den_fit_error->Draw ("E3same") ;
  sidebaRegionMC->Draw ("sames") ;
  c1->Print ("05_denominator_MC_fit.png", "png") ;
  c1->SetLogy () ;
  sidebaRegionMC->Draw () ;
  den_fit_error->Draw ("E3same") ;
  sidebaRegionMC->Draw ("sames") ;
  c1->Print ("06_denominator_MC_fit_log.png", "png") ;
  c1->SetLogy (0) ;

  sidebaRegion->Draw () ;
  c1->Print ("07_sideband.png", "png") ;
  c1->SetLogy () ;
  sidebaRegion->Draw () ;
  c1->Print ("08_sideband_log.png", "png") ;
  c1->SetLogy (0) ;

  TH1F * h_correctionBand ;

  //PG toy experiments to determine the size of the error band on the extrapolation factor
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  if (histosRatio)
    {
      h_correctionBand = (TH1F *) ratio_total->Clone ("h_correctionBand") ; 
      if (makeToys)  
        {  
          TRandom3 r ;
          TH2F * correctionPlane = new TH2F ("correctionPlane", "", nBins, m4_min, m4_max, 600, 0, 9) ;
        
          TH1F * dummyNum = (TH1F *) signalRegionMC->Clone ("dummyNum") ;
          TH1F * dummyDen = (TH1F *) sidebaRegionMC->Clone ("dummyDen") ;
          double intSignal = signalRegionMC->Integral () ;   //PG put in the W+jets MC statistics for the uncertainty
          double intSideband = sidebaRegionMC->Integral () ; //PG put in the W+jets MC statistics for the uncertainty
         
          for (int iToy = 0 ; iToy < nToys ; ++iToy)
            {
              if (iToy %(nToys/10) == 0) cout << "toy number " << iToy << endl ;
        
              int nNum = r.Poisson (intSignal) ;
              dummyNum->Reset () ;
              dummyNum->FillRandom ("numFitFunc", nNum) ;
        
              int nDen = r.Poisson (intSideband) ;
              dummyDen->Reset () ;
              dummyDen->FillRandom ("denFitFunc", nDen) ;
        
              dummyNum->Divide (dummyDen) ;
              for (int iBin = 1 ; iBin <= dummyNum->GetNbinsX () ; ++iBin)
                {
                  correctionPlane->Fill (dummyNum->GetBinCenter (iBin), dummyNum->GetBinContent (iBin)) ;
                }
            }
    
          //PG correction factor from the profile of the many toys
          //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
    
          TProfile * correctionBand = correctionPlane->ProfileX ("correctionBand", 1, -1, "s") ;
          h_correctionBand = dumpProfile ("h_correctionBand", correctionBand) ;
    
          //PG use the ratio of functions as central value
          //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
          
          for (int iBin = 1 ; iBin <= h_correctionBand->GetNbinsX () ; ++iBin)
            {
              double center = h_correctionBand->GetBinCenter (iBin) ;
              double corr = numFitFunc->Eval (center) / denFitFunc->Eval (center) ; 
              h_correctionBand->SetBinContent (iBin, corr) ;
            }
    
          //PG correction factor from the gaussian fit to slices of the many toys
          //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
          //PG FIXME this needs to be extracted from the loop
        
          TObjArray aSlices;
          correctionPlane->FitSlicesY (0, 0, -1, 0, "QNRL", &aSlices) ; //PG error comes from the rms of the toys histo
          TH1F * gaussianBand = (TH1F *) aSlices.At (1)->Clone ("gaussianBand") ;
          for (int iBin = 1 ; iBin <= gaussianBand->GetNbinsX () ; ++iBin) //PG error comes from the gaussian fit in each slice
            {
              double sigma = ((TH1F *) aSlices.At (2))->GetBinContent (iBin) ;
              gaussianBand->SetBinError (iBin, sigma) ;
            }
          gaussianBand->SetFillStyle (3002) ;
          gaussianBand->SetFillColor (kGreen + 2) ;
      } //PG if maketoys
    } //PG if histosRatio
  else 
    {
      // prepare the histo
      h_correctionBand = (TH1F *) signalRegionMC->Clone ("h_correctionBand") ;
      h_correctionBand->Reset () ;
      for (int iBin = 1 ; iBin <= h_correctionBand->GetNbinsX () ; ++iBin)
        {
          // fill the histo central values
          double center = h_correctionBand->GetBinCenter (iBin) ;

          // skip the bins where the num and den functions don't exist
          if (center < startFit || center > endFit) continue ;

          double corr = numFitFunc->Eval (center) / denFitFunc->Eval (center) ; 
          h_correctionBand->SetBinContent (iBin, corr) ;
          // fill the histo errors
          int binForError = num_fit_error->GetXaxis ()->FindBin (center) ;
          double error = 0. ;
          //PG under the hipothesis of having the error only for the bins of the fit, need to check this first
          if (binForError > 0 && binForError <= num_fit_error->GetNbinsX ())
            {
              double numErr = num_fit_error->GetBinError (binForError) / num_fit_error->GetBinContent (binForError) ;
              double denErr = den_fit_error->GetBinError (binForError)  / den_fit_error->GetBinContent (binForError) ;
              error = sqrt (numErr * numErr + denErr * denErr) ;
              error *= h_correctionBand->GetBinContent (iBin) ;
            }

          h_correctionBand->SetBinError (iBin, error) ;
        }
    } //PG else (if maketoys)

  h_correctionBand->SetStats (0) ;
  h_correctionBand->SetFillColor (kOrange) ;
  h_correctionBand->SetMarkerStyle (29) ;
   
  TLegend * leg_correctionFactor = new TLegend (0.5, 0.8, 0.95, 0.95, NULL, "brNDC") ;
  legendMaquillage (leg_correctionFactor) ;
  leg_correctionFactor->AddEntry (h_correctionBand, "correction band", "pf") ;
  leg_correctionFactor->AddEntry (ratio_total, "MC ratio", "pl") ;

  c1->DrawFrame (100, 0.01, 800, 2.) ;
  h_correctionBand->Draw ("E3same") ;
  h_correctionBand->Draw ("Psame") ;
  gStyle->SetPalette (1) ;
  ratio_total->Draw ("same") ;
  leg_correctionFactor->Draw () ;
  c1->Print ("09_correctionFactor.png", "png") ;
 
    
  //PG fit the data sideband before the extrapolation
  TH1F * sideband_bkg;
  TH1F * sideband_bkg_fitBand;
  TH1F sidebandFitPull ("sidebandFitPull", "", 50, -5, 5) ;
  if(fitsideband)
  {
    //FC ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   
    //FC fit the sideband shape (clone not to get the function on the sideband histo in the closure test)
    TH1F * sideband_bkg = (TH1F *) sidebaRegion->Clone ("sideband_bkg") ;

    TF1 * bkgFitFunc ;
    if (!attenuated)
      {  
        bkgFitFunc = new TF1 ("bkgFitFunc", doubleExponential, 0., 1000., 4);
        setDoubleExpPars (bkgFitFunc);               
      } else
      {
        bkgFitFunc = new TF1 ("bkgFitFunc", attenuatedDoubleExponential, 0., 1000., 6);
        setAttenuatedDoubleExpPars (bkgFitFunc);        
      }
    
    TFitResultPtr fitResultPtr2 = sideband_bkg->Fit (bkgFitFunc, "L", "", startFit, endFit) ;
    fitStatus = (int)(fitResultPtr2) ;
    loops = 0 ; 
    while (fitStatus != 0 && loops < 30)
    {
      fitResultPtr2 = sideband_bkg->Fit (bkgFitFunc, "L", "", startFit, endFit) ;
      fitStatus = (int)(fitResultPtr2) ;
      ++loops ;
    }
    cout << "`--> " << fitStatus << " @ " << loops << "\n" ;
    
    sideband_bkg_fitBand = (TH1F *) sidebaRegion->Clone("sideband_bkg_fitBand");
    sideband_bkg_fitBand->Reset();
    (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (sideband_bkg_fitBand, 0.68) ;

    //FC set the error to this estimate of the background
    for (int iBin = 1 ; iBin <= sideband_bkg->GetNbinsX () ; ++iBin)
    {
      double center = sideband_bkg->GetBinCenter (iBin) ;
      
      if (center < startFit || center > endFit) {
      
        sideband_bkg_fitBand->SetBinContent(iBin, 0.) ;
        sideband_bkg_fitBand->SetBinError(iBin, 0.);
      }
    }

    c1->SetLogy () ;
    sideband_bkg->SetFillColor(kPink) ;
    sideband_bkg->Draw ("E3") ;
    sideband_bkg->SetStats(1);
    sideband_bkg_fitBand->SetFillStyle (3001) ;
    sideband_bkg_fitBand->SetFillColor (kBlue) ;
    sideband_bkg_fitBand->Draw ("E3same") ;
    c1->Print ("10_sidebandFittedBackground.png", "png") ;
    c1->SetLogy (0) ;
    c1->Update () ;
    c1->Print ("11_sidebandFittedBackground_lin.png", "png") ;

    int startFitBin = sideband_bkg->FindBin(startFit);
    int endFitBin   = sideband_bkg->FindBin(endFit);
    for (int iBin = startFitBin ; iBin <= endFitBin ; ++iBin) 
      {
        double num        = sideband_bkg->GetBinContent (iBin) ;
        double mean       = sideband_bkg_fitBand->GetBinContent (iBin) ;
        double sigma_num  = sideband_bkg->GetBinError (iBin) ;
        double sigma_mean = sideband_bkg_fitBand->GetBinError (iBin) ;
      
        double sigma = sqrt(sigma_num*sigma_num + sigma_mean*sigma_mean) ;
      
        sidebandFitPull.Fill ((num - mean) / sigma) ;
      }
  
    sidebandFitPull.Fit ("gaus","QL") ;
    c1->Print ("12_sidebandFit_pull.png", "png") ;
  
  } //PG if(fitsideband)

  //FC calculate the extrapolated background
  TH1F * extrapolated_bkg;
  if( fitsideband == true )  extrapolated_bkg = (TH1F *) sideband_bkg_fitBand->Clone ("extrapolated_bkg") ;
  if( fitsideband == false ) extrapolated_bkg = (TH1F *) sidebaRegion->Clone ("extrapolated_bkg") ;
  extrapolated_bkg->Multiply(h_correctionBand);
  
  extrapolated_bkg->SetStats (0) ;
  extrapolated_bkg->SetTitle ("") ;
  extrapolated_bkg->SetLineColor (kBlack) ;
  extrapolated_bkg->SetFillColor (kOrange) ;

  if(subtractWW   == true) { extrapolated_bkg->Add(signalRegionWW, 1);   signalRegionMC->Add(signalRegionWW, 1);}
  if(subtractDY   == true) { extrapolated_bkg->Add(signalRegionDY, 1);   signalRegionMC->Add(signalRegionDY, 1);}
  if(subtractTT   == true) { extrapolated_bkg->Add(signalRegionTT, 1);   signalRegionMC->Add(signalRegionTT, 1);}
  if(subtracttop  == true) { extrapolated_bkg->Add(signalRegiontop, 1);  signalRegionMC->Add(signalRegiontop, 1);}
  if(subtractWjet == true) { extrapolated_bkg->Add(signalRegionWjet, 1); signalRegionMC->Add(signalRegionWjet, 1);}
  if(subtractWZ   == true) { extrapolated_bkg->Add(signalRegionWZ, 1);   signalRegionMC->Add(signalRegionWZ, 1);}
  if(subtractZZ   == true) { extrapolated_bkg->Add(signalRegionZZ, 1);   signalRegionMC->Add(signalRegionZZ, 1);}

//PG ---- ---- ---- riletto fino a qui ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
  
  

  if(fitbkg)
  {
    //LS fit the background
    TF1 * bkgFitFunc ;
    if (!attenuated)
      {
        bkgFitFunc = new TF1 ("bkgFitFunc", doubleExponential, 0., 1000., 4);
        setDoubleExpPars (bkgFitFunc);
      } else
      {   
        bkgFitFunc = new TF1 ("bkgFitFunc", attenuatedDoubleExponential, 0., 1000., 6);
        setAttenuatedDoubleExpPars (bkgFitFunc);    
      }
  
    TFitResultPtr fitResultPtr3 = extrapolated_bkg->Fit (bkgFitFunc, "L", "", startFit, endFit) ;
    fitStatus = (int)(fitResultPtr3) ;
    loops = 0 ; 
    while (fitStatus != 0 && loops < 30)
      {
        fitResultPtr3 = extrapolated_bkg->Fit (bkgFitFunc, "L", "", startFit, endFit) ;
        fitStatus = (int)(fitResultPtr3) ;
        ++loops ;
      }
    cout << "`--> " << fitStatus << " @ " << loops << "\n" ;

    TH1F * extrapolated_bkg_fitBand = new TH1F ("extrapolated_bkg_fitBand", "", fitBins, startFit, endFit) ;
    (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (extrapolated_bkg_fitBand, 0.68) ;
  
    c1->SetLogy () ;
    extrapolated_bkg->Draw ("E3") ;
    extrapolated_bkg_fitBand->SetFillStyle (3001) ;
    extrapolated_bkg_fitBand->SetFillColor (kBlue) ;
    extrapolated_bkg_fitBand->Draw ("E3same") ;
    c1->Print ("13_fittedBackground.png", "png") ;
    c1->SetLogy (0) ;
    c1->Update () ;
    c1->Print ("14_fittedBackground_lin.png", "png") ;

    
    //LS set the error to this estimate of the background
    for (int iBin = 1 ; iBin <= extrapolated_bkg->GetNbinsX () ; ++iBin)
      {
        double center = extrapolated_bkg->GetBinCenter (iBin) ;
        int iBinBand = extrapolated_bkg_fitBand->GetXaxis()->FindBin (center) ;
        if(iBinBand == 0) continue;

        double errBkg = 0. ;
        if(extrapolated_bkg->GetBinContent (iBin)==0||extrapolated_bkg_fitBand->GetBinContent (iBinBand)==0){
            cout << "warning!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! denominator = 0" << endl;
            continue;
        }
        
        errBkg = extrapolated_bkg->GetBinError (iBin) / extrapolated_bkg->GetBinContent (iBin) ;
        double errFit = 0. ;
        errFit = extrapolated_bkg_fitBand->GetBinError (iBinBand) /  extrapolated_bkg_fitBand->GetBinContent (iBinBand) ;

//         double errTot = sqrt (errFit * errFit + errBkg * errBkg) * extrapolated_bkg_fitBand->GetBinContent (iBinBand) ;
        double errTot = sqrt (errFit * errFit) * extrapolated_bkg_fitBand->GetBinContent (iBinBand) ;

        extrapolated_bkg->SetBinContent (iBin,extrapolated_bkg_fitBand->GetBinContent (iBinBand)); 
        extrapolated_bkg->SetBinError (iBin, errTot) ;
      }
  
  }

  extrapolated_bkg->Draw ("E3L") ;
  c1->Print ("15_extrapolatedBkg.png", "png") ;
  c1->SetLogy () ; 
  c1->Update () ; 
  c1->Print ("16_extrapolatedBkg_log.png", "png") ;
  c1->SetLogy (0) ; 

  
  // ---------------------------------------------
  
  TH1F* extrapolated_bkgUp   = (TH1F*) extrapolated_bkg->Clone("extrapolated_bkgUp");
  TH1F* extrapolated_bkgDown = (TH1F*) extrapolated_bkg->Clone("extrapolated_bkgDown");
  
  for (int iBin=1; iBin <= extrapolated_bkg->GetNbinsX(); iBin++) {
    
    double binContent = extrapolated_bkg->GetBinContent(iBin);
    double binError   = extrapolated_bkg->GetBinError(iBin);
    
    extrapolated_bkgUp  ->SetBinContent(iBin, binContent + binError);
    extrapolated_bkgDown->SetBinContent(iBin, binContent - binError);
    
  }
  
  //PG first plot of the result
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  signalRegionMC->SetStats (0) ;
  signalRegionMC->SetMarkerStyle (24) ;
  signalRegionMC->SetMarkerSize (1);
  signalRegionMC->SetMarkerColor (kBlack) ;
  
  TLegend * leg_extrapAndData = new TLegend (0.5, 0.8, 0.95, 0.95, NULL, "brNDC") ;
  legendMaquillage (leg_extrapAndData) ;
  leg_extrapAndData->AddEntry (signalRegion, "data in SR", "p") ;
  leg_extrapAndData->AddEntry (extrapolated_bkg, "extrapolated bkg in SR", "lf") ;

  c1->SetLogy () ;
  signalRegion->SetStats (0) ;
  extrapolated_bkg->Draw ("E3") ;
  signalRegion->SetMarkerStyle (20) ;
  signalRegion->Draw ("same") ;
  leg_extrapAndData->Draw () ;
  c1->Print ("17_extrapAndData.png", "png") ;
  c1->SetLogy (0) ;
  c1->Update () ;
  c1->Print ("18_extrapAndData_lin.png", "png") ;
  
  pair<TGraphErrors*, TGraphErrors*> extrapAndData_pulls =
    getPullTrend (signalRegion, extrapolated_bkg) ;
  extrapAndData_pulls.second->Draw ("AE3") ;
  extrapAndData_pulls.first->Draw ("samePE") ;
  c1->Print ("19_extrapAndData_pull.png", "png") ;

  TH1F * extrapAndData_pull = getPullPlot (signalRegion, extrapolated_bkg, startFit, endFit) ;
  extrapAndData_pull->Draw () ;
  c1->Print ("20_extrapAndData_pull2.png", "png") ;
  
  //PG prepare the windows for the fast cut-n-count thing
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<double> masses ;
  vector<double> mMin ;
  vector<double> mMax ;
  
  
//   masses.push_back (200.) ;  mMin.push_back (180) ; mMax.push_back (220) ; //PG Hwindow.push_back (24.) ;
  masses.push_back (250.) ;  mMin.push_back (225) ; mMax.push_back (275) ; //PG Hwindow.push_back (24.) ;
  masses.push_back (300.) ;  mMin.push_back (270) ; mMax.push_back (330) ; //PG Hwindow.push_back (27.) ;
  masses.push_back (350.) ;  mMin.push_back (310) ; mMax.push_back (390) ; //PG Hwindow.push_back (32.) ;
  masses.push_back (400.) ;  mMin.push_back (355) ; mMax.push_back (445) ; //PG Hwindow.push_back (40.) ;
  masses.push_back (450.) ;  mMin.push_back (390) ; mMax.push_back (510) ; //PG Hwindow.push_back (46.) ;
  masses.push_back (500.) ;  mMin.push_back (415) ; mMax.push_back (575) ; //PG Hwindow.push_back (54.) ;
  masses.push_back (550.) ;  mMin.push_back (470) ; mMax.push_back (610) ; //PG Hwindow.push_back (61.) ;
  masses.push_back (600.) ;  mMin.push_back (485) ; mMax.push_back (665) ; //PG Hwindow.push_back (68.) ;

  
  for (unsigned int iMass = 0 ; iMass < masses.size () ; ++iMass)
    {
      if (mMin.at (iMass) < startFit) cout << "WARNING: mass " << masses.at (iMass) << " not trustable\n";  
      if (mMax.at (iMass) > endFit) cout << "WARNING: mass " << masses.at (iMass) << " not trustable\n";  
    }
  
  //PG the fast cut-n-count thing
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  TGraphErrors g_total ;
  TGraphErrors g_background_count ;
  TGraph g_error ;
  
  //FC Some pull plots
  TGraphAsymmErrors g_pull_total ;
  TGraphErrors g_pull_forEveryBin ;
  
  TH1F *gPullHisto_forEveryBin = new TH1F("gPullHisto_forEveryBin","gPullHisto_forEveryBin",50,-5,5);
  
  ofstream outFile("./mass_window_counts.txt");  
  outFile << "mass" 
          << " : (" << "min"
          << "," << "max"
          << ") : " << "total" //PG the bin width 
          << " : " << "bkg_count"
          << " +- " << "bkg_count_error"
          << " | "
          << "expSignal" << " : "
          << "bkg_count" << " : "
          << "bkg_count_error / bkg_count" << " : "
          << "total" << " : "
          << "\n" ;

  double bkg_count_min = 99999999.;  //FC Needed for giving a fancier axis range, see later... 
  

  //PG loop on the mass points
  for (int i = 0 ; i < masses.size () ; ++i)
    {
      int minBin = -1;
      int maxBin = -1;
      int nBins = signalRegion -> GetNbinsX();
      float binWidth = signalRegion -> GetBinWidth(1);
      float minX = signalRegion -> GetBinLowEdge(1);
      float maxX = signalRegion -> GetBinLowEdge(nBins)+binWidth;
      
      for(int bin = 1; bin <= nBins; ++bin)
        {
          float binCenter = minX + 0.5*binWidth + binWidth*(bin-1);
          
          if( (binCenter >= mMin.at(i)) && (minBin == -1) ) minBin = bin;
          if( (binCenter >= mMin.at(i)) && (binCenter < mMax.at(i)) ) maxBin = bin;
        }

      double total = signalRegion->Integral (minBin, maxBin) ; //PG analysis
      double min = signalRegion->GetBinLowEdge (minBin) ;
      double max = signalRegion->GetBinLowEdge (maxBin + 1) ;
      
      int minBinBkg = -1 ;
      int maxBinBkg = -1 ;
      
      for(int binBkg = 1; binBkg <= extrapolated_bkg->GetNbinsX() ; ++binBkg)
        {
          float binCenter = extrapolated_bkg->GetBinLowEdge(1) 
                            + 0.5 * extrapolated_bkg->GetBinWidth(1) 
                            + extrapolated_bkg->GetBinWidth(1)*(binBkg-1);
          
          if( (binCenter >= mMin.at(i)) && (minBinBkg == -1) ) minBinBkg = binBkg;
          if( (binCenter >= mMin.at(i)) && (binCenter < mMax.at(i)) ) maxBinBkg = binBkg;
        }

      double minBkg = extrapolated_bkg->GetBinLowEdge (minBinBkg) ;
      double maxBkg = extrapolated_bkg->GetBinLowEdge (maxBinBkg + 1) ;


      double bkg_count = extrapolated_bkg->Integral (minBinBkg, maxBinBkg) ;
      double bkg_countUp = extrapolated_bkgUp->Integral (minBinBkg, maxBinBkg) ;
      double bkg_count_error = bkg_countUp - bkg_count ;     
      
      if (bkg_count < bkg_count_min) bkg_count_min = bkg_count;
      
      double expSignal = m4_signal_total_SIG->Integral (minBin, maxBin) ;

      outFile << "MH | " << masses.at (i) 
              << " : (" << min
              << "," << max
              << ") : " << total
              << " : " << bkg_count
              << " +- " << bkg_count_error
              << " | "
              << expSignal << " : "
              << bkg_count << " : "
              << bkg_count_error / bkg_count << " : "
              << total << " : "
              << endl;
        
      g_total.SetPoint (i, masses.at (i), total) ;
      g_total.SetPointError (i, 50., sqrt (total)) ;
      g_background_count.SetPoint (i, masses.at (i), bkg_count) ;
      g_background_count.SetPointError (i, 0., bkg_count_error) ;
      g_error.SetPoint (i, masses.at (i), sqrt (bkg_count_error * bkg_count_error + total)) ;
      
      //LS --- --- ---
      double error = sqrt(total + bkg_count_error*bkg_count_error) ;
      double pull = total - bkg_count;
      g_pull_total.SetPoint(i, masses.at (i), pull) ;
      g_pull_total.SetPointError(i,masses.at(i) - mMin.at(i), mMax.at(i) - masses.at(i), error, error) ;
    
      
    } //PG loop on the mass points


  //FC loop on the mass points
  int index = 0;
  int startFitBin = extrapolated_bkg->FindBin(startFit);
  int endFitBin   = extrapolated_bkg->FindBin(endFit);
  for (int i = startFitBin; i <= endFitBin ; i++)
    {
      //FC skip the entries below mMin or above mMax 
      if (signalRegion->GetBinCenter(i) < mMin.at(0) || 
	  signalRegion->GetBinCenter(i) > mMax.at(mMax.size()-1)) continue;

      double obs = signalRegion->GetBinContent(i) ;
      double exp = extrapolated_bkg->GetBinContent(i) ;
      double sigma_obs = sqrt(obs) ;
      double sigma_exp = extrapolated_bkg->GetBinError(i) ;
      
      double error = sqrt(sigma_obs*sigma_obs + sigma_exp*sigma_exp);     
      double pull = (obs - exp)/error;
      g_pull_forEveryBin.SetPoint(index,signalRegion->GetBinCenter(i),pull);
      g_pull_forEveryBin.SetPointError(index,binSize/2.,0.);
      
      gPullHisto_forEveryBin->Fill(pull);

      index++ ;
    }
    
    
  g_total.SetMarkerStyle (6) ;
  g_background_count.SetLineColor (kOrange) ;
  g_background_count.SetFillColor (kOrange) ;

  TLegend * leg_results = new TLegend (0.5, 0.8, 0.95, 0.95, NULL, "brNDC") ;
  legendMaquillage (leg_results) ;
  leg_results->AddEntry (&g_total, "measured events in signal window", "p") ;
  leg_results->AddEntry (&g_background_count, "expected bkg in signal window", "f") ;
  
  c1->SetLogy () ;
  g_background_count.SetMinimum(bkg_count_min - 300);  //FC For a fancier logarithmic Y-axis
  g_background_count.Draw ("AE3") ;
  g_total.Draw ("EPsame") ;
  leg_results->Draw () ;
  c1->Print ("21_results.png", "png") ;
  c1->SetLogy (0) ;

  c1->DrawFrame (200, 0, 700, 200) ;
  g_error.SetLineWidth (2) ;
  g_error.Draw ("L") ;
  c1->Print ("22_systematic_and_statistics.png", "png") ;
  
  
  g_pull_total.SetMarkerStyle(21);
  g_pull_total.SetMarkerSize(0.9);
  g_pull_total.GetYaxis()->SetTitle("total - expbkg");
  g_pull_total.Draw("AP");
  c1->Print ("23_resultsPull.png", "png") ;
  
  g_pull_forEveryBin.SetMarkerStyle(1);
  g_pull_forEveryBin.GetYaxis()->SetTitle("(total - exp bkg)/err");
  g_pull_forEveryBin.GetYaxis()->SetRangeUser(-3,3);
  g_pull_forEveryBin.Draw("AP");
  c1->Print ("24_resultsPull_forEveryBin.png", "png") ;
  
  gPullHisto_forEveryBin->Draw();
  gPullHisto_forEveryBin->Fit("gaus","QL");
  c1->Print ("25_resultsPull_forEveryBin_histo.png", "png") ;


  TFile output ("output_017.root", "recreate") ;
  output.cd () ;
  
  sidebaRegionMC->Write();
  sidebaRegion->Write();
  signalRegion->Write();
  signalRegionMC->Write();
  if (subtractWW == true) signalRegionWW->Write();
  
  num_fit_error->Write();
  den_fit_error->Write();

  ratio_total->Write();
  h_correctionBand->Write();
  
  if (fitsideband) sideband_bkg->Write();
  if (fitsideband) sideband_bkg_fitBand->Write();
  if (fitsideband) sidebandFitPull.Write();

  extrapolated_bkg->Write();
  extrapolated_bkgUp->Write();
  extrapolated_bkgDown->Write();
  
  g_total.Write ("g_total") ;
  g_background_count.Write ("g_background_count") ;
  g_error.Write ("g_error") ;
  
  g_pull_total.Write("g_pull_total");
  g_pull_forEveryBin.Write("g_pull_forEveryBin");
  gPullHisto_forEveryBin->Write();


  output.Close () ;

//  gApplication->Terminate (0) ;

}



/*
  KEY: TH1F	m4_upper_DY;1	DY
  KEY: TH1F	m4_upper_Gjet;1	Gjet
  KEY: TH1F	m4_upper_QCD;1	QCD
  KEY: TH1F	m4_upper_TT;1	TT
  KEY: TH1F	m4_upper_WW;1	WW
  KEY: TH1F	m4_upper_WZ;1	WZ
  KEY: TH1F	m4_upper_Wjet;1	Wjet
  KEY: TH1F	m4_upper_ZZ;1	ZZ
  KEY: TH1F	m4_upper_top;1	top
  KEY: THStack	stack_m4_upper;1	

  KEY: TH1F	m4_signal_DY;1	DY
  KEY: TH1F	m4_signal_Gjet;1	Gjet
  KEY: TH1F	m4_signal_QCD;1	QCD
  KEY: TH1F	m4_signal_TT;1	TT
  KEY: TH1F	m4_signal_WW;1	WW
  KEY: TH1F	m4_signal_WZ;1	WZ
  KEY: TH1F	m4_signal_Wjet;1	Wjet
  KEY: TH1F	m4_signal_ZZ;1	ZZ
  KEY: TH1F	m4_signal_top;1	top
  KEY: THStack	stack_m4_signal;1	

  KEY: TH1F	m4_lower_DY;1	DY
  KEY: TH1F	m4_lower_Gjet;1	Gjet
  KEY: TH1F	m4_lower_QCD;1	QCD
  KEY: TH1F	m4_lower_TT;1	TT
  KEY: TH1F	m4_lower_WW;1	WW
  KEY: TH1F	m4_lower_WZ;1	WZ
  KEY: TH1F	m4_lower_Wjet;1	Wjet
  KEY: TH1F	m4_lower_ZZ;1	ZZ
  KEY: TH1F	m4_lower_top;1	top
  KEY: THStack	stack_m4_lower;1	
*/
