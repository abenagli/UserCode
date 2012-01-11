#include "interface/Functions.h"


void setAttenuatedDoubleExpPars (TF1 * func)
{
  func->FixParameter (0, 190) ;   //PG NB it's fixed
  func->FixParameter (1, 55) ;    //PG NB it's fixed
  func->SetParameter (2, 12.05) ;
  func->SetParameter (3, 0.023) ;
  func->SetParameter (4, 9.219) ;
  func->SetParameter (5, 0.0109) ;
  
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


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


double fitFunc(double* x, double* par)
{
  // variable
  double xx = x[0];
  // ranges
  double xMin1 = par[0];
  double xMax1 = par[1];
  double xMin2 = par[2];
  double xMax2 = par[3];
  //std::cout << "x: " << xx << "   xMin1: " << xMin1 <<  "   xMax1: " << xMax1 << "   xMin2: " << xMin2 <<  "   xMax2: " << xMax2 << std::endl; 
  
  // fit function ranges
  if( xx < xMin1 )                    TF1::RejectPoint();
  if( (xx >= xMax1) && (xx < xMin2) ) TF1::RejectPoint();
  if( xx >= xMax2 )                   TF1::RejectPoint();  
  
  return attenuatedDoubleExponential(x,&par[4]);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int macro () 
{
  TFile * f = new TFile ("testBkg_017_S350_2011AB.root") ;
  TH1F * m4_sideband_DATA = (TH1F *) f->Get ("m4_sideband_DATA") ;
  
  double startFit = 180. ;
  double endFit   = 800. ;

  double lepNuWMMIN = 310 ;
  double lepNuWMMAX = 390 ;
  
  int nBins = 200;
  float xMin = 0.;
  float xMax = 1000.;
  float xWidth = (xMax-xMin)/nBins;
  
  int nBinsFit = int((endFit-startFit)/xWidth);
  
  int binMin = -1;
  int binMax = -1;
  int binFitMin1 = -1;
  int binFitMax1 = -1;
  int binFitMin2 = -1;
  int binFitMax2 = -1;
  for(int bin = 1; bin <= nBins; ++bin)
  {
    float binCenter = xMin + 0.5*xWidth + xWidth*(bin-1);
    
    if( (binCenter >= lepNuWMMIN) && (binMin == -1) ) binMin = bin;
    if( (binCenter >= lepNuWMMIN) && (binCenter < lepNuWMMAX) ) binMax = bin;
    if( (binCenter >= startFit) && (binFitMin1 == -1) ) binFitMin1 = bin;
    if( (binCenter >= startFit) && (binCenter < lepNuWMMAX) ) binFitMax1 = bin;
    if( (binCenter >= lepNuWMMIN) && (binFitMin2 == -1) ) binFitMin2 = bin;
    if( (binCenter >= lepNuWMMIN) && (binCenter < endFit) ) binFitMax2 = bin;
  }
  //std::cout << "binMin: " << binMin << "   binMax: " << binMax << std::endl;
  
  TCanvas * c1 = new TCanvas () ;
  m4_sideband_DATA->SetMarkerStyle (4) ;
  m4_sideband_DATA->Draw () ;

  TF1 * fullShape = new TF1 ("fullShape", attenuatedDoubleExponential, 0., 800., 6) ;
  setAttenuatedDoubleExpPars (fullShape) ;        
  TFitResultPtr fullFitResult = m4_sideband_DATA->Fit ("fullShape", "L+", "", startFit, endFit) ;


  TH1F* hint_full = new TH1F ("hint_full", "", nBins ,xMin, xMax) ;
  (TVirtualFitter::GetFitter()) -> GetConfidenceIntervals (hint_full, 0.68) ;
  
//    hint -> SetStats(kFALSE);
//    hint -> SetMarkerSize(0);
//    hint -> SetFillColor(kRed);
//    hint -> SetFillStyle(3002);

  TF1 * shapeWithHole = new TF1 ("shapeWithHole", fitFunc, 0., 800., 10) ;
  shapeWithHole->FixParameter (0, startFit) ;
  shapeWithHole->FixParameter (1, lepNuWMMIN) ;
  shapeWithHole->FixParameter (2, lepNuWMMAX) ;
  shapeWithHole->FixParameter (3, endFit) ;
  shapeWithHole->FixParameter (4, 190) ;   //PG NB it's fixed
  shapeWithHole->FixParameter (5, 55) ;    //PG NB it's fixed
  shapeWithHole->SetParameter (6, 12.05) ;
  shapeWithHole->SetParameter (7, 0.023) ;
  shapeWithHole->SetParameter (8, 9.219) ;
  shapeWithHole->SetParameter (9, 0.0109) ;
  shapeWithHole->SetLineWidth (1) ;
  shapeWithHole->SetLineColor (kRed+2) ;
  shapeWithHole->SetNpx (10000) ;

  TFitResultPtr holeFitResult = m4_sideband_DATA->Fit (shapeWithHole, "L+", "", startFit, endFit) ;
  TH1F* hint_hole = new TH1F ("hint_hole", "", nBins ,xMin, xMax) ;
  (TVirtualFitter::GetFitter()) -> GetConfidenceIntervals (hint_hole, 0.68) ;

  for (int i = 0 ; i < 6 ; ++i)
    {
      cout << fullShape->GetParName (i) << " : " 
           << (fullShape->GetParameter (i) - shapeWithHole->GetParameter (i+4)) / (fullShape->GetParameter (i) + shapeWithHole->GetParameter (i+4)) 
           << endl ;
    }

  int N_observed = m4_sideband_DATA->Integral (binMin,binMax) ;
  double N_estimated_hole = shapeWithHole->Integral(lepNuWMMIN, lepNuWMMAX) / xWidth;
  double N_estimated_err_hole = 0. ;
  for(int bin = 1; bin <= hint_hole->GetNbinsX(); ++bin)
    {
      float thisX = hint_hole -> GetBinCenter(bin);
      if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
        N_estimated_err_hole += hint_hole -> GetBinError(bin);
    }

  double N_estimated_full = fullShape->Integral(lepNuWMMIN, lepNuWMMAX) / xWidth;
  double N_estimated_err_full = 0. ;
  for(int bin = 1; bin <= hint_full->GetNbinsX(); ++bin)
    {
      float thisX = hint_full -> GetBinCenter(bin);
      if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
        N_estimated_err_full += hint_full -> GetBinError(bin);
    }

  cout << "observed : " << N_observed << "\n"
       << "--> estimated full fit : " << N_estimated_full << " +- " << N_estimated_err_full << "\n"
       << "--> estimated hole fit : " << N_estimated_hole << " +- " << N_estimated_err_hole
       << endl ;
       
  //PG singal injection
  //PG -----------------
  
  TH1F * m4_signal_DATA = (TH1F *) f->Get ("m4_sideband_DATA") ;
  
  TH1F * m4_signal_SIG_ggH350 = (TH1F *) f->Get ("m4_signal_SIG_ggH350") ;
  TH1F * m4_signal_SIG_qqH350 = (TH1F *) f->Get ("m4_signal_SIG_qqH350") ;
  TH1F * signal = m4_signal_SIG_ggH350->Clone ("signal") ;
  signal->Add (m4_signal_SIG_qqH350) ;
  double expected = signal->Integral () ;

  TH1F * h_injection = new TH1F ("h_injection", "h_injection", 100, N_estimated_full - 5 * N_estimated_err_full, N_estimated_full + 5 * N_estimated_err_full) ;
  //PG loop over toys 
  for (int i = 0 ; i < 100 ; ++i)
    { 
      TH1F * injection = signal->Clone ("injection") ;
      injection->Reset () ;
      injection->FillRandom (signal, (int) expected) ;
      TH1F * injected = m4_sideband_DATA->Clone ("injected") ;
      injected->Add (injection) ;
    
      TF1 * shapeWithHole_inj = new TF1 ("shapeWithHole_inj", fitFunc, 0., 800., 10) ;
      shapeWithHole_inj->FixParameter (0, startFit) ;
      shapeWithHole_inj->FixParameter (1, lepNuWMMIN) ;
      shapeWithHole_inj->FixParameter (2, lepNuWMMAX) ;
      shapeWithHole_inj->FixParameter (3, endFit) ;
      shapeWithHole_inj->FixParameter (4, 190) ;   //PG NB it's fixed
      shapeWithHole_inj->FixParameter (5, 55) ;    //PG NB it's fixed
      shapeWithHole_inj->SetParameter (6, 12.05) ;
      shapeWithHole_inj->SetParameter (7, 0.023) ;
      shapeWithHole_inj->SetParameter (8, 9.219) ;
      shapeWithHole_inj->SetParameter (9, 0.0109) ;
      shapeWithHole_inj->SetLineWidth (1) ;
      shapeWithHole_inj->SetLineColor (kGreen+2) ;
      shapeWithHole_inj->SetNpx (10000) ;
    
      TFitResultPtr hole_injFitResult = injected->Fit (shapeWithHole_inj, "L+", "", startFit, endFit) ;
      TH1F* hint_hole_inj = new TH1F ("hint_hole_inj", "", nBins ,xMin, xMax) ;
      (TVirtualFitter::GetFitter()) -> GetConfidenceIntervals (hint_hole_inj, 0.68) ;
      
      double N_estimated_hole_inj = shapeWithHole_inj->Integral(lepNuWMMIN, lepNuWMMAX) / xWidth;
      double N_estimated_err_hole_inj = 0. ;
      for(int bin = 1; bin <= hint_hole_inj->GetNbinsX(); ++bin)
        {
          float thisX = hint_hole_inj -> GetBinCenter(bin);
          if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
            N_estimated_err_hole_inj += hint_hole_inj -> GetBinError(bin);
        }
      cout << "--> estimated hole fit inj : " << N_estimated_hole_inj << " +- " << N_estimated_err_hole_inj << endl ;
      h_injection->Fill (N_estimated_hole_inj) ;
      delete injection ;
      delete shapeWithHole_inj ;
      delete hint_hole_inj ;
    } //PG loop over toys  

  TCanvas * c3 = new TCanvas () ;
  h_injection->Draw () ;


  // calcola gli errori, e l'integrale, confronta, fai il loop con tanti toy
  
// TH1::FillRandom(TH1 *) or TH1::FillRandom(const char *tf1name)
  
  

}