int macro_007 ()
{
  TFile * input = new TFile ("testBkg_007.root") ;
  THStack * stack_m4 = (THStack *) input->Get ("stack_m4") ;
  THStack * stack_m4_sideband = (THStack *) input->Get ("stack_m4_sideband") ;
  THStack * stack_m4_signal = (THStack *) input->Get ("stack_m4_signal") ;

  TH1F * tot_m4 = (TH1F *) stack_m4->GetStack ()->Last () ;
  TH1F * tot_m4_sideband = (TH1F *) stack_m4_sideband->GetStack ()->Last () ;
  TH1F * tot_m4_signal = (TH1F *) stack_m4_signal->GetStack ()->Last () ;
 
  TCanvas * c1 = new TCanvas ;
  tot_m4->SetLineColor (kBlack) ; 
  tot_m4->SetLineWidth (2) ; 
  tot_m4_sideband->SetLineColor (kRed) ; 
  tot_m4_sideband->SetLineWidth (2) ; 
  tot_m4_signal->SetLineColor (kGreen+2) ; 
  tot_m4_signal->SetLineWidth (2) ; 
  
  tot_m4->GetXaxis ()->SetTitle ("m_{lvjj}") ;
  tot_m4->SetTitle ("") ;
  tot_m4_signal->GetXaxis ()->SetTitle ("m_{lvjj}") ;
  tot_m4_signal->SetTitle ("") ;
  tot_m4_signal->DrawNormalized () ;
  tot_m4_sideband->DrawNormalized ("same") ;
  tot_m4->DrawNormalized ("same") ;

  TLegend * leg_shapes = new TLegend (0.5, 0.6, 0.9, 0.9, NULL, "brNDC") ;
  leg_shapes->SetBorderSize (0) ;
  leg_shapes->SetTextFont (42) ;
  leg_shapes->SetTextSize (0.04) ;
  leg_shapes->SetLineColor (1) ;
  leg_shapes->SetLineStyle (1) ;
  leg_shapes->SetLineWidth (1) ;
  leg_shapes->SetFillColor (0) ;
  leg_shapes->SetFillStyle (0) ;
  leg_shapes->AddEntry (tot_m4,"total","lp") ;
  leg_shapes->AddEntry (tot_m4_signal,"signal region","lp") ;
  leg_shapes->AddEntry (tot_m4_sideband,"sideband region","lp") ;
  leg_shapes->Draw () ;
	
}