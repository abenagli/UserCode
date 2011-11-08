#include <iostream>

#include "TFile.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TF1.h"
#include "TAxis.h"



int nMasses = 8;
int* masses = new int[nMasses];
float* widths = new float[nMasses];

void FitHiggsMassWindow(TFile* f, const int& mass, double& bestHalfWindow, double& bestHalfWindow_KF);
void FitAsymmHiggsMassWindow(TFile* f, TCanvas* c, const int& iMass, const int& mass, double& bestLowWindow, double& bestHigWindow, double& bestLowWindow_KF, double& bestHigWindow_KF);
double FindXMaximum(TGraph* graph);



void higgsMassWindowOptimization()
{
  TFile* f = TFile::Open("higgsMassWindowOptimization_PFlow.root");
  
  
  masses[0] = 250;
  masses[1] = 300;
  masses[2] = 350;
  masses[3] = 400;
  masses[4] = 450;
  masses[5] = 500;
  masses[6] = 550;
  masses[7] = 600;
  
  widths[0] = 4.04;
  widths[1] = 8.43;
  widths[2] = 15.2;
  widths[3] = 29.2;
  widths[4] = 47.0;
  widths[5] = 68.0;
  widths[6] = 93.2;
  widths[7] = 123.;
  
  
  TGraph* g = new TGraph();
  g -> SetMarkerColor(kRed);
  
  TGraph* g_KF = new TGraph();
  g_KF -> SetMarkerColor(kBlue);
  
  TGraph2D* asymmG = new TGraph2D();
  asymmG -> SetMarkerColor(kRed);
  
  TGraph2D* asymmG_KF = new TGraph2D();
  asymmG_KF -> SetMarkerColor(kBlue);
      
  double bestHalfWindow = -1.;
  double bestHalfWindow_KF = -0.5;
  
  double bestLowWindow = -1;
  double bestHigWindow = -1;
  
  double bestLowWindow_KF = -1;
  double bestHigWindow_KF = -1;
  
  
  
  TCanvas* c1 = new TCanvas("significances","significances",800,700);
  c1 -> Divide(3,3);
  
  for(int i = 0; i < nMasses; ++i)
  {
    FitHiggsMassWindow(f,c1,i,masses[i],bestHalfWindow,bestHalfWindow_KF);
    g -> SetPoint(i,masses[i],bestHalfWindow);
    g_KF -> SetPoint(i,masses[i],bestHalfWindow_KF);
  }
  
  c1 -> Print("significances.pdf","pdf");
  //delete c1;
  
  
  
  TCanvas* c2 = new TCanvas("asymmSignificances","asymmSignificances",800,700);
  c2 -> Divide(3,3);
  
  for(int i = 0; i < nMasses; ++i)
  {
    FitAsymmHiggsMassWindow(f,c2,i,masses[i],bestLowWindow,bestHigWindow,bestLowWindow_KF,bestHigWindow_KF);
    //asymmG -> SetPoint(i,masses[i],bestLowWindow,bestHighWindow,bestLowWindow_KF,bestHighWindow_KF);
    //asymmG_KF -> SetPoint(i,masses[i],bestLowWindow_KF,bestHighWindow_KF);
  }
  
  c2 -> Print("asymmSignificances.pdf","pdf");
  //delete c2;
  
  
  
  
  TCanvas* c3 = new TCanvas("significance vs. Higgs mass","significance vs. Higgs mass");
  c3 -> cd();
  c3 -> SetGridx();
  c3 -> SetGridy();
  
  g -> SetMinimum(20.);
  g -> SetMaximum(100.);
  
  g -> GetXaxis() -> SetTitle("Higgs mass   (GeV/c^{2})");
  g -> GetYaxis() -> SetTitle("best half-window   (GeV/c^{2})");

  g_KF -> GetXaxis() -> SetTitle("Higgs mass   (GeV/c^{2})");
  g_KF -> GetYaxis() -> SetTitle("best half-window   (GeV/c^{2})");
  
  g -> Draw("AP");
  g_KF -> Draw("P,same");
  
  // define legend                                                                                                                                                               
  TLegend* legend = new TLegend(0.20,0.75,0.40,0.90);
  legend -> SetFillColor(kWhite);
  legend -> SetLineColor(kWhite);
  legend -> SetFillStyle(1001);
  legend -> SetTextFont(42);
  legend -> SetTextSize(0.025);
  legend -> AddEntry(g,"before KF","P");
  legend -> AddEntry(g_KF,"after KF","P");
  legend -> Draw("same");
  
  c3 -> Update();
  c3 -> Print("significance_vs_HiggsMass.pdf","pdf");
  //delete c3;
}






void FitHiggsMassWindow(TFile* f, TCanvas* c, const int& iMass, const int& mass, double& bestHalfWindow, double& bestHalfWindow_KF)
{
  f -> cd();
  
  
  //------
  // no KF
  char graphName[50];
  sprintf(graphName,"g_significance_%d",mass);
  TGraph* g = (TGraph*)(f->Get(graphName));
  g -> SetMarkerColor(kRed);
  g -> SetMarkerStyle(1);
  g -> SetMarkerSize(0.5);
  g -> GetXaxis() -> SetTitle("half window   (GeV/c^{2})");
  g -> GetYaxis() -> SetTitle("S / #sqrt{S+B}");
  
  double x = FindXMaximum(g);
  double xMin = x - 0.40*x;
  double xMax = x + 0.75*x;
  
  char funzName[50];
  sprintf(funzName,"f_H%d",mass);
  TF1* funz = new TF1(funzName,"pol4",xMin,xMax);
  funz -> SetNpx(10000);
  funz -> SetLineColor(kRed);
  funz -> SetLineWidth(2);
  
  g -> Fit(funzName,"QNR+");
  bestHalfWindow = funz -> GetMaximumX();
  
  
  //--------
  // with KF
  sprintf(graphName,"g_significance_KF_%d",mass);
  TGraph* g_KF = (TGraph*)(f->Get(graphName));
  g_KF -> SetMarkerColor(kBlue);
  g_KF -> SetMarkerStyle(1);
  g_KF -> SetMarkerSize(0.5);
  g_KF -> GetXaxis() -> SetTitle("half window   (GeV/c^{2})");
  g_KF -> GetYaxis() -> SetTitle("S / #sqrt{S+B}");
    
  x = FindXMaximum(g_KF);
  xMin = x - 0.40*x;
  xMax = x + 0.75*x;
  
  sprintf(funzName,"f_KF_H%d",mass);
  TF1* funz_KF = new TF1(funzName,"pol4",xMin,xMax);
  funz_KF -> SetNpx(10000);
  funz_KF -> SetLineColor(kBlue);
  funz_KF -> SetLineWidth(2);
  
  g_KF -> Fit(funzName,"QNR+");
  bestHalfWindow_KF = funz_KF -> GetMaximumX();
  
  
  
  c -> cd(iMass+1);    
  gPad -> SetGridx();
  gPad -> SetGridy();
  
  g_KF -> GetXaxis() -> SetRangeUser(0.,x+1.5*x);
  g_KF -> SetMinimum(0.);
  g_KF -> SetMaximum(2.);
  g_KF -> Draw("AP");
  funz_KF -> Draw("same");
  
  g -> Draw("P,same");
  funz -> Draw("same");
  
  // define legend                                                                                                                                                               
  TLegend* legend = new TLegend(0.55,0.75,0.75,0.90);
  legend -> SetFillColor(kWhite);
  legend -> SetLineColor(kWhite);
  legend -> SetFillStyle(1001);
  legend -> SetTextFont(42);
  legend -> SetTextSize(0.025);
  legend -> AddEntry(g,"before KF","P");
  legend -> AddEntry(g_KF,"after KF","P");
  legend -> Draw("same");
  
  char latexBuffer[250];
  sprintf(latexBuffer,"M = %d GeV/c^{2}   #Gamma = %.2e",masses[iMass],widths[iMass]);
  TLatex* latex_bw = new TLatex(0.13,0.96,latexBuffer);
  latex_bw -> SetNDC();
  latex_bw -> SetTextFont(42);
  latex_bw -> SetTextSize(0.03);
  latex_bw -> Draw("same");
  
  sprintf(latexBuffer,"best half window = %.f GeV/c^{2}",bestHalfWindow);
  TLatex* latex = new TLatex(0.15,0.88,latexBuffer);
  latex -> SetNDC();
  latex -> SetTextFont(42);
  latex -> SetTextColor(kRed);
  latex -> SetTextSize(0.03);
  latex -> Draw("same");
  
  sprintf(latexBuffer,"best half window = %.f GeV/c^{2}",bestHalfWindow_KF);
  TLatex* latex_KF = new TLatex(0.15,0.83,latexBuffer);
  latex_KF -> SetNDC();
  latex_KF -> SetTextFont(42);
  latex_KF -> SetTextColor(kBlue);
  latex_KF -> SetTextSize(0.03);
  latex_KF -> Draw("same");
  
  c -> Update();
  
  
  std::cout << ">>> mass " << masses[iMass] << ":"
            << "   w/o KF [" << masses[iMass]-int(bestHalfWindow)    << "," << masses[iMass]+int(bestHalfWindow)    << "]"
            << "   w/ KF ["  << masses[iMass]-int(bestHalfWindow_KF) << "," << masses[iMass]+int(bestHalfWindow_KF) << "]"
            << std::endl;
  return;
}






void FitAsymmHiggsMassWindow(TFile* f, TCanvas* c, const int& iMass, const int& mass, double& bestLowWindow, double& bestHigWindow, double& bestLowWindow_KF, double& bestHigWindow_KF)
{
  f -> cd();
  


  //------
  // no KF
  char graphName[50];
  sprintf(graphName,"g_asymmSignificance_%d",mass);
  TGraph2D* g = (TGraph2D*)(f->Get(graphName));
  g -> GetXaxis() -> SetTitle("low window   (GeV/c^{2})");
  g -> GetYaxis() -> SetTitle("high window   (GeV/c^{2})");
  g -> GetZaxis() -> SetTitle("S / #sqrt{S+B}");
  g -> SetMinimum(0.4);
  g -> SetMaximum(2.0);
  
  /*
  double x = FindXMaximum(g);
  double xMin = x - 0.40*x;
  double xMax = x + 0.75*x;
  
  char funzName[50];
  sprintf(funzName,"f_H%d",mass);
  TF1* funz = new TF1(funzName,"pol4",xMin,xMax);
  funz -> SetNpx(10000);
  funz -> SetLineColor(kRed);
  funz -> SetLineWidth(2);
  
  g -> Fit(funzName,"QNR+");
  bestHalfWindow = funz -> GetMaximumX();
  */
  
  
  //--------
  // with KF
  sprintf(graphName,"g_asymmSignificance_KF_%d",mass);
  TGraph2D* g_KF = (TGraph2D*)(f->Get(graphName));
  g_KF -> GetXaxis() -> SetTitle("low window   (GeV/c^{2})");
  g_KF -> GetYaxis() -> SetTitle("high window   (GeV/c^{2})");
  g_KF -> GetZaxis() -> SetTitle("S / #sqrt{S+B}");
  
  /*
  x = FindXMaximum(g_KF);
  xMin = x - 0.40*x;
  xMax = x + 0.75*x;
  
  sprintf(funzName,"f_KF_H%d",mass);
  TF1* funz_KF = new TF1(funzName,"pol4",xMin,xMax);
  funz_KF -> SetNpx(10000);
  funz_KF -> SetLineColor(kBlue);
  funz_KF -> SetLineWidth(2);
  
  g_KF -> Fit(funzName,"QNR+");
  bestHalfWindow_KF = funz_KF -> GetMaximumX();
  */
  //g_KF -> Draw("same,P");
  
  
  c -> cd(iMass+1);    
  //gPad -> SetGridx();
  //gPad -> SetGridy();
  
  //g_KF -> GetXaxis() -> SetRangeUser(0.,x+1.5*x);
  //g_KF -> SetMinimum(0.);
  //g_KF -> SetMaximum(2.);
  //g_KF -> Draw("AP");
  //funz_KF -> Draw("same");
  
  g -> Draw("CONT");
  /*funz -> Draw("same");
  
  // define legend                                                                                                                                                               
  TLegend* legend = new TLegend(0.55,0.75,0.75,0.90);
  legend -> SetFillColor(kWhite);
  legend -> SetLineColor(kWhite);
  legend -> SetFillStyle(1001);
  legend -> SetTextFont(42);
  legend -> SetTextSize(0.025);
  legend -> AddEntry(g,"before KF","P");
  legend -> AddEntry(g_KF,"after KF","P");
  legend -> Draw("same");
  
  char latexBuffer[250];
  sprintf(latexBuffer,"M = %d GeV/c^{2}   #Gamma = %.2e",masses[iMass],widths[iMass]);
  TLatex* latex_bw = new TLatex(0.13,0.96,latexBuffer);
  latex_bw -> SetNDC();
  latex_bw -> SetTextFont(42);
  latex_bw -> SetTextSize(0.03);
  latex_bw -> Draw("same");
  
  sprintf(latexBuffer,"best half window = %.f GeV/c^{2}",bestHalfWindow);
  TLatex* latex = new TLatex(0.15,0.88,latexBuffer);
  latex -> SetNDC();
  latex -> SetTextFont(42);
  latex -> SetTextColor(kRed);
  latex -> SetTextSize(0.03);
  latex -> Draw("same");
  
  sprintf(latexBuffer,"best half window = %.f GeV/c^{2}",bestHalfWindow_KF);
  TLatex* latex_KF = new TLatex(0.15,0.83,latexBuffer);
  latex_KF -> SetNDC();
  latex_KF -> SetTextFont(42);
  latex_KF -> SetTextColor(kBlue);
  latex_KF -> SetTextSize(0.03);
  latex_KF -> Draw("same");
  
  c -> Update();
  
  
  std::cout << ">>> mass " << masses[iMass] << ":"
            << "   w/o KF [" << masses[iMass]-int(bestHalfWindow)    << "," << masses[iMass]+int(bestHalfWindow)    << "]"
            << "   w/ KF ["  << masses[iMass]-int(bestHalfWindow_KF) << "," << masses[iMass]+int(bestHalfWindow_KF) << "]"
            << std::endl;
  */
  return;
}





double FindXMaximum(TGraph* graph)
{
  double x,y;
  double xMax = 0.;
  double yMax = -999999999999.;
  
  for(int point = 0; point < graph->GetN(); ++point)
  {
    graph->GetPoint(point,x,y);
    if( y > yMax)
    {
      yMax = y;
      xMax = x;
    }
  }
  
  return xMax;
}
