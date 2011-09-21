#include <vector>
#include <sstream>

int macro_004_3 ()
{
  TFile input ("testBkg_004.root") ;

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
  std::vector<TH1F *> comparisons ;

  //PG loop on alpha   
  for (double alpha = 0. ; alpha < 1.01 ; alpha += 0.1)
    {
      //PG signal = beta (alpha * lower + (1-alpha) * upper)
      
      TH1F * combination = (TH1F *) m4_lower_total->Clone ("combination") ;
      combination->Reset () ;
      combination->Sumw2 () ;
      combination->Add (m4_lower_total, alpha) ;
      combination->Add (m4_upper_total, 1. - alpha) ;

      TH1F * scaled_extrap = (TH1F *) combination->Clone ("scaled_extrap") ;
      scaled_extrap->Scale (1./scaled_extrap->Integral ()) ;

      TH1F * scaled_signal = (TH1F *) m4_signal_total->Clone ("scaled_signal") ;
      scaled_signal->Scale (1. / scaled_signal->Integral ()) ;

      double chi2 = -1. ; 
      double Ndof = scaled_extrap->GetNbinsX () ;
      int igood = 0 ;
      vector<double> residuals (scaled_extrap.GetNbinsX (), 0.) ;
      chi2 = scaled_signal->Chi2Test (scaled_extrap, "WWCHI2/NDF", &residuals[0]) ;
//      m4_signal_Wjet->Chi2TestX (scaled, chi2, Ndof, igood, "WW", &residuals[0]) ;
      v_chi2.push_back (chi2) ;
      v_alpha.push_back (alpha) ;


      stringstream name ;
      name << "sig_o_side_" << alpha ;
      
//      TString name = "sig_o_side" ;
//      name += alpha ; 
      TH1F * sig_o_side = (TH1F *) scaled_signal->Clone (name.str ().c_str ()) ;
      sig_o_side->Divide (scaled_extrap) ;
      sig_o_side->SetFillColor (0) ;
      comparisons.push_back (sig_o_side) ;
    
    } //PG loop on alpha

  TCanvas c1 ;

  TGraph g_chi2 (v_alpha.size (), &v_alpha[0], &v_chi2[0]) ;
  g_chi2.Draw ("AL*") ;
  c1.Print ("chi2Trend.pdf", "pdf") ;

  TLegend * leg_ratios = new TLegend (0.5, 0.5, 0.9, 0.95, NULL, "brNDC") ;
  leg_ratios->SetBorderSize (0) ;
  leg_ratios->SetTextFont (42) ;
  leg_ratios->SetTextSize (0.04) ;
  leg_ratios->SetLineColor (1) ;
  leg_ratios->SetLineStyle (1) ;
  leg_ratios->SetLineWidth (1) ;
  leg_ratios->SetFillColor (0) ;
  leg_ratios->SetFillStyle (0) ;

  c1.DrawFrame (100., 0., 1000., 5) ;
  for (unsigned int i = 0 ; i < comparisons.size () ; ++i) 
    {
      comparisons.at (i)->SetLineColor (50 + 2 * i) ;
      leg_ratios->AddEntry (comparisons.at (i), comparisons.at (i)->GetName (), "lp") ;
      comparisons.at (i)->Draw ("histsame") ;
    }
  leg_ratios->Draw () ;  

  c1.Print ("sig_o_side_vs_alpha.pdf", "pdf") ;


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