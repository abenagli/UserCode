TH1F * background ;

Double_t ftotal (Double_t * x, Double_t * par) 
{
   Double_t xx = x[0];
   Int_t bin = background->GetXaxis()->FindBin(xx);
   Double_t br = par[0]*background->GetBinContent(bin);
   return br ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int macro_004 ()
{
  TFile input ("../testBkg_004.root") ;

  //PG get the histograms
  //PG ---- ---- ---- ---- ---- ---- ----

  THStack * stack_m4_upper = (THStack *) input.Get ("stack_m4_upper") ;
  TH1F * m4_upper_total = (TH1F*) stack_m4_upper->GetStack ()->Last () ;
  TH1F * m4_upper_Wjet = (TH1F *) input.Get ("m4_upper_Wjet") ;
    
  THStack * stack_m4_lower = (THStack *) input.Get ("stack_m4_lower") ;
  TH1F * m4_lower_total = (TH1F *) stack_m4_lower->GetStack ()->Last () ;
  TH1F * m4_lower_Wjet = (TH1F *) input.Get ("m4_lower_Wjet") ;
    
  THStack * stack_m4_signal = (THStack *) input.Get ("stack_m4_signal") ;
  TH1F * m4_signal_total = (TH1F *) stack_m4_signal->GetStack ()->Last () ;
  TH1F * m4_signal_Wjet = (TH1F *) input.Get ("m4_signal_Wjet") ;

  TCanvas c1 ;
  
  vector<double> v_chi2 ;
  vector<double> v_alpha ;
  int index = 0 ;
  //PG loop on alpha   
  for (double alpha = 0. ; alpha < 1.01 ; alpha += 0.05)
    {
      //PG signal = beta (alpha * lower + (1-alpha) * upper)
      
      TH1F * combination = (TH1F *) m4_lower_Wjet->Clone ("combination") ;
      combination->Reset () ;
      combination->Sumw2 () ;
      combination->Add (m4_lower_Wjet, alpha) ;
      combination->Add (m4_upper_Wjet, 1. - alpha) ;

      background = combination ;  
      TF1 * f_combination = new TF1 ("f_combination", ftotal, 0., 1000., 1) ;
      m4_signal_Wjet->Fit ("f_combination","b", "", 200., 600.) ;   //PG limiti stretti
      double beta = f_combination->GetParameter (0) ;
      cout << "alpha: " << alpha
           << " beta: " << beta << "\n" ;
      
      TH1F * scaled = (TH1F *) combination.Clone ("scaled") ;
      scaled->Scale (beta) ;
      
      double chi2 = -1. ; 
      double Ndof = scaled.GetNbinsX () ;
      int igood = 0 ;
      vector<double> residuals (scaled.GetNbinsX (), 0.) ;
      chi2 = m4_signal_Wjet->Chi2Test (scaled, "WWCHI2/NDF", &residuals[0]) ;
//      m4_signal_Wjet->Chi2TestX (scaled, chi2, Ndof, igood, "WW", &residuals[0]) ;
      v_chi2.push_back (chi2) ;
      v_alpha.push_back (alpha) ;
      
      c1.cd () ;
      m4_signal_Wjet->Draw ("hist") ;
      f_combination->Draw ("same") ;

      TString outputName = "fit_" ;
      outputName += index++ ;
      outputName += ".pdf" ;
      c1.Print (outputName, "pdf") ;
    
    } //PG loop on alpha

  TGraph g_chi2 (v_alpha.size (), &v_alpha[0], &v_chi2[0]) ;
  g_chi2.Draw ("AL*") ;
  c1.Print ("chi2Trend.pdf", "pdf") ;


  m4_lower_Wjet-> SetFillStyle (0) ;
  m4_signal_Wjet->SetFillStyle (0) ;
  m4_upper_Wjet-> SetFillStyle (0) ;

  m4_lower_Wjet-> SetLineStyle (2) ;
  m4_signal_Wjet->SetLineStyle (1) ;
  m4_upper_Wjet-> SetLineStyle (3) ;

  m4_lower_Wjet-> SetLineColor (kBlue+2) ;
  m4_signal_Wjet->SetLineColor (kBlue+2) ;
  m4_upper_Wjet-> SetLineColor (kBlue+2) ;

  m4_upper_Wjet-> DrawNormalized ("hist") ;
  m4_signal_Wjet->DrawNormalized ("histsame") ;
  m4_lower_Wjet-> DrawNormalized ("histsame") ;

  c1.Print ("flatCompare.pdf", "pdf") ;

}


/*
TH1F * background ;

Double_t ftotal (Double_t * x, Double_t * par) 
{
   Double_t xx = x[0];
   Int_t bin = background->GetXaxis()->FindBin(xx);
   Double_t br = par[0]*background->GetBinContent(bin);
   return br ;
}

  TF1 * f_DY = new TF1 ("f_DY", ftotal, 0, 200, 1) ;
  f_DY->SetParameter (0, 1.) ;
  h_data_Mll->Fit ("f_DY","b", "", 10, 200) ;   
  cout << "param 10-200: " << f_DY->GetParameter (0) << "\n" ;
  TCanvas c1 ;
  c1.SetLogy () ;
  h_data_Mll->Draw () ;
  f_DY->Draw ("same") ;
  c1.Print ("fitted_10-200.pdf","pdf") ;

  h_data_Mll->Fit ("f_DY","b", "", 10, 20) ;   
  cout << "param 10-20: " << f_DY->GetParameter (0) << "\n" ;
  h_data_Mll->Draw () ;
  f_DY->Draw ("same") ;
  c1.Print ("fitted_10-20.pdf","pdf") ;
  h_data_Mll->Fit ("f_DY","b", "", 70, 110) ;   

  cout << "param 70-110: " << f_DY->GetParameter (0) << "\n" ;
  h_data_Mll->Draw () ;
  f_DY->Draw ("same") ;
  c1.Print ("fitted_70-110.pdf","pdf") ;
  h_data_Mll->Fit ("f_DY","b", "", 70, 110) ;   
*/



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