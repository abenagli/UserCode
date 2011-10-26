#include "../interface/Functions.h"



void compareHiggsMassFitFunctions()
{
  TFile* f = TFile::Open("/gwteraz/users/benaglia/data/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_Summer11_EGMu/fitHiggsMass_doubleExponential_PFlow_H400.root","READ");
  f -> cd();

  TH1F* histo = (TH1F*)(f->Get("data/h_lepNuW_m"));
  
  
  
  TCanvas* c1 = new TCanvas("c1","powerLaw");
  c1 -> cd();
  TF1* func = new TF1("func",powerLaw,200.,1000.,4);
  func -> SetNpx(10000);
  func -> SetLineColor(kRed);
  func -> SetLineWidth(1.);
  func -> SetParameters(15.55,1471.,150.,23.14);
  func -> SetParName(0,"N");
  func -> SetParName(1,"x_{0}");
  func -> SetParName(2,"#lambda");
  func -> SetParName(3,"n");
  func -> FixParameter(2,150.);
  //func -> Draw("same");
  histo -> Fit("func","RL");
  c1 -> Update();
  c1 -> Print("powerLaw.png","png");
  
  
  
  TCanvas* c2 = new TCanvas("c2","exponential");
  c2 -> cd();  
  TF1* func2 = new TF1("func2",exponential,200.,1000.,2);
  func2 -> SetNpx(10000);
  func2 -> SetLineColor(kBlue);
  func2 -> SetLineWidth(1.);
  func2 -> SetParameters(5.,0.012);
  func2 -> SetParName(0,"N");
  func2 -> SetParName(1,"#lambda");
  //func2 -> Draw("same");
  histo -> Fit("func2","RL");
  c2 -> Update();
  c2 -> Print("exponential.png","png");  
  
  
  
  TCanvas* c3 = new TCanvas("c3","doubleExponential");
  c3 -> cd();
  TF1* func3 = new TF1("func3",doubleExponential,200.,1000.,4);
  func3 -> SetNpx(10000);
  func3 -> SetLineColor(kGreen);
  func3 -> SetLineWidth(1.);
  func3 -> SetParameters(3.,0.012,5.,0.010.);
  func3 -> SetParName(0,"N1");
  func3 -> SetParName(1,"#lambda1");
  func3 -> SetParName(2,"N2");
  func3 -> SetParName(3,"#lambda2");
  //func3 -> Draw("same");
  histo -> Fit("func3","RL");
  c3 -> Update();
  c3 -> Print("doubleExponential.png","png");
  
  
  
  TCanvas* c4 = new TCanvas("c4","comparison");
  c4 -> cd();
  c4 -> SetLogy();
  histo -> Draw();
  func -> Draw("sames");
  func2 -> Draw("sames");
  func3 -> Draw("sames");
  c4 -> Update();
  c4 -> Print("comparison.png","png");
}
