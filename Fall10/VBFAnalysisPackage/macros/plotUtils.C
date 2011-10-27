#include <map>


TLegend * makeLegend (THStack stack)
{
  TLegend * leg = new TLegend (0.6, 0.5, 0.95, 0.95, NULL, "brNDC") ;
  leg->SetBorderSize (0) ;
  leg->SetTextFont (42) ;
  leg->SetTextSize (0.04) ;
  leg->SetLineColor (1) ;
  leg->SetLineStyle (1) ;
  leg->SetLineWidth (1) ;
  leg->SetFillColor (0) ;
  leg->SetFillStyle (0) ;

  TList * pippo = stack.GetHists () ;
  TIter next (pippo) ;
  TObject * obj ;
  while ((obj = next ()))
    {
      leg->AddEntry (obj, obj->GetName (),"f") ;
    }
  return leg ;

}


// --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + 


TLegend * makeLegendTitle (THStack stack)
{
  TLegend * leg = new TLegend (0.6, 0.5, 0.95, 0.95, NULL, "brNDC") ;
  leg->SetBorderSize (0) ;
  leg->SetTextFont (42) ;
  leg->SetTextSize (0.04) ;
  leg->SetLineColor (1) ;
  leg->SetLineStyle (1) ;
  leg->SetLineWidth (1) ;
  leg->SetFillColor (0) ;
  leg->SetFillStyle (0) ;

  TList * pippo = stack.GetHists () ;
  TIter next (pippo) ;
  TObject * obj ;
  while ((obj = next ()))
    {
      leg->AddEntry (obj, obj->GetTitle (),"f") ;
    }
  return leg ;

}


// --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + 


pair<TGraphErrors *, TGraphErrors *> 
getPullTrend  (TH1F * hDATA, TH1F * hMC)
{
  int nbin = hDATA->GetNbinsX () ;
  double max = hDATA->GetXaxis ()->GetXmax () ;
  double min = hDATA->GetXaxis ()->GetXmin () ;
  std::string name1 = hDATA->GetName () ;
  std::string name2 = hMC->GetName () ; 
  std::string nameNew = name1 + "_o_" + name2 ; 
  std::string yaxisTitle = name1 + "/" + name2 ; 
  
  TGraphErrors * grPull = new TGraphErrors () ;   //PG the Pull plot
  TGraphErrors * grPullMC = new TGraphErrors () ; //PG the error band
  int point = 0 ; 
  for  (int iBin = 0 ; iBin<nbin ; iBin++)
    {
      double X = hDATA->GetBinCenter (iBin+1) ;
      double DATA = hDATA->GetBinContent (iBin+1) ;
      int binMC = hMC->GetXaxis ()->FindBin (X) ;
      double MC = hMC->GetBinContent (binMC) ;
      double errMC = hMC->GetBinError (binMC) ;
      
      grPull->SetPoint       (point, X,  (MC ? DATA/MC : 0)) ;
      grPull->SetPointError  (point, 0,  (MC ? sqrt (DATA)/MC : 0)) ;
      
      grPullMC->SetPoint       (point, X, 1.) ;
      grPullMC->SetPointError  (point, 0,  (MC ? errMC / MC : 0)) ;
      point++ ;
    }
  grPull->SetLineColor (kBlue+2) ;
  grPull->SetLineWidth (1) ;
  grPull->SetMarkerColor (kBlue+2) ;
  grPull->SetMarkerStyle (20) ;
  grPull->SetMarkerSize (0.5) ;
  grPull->GetXaxis ()->SetTitle (hMC->GetXaxis ()->GetTitle ()) ; 
  grPull->GetYaxis ()->SetTitle (yaxisTitle.c_str ()) ; 
  grPull->GetYaxis ()->SetRangeUser (0.,3.) ; 
  
  grPullMC->SetLineWidth (0) ;
  grPullMC->SetMarkerSize (0) ;
  grPullMC->SetFillColor (kOrange) ;
  grPullMC->GetXaxis ()->SetTitle (hMC->GetXaxis ()->GetTitle ()) ; 
  grPullMC->GetYaxis ()->SetTitle (yaxisTitle.c_str ()) ; 
  grPullMC->GetYaxis ()->SetRangeUser (0.,3.) ; 
  
  return std::pair<TGraphErrors *, TGraphErrors *>  (grPull, grPullMC) ; 
}


// --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + 


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


// --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + 


TH1F *
getPullPlot  (TH1F * hDATA, TH1F * hMC, double min, double max)
{
  int nbin = 0.3 * hDATA->GetNbinsX () + 3 ;
  std::string name1 = hDATA->GetName () ;
  std::string name2 = hMC->GetName () ; 
  std::string nameNew = "p_" + name1 + "_o_" + name2 ; 
  
  TH1F * pull = new TH1F (nameNew.c_str (), "", nbin, -3, 3) ;

  for  (int iBin = 0 ; iBin < hDATA->GetNbinsX () ; iBin++)
    {    	
      double X = hDATA->GetBinCenter (iBin+1) ;
      if (X < min || X > max) continue ;
      double DATA = hDATA->GetBinContent (iBin+1) ;
      int binMC = hMC->GetXaxis ()->FindBin (X) ;
      double MC = hMC->GetBinContent (binMC) ;
      double errMC = hMC->GetBinError (binMC) ;
      pull->Fill ((DATA - MC) / sqrt (errMC * errMC + DATA)) ;
    }
  pull->Fit ("gaus", "QL") ;
  pull->SetFillColor (kGreen + 2) ;
  pull->SetFillStyle (3001) ;
  return pull ;
}


// --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + 


TH1F * PullPlot (TH1F* hDATA, TH1F* hMC, double min, double max)
  {
     std::string nameNew = "pp_" ;
     nameNew += hDATA->GetName() ;
     nameNew += "_" ;
     nameNew += hMC->GetName() ; 
     TH1F * hPool = (TH1F *) hDATA->Clone (nameNew.c_str ()) ;
     for (int iBin = 0 ; iBin < hDATA->GetNbinsX () ; ++iBin)
       {
         double A = hDATA->GetBinContent (iBin) ;
         double B = hMC->GetBinContent (iBin) ;

         double e_A = hDATA->GetBinError (iBin) ;
         double e_B = hMC->GetBinError (iBin) ;

         if  (A+B != 0) 
           {
             hPool->SetBinContent (iBin, (A-B)/ (A+B)*2.) ;
//             hPool->SetBinError (iBin,4. * A /  (A+B) /  (A+B) * sqrt (A+B)) ;
/*
             e_A * d/dA (0.5 * (A-B)/(A+B)) =
             e_A * 0.5 * ( d/d(A-B) (A-B)/(A+B) * d/dA (A-B) +
                           d/d(A+B) (A-B)/(A+B) * d/dA (A+B) ) = 
*/                       
             double d_A = e_A * 0.5 * ( 1./(A+B) - (A-B)/( (A+B)*(A+B) ) ) ;                            
             double d_B = e_B * 0.5 * ( -1./(A+B) + (A-B)/( (A+B)*(A+B) ) ) ;
             double d_tot = sqrt (d_A * d_A + d_B * d_B) ;                     
             hPool->SetBinError (iBin, d_tot) ;
           }
       }

   hPool->SetLineColor (kBlack) ;
   hPool->SetLineWidth (1) ;
   hPool->SetMarkerColor (kBlack) ;
   hPool->SetMarkerStyle (20) ;
   hPool->SetMarkerSize (1) ;
   hPool->GetXaxis ()->SetTitle (hMC->GetXaxis ()->GetTitle ()) ; 
   hPool->GetYaxis ()->SetTitle ("2 (DATA-MC)/ (DATA+MC)") ; 
   hPool->GetYaxis ()->SetRangeUser (min, max) ; 
   return hPool;
  }


// --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + 


TH1F * PullPlotAsRatio (TH1F* hDATA, TH1F* hMC, double min, double max)
  {
    std::string nameNew = "ppR_" ;
    nameNew += hDATA->GetName() ;
    nameNew += "_" ;
    nameNew += hMC->GetName() ; 
    TH1F * hPool = (TH1F *) hMC->Clone (nameNew.c_str ()) ;
    for (int iBin = 0 ; iBin < hDATA->GetNbinsX () ; ++iBin)
       {
         double A = hDATA->GetBinContent (iBin) ;
         double B = hMC->GetBinContent (iBin) ;

         double e_A = hDATA->GetBinError (iBin) ;
         double e_B = hMC->GetBinError (iBin) ;

         if  (B != 0) 
           {
             hPool->SetBinContent (iBin, A/B) ;
//             hPool->SetBinError (iBin,4. * A /  (A+B) /  (A+B) * sqrt (A+B)) ;
/*
             f = A / B
             d_A = e_A * df/dA
             d_B = e_B * df/dB = e_B * (-A/B^2) 
*/                       
             double d_A = e_A / B ;
             double d_B = e_B * A / (B * B) ;
             double d_tot = sqrt (d_A * d_A + d_B * d_B) ;                     
             hPool->SetBinError (iBin, d_tot) ;
           }
       }
    hPool->SetLineColor (kBlack) ;
    hPool->SetLineWidth (1) ;
    hPool->SetMarkerColor (kBlack) ;
    hPool->SetMarkerStyle (20) ;
    hPool->SetMarkerSize (1) ;
    hPool->GetXaxis ()->SetTitle (hMC->GetXaxis ()->GetTitle ()) ; 
    hPool->GetYaxis ()->SetTitle ("data/mc") ; 
    hPool->GetYaxis ()->SetRangeUser (min, max) ; 
    return hPool ;
  }


// --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + --- + 


void PrintHContent (TH1F * histo)
  {
    for (int iBin = 0 ; iBin < histo->GetNbinsX () ; ++iBin)
       {
         double A = histo->GetBinContent (iBin) ;
         cout << A << ", " ;
       }
    cout << "\n" ;   
  }
