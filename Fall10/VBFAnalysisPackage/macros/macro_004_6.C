/* 
- su che ntuple sto girando?
- c'e' ancora troppo qcd
*/


#include "./plotUtils.C"
#include "../interface/Functions.h"

int macro_004_6 ()
{
  TFile input ("testBkg_004.root") ;

  //PG get the histograms
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  THStack * stack_m4_upper = (THStack *) input.Get ("stack_m4_upper") ;
  TH1F * m4_upper_total = (TH1F*) stack_m4_upper->GetStack ()->Last () ;
  TH1F * m4_upper_Wjet = (TH1F *) input.Get ("m4_upper_Wjet") ;
    
  THStack * stack_m4_lower = (THStack *) input.Get ("stack_m4_lower") ;
  TH1F * m4_lower_total = (TH1F *) stack_m4_lower->GetStack ()->Last () ;
  TH1F * m4_lower_Wjet = (TH1F *) input.Get ("m4_lower_Wjet") ;
    
  THStack * stack_m4_signal = (THStack *) input.Get ("stack_m4_signal") ;
  TH1F * m4_signal_total = (TH1F *) stack_m4_signal->GetStack ()->Last () ;
  TH1F * m4_signal_Wjet = (TH1F *) input.Get ("m4_signal_Wjet") ;

  THStack * stack_m4_sideband = (THStack *) input.Get ("stack_m4_sideband") ;
  TH1F * m4_sideband_total = (TH1F *) stack_m4_sideband->GetStack ()->Last () ;
  TH1F * m4_sideband_Wjet = (TH1F *) input.Get ("m4_sideband_Wjet") ;

  //PG calculate the ratio for the total distribution
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * ratio_total = (TH1F *) m4_signal_total->Clone ("ratio") ;
  ratio_total->Divide (m4_sideband_total) ;

  //PG apply the scale factor to the data distribution in the sideband
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * m4_upper_DATA = (TH1F *) input.Get ("m4_upper_DATA") ;      
  TH1F * m4_signal_DATA = (TH1F *) input.Get ("m4_signal_DATA") ;    
  TH1F * m4_lower_DATA = (TH1F *) input.Get ("m4_lower_DATA") ;      
  TH1F * m4_sideband_DATA = (TH1F *) input.Get ("m4_sideband_DATA") ;

  TH1F * extrapolated_bkg = m4_sideband_DATA->Clone ("extrapolated_bkg") ;
  extrapolated_bkg->Multiply (ratio_total) ;

  //PG define the functions
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  TF1 * fitFunc = new TF1 ("fitFunc", attenuatedCB, 0., 1000., 7) ;
  fitFunc->SetLineWidth (1) ;
  fitFunc->SetLineColor (kBlue+2) ;
  fitFunc->SetNpx (10000) ;
  fitFunc->SetParameter (0, m4_signal_total->Integral ()) ;
  fitFunc->SetParameter (1, 200.) ;
  fitFunc->SetParameter (2, 20.) ;
  fitFunc->SetParameter (3, 0.5) ;
  fitFunc->SetParameter (4, 2) ;
  fitFunc->SetParameter (5, 200) ;
  fitFunc->SetParameter (6, 10) ;


//  TF1 * fitFunc = new TF1 ("fitFunc", attenuatedExponential, 100., 1000., 4) ;
//  fitFunc->SetLineWidth (1) ;
//  fitFunc->SetLineColor (kBlue+2) ;
//  fitFunc->SetNpx (10000) ;
//  fitFunc->SetParameter (0, 180.) ;
//  fitFunc->SetParameter (1, 20.) ;
////  fitFunc->SetParameter (2, 5.) ;
//  fitFunc->SetParameter (2, m4_signal_total->Integral ()) ;
//  fitFunc->SetParameter (3, 0.0012) ;

  //PG perform the fit
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  
  extrapolated_bkg->Fit (fitFunc, "L+", "", 100., 750.) ;
  TH1F * hint = new TH1F ("hint", "", 65, 100., 750.) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (hint, 0.68) ;


//  c1->SetLogy () ;
//  c1->DrawFrame (120., 6., 600., 2500.) ;
  hint->SetStats (kFALSE) ;
  hint->SetMarkerSize (0) ;
  hint->SetFillColor (kRed) ;
  extrapolated_bkg->Draw () ;
  hint->Draw ("E3same") ;
  hint->GetXaxis ()->SetTitle ("m_{lvjj} (GeV)") ;
  extrapolated_bkg->SetMarkerStyle (8) ;
  extrapolated_bkg->SetMarkerColor (kBlue) ;
  extrapolated_bkg->GetXaxis ()->SetTitle ("m_{lvjj} (GeV)") ;
  extrapolated_bkg->SetTitle ("") ;
  extrapolated_bkg->Draw ("sames") ;
  c1->Print ("CB_fit_DATA.pdf", "pdf") ;



  
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