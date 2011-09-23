/* 
- su che ntuple sto girando?
- c'e' ancora troppo qcd
*/


#include <./test/plotUtils.C>
#include "./interface/Functions.h"

int macro_004_7 ()
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


  c1->SetLogy () ;
//  c1->DrawFrame (120., 6., 600., 2500.) ;
  hint->SetStats (kFALSE) ;
  hint->SetMarkerSize (0) ;
  hint->SetFillColor (kRed) ;
  hint->Draw ("E3") ;
  extrapolated_bkg->Draw ("same") ;
  hint->Draw ("E3same") ;
  hint->GetXaxis ()->SetTitle ("m_{lvjj} (GeV)") ;
  extrapolated_bkg->SetMarkerStyle (8) ;
  extrapolated_bkg->SetMarkerColor (kBlue) ;
  extrapolated_bkg->GetXaxis ()->SetTitle ("m_{lvjj} (GeV)") ;
  extrapolated_bkg->SetTitle ("") ;
  extrapolated_bkg->Draw ("same") ;
  c1->Print ("CB_fit_DATA_log.pdf", "pdf") ;
  c1->SetLogy (0) ;

  cout << fitFunc->Integral (100, 800) / 10. << " " << extrapolated_bkg->Integral () << endl ;

  vector<double> masses ;
  vector<double> Hwindow ;
  vector<double> mMin ;
  vector<double> mMax ;
  masses.push_back (250.) ;  mMin.push_back (225) ; mMax.push_back (275) ; //PG Hwindow.push_back (24.) ;
  masses.push_back (300.) ;  mMin.push_back (270) ; mMax.push_back (330) ; //PG Hwindow.push_back (27.) ;
  masses.push_back (350.) ;  mMin.push_back (320) ; mMax.push_back (380) ; //PG Hwindow.push_back (32.) ;
  masses.push_back (400.) ;  mMin.push_back (360) ; mMax.push_back (440) ; //PG Hwindow.push_back (40.) ;
  masses.push_back (450.) ;  mMin.push_back (405) ; mMax.push_back (495) ; //PG Hwindow.push_back (46.) ;
  masses.push_back (500.) ;  mMin.push_back (445) ; mMax.push_back (565) ; //PG Hwindow.push_back (54.) ;
  masses.push_back (550.) ;  mMin.push_back (490) ; mMax.push_back (630) ; //PG Hwindow.push_back (61.) ;
  masses.push_back (600.) ;  mMin.push_back (530) ; mMax.push_back (700) ; //PG Hwindow.push_back (68.) ;

  TGraphErrors g_total ;
  TGraphErrors g_background ;
  TGraphErrors g_background_count ;
  TGraph g_error ;
  
  for (int i = 0 ; i < masses.size () ; ++i)
    {
      int minBin = m4_signal_DATA->FindBin (mMin.at (i)) ;
      int maxBin = m4_signal_DATA->FindBin (mMax.at (i)) ;
//      int minBin = m4_signal_DATA->FindBin (masses.at (i) - Hwindow.at (i)) ;
//      int maxBin = m4_signal_DATA->FindBin (masses.at (i) + Hwindow.at (i)) ;
      double total = m4_signal_DATA->Integral (minBin, maxBin) ;

      double min = m4_signal_DATA->GetBinLowEdge (minBin) ;
      double max = m4_signal_DATA->GetBinLowEdge (maxBin + 1) ;
      double bkg = fitFunc->Integral (min, max) ;
      double error = 0. ;
      for (int j = minBin ; j <= maxBin ; ++j) error += hint->GetBinError (j) ;

      double bkg_count_error = 0. ;
      double bkg_count = extrapolated_bkg->IntegralAndError (minBin, maxBin, bkg_count_error) ;
      
      cout << masses.at (i) 
           << " : (" << min
           << "," << max
           << ") : " << total //PG the bin width 
           << " : " << bkg / 10.
           << " +- " << error
           << "\n" ;
           
      g_total.SetPoint (i, masses.at (i), total) ;
      g_total.SetPointError (i, 50., sqrt (total)) ;
      g_background.SetPoint (i, masses.at (i), bkg/10.) ;
      g_background.SetPointError (i, 0., error) ;
      g_background_count.SetPoint (i, masses.at (i), bkg_count) ;
      g_background_count.SetPointError (i, 0., bkg_count_error) ;
      g_error.SetPoint (i, masses.at (i), sqrt (error*error + total)) ;
    }

  c1->SetLogy () ;
  g_total.SetMarkerStyle (6) ;
  g_background.SetFillColor (kOrange) ;
  g_background_count.SetLineColor (kBlue) ;
  g_background_count.SetFillColor (kBlue) ;
  g_background_count.SetFillStyle (3002) ;
  g_background.Draw ("AE3") ;
  g_background_count.Draw ("E3same") ;
//  g_background_count.Draw ("Lsame") ;
  g_total.Draw ("EP") ;
  c1->Print ("results.pdf", "pdf") ;
  
  TGraph g_expectedCountings ;
  int k = 0 ;
  g_expectedCountings.SetPoint (k, masses.at (k++), 74.14) ;
  g_expectedCountings.SetPoint (k, masses.at (k++), 75.13) ;
  g_expectedCountings.SetPoint (k, masses.at (k++), 76.54) ;
  g_expectedCountings.SetPoint (k, masses.at (k++), 65.89) ;
  g_expectedCountings.SetPoint (k, masses.at (k++), 43.78) ;
  g_expectedCountings.SetPoint (k, masses.at (k++), 28.21) ;
  g_expectedCountings.SetPoint (k, masses.at (k++), 17.11) ;
  g_expectedCountings.SetPoint (k, masses.at (k++), 10.35) ;

  c1->SetLogy (0) ;
  c1->DrawFrame (200, 0, 700, 200) ;
  g_error.SetLineWidth (2) ;
  g_error.Draw ("L") ;
  g_expectedCountings.SetMarkerStyle (8) ;
  g_expectedCountings.Draw ("LP") ;
  c1->Print ("trivial_sensitivity.pdf", "pdf") ;
  
  
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