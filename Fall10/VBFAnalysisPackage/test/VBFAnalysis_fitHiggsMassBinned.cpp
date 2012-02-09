#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "Functions.h"
#include "HiggsMassWindows.h"

#include <iostream>

#include "TH1F.h"
#include "TF1.h"
#include "TGraphAsymmErrors.h"
#include "TVirtualFitter.h"
#include "TRandom3.h"
#include "TDirectory.h"



float GetXFitMIN1(const float& mH, const std::string& fitMethod = "", const int& step = -1);
float GetXFitMAX1(const float& mH, const std::string& fitMethod = "");

float GetXFitMIN2(const float& mH, const std::string& fitMethod = "");
float GetXFitMAX2(const float& mH, const std::string& fitMethod = "");

double fitFunc(double* x, double* par);

int fitHiggsMassBinned(TH1F* h_lepNuW_m,
                       float xFitMIN1, float xFitMAX1, float xFitMIN2, float xFitMAX2,
                       TF1** func, bool computeCL = false, TH1F* hint = NULL);

void DrawPulls(TDirectory* outFile, TH1F* num, TH1F* den, const int& higgsMass, const std::string& fitMethod, const int& step);

std::string fitMethod = "";
std::string varName = "lepNuW_m_KF"; 
std::string WMassCut;






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_fitHiggsMassBinned::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir = gConfigParser -> readStringOption("Input::baseDir");
  float lumi    = gConfigParser -> readFloatOption("Input::lumi");
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
  float xWidth = gConfigParser -> readFloatOption("Options::xWidth");
  char xWidthChar[50];
  sprintf(xWidthChar,"%d",int(xWidth));
  
  int step = gConfigParser -> readIntOption("Options::step");
  char stepChar[50];
  sprintf(stepChar,"%d",step);
  
  std::string flavour = gConfigParser -> readStringOption("Options::flavour");
  if( flavour == "e" )
  {
    WMassCut += " * (lep_flavour == 11)";
  }
  if( flavour == "mu" )
  {
    WMassCut += " * (lep_flavour == 13)";
  }
  
  int onSidebands = gConfigParser -> readIntOption("Options::onSidebands");
  

  int onData = gConfigParser -> readIntOption("Options::onData");
  int onFake = gConfigParser -> readIntOption("Options::onFake");
  int onMC   = gConfigParser -> readIntOption("Options::onMC");
  int toyMAX = gConfigParser -> readIntOption("Options::toyMAX");
  fitMethod  = gConfigParser -> readStringOption("Options::fitMethod");
  
  //[Cuts]
  float lepNuWMMIN = GetLepNuWMMIN(higgsMass);
  float lepNuWMMAX = GetLepNuWMMAX(higgsMass);
  float xFitMIN1 = GetXFitMIN1(higgsMass,fitMethod,step);
  float xFitMAX1 = GetXFitMAX1(higgsMass,fitMethod);
  float xFitMIN2 = GetXFitMIN2(higgsMass,fitMethod);
  float xFitMAX2 = GetXFitMAX2(higgsMass,fitMethod);
  
  float xMin = 0.;
  float xMax = 1000.;
  int nBins = int((xMax-xMin)/xWidth);
  xMax = xMin + xWidth*nBins;
  
  int binMin = -1;
  int binMax = -1;
  int binFitMin1 = -1;
  int binFitMax1 = -1;
  int binFitMin2 = -1;
  int binFitMax2 = -1;
  for(int bin = 1; bin <= nBins; ++bin)
  {
    float binCenter = xMin + 0.5*xWidth + xWidth*(bin-1);
    
    if( (binCenter >= lepNuWMMIN) && (binMin == -1) ) binMin = bin;
    if( (binCenter >= lepNuWMMIN) && (binCenter < lepNuWMMAX) ) binMax = bin;
    if( (binCenter >= xFitMIN1) && (binFitMin1 == -1) ) binFitMin1 = bin;
    if( (binCenter >= xFitMIN1) && (binCenter < xFitMAX1) ) binFitMax1 = bin;
    if( (binCenter >= xFitMIN2) && (binFitMin2 == -1) ) binFitMin2 = bin;
    if( (binCenter >= xFitMIN2) && (binCenter < xFitMAX2) ) binFitMax2 = bin;
  }
  //std::cout << "binMin: " << binMin << "   binMax: " << binMax << std::endl;
  std::cout << "nBins: " << nBins << "   bin width: " << xWidth << std::endl;
  std::cout << "   xFitMIN1: " << xFitMIN1 << std::endl;
  std::cout << "   xFitMAX1: " << xFitMAX1 << std::endl;
  std::cout << "   xFitMIN2: " << xFitMIN2 << std::endl;
  std::cout << "   xFitMAX2: " << xFitMAX2 << std::endl;

  if( onSidebands == 0 )
    WMassCut = "( (WJJ_m >= 65.) && (WJJ_m < 100.) )";
  else
    WMassCut = "( ( (WJJ_m >= 55.) && (WJJ_m < 65.) ) || ( (WJJ_m >= 100.) && (WJJ_m < 120.) ) )";
  
  
  
  // Define the output file
  if( onSidebands == 0 )
    outputRootFilePath += "/combine_signal/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
  else
    outputRootFilePath += "/combine_sidebands/binWidth" + std::string(xWidthChar) + "/step" + std::string(stepChar) + "/";
  std::string outputRootFullFileName;
  outputRootFullFileName = outputRootFilePath + outputRootFileName + "_" + fitMethod + "_H" + std::string(higgsMassChar) + "_" + flavour + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  TDirectory* dataDir    = outFile -> mkdir("data");
  TDirectory* dataSigDir = outFile -> mkdir("dataSig");
  TDirectory* fakeDir = outFile -> mkdir("fake");
  TDirectory* MCDir   = outFile -> mkdir("MC");
  
  
  
  
  
  
  // Define the histograms
  TH1F* h_data_lepNuW_m = new TH1F("h_data_lepNuW_m","",nBins,xMin,xMax);
  h_data_lepNuW_m -> Sumw2();
  TH1F* h_dataSig_lepNuW_m = new TH1F("h_dataSig_lepNuW_m","",nBins,xMin,xMax);
  h_dataSig_lepNuW_m -> Sumw2();
  
  TH1F* h_fake_lepNuW_m = new TH1F("h_fake_lepNuW_m","",nBins,xMin,xMax);
  h_fake_lepNuW_m -> Sumw2();

  TH1F* h_mcSum_lepNuW_m = new TH1F("h_mcSum_lepNuW_m","",nBins,xMin,xMax);
  h_mcSum_lepNuW_m -> Sumw2();
  TH1F* h_sig_lepNuW_m = new TH1F("h_sig_lepNuW_m","",nBins,xMin,xMax);
  h_sig_lepNuW_m -> Sumw2();
  
  TH1F* h_toy_lepNuW_m = new TH1F("h_toy_lepNuW_m","",nBins,xMin,xMax);
  h_toy_lepNuW_m -> Sumw2();
  TH1F* h_toy_lepNuW_m_sig = new TH1F("h_toy_lepNuW_m_sig","",nBins,xMin,xMax);
  h_toy_lepNuW_m_sig -> Sumw2();
  
  TH1F* h_mcSumToSample_lepNuW_m;
  TH1F* h_sigToSample_lepNuW_m;
  
  TF1* func;
  TF1* func_sig;
  TF1* func_toSample;
    
  TH1F* hint = new TH1F("hint","",nBins,xMin,xMax);
  TH1F* hint_sig = new TH1F("hint_sig","",nBins,xMin,xMax);
  
  TGraphAsymmErrors* pull;
  TGraphAsymmErrors* pull_sig;
  
  TH1F* h_chi2     = new TH1F("h_chi2","",     100,    0.,   3.);
  TH1F* h_diff_obs = new TH1F("h_diff_obs","",2000,-1000.,1000.);
  TH1F* h_res_obs  = new TH1F("h_res_obs", "", 500,   -1.,   1.);
  TH1F* h_diff_est = new TH1F("h_diff_est","",2000,-1000.,1000.);
  TH1F* h_res_est  = new TH1F("h_res_est", "", 500,   -1.,   1.);
  TH1F* h_err_est  = new TH1F("h_err_est", "",2000,    0.,2000.);
  
  
  
  // Define the output tree
  TTree* outTree_data    = new TTree("ntu_data",   "ntu_data");
  TTree* outTree_dataSig = new TTree("ntu_dataSig","ntu_dataSig");
  TTree* outTree_fake    = new TTree("ntu_fake",   "ntu_fake");
  TTree* outTree_MC      = new TTree("ntu_MC",     "ntu_MC");
  
  int toyIt;
  
  int fitStatus;
  int fitStatus_sig;
  
  float chi2;
  float chi2_sig;
  
  int nPar = -1;
  if( (fitMethod == "exponential") || (fitMethod == "exponentialNoHoles") ) nPar = 2;
  if( (fitMethod == "attenuatedExponential") || (fitMethod == "attenuatedExponentialNoHoles") ) nPar = 4;
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") ) nPar = 4;
  if( (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") ) nPar = 6;
  
  float* par = new float[nPar];
  float* parErr = new float[nPar];
  
  float N_expected;
  float N_expected_err;
  float N_estimated;
  float N_estimated_err;
  float N_observed;
  float N_expected_sig;
  float N_expected_err_sig;
  float N_estimated_sig;
  float N_estimated_err_sig;
  float N_observed_sig;
  
  
  // data tree
  outTree_data -> Branch("xFitMIN1",   &xFitMIN1,      "xFitMIN1/F");
  outTree_data -> Branch("xFitMAX1",   &xFitMAX1,      "xFitMAX1/F");
  outTree_data -> Branch("xFitMIN2",   &xFitMIN2,      "xFitMIN2/F");
  outTree_data -> Branch("xFitMAX2",   &xFitMAX2,      "xFitMAX2/F");
  outTree_data -> Branch("lepNuWMMIN",&lepNuWMMIN,"lepNuWMMIN/F");
  outTree_data -> Branch("lepNuWMMAX",&lepNuWMMAX,"lepNuWMMAX/F");
  
  outTree_data -> Branch("fitStatus",&fitStatus,"fitStatus/I");
  outTree_data -> Branch("chi2",     &chi2,          "chi2/F");
  outTree_data -> Branch("nPar",     &nPar,          "nPar/I");
  outTree_data -> Branch("par",       par,      "par[nPar]/F");
  outTree_data -> Branch("parErr",    parErr,"parErr[nPar]/F");
  
  outTree_data -> Branch("N_estimated",    &N_estimated,        "N_estimated/F");
  outTree_data -> Branch("N_estimated_err",&N_estimated_err,"N_estimated_err/F");
  outTree_data -> Branch("N_observed",     &N_observed,          "N_observed/F");
  
  // dataSig tree
  outTree_dataSig -> Branch("xFitMIN1",   &xFitMIN1,      "xFitMIN1/F");
  outTree_dataSig -> Branch("xFitMAX1",   &xFitMAX1,      "xFitMAX1/F");
  outTree_dataSig -> Branch("xFitMIN2",   &xFitMIN2,      "xFitMIN2/F");
  outTree_dataSig -> Branch("xFitMAX2",   &xFitMAX2,      "xFitMAX2/F");
  outTree_dataSig -> Branch("lepNuWMMIN",&lepNuWMMIN,"lepNuWMMIN/F");
  outTree_dataSig -> Branch("lepNuWMMAX",&lepNuWMMAX,"lepNuWMMAX/F");
  
  outTree_dataSig -> Branch("fitStatus",&fitStatus,"fitStatus/I");
  outTree_dataSig -> Branch("chi2",     &chi2,          "chi2/F");
  outTree_dataSig -> Branch("nPar",     &nPar,          "nPar/I");
  outTree_dataSig -> Branch("par",       par,      "par[nPar]/F");
  outTree_dataSig -> Branch("parErr",    parErr,"parErr[nPar]/F");
  
  outTree_dataSig -> Branch("N_estimated",    &N_estimated,        "N_estimated/F");
  outTree_dataSig -> Branch("N_estimated_err",&N_estimated_err,"N_estimated_err/F");
  outTree_dataSig -> Branch("N_observed",     &N_observed,          "N_observed/F");
  
  // fake tree
  outTree_fake -> Branch("xFitMIN1",   &xFitMIN1,      "xFitMIN1/F");
  outTree_fake -> Branch("xFitMAX1",   &xFitMAX1,      "xFitMAX1/F");
  outTree_fake -> Branch("xFitMIN2",   &xFitMIN2,      "xFitMIN2/F");
  outTree_fake -> Branch("xFitMAX2",   &xFitMAX2,      "xFitMAX2/F");
  outTree_fake -> Branch("lepNuWMMIN",&lepNuWMMIN,"lepNuWMMIN/F");
  outTree_fake -> Branch("lepNuWMMAX",&lepNuWMMAX,"lepNuWMMAX/F");
  
  outTree_fake -> Branch("fitStatus",&fitStatus,"fitStatus/I");
  outTree_fake -> Branch("chi2",     &chi2,          "chi2/F");
  outTree_fake -> Branch("nPar",     &nPar,          "nPar/I");
  outTree_fake -> Branch("par",       par,      "par[nPar]/F");
  outTree_fake -> Branch("parErr",    parErr,"parErr[nPar]/F");
  
  outTree_fake -> Branch("N_estimated",    &N_estimated,        "N_estimated/F");
  outTree_fake -> Branch("N_estimated_err",&N_estimated_err,"N_estimated_err/F");
  outTree_fake -> Branch("N_observed",     &N_observed,          "N_observed/F");
  
  // MC tree
  outTree_MC -> Branch("xFitMIN1",   &xFitMIN1,      "xFitMIN1/F");
  outTree_MC -> Branch("xFitMAX1",   &xFitMAX1,      "xFitMAX1/F");
  outTree_MC -> Branch("xFitMIN2",   &xFitMIN2,      "xFitMIN2/F");
  outTree_MC -> Branch("xFitMAX2",   &xFitMAX2,      "xFitMAX2/F");
  outTree_MC -> Branch("lepNuWMMIN",&lepNuWMMIN,"lepNuWMMIN/F");
  outTree_MC -> Branch("lepNuWMMAX",&lepNuWMMAX,"lepNuWMMAX/F");
  
  outTree_MC -> Branch("toyIt",&toyIt,"toyIt/I");
  
  outTree_MC -> Branch("fitStatus",    &fitStatus,        "fitStatus/I");
  outTree_MC -> Branch("fitStatus_sig",&fitStatus_sig,"fitStatus_sig/I");
  
  outTree_MC -> Branch("chi2",    &chi2,        "chi2/F");
  outTree_MC -> Branch("chi2_sig",&chi2_sig,"chi2_sig/F");
  
  outTree_MC -> Branch("N_expected",     &N_expected,          "N_expected/F");
  outTree_MC -> Branch("N_expected_err", &N_expected_err,"  N_expected_err/F");
  outTree_MC -> Branch("N_estimated",    &N_estimated,        "N_estimated/F");
  outTree_MC -> Branch("N_estimated_err",&N_estimated_err,"N_estimated_err/F");
  outTree_MC -> Branch("N_observed",     &N_observed,          "N_observed/F");
  outTree_MC -> Branch("N_expected_sig",     &N_expected_sig,          "N_expected_sig/F");
  outTree_MC -> Branch("N_expected_err_sig", &N_expected_err_sig,"  N_expected_err_sig/F");
  outTree_MC -> Branch("N_estimated_sig",    &N_estimated_sig,        "N_estimated_sig/F");
  outTree_MC -> Branch("N_estimated_err_sig",&N_estimated_err_sig,"N_estimated_err_sig/F");
  outTree_MC -> Branch("N_observed_sig",     &N_observed_sig,          "N_observed_sig/F");
  
  
  
  
  
    
  //---------------------------------
  // ON DATA - FILL MASS DISTRIBUTION
  //---------------------------------
  
  if( onData == 1 )
  {
    // loop on the samples
    for(unsigned int i = 0; i < nDataTrees; ++i)
    {
      // open root file
      std::string inputFullFileName;
      inputFullFileName = baseDir + "/" + inputDataDirs.at(i)   + "/" + inputFileName + ".root";
      TFile* inputFile = TFile::Open(inputFullFileName.c_str());
      std::cout << ">>>>>> VBFAnalysis_fitHiggsMassBinned::data tree in " << inputDataDirs.at(i) << " opened" << std::endl;
      
      
      // get the tree at nth step
      TTree* tree = NULL;
      char treeName[50];
      sprintf(treeName, "ntu_%d", step);
      inputFile -> GetObject(treeName, tree);
      if ( tree -> GetEntries() == 0 ) continue; 
      
      
      // fill histogram
      outFile -> cd();
      std::stringstream weight;
      weight << "( 1. )";
      //weight << "( 1. ) * ( (lepNuW_m_KF < " << lepNuWMMIN << ") || ( lepNuW_m_KF >= " << lepNuWMMAX << ") )";
      std::string extendedCut = weight.str() + " * " + WMassCut;      
      
      tree -> Draw((varName+" >>+ h_data_lepNuW_m").c_str(),   extendedCut.c_str(),"goff");
      tree -> Draw((varName+" >>+ h_dataSig_lepNuW_m").c_str(),extendedCut.c_str(),"goff");
    }
  }
  
  
  
  //--------
  // ON FAKE
  //--------
  
  if( onFake == 1 )
  {
    fitHiggsMassBinned(h_data_lepNuW_m,
                       xFitMIN1,-1.,-1.,xFitMAX2,
                       &func_toSample);
    
    TH1F* temp = (TH1F*)( h_data_lepNuW_m->Clone("temp") );
    temp -> Reset();
    
    int N = int(h_data_lepNuW_m->Integral(binFitMin1,binFitMax2));
    
    int j = 0;
    float bkgStrength = 1.;
    while( j < bkgStrength*N )
    {
      double lepNuW_m = func_toSample -> GetRandom();
      
      if( (lepNuW_m >= xFitMIN1) && (lepNuW_m < xFitMAX2) )
      {
        h_fake_lepNuW_m -> Fill(lepNuW_m,1./bkgStrength);
        ++j;
      }
    }
  }
  
  
  //-------------------------------
  // ON MC - FILL MASS DISTRIBUTION
  //-------------------------------
  
  if( onMC == 1 )
  {
    /*
    // loop on bkg samples
    for(unsigned int i = 0; i < nBkgTrees; ++i)
    {
      // open root file
      std::string inputFullFileName;
      inputFullFileName = baseDir + "/" + inputBkgDirs.at(i)   + "/" + inputFileName + ".root";
      TFile* inputFile = TFile::Open(inputFullFileName.c_str());
      std::cout << ">>>>>> VBFAnalysis_fitHiggsMassBinned::bkg tree in " << inputBkgDirs.at(i) << " opened" << std::endl;
      
      
      // get the tree at nth step
      TTree* tree = NULL;
      char treeName[50];
      sprintf(treeName, "ntu_%d", step);
      inputFile -> GetObject(treeName, tree);
      if ( tree -> GetEntries() == 0 ) continue; 
      
      
      // fill histogram
      outFile -> cd();
      std::stringstream weight;
      weight << "( 1000 * " << lumi << " * 1. / totEvents * crossSection * eventWeight * PUWeight )";
      std::string extendedCut = weight.str() + " * " + WMassCut;      
      
      tree -> Draw((varName+" >>+ h_mcSum_lepNuW_m").c_str(),extendedCut.c_str(),"goff");
    }
    */
    
    // loop on sig samples
    for(unsigned int i = 0; i < nSigTrees; ++i)
    {
      // open root file
      std::string inputFullFileName;
      inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
      TFile* inputFile = TFile::Open(inputFullFileName.c_str());
      std::cout << ">>>>>> VBFAnalysis_fitHiggsMassBinned::sig tree in " << inputSigDirs.at(i) << " opened" << std::endl;
      
      
      // get the tree at nth step
      TTree* tree = NULL;
      char treeName[50];
      sprintf(treeName, "ntu_%d", step);
      inputFile -> GetObject(treeName, tree);
      if ( tree -> GetEntries() == 0 ) continue; 
      
      
      // fill histogram
      outFile -> cd();
      std::stringstream weight;
      weight << "( 1000 * " << lumi << " * 1. * eventWeight / totEvents * crossSection * PUWeight )";
      std::string extendedCut = weight.str() + " * " + WMassCut;      
      
      tree -> Draw((varName+" >>+ h_sig_lepNuW_m").c_str(),extendedCut.c_str(),"goff");
      tree -> Draw((varName+" >>+ h_dataSig_lepNuW_m").c_str(),extendedCut.c_str(),"goff");
    }
  }
  
  
  
  
  
  
  //--------------------------------
  // ON DATA - FIT MASS DISTRIBUTION
  //--------------------------------
  
  if( onData == 1 )
  {
    // Do the fit
    fitStatus = fitHiggsMassBinned(h_data_lepNuW_m,
                                   xFitMIN1,xFitMAX1,xFitMIN2,xFitMAX2,
                                   &func,true,hint);
    
    
    // Get the error bands
    float bandIntegral = 0;
    for(int bin = 1; bin <= hint->GetNbinsX(); ++bin)
    {
      float thisX = hint -> GetBinCenter(bin);
      if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
        bandIntegral += hint -> GetBinError(bin);
    }
    
    
    chi2 = func->GetChisquare()/func->GetNDF();
    
    for(int iPar = 0; iPar < nPar; ++iPar)
    {
      par[iPar] = func -> GetParameter(4+iPar);
      parErr[iPar] = func -> GetParError(4+iPar);
    }
    
    
    // Print the results
    std::cout << "**************************" << std::endl;
    std::cout << ">>> FIT RESULTS - DATA <<<" << std::endl;
    std::cout << "**************************" << std::endl;
    
    N_observed  = h_data_lepNuW_m->Integral(binMin,binMax);
    N_estimated = hint->Integral(binMin,binMax);
    N_estimated_err = bandIntegral;
    
    std::cout << "Number of events OBSERVED  in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_observed << std::endl;  
    std::cout << "Number of events ESTIMATED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_estimated << " +/- " << N_estimated_err << std::endl;
    
    
    dataDir -> cd();
    
    h_data_lepNuW_m -> Write();
    func -> Write();
    hint -> Write();
    
    DrawPulls(dataDir,h_data_lepNuW_m,hint,higgsMass,fitMethod,step);
    
    outFile -> cd();
    
    outTree_data -> Fill();
  }
  
  
  
  if( onData == 1 )
  {
    // Do the fit
    fitStatus = fitHiggsMassBinned(h_dataSig_lepNuW_m,
                                   xFitMIN1,xFitMAX1,xFitMIN2,xFitMAX2,
                                   &func,true,hint);
    
    
    // Get the error bands
    float bandIntegral = 0;
    for(int bin = 1; bin <= hint->GetNbinsX(); ++bin)
    {
      float thisX = hint -> GetBinCenter(bin);
      if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
        bandIntegral += hint -> GetBinError(bin);
    }
    
    
    chi2 = func->GetChisquare()/func->GetNDF();
    
    for(int iPar = 0; iPar < nPar; ++iPar)
    {
      par[iPar] = func -> GetParameter(4+iPar);
      parErr[iPar] = func -> GetParError(4+iPar);
    }
    
    
    // Print the results
    //std::cout << "**************************" << std::endl;
    //std::cout << ">>> FIT RESULTS - DATA <<<" << std::endl;
    //std::cout << "**************************" << std::endl;
    
    N_observed  = h_dataSig_lepNuW_m->Integral(binMin,binMax);
    N_estimated = hint->Integral(binMin,binMax);
    N_estimated_err = bandIntegral;
    
    std::cout << "Number of events OBSERVED  in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_observed << std::endl;  
    std::cout << "Number of events ESTIMATED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_estimated << " +/- " << N_estimated_err << std::endl;
    
    
    dataSigDir -> cd();
    
    h_dataSig_lepNuW_m -> Write();
    func -> Write();
    hint -> Write();
    h_sig_lepNuW_m -> Write();
    
    DrawPulls(dataSigDir,h_dataSig_lepNuW_m,hint,higgsMass,fitMethod,step);
    
    outFile -> cd();
    
    outTree_dataSig -> Fill();
  }
  
  
  
  //--------------------------------
  // ON FAKE - FIT MASS DISTRIBUTION
  //--------------------------------
  
  if( onFake == 1 )
  {
    // Do the fit
    fitStatus = fitHiggsMassBinned(h_fake_lepNuW_m,
                                   xFitMIN1,xFitMAX1,xFitMIN2,xFitMAX2,
                                   &func,true,hint);
    
    
    // Get the error bands
    float bandIntegral = 0;
    for(int bin = 1; bin <= hint->GetNbinsX(); ++bin)
    {
      float thisX = hint -> GetBinCenter(bin);
      if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
        bandIntegral += hint -> GetBinError(bin);
    }
    
    
    chi2 = func->GetChisquare()/func->GetNDF();
    
    for(int iPar = 0; iPar < nPar; ++iPar)
    {
      par[iPar] = func -> GetParameter(4+iPar);
      parErr[iPar] = func -> GetParError(4+iPar);
    }
    
    
    // Print the results
    std::cout << "**************************" << std::endl;
    std::cout << ">>> FIT RESULTS - FAKE <<<" << std::endl;
    std::cout << "**************************" << std::endl;
    
    N_observed  = h_fake_lepNuW_m->Integral(binMin,binMax);
    N_estimated = hint->Integral(binMin,binMax);
    N_estimated_err = bandIntegral;
    
    std::cout << "Number of events OBSERVED  in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_observed << std::endl;  
    std::cout << "Number of events ESTIMATED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_estimated << " +/- " << N_estimated_err << std::endl;
    
    
    fakeDir -> cd();
    
    h_fake_lepNuW_m -> Write();
    func -> Write();
    hint -> Write();
    
    outFile -> cd();
    
    outTree_fake -> Fill();    
  }
  
  
  
  //------------------------------
  // ON MC - FIT MASS DISTRIBUTION
  //------------------------------
  
  if( onMC == 1 )
  {
    // count events
    float N = h_mcSumToSample_lepNuW_m -> Integral(binFitMin1,binFitMax2);
    float N_sig = h_sigToSample_lepNuW_m -> Integral(binFitMin1,binFitMax2);
    
    N_expected = h_mcSumToSample_lepNuW_m -> Integral(binMin,binMax);
    N_expected_err = 0;
    for(int bin = 1; bin <= h_mcSumToSample_lepNuW_m->GetNbinsX(); ++bin)
    {
      float thisX = h_mcSumToSample_lepNuW_m -> GetBinCenter(bin);
      if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
        N_expected_err += h_mcSumToSample_lepNuW_m -> GetBinError(bin);
    }

    N_expected_sig = h_sigToSample_lepNuW_m -> Integral(binMin,binMax);
    N_expected_err_sig = 0;
    for(int bin = 1; bin <= h_sigToSample_lepNuW_m->GetNbinsX(); ++bin)
    {
      float thisX = h_sigToSample_lepNuW_m -> GetBinCenter(bin);
      if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
        N_expected_err_sig += h_sigToSample_lepNuW_m -> GetBinError(bin);
    }
    
    
    // Print the results
    std::cout << "************************" << std::endl;
    std::cout << ">>> FIT RESULTS - MC <<<" << std::endl;
    std::cout << "************************" << std::endl;
    std::cout << "Number of events EXPECTED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_expected << " +/- " << N_expected_err << std::endl;
    std::cout << "Number of signal events EXPECTED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_expected_sig << " +/- " << N_expected_err_sig << std::endl;
    
    
    
    // loop on toy experiments
    TRandom3 r;
    for(toyIt = 0; toyIt < toyMAX; ++toyIt)
    {
      if( (toyIt%100) == 0 ) std::cout << ">>>>>> VBFAnalysis_fitHiggsMassBinned::ToyExperiment " << toyIt << "/" << toyMAX << std::endl;
      
      
      h_toy_lepNuW_m -> Reset();
      h_toy_lepNuW_m_sig -> Reset();
      
      
      
      //-----------------------------------------------------------------------------------------
      // background only
      int N_poisson = r.Poisson(N);
      for(int entry = 0; entry < N_poisson; ++entry)
      {
        double lepNuW_m = h_mcSumToSample_lepNuW_m -> GetRandom();
        double weight = 1.;
        
        h_toy_lepNuW_m -> Fill(lepNuW_m,weight);
        h_toy_lepNuW_m_sig -> Fill(lepNuW_m,weight);
      }
      
      // Do the fit
      fitStatus = fitHiggsMassBinned(h_toy_lepNuW_m,
				     xFitMIN1,xFitMAX1,xFitMIN2,xFitMAX2,
                                     &func,true,hint);
      
      // Get the error bands
      float bandIntegral = 0;
      for(int bin = 1; bin <= hint->GetNbinsX(); ++bin)
      {
        float thisX = hint -> GetBinCenter(bin);
        if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
          bandIntegral += hint -> GetBinError(bin);
      }
            
      N_observed  = h_toy_lepNuW_m->Integral(binMin,binMax);
      N_estimated = hint->Integral(binMin,binMax);
      N_estimated_err = bandIntegral;
      
      if( toyIt < 10 )
      {
        std::cout << "toy: " << toyIt << std::endl;
        //std::cout << "Number of events EXPECTED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_expected << std::endl;
        std::cout << "Number of events OBSERVED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_observed << std::endl;
        std::cout << "Number of events ESTIMATED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_estimated << " +/- " << N_estimated_err << std::endl;
        
        char dirName[50];
        sprintf(dirName,"toy%d",toyIt);
        MCDir -> mkdir(dirName);
        MCDir -> cd(dirName);
        
        h_toy_lepNuW_m -> Write();
        func -> Write();
        hint -> Write(); 
        
        outFile -> cd();
      }
      
      
      
      //-----------------------------------------------------------------------------------------
      // background + signal
      int N_poisson_sig = r.Poisson(N_sig);
      for(int entry = 0; entry < N_poisson_sig; ++entry)
      {
        double lepNuW_m = h_sigToSample_lepNuW_m -> GetRandom();
        double weight = 1.;
        
        h_toy_lepNuW_m_sig -> Fill(lepNuW_m,weight);
      }
      
      // Do the fit
      fitStatus_sig = fitHiggsMassBinned(h_toy_lepNuW_m_sig,
                                         xFitMIN1,xFitMAX1,xFitMIN2,xFitMAX2,
                                         &func_sig,true,hint_sig);
      
      // Get the error bands
      float bandIntegral_sig = 0;
      for(int bin = 1; bin <= hint_sig->GetNbinsX(); ++bin)
      {
        float thisX = hint_sig -> GetBinCenter(bin);
        if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
          bandIntegral_sig += hint_sig -> GetBinError(bin);
      }
            
      N_observed_sig  = h_toy_lepNuW_m_sig->Integral(binMin,binMax);
      N_estimated_sig = hint_sig->Integral(binMin,binMax);
      N_estimated_err_sig = bandIntegral_sig;
      
      if( toyIt < 10 )
      {
        std::cout << "-------------------------------------------------------------" << std::endl;      
        //std::cout << "Number of events EXPECTED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_expected_sig << std::endl;
        std::cout << "Number of events OBSERVED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_observed_sig << std::endl;
        std::cout << "Number of events ESTIMATED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_estimated_sig << " +/- " << N_estimated_err_sig << std::endl;
        std::cout << std::endl;
        
        
        
        char dirName[50];
        sprintf(dirName,"toy%d_sig",toyIt);
        MCDir -> mkdir(dirName);
        MCDir -> cd(dirName);
        
        h_toy_lepNuW_m_sig -> Write();
        func_sig -> Write();
        hint_sig -> Write(); 
        
        outFile -> cd();
      }      
      
      
      
      // fill histograms
      MCDir -> cd();
      
      chi2 = func->GetChisquare()/func->GetNDF();
      chi2_sig = func_sig->GetChisquare()/func_sig->GetNDF();
      
      h_chi2 -> Fill( chi2 );
      h_diff_obs -> Fill( N_observed - N_expected );
      h_res_obs  -> Fill( N_observed / N_expected - 1. );
      h_diff_est -> Fill( N_estimated - N_expected );
      h_res_est  -> Fill( N_estimated / N_expected - 1. );
      h_err_est  -> Fill( bandIntegral );
      
      outTree_MC -> Fill();
      
      outFile -> cd();
    }
  }
  
  
  
  
  
  
  //-----------------
  // Save the results
  
  if( onData == 1 )
  {
    dataDir -> cd();
    
    outTree_data -> Write();
    
    outFile -> cd();
  }
  
  if( onMC == 1 )
  {
    MCDir -> cd();
    
    h_mcSum_lepNuW_m -> Write();
    
    func_toSample -> Write();
    h_mcSumToSample_lepNuW_m -> Write();
    h_sigToSample_lepNuW_m -> Write();
    
    h_chi2 -> Write();
    
    h_diff_obs -> Fit("gaus","Q+");
    h_diff_obs -> Write();
    h_res_obs -> Fit("gaus","Q+");
    h_res_obs  -> Write();
    
    h_diff_est -> Fit("gaus","Q+");
    h_diff_est -> Write();
    h_res_est -> Fit("gaus","Q+");
    h_res_est  -> Write();
    
    h_err_est  -> Write();
    
    outTree_MC -> Write();
    
    outFile -> cd();
  }
  
  outFile -> Close();
  
  
  
  return 0;
}






double fitFunc(double* x, double* par)
{
  // variable
  double xx = x[0];
  // ranges
  double xMin1 = par[0];
  double xMax1 = par[1];
  double xMin2 = par[2];
  double xMax2 = par[3];
  //std::cout << "x: " << xx << "   xMin1: " << xMin1 <<  "   xMax1: " << xMax1 << "   xMin2: " << xMin2 <<  "   xMax2: " << xMax2 << std::endl; 
  
  // fit function ranges
  if( xx < xMin1 )                    TF1::RejectPoint();
  if( (xx >= xMax1) && (xx < xMin2) ) TF1::RejectPoint();
  if( xx >= xMax2 )                   TF1::RejectPoint();  

  if( (fitMethod == "exponential") || (fitMethod == "exponentialNoHoles") )
    return exponential(x,&par[4]);
  if( (fitMethod == "attenuatedExponential") || (fitMethod == "attenuatedExponentialNoHoles") )
    return attenuatedExponential(x,&par[4]);
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") )
    return doubleExponential(x,&par[4]);
  if( (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
    return attenuatedDoubleExponential(x,&par[4]);
  else
    return -1.;
}



double fitFuncToSample(double* x, double* par)
{
  // variable
  double xx = x[0];
  // ranges
  double xMin = par[0];
  //std::cout << "x: " << xx << "   xMin: " << xMin << std::endl; 
  
  // fit function ranges
  if( xx < xMin ) return 0.;
  
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") )
    return doubleExponential(x,&par[1]);
  if( (fitMethod == "attenuatedExponential") || (fitMethod == "attenuatedExponentialNoHoles") )
    return attenuatedExponential(x,&par[1]);
  if( (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
    return attenuatedDoubleExponential(x,&par[1]);
  else
    return -1.;
}



int fitHiggsMassBinned(TH1F* h_lepNuW_m,
                       float xFitMIN1, float xFitMAX1, float xFitMIN2, float xFitMAX2,
                       TF1** func, bool computeCL, TH1F* hint)
{
  int nPar = -1;
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") ) nPar = 4;
  if( (fitMethod == "attenuatedExponential") || (fitMethod == "attenuatedExponentialNoHoles") ) nPar = 4;
  if( (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") ) nPar = 6;
  
  (*func) = new TF1("func",fitFunc,0.,1000.,4+nPar);
  (*func) -> FixParameter(0,xFitMIN1);
  (*func) -> FixParameter(1,xFitMAX1);
  (*func) -> FixParameter(2,xFitMIN2);
  (*func) -> FixParameter(3,xFitMAX2);
  
  
  
  if( (fitMethod == "exponential") || (fitMethod == "exponentialNoHoles") )
  {
    //------------
    // exponential
    
    (*func) -> SetParLimits(4,0.,100.);
    (*func) -> SetParLimits(5,0.,0.1);
    
    (*func) -> SetParameter(4,10.);
    (*func) -> SetParameter(5,0.012);
    
    (*func) -> SetParName(4,"N1");
    (*func) -> SetParName(5,"#lambda1");
  }
  
  
  if( (fitMethod == "attenuatedExponential") || (fitMethod == "attenuatedExponentialNoHoles") )
  {
    //-----------------------
    // attenuated exponential
    
    (*func) -> SetParLimits(4,0.,500.);
    (*func) -> SetParLimits(5,0.,100.);
    (*func) -> SetParLimits(6,0.,100.);
    (*func) -> SetParLimits(7,0.,0.1);
    
    (*func) -> FixParameter(4,165.);
    (*func) -> SetParameter(5,10.);
    (*func) -> SetParameter(6,10.);
    (*func) -> SetParameter(7,0.012);
    
    (*func) -> SetParName(4,"#mu");
    (*func) -> SetParName(5,"kT");
    (*func) -> SetParName(6,"N1");
    (*func) -> SetParName(7,"#lambda1");
  }
  
  
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") )
  {
    //-------------------
    // double exponential
    
    (*func) -> SetParLimits(4,0.,100.);
    (*func) -> SetParLimits(5,0.,0.1);
    (*func) -> SetParLimits(6,0.,100.);
    (*func) -> SetParLimits(7,0.,0.1);
    
    (*func) -> SetParameter(4,1.1);
    (*func) -> SetParameter(5,0.057);
    (*func) -> SetParameter(6,10.);
    (*func) -> SetParameter(7,0.012);
    
    (*func) -> SetParName(4,"N1");
    (*func) -> SetParName(5,"#lambda1");
    (*func) -> SetParName(6,"N2");
    (*func) -> SetParName(7,"#lambda2");
  }
  
  
  if( (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
  {
    //------------------------------
    // attenuated double exponential
    
    (*func) -> SetParLimits(4,0.,500.);
    (*func) -> SetParLimits(5,0.,100.);
    (*func) -> SetParLimits(6,0.,100.);
    (*func) -> SetParLimits(7,0.,0.1);
    (*func) -> SetParLimits(8,0.,100.);
    (*func) -> SetParLimits(9,0.,0.1);
    
    (*func) -> FixParameter(4,165.);
    (*func) -> SetParameter(5,50.);
    (*func) -> SetParameter(6,12.);
    (*func) -> SetParameter(7,0.023);
    (*func) -> SetParameter(8,10.);
    (*func) -> SetParameter(9,0.011);
    
    (*func) -> SetParName(4,"#mu");
    (*func) -> SetParName(5,"kT");
    (*func) -> SetParName(6,"N1");
    (*func) -> SetParName(7,"#lambda1");
    (*func) -> SetParName(8,"N2");
    (*func) -> SetParName(9,"#lambda2");
  }
  
  
  //----------------------------
  // superParabolaCumDoubleCauda
  
  //(*func) -> SetParameter(4,-0.35);
  //(*func) -> SetParameter(5,250.);
  //(*func) -> SetParameter(6,10.);
  //(*func) -> SetParameter(7,0.020);
  //(*func) -> SetParameter(8,10.);
  //(*func) -> SetParameter(9,0.010);
  //
  //(*func) -> SetParLimits(4,-1.,0.);
  //
  //(*func) -> SetParName(4,"a");
  //(*func) -> SetParName(5,"x0");
  //(*func) -> SetParName(6,"N1");
  //(*func) -> SetParName(7,"#lambda1");
  //(*func) -> SetParName(8,"N2");
  //(*func) -> SetParName(9,"#lambda2");
  
  
  //TVirtualFitter::SetDefaultFitter("Minuit2");
  (*func) -> SetNpx(10000);
  (*func) -> SetLineWidth(2);
  (*func) -> SetLineColor(kRed);
  int counter = 0;
  int fitStatus = -1;
  while( counter < 100 )
  {
    TFitResultPtr fitResultPtr = h_lepNuW_m -> Fit("func","QLRS+","",xFitMIN1,xFitMAX2);
    fitStatus = (int)(fitResultPtr);
    if( fitStatus == 0 ) break;
    ++counter;
  }
  
  
  if( (computeCL) && (fitStatus == 0) )
  {
    // Create a histogram to hold the confidence intervals
    hint -> Reset();
    (TVirtualFitter::GetFitter()) -> GetConfidenceIntervals(hint,0.68);
    
    for(int bin = 1; bin <= hint->GetNbinsX(); ++bin)
    {
      float binCenter = hint -> GetBinCenter(bin);
      if( (binCenter < xFitMIN1) || (binCenter >= xFitMAX2) )
      {
        hint -> SetBinContent(bin,0.);
        hint -> SetBinError(bin,0.);
      }
    }
    
    hint -> SetStats(kFALSE);
    hint -> SetMarkerSize(0);
    hint -> SetFillColor(kRed);
    hint -> SetFillStyle(3002);
  }    
    
  return fitStatus;
}



void DrawPulls(TDirectory* outFile, TH1F* num, TH1F* den, const int& higgsMass, const std::string& fitMethod, const int& step)
{
  float xMin    = num -> GetBinLowEdge(1);
  float xWidth  = num -> GetBinWidth(1);

  float xFitMIN1 = GetXFitMIN1(higgsMass,fitMethod,step);
  float xFitMAX2 = GetXFitMAX2(higgsMass,fitMethod);
  
  std::vector<int> masses = GetMasses();  
  
  // open outfile
  outFile -> cd();
  
  
  // create a graph to hold the pull plot  
  TGraphAsymmErrors* g_pull = new TGraphAsymmErrors();
  TGraphAsymmErrors* g_pullWindow = new TGraphAsymmErrors();
  int point = 0;
  int pointWindow = 0;
  
  
  // fill the graph
  for(int bin = 1; bin <= num->GetNbinsX(); ++bin)
  {
    float binCenter = num -> GetBinCenter(bin);
    
    if( (binCenter >= xFitMIN1) && (binCenter < xFitMAX2) )
    {
      float numBinContent = num -> GetBinContent(bin);
      float numBinError   = num -> GetBinError(bin);
      float denBinContent = den -> GetBinContent(bin);
      float denBinError   = den -> GetBinError(bin);
      
      if( denBinContent > 0. )
      {
        float ratio = numBinContent/denBinContent;
        float ratioErr = ratio * sqrt( pow(numBinError/numBinContent,2.) + pow(denBinError/denBinContent,2.) );
        
        g_pull -> SetPoint(point,binCenter,ratio);
        g_pull -> SetPointError(point,0.5*xWidth,0.5*xWidth,ratioErr,ratioErr);
        ++point;
      }
    }
  }
  
  
  for(unsigned int massIt = 0; massIt < masses.size(); ++massIt )
  {
    int mass = masses.at(massIt);
    float lepNuWMMIN = GetLepNuWMMIN(mass);
    float lepNuWMMAX = GetLepNuWMMAX(mass);
    int binMin = -1;
    int binMax = -1;

    for(int bin = 1; bin <= num->GetNbinsX(); ++bin)
    {
      float binCenter = xMin + 0.5*xWidth + xWidth*(bin-1);
      if( (binCenter >= lepNuWMMIN) && (binMin == -1) ) binMin = bin;
      if( (binCenter >= lepNuWMMIN) && (binCenter < lepNuWMMAX) ) binMax = bin;
    }
    
    double numContent;
    double numError;
    numContent = num -> IntegralAndError(binMin,binMax,numError);
    
    double denContent;
    double denError;
    denContent = den -> IntegralAndError(binMin,binMax,denError);
    
    if( denContent > 0. )
    {
      double ratio = numContent/denContent;
      double ratioErr = ratio * sqrt( pow(numError/numContent,2.) + pow(denError/denContent,2.) );
      
      g_pullWindow -> SetPoint(pointWindow,mass,ratio);
      g_pullWindow -> SetPointError(pointWindow,mass-lepNuWMMIN,lepNuWMMAX-mass,ratioErr,ratioErr);
      ++pointWindow;
    }
  }
  
  
  // fit the graph
  TF1* f_pol0 = new TF1("f_pol0","pol0",0.,1000.);
  f_pol0 -> SetLineColor(kRed);
  f_pol0 -> SetLineWidth(2);
  
  g_pull       -> Fit("f_pol0","QR+");
  g_pullWindow -> Fit("f_pol0","QR+");
  
  g_pull       -> Write("pull");
  g_pullWindow -> Write("pullWindow");
  
  delete g_pull;
  delete f_pol0;
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
