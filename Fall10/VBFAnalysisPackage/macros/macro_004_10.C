#include "ntupleUtils.h"
#include "../interface/Functions.h"


void setDoubleExpPars (TF1 * func)
{
  //PG numbers from Andrea
  func->SetParameter (0, 10.) ;
  func->SetParameter (1, 0.013) ;
  func->SetParameter (2, 5.) ;
  func->SetParameter (3, 0.005) ;
  
  func->SetParName (0, "N1") ;
  func->SetParName (1, "#lambda1") ;
  func->SetParName (2, "N2") ;
  func->SetParName (3, "#lambda2") ;
  func->SetLineWidth (1) ;
  func->SetLineColor (kBlue+2) ;
  func->SetNpx (10000) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



//PG FIXME questo e' corretto?
void setAttDoubleExpPars (TF1 * func)
{
  //PG numbers from Andrea
  func->SetParameter (0, 180.) ;
  func->SetParameter (1, 11.) ;
  func->SetParameter (2, 10.) ;
  func->SetParameter (3, 0.012) ;
  func->SetParameter (4, 5.) ;
  func->SetParameter (5, 0.005) ;
  
  func->SetParLimits (0, 150., 250.) ;
  func->SetParLimits (1, 0., 100.) ;
  
  func->SetParName (0, "#mu") ;
  func->SetParName (1, "kT") ;
  func->SetParName (2, "N1") ;
  func->SetParName (3, "#lambda1") ;
  func->SetParName (4, "N2") ;
  func->SetParName (5, "#lambda2") ;
  func->SetLineWidth (1) ;
  func->SetLineColor (kBlue+2) ;
  func->SetNpx (10000) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void setCBPars (TF1 * func, int scale)
{
  //PG N, gaus m, gaus s, joint, exp, fermi E, kT
  func->SetParameter (0, scale) ;
  func->SetParameter (1, 200.) ;
  func->SetParameter (2, 20.) ;
  func->SetParameter (3, 0.1) ;
  func->SetParameter (4, 10.) ;
  func->SetParameter (5, 200) ;
  func->SetParameter (6, 10) ;

  func->SetLineWidth (1) ;
  func->SetLineColor (kBlue+2) ;
  func->SetNpx (10000) ;
  return ;
  
  //PG alternative parameters
//  func->SetParameter (0, scale) ;
//  func->SetParameter (1, 180.) ;
//  func->SetParameter (2, 23.) ;
//  func->SetParameter (3, 0.3) ;
//  func->SetParameter (4, 20.) ;
//  func->SetParameter (5, 180) ;
//  func->SetParameter (6, 15) ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


void legendMaquillage (TLegend * leg)
{
  leg->SetBorderSize (0) ;
  leg->SetTextFont (42) ;
  leg->SetTextSize (0.04) ;
  leg->SetLineColor (1) ;
  leg->SetLineStyle (1) ;
  leg->SetLineWidth (1) ;
  leg->SetFillColor (0) ;
  leg->SetFillStyle (0) ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/**
  dump a tprofile in a TH1F, so that the errors are easily modifiable afterwards
*/
TH1F * dumpProfile (TString outputName, TProfile * input)
{
  TH1F * output = new TH1F (outputName, outputName, input->GetNbinsX (), input->GetXaxis ()->GetXmin (), input->GetXaxis ()->GetXmax ()) ;
  for (int iBin = 1 ; iBin <= input->GetNbinsX () ; ++iBin)
    {
      double val = input->GetBinContent (iBin) ;
      output->SetBinContent (iBin, val) ;
      double err = input->GetBinError (iBin) ;
      output->SetBinError (iBin, err) ;
    }
  return output ;  
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


/**
 add to the output the relative error in the input
*/ 
// void addError (TH1F * modified, TH1F * source)
// {
//   for (int iBin = 1 ; iBin <= modified->GetNbinsX () ; ++iBin) 
//     {
//       ;
//     }
// 
// }


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int macro_004_10 (int mass)
{
//  TString inputFile = "testBkg_004_S" ;
//  TString inputFile = "testBkg_004_mu_S" ; //PG only muons
//  TString inputFile = "testBkg_004_el_S" ; //PG only muons
//  TString inputFile = "testBkg_004_5GeV_S" ;
//  TString inputFile = "testBkg_004_noKF_S" ; //PG no kinematic fit
  TString inputFile = "testBkg_004_S" ; //PG kinematic fit
//  TString inputFile = "testBkg_004_20GeV_S" ; //PG kinematic fit, coarse binning
  inputFile += mass ;
  inputFile += ".root" ;
  cout << inputFile << endl ;
  TFile input (inputFile) ;

  bool makeToys = true ; //PG make toys to eval the error
       makeToys = false ; //PG error as the ratio of the errors of the function
  int nToys = 10000 ;
  bool scaleBand = false ; //PG scale the band according to the pool wrt the MC histo
  bool makeBkgFit = true ; //PG smooth the extrap bkg shape with a double exp fit
                            //PG leave it false by now, it generates a big bias
 
  //PG get the histograms
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  //PG get the bkg samples

  THStack * stack_m4_EvenHigher = (THStack *) input.Get ("stack_m4_EvenHigher") ;
  TH1F * m4_EvenHigher_total = (TH1F*) stack_m4_EvenHigher->GetStack ()->Last () ;  
  m4_EvenHigher_total->SetTitle ("") ;  

  THStack * stack_m4_upper = (THStack *) input.Get ("stack_m4_upper") ;
  TH1F * m4_upper_total = (TH1F*) stack_m4_upper->GetStack ()->Last () ;
  m4_upper_total->SetTitle ("") ;
    
  THStack * stack_m4_upper_a = (THStack *) input.Get ("stack_m4_upper_a") ;
  TH1F * m4_upper_a_total = (TH1F*) stack_m4_upper_a->GetStack ()->Last () ;
  m4_upper_a_total->SetTitle ("") ;
    
  THStack * stack_m4_upper_c = (THStack *) input.Get ("stack_m4_upper_c") ;
  TH1F * m4_upper_c_total = (TH1F*) stack_m4_upper_c->GetStack ()->Last () ;
  m4_upper_c_total->SetTitle ("") ;
    
  THStack * stack_m4_lower = (THStack *) input.Get ("stack_m4_lower") ;
  TH1F * m4_lower_total = (TH1F *) stack_m4_lower->GetStack ()->Last () ;
  m4_lower_total->SetTitle ("") ;
    
  THStack * stack_m4_lower_a = (THStack *) input.Get ("stack_m4_lower_a") ;
  TH1F * m4_lower_a_total = (TH1F *) stack_m4_lower_a->GetStack ()->Last () ;
  m4_lower_a_total->SetTitle ("") ;
    
  THStack * stack_m4_lower_c = (THStack *) input.Get ("stack_m4_lower_c") ;
  TH1F * m4_lower_c_total = (TH1F *) stack_m4_lower_c->GetStack ()->Last () ;
  m4_lower_c_total->SetTitle ("") ;
    
  THStack * stack_m4_signal = (THStack *) input.Get ("stack_m4_signal") ;
  TH1F * m4_signal_total = (TH1F *) stack_m4_signal->GetStack ()->Last () ;
  m4_signal_total->SetTitle ("") ;
  
  THStack * stack_m4_sideband = (THStack *) input.Get ("stack_m4_sideband") ;
  TH1F * m4_sideband_total = (TH1F *) stack_m4_sideband->GetStack ()->Last () ;
  m4_sideband_total->SetTitle ("") ;
  
  //PG get the signal samples

  THStack * stack_m4_EvenHigher_SIG = (THStack *) input.Get ("stack_m4_EvenHigher_SIG") ;
  TH1F * m4_EvenHigher_total_SIG = (TH1F*) stack_m4_EvenHigher_SIG->GetStack ()->Last () ;    
  m4_EvenHigher_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_upper_SIG = (THStack *) input.Get ("stack_m4_upper_SIG") ;
  TH1F * m4_upper_total_SIG = (TH1F*) stack_m4_upper_SIG->GetStack ()->Last () ;
  m4_upper_total_SIG->SetTitle ("") ;
      
  THStack * stack_m4_upper_a_SIG = (THStack *) input.Get ("stack_m4_upper_a_SIG") ;
  TH1F * m4_upper_a_total_SIG = (TH1F*) stack_m4_upper_a_SIG->GetStack ()->Last () ;
  m4_upper_a_total_SIG->SetTitle ("") ;
      
  THStack * stack_m4_upper_c_SIG = (THStack *) input.Get ("stack_m4_upper_c_SIG") ;
  TH1F * m4_upper_c_total_SIG = (TH1F*) stack_m4_upper_c_SIG->GetStack ()->Last () ;
  m4_upper_c_total_SIG->SetTitle ("") ;
      
  THStack * stack_m4_lower_SIG = (THStack *) input.Get ("stack_m4_lower_SIG") ;
  TH1F * m4_lower_total_SIG = (TH1F *) stack_m4_lower_SIG->GetStack ()->Last () ;
  m4_lower_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_lower_a_SIG = (THStack *) input.Get ("stack_m4_lower_a_SIG") ;
  TH1F * m4_lower_a_total_SIG = (TH1F *) stack_m4_lower_a_SIG->GetStack ()->Last () ;
  m4_lower_a_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_lower_c_SIG = (THStack *) input.Get ("stack_m4_lower_c_SIG") ;
  TH1F * m4_lower_c_total_SIG = (TH1F *) stack_m4_lower_c_SIG->GetStack ()->Last () ;
  m4_lower_c_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_signal_SIG = (THStack *) input.Get ("stack_m4_signal_SIG") ;
  TH1F * m4_signal_total_SIG = (TH1F *) stack_m4_signal_SIG->GetStack ()->Last () ;
  m4_signal_total_SIG->SetTitle ("") ;
  
  THStack * stack_m4_sideband_SIG = (THStack *) input.Get ("stack_m4_sideband_SIG") ;
  TH1F * m4_sideband_total_SIG = (TH1F *) stack_m4_sideband_SIG->GetStack ()->Last () ;
  m4_sideband_total_SIG->SetTitle ("") ;
  
  //PG get the data
  
  TH1F * m4_EvenHigher_DATA = (TH1F *) input.Get ("m4_EvenHigher_DATA") ;      
  m4_EvenHigher_DATA->SetTitle ("") ;
  TH1F * m4_upper_DATA = (TH1F *) input.Get ("m4_upper_DATA") ;      
  m4_upper_DATA->SetTitle ("") ;
  TH1F * m4_upper_a_DATA = (TH1F *) input.Get ("m4_upper_a_DATA") ;      
  m4_upper_a_DATA->SetTitle ("") ;
  TH1F * m4_upper_c_DATA = (TH1F *) input.Get ("m4_upper_c_DATA") ;      
  m4_upper_c_DATA->SetTitle ("") ;
  TH1F * m4_signal_DATA = (TH1F *) input.Get ("m4_signal_DATA") ;    
  m4_signal_DATA->SetTitle ("") ;
  TH1F * m4_lower_DATA = (TH1F *) input.Get ("m4_lower_DATA") ;      
  m4_lower_DATA->SetTitle ("") ;
  TH1F * m4_lower_a_DATA = (TH1F *) input.Get ("m4_lower_a_DATA") ;      
  m4_lower_a_DATA->SetTitle ("") ;
  TH1F * m4_lower_c_DATA = (TH1F *) input.Get ("m4_lower_c_DATA") ;      
  m4_lower_c_DATA->SetTitle ("") ;
  TH1F * m4_sideband_DATA = (TH1F *) input.Get ("m4_sideband_DATA") ;
  m4_sideband_DATA->SetTitle ("") ;

  //PG which histograms I use = possible analysis configurations
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

/*
  cout << " A\'B\'C\'D\' test with upper and EvenHigher" << endl ;
  TH1F * sidebaRegionMC = m4_EvenHigher_total ; //PG analysis
  TH1F * signalRegionMC = m4_upper_total ; //PG analysis 
  TH1F * sidebaRegion   = m4_EvenHigher_DATA ;
  TH1F * signalRegion   = m4_upper_DATA ;  
*/

/*
  cout << " A\'B\'C\'D\' test with upper and lower" << endl ;
  TH1F * sidebaRegionMC = m4_lower_total ; 
  TH1F * signalRegionMC = m4_upper_total ;  
  TH1F * sidebaRegion   = m4_lower_DATA ;
  TH1F * signalRegion   = m4_upper_DATA ;  
*/

/*
  cout << "upper sideband analysis" << endl ;
  TH1F * sidebaRegionMC = m4_upper_total ; 
  TH1F * signalRegionMC = m4_signal_total ;
  TH1F * sidebaRegion   = m4_upper_DATA ;
  TH1F * signalRegion   = m4_signal_DATA ;  
*/

/*
  cout << "upper sideband analysis closure test" << endl ;
  TH1F * sidebaRegionMC = m4_upper_total ; 
  TH1F * signalRegionMC = m4_signal_total ;  
  TH1F * sidebaRegion   = sidebaRegionMC ; 
  TH1F * signalRegion   = signalRegionMC ; 
*/

/*
  cout << "final analysis" << endl ;
  TH1F * sidebaRegionMC = m4_sideband_total ; 
  TH1F * signalRegionMC = m4_signal_total ;
  TH1F * sidebaRegion   = m4_sideband_DATA ;
  TH1F * signalRegion   = m4_signal_DATA ;  
*/

/*
  cout << "final analysis closure test" << endl ;
  TH1F * sidebaRegionMC = m4_sideband_total ; 
  TH1F * signalRegionMC = m4_signal_total ;  
  TH1F * sidebaRegion   = sidebaRegionMC ; 
  TH1F * signalRegion   = signalRegionMC ; 
*/

/*
  cout << "final analysis closure test with signal injection" << endl ;
  TH1F * sidebaRegionMC = m4_sideband_total ; 
  TH1F * signalRegionMC = m4_signal_total ;  
  TH1F * sidebaRegion   = sidebaRegionMC ;
  sidebaRegion->Add (m4_sideband_total_SIG) ; 
  TH1F * signalRegion   = signalRegionMC ; 
  signalRegion->Add (m4_signal_total_SIG) ; 
*/

  cout << "Martijn's test" << endl ;
  TH1F * sidebaRegionMC = m4_upper_a_total ; 
  sidebaRegionMC->Add (m4_lower_a_total) ;
  TH1F * signalRegionMC = m4_upper_c_total ; 
  signalRegionMC->Add (m4_lower_c_total) ;
  TH1F * sidebaRegion = m4_upper_a_DATA ; 
  sidebaRegion->Add (m4_lower_a_DATA) ;
  TH1F * signalRegion = m4_upper_c_DATA ; 
  signalRegion->Add (m4_lower_c_DATA) ;

  //PG plot MC singal and background region 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TCanvas * c1 = new TCanvas () ;

  TLegend * leg_signal = makeLegendTitle (*stack_m4_signal) ;
  stack_m4_signal->Draw ("hist") ;
  leg_signal->Draw () ;
  c1->Print ("signal_stack.pdf", "pdf") ;

  TLegend * leg_sideband = makeLegendTitle (*stack_m4_sideband) ;
  stack_m4_sideband->Draw ("hist") ;
  leg_sideband->Draw () ;
  c1->Print ("sideband_stack_MC.pdf", "pdf") ;

  //PG correction factor from the ratio of histograms 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * ratio_total = (TH1F *) signalRegionMC->Clone ("ratio") ;
  ratio_total->Divide (sidebaRegionMC) ;
  ratio_total->SetMarkerColor (kOrange) ;

  int nBins =  m4_signal_DATA->GetNbinsX () ;
  double m4_min = m4_signal_DATA->GetXaxis ()->GetXmin () ;
  double m4_max = m4_signal_DATA->GetXaxis ()->GetXmax () ;
 
  double binSize = (m4_max - m4_min) / nBins ;
  double startFit = 200 ; //GeV, bin from where to start the num & den fit for corr factor
  double endFit = 1000 ;   //GeV, bin from where to end the num & den fit for corr factor
  int fitBins = (endFit-startFit) / binSize ;

  cout << nBins << " " << m4_min << " " << m4_max << " " << binSize << endl ;

  //PG fit separately numerator and denominator of MC 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TF1 * numFitFunc = new TF1 ("numFitFunc", doubleExponential, 0., 1000., 4) ;
  setDoubleExpPars (numFitFunc) ;

//  TF1 * numFitFunc = new TF1 ("numFitFunc", attenuatedDoubleExponential, 0., 1000., 6) ;
//  setAttDoubleExpPars (numFitFunc) ;

//  TF1 * numFitFunc = new TF1 ("numFitFunc", attenuatedCB, 0., 1000., 7) ;
//  setCBPars (numFitFunc, signalRegionMC->Integral () / 5.) ;

  TFitResultPtr fitResultPtr = signalRegionMC->Fit (numFitFunc, "L", "", startFit, endFit) ;
  int fitStatus = (int)(fitResultPtr) ;
  int loops = 0 ; 
  while (fitStatus != 0 && loops < 30)
    {
      fitResultPtr = signalRegionMC->Fit (numFitFunc, "L", "", startFit, endFit) ;
      fitStatus = (int)(fitResultPtr) ;
      ++loops ;
    }
  cout << "`--> " << fitStatus << " @ " << loops << "\n" ;

  TH1F * num_fit_error = new TH1F ("num_fit_error", "", fitBins, startFit, endFit) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (num_fit_error, 0.68) ;

  num_fit_error->SetFillColor (kGray+2) ;
  signalRegionMC->Draw () ;
  num_fit_error->Draw ("E3same") ;
  signalRegionMC->Draw ("sames") ;
  c1->Print ("numerator_fit.pdf", "pdf") ;
  c1->SetLogy () ;
  signalRegionMC->Draw () ;
  num_fit_error->Draw ("E3same") ;
  signalRegionMC->Draw ("sames") ;
  c1->Print ("numerator_fit_log.pdf", "pdf") ;
  c1->SetLogy (0) ;

  signalRegion->Draw () ;
  c1->Print ("signal.pdf", "pdf") ;
  c1->SetLogy () ;
  signalRegion->Draw () ;
  c1->Print ("signal_log.pdf", "pdf") ;
  c1->SetLogy (0) ;

  TF1 * denFitFunc = new TF1 ("denFitFunc", doubleExponential, 0., 1000., 4) ;
  setDoubleExpPars (denFitFunc) ;

//  TF1 * denFitFunc = new TF1 ("denFitFunc", attenuatedDoubleExponential, 0., 1000., 6) ;
//  setAttDoubleExpPars (denFitFunc) ;

//  TF1 * denFitFunc = new TF1 ("denFitFunc", attenuatedCB, 0., 1000., 7) ;
//  setCBPars (denFitFunc, sidebaRegionMC->Integral () / 5.) ;

  fitResultPtr = sidebaRegionMC->Fit (denFitFunc, "L", "", startFit, endFit) ;
  fitStatus = (int)(fitResultPtr) ;
  loops = 0 ; 
  while (fitStatus != 0 && loops < 50)
    {
      TFitResultPtr fitResultPtr = sidebaRegionMC->Fit (denFitFunc, "L", "", startFit, endFit) ;
      fitStatus = (int)(fitResultPtr) ;
      ++loops ;
    }
  cout << "`--> " << fitStatus << " @ " << loops << "\n" ;

  TH1F * den_fit_error = new TH1F ("den_fit_error", "", fitBins, startFit, endFit) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (den_fit_error, 0.68) ;

  den_fit_error->SetFillColor (kGray+2) ;
  sidebaRegionMC->Draw () ;
  den_fit_error->Draw ("E3same") ;
  sidebaRegionMC->Draw ("sames") ;
  c1->Print ("denominator_fit.pdf", "pdf") ;
  c1->SetLogy () ;
  sidebaRegionMC->Draw () ;
  den_fit_error->Draw ("E3same") ;
  sidebaRegionMC->Draw ("sames") ;
  c1->Print ("denominator_fit_log.pdf", "pdf") ;
  c1->SetLogy (0) ;

  sidebaRegion->Draw () ;
  c1->Print ("sideband.pdf", "pdf") ;
  c1->SetLogy () ;
  sidebaRegion->Draw () ;
  c1->Print ("sideband_log.pdf", "pdf") ;
  c1->SetLogy (0) ;

  TH1F * h_correctionBand ;

  //PG toy experiments to determine the size of the error band on the extrapolation factor
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  if (makeToys)
    {
      TRandom3 r ;
      TH2F * correctionPlane = new TH2F ("correctionPlane", "", nBins, m4_min, m4_max, 600, 0, 9) ;
    
      TH1F * dummyNum = (TH1F *) signalRegionMC->Clone ("dummyNum") ;
      TH1F * dummyDen = (TH1F *) sidebaRegionMC->Clone ("dummyDen") ;
      double intSignal = signalRegionMC->Integral () * 2.6 / 2.1 ;   //PG put in the W+jets MC statistics for the uncertainty
      double intSideband = sidebaRegionMC->Integral () * 2.6 / 2.1 ; //PG put in the W+jets MC statistics for the uncertainty
     
      for (int iToy = 0 ; iToy < nToys ; ++iToy)
        {
          if (iToy %(nToys/10) == 0) cout << "toy number " << iToy << endl ;
    
          int nNum = r.Poisson (intSignal) ;
          dummyNum->Reset () ;
          dummyNum->FillRandom ("numFitFunc", nNum) ;
    
          int nDen = r.Poisson (intSideband) ;
          dummyDen->Reset () ;
          dummyDen->FillRandom ("denFitFunc", nDen) ;
    
          dummyNum->Divide (dummyDen) ;
          for (int iBin = 1 ; iBin <= dummyNum->GetNbinsX () ; ++iBin)
            {
              correctionPlane->Fill (dummyNum->GetBinCenter (iBin), dummyNum->GetBinContent (iBin)) ;
            }
        }

      //PG correction factor from the profile of the many toys
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

      TProfile * correctionBand = correctionPlane->ProfileX ("correctionBand", 1, -1, "s") ;
      TH1F * h_correctionBand = dumpProfile ("h_correctionBand", correctionBand) ;

      //PG use the ratio of functions as central value
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      
      for (int iBin = 1 ; iBin <= h_correctionBand->GetNbinsX () ; ++iBin)
        {
          double center = h_correctionBand->GetBinCenter (iBin) ;
          double corr = numFitFunc->Eval (center) / denFitFunc->Eval (center) ; 
          h_correctionBand->SetBinContent (iBin, corr) ;
        }

      //PG correction factor from the gaussian fit to slices of the many toys
      //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
      //PG FIXME this needs to be extracted from the loop
    
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

    }
  else 
    {
      // prepare the histo
      h_correctionBand = (TH1F *) signalRegionMC->Clone ("h_correctionBand") ;
      h_correctionBand->Reset () ;
      for (int iBin = 1 ; iBin <= h_correctionBand->GetNbinsX () ; ++iBin)
        {
          // fill the histo central values
          double center = h_correctionBand->GetBinCenter (iBin) ;
          double corr = numFitFunc->Eval (center) / denFitFunc->Eval (center) ; 
          h_correctionBand->SetBinContent (iBin, corr) ;
          // fill the histo errors
          int binForError = num_fit_error->GetXaxis ()->FindBin (center) ;
          double error = 0. ;
          //PG under the hipothesis of having the error only for the bins of the fit, need to check this first
          if (binForError > 0 && binForError <= num_fit_error->GetNbinsX ())
            {
              double numErr = num_fit_error->GetBinError (binForError) / num_fit_error->GetBinContent (binForError) ;
              double denErr = den_fit_error->GetBinError (binForError)  / den_fit_error->GetBinContent (binForError) ;
              error = sqrt (numErr * numErr + denErr * denErr) ;
              error *= h_correctionBand->GetBinContent (iBin) ;
            }
//          cout << " CB " << iBin 
//               << " | m : " << center
//               << " | corr : " << corr 
//               << " | errBin : " << binForError
//               << " | err : " << error << endl ;

          h_correctionBand->SetBinError (iBin, error) ;
        }
    }

  h_correctionBand->SetStats (0) ;
  h_correctionBand->SetFillColor (kOrange) ;
  h_correctionBand->SetMarkerStyle (29) ;
   
  TLegend * leg_correctionFactor = new TLegend (0.5, 0.8, 0.95, 0.95, NULL, "brNDC") ;
  legendMaquillage (leg_correctionFactor) ;
  leg_correctionFactor->AddEntry (h_correctionBand, "correction band", "pf") ;
  leg_correctionFactor->AddEntry (ratio_total, "MC ratio", "pl") ;

  c1->DrawFrame (100, 0.01, 800, 2.) ;
//  correctionPlane->SetStats (0) ;
//  correctionPlane->Draw ("COLZ") ;
  h_correctionBand->Draw ("E3same") ;
  h_correctionBand->Draw ("Psame") ;
//  gaussianBand->Draw ("E3same") ;
  gStyle->SetPalette (1) ;
  ratio_total->Draw ("same") ;
  leg_correctionFactor->Draw () ;
  c1->Print ("correctionFactor.pdf", "pdf") ;

  //PG look at the point distro wrt the band width, to determine whether the extrap. factor is ok
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F pullPlot ("pullPlot", "", 50, -5, 5) ;
//  TH1F pullPlotGaus ("pullPlotGaus", "", 50, -5, 5) ; FIXME this is there only in the case of toys, commented by now, therefore
  for (int iBin = 1 ; iBin <= h_correctionBand->GetNbinsX () ; ++iBin) 
    {
      double num = ratio_total->GetBinContent (iBin) ;
//      double mean = ((TH1F *) aSlices.At (1))->GetBinContent (iBin) ;  FIXME this is there only in the case of toys, commented by now, therefore
//      double sigma = ((TH1F *) aSlices.At (2))->GetBinContent (iBin) ; FIXME this is there only in the case of toys, commented by now, therefore
//      pullPlotGaus.Fill ((num - mean) / sigma) ; FIXME this is there only in the case of toys, commented by now, therefore
      double mean = h_correctionBand->GetBinContent (iBin) ;
      double sigma = h_correctionBand->GetBinError (iBin) ;
      pullPlot.Fill ((num - mean) / sigma) ;
    }
  pullPlot.Fit ("gaus","L") ;
//  c1->Print ("pullPlot.pdf", "pdf") ;
//  pullPlotGaus.Fit ("gaus","L") ;         FIXME this is there only in the case of toys, commented by now, therefore
//  c1->Print ("pullPlotGaus.pdf", "pdf") ; FIXME this is there only in the case of toys, commented by now, therefore

//PG FIXME this is there only in the case of toys, commented by now, therefore
//  //PG scale the error on the correction band by the sigma of the pullPlot, if bigger than 1
//  double poolScaleGaus = pullPlotGaus.GetFunction ("gaus")->GetParameter (2) ;
//  if (poolScaleGaus > 1) 
//    {
//      for (int iBin = 1 ; iBin <= gaussianBand->GetNbinsX () ; ++iBin)
//        {
//          double newError = poolScaleGaus * gaussianBand->GetBinError (iBin) ;
//          gaussianBand->SetBinError (iBin, newError) ;
//        }
//    }
// 

  //PG scale the error on the correction band by the sigma of the pullPlot, if bigger than 1
  if (scaleBand)
    {
      double poolScale = pullPlot.GetFunction ("gaus")->GetParameter (2) ;
      if (poolScale > 1) 
        {
          for (int iBin = 1 ; iBin <= h_correctionBand->GetNbinsX () ; ++iBin)
            {
              double newError = poolScale * h_correctionBand->GetBinError (iBin) ;
              h_correctionBand->SetBinError (iBin, newError) ;
            }
        }
    } 
  //PG calculate the extrapolated background
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  TH1F * extrapolated_bkg = sidebaRegion->Clone ("extrapolated_bkg") ;

  extrapolated_bkg->Multiply (h_correctionBand) ; //PG profile correction
//  extrapolated_bkg->Multiply (gaussianBand) ; //PG gaus fit correction

  //PG  plot of the result
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

//  extrapolated_bkg_fitBand->SetFillColor (kGray+2) ;
//  extrapolated_bkg_fitBand->SetFillStyle (3004) ;
  
//  c1->DrawFrame (100, 0.1, 800, 5000) ;
//  extrapolated_bkg->Draw ("E3same") ;
//  extrapolated_bkg_fitBand->Draw ("E3same") ;
//  extrapolated_bkg_fitBand->Draw () ;

  extrapolated_bkg->SetStats (0) ;
  extrapolated_bkg->SetTitle ("") ;
  extrapolated_bkg->SetLineColor (kBlack) ;
  extrapolated_bkg->SetFillColor (kOrange) ;

  extrapolated_bkg->Draw ("E3P") ;
//  extrapolated_bkg_fitBand->Draw ("E3same") ;
  c1->Print ("extrapolatedBkg.pdf", "pdf") ;
  c1->SetLogy () ;
  c1->Update () ;
  c1->Print ("extrapolatedBkg_log.pdf", "pdf") ;
  c1->SetLogy (0) ;


  //PG fit the extrapolated bkg
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  if (makeBkgFit)
    {
      TF1 * bkgFitFunc = new TF1 ("bkgFitFunc", doubleExponential, 0., 1000., 4) ;
      setDoubleExpPars (bkgFitFunc) ;
    
      TFitResultPtr fitResultPtr2 = extrapolated_bkg->Fit (bkgFitFunc, "L", "", startFit, endFit) ;
      fitStatus = (int)(fitResultPtr2) ;
      loops = 0 ; 
      while (fitStatus != 0 && loops < 30)
        {
          fitResultPtr2 = extrapolated_bkg->Fit (bkgFitFunc, "L", "", startFit, endFit) ;
          fitStatus = (int)(fitResultPtr2) ;
          ++loops ;
        }
      cout << "`--> " << fitStatus << " @ " << loops << "\n" ;
    
      TH1F * extrapolated_bkg_fitBand = new TH1F ("extrapolated_bkg_fitBand", "", fitBins, startFit, endFit) ;
      (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (extrapolated_bkg_fitBand, 0.68) ;
      
      //PG set the error to this estimate of the background
      for (iBin = 1 ; iBin <= extrapolated_bkg.GetNbinsX () ; ++iBin)
        {
          double center = extrapolated_bkg->GetBinCenter (iBin) ;
          int iBinBand = extrapolated_bkg_fitBand->GetXaxis ()->FindBin (center) ;

          double errBkg = 0. ;
          if (extrapolated_bkg->GetBinContent (iBin))
            errBkg = extrapolated_bkg->GetBinError (iBin) /
                     extrapolated_bkg->GetBinContent (iBin) ;
          double errFit = 0. ;
          if (extrapolated_bkg_fitBand->GetBinContent (iBinBand))
            errFit = extrapolated_bkg_fitBand->GetBinError (iBinBand) /
                     extrapolated_bkg_fitBand->GetBinContent (iBinBand) ;
          double errTot = sqrt (errFit * errFit + errBkg * errBkg) * extrapolated_bkg_fitBand->GetBinContent (iBinBand) ; 
          extrapolated_bkg_fitBand->SetBinError (iBinBand, errTot) ;                    
        }

      c1->SetLogy () ;
      extrapolated_bkg->Draw ("E3") ;
      extrapolated_bkg_fitBand->SetFillStyle (3001) ;
      extrapolated_bkg_fitBand->SetFillColor (kBlue) ;
      extrapolated_bkg_fitBand->Draw ("E3same") ;
      c1->Print ("fittedBackground.pdf", "pdf") ;
      c1->SetLogy (0) ;
      c1->Update () ;
      c1->Print ("fittedBackground_lin.pdf", "pdf") ;
      
      //PG NB here I change the background I use,
      //PG therefore I loose the old bkg!
      extrapolated_bkg = extrapolated_bkg_fitBand ;
    }
  //PG first plot of the result
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  signalRegionMC->SetStats (0) ;
  signalRegionMC->SetMarkerStyle (24) ;
  signalRegionMC->SetMarkerColor (kBlack) ;

  TLegend * leg_extrapAndMc = new TLegend (0.5, 0.8, 0.95, 0.95, NULL, "brNDC") ;
  legendMaquillage (leg_extrapAndMc) ;
  leg_extrapAndMc->AddEntry (signalRegionMC, "simulation in SR", "p") ;
  leg_extrapAndMc->AddEntry (extrapolated_bkg, "extrapolated bkg in SR", "fp") ;

  c1->SetLogy () ;
//  c1->DrawFrame (100, 0.1, 800, 5000) ;
  extrapolated_bkg->Draw ("E3") ;
  signalRegionMC->Draw ("Psame") ;
  leg_extrapAndMc->Draw () ;
  c1->Print ("extrapAndMc.pdf", "pdf") ;
  c1->SetLogy (0) ;
  c1->Update () ;
  c1->Print ("extrapAndMc_lin.pdf", "pdf") ;

  //PG the pull trend
  pair<TGraphErrors*, TGraphErrors*> extrapAndMc_pulls =
    getPullTrend (signalRegionMC, extrapolated_bkg) ;
  extrapAndMc_pulls.second->Draw ("AE3") ;
  extrapAndMc_pulls.first->SetMarkerStyle (24) ;
  extrapAndMc_pulls.first->Draw ("samePE") ;
  c1->Print ("extrapAndMc_pull.pdf", "pdf") ;

  TH1F * extrapAndMc_pull = getPullPlot (signalRegionMC, extrapolated_bkg, startFit, endFit) ;
  extrapAndMc_pull->Draw () ;
  c1->Print ("extrapAndMc_pull2.pdf", "pdf") ;
  
  TLegend * leg_extrapAndData = new TLegend (0.5, 0.8, 0.95, 0.95, NULL, "brNDC") ;
  legendMaquillage (leg_extrapAndData) ;
  leg_extrapAndData->AddEntry (signalRegion, "data in SR", "p") ;
  leg_extrapAndData->AddEntry (extrapolated_bkg, "extrapolated bkg in SR", "fp") ;

  c1->SetLogy () ;
  signalRegion->SetStats (0) ;
  extrapolated_bkg->Draw ("E3") ;
  signalRegion->SetMarkerStyle (20) ;
  signalRegion->Draw ("same") ;
  leg_extrapAndData->Draw () ;
  c1->Print ("extrapAndData.pdf", "pdf") ;
  c1->SetLogy (0) ;
  c1->Update () ;
  c1->Print ("extrapAndData_lin.pdf", "pdf") ;
  
  pair<TGraphErrors*, TGraphErrors*> extrapAndData_pulls =
    getPullTrend (signalRegion, extrapolated_bkg) ;
  extrapAndData_pulls.second->Draw ("AE3") ;
  extrapAndData_pulls.first->Draw ("samePE") ;
  c1->Print ("extrapAndData_pull.pdf", "pdf") ;

  TH1F * extrapAndData_pull = getPullPlot (signalRegion, extrapolated_bkg, startFit, endFit) ;
  extrapAndData_pull->Draw () ;
  c1->Print ("extrapAndData_pull2.pdf", "pdf") ;
  
  
  //PG prepare the windows for the fast cut-n-count thing
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  vector<double> masses ;
  vector<double> mMin ;
  vector<double> mMax ;
  masses.push_back (250.) ;  mMin.push_back (225) ; mMax.push_back (275) ; //PG Hwindow.push_back (24.) ;
  masses.push_back (300.) ;  mMin.push_back (270) ; mMax.push_back (330) ; //PG Hwindow.push_back (27.) ;
  masses.push_back (350.) ;  mMin.push_back (310) ; mMax.push_back (390) ; //PG Hwindow.push_back (32.) ;
  masses.push_back (400.) ;  mMin.push_back (355) ; mMax.push_back (445) ; //PG Hwindow.push_back (40.) ;
  masses.push_back (450.) ;  mMin.push_back (390) ; mMax.push_back (510) ; //PG Hwindow.push_back (46.) ;
  masses.push_back (500.) ;  mMin.push_back (415) ; mMax.push_back (575) ; //PG Hwindow.push_back (54.) ;
  masses.push_back (550.) ;  mMin.push_back (470) ; mMax.push_back (610) ; //PG Hwindow.push_back (61.) ;
  masses.push_back (600.) ;  mMin.push_back (485) ; mMax.push_back (665) ; //PG Hwindow.push_back (68.) ;

  for (int iMass = 0 ; iMass < masses.size () ; ++iMass)
    {
      if (mMin.at (iMass) < startFit) cout << "WARNING: mass " << masses.at (iMass) << " not trustable\n";  
      if (mMax.at (iMass) > endFit) cout << "WARNING: mass " << masses.at (iMass) << " not trustable\n";  
    }
  
  //PG the fast cut-n-count thing
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  
  TGraphErrors g_total ;
  TGraphErrors g_background_count ;
  TGraph g_error ;

  cout << "mass" 
       << " : (" << "min"
       << "," << "max"
       << ") : " << "total" //PG the bin width 
       << " : " << "bkg_count"
       << " +- " << "bkg_count_error"
       << " | "
       << "expSignal" << " : "
       << "bkg_count" << " : "
       << "bkg_count_error / bkg_count" << " : "
       << "total" << " : "
       << "\n" ;

  //PG loop on the mass points
  for (int i = 0 ; i < masses.size () ; ++i)
    {
      int minBin = -1;
      int maxBin = -1;
      int nBins = signalRegion -> GetNbinsX();
      float binWidth = signalRegion -> GetBinWidth(1);
      float minX = signalRegion -> GetBinLowEdge(1);
      float maxX = signalRegion -> GetBinLowEdge(nBins)+binWidth;
      
      for(int bin = 1; bin <= nBins; ++bin)
        {
          float binCenter = minX + 0.5*binWidth + binWidth*(bin-1);
          
          if( (binCenter >= mMin.at(i)) && (minBin == -1) ) minBin = bin;
          if( (binCenter >= mMin.at(i)) && (binCenter < mMax.at(i)) ) maxBin = bin;
        }

      double total = signalRegion->Integral (minBin, maxBin) ; //PG analysis
      double min = signalRegion->GetBinLowEdge (minBin) ;
      double max = signalRegion->GetBinLowEdge (maxBin + 1) ;

//      int minBinBkg = extrapolated_bkg->GetXaxis ()->FindBin (mMin.at (i)) ;
//      int maxBinBkg = extrapolated_bkg->GetXaxis ()->FindBin (mMax.at (i)) ;
      int minBinBkg = -1 ;
      int maxBinBkg = -1 ;
      for(int binBkg = 1; binBkg <= extrapolated_bkg->GetNbinsX() ; ++binBkg)
        {
          float binCenter = extrapolated_bkg->GetBinLowEdge(1) 
                            + 0.5 * extrapolated_bkg->GetBinWidth(1) 
                            + extrapolated_bkg->GetBinWidth(1)*(binBkg-1);
          
          if( (binCenter >= mMin.at(i)) && (minBinBkg == -1) ) minBinBkg = binBkg;
          if( (binCenter >= mMin.at(i)) && (binCenter < mMax.at(i)) ) maxBinBkg = binBkg;
        }


      double minBkg = extrapolated_bkg->GetBinLowEdge (minBinBkg) ;
      double maxBkg = extrapolated_bkg->GetBinLowEdge (maxBinBkg + 1) ;

      double bkg_count_error = 0. ;
      double bkg_count = extrapolated_bkg->IntegralAndError (minBinBkg, maxBinBkg, bkg_count_error) ;

      double expSignal = m4_signal_total_SIG->Integral (minBin, maxBin) ;

      cout << "MH | " << masses.at (i) 
           << " : (" << min
           << "," << max
           << ") : " << total //PG the bin width 
           << " : " << bkg_count
           << " +- " << bkg_count_error
           << " | "
           << expSignal << " : "
           << bkg_count << " : "
           << bkg_count_error / bkg_count << " : "
           << total << " : "
           << endl;
           
      g_total.SetPoint (i, masses.at (i), total) ;
      g_total.SetPointError (i, 50., sqrt (total)) ;
      g_background_count.SetPoint (i, masses.at (i), bkg_count) ;
      g_background_count.SetPointError (i, 0., bkg_count_error) ;
      g_error.SetPoint (i, masses.at (i), sqrt (bkg_count_error * bkg_count_error + total)) ;
    } //PG loop on the mass points

  g_total.SetMarkerStyle (6) ;
  g_background_count.SetLineColor (kOrange) ;
  g_background_count.SetFillColor (kOrange) ;

  TLegend * leg_results = new TLegend (0.5, 0.8, 0.95, 0.95, NULL, "brNDC") ;
  legendMaquillage (leg_results) ;
  leg_results->AddEntry (&g_total, "measured events in signal window", "p") ;
  leg_results->AddEntry (&g_background_count, "expected bkg in signal window", "f") ;

  c1->SetLogy () ;
  g_background_count.Draw ("AE3") ;
  g_total.Draw ("EPsame") ;
  leg_results->Draw () ;
  c1->Print ("results.pdf", "pdf") ;
  c1->SetLogy (0) ;

  c1->DrawFrame (200, 0, 700, 200) ;
  g_error.SetLineWidth (2) ;
  g_error.Draw ("L") ;
//  g_expectedCountings.SetMarkerStyle (8) ;
//  g_expectedCountings.Draw ("LP") ;
  c1->Print ("systematic_and_statistics.pdf", "pdf") ;

  TFile output ("output_004_10.root", "recreate") ;
  output.cd () ;
  ratio_total->Write () ;
//  gaussianBand->Write () ; //PG FIXME find another way to have this in the root file
  pullPlot.Write () ;
//  pullPlotGaus.Write () ; 
  extrapolated_bkg->Write () ;
//  extrapolated_bkg_fitBand->Write () ;      //PG FIXME find another way to have this in the root file
//  if (makeToys) correctionPlane->Write () ; //PG FIXME find another way to have this in the root file
//  if (makeToys) correctionBand->Write () ;  //PG FIXME find another way to have this in the root file
  m4_signal_total_SIG->Write ("sig") ;
  g_total.Write ("g_total") ;
  g_background_count.Write ("g_background_count") ;
  g_error.Write ("g_error") ;
//  g_expectedCountings.Write ("g_expectedCountings") ;
  h_correctionBand->Write () ;
  output.Close () ;

  gApplication->Terminate (0) ;
 
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
