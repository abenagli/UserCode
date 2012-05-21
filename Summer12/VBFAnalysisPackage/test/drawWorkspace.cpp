#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"

#include <iostream>
#include <sstream>

#include "TFile.h"
#include "TCanvas.h"

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

void InitializeBkgPdf(RooRealVar* x, RooGenericPdf** bkg_toy, RooRealVar** pars_toy, float* initPars, const std::string& fitMethod, const int& nPars );



int main(int argc, char** argv)
{
  RooMsgService::instance().deleteStream(0);
  RooMsgService::instance().deleteStream(1);
  
  
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> drawWorkspace::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  // Parse the config file
  parseConfigFile (argv[1]);
  
  
  //[Input]
  std::string inputDir       = gConfigParser -> readStringOption("Input::inputDir");
  std::string analysisMethod = gConfigParser -> readStringOption("Input::analysisMethod");
  std::string fitMethod      = gConfigParser -> readStringOption("Input::fitMethod");
  
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
  
  int nToys = gConfigParser -> readIntOption("Options::nToys");
  
  
  if( additionalCuts == "none" )
  {
    inputDir += "/combine_signal/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
  }
  else
  {
    inputDir += "/coumbine_signal/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "_" + additionalCuts + "/";
  }
  
  // define infile
  std::stringstream inFileName;
  if( analysisMethod != "sidebands" )
    inFileName << inputDir << "/shapes_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << ".root";
  else
    inFileName << inputDir << "/shapes_" << analysisMethod << "_" << mass << "_" << flavour << ".root";
  
  std::stringstream outFileName;
  if( analysisMethod != "sidebands" )
    outFileName << outputDir << "/drawWorkspace_" << analysisMethod << "_" << fitMethod << "_" << mass << "_" << flavour << ".root";
  else
    outFileName << outputDir << "/drawWorkspace_" << analysisMethod << "_" << mass << "_" << flavour << ".root";
  
  
  
  
  
  
  //------------------------------------
  // open the file and get the workspace
  
  std::cout << ">>> drawWorkspace::open file " << inFileName.str() << std::endl;
  TFile* inFile  = new TFile((inFileName.str()).c_str(), "READ");
  TFile* outFile = new TFile((outFileName.str()).c_str(),"RECREATE");
  
  inFile -> cd();
  RooWorkspace* workspace = (RooWorkspace*)( inFile->Get("workspace") );
  workspace -> Print();
  
  
  
  //-------------------
  // get the x variable
  
  RooRealVar* x         = (RooRealVar*)( workspace->var("x"));
  RooRealVar* rooXMin   = (RooRealVar*)( workspace->var("rooXMin"));
  RooRealVar* rooXMax   = (RooRealVar*)( workspace->var("rooXMax"));
  RooRealVar* rooXWidth = (RooRealVar*)( workspace->var("rooXWidth"));
 
  x -> setMin(rooXMin->getVal());
  x -> setMax(rooXMax->getVal());
  x -> setBins(int((rooXMax->getVal()-rooXMin->getVal())/rooXWidth->getVal()));
  x -> setRange("signal",GetLepNuWMMIN(mass),GetLepNuWMMAX(mass));
  
  
  
  //-------------------------
  // get the number of events
  
  RooRealVar* rooN_data_obs = (RooRealVar*)( workspace->var("rooN_data_obs") );
  RooRealVar* rooN_ggH      = (RooRealVar*)( workspace->var("rooN_ggH") );
  RooRealVar* rooN_qqH      = (RooRealVar*)( workspace->var("rooN_qqH") );
  
  double n_data_obs = rooN_data_obs -> getVal();
  double n_ggH = sigStrength * (rooN_ggH -> getVal());
  double n_qqH = sigStrength * (rooN_qqH -> getVal());
  double n_H = n_ggH + n_qqH;
  
  
  
  //------------
  // get the pdf
  RooDataHist* data_obs = (RooDataHist*)( workspace->data("data_obs") );
  RooAbsPdf* ggH = (RooAbsPdf*)( workspace->pdf("ggH") );
  RooAbsPdf* qqH = (RooAbsPdf*)( workspace->pdf("qqH") );
  RooGenericPdf* bkg = (RooGenericPdf*)( workspace->pdf("bkg") );
  
  
  
  //-------------------
  // get the parameters  
  
  int nPars = 0;
  if( fitMethod == "exponential" )                 nPars = 1;
  if( fitMethod == "attenuatedExponential" )       nPars = 3;
  if( fitMethod == "doubleExponential" )           nPars = 3;
  if( fitMethod == "attenuatedDoubleExponential" ) nPars = 5;
  
  float* initPars = new float[nPars];
  std::string* initParNames = new std::string[nPars];
  
  if( fitMethod == "exponential")
  {
    RooRealVar* roo_L1 = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_L1").c_str()) );
    initPars[0] = roo_L1 -> getVal();
    initParNames[0] == "L1";
  }
  
  if( fitMethod == "attenuatedExponential")
  {
    RooRealVar* roo_mu = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_mu").c_str()) );
    initPars[0] = roo_mu -> getVal();
    initParNames[0] = "mu";
    
    RooRealVar* roo_kT = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_kT").c_str()) );
    initPars[1] = roo_kT -> getVal();
    initParNames[1] = "kT";
    
    RooRealVar* roo_L1 = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_L1").c_str()) );
    initPars[2] = roo_L1 -> getVal();
    initParNames[2] = "L1";
  }
  
  if( fitMethod == "doubleExponential")
  {
    RooRealVar* roo_N = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_N").c_str()) );
    initPars[0] = roo_N -> getVal();
    initParNames[0] = "N";
    
    RooRealVar* roo_L1 = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_L1").c_str()) );
    initPars[1] = roo_L1 -> getVal();
    initParNames[1] = "L1";
    
    RooRealVar* roo_L2 = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_L2").c_str()) );
    initPars[2] = roo_L2 -> getVal();
    initParNames[2] = "L2";
  }
  
  if( fitMethod == "attenuatedDoubleExponential")
  {
    RooRealVar* roo_mu = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_mu").c_str()) );
    initPars[0] = roo_mu -> getVal();
    initParNames[0] = "mu";
    
    RooRealVar* roo_kT = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_kT").c_str()) );
    initPars[1] = roo_kT -> getVal();
    initParNames[1] = "kT";
    
    RooRealVar* roo_N = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_N").c_str()) );
    initPars[2] = roo_N -> getVal();
    initParNames[2] = "N";
    
    RooRealVar* roo_L1 = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_L1").c_str()) );
    initPars[3] = roo_L1 -> getVal();
    initParNames[3] = "L1";
    
    RooRealVar* roo_L2 = (RooRealVar*)( workspace->var(("CMS_HWWlvjj_"+flavour+"_L2").c_str()) );
    initPars[4] = roo_L2 -> getVal();
    initParNames[4] = "L2";
  }
  
  
  
  
  
  
  std::cout << "\n\n\n***********************************************************" << std::endl;
  std::cout << "*** VARIABLES ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  std::cout << "x: "      << x->getVal() << std::endl;
  std::cout << "xMin: "   << rooXMin->getVal() << std::endl;
  std::cout << "xMax: "   << rooXMax->getVal() << std::endl;
  std::cout << "xWidth: " << rooXWidth->getVal() << std::endl;
  
  std::cout << "n_data_obs: " << n_data_obs << std::endl;
  std::cout << "n_ggH: "      << n_ggH      << std::endl;
  std::cout << "n_qqH: "      << n_qqH      << std::endl;
  
  
  
  std::cout << "\n\n\n***********************************************************" << std::endl;
  std::cout << "*** PARAMETERS ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  for(int parIt = 0; parIt < nPars; ++parIt)
  {
    std::cout << initParNames[parIt] << ": " << initPars[parIt] << std::endl;
  }
  
  
  
  
  
  
  std::cout << "\n\n\n***********************************************************" << std::endl;
  std::cout << "*** PRINT HISTOGRAMS ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  outFile -> cd();
  
  TCanvas* c_data_obs = new TCanvas("c_data_obs","c_data_obs");  
  RooPlot* plot_data_obs = x->frame();
  data_obs -> plotOn(plot_data_obs);
  plot_data_obs -> Draw();
  c_data_obs -> Write();
  delete plot_data_obs;
  delete c_data_obs;
  
  
  TCanvas* c_H = new TCanvas("c_H","c_H");
  RooPlot* plot_H = x->frame();
  ggH -> plotOn(plot_H,LineColor(kRed));
  qqH -> plotOn(plot_H,LineColor(kBlue));
  plot_H -> Draw();
  c_H -> Write();
  delete plot_H;
  delete c_H;
  

  TCanvas* c_bkg = new TCanvas("c_bkg","c_bkg");
  RooPlot* plot_bkg = x->frame();
  bkg -> plotOn(plot_bkg,LineColor(kRed));
  plot_bkg -> Draw();
  c_bkg -> Write();
  delete plot_bkg;
  delete c_bkg;
  
  
  
  
  
  
  std::cout << "\n\n\n***********************************************************" << std::endl;
  std::cout << "*** FIT B ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  inFile -> cd();
  
  RooRealVar* B = new RooRealVar("B","",n_data_obs,0.,2.*n_data_obs);
  RooAddPdf* rooTotPdf_B = new RooAddPdf("rooTotPdf_B","",RooArgList(*bkg),RooArgList(*B));
  rooTotPdf_B -> fitTo(*data_obs,Extended(kTRUE),Save(),PrintLevel(-1));
  
  std::cout  << ">>> B: " << B -> getVal() << std::endl;
  
  
  outFile -> cd();
  
  TCanvas* c_fit_B = new TCanvas("c_fit_B","c_fit_B");
  RooPlot* plot_fit_B = x->frame();
  data_obs -> plotOn(plot_fit_B);
  rooTotPdf_B -> plotOn(plot_fit_B);
  plot_fit_B -> Draw();
  c_fit_B -> Write();
  delete plot_fit_B;
  delete c_fit_B;
  
  
  
  
  
  
  std::cout << "\n\n\n***********************************************************" << std::endl;
  std::cout << "*** TOY EXPERIMENTS ***" << std::endl;
  std::cout << "***********************************************************" << std::endl;
  
  inFile -> cd();
  
  TH1F* h_diffB_parentB_fitB   = new TH1F("h_diffB_parentB_fitB",  "",400,-1.,1.);
  TH1F* h_diffB_parentBS_fitB  = new TH1F("h_diffB_parentBS_fitB", "",400,-1.,1.);
  TH1F* h_diffB_parentBS_fitBS = new TH1F("h_diffB_parentBS_fitBS","",400,-1.,1.);
  TH1F* h_diffS_parentBS_fitBS = new TH1F("h_diffS_parentBS_fitBS","",400,-1.,1.);
  
  
  
  //--------------------------------------1.
  // define background parent distribution
  
  RooRealVar** pars = new RooRealVar*[nPars];
  RooGenericPdf* rooParentPdf_B;
  
  if( fitMethod == "exponential" )
  {
    pars[0] = new RooRealVar("parent_L1","",initPars[0],initPars[0],initPars[0]);
    rooParentPdf_B = new RooGenericPdf("rooParentPdf_B","","exp(-1*@1*@0)",RooArgSet(*x,*pars[0]));
  }
  
  if( fitMethod == "attenuatedExponential" )
  {
    pars[0] = new RooRealVar("parent_mu","",initPars[0],initPars[0],initPars[0]);
    pars[1] = new RooRealVar("parent_kT","",initPars[1],initPars[1],initPars[1]);
    pars[2] = new RooRealVar("parent_L1","",initPars[2],initPars[2],initPars[2]);
    rooParentPdf_B = new RooGenericPdf("rooParentPdf_B","","1./(exp(-1.*(@0-@1)/@2)+1.) * exp(-1*@3*@0)",RooArgSet(*x,*pars[0],*pars[1],*pars[2]));
  }
  
  if( fitMethod == "doubleExponential" )
  {
    pars[0] = new RooRealVar("parent_N","", initPars[0],initPars[0],initPars[0]);
    pars[1] = new RooRealVar("parent_L1","",initPars[1],initPars[1],initPars[1]);
    pars[2] = new RooRealVar("parent_L2","",initPars[2],initPars[2],initPars[2]);
    rooParentPdf_B = new RooGenericPdf("rooParentPdf_B","","(exp(-1*@2*@0) + @1*exp(-1*@3*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2]));
  }
  
  if( fitMethod == "attenuatedDoubleExponential" )
  {
    pars[0] = new RooRealVar("parent_mu","",initPars[0],initPars[0],initPars[0]);
    pars[1] = new RooRealVar("parent_kT","",initPars[1],initPars[1],initPars[1]);
    pars[2] = new RooRealVar("parent_N","", initPars[2],initPars[2],initPars[2]);
    pars[3] = new RooRealVar("parent_L1","",initPars[3],initPars[3],initPars[3]);
    pars[4] = new RooRealVar("parent_L2","",initPars[4],initPars[4],initPars[4]);
    rooParentPdf_B = new RooGenericPdf("bkg","","1./(exp(-1.*(@0-@1)/@2)+1.) * (exp(-1*@4*@0) + @3*exp(-1*@5*@0))",RooArgSet(*x,*pars[0],*pars[1],*pars[2],*pars[3],*pars[4]));
  }
  
  RooAbsReal* integral_parent_B = rooParentPdf_B -> createIntegral(*x,NormSet(*x),Range("signal"));
  double n_parent_B = integral_parent_B->getVal() * int(n_data_obs);
  std::cout << ">>> n_parent_B: " << n_parent_B << std::endl; 
  
  
  
  //----------------------------------
  // define signal parent distribution
  
  RooGenericPdf* rooParentPdf_ggS = (RooGenericPdf*)( ggH->Clone("rooParentPdf_ggS") );
  RooGenericPdf* rooParentPdf_qqS = (RooGenericPdf*)( qqH->Clone("rooParentPdf_qqS") );
  
  RooAddPdf* rooParentPdf_S = new RooAddPdf("rooParentPdf_S","",RooArgList(*rooParentPdf_ggS,*rooParentPdf_qqS),RooArgList(*rooN_ggH,*rooN_qqH));
  
  RooAbsReal* integral_parent_S = rooParentPdf_S -> createIntegral(*x,NormSet(*x),Range("signal"));
  double n_parent_S = integral_parent_S->getVal() * (n_H);
  std::cout << ">>> n_parent_S: " << n_parent_S << std::endl; 
  
  
  
  //------------
  // create toys
  
  for(int toyIt = 0; toyIt < nToys; ++toyIt)
  {
    if(toyIt%100 == 0) std::cout << ">>> generating toy " << toyIt << " / " << nToys << "\r" << std::flush;
    
    RooDataSet* ds_B_toy = rooParentPdf_B -> generate(*x,int(n_data_obs));
    RooDataHist* dh_B_toy = ds_B_toy -> binnedClone();
    
    RooDataSet* ds_BS_toy = rooParentPdf_S -> generate(*x,int(n_H));
    ds_BS_toy -> append(*ds_B_toy);
    RooDataHist* dh_BS_toy = ds_BS_toy -> binnedClone();
    
    
    
    // generate B - fit B
    RooRealVar* B_toy = new RooRealVar("B_toy","",n_data_obs,0.,2.*n_data_obs);
    
    RooRealVar** pars_toy = new RooRealVar*[nPars];
    RooGenericPdf* bkg_toy;
    InitializeBkgPdf(x,&bkg_toy,pars_toy,initPars,fitMethod,nPars);
    
    RooAddPdf* rooTotPdf_B_toy = new RooAddPdf("rooTotPdf_B_toy","",RooArgList(*bkg_toy),RooArgList(*B_toy));
    rooTotPdf_B_toy -> fitTo(*dh_B_toy,Extended(kTRUE),Save(),PrintLevel(-10));
    
    RooAbsReal* integral_B_toy = rooTotPdf_B_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    double n_B_toy = integral_B_toy->getVal() * B_toy->getVal();
    
    h_diffB_parentB_fitB -> Fill(n_B_toy/n_parent_B - 1.);
    
    
    
    // generate BS - fit B
    RooRealVar* B2_toy = new RooRealVar("B2_toy","",n_data_obs,0.,2.*n_data_obs);
    
    RooRealVar** pars2_toy = new RooRealVar*[nPars];
    RooGenericPdf* bkg2_toy;
    InitializeBkgPdf(x,&bkg2_toy,pars2_toy,initPars,fitMethod,nPars);
    
    RooAddPdf* rooTotPdf_B2_toy = new RooAddPdf("rooTotPdf_B2_toy","",RooArgList(*bkg2_toy),RooArgList(*B2_toy));
    rooTotPdf_B2_toy -> fitTo(*dh_BS_toy,Extended(kTRUE),Save(),PrintLevel(-10));
    
    RooAbsReal* integral_B2_toy = rooTotPdf_B2_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    double n_B2_toy = integral_B2_toy->getVal() * B2_toy->getVal();
    
    h_diffB_parentBS_fitB -> Fill(n_B2_toy/n_parent_B - 1.);
    
    
    
    // generate BS - fit BS
    RooRealVar* B3_toy = new RooRealVar("B3_toy","",n_data_obs,0.,2.*n_data_obs);
    RooRealVar* S3_toy = new RooRealVar("S3_toy","",n_H,0.,2.*n_H);
    
    RooRealVar** pars3_toy = new RooRealVar*[nPars];
    RooGenericPdf* bkg3_toy;
    InitializeBkgPdf(x,&bkg3_toy,pars3_toy,initPars,fitMethod,nPars);
    
    RooGenericPdf* sig3_toy = (RooGenericPdf*)( rooParentPdf_S -> Clone("sig3_toy") );
    
    RooAddPdf* rooTotPdf_BS3_toy = new RooAddPdf("rooTotPdf_BS3_toy","",RooArgList(*bkg3_toy,*sig3_toy),RooArgList(*B3_toy,*S3_toy));
    rooTotPdf_BS3_toy -> fitTo(*dh_BS_toy,Extended(kTRUE),Save(),PrintLevel(-10));
    
    RooAbsReal* integral_B3_toy = bkg3_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    RooAbsReal* integral_S3_toy = sig3_toy -> createIntegral(*x,NormSet(*x),Range("signal"));
    double n_B3_toy = integral_B3_toy->getVal() * B3_toy->getVal();
    double n_S3_toy = integral_S3_toy->getVal() * S3_toy->getVal();
    
    h_diffB_parentBS_fitBS -> Fill(n_B3_toy/n_parent_B - 1.);
    h_diffS_parentBS_fitBS -> Fill(n_S3_toy/n_parent_S - 1.);
    
    
    
    if(toyIt < 10)
    {
      outFile -> cd();
      
      char dirName[50];
      sprintf(dirName,"toy%d",toyIt);
      
      outFile -> mkdir(dirName);
      outFile -> cd(dirName);
      
      char canvasName[50];
      
      sprintf(canvasName,"parentB_fitB_%d",toyIt);
      TCanvas* c_parentB_fitB_toy = new TCanvas(canvasName);
      RooPlot* plot_parentB_fitB_toy = x->frame();
      dh_B_toy -> plotOn(plot_parentB_fitB_toy);
      rooTotPdf_B_toy -> plotOn(plot_parentB_fitB_toy);
      plot_parentB_fitB_toy -> Draw();
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
    }
    
    
    
    delete integral_B_toy;
    delete rooTotPdf_B_toy;
    delete bkg_toy;
    for(int parIt = 0; parIt < nPars; ++parIt)
      delete pars_toy[parIt];
    delete B_toy;
    
    delete integral_B2_toy;
    delete rooTotPdf_B2_toy;
    delete bkg2_toy;
    for(int parIt = 0; parIt < nPars; ++parIt)
      delete pars2_toy[parIt];
    delete B2_toy;
    
    delete integral_B3_toy;
    delete rooTotPdf_BS3_toy;
    delete bkg3_toy;
    for(int parIt = 0; parIt < nPars; ++parIt)
      delete pars3_toy[parIt];
    delete B3_toy;
    
    delete integral_S3_toy;
    delete sig3_toy;
    delete S3_toy;
    
    delete dh_B_toy;
    delete ds_B_toy;
    delete dh_BS_toy;
    delete ds_BS_toy;
  }
  
  
  
  
  outFile -> cd();
  
  h_diffB_parentB_fitB -> Write();
  h_diffB_parentBS_fitB -> Write();
  h_diffB_parentBS_fitBS -> Write();
  h_diffS_parentBS_fitBS -> Write();
  
  outFile -> Close();
  
  
    
  return 0;
}






void InitializeBkgPdf(RooRealVar* x, RooGenericPdf** bkg_toy, RooRealVar** pars_toy, float* initPars, const std::string& fitMethod, const int& nPars )
{
  if( fitMethod == "exponential" )
  {
    pars_toy[0] = new RooRealVar("parent_L1","",initPars[0],0.,0.1);
    (*bkg_toy) = new RooGenericPdf("bkg_toy","","exp(-1*@1*@0)",RooArgSet(*x,*pars_toy[0]));
  }
  
  if( fitMethod == "attenuatedExponential" )
  {
    pars_toy[0] = new RooRealVar("parent_mu","",initPars[0],initPars[0],initPars[0]);
    pars_toy[1] = new RooRealVar("parent_kT","",initPars[1],initPars[1],initPars[1]);
    pars_toy[2] = new RooRealVar("parent_L1","",initPars[2],0.,0.1);
    (*bkg_toy) = new RooGenericPdf("bkg_toy","","1./(exp(-1.*(@0-@1)/@2)+1.) * exp(-1*@3*@0)",RooArgSet(*x,*pars_toy[0],*pars_toy[1],*pars_toy[2]));
  }
  
  if( fitMethod == "doubleExponential" )
  {
    pars_toy[0] = new RooRealVar("parent_N","", initPars[0],0.,1000000000.);
    pars_toy[1] = new RooRealVar("parent_L1","",initPars[1],0.,1.);
    pars_toy[2] = new RooRealVar("parent_L2","",initPars[2],0.,1.);
    (*bkg_toy) = new RooGenericPdf("bkg_toy","","(exp(-1*@2*@0) + @1*exp(-1*@3*@0))",RooArgSet(*x,*pars_toy[0],*pars_toy[1],*pars_toy[2]));
  }
  
  if( fitMethod == "attenuatedDoubleExponential" )
  {
    pars_toy[0] = new RooRealVar("parent_mu","",initPars[0],initPars[0],initPars[0]);
    pars_toy[1] = new RooRealVar("parent_kT","",initPars[1],initPars[1],initPars[1]);
    pars_toy[2] = new RooRealVar("parent_N","", initPars[2],0.,1000000000.);
    pars_toy[3] = new RooRealVar("parent_L1","",initPars[3],0.,0.1);
    pars_toy[4] = new RooRealVar("parent_L2","",initPars[4],0.,0.1);
    (*bkg_toy) = new RooGenericPdf("bkg_toy","","1./(exp(-1.*(@0-@1)/@2)+1.) * (exp(-1*@4*@0) + @3*exp(-1*@5*@0))",RooArgSet(*x,*pars_toy[0],*pars_toy[1],*pars_toy[2],*pars_toy[3],*pars_toy[4]));
  }
}
