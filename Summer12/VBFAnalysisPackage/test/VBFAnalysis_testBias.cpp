#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"
#include "RooFitFunctions.h"

#include <iostream>
#include <sstream>

#include "TFile.h"
#include "TCanvas.h"
#include "TNtuple.h"

#include "RooMsgService.h"
#include "RooWorkspace.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooDataSet.h"
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
  RooMsgService::instance().deleteStream(0);
  RooMsgService::instance().deleteStream(1);
  
  
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_testBias::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  // Parse the config file
  parseConfigFile (argv[1]);
  
  
  //[Input]
  std::string inputDir       = gConfigParser -> readStringOption("Input::inputDir");
  std::string inputDir2      = gConfigParser -> readStringOption("Input::inputDir");
  std::string analysisMethod = gConfigParser -> readStringOption("Input::analysisMethod");
  std::string parentMethod   = gConfigParser -> readStringOption("Input::parentMethod");
  std::string fitMethod      = gConfigParser -> readStringOption("Input::fitMethod");
  std::string workspaceName  = gConfigParser -> readStringOption("Input::workspaceName");
    
  //[Output]
  std::string outputDir = gConfigParser -> readStringOption("Output::outputDir");
  
  //[Options]
  float mass = gConfigParser -> readIntOption("Options::mH");
  
  float xWidth = gConfigParser -> readFloatOption("Options::xWidth");
  char xWidthChar[50];
  sprintf(xWidthChar,"%d",int(xWidth));
  
  int step = gConfigParser -> readIntOption("Options::step");
  char stepChar[50];
  sprintf(stepChar,"%d",step);
  
  std::string additionalCuts = gConfigParser -> readStringOption("Options::additionalCuts");
  
  std::string flavour = gConfigParser -> readStringOption("Options::flavour");
  
  float sigStrength = gConfigParser -> readFloatOption("Options::sigStrength");
  float bkgStrength = gConfigParser -> readFloatOption("Options::bkgStrength");
  
  int nToys = gConfigParser -> readIntOption("Options::nToys");
  
  
  if( additionalCuts == "none" )
  {
    inputDir  +=    "/combine_signal/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
    inputDir2 += "/countSignalEvents/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
    
    outputDir += "/combine_signal/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
  }
  else
  {
    inputDir  +=    "/combine_signal/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "_" + additionalCuts + "/";
    inputDir2 += "/countSignalEvents/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "_" + additionalCuts + "/";
    
    outputDir += "/combine_signal/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "_" + additionalCuts + "/";
  }
  
  
  // define infile
  std::stringstream inFileName;
  inFileName << inputDir << "/shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << ".root";
  
  std::stringstream inFileName2;
  inFileName2 << inputDir2 << "/countSignalEvents.root";
  
  std::stringstream outFileName;
  outFileName << outputDir << "/VBFAnalysis_testBias_parent_" << parentMethod << "_fit_" << fitMethod << "_" << mass << "_" << flavour << ".root";
  
  
  // define infile
  std::cout << ">>> drawWorkspace::open file " << inFileName.str() << std::endl;
  TFile* inFile  = new TFile((inFileName.str()).c_str(), "READ");
  TFile* inFile2 = new TFile((inFileName2.str()).c_str(), "READ");
  TFile* outFile = new TFile((outFileName.str()).c_str(),"RECREATE");

  inFile -> cd();
  RooWorkspace* workspace = (RooWorkspace*)( inFile->Get(workspaceName.c_str()) );
  //workspace -> Print();
  
  
  
  
  
  
  
  //-------------------
  // get the x variable
  
  RooRealVar* x         = (RooRealVar*)( workspace->var("x"));
  RooRealVar* rooXMin   = (RooRealVar*)( workspace->var("rooXMin"));
  RooRealVar* rooXMax   = (RooRealVar*)( workspace->var("rooXMax"));
  RooRealVar* rooXWidth = (RooRealVar*)( workspace->var("rooXWidth"));
  
  double xMin = rooXMin -> getVal();
  double xMax = rooXMax -> getVal();
  int nBins = int( (xMax-xMin)/xWidth );
  
  double lepNuWMMIN = GetLepNuWMMIN(mass);
  double lepNuWMMAX = GetLepNuWMMAX(mass);
  
  x -> setMin(xMin);
  x -> setMax(xMax);
  x -> setBins(nBins);
  x -> setRange("signal",lepNuWMMIN,lepNuWMMAX);
  
  
  
  //-------------------------
  // get the number of events
  
  RooRealVar* rooN_data_obs = (RooRealVar*)( workspace->var("rooN_data_obs") );
  RooRealVar* rooN_ggH      = (RooRealVar*)( workspace->var("rooN_ggH") );
  RooRealVar* rooN_qqH      = (RooRealVar*)( workspace->var("rooN_qqH") );
  
  double n_data_obs = rooN_data_obs -> getVal();
  double n_ggH = sigStrength * (rooN_ggH -> getVal());
  double n_qqH = sigStrength * (rooN_qqH -> getVal());
  double n_H = n_ggH + n_qqH;
  
  
  
  //-------------------
  // get the parent pdf
  
  std::cout << "\n\n\n***********************************************************" << std::endl;
  std::cout << "*** PARENT ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  std::stringstream parentName;
  parentName << "H" << mass << "/fitFunc_" << mass << "_" << flavour << "_" << parentMethod;
  
  TF1* parent = (TF1*)( inFile2->Get(parentName.str().c_str()) );
  parent -> SetName("parent");
  parent -> Print();
  
  
  
  //-------------------
  // get the signal pdf
  RooAbsPdf* ggH = (RooAbsPdf*)( workspace->pdf("ggH") );
  RooAbsPdf* qqH = (RooAbsPdf*)( workspace->pdf("qqH") );
  
  
  
  
  
  
  std::cout << "\n\n\n***********************************************************" << std::endl;
  std::cout << "*** VARIABLES ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  std::cout << "x: "      << x->getVal() << std::endl;
  std::cout << "xMin: "   << xMin        << std::endl;
  std::cout << "xMax: "   << xMax        << std::endl;
  std::cout << "xWidth: " << xWidth      << std::endl;
  
  std::cout << "n_data_obs: " << n_data_obs << std::endl;
  std::cout << "n_ggH: "      << n_ggH      << std::endl;
  std::cout << "n_qqH: "      << n_qqH      << std::endl;
  
  
  
    
  
  
  std::cout << "\n\n\n***********************************************************" << std::endl;
  std::cout << "*** TOY EXPERIMENTS ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  inFile -> cd();
  
  TH1F* h_diffB_parentB_fitB   = new TH1F("h_diffB_parentB_fitB",  "",20000,-10.,10.);
  TH1F* h_diffB_parentBS_fitB  = new TH1F("h_diffB_parentBS_fitB", "",20000,-10.,10.);
  TH1F* h_diffB_parentBS_fitBS = new TH1F("h_diffB_parentBS_fitBS","",20000,-10.,10.);
  TH1F* h_diffS_parentBS_fitBS = new TH1F("h_diffS_parentBS_fitBS","",20000,-10.,10.);
  TH1F* h_diffB_parentB_fitBS  = new TH1F("h_diffB_parentB_fitBS", "",20000,-10.,10.);
  TH1F* h_diffS_parentB_fitBS  = new TH1F("h_diffS_parentB_fitBS", "",20000,-10.,10.);
  
  TNtuple *ntuple = new TNtuple("ntuple","ntuple","n_parent_S:n_parent_B");
  
  //--------------------------------------
  // define background parent distribution
  
  double n_parent_B = parent->Integral(lepNuWMMIN,lepNuWMMAX)/parent->Integral(xMin,xMax) * int(n_data_obs);
  std::cout << ">>> n_parent_B: " << n_parent_B << std::endl; 
  
  
  
  //----------------------------------
  // define signal parent distribution
  
  RooGenericPdf* rooParentPdf_ggS = (RooGenericPdf*)( ggH->Clone("rooParentPdf_ggS") );
  RooGenericPdf* rooParentPdf_qqS = (RooGenericPdf*)( qqH->Clone("rooParentPdf_qqS") );
  
  RooAddPdf* rooParentPdf_S = new RooAddPdf("rooParentPdf_S","",RooArgList(*rooParentPdf_ggS,*rooParentPdf_qqS),RooArgList(*rooN_ggH,*rooN_qqH));
  
  RooAbsReal* integral_parent_S = rooParentPdf_S -> createIntegral(*x,NormSet(*x),Range("signal"));
  double n_parent_S = integral_parent_S->getVal() * (n_H);
  std::cout << ">>> n_parent_S: " << n_parent_S << std::endl; 
  
  //----------------------------------
  // fill the ntuple with nS and nB
  ntuple->Fill(n_parent_S,n_parent_B);

  
  
  //------------
  // create toys
  
  int nPars;
  
  for(int toyIt = 0; toyIt < nToys; ++toyIt)
  {
    if(toyIt%100 == 0) std::cout << ">>> generating toy " << toyIt << " / " << nToys << std::endl;
    
    TH1F* h_B_toy = new TH1F("h_B_toy","",nBins,xMin,xMax);
    h_B_toy -> Sumw2();
    for(int bin = 1; bin <= h_B_toy->GetNbinsX(); ++bin)
    {
      float binCenter = h_B_toy -> GetBinCenter(bin);
      float binLowEdge = binCenter - 0.5 * xWidth;
      float binHigEdge = binCenter + 0.5 * xWidth;
      float binContent = parent->Integral(binLowEdge,binHigEdge) / xWidth;
      h_B_toy -> SetBinContent(bin,binContent);
      h_B_toy -> SetBinError(bin,sqrt(binContent/bkgStrength));
    }
    
    RooDataHist* dh_B_toy = new RooDataHist("dh_B_toy","",RooArgList(*x),h_B_toy);
    
    RooDataSet* ds_S_toy = rooParentPdf_S -> generate(*x,int(n_H));
    TH1* h_S_toy = ds_S_toy -> createHistogram("h_S_toy",*x);
    
    TH1F* h_BS_toy = (TH1F*)( h_B_toy -> Clone("h_BS_toy") );
    h_BS_toy -> Add(h_S_toy);
    RooDataHist* dh_BS_toy = new RooDataHist("dh_BS_toy","",RooArgList(*x),h_BS_toy);
    
    
    
    TF1* fitFunc;

    FitData(&fitFunc,"fitFunc_"+fitMethod,
            h_B_toy,mass,step,flavour,additionalCuts,
            fitMethod,false);
    
    double n_toy_B = fitFunc->Integral(lepNuWMMIN,lepNuWMMAX)/fitFunc->Integral(xMin,xMax) * int(n_data_obs);
    
    h_diffB_parentB_fitB -> Fill(n_toy_B/n_parent_B - 1.);
    
    
    /*
    // generate B - fit B
    RooRealVar* B_toy = new RooRealVar("B_toy","",n_data_obs,0.,2.*n_data_obs);
    
    std::vector<RooRealVar*> pars_toy;
    std::vector<std::string> parNames_toy;
    RooGenericPdf* bkg_toy;
    nPars = DefineRooFitFunction(x,&bkg_toy,pars_toy,parNames_toy,fitMethod,mass,step,flavour,additionalCuts);
    
    RooAddPdf* rooTotPdf_B_toy = new RooAddPdf("rooTotPdf_B_toy","",RooArgList(*bkg_toy),RooArgList(*B_toy));
    rooTotPdf_B_toy -> fitTo(*dh_B_toy,Extended(kTRUE),Save(),PrintLevel(3));
    
    RooAbsReal* integral_B_toy = rooTotPdf_B_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    double n_B_toy = integral_B_toy->getVal() * B_toy->getVal();
    
    h_diffB_parentB_fitB -> Fill(n_B_toy/n_parent_B - 1.);
    
    
    
    // generate BS - fit B
    RooRealVar* B2_toy = new RooRealVar("B2_toy","",n_data_obs,0.,2.*n_data_obs);
    
    std::vector<RooRealVar*> pars2_toy;
    std::vector<std::string> parNames2_toy;
    RooGenericPdf* bkg2_toy;
    nPars = DefineRooFitFunction(x,&bkg2_toy,pars2_toy,parNames2_toy,fitMethod,mass,step,flavour,additionalCuts);
    
    RooAddPdf* rooTotPdf_B2_toy = new RooAddPdf("rooTotPdf_B2_toy","",RooArgList(*bkg2_toy),RooArgList(*B2_toy));
    rooTotPdf_B2_toy -> fitTo(*dh_BS_toy,Extended(kTRUE),Save(),PrintLevel(-10));
    
    RooAbsReal* integral_B2_toy = rooTotPdf_B2_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    double n_B2_toy = integral_B2_toy->getVal() * B2_toy->getVal();
    
    h_diffB_parentBS_fitB -> Fill(n_B2_toy/n_parent_B - 1.);
    
    
    
    // generate BS - fit BS
    RooRealVar* B3_toy = new RooRealVar("B3_toy","",n_data_obs,0.,2.*n_data_obs);
    RooRealVar* S3_toy = new RooRealVar("S3_toy","",n_H,0.,10.*n_H);
    
    std::vector<RooRealVar*> pars3_toy;
    std::vector<std::string> parNames3_toy;
    RooGenericPdf* bkg3_toy;
    nPars = DefineRooFitFunction(x,&bkg3_toy,pars3_toy,parNames3_toy,fitMethod,mass,step,flavour,additionalCuts);
    
    RooGenericPdf* sig3_toy = (RooGenericPdf*)( rooParentPdf_S -> Clone("sig3_toy") );
    
    RooAddPdf* rooTotPdf_BS3_toy = new RooAddPdf("rooTotPdf_BS3_toy","",RooArgList(*bkg3_toy,*sig3_toy),RooArgList(*B3_toy,*S3_toy));
    rooTotPdf_BS3_toy -> fitTo(*dh_BS_toy,Extended(kTRUE),Save(),PrintLevel(-10));
    
    RooAbsReal* integral_B3_toy = bkg3_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    RooAbsReal* integral_S3_toy = sig3_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    double n_B3_toy = integral_B3_toy->getVal() * B3_toy->getVal();
    double n_S3_toy = integral_S3_toy->getVal() * S3_toy->getVal();
    
    h_diffB_parentBS_fitBS -> Fill(n_B3_toy/n_parent_B - 1.);
    h_diffS_parentBS_fitBS -> Fill(n_S3_toy/n_parent_S - 1.);
    
    
    
    // generate B - fit BS
    RooRealVar* B4_toy = new RooRealVar("B4_toy","",n_data_obs,0.,2.*n_data_obs);
    RooRealVar* S4_toy = new RooRealVar("S4_toy","",n_H,0.,10.*n_H);
    
    std::vector<RooRealVar*> pars4_toy;
    std::vector<std::string> parNames4_toy;
    RooGenericPdf* bkg4_toy;
    nPars = DefineRooFitFunction(x,&bkg4_toy,pars4_toy,parNames4_toy,fitMethod,mass,step,flavour,additionalCuts);
    
    RooGenericPdf* sig4_toy = (RooGenericPdf*)( rooParentPdf_S -> Clone("sig4_toy") );
    
    RooAddPdf* rooTotPdf_BS4_toy = new RooAddPdf("rooTotPdf_BS4_toy","",RooArgList(*bkg4_toy,*sig4_toy),RooArgList(*B4_toy,*S4_toy));
    rooTotPdf_BS4_toy -> fitTo(*dh_B_toy,Extended(kTRUE),Save(),PrintLevel(-10));
    
    RooAbsReal* integral_B4_toy = bkg4_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    RooAbsReal* integral_S4_toy = sig4_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    double n_B4_toy = integral_B4_toy->getVal() * B4_toy->getVal();
    double n_S4_toy = integral_S4_toy->getVal() * S4_toy->getVal();
    
    h_diffB_parentB_fitBS -> Fill(n_B4_toy/n_parent_B - 1.);
    h_diffS_parentB_fitBS -> Fill(n_S4_toy/n_parent_S); */
    
    
    
    if(toyIt < 10)
    {
      outFile -> cd();
      
      char dirName[50];
      sprintf(dirName,"toy%d",toyIt);
      
      outFile -> mkdir(dirName);
      outFile -> cd(dirName);
      
      h_B_toy -> Write();
      parent -> Write();
      fitFunc -> Write();
      
      /*
      char canvasName[50];
      
      sprintf(canvasName,"parentB_fitB_%d",toyIt);
      TCanvas* c_parentB_fitB_toy = new TCanvas(canvasName);
      parent -> Draw();
      RooPlot* plot_parentB_fitB_toy = x->frame();
      dh_B_toy -> plotOn(plot_parentB_fitB_toy);
      rooTotPdf_B_toy -> plotOn(plot_parentB_fitB_toy,RooFit::LineColor(kRed));
      plot_parentB_fitB_toy -> Draw("same");
      c_parentB_fitB_toy -> Write();
      delete plot_parentB_fitB_toy;
      delete c_parentB_fitB_toy;
      
      sprintf(canvasName,"parentBS_fitB_%d",toyIt);
      TCanvas* c_parentBS_fitB_toy = new TCanvas(canvasName);
      RooPlot* plot_parentBS_fitB_toy = x->frame();
      dh_BS_toy -> plotOn(plot_parentBS_fitB_toy);
      rooTotPdf_B2_toy -> plotOn(plot_parentBS_fitB_toy);
      plot_parentBS_fitB_toy -> Draw();
      c_parentBS_fitB_toy -> Write();
      delete plot_parentBS_fitB_toy;
      delete c_parentBS_fitB_toy;
      
      sprintf(canvasName,"parentBS_fitBS_%d",toyIt);
      TCanvas* c_parentBS_fitBS_toy = new TCanvas(canvasName);
      RooPlot* plot_parentBS_fitBS_toy = x->frame();
      dh_BS_toy -> plotOn(plot_parentBS_fitBS_toy);
      rooTotPdf_BS3_toy -> plotOn(plot_parentBS_fitBS_toy);
      plot_parentBS_fitBS_toy -> Draw();
      c_parentBS_fitBS_toy -> Write();
      delete plot_parentBS_fitBS_toy;
      delete c_parentBS_fitBS_toy;
      
      sprintf(canvasName,"parentB_fitBS_%d",toyIt);
      TCanvas* c_parentB_fitBS_toy = new TCanvas(canvasName);
      RooPlot* plot_parentB_fitBS_toy = x->frame();
      dh_B_toy -> plotOn(plot_parentB_fitBS_toy);
      rooTotPdf_BS4_toy -> plotOn(plot_parentB_fitBS_toy);
      plot_parentB_fitBS_toy -> Draw();
      c_parentB_fitBS_toy -> Write();
      delete plot_parentB_fitBS_toy;
      delete c_parentB_fitBS_toy;*/
    }
    
    
    
    /*
    delete integral_B_toy;
    delete rooTotPdf_B_toy;
    delete bkg_toy;
    for(int parIt = 0; parIt < nPars; ++parIt)
      delete pars_toy[parIt];
    pars_toy.clear();
    parNames_toy.clear();
    delete B_toy;
    
    delete integral_B2_toy;
    delete rooTotPdf_B2_toy;
    delete bkg2_toy;
    for(int parIt = 0; parIt < nPars; ++parIt)
      delete pars2_toy[parIt];
    pars2_toy.clear();
    parNames2_toy.clear();
    delete B2_toy;
    
    delete integral_B3_toy;
    delete rooTotPdf_BS3_toy;
    delete bkg3_toy;
    for(int parIt = 0; parIt < nPars; ++parIt)
      delete pars3_toy[parIt];
    pars3_toy.clear();
    parNames3_toy.clear();
    delete B3_toy;
    
    delete integral_S3_toy;
    delete sig3_toy;
    delete S3_toy;
    
    delete integral_B4_toy;
    delete rooTotPdf_BS4_toy;
    delete bkg4_toy;
    for(int parIt = 0; parIt < nPars; ++parIt)
      delete pars4_toy[parIt];
    pars4_toy.clear();
    parNames4_toy.clear();
    delete B4_toy; 
    
    delete integral_S4_toy;
    delete sig4_toy;
    delete S4_toy;*/
    
    delete dh_B_toy;
    delete h_B_toy;
    delete h_S_toy;
    delete ds_S_toy;
    delete dh_BS_toy;
    delete h_BS_toy;
  }
  
  
  
  
  outFile -> cd();
  
  h_diffB_parentB_fitB -> Write();
  h_diffB_parentBS_fitB -> Write();
  h_diffB_parentBS_fitBS -> Write();
  h_diffS_parentBS_fitBS -> Write();
  h_diffB_parentB_fitBS -> Write();
  h_diffS_parentB_fitBS -> Write();
  ntuple -> Write();
  
  outFile -> Close();
  
  
    
  return 0;
}
