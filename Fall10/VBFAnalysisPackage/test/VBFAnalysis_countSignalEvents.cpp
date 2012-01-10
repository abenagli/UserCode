#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "PUUtils.h"
#include "HiggsMassWindows.h"

#include <iostream>
#include <iomanip>
#include <fstream>

#include "TH1F.h"
#include "TGraph.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_countSignalEvents::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  float lumi = gConfigParser -> readFloatOption("Input::lumi");
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  std::vector<std::string> inputSigDirs = gConfigParser -> readStringListOption("Input::inputSigDirs");
  
  unsigned int nSigTrees = inputSigDirs.size();
  unsigned int nTotTrees = nSigTrees;
  
  std::vector<std::string> inputTotDirs;
  for(unsigned int i = 0; i < nSigTrees; ++ i) inputTotDirs.push_back(inputSigDirs.at(i));
  
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");

  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
  
  //[Options]
  int step    = gConfigParser -> readIntOption("Options::step");
  int PUScale = gConfigParser -> readIntOption("Options::PUScale");
  std::string pileupFileName = gConfigParser -> readStringOption("Options::pileupFileName");
  
  
  
  float xMin = 0.;
  float xMax = 1000.;
  float xWidth = GetBinWidth();
  int nBins = int((xMax-xMin)/xWidth);
  
  
  
  // Define tree variables
  float mH;
  int totEvents;
  int PUtrue_n;
  float eventWeight;
  float crossSection;
  float lepNuW_m_KF;
  
  
  
  // get the data pileup distribution
  TFile* inFile_pileup = TFile::Open(pileupFileName.c_str(),"READ");
  TH1F* distrPU_DATA = (TH1F*)( inFile_pileup->Get("pileup") );
  
  
  
  // Define the output graph
  std::vector<int> masses = GetMasses();
  unsigned int nMasses = masses.size();
  
  
  
  // Define counters and histograms
  std::map<int,float> S;
  std::map<int,float> ggS;
  std::map<int,float> qqS;
  
  std::map<int,TH1F*> h_S;
  std::map<int,TH1F*> h_ggS;
  std::map<int,TH1F*> h_qqS;
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    // build histograms
    char histoName[50];
    
    sprintf(histoName,"h_H%d",mass);
    h_S[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    
    sprintf(histoName,"h_ggH%d",mass);
    h_ggS[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    
    sprintf(histoName,"h_qqH%d",mass);
    h_qqS[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
  }
  
  
  
  // count S events
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if(i < nSigTrees) inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << ">>>>> VBFAnalysis_countSignalEvents::signal tree in " << inputSigDirs.at(i) << " opened" << std::endl;    
    
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue; 
    
    
    
    // get the mc pileup distribution
    TH1F* distrPU_MC = (TH1F*)( inputFile->Get("nPUtrue") );
    distrPU_MC -> Scale(1./distrPU_MC->Integral());
    
    
    
    // set tree branches
    tree -> SetBranchAddress("mH",           &mH);
    tree -> SetBranchAddress("eventWeight",  &eventWeight);
    tree -> SetBranchAddress("PUtrue_n",     &PUtrue_n);
    tree -> SetBranchAddress("totEvents",    &totEvents);
    tree -> SetBranchAddress("crossSection", &crossSection);
    tree -> SetBranchAddress("lepNuW_m_KF",  &lepNuW_m_KF);
    
    
    // fill counters and histograms
    for(int entry = 0; entry < tree->GetEntries(); ++entry)
    {
      tree -> GetEntry(entry);
      double weight = lumi * 1000 * 1. / totEvents * crossSection * PURescaleFactor(distrPU_DATA,distrPU_MC,PUtrue_n,PUScale) * eventWeight;
      
      int mass = int(int(mH)%1000);
      
      // count events
      if( (lepNuW_m_KF >= GetLepNuWMMIN(mass)) && (lepNuW_m_KF < GetLepNuWMMAX(mass)) )
      {
        S[mass] += weight;
        if( mH < 1000 ) ggS[mass] += weight;
        else            qqS[mass] += weight;
      }
      
      // fill histograms
      h_S[mass] -> Fill(lepNuW_m_KF,weight);
      if( mH < 1000 ) h_ggS[mass] -> Fill(lepNuW_m_KF,weight);
      else            h_qqS[mass] -> Fill(lepNuW_m_KF,weight);
    }

  inputFile -> Close(); 

  }
  
  
  
  // gluon-gluon fusion
  std::cout << ">>>>>> RESULTS FOR ggH <<<<<<" << std::endl;
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass
              << "      ggS: "      << std::fixed << std::setprecision(3) << std::setw(7) << ggS[mass]
              << std::endl;
  }
  
  // vector boson fusion
  std::cout << ">>>>>> RESULTS FOR qqH <<<<<<" << std::endl;
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass
              << "      qqS: "      << std::fixed << std::setprecision(3) << std::setw(7) << qqS[mass]
              << std::endl;
  }
  
  // gluon-gluon fusion + vector boson fusion
  std::cout << ">>>>>> RESULTS FOR ggH+qqH <<<<<<" << std::endl;
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass
              << "        S: "      << std::fixed << std::setprecision(3) << std::setw(7) << S[mass]
              << std::endl;
  }
  
  
  
  // save histograms
  outFile -> cd();
  
  for(unsigned int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    h_S[mass]   -> Write();
    h_ggS[mass] -> Write();
    h_qqS[mass] -> Write();
  }
  
  outFile -> Close();
 
  
  return 0;
}
