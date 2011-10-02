/* 
- su che ntuple sto girando?
- c'e' ancora troppo qcd
*/


#include <./test/plotUtils.C>
#include "./interface/Functions.h"

int macro_004_10 ()
{
  TFile input ("testBkg_004.root") ;

  //PG get the histograms
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  THStack * stack_m4_upper = (THStack *) input.Get ("stack_m4_upper") ;
  TH1F * m4_upper_total = (TH1F*) stack_m4_upper->GetStack ()->Last () ;
    
  THStack * stack_m4_lower = (THStack *) input.Get ("stack_m4_lower") ;
  TH1F * m4_lower_total = (TH1F *) stack_m4_lower->GetStack ()->Last () ;
    
  THStack * stack_m4_signal = (THStack *) input.Get ("stack_m4_signal") ;
  TH1F * m4_signal_total = (TH1F *) stack_m4_signal->GetStack ()->Last () ;

  THStack * stack_m4_sideband = (THStack *) input.Get ("stack_m4_sideband") ;
  TH1F * m4_sideband_total = (TH1F *) stack_m4_sideband->GetStack ()->Last () ;

  TH1F * m4_upper_DATA = (TH1F *) input.Get ("m4_upper_DATA") ;      
  TH1F * m4_signal_DATA = (TH1F *) input.Get ("m4_signal_DATA") ;    
  TH1F * m4_lower_DATA = (TH1F *) input.Get ("m4_lower_DATA") ;      
  TH1F * m4_sideband_DATA = (TH1F *) input.Get ("m4_sideband_DATA") ;

  //PG fit separately numerator and denominator
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  stack_m4_signal->Draw ("hist") ;
  c1.Print ("numerator.pdf", "pdf") ;

  stack_m4_sideband->Draw ("hist") ;
  c1.Print ("denominator.pdf", "pdf") ;

  TF1 * numFitFunc = new TF1 ("numFitFunc", attenuatedCB, 0., 1000., 7) ;
  numFitFunc->SetLineWidth (1) ;
  numFitFunc->SetLineColor (kBlue+2) ;
  numFitFunc->SetNpx (10000) ;
  numFitFunc->SetParameter (0, m4_signal_total->Integral ()) ;  //PG N
  numFitFunc->SetParameter (1, 200.) ;                          //PG gaussian mean
  numFitFunc->SetParameter (2, 20.) ;                           //PG gaussian sigma
  numFitFunc->SetParameter (3, 0.1) ;                           //PG joint point
  numFitFunc->SetParameter (4, 10) ;                            //PG power law exponent
  numFitFunc->SetParameter (5, 200) ;                           //PG fermi E
  numFitFunc->SetParameter (6, 10) ;                            //PG kT
  m4_signal_total->Fit (numFitFunc, "L", "", 100., 800.) ;
  TH1F * num_fit_error = new TH1F ("num_fit_error", "", 70, 100., 800.) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (num_fit_error, 0.68) ;

  TCanvas c1 ;
  num_fit_error->SetFillColor (kGray+2) ;
  m4_signal_total->Draw () ;
  num_fit_error->Draw ("E3same") ;
  m4_signal_total->Draw ("sames") ;
  c1.Print ("numerator_fit.pdf", "pdf") ;

  TF1 * denFitFunc = new TF1 ("denFitFunc", attenuatedCB, 0., 1000., 7) ;
  denFitFunc->SetLineWidth (1) ;
  denFitFunc->SetLineColor (kBlue+2) ;
  denFitFunc->SetNpx (10000) ;
  denFitFunc->SetParameter (0, m4_sideband_total->Integral ()) ;
  denFitFunc->SetParameter (1, 200.) ;
  denFitFunc->SetParameter (2, 5.) ;
  denFitFunc->SetParameter (3, 0.1) ;
  denFitFunc->SetParameter (4, 5) ;
  denFitFunc->SetParameter (5, 200) ;
  denFitFunc->SetParameter (6, 10) ;
  m4_sideband_total->Fit (denFitFunc, "L", "", 100., 800.) ;
  TH1F * den_fit_error = new TH1F ("den_fit_error", "", 70, 100., 800.) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (den_fit_error, 0.68) ;

  den_fit_error->SetFillColor (kGray+2) ;
  m4_sideband_total->Draw () ;
  den_fit_error->Draw ("E3same") ;
  m4_sideband_total->Draw ("sames") ;
  c1.Print ("denominator_fit.pdf", "pdf") ;

  TRandom3 r ;
  int nToys = 10000 ;
  TH2F * correctionPlane = new TH2F ("correctionPlane", "", 70, 100, 800, 200, 0, 3) ;
  for (iToy = 0 ; iToy < nToys ; ++iToy)
    {
      if (iToy %(nToys/10) == 0) cout << "toy number " << iToy << endl ;

      int nNum = r.Poisson (m4_signal_total->Integral ()) ;
      TString name = "dummyNum_" ; name += iToy ;
      TH1F * dummyNum = (TH1F *) m4_signal_total->Clone (name) ;
      dummyNum->Reset () ;
      dummyNum->FillRandom ("numFitFunc", nNum) ;

      int nDen = r.Poisson (m4_sideband_total->Integral ()) ;
      name = "dummyDen_" ; name += iToy ;
      TH1F * dummyDen = (TH1F *) m4_sideband_total->Clone (name) ;
      dummyDen->Reset () ;
      dummyDen->FillRandom ("denFitFunc", nDen) ;

      name = "ratio_" ; name += iToy ;
      TH1F * ratio = (TH1F *) dummyNum->Clone (name) ;
      ratio->Divide (dummyDen) ;
      for (int iBin = 1 ; iBin <= ratio->GetNbinsX () ; ++iBin)
        {
          correctionPlane->Fill (ratio->GetBinCenter (iBin), ratio->GetBinContent (iBin)) ;
        }
    }

  TH1F * ratio_total = (TH1F *) m4_signal_total->Clone ("ratio") ;
  ratio_total->Divide (m4_sideband_total) ;
  ratio_total->SetMarkerColor (kOrange) ;
  
  TProfile * correctionBand = correctionPlane->ProfileX ("correctionBand", 1, -1, "s") ;
  correctionBand->SetStats (0) ;
  correctionBand->SetFillColor (kOrange) ;
 
  TObjArray aSlices;
  correctionPlane->FitSlicesY (0, 0, -1, 0, "QNRL", &aSlices) ;
  TH1F * gaussianBand = aSlices.At (1)->Clone ("gaussianBand") ;
  for (int iBin = 1 ; iBin <= gaussianBand->GetNbinsX () ; ++iBin) 
    {
      double sigma = ((TH1F *) aSlices.At (2))->GetBinContent (iBin) ;
      gaussianBand->SetBinError (iBin, sigma) ;
    }
  gaussianBand->SetFillStyle (3002) ;
  gaussianBand->SetFillColor (kGreen + 2) ;
  
  correctionPlane->SetStats (0) ;
//  correctionPlane->Draw ("COLZ") ;
  correctionBand->Draw ("E3") ;
  gaussianBand->Draw ("E3same") ;
  gStyle->SetPalette (1) ;
  ratio_total->Draw ("same") ;
  c1.Print ("correctionPlane.pdf", "pdf") ;

  //PG vedere come sono distribuiti i singoli punti rispetto alla larghezza della banda

  TH1F pooPlot ("poolPlot", "", 50, -5, 5) ;
  TH1F pooPlotGaus ("poolPlotGaus", "", 50, -5, 5) ;
  for (int iBin = 1 ; iBin <= gaussianBand->GetNbinsX () ; ++iBin) 
    {
      double num = ratio_total->GetBinContent (iBin) ;
      double mean = ((TH1F *) aSlices.At (1))->GetBinContent (iBin) ;
      double sigma = ((TH1F *) aSlices.At (2))->GetBinContent (iBin) ;
      poolPlotGaus.Fill ((num - mean) / sigma) ;
      mean = correctionBand->GetBinContent (iBin) ;
      sigma = correctionBand->GetBinError (iBin) ;
      poolPlot.Fill ((num - mean) / sigma) ;
    }
  poolPlotGaus.Fit ("gaus","L") ;
  c1.Print ("poolPlotGaus.pdf", "pdf") ;
  poolPlot.Fit ("gaus","L") ;
  c1.Print ("poolPlot.pdf", "pdf") ;
 
//PG FIXME propagare la banda!!! ... dovrebbe essere propagata, controllo i numeri

  //PG calculate the extrapolated background
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
//  TH1F * extrapolated_bkg = m4_sideband_total->Clone ("extrapolated_bkg") ; //PG simil-closure test --> stats issue
  TH1F * extrapolated_bkg = m4_sideband_DATA->Clone ("extrapolated_bkg") ; //PG analysis
  extrapolated_bkg->Multiply (correctionBand) ;

  //PG first plot of the result
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TLegend leg_compare (0.2, 0.2, 0.6, 0.4, NULL, "brNDC") ;
  leg_compare.SetBorderSize (0) ;
  leg_compare.SetTextFont (42) ;
  leg_compare.SetTextSize (0.04) ;
  leg_compare.SetLineColor (1) ;
  leg_compare.SetLineStyle (1) ;
  leg_compare.SetLineWidth (1) ;
  leg_compare.SetFillColor (0) ;
  leg_compare.SetFillStyle (0) ;
  leg_compare.AddEntry (m4_signal_total, "simulation in signal region", "lfp") ;
  leg_compare.AddEntry (extrapolated_bkg, "extrapolated bkg in SR", "lp") ;

  c1.SetLogy () ;
  c1.DrawFrame (100, 0.1, 800, 5000) ;
  extrapolated_bkg->SetStats (0) ;
  extrapolated_bkg->SetTitle ("") ;
  extrapolated_bkg->SetLineColor (kRed) ;
  extrapolated_bkg->SetFillColor (kOrange) ;
  extrapolated_bkg->Draw ("E3same") ;

  m4_signal_total->SetStats (0) ;
  m4_signal_total->SetMarkerStyle (5) ;
  m4_signal_total->SetMarkerColor (kBlack) ;
  m4_signal_total->Draw ("same") ;
  leg_compare.Draw () ;
  c1.Print ("compare_signal_region_new.pdf", "pdf") ;

  //PG fit the extrapolated bkg
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TF1 * fitFuncBkg = new TF1 ("fitFuncBkg", attenuatedCB, 0., 1000., 7) ;
  fitFuncBkg->SetLineWidth (1) ;
  fitFuncBkg->SetLineColor (kBlue+2) ;
  fitFuncBkg->SetNpx (10000) ;
  fitFuncBkg->SetParameter (0, m4_signal_total->Integral ()) ;
  fitFuncBkg->SetParameter (1, 200.) ;
  fitFuncBkg->SetParameter (2, 20.) ;
  fitFuncBkg->SetParameter (3, 0.1) ;
  fitFuncBkg->SetParameter (4, 10) ;
  fitFuncBkg->SetParameter (5, 200) ;
  fitFuncBkg->SetParameter (6, 10) ;
  
  extrapolated_bkg->Fit (fitFuncBkg, "L+", "", 100., 800.) ;
  TH1F * extrapolated_bkg_fitBand = new TH1F ("extrapolated_bkg_fitBand", "", 70, 100., 800.) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (extrapolated_bkg_fitBand, 0.68) ;

  //PG  plot of the result
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  extrapolated_bkg_fitBand->SetFillColor (kGray+2) ;
//  extrapolated_bkg_fitBand->SetFillStyle (3004) ;
  
//  c1.DrawFrame (100, 0.1, 800, 5000) ;
//  extrapolated_bkg->Draw ("E3same") ;
//  extrapolated_bkg_fitBand->Draw ("E3same") ;
//  extrapolated_bkg_fitBand->Draw () ;
  extrapolated_bkg->Draw ("") ;
  extrapolated_bkg_fitBand->Draw ("E3same") ;
  c1.Print ("extrapolatedBkg.pdf", "pdf") ;

  TFile output ("output_004_10.root", "recreate") ;
  output->cd () ;
  ratio_total->Write () ;
  gaussianBand->Write () ;
  pooPlot.Write () ;
  pooPlotGaus.Write () ;
  extrapolated_bkg->Write () ;
  extrapolated_bkg_fitBand->Write () ;
  correctionPlane->Write () ;
  correctionBand->Write () ;
  output.Close () ;

 
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