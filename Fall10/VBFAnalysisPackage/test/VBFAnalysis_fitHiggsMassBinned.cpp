#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "Functions.h"
#include "HiggsMassWindows.h"

#include <iostream>

#include "TH1F.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TRandom3.h"
#include "TDirectory.h"



double fitFunc(double* x, double* par);
double fitFuncToSample(double* x, double* par);

int fitHiggsMassBinned(TH1F* h_lepNuW_m,
                       float xFitMIN1, float xFitMAX1, float xFitMIN2, float xFitMAX2, float xWidth,
                       TF1** func, bool computeCL = false, TH1F* hint = NULL);

std::string fitMethod = "";
std::string varName = "lepNuW_m_KF"; 
std::string WMassCut = "( (WJJ_m >= 65.) && (WJJ_m < 95.) )";
//std::string WMassCut = "( ( (WJJ_m >= 55.) && (WJJ_m < 65.) ) || ( (WJJ_m >= 95.) && (WJJ_m < 120.) ) )";





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
  std::string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType      = gConfigParser -> readStringOption("Input::jetType");
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
  int step   = gConfigParser -> readIntOption("Options::step");
  int onData = gConfigParser -> readIntOption("Options::onData");
  int onMC   = gConfigParser -> readIntOption("Options::onMC");
  int toyMAX = gConfigParser -> readIntOption("Options::toyMAX");
  fitMethod  = gConfigParser -> readStringOption("Options::fitMethod");
  
  //[Cuts]
  float lepNuWMMIN = GetLepNuWMMIN(higgsMass);
  float lepNuWMMAX = GetLepNuWMMAX(higgsMass);
  float xFitMIN1 = GetXFitMIN1(higgsMass,fitMethod);
  float xFitMAX1 = GetXFitMAX1(higgsMass,fitMethod);
  float xFitMIN2 = GetXFitMIN2(higgsMass,fitMethod);
  float xFitMAX2 = GetXFitMAX2(higgsMass,fitMethod);
  
  float xMin = 0.;
  float xMax = 1000.;
  float xWidth = GetBinWidth();
  int nBins = int((xMax-xMin)/xWidth);
  
  int nBinsFit = int((xFitMAX2-xFitMIN1)/xWidth);
  
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
  
  
  
  
  
  
  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + fitMethod + "_" + jetAlgorithm + "_H" + std::string(higgsMassChar) + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  TDirectory* MCDir = outFile -> mkdir("MC");
  TDirectory* dataDir = outFile -> mkdir("data");
  
  
  
  
  
  
  // Define the histograms
  TH1F* h_lepNuW_m = new TH1F("h_lepNuW_m","",nBins,xMin,xMax);
  h_lepNuW_m -> Sumw2();
  TH1F* h_lepNuW_m_sig = new TH1F("h_lepNuW_m_sig","",nBins,xMin,xMax);
  h_lepNuW_m_sig -> Sumw2();
  
  TH1F* h_mcSum_lepNuW_m = new TH1F("h_mcSum_lepNuW_m","",nBins,xMin,xMax);
  h_mcSum_lepNuW_m -> Sumw2();
  
  TH1F* h_sig_lepNuW_m = new TH1F("h_sig_lepNuW_m","",nBins,xMin,xMax);
  h_sig_lepNuW_m -> Sumw2();
  
  TH1F* h_mcSumToSample_lepNuW_m;
  TH1F* h_sigToSample_lepNuW_m;
  
  TF1* func;
  TF1* func_sig;
  
  //TH1F* hint = new TH1F("hint","",nBinsFit,xFitMIN1,xFitMAX2);
  //TH1F* hint_sig = new TH1F("hint_sig","",nBinsFit,xFitMIN1,xFitMAX2);
  TH1F* hint = new TH1F("hint","",nBins,xMin,xMax);
  TH1F* hint_sig = new TH1F("hint_sig","",nBins,xMin,xMax);
  
  TH1F* h_chi2     = new TH1F("h_chi2","",     100,    0.,   3.);
  TH1F* h_diff_obs = new TH1F("h_diff_obs","",2000,-1000.,1000.);
  TH1F* h_res_obs  = new TH1F("h_res_obs", "", 500,   -1.,   1.);
  TH1F* h_diff_est = new TH1F("h_diff_est","",2000,-1000.,1000.);
  TH1F* h_res_est  = new TH1F("h_res_est", "", 500,   -1.,   1.);
  TH1F* h_err_est  = new TH1F("h_err_est", "",2000,    0.,2000.);
  
  
  
  // Define the output tree
  TTree* outTree_data = new TTree("ntu_data","ntu_data");
  TTree* outTree_MC = new TTree("ntu_MC","ntu_MC");
  
  int toyIt;
  
  int fitStatus;
  int fitStatus_sig;
  
  float chi2;
  float chi2_sig;
  
  int nPar = -1;
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
      
      tree -> Draw((varName+" >>+ h_lepNuW_m").c_str(),extendedCut.c_str(),"goff");
    }
  }
  
  
  
  
  
  
  //-------------------------------
  // ON MC - FILL MASS DISTRIBUTION
  //-------------------------------
  
  if( onMC == 1 )
  {
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
      weight << "( 1000 * " << lumi << " * 1. * eventWeight / totEvents * crossSection * eventWeight * PUWeight )";
      std::string extendedCut = weight.str() + " * " + WMassCut;      
      
      tree -> Draw((varName+" >>+ h_sig_lepNuW_m").c_str(),extendedCut.c_str(),"goff");
    }
  }
  
  
  
  
  
  
  //--------------------------------
  // ON DATA - FIT MASS DISTRIBUTION
  //--------------------------------
  
  if( onData == 1 )
  {
    // Do the fit
    fitStatus = fitHiggsMassBinned(h_lepNuW_m,
                                   xFitMIN1,xFitMAX1,xFitMIN2,xFitMAX2,xWidth,
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
    
    N_observed  = h_lepNuW_m->Integral(binMin,binMax);
    N_estimated = hint->Integral(binMin,binMax);
    N_estimated_err = bandIntegral;
    
    std::cout << "Number of events OBSERVED  in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_observed << std::endl;  
    std::cout << "Number of events ESTIMATED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_estimated << " +/- " << N_estimated_err << std::endl;
    
    
    dataDir -> cd();
    
    h_lepNuW_m -> Write();
    func -> Write();
    hint -> Write();
    
    outFile -> cd();
    
    outTree_data -> Fill();    
  }
  
  
  
  
  
  
  //------------------------------
  // ON MC - FIT MASS DISTRIBUTION
  //------------------------------
  
  TF1* f_toSample = new TF1("f_toSample",fitFuncToSample,xMin,xMax,1+6);
  f_toSample -> SetNpx(10000);
  f_toSample -> SetLineColor(kBlue);
  f_toSample -> SetLineWidth(2);

  f_toSample -> FixParameter(0,160.);
  f_toSample -> SetParameter(1,190.);
  f_toSample -> SetParameter(2,20.);
  f_toSample -> SetParameter(3,10.);
  f_toSample -> SetParameter(4,0.013);
  f_toSample -> SetParameter(5,5.);
  f_toSample -> SetParameter(6,0.005);
  
  f_toSample -> SetParName(1,"#mu");
  f_toSample -> SetParName(2,"kT");
  f_toSample -> SetParName(3,"N1");
  f_toSample -> SetParName(4,"#lambda1");
  f_toSample -> SetParName(5,"N2");
  f_toSample -> SetParName(6,"#lambda2");
  
  TFitResultPtr fitResultPtr = h_mcSum_lepNuW_m -> Fit("f_toSample","QLRS+","",xFitMIN1,xFitMAX2);
  fitStatus = (int)(fitResultPtr);
  int counter = 0;
  while( counter < 100 )
  {
    fitResultPtr = h_mcSum_lepNuW_m -> Fit("f_toSample","QLRS+","",xFitMIN1,xFitMAX2);
    fitStatus = (int)(fitResultPtr);
    if( fitStatus == 0 ) break;
    ++counter;
  }
  
  TH1F* temp = new TH1F("temp","",nBins,xMin,xMax);
  temp -> Sumw2();
  for(int j = 0; j < 10000000; ++j)
    temp -> Fill( f_toSample->GetRandom() );
  temp -> Scale(h_mcSum_lepNuW_m->Integral(binFitMin1,binFitMax2)/temp->Integral(binFitMin1,binFitMax2));
  h_mcSumToSample_lepNuW_m = (TH1F*)(temp->Clone("h_mcSumToSample_lepNuW_m"));
  
  h_sigToSample_lepNuW_m = (TH1F*)(h_sig_lepNuW_m->Clone("h_sigToSample_lepNuW_m"));
  
  
  
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
      
      
      h_lepNuW_m -> Reset();
      h_lepNuW_m_sig -> Reset();
      
      
      
      //-----------------------------------------------------------------------------------------
      // background only
      int N_poisson = r.Poisson(N);
      for(int entry = 0; entry < N_poisson; ++entry)
      {
        double lepNuW_m = h_mcSumToSample_lepNuW_m -> GetRandom();
        double weight = 1.;
        
        h_lepNuW_m -> Fill(lepNuW_m,weight);
        h_lepNuW_m_sig -> Fill(lepNuW_m,weight);
      }
      
      // Do the fit
      fitStatus = fitHiggsMassBinned(h_lepNuW_m,
				     xFitMIN1,xFitMAX1,xFitMIN2,xFitMAX2,xWidth,
                                     &func,true,hint);
      
      // Get the error bands
      float bandIntegral = 0;
      for(int bin = 1; bin <= hint->GetNbinsX(); ++bin)
      {
        float thisX = hint -> GetBinCenter(bin);
        if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
          bandIntegral += hint -> GetBinError(bin);
      }
            
      N_observed  = h_lepNuW_m->Integral(binMin,binMax);
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
        
        h_lepNuW_m -> Write();
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
        
        h_lepNuW_m_sig -> Fill(lepNuW_m,weight);
      }
      
      // Do the fit
      fitStatus_sig = fitHiggsMassBinned(h_lepNuW_m_sig,
                                         xFitMIN1,xFitMAX1,xFitMIN2,xFitMAX2,xWidth,
                                         &func_sig,true,hint_sig);
      
      // Get the error bands
      float bandIntegral_sig = 0;
      for(int bin = 1; bin <= hint_sig->GetNbinsX(); ++bin)
      {
        float thisX = hint_sig -> GetBinCenter(bin);
        if( (thisX >= lepNuWMMIN) && (thisX < lepNuWMMAX) )
          bandIntegral_sig += hint_sig -> GetBinError(bin);
      }
            
      N_observed_sig  = h_lepNuW_m_sig->Integral(binMin,binMax);
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
        
        h_lepNuW_m_sig -> Write();
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
    
    f_toSample -> Write();
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
  if( (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
    return attenuatedDoubleExponential(x,&par[1]);
  else
    return -1.;
}



int fitHiggsMassBinned(TH1F* h_lepNuW_m,
                       float xFitMIN1, float xFitMAX1, float xFitMIN2, float xFitMAX2, float xWidth,
                       TF1** func, bool computeCL, TH1F* hint)
{
  int nPar = -1;
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") ) nPar = 4;
  if( (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") ) nPar = 6;
  
  (*func) = new TF1("func",fitFunc,0.,1000.,4+nPar);
  (*func) -> FixParameter(0,xFitMIN1);
  (*func) -> FixParameter(1,xFitMAX1);
  (*func) -> FixParameter(2,xFitMIN2);
  (*func) -> FixParameter(3,xFitMAX2);
  
  
  
  if( (fitMethod == "doubleExponential") || (fitMethod == "doubleExponentialNoHoles") )
  {
    //-------------------
    // double exponential
    (*func) -> SetParameter(4,10.14);
    (*func) -> SetParameter(5,0.015);
    (*func) -> SetParameter(6,9.80);
    (*func) -> SetParameter(7,0.011);
    
    (*func) -> SetParName(4,"N1");
    (*func) -> SetParName(5,"#lambda1");
    (*func) -> SetParName(6,"N2");
    (*func) -> SetParName(7,"#lambda2");
  }
  
  
  if( (fitMethod == "attenuatedDoubleExponential") || (fitMethod == "attenuatedDoubleExponentialNoHoles") )
  {
    //------------------------------
    // attenuated double exponential
    
    (*func) -> SetParameter(4,190.);
    (*func) -> SetParameter(5,20.);
    (*func) -> SetParameter(6,10.);
    (*func) -> SetParameter(7,0.013);
    (*func) -> SetParameter(8,5.);
    (*func) -> SetParameter(9,0.005);
    
    (*func) -> SetParLimits(4,50.,500.);
    (*func) -> SetParLimits(5,0.,100.);
    
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
  
  
  
  //TVirtualFitter::SetDefaultFitter("Fumili2");
  (*func) -> SetNpx(10000);
  (*func) -> SetLineWidth(2);
  (*func) -> SetLineColor(kRed);
  TFitResultPtr fitResultPtr = h_lepNuW_m -> Fit("func","QLRS+","",xFitMIN1,xFitMAX2);
  int fitStatus = (int)(fitResultPtr);
  int counter = 0;
  while( counter < 10 )
  {
    fitResultPtr = h_lepNuW_m -> Fit("func","QLRS+","",xFitMIN1,xFitMAX2);
    fitStatus = (int)(fitResultPtr);
    if( fitStatus == 0 ) break;
    
    ++counter;
  }
  //std::cout << "computeCL: " << computeCL << "   fitStatus: " << fitStatus << "   counter: " << counter << std::endl;
  
  hint -> Reset();
  if( (computeCL) && (fitStatus == 0) )
  {
    //Create a histogram to hold the confidence intervals
    (TVirtualFitter::GetFitter()) -> GetConfidenceIntervals(hint,0.68);
    hint -> SetStats(kFALSE);
    hint -> SetMarkerSize(0);
    hint -> SetFillColor(kRed);
    hint -> SetFillStyle(3002);
  }
  
  return fitStatus;
}
