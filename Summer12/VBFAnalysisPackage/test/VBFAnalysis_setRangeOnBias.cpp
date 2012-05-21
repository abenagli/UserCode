#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"
#include "RooFitFunctions.h"

#include <iostream>
#include <sstream>

#include "TFile.h"
#include "TH2F.h"
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


int countBins (TH1F * histo, double start, double end)
{
  int counter = 0 ; 
  for (int iBin = 1 ; iBin <= histo->GetNbinsX () ; ++iBin)
    {
      if (histo->GetBinLowEdge (iBin) < start) continue ;
      if (histo->GetBinLowEdge (iBin + 1) > end) continue ;
      ++counter ;
    }
  return counter ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


int getTotError (TH1F * histo)
{
  double totError = 0 ; 
  for (int iBin = 1 ; iBin <= histo->GetNbinsX () ; ++iBin)
    {
      totError += histo->GetBinError (iBin) ;
    }
  return totError ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----


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
  
  
  string parentMethods[6] =
    {"attenuatedExponential"       , 
    "attenuatedDoubleExponential" , 
    "attenuatedPowerLaw"          , 
    "attenuatedExpPol2order"      , 
    "attenuatedExpPol3order"      , 
    "attenuatedExpPol4order"} ;
  int testingMethodsNum = 4 ; //PG how many functions have to be included in the test
  
  TF1* parents[6] ;
  for (int iMeth = 0 ; iMeth < 6 ; ++iMeth)
    { 
      std::stringstream parentName;
      parentName << "H" << mass << "/fitFunc_" << mass << "_" << flavour << "_" << parentMethods[iMeth] ;
      parents[iMeth] = (TF1*) ( inFile2->Get (parentName.str ().c_str ()));
    }
  
  
  
  
  
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
  ntuple->Fill  (n_parent_S,n_parent_B)  ;

  TH1F* h_B_toys[6] ;
  for (int iMeth = 0 ; iMeth < 6 ; ++iMeth)
    { 
      std::stringstream histoName;
      histoName << "h_B_toy_" << parentMethods[iMeth] ;
      h_B_toys[iMeth] = new TH1F (histoName.str ().c_str (),"",nBins,xMin,xMax);
      h_B_toys[iMeth] -> Sumw2();
      for(int bin = 1; bin <= h_B_toys[iMeth]->GetNbinsX(); ++bin)
      {
        float binCenter = h_B_toys[iMeth] -> GetBinCenter(bin);
        float binLowEdge = binCenter - 0.5 * xWidth;
        float binHigEdge = binCenter + 0.5 * xWidth;
        float binContent = parents[iMeth]->Integral(binLowEdge,binHigEdge) / xWidth;
        h_B_toys[iMeth] -> SetBinContent(bin,binContent);
        h_B_toys[iMeth] -> SetBinError(bin,sqrt(binContent/bkgStrength));
      }
    }
  
  double rangeStep = 40 ;
  TH2F h2_aveBias ("h2_aveBias", "", int (900./rangeStep) + 1, 100., 1000., int (900./rangeStep) + 1, 100., 1000.) ;
  TH2F h2_aveStat ("h2_aveStat", "", int (900./rangeStep) + 1, 100., 1000., int (900./rangeStep) + 1, 100., 1000.) ;
  TH2F h2_aveRati ("h2_aveRati", "", int (900./rangeStep) + 1, 100., 1000., int (900./rangeStep) + 1, 100., 1000.) ;
  //PG change the ranges
  for (double startFit = 180. ; startFit < 455. ; startFit += rangeStep)
    for (double endFit = startFit ; endFit < 1005. ; endFit += rangeStep)
      {
        double aveBias = 0. ; 
        double aveStat = 0. ; 
        //PG loop on the primitives
        for (int iMeth = 0 ; iMeth <= testingMethodsNum ; ++iMeth)
          {
            TF1* fitFunc;
            int fitBins = countBins (h_B_toys[iMeth], startFit, endFit) ;
            TFitResultPtr theFitResult = FitData (&fitFunc, "fitFunc_" + fitMethod,
                                                 h_B_toys[iMeth], mass, step, flavour, additionalCuts,
                                                 fitMethod, false, startFit, endFit) ;
              
            TH1F * theFitResult_band = new TH1F ("theFitResult_band", "", fitBins, startFit, endFit) ;
            (TVirtualFitter::GetFitter ())->GetConfidenceIntervals (theFitResult_band, 0.68) ;
            
            double statError = getTotError (theFitResult_band) ;
            aveStat += statError ;
            
            //PG qui devo controllare i range, chi sono? FIXME
            double n_toy_B = fitFunc->Integral (lepNuWMMIN, lepNuWMMAX) / 
                             fitFunc->Integral (xMin,xMax) * int (n_data_obs) ;
            aveBias += n_toy_B ;
            
            h_diffB_parentB_fitB -> Fill(n_toy_B/n_parent_B - 1.);
          
            delete theFitResult_band ;
          } //PG loop on the primitives
        aveStat /= testingMethodsNum ;
        aveBias /= testingMethodsNum ;
        h2_aveBias.Fill (startFit + 0.5 * rangeStep, endFit + 0.5 * rangeStep, aveBias) ;
        h2_aveStat.Fill (startFit + 0.5 * rangeStep, endFit + 0.5 * rangeStep, aveStat) ;
        h2_aveRati.Fill (startFit + 0.5 * rangeStep, endFit + 0.5 * rangeStep, aveBias / aveStat) ;
      
      } //PG change the ranges
  
  
  outFile -> cd();

  h2_aveBias.Write () ;
  h2_aveStat.Write () ;
  h2_aveRati.Write () ;
  
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
