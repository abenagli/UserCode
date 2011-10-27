#include "./plotUtils.C"


int compareInjections ()
{
  TFile * f_0S = new TFile ("./2011-10-27-plots/noSignalClosure/output_004_10.root") ;
  TFile * f_1S = new TFile ("./2011-10-27-plots/SignalClosure/output_004_10.root") ;
//  TFile * f_5S = new TFile ("./2011-10-27-plots/5SignalClosure/output_004_10.root") ;

  //PG correction factor
  //PG --- --- --- --- --- --- --- --- --- ---

  TH1F * corr_0S = (TH1F *) f_0S->Get ("h_correctionBand") ;
  TH1F * corr_1S = (TH1F *) f_1S->Get ("h_correctionBand") ;
//  TH1F * corr_5S = (TH1F *) f_5S->Get ("h_correctionBand") ;

  //PG resulting background
  //PG --- --- --- --- --- --- --- --- --- ---
  
  TH1F * bkg_0S = (TH1F *) f_0S->Get ("extrapolated_bkg") ;
  TH1F * bkg_1S = (TH1F *) f_1S->Get ("extrapolated_bkg") ;
//  TH1F * bkg_5S = (TH1F *) f_5S->Get ("extrapolated_bkg") ;

//  bkg_0S->GetFunction ("denFitFunc")->Delete () ;
//  bkg_1S->GetFunction ("denFitFunc")->Delete () ;
//  bkg_5S->GetFunction ("denFitFunc")->Delete () ;

  TH1F * bkg_1S_ratio = (TH1F *) bkg_1S->Clone ("bkg_1S_ratio") ;
  bkg_1S_ratio->Divide (bkg_0S) ;

//  TH1F * bkg_5S_ratio = (TH1F *) bkg_5S->Clone ("bkg_5S_ratio") ;
//  bkg_5S_ratio->Divide (bkg_0S) ;

  TCanvas * c1 = new TCanvas () ;
  bkg_0S->SetMarkerStyle (24) ;
  bkg_1S->SetMarkerStyle (28) ;
//  bkg_5S->SetMarkerStyle (30) ;
  bkg_0S->SetMarkerColor (kBlack) ;
  bkg_1S->SetMarkerColor (kBlue) ;
//  bkg_5S->SetMarkerColor (kGreen + 2) ;
  bkg_0S->SetLineColor (kBlack) ;
  bkg_1S->SetLineColor (kBlue) ;
//  bkg_5S->SetLineColor (kGreen + 2) ;
  
  bkg_0S->Draw () ;
  bkg_1S->Draw ("same") ;
//  bkg_5S->Draw ("same") ;
  
  TCanvas * c2 = new TCanvas () ;
  bkg_1S_ratio->SetMarkerStyle (28) ;
//  bkg_5S_ratio->SetMarkerStyle (30) ;
  bkg_1S_ratio->SetMarkerColor (kBlue) ;
//  bkg_5S_ratio->SetMarkerColor (kGreen + 2) ;
  bkg_1S_ratio->SetLineColor (kBlue) ;
//  bkg_5S_ratio->SetLineColor (kGreen + 2) ;
  
//  bkg_5S_ratio->Draw () ;
  bkg_1S_ratio->Draw () ;
  
  //PG get the expected background and signal from the original root file
  //PG and compare to the background shape I evaluate in the various cases
  //PG to understand what is the effect of the signal injection
  //PG --- --- --- --- --- --- --- --- --- ---
  
  TFile * input = new TFile ("../testBkg_004_S350.root") ;

  THStack * stack_m4_signal = (THStack *) input->Get ("stack_m4_signal") ;
  TH1F * m4_signal_total = (TH1F *) stack_m4_signal->GetStack ()->Last () ;
  m4_signal_total->SetTitle ("") ;

  THStack * stack_m4_signal_SIG = (THStack *) input->Get ("stack_m4_signal_SIG") ;
  TH1F * m4_signal_total_SIG = (TH1F *) stack_m4_signal_SIG->GetStack ()->Last () ;
  m4_signal_total_SIG->SetTitle ("") ;

  TH1F * bkg_0S_ratioExp = (TH1F *) bkg_0S->Clone ("bkg_0S_ratioExp") ;
  bkg_0S_ratioExp->Divide (m4_signal_total) ;

  TH1F * bkg_1S_ratioExp = (TH1F *) bkg_1S->Clone ("bkg_1S_ratioExp") ;
  bkg_1S_ratioExp->Divide (m4_signal_total) ;

//  TH1F * bkg_5S_ratioExp = (TH1F *) bkg_5S->Clone ("bkg_5S_ratioExp") ;
//  bkg_5S_ratioExp->Divide (m4_signal_total) ;

  bkg_0S_ratioExp->SetMarkerStyle (24) ;
  bkg_1S_ratioExp->SetMarkerStyle (28) ;
//  bkg_5S_ratioExp->SetMarkerStyle (30) ;
  bkg_0S_ratioExp->SetMarkerColor (kBlack) ;
  bkg_1S_ratioExp->SetMarkerColor (kBlue) ;
//  bkg_5S_ratioExp->SetMarkerColor (kGreen + 2) ;
  bkg_0S_ratioExp->SetLineColor (kBlack) ;
  bkg_1S_ratioExp->SetLineColor (kBlue) ;
//  bkg_5S_ratioExp->SetLineColor (kGreen + 2) ;

  TCanvas * c3 = new TCanvas () ;
//  bkg_5S_ratioExp->Draw () ;
  bkg_1S_ratioExp->Draw () ;
  bkg_0S_ratioExp->Draw ("same") ;

  //PG compare the bias in the background evaluation with the number of expected higgs events
  //PG --- --- --- --- --- --- --- --- --- ---

  TH1F * bkg_1S_diff = (TH1F *) bkg_1S->Clone ("bkg_1S_diff") ;
  bkg_1S_diff->Add (bkg_0S, -1.) ;

//  TH1F * bkg_5S_diff = (TH1F *) bkg_5S->Clone ("bkg_5S_diff") ;
//  bkg_5S_diff->Add (bkg_0S, -1.) ;

  TH1F * zero = (TH1F *) bkg_0S->Clone ("zero") ;
  for (int iBin = 1 ; iBin <= zero->GetNbinsX () ; ++iBin)
    {
      zero->SetBinContent (iBin, 0) ;
      zero->SetBinError (iBin, 0) ;
      if (bkg_0S->GetBinContent (iBin))
        {         
          double err = bkg_0S->GetBinError (iBin) ;
          zero->SetBinError (iBin, err) ;
        }  
    }

  zero->SetFillColor (kOrange) ;
  
  //PG analysis with outer sidebands only
  //PG --- --- --- --- --- --- --- --- --- --- --- --- ---  
  
  TFile * f_0S_outer = new TFile ("./2011-10-27-plots/noSignalClosure_outer/output_004_10.root") ;
  TFile * f_1S_outer = new TFile ("./2011-10-27-plots/SignalClosure_outer/output_004_10.root") ;

  TH1F * bkg_0S_outer = (TH1F *) f_0S_outer->Get ("extrapolated_bkg") ;
  TH1F * bkg_1S_outer = (TH1F *) f_1S_outer->Get ("extrapolated_bkg") ;

//  bkg_0S_outer->GetFunction ("denFitFunc")->Delete () ;
//  bkg_1S_outer->GetFunction ("denFitFunc")->Delete () ;

  TH1F * bkg_1S_outer_diff = (TH1F *) bkg_1S_outer->Clone ("bkg_1S_outer_diff") ;
  bkg_1S_outer_diff->Add (bkg_0S_outer, -1.) ;

  bkg_1S_outer_diff->SetMarkerStyle (34) ;
  bkg_1S_outer_diff->SetMarkerColor (kRed) ;
  bkg_1S_outer_diff->SetLineColor (kRed) ;

  //PG make the plot
  //PG --- --- --- --- --- --- --- --- --- --- --- --- ---

  TCanvas * c4 = new TCanvas () ;
  TH1F * frame = c4->DrawFrame (200, -20, 800, 80) ;
  frame->GetXaxis ()->SetTitle ("m_{lvjj} (GeV)") ;
  frame->GetYaxis ()->SetTitle ("events per bin (10 GeV)") ;
  m4_signal_total_SIG->SetFillStyle (0) ;
  m4_signal_total_SIG->Draw ("histsame") ;
  zero->Draw ("E3same") ;
//  bkg_5S_diff->Draw ("same") ;
  bkg_1S_diff->Draw ("Phistsame") ;
  bkg_1S_outer_diff->Draw ("Phistsame") ;
  m4_signal_total_SIG->Draw ("histsame") ;

  TLegend * leg_comparison = new TLegend (0.25, 0.7, 0.90, 0.90, NULL, "brNDC") ;
  legendMaquillage (leg_comparison) ;
  leg_comparison->AddEntry (m4_signal_total_SIG, "expected 350 GeV Higgs", "l") ;
  leg_comparison->AddEntry (zero, "bkg estimate uncertainty", "f") ;
  leg_comparison->AddEntry (bkg_1S_diff, "bkg bias due to signal injection", "pl") ;
  leg_comparison->AddEntry (bkg_1S_outer_diff, "... with outer sidebands only", "pl") ;
  leg_comparison->Draw () ;
  
  c4->Print ("injectionTest.pdf", "pdf") ;
  
  
  
}