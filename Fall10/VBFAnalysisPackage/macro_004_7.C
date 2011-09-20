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
  
  TF1 * CB_AF = new TF1 ("CB_AF", attenuatedCB, 0., 1000., 7) ;
  CB_AF->SetLineWidth (1) ;
  CB_AF->SetLineColor (kBlue+2) ;
  CB_AF->SetNpx (10000) ;

  //PG perform the fit
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  CB_AF->SetParameter (0, m4_signal_total->Integral ()) ;
  CB_AF->SetParameter (1, 200.) ;
  CB_AF->SetParameter (2, 20.) ;
  CB_AF->SetParameter (3, 0.5) ;
  CB_AF->SetParameter (4, 2.) ;
  CB_AF->SetParameter (5, 200.) ;
  CB_AF->SetParameter (6, 10.) ;
  extrapolated_bkg->Fit (CB_AF, "L+", "", 100., 950.) ;
  TH1F * hint = new TH1F ("hint", "", 85, 100., 950.) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (hint, 0.68) ;


//  c1->SetLogy () ;
//  c1->DrawFrame (120., 6., 600., 2500.) ;
  hint->SetStats (kFALSE) ;
  hint->SetMarkerSize (0) ;
  hint->SetFillColor (kRed) ;
  hint->Draw ("E3") ;
  hint->GetXaxis ()->SetTitle ("m_{lvjj} (GeV)") ;
  extrapolated_bkg->SetMarkerStyle (8) ;
  extrapolated_bkg->SetMarkerColor (kBlue) ;
  extrapolated_bkg->GetXaxis ()->SetTitle ("m_{lvjj} (GeV)") ;
  extrapolated_bkg->SetTitle ("") ;
  extrapolated_bkg->Draw ("sames") ;
  c1->Print ("CB_fit_DATA.pdf", "pdf") ;

  cout << CB_AF->Integral (100, 1000) / 10. << " " << extrapolated_bkg->Integral () << endl ;

  vector<double> masses ;
  vector<double> Hwindow ;
  masses.push_back (250.) ;  Hwindow.push_back (24.) ;
  masses.push_back (300.) ;  Hwindow.push_back (27.) ;
  masses.push_back (350.) ;  Hwindow.push_back (32.) ;
  masses.push_back (400.) ;  Hwindow.push_back (40.) ;
  masses.push_back (450.) ;  Hwindow.push_back (46.) ;
  masses.push_back (500.) ;  Hwindow.push_back (54.) ;
  masses.push_back (550.) ;  Hwindow.push_back (61.) ;
  masses.push_back (600.) ;  Hwindow.push_back (68.) ;
  
  for (int i = 0 ; i < masses.size () ; ++i)
    {
      int minBin = m4_signal_DATA->FindBin (masses.at (i) - Hwindow.at (i)) ;
      int maxBin = m4_signal_DATA->FindBin (masses.at (i) + Hwindow.at (i)) ;
      double total = m4_signal_DATA->Integral (minBin, maxBin) ;

      double min = m4_signal_DATA->GetBinLowEdge (minBin) ;
      double max = m4_signal_DATA->GetBinLowEdge (maxBin + 1) ;
      double bkg = CB_AF->Integral (min, max) ;
      
      cout << masses.at (i) 
           << " : (" << min
           << "," << max
           << ") : " << total //PG the bin width 
           << " : " << bkg / 10.
           << "\n" ;
    }


  
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