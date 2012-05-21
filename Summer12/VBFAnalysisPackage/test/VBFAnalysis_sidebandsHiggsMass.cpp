#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "plotUtils.h"
#include "HiggsMassWindows.h"
#include "Functions.h"

#include <iostream>
#include <string>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TRandom3.h"



float GetXFitMIN1(const float& mH, const std::string& fitMethod = "", const int& step = -1);
float GetXFitMAX1(const float& mH, const std::string& fitMethod = "");

float GetXFitMIN2(const float& mH, const std::string& fitMethod = "");
float GetXFitMAX2(const float& mH, const std::string& fitMethod = "");

double correctionFunc(double* x, double* par);

void ComputeCorrectionHisto(TH1F** h_alphaFit, const std::string& histoName,
                            TH1F* h_sig, TH1F* h_sb,
                            TF1** f_sig, TF1** f_sb,
                            const double& xFitMIN, const double& xFitMAX);

void ComputeCorrectionHisto(TH1F** h_alphaHist, const std::string& histoName,
                            TH1F* h_sig, TH1F* h_sb);

void ApplyCorrectionFunc(TH1F** h_sigExtr, const std::string& histoName,
                         TH1F* h_alpha, TH1F* h_sb);

std::string varName = "lepNuW_m_KF";
std::string method = "";
std::string analysisMethod = "";

TH1F* h_alphaHisto;
TH1F* hlog_alphaHisto;
TH1F* h_alphaFit;

TF1* f_sig;
TF1* f_sb;







int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_sidebandsHiggsMass::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  float lumi = gConfigParser -> readFloatOption("Input::lumi");
  int higgsMass = gConfigParser -> readIntOption("Input::higgsMass");
  char higgsMassChar[50];
  sprintf(higgsMassChar,"%d",higgsMass);
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  std::vector<std::string> inputSigDirs  = gConfigParser -> readStringListOption("Input::inputSigDirs");
  std::vector<std::string> inputBkgDirs  = gConfigParser -> readStringListOption("Input::inputBkgDirs");
  std::vector<std::string> inputDataDirs = gConfigParser -> readStringListOption("Input::inputDataDirs");
  
  unsigned int nSigTrees  = inputSigDirs.size();
  unsigned int nBkgTrees  = inputBkgDirs.size();
  unsigned int nDataTrees = inputDataDirs.size();
  
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  
  //[Options]
  int step        = gConfigParser -> readIntOption("Options::step");
  method          = gConfigParser -> readStringOption("Options::method");
  analysisMethod  = gConfigParser -> readStringOption("Options::analysisMethod");
  
    
  //[Cuts]
  float lepNuWMMIN = GetLepNuWMMIN(higgsMass);
  float lepNuWMMAX = GetLepNuWMMAX(higgsMass);
  float xFitMIN = GetXFitMIN1(higgsMass,analysisMethod,step);
  float xFitMAX = GetXFitMAX2(higgsMass,analysisMethod);
  
  int nBins = 200;
  float xMin = 0.;
  float xMax = 1000.;
  float xWidth = (xMax-xMin)/nBins;
  
  int nLogBins = 100;
  float xLogMin = 1.;
  float xLogMax = 3.;
  float xLogWidth = (xLogMax-xLogMin)/nLogBins;
  
  int nBinsFit = int((xFitMAX-xFitMIN)/xWidth);
  
  int binMin = -1;
  int binMax = -1;
  int binFitMin = -1;
  int binFitMax = -1;
  for(int bin = 1; bin <= nBins; ++bin)
  {
    float binCenter = xMin + 0.5*xWidth + xWidth*(bin-1);
    
    if( (binCenter >= lepNuWMMIN) && (binMin == -1) ) binMin = bin;
    if( (binCenter >= lepNuWMMIN) && (binCenter < lepNuWMMAX) ) binMax = bin;
    if( (binCenter >= xFitMIN) && (binFitMin == -1) ) binFitMin = bin;
    if( (binCenter >= xFitMIN) && (binCenter < xFitMAX) ) binFitMax = bin;
  }
  //std::cout << "binMin: " << binMin << "   binMax: " << binMax << std::endl;
  
  
  
  
  
  
  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + "_H" + std::string(higgsMassChar) + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
  
  
  
  
  
  // define the regions
  std::string generalCut = " ( " + varName + " > 180. ) ";
  std::string sigRegion = "( (WJJ_m >= 65.) && (WJJ_m < 95.) )";
  std::string sbRegion = "( ( (WJJ_m >= 50.) && (WJJ_m < 65.) ) || ( (WJJ_m >= 95.) && (WJJ_m < 130.) ) )";
  
  
  
  // Define the histograms
  TH1F* h_data_sigRegion = new TH1F("h_data_sigRegion","",nBins,xMin,xMax);
  h_data_sigRegion -> Sumw2();
  TH1F* h_data_sbRegion = new TH1F("h_data_sbRegion","",nBins,xMin,xMax);
  h_data_sbRegion -> Sumw2();
  TH1F* h_data_sigRegionExtr;
  TH1F* h_data_sigRegionExtrFit;
  
  
  TH1F* h_mcSum_sigRegion = new TH1F("h_mcSum_sigRegion","",nBins,xMin,xMax);
  h_mcSum_sigRegion -> Sumw2();
  TH1F* h_mcSum_sbRegion = new TH1F("h_mcSum_sbRegion","",nBins,xMin,xMax);
  h_mcSum_sbRegion -> Sumw2();
  TH1F* h_mcSum_sigRegionExtr;
  TH1F* h_mcSum_sigRegionExtrFit;
  
  TH1F* hlog_mcSum_sigRegion = new TH1F("hlog_mcSum_sigRegion","",nLogBins,xLogMin,xLogMax);
  hlog_mcSum_sigRegion -> Sumw2();
  BinLogX(hlog_mcSum_sigRegion);
  TH1F* hlog_mcSum_sbRegion = new TH1F("hlog_mcSum_sbRegion","",nLogBins,xLogMin,xLogMax);
  hlog_mcSum_sbRegion -> Sumw2();
  BinLogX(hlog_mcSum_sbRegion);
  TH1F* hlog_mcSum_sbRegionExtr;
  
  
  
  
  
  
  //---------------------------------
  // ON DATA - FILL MASS DISTRIBUTION
  //---------------------------------
  
  // loop on the samples
  for(unsigned int i = 0; i < nDataTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    inputFullFileName = baseDir + "/" + inputDataDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << ">>>>>> VBFAnalysis_sidebandsHiggsMass::data tree in " << inputDataDirs.at(i) << " opened" << std::endl;
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue; 
    
    outFile -> cd();
    
    std::stringstream weight;
    weight << "( 1. )";
    
    std::string sigCut = weight.str() + " * " + sigRegion + " * " + generalCut;
    std::string sbCut = weight.str() + " * " + sbRegion + " * " + generalCut;
    
    tree -> Draw((varName+" >>+ h_data_sigRegion").c_str(),sigCut.c_str(),"goff");
    tree -> Draw((varName+" >>+ h_data_sbRegion").c_str(),sbCut.c_str(),"goff");
  }
  
  
  //-------------------------------
  // ON MC - FILL MASS DISTRIBUTION
  //-------------------------------  
  
  // loop on the samples
  for(unsigned int i = 0; i < nBkgTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    inputFullFileName = baseDir + "/" + inputBkgDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << ">>>>>> VBFAnalysis_sidebandsHiggsMass::mc tree in " << inputBkgDirs.at(i) << " opened" << std::endl;
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue; 
    
    outFile -> cd();
    
    std::stringstream weight;
    weight << "( 1000 * " << lumi << " * 1. * eventWeight / totEvents * crossSection * PURescaleFactor(PUit_n) )";
    
    std::string sigCut = weight.str() + " * " + sigRegion + " * " + generalCut;
    std::string sbCut = weight.str() + " * " + sbRegion + " * " + generalCut;
    
    tree -> Draw((varName+" >>+ h_mcSum_sigRegion").c_str(),   sigCut.c_str(),"goff"); 
    tree -> Draw((varName+" >>+ hlog_mcSum_sigRegion").c_str(),sigCut.c_str(),"goff"); 
    
    tree -> Draw((varName+" >>+ h_mcSum_sbRegion").c_str(),   sbCut.c_str(),"goff"); 
    tree -> Draw((varName+" >>+ hlog_mcSum_sbRegion").c_str(),sbCut.c_str(),"goff"); 
  }
  
  
  
  
  
  
  //----------------------------------
  // GET and APPLY CORRECTION FUNCTION
  //----------------------------------
  
  ComputeCorrectionHisto(&h_alphaHisto,"h_alphaHisto",
                         h_mcSum_sigRegion,h_mcSum_sbRegion);
  
  ComputeCorrectionHisto(&hlog_alphaHisto,"hlog_alphaHisto",
                         hlog_mcSum_sigRegion,hlog_mcSum_sbRegion);
  
  ComputeCorrectionHisto(&h_alphaFit,"h_alphaFit",
                         h_mcSum_sigRegion,h_mcSum_sbRegion,
                         &f_sig,&f_sb,
                         xFitMIN,xFitMAX);
  
  
  ApplyCorrectionFunc(&h_mcSum_sigRegionExtr,"h_mcSum_sigRegionExtr",
                      h_alphaFit,h_mcSum_sbRegion);
  
  ApplyCorrectionFunc(&h_data_sigRegionExtr,"h_data_sigRegionExtr",
                      h_alphaFit,h_data_sbRegion);
  
  
  
  
  
  
  //----------------
  // GET THE RESULTS
  //----------------
  
  // Print the results
  std::cout << "*******************************" << std::endl;
  std::cout << ">>> SIDEBAND RESULTS - MC <<<" << std::endl;
  std::cout << "*******************************" << std::endl;
  
  float N_observed  = h_mcSum_sigRegion -> Integral(binMin,binMax);
  float N_estimated = h_mcSum_sigRegionExtr -> Integral(binMin,binMax);
  float N_estimated_err = 0.;
  
  std::cout << "Number of events OBSERVED  in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_observed << std::endl;  
  std::cout << "Number of events ESTIMATED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_estimated << " +/- " << N_estimated_err << std::endl;
  
  
  
  std::cout << "*******************************" << std::endl;
  std::cout << ">>> SIDEBAND RESULTS - DATA <<<" << std::endl;
  std::cout << "*******************************" << std::endl;
  
  N_observed  = h_data_sigRegion -> Integral(binMin,binMax);
  N_estimated = h_data_sigRegionExtr -> Integral(binMin,binMax);
  N_estimated_err = 0.;
  
  std::cout << "Number of events OBSERVED  in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_observed << std::endl;  
  std::cout << "Number of events ESTIMATED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_estimated << " +/- " << N_estimated_err << std::endl;
  
  
  
  
  
  
  //-----------------
  // Save the results
  
  outFile -> cd();
  
  h_data_sigRegion -> Write();
  h_data_sbRegion -> Write();
  h_data_sigRegionExtr -> Write();
  
  h_mcSum_sigRegion -> Write();
  h_mcSum_sbRegion -> Write();
  h_mcSum_sigRegionExtr -> Write();
  
  h_alphaHisto -> Write();
  hlog_alphaHisto -> Write();
  h_alphaFit -> Write();
  
  f_sig -> Write();
  f_sb -> Write();
  
  outFile -> Close();
  
  
  
  return 0;
}






double correctionFunc(double* x, double* par)
{
  return 1. * f_sig->Eval(x[0]) / f_sb->Eval(x[0]);
}



void ComputeCorrectionHisto(TH1F** h_alphaHisto, const std::string& histoName,
                            TH1F* h_sig, TH1F* h_sb)
{
  std::cout << ">>>VBFAnalysis_sidebandsHiggsMass::ComputeCorrectionHisto " << histoName << std::endl;
  
  (*h_alphaHisto) = (TH1F*)( h_sig->Clone(histoName.c_str()) );
  (*h_alphaHisto) -> Divide(h_sb);
  (*h_alphaHisto) -> SetMarkerStyle(20);
  (*h_alphaHisto) -> SetMarkerColor(kBlue);
}  



void ComputeCorrectionHisto(TH1F** h_alphaFit, const std::string& histoName,
                            TH1F* h_sig, TH1F* h_sb,
                            TF1** f_sig, TF1** f_sb,
                            const double& xFitMIN, const double& xFitMAX)
{
  std::cout << ">>>VBFAnalysis_sidebandsHiggsMass::ComputeCorrectionHisto " << histoName << std::endl;
  
  int nPar = -1;
  
  if(method == "doubleExponential" )
  {
    nPar = 4;
    (*f_sig) = new TF1("f_sig",doubleExponential,0.,1000.,nPar);
    (*f_sb)  = new TF1("f_sb", doubleExponential,0.,1000.,nPar);
  }
  if(method == "attenuatedExponential" )
  {
    nPar = 4;
    (*f_sig) = new TF1("f_sig",attenuatedExponential,0.,1000.,nPar);
    (*f_sb)  = new TF1("f_sb", attenuatedExponential,0.,1000.,nPar);
  }
  if(method == "attenuatedDoubleExponential" )
  {
    nPar = 6;
    (*f_sig) = new TF1("f_sig",attenuatedDoubleExponential,0.,1000.,nPar);
    (*f_sb)  = new TF1("f_sb", attenuatedDoubleExponential,0.,1000.,nPar);
  }
  
  
  
  if( method == "doubleExponential" )
  {
    //-------------------
    // double exponential
    
    (*f_sig) -> SetParameter(4,10.);
    (*f_sig) -> SetParameter(5,0.012);
    (*f_sig) -> SetParameter(6,5.);
    (*f_sig) -> SetParameter(7,0.005);
    (*f_sig) -> SetParName(4,"N1");
    (*f_sig) -> SetParName(5,"#lambda1");
    (*f_sig) -> SetParName(6,"N2");
    (*f_sig) -> SetParName(7,"#lambda2");
    
    (*f_sb) -> SetParameter(4,10.);
    (*f_sb) -> SetParameter(5,0.012);
    (*f_sb) -> SetParameter(6,5.);
    (*f_sb) -> SetParameter(7,0.005);
    (*f_sb) -> SetParName(4,"N1");
    (*f_sb) -> SetParName(5,"#lambda1");
    (*f_sb) -> SetParName(6,"N2");
    (*f_sb) -> SetParName(7,"#lambda2");
  }
  
  if( method == "attenuatedExponential" )
  {
    //-----------------------
    // attenuated exponential
    
    (*f_sig) -> SetParameter(4,180.);
    (*f_sig) -> SetParameter(5,11.);
    (*f_sig) -> SetParameter(6,10.);
    (*f_sig) -> SetParameter(7,0.012);
    (*f_sig) -> SetParLimits(4,150.,250.);
    (*f_sig) -> SetParLimits(5,0.,100.);
    (*f_sig) -> SetParName(4,"#mu");
    (*f_sig) -> SetParName(5,"kT");
    (*f_sig) -> SetParName(6,"N1");
    (*f_sig) -> SetParName(7,"#lambda1");
    
    (*f_sb) -> SetParameter(4,180.);
    (*f_sb) -> SetParameter(5,11.);
    (*f_sb) -> SetParameter(6,10.);
    (*f_sb) -> SetParameter(7,0.012);
    (*f_sb) -> SetParLimits(4,150.,250.);
    (*f_sb) -> SetParLimits(5,0.,100.);
    (*f_sb) -> SetParName(4,"#mu");
    (*f_sb) -> SetParName(5,"kT");
    (*f_sb) -> SetParName(6,"N1");
    (*f_sb) -> SetParName(7,"#lambda1");
  }
  
  if( method == "attenuatedDoubleExponential" )
  {
    //------------------------------
    // attenuated double exponential
    
    (*f_sig) -> SetParameter(4,180.);
    (*f_sig) -> SetParameter(5,11.);
    (*f_sig) -> SetParameter(6,10.);
    (*f_sig) -> SetParameter(7,0.012);
    (*f_sig) -> SetParameter(8,5.);
    (*f_sig) -> SetParameter(9,0.005);
    (*f_sig) -> SetParLimits(4,150.,250.);
    (*f_sig) -> SetParLimits(5,0.,100.);
    (*f_sig) -> SetParName(4,"#mu");
    (*f_sig) -> SetParName(5,"kT");
    (*f_sig) -> SetParName(6,"N1");
    (*f_sig) -> SetParName(7,"#lambda1");
    (*f_sig) -> SetParName(8,"N2");
    (*f_sig) -> SetParName(9,"#lambda2");
    
    (*f_sb) -> SetParameter(4,180.);
    (*f_sb) -> SetParameter(5,11.);
    (*f_sb) -> SetParameter(6,10.);
    (*f_sb) -> SetParameter(7,0.012);
    (*f_sb) -> SetParameter(8,5.);
    (*f_sb) -> SetParameter(9,0.005);
    (*f_sb) -> SetParLimits(4,150.,250.);
    (*f_sb) -> SetParLimits(5,0.,100.);
    (*f_sb) -> SetParName(4,"#mu");
    (*f_sb) -> SetParName(5,"kT");
    (*f_sb) -> SetParName(6,"N1");
    (*f_sb) -> SetParName(7,"#lambda1");
    (*f_sb) -> SetParName(8,"N2");
    (*f_sb) -> SetParName(9,"#lambda2");
  }

  
  
  (*f_sig) -> SetNpx(10000);
  (*f_sig) -> SetLineWidth(2);
  (*f_sig) -> SetLineColor(kRed);
  
  (*f_sb) -> SetNpx(10000);
  (*f_sb) -> SetLineWidth(2);
  (*f_sb) -> SetLineColor(kRed);
  
  
  
  TFitResultPtr fitResultPtr_sig = h_sig -> Fit("f_sig","NQLRS+","",xFitMIN,xFitMAX);
  int fitStatus_sig = (int)(fitResultPtr_sig);
  int counter_sig = 0;
  if( (fitStatus_sig != 0) && (counter_sig < 100) )
  {
    fitResultPtr_sig = h_sig -> Fit("f_sig","NQLRS+","",xFitMIN,xFitMAX);
    fitStatus_sig = (int)(fitResultPtr_sig);
    ++counter_sig;
  }
  
  TH1F* h_sigFit = (TH1F*)( h_sig->Clone("h_sigFit") );
  (TVirtualFitter::GetFitter()) -> GetConfidenceIntervals(h_sigFit,0.68);
  TH1F* h_sigFitClone = (TH1F*)( h_sigFit->Clone("h_sigFitClone") );
  h_sigFitClone -> Reset();
  for(int bin = 1; bin <= h_sigFit->GetNbinsX(); ++bin)
  {
    float binCenter  = h_sigFit -> GetBinCenter(bin);
    float binContent = h_sigFit -> GetBinContent(bin);
    float binError   = h_sigFit -> GetBinError(bin);
    
    if( binCenter > 180.)
    {
      h_sigFitClone -> SetBinContent(bin,binContent);
      h_sigFitClone -> SetBinError(bin,binError);
    }
  }
  
  
  
  TFitResultPtr fitResultPtr_sb = h_sb -> Fit("f_sb","NQLRS+","",xFitMIN,xFitMAX);
  int fitStatus_sb = (int)(fitResultPtr_sb);
  int counter_sb = 0;
  if( (fitStatus_sb != 0) && (counter_sb < 100) )
  {
    fitResultPtr_sb = h_sb -> Fit("f_sb","NQLRS+","",xFitMIN,xFitMAX);
    fitStatus_sb = (int)(fitResultPtr_sb);
    ++counter_sb;
  }
  
  TH1F* h_sbFit = (TH1F*)( h_sb->Clone("h_sbFit") );
  (TVirtualFitter::GetFitter()) -> GetConfidenceIntervals(h_sbFit,0.68);
  TH1F* h_sbFitClone = (TH1F*)( h_sbFit->Clone("h_sbFitClone") );
  h_sbFitClone -> Reset();
  for(int bin = 1; bin <= h_sbFit->GetNbinsX(); ++bin)
  {
    float binCenter  = h_sbFit -> GetBinCenter(bin);
    float binContent = h_sbFit -> GetBinContent(bin);
    float binError   = h_sbFit -> GetBinError(bin);
    
    if( binCenter > 180.)
    {
      h_sbFitClone -> SetBinContent(bin,binContent);
      h_sbFitClone -> SetBinError(bin,binError);
    }
  }  
  
  
  
  (*h_alphaFit) = (TH1F*)( h_sigFitClone->Clone("h_alphaFit") );
  (*h_alphaFit) -> Divide(h_sbFitClone);
}




void ApplyCorrectionFunc(TH1F** h_sigExtr, const std::string& histoName,
                         TH1F* h_alpha, TH1F* h_sb)
{
  (*h_sigExtr) = (TH1F*)( h_sb->Clone(histoName.c_str()) );
  (*h_sigExtr) -> Multiply(h_alpha);
}






float GetXFitMIN1(const float& mH, const std::string& fitMethod, const int& step)
{
  if( (fitMethod == "exponential") || (fitMethod == "exponentialNoHoles") ||
      (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") )
    {
      if     ( mH == 200. ) return 180.;
      if     ( mH == 250. ) return 180.;
      else if( mH == 300. ) return 180.;
      else if( mH == 350. ) return 180.;
      else if( mH == 400. ) return 180.;
      else if( mH == 450. ) return 180.;
      else if( mH == 500. ) return 180.;
      else if( mH == 550. ) return 180.;
      else if( mH == 600. ) return 180.;
      else return 1.;
    }
  
  else if( (fitMethod == "attenuatedExponential") || (fitMethod == "attenuatedExponentialNoHoles") ||
           (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
    {
      if     ( mH == 200. ) return 180.;
      if     ( mH == 250. ) return 180.;
      else if( mH == 300. ) return 180.;
      else if( mH == 350. ) return 180.;
      else if( mH == 400. ) return 180.;
      else if( mH == 450. ) return 180.;
      else if( mH == 500. ) return 180.;
      else if( mH == 550. ) return 180.;
      else if( mH == 600. ) return 180.;
      else return 1.;
    }
  
  else return -1.;
}

float GetXFitMAX1(const float& mH, const std::string& fitMethod)
{
  if( (fitMethod == "doubleExponentialNoHoles") ||
      (fitMethod == "attenuatedExponentialNoHoles") ||
      (fitMethod == "attenuatedDoubleExponentialNoHoles") )
    {
      return -1.;
    }
  
  else
    {
      if     ( mH == 200. ) return GetLepNuWMMIN(200.);
      if     ( mH == 250. ) return GetLepNuWMMIN(250.);
      else if( mH == 300. ) return GetLepNuWMMIN(300.);
      else if( mH == 350. ) return GetLepNuWMMIN(350.);
      else if( mH == 400. ) return GetLepNuWMMIN(400.);
      else if( mH == 450. ) return GetLepNuWMMIN(450.);
      else if( mH == 500. ) return GetLepNuWMMIN(500.);
      else if( mH == 550. ) return GetLepNuWMMIN(550.);
      else if( mH == 600. ) return GetLepNuWMMIN(600.);
      else return -1.;
    }
}






float GetXFitMIN2(const float& mH, const std::string& fitMethod)
{
  if( (fitMethod == "doubleExponentialNoHoles") ||
      (fitMethod == "attenuatedExponentialNoHoles") ||
      (fitMethod == "attenuatedDoubleExponentialNoHoles") )
    {
      return -1.;
    }
  
  else
    {
      if     ( mH == 200. ) return GetLepNuWMMAX(200.);
      if     ( mH == 250. ) return GetLepNuWMMAX(250.);
      else if( mH == 300. ) return GetLepNuWMMAX(300.);
      else if( mH == 350. ) return GetLepNuWMMAX(350.);
      else if( mH == 400. ) return GetLepNuWMMAX(400.);
      else if( mH == 450. ) return GetLepNuWMMAX(450.);
      else if( mH == 500. ) return GetLepNuWMMAX(500.);
      else if( mH == 550. ) return GetLepNuWMMAX(550.);
      else if( mH == 600. ) return GetLepNuWMMAX(600.);
      else return -1.;
    }
}

float GetXFitMAX2(const float& mH, const std::string& fitMethod)
{
  if     ( mH == 200. ) return 800.;
  if     ( mH == 250. ) return 800.;
  else if( mH == 300. ) return 800.;
  else if( mH == 350. ) return 800.;
  else if( mH == 400. ) return 800.;
  else if( mH == 450. ) return 800.;
  else if( mH == 500. ) return 800.;
  else if( mH == 550. ) return 800.;
  else if( mH == 600. ) return 800.;
  else return -1.;
}
