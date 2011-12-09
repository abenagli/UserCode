#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "HiggsMassWindows.h"

#include <iostream>
#include <iomanip>
#include <fstream>

#include "TH1F.h"
#include "TGraph.h"



int nBins = 200;
float xMin = 0.;
float xMax = 1000.;
float xWidth = (xMax-xMin)/nBins;






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
  std::string jetType      = gConfigParser -> readStringOption("Input::jetType");
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
  
  
  
  // Define tree variables
  float mH;
  int totEvents;
  float eventWeight;
  float crossSection;
  int PUit_n;
  int PUoot_n;
  float lepNuW_m_KF;
  
  
  
  // Define the output graph
  int nMasses = 8;
  
  int* masses = new int[nMasses];
  masses[0] = 250;
  masses[1] = 300;
  masses[2] = 350;
  masses[3] = 400;
  masses[4] = 450;
  masses[5] = 500;
  masses[6] = 550;
  masses[7] = 600;
  
  
  
  // Define counters and histograms
  std::map<int,float> ggS;
  std::map<int,float> qqS;
  
  std::map<int,TH1F*> h_ggS;
  std::map<int,TH1F*> h_qqS;
  
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    char histoName[50];
    
    sprintf(histoName,"ggH%d",mass);
    h_ggS[mass] = new TH1F(histoName,"",nBins,xMin,xMax);
    
    sprintf(histoName,"qqH%d",mass);
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
    
    
    // set tree branches
    tree -> SetBranchAddress("mH",           &mH);
    tree -> SetBranchAddress("eventWeight",  &eventWeight);
    tree -> SetBranchAddress("totEvents",    &totEvents);
    tree -> SetBranchAddress("crossSection", &crossSection);
    tree -> SetBranchAddress("PUit_n",       &PUit_n);
    tree -> SetBranchAddress("PUoot_n",      &PUoot_n);
    tree -> SetBranchAddress("lepNuW_m_KF",  &lepNuW_m_KF);
    
    
    // fill counters and histograms
    for(int entry = 0; entry < tree->GetEntries(); ++entry)
    {
      tree -> GetEntry(entry);
      double weight = lumi * 1000 * 1. / totEvents * crossSection * PURescaleFactor(PUit_n,PUScale) * eventWeight;
      
      int mass = int(int(mH)%1000);
      
      if( (lepNuW_m_KF >= GetLepNuWMMIN(mass)) && (lepNuW_m_KF < GetLepNuWMMAX(mass)) )
      {
        if( mH < 1000 )
        {
          ggS[mass] += weight;
          h_ggS[mass] -> Fill(lepNuW_m_KF,weight);
	}
        
        else
        {
          qqS[mass] += weight;
          h_qqS[mass] -> Fill(lepNuW_m_KF,weight);
	}
      }
    }

  inputFile -> Close(); 

  }
  
  
  
  // gluon-gluon fusion
  std::cout << ">>>>>> RESULTS FOR ggH <<<<<<" << std::endl;
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass
              << "      ggS: "      << std::fixed << std::setprecision(3) << std::setw(7) << ggS[mass]
              << std::endl;
  }
  
  // vector boson fusion
  std::cout << ">>>>>> RESULTS FOR qqH <<<<<<" << std::endl;
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass
              << "      qqS: "      << std::fixed << std::setprecision(3) << std::setw(7) << qqS[mass]
              << std::endl;
  }
  
  // gluon-gluon fusion + vector boson fusion
  std::cout << ">>>>>> RESULTS FOR ggH+qqH <<<<<<" << std::endl;
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass
              << "        S: "      << std::fixed << std::setprecision(3) << std::setw(7) << ggS[mass]+qqS[mass]
              << std::endl;
  }
  
  
  
  // save histograms
  outFile -> cd();
  
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    h_ggS[mass] -> Write();
    h_qqS[mass] -> Write();
  }
  
  outFile -> Close();
 
  
  return 0;
}
