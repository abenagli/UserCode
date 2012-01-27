#include <iostream>

#include "TFile.h"
#include "TCanvas.h"

#include "RooWorkspace.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooGlobalFunc.h"
#include "RooChi2Var.h"

using namespace RooFit;



int main(int argc, char** argv)
{
  
  if( argc != 2)
  {
    std::cout << ">>> drawWorkspace::usage:   drawWorkspace   file.root" << std::endl;
    return -1;
  }
  
  
  TFile* f = TFile::Open(argv[1],"READ");
  f -> cd();
  
  RooWorkspace* workspace = (RooWorkspace*)( f->Get("workspace") );
  workspace -> Print();
  
  RooRealVar* x = (RooRealVar*)( workspace->var("x") );
  RooRealVar* N = (RooRealVar*)( workspace->var("N") );
  RooRealVar* L1 = (RooRealVar*)( workspace->var("L1") );
  RooRealVar* L2 = (RooRealVar*)( workspace->var("L2") );
  RooDataHist* data_obs = (RooDataHist*)( workspace->data("data_obs") );
  RooAbsPdf* ggH = (RooAbsPdf*)( workspace->pdf("ggH") );
  RooAbsPdf* qqH = (RooAbsPdf*)( workspace->pdf("qqH") );
  RooGenericPdf* bkg = (RooGenericPdf*)( workspace->pdf("bkg") );
  
  
  RooPlot* plot_data_obs = x->frame();
  TCanvas* c_data_obs = new TCanvas("c_data_obs","c_data_obs");
  data_obs -> plotOn(plot_data_obs);
  plot_data_obs -> Draw();
  c_data_obs -> Print("workspacePlot_data_obs.pdf","pdf");
  delete c_data_obs;
  
  RooPlot* plot_H = x->frame();
  TCanvas* c_H = new TCanvas("c_H","c_H");
  ggH -> plotOn(plot_H,LineColor(kRed));
  qqH -> plotOn(plot_H,LineColor(kBlue));
  plot_H -> Draw();
  c_H -> Print("workspacePlot_H.pdf","pdf");
  delete c_H;
  
  RooPlot* plot_bkg = x->frame();
  TCanvas* c_bkg = new TCanvas("c_bkg","c_bkg");
  bkg -> plotOn(plot_bkg,LineColor(kRed));
  plot_bkg -> Draw();
  c_bkg -> Print("workspacePlot_bkg.pdf","pdf");
  delete c_bkg;
  
  
  
  std::cout << "***********************************************************" << std::endl;
  std::cout << "*** FIT B ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  RooRealVar* B = new RooRealVar("B","B",89490.,40000.,100000.);
  RooAddPdf* rooTotPdf_B = new RooAddPdf("rooTotPdf_B","",RooArgList(*bkg),RooArgList(*B));
  RooFitResult* fitResult_B  = rooTotPdf_B -> fitTo(*data_obs,Save(),PrintLevel(-10));
  
  RooPlot* plot_fit_B = x->frame();
  TCanvas* c_fit_B = new TCanvas("c_fit_B","c_fit_B");
  data_obs -> plotOn(plot_fit_B);
  rooTotPdf_B -> plotOn(plot_fit_B);
  plot_fit_B -> Draw();
  c_fit_B -> Print("workspacePlot_fit_B.pdf","pdf");
  delete c_fit_B;
  
  
  
  std::cout << "***********************************************************" << std::endl;
  std::cout << "*** FIT S+B ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  RooRealVar* S = new RooRealVar("S","S",259.01,259.01,259.01);
  RooAddPdf* rooTotPdf_BS = new RooAddPdf("rooTotPdf_BS","",RooArgList(*bkg,*ggH),RooArgList(*B,*S));
  RooFitResult* fitResult_BS = rooTotPdf_BS -> fitTo(*data_obs,Save(),PrintLevel(-10));
  
  RooPlot* plot_fit_BS = x->frame();
  TCanvas* c_fit_BS = new TCanvas("c_fit_BS","c_fit_BS");
  data_obs -> plotOn(plot_fit_BS);
  rooTotPdf_BS -> plotOn(plot_fit_BS);
  plot_fit_BS -> Draw();
  c_fit_BS -> Print("workspacePlot_fit_BS.pdf","pdf");
  delete c_fit_BS;
  
  
  
  return 0;
}
