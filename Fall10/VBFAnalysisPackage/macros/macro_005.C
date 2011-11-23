#include "./plotUtils.C"
#include "../interface/Functions.h"
 

void setSqrtErrors (TH1F * input)
{
  for (int iBin = 1 ; iBin <= input->GetNbinsX () ; ++iBin)
    {
      input->SetBinError (iBin, sqrt (input->GetBinContent (iBin))) ;
    }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


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


int macro_005 (int mass)
{

//   TString inputFile = "../testBkg_004_mu_S" ; //PG only muons
//   TString inputFile = "../testBkg_004_el_S" ; //PG only electrons
  TString inputFile = "../testBkg_004_S" ; //PG kinematic fit

  bool makeToys = true ; //PG make toys to eval the error
       makeToys = false ; //PG error as the ratio of the errors of the function
  int nToys = 10000 ;
  bool scaleBand = false ; //PG scale the band according to the pool wrt the MC histo
  
  
  bool subtractResonant = true ; //FC subtract resonant WW bkg
  
  inputFile += mass ;
  inputFile += ".root" ;
  cout << inputFile << endl ;
  TFile input (inputFile) ;
  
  
  //FC get the single histograms
  //FC ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  //FC get the bkg samples
  TH1F * m4_EvenHigher_DY   = (TH1F *) input.Get ("m4_EvenHigher_DY") ;
  TH1F * m4_EvenHigher_TT   = (TH1F *) input.Get ("m4_EvenHigher_TT") ;
  TH1F * m4_EvenHigher_WW   = (TH1F *) input.Get ("m4_EvenHigher_WW") ;
  TH1F * m4_EvenHigher_WZ   = (TH1F *) input.Get ("m4_EvenHigher_WZ") ;
  TH1F * m4_EvenHigher_Wjet = (TH1F *) input.Get ("m4_EvenHigher_Wjet") ;
  TH1F * m4_EvenHigher_ZZ   = (TH1F *) input.Get ("m4_EvenHigher_ZZ") ;
  TH1F * m4_EvenHigher_top  = (TH1F *) input.Get ("m4_EvenHigher_top") ;
  
  if (subtractResonant == false) {
   TH1F * m4_EvenHigher_total = (TH1F*) m4_EvenHigher_DY->Clone ("m4_EvenHigher_total");
   m4_EvenHigher_total->Add(m4_EvenHigher_TT);
   m4_EvenHigher_total->Add(m4_EvenHigher_WW);
   m4_EvenHigher_total->Add(m4_EvenHigher_WZ);
   m4_EvenHigher_total->Add(m4_EvenHigher_Wjet);
   m4_EvenHigher_total->Add(m4_EvenHigher_ZZ);
   m4_EvenHigher_total->Add(m4_EvenHigher_top);     
  }
  
  else {
   TH1F * m4_EvenHigher_total = (TH1F*) m4_EvenHigher_DY->Clone ("m4_EvenHigher_total");
   m4_EvenHigher_total->Add(m4_EvenHigher_TT);
   m4_EvenHigher_total->Add(m4_EvenHigher_WZ);
   m4_EvenHigher_total->Add(m4_EvenHigher_Wjet);
   m4_EvenHigher_total->Add(m4_EvenHigher_ZZ);
   m4_EvenHigher_total->Add(m4_EvenHigher_top);     
  }

  TH1F * m4_upper_DY   = (TH1F *) input.Get ("m4_upper_DY") ;
  TH1F * m4_upper_TT   = (TH1F *) input.Get ("m4_upper_TT") ;
  TH1F * m4_upper_WW   = (TH1F *) input.Get ("m4_upper_WW") ;
  TH1F * m4_upper_WZ   = (TH1F *) input.Get ("m4_upper_WZ") ;
  TH1F * m4_upper_Wjet = (TH1F *) input.Get ("m4_upper_Wjet") ;
  TH1F * m4_upper_ZZ   = (TH1F *) input.Get ("m4_upper_ZZ") ;
  TH1F * m4_upper_top  = (TH1F *) input.Get ("m4_upper_top") ;

  if (subtractResonant == false) {
   TH1F * m4_upper_total = (TH1F*) m4_upper_DY->Clone ("m4_upper_total");
   m4_upper_total->Add(m4_upper_TT);
   m4_upper_total->Add(m4_upper_WW);
   m4_upper_total->Add(m4_upper_WZ);
   m4_upper_total->Add(m4_upper_Wjet);
   m4_upper_total->Add(m4_upper_ZZ);
   m4_upper_total->Add(m4_upper_top);     
  }
  
  else {
   TH1F * m4_upper_total = (TH1F*) m4_upper_DY->Clone ("m4_upper_total");
   m4_upper_total->Add(m4_upper_TT);
   m4_upper_total->Add(m4_upper_WZ);
   m4_upper_total->Add(m4_upper_Wjet);
   m4_upper_total->Add(m4_upper_ZZ);
   m4_upper_total->Add(m4_upper_top);     
  }
  
  TH1F * m4_upper_c_DY   = (TH1F *) input.Get ("m4_upper_c_DY") ;
  TH1F * m4_upper_c_TT   = (TH1F *) input.Get ("m4_upper_c_TT") ;
  TH1F * m4_upper_c_WW   = (TH1F *) input.Get ("m4_upper_c_WW") ;
  TH1F * m4_upper_c_WZ   = (TH1F *) input.Get ("m4_upper_c_WZ") ;
  TH1F * m4_upper_c_Wjet = (TH1F *) input.Get ("m4_upper_c_Wjet") ;
  TH1F * m4_upper_c_ZZ   = (TH1F *) input.Get ("m4_upper_c_ZZ") ;
  TH1F * m4_upper_c_top  = (TH1F *) input.Get ("m4_upper_c_top") ;
  
  if (subtractResonant == false) {
   TH1F * m4_upper_c_total = (TH1F*) m4_upper_c_DY->Clone ("m4_upper_c_total");
   m4_upper_c_total->Add(m4_upper_c_TT);
   m4_upper_c_total->Add(m4_upper_c_WW);
   m4_upper_c_total->Add(m4_upper_c_WZ);
   m4_upper_c_total->Add(m4_upper_c_Wjet);
   m4_upper_c_total->Add(m4_upper_c_ZZ);
   m4_upper_c_total->Add(m4_upper_c_top);     
  }
  
  else {
   TH1F * m4_upper_c_total = (TH1F*) m4_upper_c_DY->Clone ("m4_upper_c_total");
   m4_upper_c_total->Add(m4_upper_c_TT);
   m4_upper_c_total->Add(m4_upper_c_WZ);
   m4_upper_c_total->Add(m4_upper_c_Wjet);
   m4_upper_c_total->Add(m4_upper_c_ZZ);
   m4_upper_c_total->Add(m4_upper_c_top);     
  }
  
  TH1F * m4_upper_a_DY   = (TH1F *) input.Get ("m4_upper_a_DY") ;
  TH1F * m4_upper_a_TT   = (TH1F *) input.Get ("m4_upper_a_TT") ;
  TH1F * m4_upper_a_WW   = (TH1F *) input.Get ("m4_upper_a_WW") ;
  TH1F * m4_upper_a_WZ   = (TH1F *) input.Get ("m4_upper_a_WZ") ;
  TH1F * m4_upper_a_Wjet = (TH1F *) input.Get ("m4_upper_a_Wjet") ;
  TH1F * m4_upper_a_ZZ   = (TH1F *) input.Get ("m4_upper_a_ZZ") ;
  TH1F * m4_upper_a_top  = (TH1F *) input.Get ("m4_upper_a_top") ;
  
  if (subtractResonant == false) {
   TH1F * m4_upper_a_total = (TH1F*) m4_upper_a_DY->Clone ("m4_upper_a_total");
   m4_upper_a_total->Add(m4_upper_a_TT);
   m4_upper_a_total->Add(m4_upper_a_WW);
   m4_upper_a_total->Add(m4_upper_a_WZ);
   m4_upper_a_total->Add(m4_upper_a_Wjet);
   m4_upper_a_total->Add(m4_upper_a_ZZ);
   m4_upper_a_total->Add(m4_upper_a_top);     
  }
  
  else {
   TH1F * m4_upper_a_total = (TH1F*) m4_upper_a_DY->Clone ("m4_upper_a_total");
   m4_upper_a_total->Add(m4_upper_a_TT);
   m4_upper_a_total->Add(m4_upper_a_WZ);
   m4_upper_a_total->Add(m4_upper_a_Wjet);
   m4_upper_a_total->Add(m4_upper_a_ZZ);
   m4_upper_a_total->Add(m4_upper_a_top);     
  }
  
  TH1F * m4_signal_DY   = (TH1F *) input.Get ("m4_signal_DY") ;
  TH1F * m4_signal_TT   = (TH1F *) input.Get ("m4_signal_TT") ;
  TH1F * m4_signal_WW   = (TH1F *) input.Get ("m4_signal_WW") ;
  TH1F * m4_signal_WZ   = (TH1F *) input.Get ("m4_signal_WZ") ;
  TH1F * m4_signal_Wjet = (TH1F *) input.Get ("m4_signal_Wjet") ;
  TH1F * m4_signal_ZZ   = (TH1F *) input.Get ("m4_signal_ZZ") ;
  TH1F * m4_signal_top  = (TH1F *) input.Get ("m4_signal_top") ;
  
  if (subtractResonant == false) {
   TH1F * m4_signal_total = (TH1F*) m4_signal_DY->Clone ("m4_signal_total");
   m4_signal_total->Add(m4_signal_TT);
   m4_signal_total->Add(m4_signal_WW);
   m4_signal_total->Add(m4_signal_WZ);
   m4_signal_total->Add(m4_signal_Wjet);
   m4_signal_total->Add(m4_signal_ZZ);
   m4_signal_total->Add(m4_signal_top);
   THStack * stack_m4_signal = (THStack *) input.Get ("stack_m4_signal") ;
  }
  
  else {
   TH1F * m4_signal_total = (TH1F*) m4_signal_DY->Clone ("m4_signal_total");
   m4_signal_total->Add(m4_signal_TT);
   m4_signal_total->Add(m4_signal_WZ);
   m4_signal_total->Add(m4_signal_Wjet);
   m4_signal_total->Add(m4_signal_ZZ);
   m4_signal_total->Add(m4_signal_top);
   THStack * stack_m4_signal = new THStack("stack_m4_signal","");
   stack_m4_signal->Add(m4_signal_DY);
   stack_m4_signal->Add(m4_signal_TT);
   stack_m4_signal->Add(m4_signal_WZ);
   stack_m4_signal->Add(m4_signal_Wjet);
   stack_m4_signal->Add(m4_signal_ZZ);
   stack_m4_signal->Add(m4_signal_top);
  }
  
  TH1F * m4_lower_DY   = (TH1F *) input.Get ("m4_lower_DY") ;
  TH1F * m4_lower_TT   = (TH1F *) input.Get ("m4_lower_TT") ;
  TH1F * m4_lower_WW   = (TH1F *) input.Get ("m4_lower_WW") ;
  TH1F * m4_lower_WZ   = (TH1F *) input.Get ("m4_lower_WZ") ;
  TH1F * m4_lower_Wjet = (TH1F *) input.Get ("m4_lower_Wjet") ;
  TH1F * m4_lower_ZZ   = (TH1F *) input.Get ("m4_lower_ZZ") ;
  TH1F * m4_lower_top  = (TH1F *) input.Get ("m4_lower_top") ;
  
  if (subtractResonant == false) {
   TH1F * m4_lower_total = (TH1F*) m4_lower_DY->Clone ("m4_lower_total");
   m4_lower_total->Add(m4_lower_TT);
   m4_lower_total->Add(m4_lower_WW);
   m4_lower_total->Add(m4_lower_WZ);
   m4_lower_total->Add(m4_lower_Wjet);
   m4_lower_total->Add(m4_lower_ZZ);
   m4_lower_total->Add(m4_lower_top);     
  }
  
  else {
   TH1F * m4_lower_total = (TH1F*) m4_lower_DY->Clone ("m4_lower_total");
   m4_lower_total->Add(m4_lower_TT);
   m4_lower_total->Add(m4_lower_WZ);
   m4_lower_total->Add(m4_lower_Wjet);
   m4_lower_total->Add(m4_lower_ZZ);
   m4_lower_total->Add(m4_lower_top);     
  }
  
  TH1F * m4_lower_c_DY   = (TH1F *) input.Get ("m4_lower_c_DY") ;
  TH1F * m4_lower_c_TT   = (TH1F *) input.Get ("m4_lower_c_TT") ;
  TH1F * m4_lower_c_WW   = (TH1F *) input.Get ("m4_lower_c_WW") ;
  TH1F * m4_lower_c_WZ   = (TH1F *) input.Get ("m4_lower_c_WZ") ;
  TH1F * m4_lower_c_Wjet = (TH1F *) input.Get ("m4_lower_c_Wjet") ;
  TH1F * m4_lower_c_ZZ   = (TH1F *) input.Get ("m4_lower_c_ZZ") ;
  TH1F * m4_lower_c_top  = (TH1F *) input.Get ("m4_lower_c_top") ;
  
  if (subtractResonant == false) {
   TH1F * m4_lower_c_total = (TH1F*) m4_lower_c_DY->Clone ("m4_lower_c_total");
   m4_lower_c_total->Add(m4_lower_c_TT);
   m4_lower_c_total->Add(m4_lower_c_WW);
   m4_lower_c_total->Add(m4_lower_c_WZ);
   m4_lower_c_total->Add(m4_lower_c_Wjet);
   m4_lower_c_total->Add(m4_lower_c_ZZ);
   m4_lower_c_total->Add(m4_lower_c_top);     
  }
  
  else {
   TH1F * m4_lower_c_total = (TH1F*) m4_lower_c_DY->Clone ("m4_lower_c_total");
   m4_lower_c_total->Add(m4_lower_c_TT);
   m4_lower_c_total->Add(m4_lower_c_WZ);
   m4_lower_c_total->Add(m4_lower_c_Wjet);
   m4_lower_c_total->Add(m4_lower_c_ZZ);
   m4_lower_c_total->Add(m4_lower_c_top);     
  }
  
  TH1F * m4_lower_a_DY   = (TH1F *) input.Get ("m4_lower_a_DY") ;
  TH1F * m4_lower_a_TT   = (TH1F *) input.Get ("m4_lower_a_TT") ;
  TH1F * m4_lower_a_WW   = (TH1F *) input.Get ("m4_lower_a_WW") ;
  TH1F * m4_lower_a_WZ   = (TH1F *) input.Get ("m4_lower_a_WZ") ;
  TH1F * m4_lower_a_Wjet = (TH1F *) input.Get ("m4_lower_a_Wjet") ;
  TH1F * m4_lower_a_ZZ   = (TH1F *) input.Get ("m4_lower_a_ZZ") ;
  TH1F * m4_lower_a_top  = (TH1F *) input.Get ("m4_lower_a_top") ;
  
  if (subtractResonant == false) {
   TH1F * m4_lower_a_total = (TH1F*) m4_lower_a_DY->Clone ("m4_lower_a_total");
   m4_lower_a_total->Add(m4_lower_a_TT);
   m4_lower_a_total->Add(m4_lower_a_WW);
   m4_lower_a_total->Add(m4_lower_a_WZ);
   m4_lower_a_total->Add(m4_lower_a_Wjet);
   m4_lower_a_total->Add(m4_lower_a_ZZ);
   m4_lower_a_total->Add(m4_lower_a_top);     
  }
  
  else {
   TH1F * m4_lower_a_total = (TH1F*) m4_lower_a_DY->Clone ("m4_lower_a_total");
   m4_lower_a_total->Add(m4_lower_a_TT);
   m4_lower_a_total->Add(m4_lower_a_WZ);
   m4_lower_a_total->Add(m4_lower_a_Wjet);
   m4_lower_a_total->Add(m4_lower_a_ZZ);
   m4_lower_a_total->Add(m4_lower_a_top);     
  }
  
  TH1F * m4_sideband_DY   = (TH1F *) input.Get ("m4_sideband_DY") ;
  TH1F * m4_sideband_TT   = (TH1F *) input.Get ("m4_sideband_TT") ;
  TH1F * m4_sideband_WW   = (TH1F *) input.Get ("m4_sideband_WW") ;
  TH1F * m4_sideband_WZ   = (TH1F *) input.Get ("m4_sideband_WZ") ;
  TH1F * m4_sideband_Wjet = (TH1F *) input.Get ("m4_sideband_Wjet") ;
  TH1F * m4_sideband_ZZ   = (TH1F *) input.Get ("m4_sideband_ZZ") ;
  TH1F * m4_sideband_top  = (TH1F *) input.Get ("m4_sideband_top") ;
  
  if (subtractResonant == false) {
   TH1F * m4_sideband_total = (TH1F*) m4_sideband_DY->Clone ("m4_sideband_total");
   m4_sideband_total->Add(m4_sideband_TT);
   m4_sideband_total->Add(m4_sideband_WW);
   m4_sideband_total->Add(m4_sideband_WZ);
   m4_sideband_total->Add(m4_sideband_Wjet);
   m4_sideband_total->Add(m4_sideband_ZZ);
   m4_sideband_total->Add(m4_sideband_top);
   THStack * stack_m4_sideband = (THStack *) input.Get ("stack_m4_sideband") ;
  }
  
  else {
   TH1F * m4_sideband_total = (TH1F*) m4_sideband_DY->Clone ("m4_sideband_total");
   m4_sideband_total->Add(m4_sideband_TT);
   m4_sideband_total->Add(m4_sideband_WZ);
   m4_sideband_total->Add(m4_sideband_Wjet);
   m4_sideband_total->Add(m4_sideband_ZZ);
   m4_sideband_total->Add(m4_sideband_top);
   THStack * stack_m4_sideband = new THStack("stack_m4_sideband","");
   stack_m4_sideband->Add(m4_sideband_DY);
   stack_m4_sideband->Add(m4_sideband_TT);
   stack_m4_sideband->Add(m4_sideband_WZ);
   stack_m4_sideband->Add(m4_sideband_Wjet);
   stack_m4_sideband->Add(m4_sideband_ZZ);
   stack_m4_sideband->Add(m4_sideband_top);
  }
  
 
  //FC get the signal samples
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
  
  //FC get the data
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
  cout << "Martijn\'s test" << endl ;
  TH1F * sidebaRegionMC = m4_upper_a_total->Clone ("sidebaRegionMC") ; 
  sidebaRegionMC->Add (m4_lower_a_total) ;
  TH1F * signalRegionMC = m4_upper_c_total->Clone ("signalRegionMC") ; 
  signalRegionMC->Add (m4_lower_c_total) ;
  
  //Subtract the WW shape from data in the sideband region
  if (subtractResonant) {  
   m4_upper_a_DATA->Add (m4_upper_a_WW, -1);   
   m4_lower_a_DATA->Add (m4_lower_a_WW, -1);   
  } 
   
  TH1F * sidebaRegion = m4_upper_a_DATA->Clone ("sidebaRegion") ; 
  sidebaRegion->Add (m4_lower_a_DATA) ;
  
  if (subtractResonant) {  
   //FC FIXME: not really rigorous; check for other solutions!!!
   for (int i = 0; i <= sidebaRegion->GetNbinsX(); i++) {
     if (sidebaRegion->GetBinContent(i) < 0) { sidebaRegion->SetBinContent(i,0.) ;  sidebaRegion->SetBinError(i,0.) ; }
   }  
   
   TH1F * signalRegionWW = m4_upper_c_WW->Clone ("signalRegionWW") ;  
   signalRegionWW->Add (m4_lower_c_WW) ;
  }  
    
  TH1F * signalRegion = m4_upper_c_DATA->Clone ("signalRegion") ; 
  signalRegion->Add (m4_lower_c_DATA) ; 
*/ 
/*  
  cout << "final analysis" << endl ;
  TH1F * sidebaRegionMC = m4_sideband_total->Clone ("sidebaRegionMC") ; 
  TH1F * signalRegionMC = m4_signal_total->Clone ("signalRegionMC") ;
  
  //Subtract the WW shape from data in the sideband region
  if (subtractResonant) m4_sideband_DATA->Add (m4_sideband_WW, -1) ;

  TH1F * sidebaRegion   = m4_sideband_DATA->Clone ("sidebaRegion") ;
  TH1F * signalRegion   = m4_signal_DATA->Clone ("signalRegion") ;
  
  if (subtractResonant) TH1F * signalRegionWW = m4_signal_WW->Clone ("signalRegionWW") ;  
*/  
/*
  cout << "final analysis closure test" << endl ;
  TH1F * sidebaRegionMC = m4_sideband_total->Clone ("sidebaRegionMC") ; 
  TH1F * signalRegionMC = m4_signal_total->Clone ("signalRegionMC") ;  
  TH1F * sidebaRegion   = sidebaRegionMC->Clone ("sidebaRegion") ; 
  TH1F * signalRegion   = signalRegionMC->Clone ("signalRegion") ; 
  if (subtractResonant) TH1F * signalRegionWW = m4_signal_WW->Clone ("signalRegionWW") ;  
*/
  
  //PG plot MC signal and background region 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
  int nBins =  m4_signal_DATA->GetNbinsX () ;
  double m4_min = m4_signal_DATA->GetXaxis ()->GetXmin () ;
  double m4_max = m4_signal_DATA->GetXaxis ()->GetXmax () ;
 
  double binSize = (m4_max - m4_min) / nBins ;
  double startFit = 200 ; //GeV, bin from where to start the num & den fit for corr factor
  double endFit = 800 ;   //GeV, bin from where to end the num & den fit for corr factor
  int fitBins = (endFit-startFit) / binSize ;

  cout << nBins << " " << m4_min << " " << m4_max << " " << binSize << endl ;
  

  TCanvas * c1 = new TCanvas () ;

  TLegend * leg_signal = makeLegendTitle (*stack_m4_signal) ;
  stack_m4_signal->Draw ("hist") ;
  leg_signal->Draw () ;
  c1->Print ("signal_stack.pdf", "pdf") ;

  TLegend * leg_sideband = makeLegendTitle (*stack_m4_sideband) ;
  stack_m4_sideband->Draw ("hist") ;
  leg_sideband->Draw () ;
  c1->Print ("sideband_stack_MC.pdf", "pdf") ;

  TH1F * sidebandMCAndData_pull = getPullPlot (sidebaRegion, sidebaRegionMC, m4_min, m4_max) ;
  gStyle->SetOptFit(1111);
  sidebandMCAndData_pull->Draw () ;
  c1->Print ("sidebandMCAndData_pull.pdf", "pdf") ;
  

  //PG correction factor from the ratio of histograms 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F * ratio_total = (TH1F *) signalRegionMC->Clone ("ratio") ;
  ratio_total->Divide (sidebaRegionMC) ;
  ratio_total->SetMarkerColor (kOrange) ;

  //PG fit separately numerator and denominator of MC 
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TF1 * numFitFunc = new TF1 ("numFitFunc", doubleExponential, 0., 1000., 4) ;
  setDoubleExpPars (numFitFunc) ;
  

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
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (num_fit_error, 0.95) ;

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
  

  fitResultPtr = sidebaRegionMC->Fit (denFitFunc, "L", "", startFit, endFit) ;
  fitStatus = (int)(fitResultPtr) ;
  loops = 0 ; 
  while (fitStatus != 0 && loops < 30)
    {
      fitResultPtr = sidebaRegionMC->Fit (denFitFunc, "L", "", startFit, endFit) ;
      fitStatus = (int)(fitResultPtr) ;
      ++loops ;
    }
  cout << "`--> " << fitStatus << " @ " << loops << "\n" ;

  TH1F * den_fit_error = new TH1F ("den_fit_error", "", fitBins, startFit, endFit) ;
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (den_fit_error, 0.95) ;

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
  h_correctionBand->Draw ("E3same") ;
  h_correctionBand->Draw ("Psame") ;
  gStyle->SetPalette (1) ;
  ratio_total->Draw ("same") ;
  leg_correctionFactor->Draw () ;
  c1->Print ("correctionFactor.pdf", "pdf") ;

  
  //PG look at the point distro wrt the band width, to determine whether the extrap. factor is ok
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

  TH1F correctionFactorPull ("correctionFactorPull", "", 50, -5, 5) ;
//  TH1F pullPlotGaus ("pullPlotGaus", "", 50, -5, 5) ; FIXME this is there only in the case of toys, commented by now, therefore
  for (int iBin = 1 ; iBin <= h_correctionBand->GetNbinsX () ; ++iBin) 
    {
//      double mean = ((TH1F *) aSlices.At (1))->GetBinContent (iBin) ;  FIXME this is there only in the case of toys, commented by now, therefore
//      double sigma = ((TH1F *) aSlices.At (2))->GetBinContent (iBin) ; FIXME this is there only in the case of toys, commented by now, therefore
//      pullPlotGaus.Fill ((num - mean) / sigma) ; FIXME this is there only in the case of toys, commented by now, therefore

      double num        = ratio_total->GetBinContent (iBin) ;
      double mean       = h_correctionBand->GetBinContent (iBin) ;
      double sigma_num  = ratio_total->GetBinError (iBin) ;
      double sigma_mean = h_correctionBand->GetBinError (iBin) ;
      
      double sigma = sqrt(sigma_num*sigma_num + sigma_mean*sigma_mean) ;
      
      correctionFactorPull.Fill ((num - mean) / sigma) ;
    }
  
  
  correctionFactorPull.Fit ("gaus","QL") ;
  c1->Print ("correctionFactor_pull.pdf", "pdf") ;
  
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
      double poolScale = correctionFactorPull.GetFunction ("gaus")->GetParameter (2) ;
      if (poolScale > 1) 
        {
          for (int iBin = 1 ; iBin <= h_correctionBand->GetNbinsX () ; ++iBin)
            {
              double newError = poolScale * h_correctionBand->GetBinError (iBin) ;
              h_correctionBand->SetBinError (iBin, newError) ;
            }
        }
    }
    
  //FC ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----   
  //FC fit the sideband shape
  TH1F * sideband_bkg = sidebaRegion->Clone ("sideband_bkg") ;
  
  TF1 * bkgFitFunc = new TF1 ("bkgFitFunc", doubleExponential, 0., 1000., 4) ;
  setDoubleExpPars (bkgFitFunc) ;
    
  TFitResultPtr fitResultPtr2 = sideband_bkg->Fit (bkgFitFunc, "L", "", startFit, endFit) ;
  fitStatus = (int)(fitResultPtr2) ;
  loops = 0 ; 
  while (fitStatus != 0 && loops < 30)
  {
    fitResultPtr2 = sideband_bkg->Fit (bkgFitFunc, "L", "", startFit, endFit) ;
    fitStatus = (int)(fitResultPtr2) ;
    ++loops ;
  }
  cout << "`--> " << fitStatus << " @ " << loops << "\n" ;
    
  TH1F * sideband_bkg_fitBand = sidebaRegion->Clone("sideband_bkg_fitBand");
  sideband_bkg_fitBand->Reset();
  (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (sideband_bkg_fitBand, 0.95) ;

  //FC set the error to this estimate of the background
  for (int iBin = 1 ; iBin <= sideband_bkg->GetNbinsX () ; ++iBin)
  {
    if (sideband_bkg->GetBinCenter(iBin) < startFit) {
      
      sideband_bkg_fitBand->SetBinContent(iBin, 0.) ;
      sideband_bkg_fitBand->SetBinError(iBin, 0.);
    }
  }
  
  c1->SetLogy () ;
  sideband_bkg->Draw ("E3") ;
  sideband_bkg_fitBand->SetFillStyle (3001) ;
  sideband_bkg_fitBand->SetFillColor (kBlue) ;
  sideband_bkg_fitBand->Draw ("E3same") ;
  c1->Print ("sidebandFittedBackground.pdf", "pdf") ;
  c1->SetLogy (0) ;
  c1->Update () ;
  c1->Print ("sidebandFittedBackground_lin.pdf", "pdf") ;

  TH1F sidebandFitPull ("sidebandFitPull", "", 50, -5, 5) ;
  int startFitBin = sideband_bkg->FindBin(startFit);
  int endFitBin   = sideband_bkg->FindBin(endFit);
  for (int iBin = startFitBin ; iBin <= endFitBin ; ++iBin) 
    {
      double num        = sideband_bkg->GetBinContent (iBin) ;
      double mean       = sideband_bkg_fitBand->GetBinContent (iBin) ;
      double sigma_num  = sideband_bkg->GetBinError (iBin) ;
      double sigma_mean = sideband_bkg_fitBand->GetBinError (iBin) ;
      
      double sigma = sqrt(sigma_num*sigma_num + sigma_mean*sigma_mean) ;
      
      sidebandFitPull.Fill ((num - mean) / sigma) ;
    }
  
  
  sidebandFitPull.Fit ("gaus","QL") ;
  c1->Print ("sidebandFit_pull.pdf", "pdf") ;
  
  
  //FC calculate the extrapolated background
  TH1F * extrapolated_bkg = sideband_bkg_fitBand->Clone ("extrapolated_bkg") ;
  extrapolated_bkg->Multiply(h_correctionBand);

  if (subtractResonant) {
    extrapolated_bkg->Add(signalRegionWW, 1);
    signalRegionMC->Add(signalRegionWW, 1);
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
  gStyle->SetOptFit(1111);
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
  
  //FC Some pull plots
  TGraphErrors g_pull_total ;
  TGraphErrors g_pull_forEveryBin ;
  
  const int nMasses = masses.size();
  char hname[100];
  
  TH1F *gPullHisto_forMassWin [nMasses] ;
  TH1F *gPullHisto_forEveryBin = new TH1F("gPullHisto_forEveryBin","gPullHisto_forEveryBin",50,-5,5);
  
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

  
  double bkg_count_min = 99999999.;  //FC Needed for giving a fancier axis range, see later... 

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

      if (bkg_count < bkg_count_min) bkg_count_min = bkg_count;
      
      double expSignal = m4_signal_total_SIG->Integral (minBin, maxBin) ;

      cout << "MH | " << masses.at (i) 
           << " : (" << min
           << "," << max
           << ") : " << total
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
      
      //FC --- --- ---
      double error = sqrt(total + bkg_count_error*bkg_count_error) ;
      double pull = (total - bkg_count)/ error ;
      g_pull_total.SetPoint(i, masses.at (i), pull) ;
      g_pull_total.SetPointError(i, 50., 0.) ;
    
      sprintf(hname,"pull_mass_%d",masses.at (i));
      gPullHisto_forMassWin [i] = new TH1F(hname, hname,30,-5,5);
      for (int iBin = minBinBkg; iBin <= maxBinBkg; iBin++) {

       double error = sqrt(signalRegion->GetBinContent(iBin) + extrapolated_bkg->GetBinError(iBin)*extrapolated_bkg->GetBinError(iBin));
       pull = (signalRegion->GetBinContent(iBin) - extrapolated_bkg->GetBinContent(iBin))/error;
       
       gPullHisto_forMassWin [i]->Fill(pull);
      }
      
    } //PG loop on the mass points


  //FC loop on the mass points
  int index = 0;
  int startFitBin = extrapolated_bkg->FindBin(startFit);
  int endFitBin   = extrapolated_bkg->FindBin(endFit);
  for (int i = startFitBin; i <= endFitBin ; i++)
    {
      //FC skip the entries below mMin or above mMax 
      if (signalRegion->GetBinCenter(i) < mMin.at(0) || 
	  signalRegion->GetBinCenter(i) > mMax.at(mMax.size()-1)) continue;

      double obs = signalRegion->GetBinContent(i) ;
      double exp = extrapolated_bkg->GetBinContent(i) ;
      double sigma_obs = sqrt(obs) ;
      double sigma_exp = extrapolated_bkg->GetBinError(i) ;
      
      double error = sqrt(sigma_obs*sigma_obs + sigma_exp*sigma_exp);     
      double pull = (obs - exp)/error;
      g_pull_forEveryBin.SetPoint(index,signalRegion->GetBinCenter(i),pull);
      g_pull_forEveryBin.SetPointError(index,binSize/2.,0.);
      gPullHisto_forEveryBin->Fill(pull);
      
      /*
      double pull = (obs - exp)/exp ;
      double err  = 1./pow(exp,2) * pow(sigma_obs,2) + pow(obs,2)/pow(exp,4) * pow(sigma_exp,2) ;
      err = err * pow(pull,2) ;
      err = sqrt (err) ;
      
      g_pull.SetPoint (index,signalRegion->GetBinCenter(i),pull) ;
      g_pull.SetPointError (index,binSize/2,err) ;           
      
      double err2  = sqrt(sigma_obs*sigma_obs + sigma_exp*sigma_exp) ;
      double pull2 = (obs - exp)/err2 ;
      
      g_pull2.SetPoint (index,signalRegion->GetBinCenter(i),pull) ;
      g_pull2.SetPointError (index,binSize/2,0.) ;
      
      gPullHisto->Fill(pull2) ;
      */
      
      index++ ;
    }
    
    
  g_total.SetMarkerStyle (6) ;
  g_background_count.SetLineColor (kOrange) ;
  g_background_count.SetFillColor (kOrange) ;

  TLegend * leg_results = new TLegend (0.5, 0.8, 0.95, 0.95, NULL, "brNDC") ;
  legendMaquillage (leg_results) ;
  leg_results->AddEntry (&g_total, "measured events in signal window", "p") ;
  leg_results->AddEntry (&g_background_count, "expected bkg in signal window", "f") ;
  
  c1->SetLogy () ;
  g_background_count.SetMinimum(bkg_count_min - 300);  //FC For a fancier logarithmic Y-axis
  g_background_count.Draw ("AE3") ;
  g_total.Draw ("EPsame") ;
  leg_results->Draw () ;
  c1->Print ("results.pdf", "pdf") ;
  c1->SetLogy (0) ;

  c1->DrawFrame (200, 0, 700, 200) ;
  g_error.SetLineWidth (2) ;
  g_error.Draw ("L") ;
  c1->Print ("systematic_and_statistics.pdf", "pdf") ;
  
  
  g_pull_total.SetMarkerStyle(21);
  g_pull_total.SetMarkerSize(0.9);
  g_pull_total.GetYaxis()->SetTitle("(total - exp bkg)/err");
  g_pull_total.Draw("AP");
  c1->Print ("resultsPull.pdf", "pdf") ;
  
  g_pull_forEveryBin.SetMarkerStyle(1);
  g_pull_forEveryBin.GetYaxis()->SetTitle("(total - exp bkg)/err");
  g_pull_forEveryBin.GetYaxis()->SetRangeUser(-3,3);
  g_pull_forEveryBin.Draw("AP");
  c1->Print ("resultsPull_forEveryBin.pdf", "pdf") ;
  
  gPullHisto_forEveryBin->Draw();
  gPullHisto_forEveryBin->Fit("gaus","QL");
  c1->Print ("resultsPull_forEveryBin_histo.pdf", "pdf") ;
  
//   c1->SetGridx() ;
//   c1->SetGridy() ;
//   g_pull.SetMarkerStyle (1) ;
//   g_pull.SetMarkerColor (kOrange) ;
//   g_pull.Draw("AP") ;
//   c1->Print("resultPull.pdf", "pdf") ;
 
  
  TFile output ("output_005.root", "recreate") ;
  output.cd () ;
  
  sidebaRegionMC->Write();
  sidebaRegion->Write();
  signalRegion->Write();
  signalRegionMC->Write();
  if (subtractResonant) signalRegionWW->Write(); 
  
  ratio_total->Write();
  h_correctionBand->Write();
  correctionFactorPull.Write();
  
  sideband_bkg->Write();
  sideband_bkg_fitBand->Write();
  sidebandFitPull.Write();
  extrapolated_bkg->Write() ;
  
  g_total.Write ("g_total") ;
  g_background_count.Write ("g_background_count") ;
  g_error.Write ("g_error") ;
  
  g_pull_total.Write("g_pull_total");
  g_pull_forEveryBin.Write("g_pull_forEveryBin");
  gPullHisto_forEveryBin->Write();
  
  for (int i = 0; i<nMasses; i++)  gPullHisto_forMassWin [i]->Write();
  
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
