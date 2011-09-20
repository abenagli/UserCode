/* 
- su che ntuple sto girando?
- c'e' ancora troppo qcd
*/


#include <./test/plotUtils.C>


int macro_004_2 ()
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

  //PG calculate the ratio for W+jets
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * ratio_Wjet = (TH1F *) m4_signal_Wjet->Clone ("ratio") ;
  ratio_Wjet->Divide (m4_sideband_Wjet) ;

  //PG calculate the ratio for the total distribution
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * ratio_total = (TH1F *) m4_signal_total->Clone ("ratio") ;
  ratio_total->Divide (m4_sideband_total) ;

  //PG make plots
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TCanvas c1 ;
  ratio_Wjet->SetMarkerStyle (4) ;
  ratio_Wjet->SetMarkerColor (kGreen+2) ;  
  ratio_Wjet->SetTitle ("") ;
  ratio_Wjet->Draw () ;

  ratio_total->SetMarkerStyle (20) ;
  ratio_total->SetMarkerColor (kBlue+2) ;  
  ratio_total->SetTitle ("") ;
  ratio_total->Draw ("same") ;

  TLegend leg_ratio (0.5, 0.6, 0.9, 0.9, NULL, "brNDC") ;
  leg_ratio.SetBorderSize (0) ;
  leg_ratio.SetTextFont (42) ;
  leg_ratio.SetTextSize (0.04) ;
  leg_ratio.SetLineColor (1) ;
  leg_ratio.SetLineStyle (1) ;
  leg_ratio.SetLineWidth (1) ;
  leg_ratio.SetFillColor (0) ;
  leg_ratio.SetFillStyle (0) ;
  leg_ratio.AddEntry (ratio_Wjet,"signal/side Wjet","lp") ;
  leg_ratio.AddEntry (ratio_total,"signal/side total","lp") ;
  leg_ratio.Draw () ;

  c1.Print ("ratioSigOSideband.pdf", "pdf") ;

  //PG plot the stacks of the distributions, that enter in the ratio
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TLegend * leg_signal = makeLegendTitle (*stack_m4_signal) ;
  stack_m4_signal->Draw ("hist") ;
  leg_signal->Draw () ;
  c1.Print ("signal_stack.pdf", "pdf") ;

  TLegend * leg_sideband = makeLegendTitle (*stack_m4_sideband) ;
  stack_m4_sideband->Draw ("hist") ;
  leg_sideband->Draw () ;
  c1.Print ("sideband_stack.pdf", "pdf") ;

  //PG apply the scale factor to the data distribution in the sideband
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * m4_upper_DATA = (TH1F *) input.Get ("m4_upper_DATA") ;      
  TH1F * m4_signal_DATA = (TH1F *) input.Get ("m4_signal_DATA") ;    
  TH1F * m4_lower_DATA = (TH1F *) input.Get ("m4_lower_DATA") ;      
  TH1F * m4_sideband_DATA = (TH1F *) input.Get ("m4_sideband_DATA") ;

  TH1F * extrapolated_bkg = m4_sideband_DATA->Clone ("extrapolated_bkg") ;
  extrapolated_bkg->Multiply (ratio_total) ;

  //PG plot the results
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  extrapolated_bkg->SetMarkerStyle (8) ;
  extrapolated_bkg->SetMarkerColor (kBlue) ;
  extrapolated_bkg->GetXaxis ()->SetTitle ("m_{lvjj} (GeV)") ;
  extrapolated_bkg->SetTitle ("") ;
  extrapolated_bkg->Draw () ;
  stack_m4_signal->Draw ("histsame") ;
  extrapolated_bkg->Draw ("same") ;
  leg_signal->Draw () ;
  c1.Print ("extrapolated_bkg.pdf", "pdf") ;
  
  m4_sideband_DATA->SetMarkerStyle (8) ;
  m4_sideband_DATA->GetXaxis ()->SetTitle ("m_{lvjj} (GeV)") ;
  m4_sideband_DATA->SetTitle ("") ;
  m4_sideband_DATA->Draw () ;
  stack_m4_sideband->Draw ("histsame") ;
  m4_sideband_DATA->Draw ("same") ;
  leg_sideband->Draw () ;
  c1.Print ("sideband_region.pdf", "pdf") ;

  TLegend leg_compare (0.4, 0.7, 0.9, 0.9, NULL, "brNDC") ;
  leg_compare.SetBorderSize (0) ;
  leg_compare.SetTextFont (42) ;
  leg_compare.SetTextSize (0.04) ;
  leg_compare.SetLineColor (1) ;
  leg_compare.SetLineStyle (1) ;
  leg_compare.SetLineWidth (1) ;
  leg_compare.SetFillColor (0) ;
  leg_compare.SetFillStyle (0) ;
  leg_compare.AddEntry (m4_signal_total, "data in signal region", "lp") ;
  leg_compare.AddEntry (extrapolated_bkg, "extrapolated bkg in SR", "lp") ;

  extrapolated_bkg->Draw () ;
  extrapolated_bkg->SetStats (0) ;
  m4_signal_total->SetStats (0) ;
  m4_signal_total->SetMarkerStyle (8) ;
  m4_signal_total->SetMarkerColor (kBlack) ;
  m4_signal_total->Draw ("same") ;
  leg_compare.Draw () ;
  c1.Print ("compare_signal_region.pdf", "pdf") ;
 

  
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