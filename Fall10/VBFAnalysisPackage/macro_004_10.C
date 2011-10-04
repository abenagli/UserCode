#include <./test/plotUtils.C>
#include "./interface/Functions.h"

int macro_004_10 ()
{
  TFile input ("testBkg_004_S350.root") ;

  //PG get the histograms
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  THStack * stack_m4_EvenHigher = (THStack *) input.Get ("stack_m4_EvenHigher") ;
  TH1F * m4_EvenHigher_total = (TH1F*) stack_m4_EvenHigher->GetStack ()->Last () ;    

  THStack * stack_m4_upper = (THStack *) input.Get ("stack_m4_upper") ;
  TH1F * m4_upper_total = (TH1F*) stack_m4_upper->GetStack ()->Last () ;
    
  THStack * stack_m4_lower = (THStack *) input.Get ("stack_m4_lower") ;
  TH1F * m4_lower_total = (TH1F *) stack_m4_lower->GetStack ()->Last () ;
    
  THStack * stack_m4_signal = (THStack *) input.Get ("stack_m4_signal") ;
  TH1F * m4_signal_total = (TH1F *) stack_m4_signal->GetStack ()->Last () ;

  THStack * stack_m4_sideband = (THStack *) input.Get ("stack_m4_sideband") ;
  TH1F * m4_sideband_total = (TH1F *) stack_m4_sideband->GetStack ()->Last () ;

  TH1F * m4_EvenHigher_DATA = (TH1F *) input.Get ("m4_EvenHigher_DATA") ;      
  TH1F * m4_upper_DATA = (TH1F *) input.Get ("m4_upper_DATA") ;      
  TH1F * m4_signal_DATA = (TH1F *) input.Get ("m4_signal_DATA") ;    
  TH1F * m4_lower_DATA = (TH1F *) input.Get ("m4_lower_DATA") ;      
  TH1F * m4_sideband_DATA = (TH1F *) input.Get ("m4_sideband_DATA") ;

  TH1F * m4_EvenHigher_SIG = (TH1F *) input.Get ("m4_EvenHigher_SIG") ;      
  TH1F * m4_upper_SIG = (TH1F *) input.Get ("m4_upper_SIG") ;      
  TH1F * m4_signal_SIG = (TH1F *) input.Get ("m4_signal_SIG") ;    
  TH1F * m4_lower_SIG = (TH1F *) input.Get ("m4_lower_SIG") ;      
  TH1F * m4_sideband_SIG = (TH1F *) input.Get ("m4_sideband_SIG") ;

  //PG which histograms I use
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

/*
  //PG A'B'C'D' test with upper and EvenHigher
  TH1F * sidebaRegionMC = m4_EvenHigher_total ; //PG analysis
  TH1F * signalRegionMC = m4_upper_total ; //PG analysis 
  TH1F * sidebaRegion   = m4_EvenHigher_DATA ;
  TH1F * signalRegion   = m4_upper_DATA ;  
*/

/*
  //PG A'B'C'D' test with upper and lower
  TH1F * sidebaRegionMC = m4_lower_total ; 
  TH1F * signalRegionMC = m4_upper_total ;  
  TH1F * sidebaRegion   = m4_lower_DATA ;
  TH1F * signalRegion   = m4_upper_DATA ;  
*/

/*
  //PG upper sideband analysis
  TH1F * sidebaRegionMC = m4_upper_total ; 
  TH1F * signalRegionMC = m4_signal_total ;
  TH1F * sidebaRegion   = m4_upper_DATA ;
  TH1F * signalRegion   = m4_signal_DATA ;  
*/

/*
  //PG upper sideband analysis closure test
  TH1F * sidebaRegionMC = m4_upper_total ; 
  TH1F * signalRegionMC = m4_signal_total ;  
  TH1F * sidebaRegion   = sidebaRegionMC ; 
  TH1F * signalRegion   = signalRegionMC ; 
*/

/*
  //PG final analysis
  TH1F * sidebaRegionMC = m4_sideband_total ; 
  TH1F * signalRegionMC = m4_signal_total ;
  TH1F * sidebaRegion   = m4_sideband_DATA ;
  TH1F * signalRegion   = m4_signal_DATA ;  
*/

  //PG final analysis closure test
  TH1F * sidebaRegionMC = m4_sideband_total ; 
  TH1F * signalRegionMC = m4_signal_total ;  
  TH1F * sidebaRegion   = sidebaRegionMC ; 
  TH1F * signalRegion   = signalRegionMC ; 

/*
  //PG final analysis closure test with additional signal
  TH1F * sidebaRegionMC = m4_sideband_total ; 
  TH1F * signalRegionMC = m4_signal_total ;  
  TH1F * sidebaRegion   = sidebaRegionMC ;
  sidebaRegion->Add (m4_sideband_SIG) ; 
  TH1F * signalRegion   = signalRegionMC ; 
  signalRegion->Add (m4_signal_SIG) ; 
*/
  //PG fit separately numerator and denominator of MC 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TCanvas * c1 = new TCanvas () ;

  stack_m4_signal->Draw ("hist") ;
  c1->Print ("numerator.pdf", "pdf") ;

  stack_m4_sideband->Draw ("hist") ;
  c1->Print ("denominator.pdf", "pdf") ;


//------------------------------
 // attenuated double exponential
 //
 //(*func) -> SetParameter(4,180.);
 //(*func) -> SetParameter(5,11.);
 //(*func) -> SetParameter(6,10.);
 //(*func) -> SetParameter(7,0.012);
 //(*func) -> SetParameter(8,5.);
 //(*func) -> SetParameter(9,0.005);
 //
 //(*func) -> SetParLimits(4,150.,250.);
 //(*func) -> SetParLimits(5,0.,100.);
 //
 //(*func) -> SetParName(4,"#mu");
 //(*func) -> SetParName(5,"kT");
 //(*func) -> SetParName(6,"N1");
 //(*func) -> SetParName(7,"#lambda1");
 //(*func) -> SetParName(8,"N2");
 //(*func) -> SetParName(9,"#lambda2");


  TF1 * numFitFunc = new TF1 ("numFitFunc", attenuatedCB, 0., 1000., 7) ;
  //PG                        N                             , gaus m, gaus s, joint, exp, fermi E, kT
//  numFitFunc->SetParameters (signalRegionMC->Integral () / 5, 200.  , 20.   , 0.1  , 10 , 200    , 10) ;
  numFitFunc->SetParameter (0, signalRegionMC->Integral () / 5.) ;
  numFitFunc->SetParameter (1, 200.) ;
  numFitFunc->SetParameter (2, 20.) ;
  numFitFunc->SetParameter (3, 0.1) ;
  numFitFunc->SetParameter (4, 10.) ;
  numFitFunc->SetParameter (5, 200) ;
  numFitFunc->SetParameter (6, 10) ;

  numFitFunc->SetLineWidth (1) ;
  numFitFunc->SetLineColor (kBlue+2) ;
  numFitFunc->SetNpx (10000) ;
  signalRegionMC->Fit (numFitFunc, "L", "", 160., 800.) ;
  TH1F * num_fit_error = new TH1F ("num_fit_error", "", 64, 160., 800.) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (num_fit_error, 0.68) ;

  num_fit_error->SetFillColor (kGray+2) ;
  signalRegionMC->Draw () ;
  num_fit_error->Draw ("E3same") ;
  signalRegionMC->Draw ("sames") ;
  c1->Print ("numerator_fit.pdf", "pdf") ;

  TF1 * denFitFunc = new TF1 ("denFitFunc", attenuatedCB, 0., 1000., 7) ;
  //PG                        N                              , gaus m, gaus s, joint, exp, fermi E, kT
  //PG denFitFunc->SetParameters (sidebaRegionMC->Integral () / 5., 180.  , 23.   , 0.3  , 20., 180    , 15) ;
  denFitFunc->SetParameter (0, sidebaRegionMC->Integral () / 5.) ;
  denFitFunc->SetParameter (1, 180.) ;
  denFitFunc->SetParameter (2, 23.) ;
  denFitFunc->SetParameter (3, 0.3) ;
  denFitFunc->SetParameter (4, 20.) ;
  denFitFunc->SetParameter (5, 180) ;
  denFitFunc->SetParameter (6, 15) ;
//  denFitFunc->SetParLimits (0, sidebaRegionMC->Integral ()/10, sidebaRegionMC->Integral () * 10) ;
//  cout << sidebaRegionMC->Integral () << endl ; 

  denFitFunc->SetLineWidth (1) ;
  denFitFunc->SetLineColor (kBlue+2) ;
  denFitFunc->SetNpx (10000) ;
  sidebaRegionMC->Fit (denFitFunc, "L", "", 160., 800.) ;
  TH1F * den_fit_error = new TH1F ("den_fit_error", "", 64, 160., 800.) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (den_fit_error, 0.68) ;

  den_fit_error->SetFillColor (kGray+2) ;
  sidebaRegionMC->Draw () ;
  den_fit_error->Draw ("E3same") ;
  sidebaRegionMC->Draw ("sames") ;
  c1->Print ("denominator_fit.pdf", "pdf") ;

  //PG toy experiments to determine the size of the error band on the extrapolation factor
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TRandom3 r ;
  int nToys = 10000 ;
  TH2F * correctionPlane = new TH2F ("correctionPlane", "", 70, 100, 800, 200, 0, 3) ;
  for (iToy = 0 ; iToy < nToys ; ++iToy)
    {
      if (iToy %(nToys/10) == 0) cout << "toy number " << iToy << endl ;

      int nNum = r.Poisson (signalRegionMC->Integral ()) ;
      TString name = "dummyNum_" ; name += iToy ;
      TH1F * dummyNum = (TH1F *) signalRegionMC->Clone (name) ;
      dummyNum->Reset () ;
      dummyNum->FillRandom ("numFitFunc", nNum) ;

      int nDen = r.Poisson (sidebaRegionMC->Integral ()) ;
      name = "dummyDen_" ; name += iToy ;
      TH1F * dummyDen = (TH1F *) sidebaRegionMC->Clone (name) ;
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

  //PG correction factor from the ratio of histograms 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * ratio_total = (TH1F *) signalRegionMC->Clone ("ratio") ;
  ratio_total->Divide (sidebaRegionMC) ;
  ratio_total->SetMarkerColor (kOrange) ;
  
  //PG correction factor from the profile of the many toys
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TProfile * correctionBand = correctionPlane->ProfileX ("correctionBand", 1, -1, "s") ;
  correctionBand->SetStats (0) ;
  correctionBand->SetFillColor (kOrange) ;
 
  //PG correction factor from the gaussian fit to slices of the many toys
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

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
  c1->Print ("correctionPlane.pdf", "pdf") ;

  //PG look at the point distro wrt the band width, to determine whether the extrap. factor is ok
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F poolPlot ("poolPlot", "", 50, -5, 5) ;
  TH1F poolPlotGaus ("poolPlotGaus", "", 50, -5, 5) ;
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
  c1->Print ("poolPlotGaus.pdf", "pdf") ;
  poolPlot.Fit ("gaus","L") ;
  c1->Print ("poolPlot.pdf", "pdf") ;
 
  //PG calculate the extrapolated background
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
//  TH1F * extrapolated_bkg = sidebaRegionMC->Clone ("extrapolated_bkg") ; //PG closure test
//  TH1F * extrapolated_bkg = m4_sideband_DATA->Clone ("extrapolated_bkg") ; //PG analysis

  TH1F * extrapolated_bkg = sidebaRegion->Clone ("extrapolated_bkg") ;

  extrapolated_bkg->Multiply (correctionBand) ; //PG profile correction
//  extrapolated_bkg->Multiply (gaussianBand) ; //PG gaus fit correction

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
  leg_compare.AddEntry (signalRegionMC, "simulation in signal region", "lfp") ;
  leg_compare.AddEntry (extrapolated_bkg, "extrapolated bkg in SR", "lp") ;

  c1->SetLogy () ;
  c1->DrawFrame (100, 0.1, 800, 5000) ;
  extrapolated_bkg->SetStats (0) ;
  extrapolated_bkg->SetTitle ("") ;
  extrapolated_bkg->SetLineColor (kRed) ;
  extrapolated_bkg->SetFillColor (kOrange) ;
  extrapolated_bkg->Draw ("E3same") ;

  signalRegionMC->SetStats (0) ;
  signalRegionMC->SetMarkerStyle (5) ;
  signalRegionMC->SetMarkerColor (kBlack) ;
  signalRegionMC->Draw ("same") ;
  leg_compare.Draw () ;
  c1->Print ("compare_signal_region_new.pdf", "pdf") ;

  //PG fit the extrapolated bkg
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//  TF1 * fitFuncBkg = new TF1 ("fitFuncBkg", attenuatedCB, 0., 1000., 7) ;
//  fitFuncBkg->SetLineWidth (1) ;
//  fitFuncBkg->SetLineColor (kBlue+2) ;
//  fitFuncBkg->SetNpx (10000) ;
//  fitFuncBkg->SetParameter (0, signalRegionMC->Integral ()) ;
//  fitFuncBkg->SetParameter (1, 200.) ;
//  fitFuncBkg->SetParameter (2, 20.) ;
//  fitFuncBkg->SetParameter (3, 0.1) ;
//  fitFuncBkg->SetParameter (4, 10) ;
//  fitFuncBkg->SetParameter (5, 200) ;
//  fitFuncBkg->SetParameter (6, 10) ;
//  
//  extrapolated_bkg->Fit (fitFuncBkg, "L+", "", 100., 800.) ;
//  TH1F * extrapolated_bkg_fitBand = new TH1F ("extrapolated_bkg_fitBand", "", 70, 100., 800.) ;
//  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (extrapolated_bkg_fitBand, 0.68) ;

  //PG  plot of the result
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//  extrapolated_bkg_fitBand->SetFillColor (kGray+2) ;
//  extrapolated_bkg_fitBand->SetFillStyle (3004) ;
  
//  c1->DrawFrame (100, 0.1, 800, 5000) ;
//  extrapolated_bkg->Draw ("E3same") ;
//  extrapolated_bkg_fitBand->Draw ("E3same") ;
//  extrapolated_bkg_fitBand->Draw () ;
  extrapolated_bkg->Draw ("E3P") ;
//  extrapolated_bkg_fitBand->Draw ("E3same") ;
  c1->Print ("extrapolatedBkg.pdf", "pdf") ;

  //PG prepare the windows for the fast cut-n-count thing
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<double> masses ;
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

  //PG the fast cut-n-count thing
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  TGraphErrors g_total ;
  TGraphErrors g_background_count ;
  TGraph g_error ;

  //PG loop on the mass points
  for (int i = 0 ; i < masses.size () ; ++i)
    {
      int minBin = signalRegion->FindBin (mMin.at (i)) ;       //PG analysis
      int maxBin = signalRegion->FindBin (mMax.at (i)) ;       //PG analysis
      double total = signalRegion->Integral (minBin, maxBin) ; //PG analysis
      double min = signalRegion->GetBinLowEdge (minBin) ;
      double max = signalRegion->GetBinLowEdge (maxBin + 1) ;

//      int minBin = m4_signal_DATA->FindBin (mMin.at (i)) ;       //PG analysis
//      int maxBin = m4_signal_DATA->FindBin (mMax.at (i)) ;       //PG analysis
//      double total = m4_signal_DATA->Integral (minBin, maxBin) ; //PG analysis
//      double min = m4_signal_DATA->GetBinLowEdge (minBin) ;
//      double max = m4_signal_DATA->GetBinLowEdge (maxBin + 1) ;

//      int minBin = signalRegionMC->FindBin (mMin.at (i)) ;       //PG closure test
//      int maxBin = signalRegionMC->FindBin (mMax.at (i)) ;       //PG closure test
//      double total = signalRegionMC->Integral (minBin, maxBin) ; //PG closure test

//      double bkg = fitFunc->Integral (min, max) ;
//      double error = 0. ;
//      for (int j = minBin ; j <= maxBin ; ++j) error += extrapolated_bkg_fitBand->GetBinError (j) ;

      double bkg_count_error = 0. ;
      double bkg_count = extrapolated_bkg->IntegralAndError (minBin, maxBin, bkg_count_error) ;
      
      cout << masses.at (i) 
           << " : (" << min
           << "," << max
           << ") : " << total //PG the bin width 
           << " : " << bkg_count
           << " +- " << bkg_count_error
           << "\n" ;
           
      g_total.SetPoint (i, masses.at (i), total) ;
      g_total.SetPointError (i, 50., sqrt (total)) ;
//      g_background.SetPoint (i, masses.at (i), bkg/10.) ;
//      g_background.SetPointError (i, 0., error) ;
      g_background_count.SetPoint (i, masses.at (i), bkg_count) ;
      g_background_count.SetPointError (i, 0., bkg_count_error) ;
      g_error.SetPoint (i, masses.at (i), sqrt (bkg_count_error * bkg_count_error + total)) ;
    } //PG loop on the mass points

  c1->SetLogy () ;
  g_total.SetMarkerStyle (6) ;
  g_background_count.SetLineColor (kOrange) ;
  g_background_count.SetFillColor (kOrange) ;
  g_background_count.Draw ("AE3") ;
  g_total.Draw ("EPsame") ;
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

  TFile output ("output_004_10.root", "recreate") ;
  output.cd () ;
  ratio_total->Write () ;
  gaussianBand->Write () ;
  poolPlot.Write () ;
  poolPlotGaus.Write () ;
  extrapolated_bkg->Write () ;
//  extrapolated_bkg_fitBand->Write () ;
  correctionPlane->Write () ;
  correctionBand->Write () ;
  g_total.Write ("g_total") ;
  g_background_count.Write ("g_background_count") ;
  g_error.Write ("g_error") ;
  g_expectedCountings.Write ("g_expectedCountings") ;
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