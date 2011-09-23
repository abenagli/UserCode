#include <vector>

int macro_008 ()
{
  std::vector<TH1F *> comparisons ;
  TFile * input = new TFile ("testBkg_008.root") ;
  for (double Mjj_width = 10. ; Mjj_width < 25. ; Mjj_width += 1.)
    {
      stringstream s_RatioName ;
      s_RatioName << "sig_o_side_" << Mjj_width ;
      TH1F * sig_o_side = (TH1F *) input->Get (s_RatioName.str ().c_str ()) ;
      sig_o_side->SetFillColor (0) ;
      sig_o_side->SetLineWidth (2) ;
      comparisons.push_back (sig_o_side) ;
    }

  TLegend * leg_ratios = new TLegend (0.5, 0.6, 0.9, 0.9, NULL, "brNDC") ;
  leg_ratios->SetBorderSize (0) ;
  leg_ratios->SetTextFont (42) ;
  leg_ratios->SetTextSize (0.04) ;
  leg_ratios->SetLineColor (1) ;
  leg_ratios->SetLineStyle (1) ;
  leg_ratios->SetLineWidth (1) ;
  leg_ratios->SetFillColor (0) ;
  leg_ratios->SetFillStyle (0) ;

  TCanvas * c1 = new TCanvas ;
  c1->DrawFrame (100., 0., 800., 2) ;
  for (unsigned int i = 0 ; i < comparisons.size () ; ++i) 
    {
      comparisons.at (i)->SetLineColor (25 + 2 * i) ;
      leg_ratios->AddEntry (comparisons.at (i), comparisons.at (i)->GetName (), "lp") ;
      comparisons.at (i)->Draw ("histsame") ;
    }
  leg_ratios->Draw () ;  
}