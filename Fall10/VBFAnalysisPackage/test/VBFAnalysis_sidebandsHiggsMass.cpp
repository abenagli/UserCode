#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "Functions.h"

#include <iostream>
#include <string>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TRandom3.h"
#include "TDirectory.h"



double fitFunc(double* x, double* par);






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_sidebands::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType      = gConfigParser -> readStringOption("Input::jetType");
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
  int onData      = gConfigParser -> readIntOption("Options::onData");
  int onMC        = gConfigParser -> readIntOption("Options::onMC");
  int toyMAX      = gConfigParser -> readIntOption("Options::toyMAX");
  
  //[Cuts]
  float lepNuWMMIN = gConfigParser -> readFloatOption("Cuts::lepNuWMMIN");
  float lepNuWMMAX = gConfigParser -> readFloatOption("Cuts::lepNuWMMAX");
  float xFitMIN = gConfigParser -> readFloatOption("Cuts::xFitMIN");
  float xFitMAX = gConfigParser -> readFloatOption("Cuts::xFitMAX");
  
  int nBins = 200;
  float xMin = 0.;
  float xMax = 1000.;
  float xWidth = (xMax-xMin)/nBins;
  
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
  TDirectory* MCDir = outFile -> mkdir("MC");
  TDirectory* dataDir = outFile -> mkdir("data");
  
  
  
  
  
  
  // define the regions
  std::string sigRegion = "( (WJJ_m >= 65.) && (WJJ_m < 95.) )";
  std::string sbRegion = "( ( (WJJ_m >= 50.) && (WJJ_m < 65.) ) || ( (WJJ_m >= 95.) && (WJJ_m < 130.) ) )";
  
  
  
  // Define the histograms
  TH1F* h_sig_lepNuW_m = new TH1F("h_sig_lepNuW_m","",nBins,xMin,xMax);
  h_sig_lepNuW_m -> Sumw2();
  TH1F* h_sb_lepNuW_m = new TH1F("h_sb_lepNuW_m","",nBins,xMin,xMax);
  h_sb_lepNuW_m -> Sumw2();
  
  TH1F* h_lepNuW_m_sig = new TH1F("h_lepNuW_m_sig","",nBins,xMin,xMax);
  h_lepNuW_m_sig -> Sumw2();
  
  TH1F* h_mcSum_lepNuW_m = new TH1F("h_mcSum_lepNuW_m","",nBins,xMin,xMax);
  h_mcSum_lepNuW_m -> Sumw2();
  
  //TH1F* h_sig_lepNuW_m = new TH1F("h_sig_lepNuW_m","",nBins,xMin,xMax);
  //h_sig_lepNuW_m -> Sumw2();
  
  TH1F* h_mcSumToSample_lepNuW_m;
  TH1F* h_sigToSample_lepNuW_m;
  
  TF1* func;
  TF1* func_sig;
  
  TH1F* hint = new TH1F("hint","",nBinsFit,xFitMIN,xFitMAX);
  TH1F* hint_sig = new TH1F("hint_sig","",nBinsFit,xFitMIN,xFitMAX);
  
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
  outTree_data -> Branch("xFitMIN",   &xFitMIN,      "xFitMIN/F");
  outTree_data -> Branch("xFitMAX",   &xFitMAX,      "xFitMAX/F");
  outTree_data -> Branch("lepNuWMMIN",&lepNuWMMIN,"lepNuWMMIN/F");
  outTree_data -> Branch("lepNuWMMAX",&lepNuWMMAX,"lepNuWMMAX/F");
  
  outTree_data -> Branch("N_estimated",    &N_estimated,        "N_estimated/F");
  outTree_data -> Branch("N_estimated_err",&N_estimated_err,"N_estimated_err/F");
  outTree_data -> Branch("N_observed",     &N_observed,          "N_observed/F");
  
  
  // MC tree
  outTree_MC -> Branch("xFitMIN",   &xFitMIN,      "xFitMIN/F");
  outTree_MC -> Branch("xFitMAX",   &xFitMAX,      "xFitMAX/F");
  outTree_MC -> Branch("lepNuWMMIN",&lepNuWMMIN,"lepNuWMMIN/F");
  outTree_MC -> Branch("lepNuWMMAX",&lepNuWMMAX,"lepNuWMMAX/F");
  
  outTree_MC -> Branch("toyIt",&toyIt,"toyIt/I");
  
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
      std::cout << ">>>>>> VBFAnalysis_sidebands::data tree in " << inputDataDirs.at(i) << " opened" << std::endl;
      
      
      // get the tree at nth step
      TTree* tree = NULL;
      char treeName[50];
      sprintf(treeName, "ntu_%d", step);
      inputFile -> GetObject(treeName, tree);
      if ( tree -> GetEntries() == 0 ) continue; 
      
      outFile -> cd();
      
      std::stringstream weight;
      //weight << "( 1000 * " << lumi << " * 1. / totEvents * crossSection * PURescaleFactor(PUit_n) )";
      weight << "( 1. )";
      
      std::string sigCut = weight.str() + " * " + sigRegion; 
      std::string sbCut = weight.str() + " * " + sbRegion; 
      
      tree -> Draw("lepNuW_m_KF >>+ h_sig_lepNuW_m",sigCut.c_str(),"goff"); 
      tree -> Draw("lepNuW_m_KF >>+ h_sb_lepNuW_m",sbCut.c_str(),"goff"); 
    }
  }
  
  
  
  
  
  /*
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
      std::cout << ">>>>>> VBFAnalysis_sidebands::bkg tree in " << inputBkgDirs.at(i) << " opened" << std::endl;
      
      
      // get the tree at nth step
      TTree* tree = NULL;
      char treeName[50];
      sprintf(treeName, "ntu_%d", step);
      inputFile -> GetObject(treeName, tree);
      if ( tree -> GetEntries() == 0 ) continue; 
      
      
      // set tree branches
      tree -> SetBranchAddress("totEvents",    &totEvents);
      tree -> SetBranchAddress("crossSection", &crossSection);
      tree -> SetBranchAddress("PUit_n",       &PUit_n);
      tree -> SetBranchAddress("PUoot_n",      &PUoot_n);
      tree -> SetBranchAddress("lepNuW_m_KF",  &lepNuW_m);
      
      
      // loop on the events
      for(int entry = 0; entry < tree->GetEntries(); ++entry)
      {
        //std::cout << "reading entry " << entry << std::endl;
        tree -> GetEntry(entry);
        double weight = lumi * 1000 * 1. / totEvents * crossSection * PURescaleFactor(PUit_n);
        
        mydata -> push_back(lepNuW_m);
        myweights -> push_back(weight);
        
        h_mcSum_lepNuW_m -> Fill(lepNuW_m,weight);
      }
    }
    
    
    // loop on sig samples
    for(unsigned int i = 0; i < nSigTrees; ++i)
    {
      // open root file
      std::string inputFullFileName;
      inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
      TFile* inputFile = TFile::Open(inputFullFileName.c_str());
      std::cout << ">>>>>> VBFAnalysis_sidebands::sig tree in " << inputSigDirs.at(i) << " opened" << std::endl;
      
      
      // get the tree at nth step
      TTree* tree = NULL;
      char treeName[50];
      sprintf(treeName, "ntu_%d", step);
      inputFile -> GetObject(treeName, tree);
      if ( tree -> GetEntries() == 0 ) continue; 
      
      
      // set tree branches
      tree -> SetBranchAddress("totEvents",    &totEvents);
      tree -> SetBranchAddress("crossSection", &crossSection);
      tree -> SetBranchAddress("PUit_n",       &PUit_n);
      tree -> SetBranchAddress("PUoot_n",      &PUoot_n);
      tree -> SetBranchAddress("lepNuW_m_KF",  &lepNuW_m);
      
      
      // loop on the events
      for(int entry = 0; entry < tree->GetEntries(); ++entry)
      {
        //std::cout << "reading entry " << entry << std::endl;
        tree -> GetEntry(entry);
        double weight = lumi * 1000 * 1. / totEvents * crossSection * PURescaleFactor(PUit_n);
        
        mydata -> push_back(lepNuW_m);
        myweights -> push_back(weight);
        
        h_sig_lepNuW_m -> Fill(lepNuW_m,weight);
      }
    }
  }
  */
  
  
  
  
  
  //--------------------------------
  // ON DATA - FIT MASS DISTRIBUTION
  //--------------------------------
  
  if( onData == 1 )
  {
    // Print the results
    std::cout << "*******************************" << std::endl;
    std::cout << ">>> SIDEBAND RESULTS - DATA <<<" << std::endl;
    std::cout << "*******************************" << std::endl;
    
    N_observed  = h_sig_lepNuW_m->Integral(binMin,binMax);
    N_estimated = 0.;
    N_estimated_err = 0.;
    
    std::cout << "Number of events OBSERVED  in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_observed << std::endl;  
    std::cout << "Number of events ESTIMATED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_estimated << " +/- " << N_estimated_err << std::endl;
    
    
    dataDir -> cd();
    
    h_sig_lepNuW_m -> Write();
    h_sb_lepNuW_m -> Write();
    
    outFile -> cd();
    
    outTree_data -> Fill();    
  }
  
  
  
  
  
  /*
  //------------------------------
  // ON MC - FIT MASS DISTRIBUTION
  //------------------------------
  
  TF1* f_toSample = new TF1("f_toSample",doubleExponential,xFitMIN1,xFitMAX2,4);
  f_toSample -> SetNpx(10000);
  f_toSample -> SetLineColor(kBlue);
  f_toSample -> SetLineWidth(2);
  
  f_toSample -> SetParameter(0,3.);
  f_toSample -> SetParameter(1,0.005);
  f_toSample -> SetParameter(2,10.);
  f_toSample -> SetParameter(3,0.012);
  
  f_toSample -> SetParName(0,"N1");
  f_toSample -> SetParName(1,"#lambda1");
  f_toSample -> SetParName(2,"N2");
  f_toSample -> SetParName(3,"#lambda2");
  
  h_mcSum_lepNuW_m -> Fit("f_toSample","QRL+","",xFitMIN1,xFitMAX2);
  h_mcSum_lepNuW_m -> Fit("f_toSample","QRL+","",xFitMIN1,xFitMAX2);
  h_mcSum_lepNuW_m -> Fit("f_toSample","QRL+","",xFitMIN1,xFitMAX2);
  
  
  TH1F* temp = new TH1F("temp","",nBins,xMin,xMax);
  temp -> Sumw2();
  for(int j = 0; j < 10000000; ++j)
    temp -> Fill( f_toSample->GetRandom() );
  temp -> Scale(h_mcSum_lepNuW_m->Integral()/temp->GetEntries());
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
      if( (toyIt%100) == 0 ) std::cout << ">>>>>> VBFAnalysis_sidebands::ToyExperiment " << toyIt << "/" << toyMAX << std::endl;
      
      
      h_lepNuW_m -> Reset();
      h_lepNuW_m_sig -> Reset();
      
      
      
      //-----------------------------------------------------------------------------------------
      // background only
      int N_poisson = r.Poisson(N);
      for(int entry = 0; entry < N_poisson; ++entry)
      {
        lepNuW_m = h_mcSumToSample_lepNuW_m -> GetRandom();
        double weight = 1.;
        
        h_lepNuW_m -> Fill(lepNuW_m,weight);
        h_lepNuW_m_sig -> Fill(lepNuW_m,weight);
      }
      
      // Do the fit
      fitStatus = sidebands(h_lepNuW_m,
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
      N_estimated = func->Integral(lepNuWMMIN,lepNuWMMAX) / xWidth;
      N_estimated_err = bandIntegral;
      
      if( toyIt < 10 )
      {
        std::cout << "toy: " << toyIt << std::endl;
        std::cout << "Number of events EXPECTED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_expected << std::endl;
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
        lepNuW_m = h_sigToSample_lepNuW_m -> GetRandom();
        double weight = 1.;
        
        h_lepNuW_m_sig -> Fill(lepNuW_m,weight);
      }
      
      // Do the fit
      fitStatus_sig = sidebands(h_lepNuW_m_sig,
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
      N_estimated_sig = func_sig->Integral(lepNuWMMIN,lepNuWMMAX) / xWidth;
      N_estimated_err_sig = bandIntegral_sig;
      
      if( toyIt < 10 )
      {
        std::cout << "-------------------------------------------------------------" << std::endl;      
        std::cout << "Number of events EXPECTED in [" << lepNuWMMIN << "," << lepNuWMMAX << "] = " << N_expected_sig << std::endl;
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
  */
  
  
  
  
  
  //-----------------
  // Save the results
  if( onData == 1 )
  {
    dataDir -> cd();
    
    outTree_data -> Write();
    
    outFile -> cd();
  }
  /*
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
  */
  
  
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
  
  return doubleExponential(x,&par[4]);
}
