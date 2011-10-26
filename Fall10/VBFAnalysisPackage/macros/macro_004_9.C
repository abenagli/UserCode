/* 
I use the lower sideband as sideband, the upper sideband as signal region ... 
   the lower sideband is too small and too different =>
I use two upper side-bands   
  upper = signal
  EvenHigher = sideband

*/


#include "./plotUtils.C"
#include "./interface/Functions.h"

int macro_004_9 ()
{
  TFile input ("testBkg_004.root") ;

  //PG get the histograms
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  THStack * stack_m4_upper = (THStack *) input.Get ("stack_m4_upper") ;
  TH1F * m4_upper_total = (TH1F*) stack_m4_upper->GetStack ()->Last () ;
  TH1F * m4_upper_Wjet = (TH1F *) input.Get ("m4_upper_Wjet") ;
    
  THStack * stack_m4_EvenHigher = (THStack *) input.Get ("stack_m4_EvenHigher") ;
  TH1F * m4_EvenHigher_total = (TH1F*) stack_m4_EvenHigher->GetStack ()->Last () ;
  TH1F * m4_EvenHigher_Wjet = (TH1F *) input.Get ("m4_EvenHigher_Wjet") ;
    
  THStack * stack_m4_lower = (THStack *) input.Get ("stack_m4_lower") ;
  TH1F * m4_lower_total = (TH1F *) stack_m4_lower->GetStack ()->Last () ;
  TH1F * m4_lower_Wjet = (TH1F *) input.Get ("m4_lower_Wjet") ;
    
  THStack * stack_m4_signal = (THStack *) input.Get ("stack_m4_signal") ;
  TH1F * m4_signal_total = (TH1F *) stack_m4_signal->GetStack ()->Last () ;
  TH1F * m4_signal_Wjet = (TH1F *) input.Get ("m4_signal_Wjet") ;

  THStack * stack_m4_sideband = (THStack *) input.Get ("stack_m4_sideband") ;
  TH1F * m4_sideband_total = (TH1F *) stack_m4_sideband->GetStack ()->Last () ;
  TH1F * m4_sideband_Wjet = (TH1F *) input.Get ("m4_sideband_Wjet") ;

  TH1F * m4_EvenHigher_DATA = (TH1F *) input.Get ("m4_EvenHigher_DATA") ;      
  TH1F * m4_upper_DATA = (TH1F *) input.Get ("m4_upper_DATA") ;      
  TH1F * m4_signal_DATA = (TH1F *) input.Get ("m4_signal_DATA") ;    
  TH1F * m4_lower_DATA = (TH1F *) input.Get ("m4_lower_DATA") ;      
  TH1F * m4_sideband_DATA = (TH1F *) input.Get ("m4_sideband_DATA") ;

  //PG fit separately numerator and denominator
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//  TF1 * numFitFunc = new TF1 ("numFitFunc", attenuatedExponential, 0., 1000., 7) ;
//  numFitFunc->SetParameters (200., 20., 10, 0.012) ;
  TF1 * numFitFunc = new TF1 ("numFitFunc", attenuatedCB, 0., 1000., 7) ;
  numFitFunc->SetParameter (0, m4_signal_total->Integral ()) ;  //PG N
  numFitFunc->SetParameter (1, 200.) ;                          //PG gaussian mean
  numFitFunc->SetParameter (2, 20.) ;                           //PG gaussian sigma
  numFitFunc->SetParameter (3, 0.1) ;                           //PG joint point
  numFitFunc->SetParameter (4, 10) ;                            //PG power law exponent
  numFitFunc->SetParameter (5, 200) ;                           //PG fermi E
  numFitFunc->SetParameter (6, 10) ;                            //PG kT
  numFitFunc->SetLineWidth (1) ;
  numFitFunc->SetLineColor (kBlue+2) ;
  numFitFunc->SetNpx (10000) ;
  m4_upper_total->Fit (numFitFunc, "L", "", 100., 800.) ;
  TH1F * num_fit_error = new TH1F ("num_fit_error", "", 70, 100., 800.) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (num_fit_error, 0.68) ;

  TCanvas c1 ;
  num_fit_error->SetFillColor (kGray+2) ;
  m4_upper_total->Draw () ;
  num_fit_error->Draw ("E3same") ;
  m4_upper_total->Draw ("same") ;
  c1.Print ("numerator_fitSIDECHECK.pdf", "pdf") ;

//  TF1 * denFitFunc = new TF1 ("denFitFunc", attenuatedExponential, 0., 1000., 7) ;
//  denFitFunc->SetParameters (200., 20., 10, 0.012) ;
  TF1 * denFitFunc = new TF1 ("denFitFunc", attenuatedCB, 0., 1000., 7) ;
  denFitFunc->SetParameter (0, m4_sideband_total->Integral ()) ;
  denFitFunc->SetParameter (1, 200.) ;
  denFitFunc->SetParameter (2, 20.) ;
  denFitFunc->SetParameter (3, 0.1) ;
  denFitFunc->SetParameter (4, 10) ;
  denFitFunc->SetParameter (5, 200) ;
  denFitFunc->SetParameter (6, 10) ;
  denFitFunc->SetLineWidth (1) ;
  denFitFunc->SetLineColor (kBlue+2) ;
  denFitFunc->SetNpx (10000) ;
  m4_EvenHigher_total->Fit (denFitFunc, "L", "", 100., 800.) ;
  TH1F * den_fit_error = new TH1F ("den_fit_error", "", 70, 100., 800.) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (den_fit_error, 0.68) ;

  den_fit_error->SetFillColor (kGray+2) ;
  m4_EvenHigher_total->Draw () ;
  den_fit_error->Draw ("E3same") ;
  m4_EvenHigher_total->Draw ("same") ;
  c1.Print ("denominator_fitSIDECHECK.pdf", "pdf") ;

  //PG calculate the ratio of the functions
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * func_ratio = (TH1F *) m4_upper_total->Clone ("func_ratio") ;
  func_ratio->Reset () ;
  for (int j = 1 ; j <= func_ratio->GetNbinsX () ; ++j)
    {
      double x = func_ratio->GetBinCenter (j) ;
      func_ratio->SetBinContent (j, numFitFunc->Eval (x) / denFitFunc->Eval (x)) ;
      double sqRelError = num_fit_error->GetBinError (j) * num_fit_error->GetBinError (j) / 
                           (num_fit_error->GetBinContent (j) * num_fit_error->GetBinContent (j)) +
                          den_fit_error->GetBinError (j) * den_fit_error->GetBinError (j) / 
                           (den_fit_error->GetBinContent (j) * den_fit_error->GetBinContent (j)) ;
      double error = sqrt (sqRelError) * func_ratio->GetBinContent (j) ;
      func_ratio->SetBinError (j, error) ;
    }

  //PG look at the ratio of the functions
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  stack_m4_upper->Draw ("hist") ;
  c1.Print ("numeratorSIDECHECK.pdf", "pdf") ;

  stack_m4_EvenHigher->Draw ("hist") ;
  c1.Print ("denominatorSIDECHECK.pdf", "pdf") ;

  TH1F * ratio_total = (TH1F *) m4_upper_total->Clone ("ratio") ;
  ratio_total->Divide (m4_EvenHigher_total) ;
  
  func_ratio->SetLineColor (kBlue) ;
  func_ratio->SetFillStyle (0) ;
  
  c1.DrawFrame (100., 0., 1000., 2.) ;
  ratio_total->Draw ("same") ;
  func_ratio->Draw ("histE2same") ;
  c1.Print ("ratioSIDECHECK.pdf", "pdf") ;
  
  //PG extrapolate the background from side-band
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  TH1F * extrapolated_bkg = m4_EvenHigher_DATA->Clone ("extrapolated_bkg") ; //PG analysis
  extrapolated_bkg->Multiply (func_ratio) ;

  TLegend leg_compare (0.2, 0.2, 0.6, 0.4, NULL, "brNDC") ;
  leg_compare.SetBorderSize (0) ;
  leg_compare.SetTextFont (42) ;
  leg_compare.SetTextSize (0.04) ;
  leg_compare.SetLineColor (1) ;
  leg_compare.SetLineStyle (1) ;
  leg_compare.SetLineWidth (1) ;
  leg_compare.SetFillColor (0) ;
  leg_compare.SetFillStyle (0) ;
  leg_compare.AddEntry (m4_upper_DATA, "DATA in pseudo-SR", "lp") ;
  leg_compare.AddEntry (extrapolated_bkg, "extrapolated bkg from DATA in pseudo-SR", "lp") ;

  c1.SetLogy () ;
  c1.DrawFrame (100, 0.1, 800, 5000) ;
  extrapolated_bkg->SetStats (0) ;
  extrapolated_bkg->SetTitle ("") ;
  extrapolated_bkg->SetLineColor (kRed) ;
  extrapolated_bkg->Draw ("same") ;
  m4_upper_DATA->SetStats (0) ;
  m4_upper_DATA->SetMarkerStyle (5) ;
  m4_upper_DATA->SetMarkerColor (kBlack) ;
  m4_upper_DATA->Draw ("same") ;
  leg_compare.Draw () ;
  c1.Print ("compare_signal_region_newSIDECHECK.pdf", "pdf") ;
 
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