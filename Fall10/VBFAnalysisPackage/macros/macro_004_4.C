/* 
- su che ntuple sto girando?
- c'e' ancora troppo qcd
*/


#include "./plotUtils.C"


int macro_004_4 ()
{
//  TFile input ("../testBkg_004.root") ;
  TFile input ("../testBkg_004_noKF_S350.root") ;

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

  TH1F * shape_signal = (TH1F *) m4_signal_total->Clone ("shape_signal") ;
  TH1F * shape_lower_sideband = (TH1F *) m4_lower_total->Clone ("shape_lower_sideband") ;
  TH1F * shape_upper_sideband = (TH1F *) m4_upper_total->Clone ("shape_upper_sideband") ;
  TH1F * shape_sideband = (TH1F *) m4_upper_total->Clone ("shape_sideband") ;
  shape_sideband->Add (m4_lower_total) ;
  TH1F * shape_total = (TH1F *) m4_upper_total->Clone ("shape_total") ;
  shape_total->Add (m4_lower_total) ;
  shape_total->Add (m4_signal_total) ;
  
  shape_signal->Scale (1./ shape_signal->Integral ()) ;
  shape_lower_sideband->Scale (1./ shape_lower_sideband->Integral ()) ;
  shape_upper_sideband->Scale (1./ shape_upper_sideband->Integral ()) ;
  shape_sideband->Scale (1./ shape_sideband->Integral ()) ;
  shape_total->Scale (1. / shape_total->Integral ()) ;

  shape_signal        ->SetLineWidth (2) ;
  shape_lower_sideband->SetLineWidth (2) ;
  shape_upper_sideband->SetLineWidth (2) ;
  shape_sideband      ->SetLineWidth (2) ;
  shape_total         ->SetLineWidth (2) ;

  shape_signal        ->SetLineColor (kGreen+2) ;
  shape_lower_sideband->SetLineColor (kBlue) ;
  shape_upper_sideband->SetLineColor (kOrange) ;
  shape_sideband      ->SetLineColor (kRed) ;
  shape_total         ->SetLineColor (kBlack) ;

  TCanvas c1 ;

  TLegend leg_shapes (0.5, 0.6, 0.9, 0.9, NULL, "brNDC") ;
  leg_shapes.SetBorderSize (0) ;
  leg_shapes.SetTextFont (42) ;
  leg_shapes.SetTextSize (0.04) ;
  leg_shapes.SetLineColor (1) ;
  leg_shapes.SetLineStyle (1) ;
  leg_shapes.SetLineWidth (1) ;
  leg_shapes.SetFillColor (0) ;
  leg_shapes.SetFillStyle (0) ;
  leg_shapes.AddEntry (shape_signal,"signal","lp") ;
  leg_shapes.AddEntry (shape_sideband,"sideband","lp") ;
  leg_shapes.AddEntry (shape_lower_sideband,"lower sideband","lp") ;
  leg_shapes.AddEntry (shape_upper_sideband,"upper sideband","lp") ;
  leg_shapes.AddEntry (shape_total,"total","lp") ;
  leg_shapes.Draw () ;

  shape_signal        ->SetTitle ("") ;
  shape_lower_sideband->SetTitle ("") ;
  shape_upper_sideband->SetTitle ("") ;
  shape_sideband      ->SetTitle ("") ;
  shape_total         ->SetTitle ("") ;

  shape_signal        ->GetXaxis ()->SetTitle ("m_{lvjj}") ;
  shape_signal        ->Draw () ;
  shape_lower_sideband->Draw ("same") ;
  shape_upper_sideband->Draw ("same") ;
  shape_sideband      ->Draw ("same") ;
  shape_total         ->Draw ("same") ;

  leg_shapes.Draw () ;
  
  c1.Print ("allShapes.pdf", "pdf") ;

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