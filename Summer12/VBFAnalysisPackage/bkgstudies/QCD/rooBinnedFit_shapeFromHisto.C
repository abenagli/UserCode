/*
To compile
g++ rooBinnedFit_shapeFromHisto.C -o rooBinnedFit_shapeFromHisto `root-config --cflags --glibs` -lRooFitCore -lRooFit -lMinuit -lFoam
*/

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TNtuple.h"

#include "RooMsgService.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooWorkspace.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooPlot.h"

using namespace RooFit;

int main (int argc, char** argv) {

  
  std::string BKGPath = "/gwteray/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_EGMu_PFlow_noEleHLTcorr_NoMET/";
  std::string QCDPath = "/gwteray/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_EGMu_PFlow_QCDSelection_noEleHLTcorr_NoIso_NoId_NoMET/";
  std::string DATAPath = "/gwteray/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_EGMu_PFlow_noEleHLTcorr_NoMET/";

  std::vector<std::string> BKG_names;
  BKG_names.push_back("WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_govoni-SQWaT_PAT_42X_Fall11_v2/");
  BKG_names.push_back("DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_govoni-SQWaT_PAT_42X_Fall11_v3_2/");
  BKG_names.push_back("TTJets_TuneZ2_7TeV-madgraph-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  BKG_names.push_back("Tbar_TuneZ2_s-channel_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  BKG_names.push_back("Tbar_TuneZ2_t-channel_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  BKG_names.push_back("Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  BKG_names.push_back("T_TuneZ2_s-channel_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  BKG_names.push_back("T_TuneZ2_t-channel_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  BKG_names.push_back("T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  BKG_names.push_back("WW_TuneZ2_7TeV_pythia6_tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  BKG_names.push_back("WZ_TuneZ2_7TeV_pythia6_tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  BKG_names.push_back("ZZ_TuneZ2_7TeV_pythia6_tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");

  std::vector<std::string> DATA_names;
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011A-May10ReReco-v1/");
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011A-05Aug2011-v1/");
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011A-PromptReco-v6/");
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011A-PromptReco-v4/");
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011B-PromptReco-v1/");

  std::vector<std::string> QCD_names;
  QCD_names.push_back("data_SingleEle_SingleMu_Run2011A-May10ReReco-v1/");
  QCD_names.push_back("data_SingleEle_SingleMu_Run2011A-05Aug2011-v1/");
  QCD_names.push_back("data_SingleEle_SingleMu_Run2011A-PromptReco-v6/");
  QCD_names.push_back("data_SingleEle_SingleMu_Run2011A-PromptReco-v4/");
  QCD_names.push_back("data_SingleEle_SingleMu_Run2011B-PromptReco-v1/");
    
  int step = 10;
  char treeName[50];
  sprintf(treeName, "ntu_%d", step);
  
  float lumi = 5000.;
  std::string s_lumi = "5000.";
  
  int nBins = 100;
  double xMin = 20.;
  double xMin_signal = 30.;
  double xMax = 200.;
  
  std::string variableName = "met_et";
  std::string cutNameDATA_ele = "(lep_flavour == 11)";
  std::string cutNameQCD_ele  = "(lep_flavour == 11)";
  std::string cutNameMC_ele   = "(lep_flavour == 11) * (eventWeight * PUWeight * crossSection / totEvents * " + s_lumi + ")" ;
  std::string cutNameDATA_mu = "(lep_flavour == 13)";
  std::string cutNameQCD_mu  = "(lep_flavour == 13)";
  std::string cutNameMC_mu   = "(lep_flavour == 13) * (eventWeight * PUWeight * crossSection / totEvents * " + s_lumi + ")" ;

  TFile* outfile = new TFile("QCD_fit.root","RECREATE");
    
  // get the QCD shape histogram
  TH1F* QCDShapeHisto_ele = new TH1F("QCDShapeHisto_ele", "QCDShapeHisto_ele", nBins, xMin, xMax);
  QCDShapeHisto_ele -> Sumw2();
  TH1F* QCDShapeHisto_mu = new TH1F("QCDShapeHisto_mu", "QCDShapeHisto_mu", nBins, xMin, xMax);
  QCDShapeHisto_mu -> Sumw2();
  TChain* QCDShapeTree = new TChain(treeName,"");
  for ( unsigned int iQCD = 0; iQCD < QCD_names.size(); iQCD++ ) QCDShapeTree -> AddFile ( (QCDPath + QCD_names[iQCD] + "VBFAnalysis_PFlow.root").c_str() );
  QCDShapeTree -> Draw( (variableName+">>+ QCDShapeHisto_ele").c_str(), cutNameQCD_ele.c_str()  );
  QCDShapeTree -> Draw( (variableName+">>+ QCDShapeHisto_mu").c_str(), cutNameQCD_mu.c_str()  );
  
  QCDShapeHisto_ele -> Write();
  QCDShapeHisto_mu -> Write();
  
  //the background shape histograms
  TH1F* BKGShapeHisto_ele = new TH1F("BKGShapeHisto_ele", "BKGShapeHisto_ele", nBins, xMin, xMax);
  BKGShapeHisto_ele -> Sumw2();
  TH1F* BKGShapeHisto_mu = new TH1F("BKGShapeHisto_mu", "BKGShapeHisto_mu", nBins, xMin, xMax);
  BKGShapeHisto_mu -> Sumw2();
  TChain* BKGShapeTree = new TChain(treeName,"");
  for ( unsigned int iBKG = 0; iBKG < BKG_names.size(); iBKG++ ) BKGShapeTree -> AddFile ( (BKGPath + BKG_names[iBKG] + "VBFAnalysis_PFlow.root").c_str() );
  BKGShapeTree -> Draw( (variableName+">>+ BKGShapeHisto_ele").c_str(), cutNameMC_ele.c_str()  );
  BKGShapeTree -> Draw( (variableName+">>+ BKGShapeHisto_mu").c_str(), cutNameMC_mu.c_str()  );
  
  float NBKG_MC_ele = BKGShapeHisto_ele -> Integral();
  float NBKG_MC_mu  = BKGShapeHisto_mu  -> Integral();
  
  BKGShapeHisto_ele -> Write();
  BKGShapeHisto_mu -> Write();
  
  // get the data histogram
  TH1F* DATAShapeHisto_ele = new TH1F("DATAShapeHisto_ele", "DATAShapeHisto_ele", nBins, xMin, xMax);
  DATAShapeHisto_ele -> Sumw2();
  TH1F* DATAShapeHisto_mu = new TH1F("DATAShapeHisto_mu", "DATAShapeHisto_mu", nBins, xMin, xMax);
  DATAShapeHisto_mu -> Sumw2();
  TChain* DATAShapeTree = new TChain(treeName,"");
  for ( unsigned int iDATA = 0; iDATA < DATA_names.size(); iDATA++ ) DATAShapeTree -> AddFile ( (DATAPath + DATA_names[iDATA] + "VBFAnalysis_PFlow.root").c_str() );
  DATAShapeTree -> Draw( (variableName+">>+ DATAShapeHisto_ele").c_str(), cutNameDATA_ele.c_str()  );
  DATAShapeTree -> Draw( (variableName+">>+ DATAShapeHisto_mu").c_str(), cutNameDATA_mu.c_str()  );

  DATAShapeHisto_ele -> Write();
  DATAShapeHisto_mu -> Write();
  
  // define the RooRealVars
  RooRealVar x("x", variableName.c_str(), xMin, xMax);
//  x.setRange("signal", xMin_signal, xMax); 
  x.setRange("signal", xMin, xMax); 
    
  // define the RooDataHist
  RooDataHist rooDataHisto_ele("rooDataHisto_ele", "data_ele", RooArgList(x), DATAShapeHisto_ele);
  RooDataHist rooBKGShapeHisto_ele("rooBKGShapeHisto_ele", "BKG_ele", RooArgList(x), BKGShapeHisto_ele);
  RooDataHist rooQCDShapeHisto_ele("rooQCDShapeHisto_ele", "QCD_ele", RooArgList(x), QCDShapeHisto_ele);

  RooDataHist rooDataHisto_mu("rooDataHisto_mu", "data_mu", RooArgList(x), DATAShapeHisto_mu);
  RooDataHist rooBKGShapeHisto_mu("rooBKGShapeHisto_mu", "BKG_mu", RooArgList(x), BKGShapeHisto_mu);
  RooDataHist rooQCDShapeHisto_mu("rooQCDShapeHisto_mu", "QCD_mu", RooArgList(x), QCDShapeHisto_mu);
    
  // define the signal/bkg shapes from histograms
  RooHistPdf BKGPdf_ele("BKGPdf_ele", "BKG_ele", x, rooBKGShapeHisto_ele, 0) ;
  RooHistPdf QCDPdf_ele("QCDPdf_ele", "QCD_ele", x, rooQCDShapeHisto_ele, 0) ;

  RooHistPdf BKGPdf_mu("BKGPdf_mu", "BKG_mu", x, rooBKGShapeHisto_mu, 0) ;
  RooHistPdf QCDPdf_mu("QCDPdf_mu", "QCD_mu", x, rooQCDShapeHisto_mu, 0) ;
  
  // define the total shape
  RooRealVar NBKG_ele("NBKG_ele", "NBKG_ele", NBKG_MC_ele, NBKG_MC_ele, NBKG_MC_ele);
  RooRealVar NBKGfit_ele("NBKGfit_ele", "NBKGfit_ele", NBKG_MC_ele, NBKG_MC_ele*0.8, NBKG_MC_ele*1.2);
  RooRealVar NQCDfit_ele("NQCDfit_ele", "NQCDfit_ele", 1000., 0., 10000000.);

  RooRealVar NBKG_mu("NBKG_mu", "NBKG_mu", NBKG_MC_mu, NBKG_MC_mu, NBKG_MC_mu);
  RooRealVar NBKGfit_mu("NBKGfit_mu", "NBKGfit_mu", NBKG_MC_mu, NBKG_MC_mu*0.8, NBKG_MC_mu*1.2);
  RooRealVar NQCDfit_mu("NQCDfit_mu", "NQCDfit_mu", 1000., 0., 10000000.);

//  RooAddPdf totPdf("totPdf", "tot", RooArgList(QCDPdf,BKGPdf), RooArgList(NQCDfit,NBKGfit));
  RooAddPdf totPdf_ele("totPdf_ele", "tot_ele", RooArgList(QCDPdf_ele,BKGPdf_ele), RooArgList(NQCDfit_ele,NBKG_ele));
  RooAddPdf totPdf_mu("totPdf_mu", "tot_mu", RooArgList(QCDPdf_mu,BKGPdf_mu), RooArgList(NQCDfit_mu,NBKG_mu));
  
  // fit the histo
  totPdf_ele.fitTo(rooDataHisto_ele, RooFit::Extended(1), RooFit::Minos(1), RooFit::SumW2Error(kTRUE));
  totPdf_mu.fitTo(rooDataHisto_mu, RooFit::Extended(1), RooFit::Minos(1), RooFit::SumW2Error(kTRUE));
  
  // calculate events in signal region
  RooAbsReal* BKGIntegral_ele = BKGPdf_ele.createIntegral(x, NormSet(x), Range("signal")); 
  RooAbsReal* QCDIntegral_ele = QCDPdf_ele.createIntegral(x, NormSet(x), Range("signal"));   

  RooAbsReal* BKGIntegral_mu = BKGPdf_mu.createIntegral(x, NormSet(x), Range("signal")); 
  RooAbsReal* QCDIntegral_mu = QCDPdf_mu.createIntegral(x, NormSet(x), Range("signal"));   
  
  std::cout << "NBKG_ele in signal region = " << BKGIntegral_ele->getVal() * NBKG_ele.getVal() << std::endl;
  std::cout << "NQCD_ele in signal region = " << QCDIntegral_ele->getVal() * NQCDfit_ele.getVal() << std::endl;

  std::cout << "NBKG_mu in signal region = " << BKGIntegral_mu->getVal() * NBKG_mu.getVal() << std::endl;
  std::cout << "NQCD_mu in signal region = " << QCDIntegral_mu->getVal() * NQCDfit_mu.getVal() << std::endl;
  
  
  // plot 
  outfile -> cd();
  TCanvas* c_plot_ele = new TCanvas("c_plot_ele","c_plot_ele");
  RooPlot* rooPlot_ele = x.frame();
  rooDataHisto_ele.plotOn(rooPlot_ele, DataError(RooAbsData::SumW2));
  totPdf_ele.plotOn(rooPlot_ele, LineColor(kRed));
  totPdf_ele.plotOn(rooPlot_ele, Components(QCDPdf_ele), LineColor(kCyan+2));
  totPdf_ele.plotOn(rooPlot_ele, Components(BKGPdf_ele), LineColor(kBlack));
  rooPlot_ele->Draw();
  c_plot_ele -> Write();
  delete rooPlot_ele;
  delete c_plot_ele;

  TCanvas* c_plot_mu = new TCanvas("c_plot_mu","c_plot_mu");
  RooPlot* rooPlot_mu = x.frame();
  rooDataHisto_mu.plotOn(rooPlot_mu, DataError(RooAbsData::SumW2));
  totPdf_mu.plotOn(rooPlot_mu, LineColor(kRed));
  totPdf_mu.plotOn(rooPlot_mu, Components(QCDPdf_mu), LineColor(kCyan+2));
  totPdf_mu.plotOn(rooPlot_mu, Components(BKGPdf_mu), LineColor(kBlack));
  rooPlot_mu->Draw();
  c_plot_mu -> Write();
  delete rooPlot_mu;
  delete c_plot_mu;
  
  TNtuple *ntuple = new TNtuple("ntuple","ntuple","n_QCD_ele:n_QCD_mu");
  ntuple -> Fill(QCDIntegral_ele->getVal() * NQCDfit_ele.getVal(), QCDIntegral_mu->getVal() * NQCDfit_mu.getVal());
  ntuple -> Write();
  
  return 1;
}
