#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"

#include <iostream>

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
  std::string jetType      = gConfigParser -> readStringOption("Input::jetType");
  float lumi = gConfigParser -> readFloatOption("Input::lumi");
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  std::vector<std::string> inputSigDirs = gConfigParser -> readStringListOption("Input::inputSigDirs");
  
  unsigned int nSigTrees = inputSigDirs.size();
  unsigned int nTotTrees = nSigTrees;
  
  std::vector<std::string> inputTotDirs;
  for(unsigned int i = 0; i < nSigTrees; ++ i) inputTotDirs.push_back(inputSigDirs.at(i));
  
  
  //[Options]
  int entryFIRST  = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX    = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int step        = gConfigParser -> readIntOption("Options::step");
  
  int PUScale = gConfigParser -> readIntOption("Options::PUScale");
  
  
  
  // Define tree variables
  float mH;
  int totEvents;
  float crossSection;
  int PUit_n;
  int PUoot_n;
  float lepNuW_m;
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
  
  int* lepNuWMMIN = new int[nMasses];
  lepNuWMMIN[0] = 225;
  lepNuWMMIN[1] = 270;
  lepNuWMMIN[2] = 310;
  lepNuWMMIN[3] = 355;
  lepNuWMMIN[4] = 390;
  lepNuWMMIN[5] = 415;
  lepNuWMMIN[6] = 470;
  lepNuWMMIN[7] = 485;
  
  int* lepNuWMMAX = new int[nMasses];
  lepNuWMMAX[0] = 275;
  lepNuWMMAX[1] = 330;
  lepNuWMMAX[2] = 390;
  lepNuWMMAX[3] = 445;
  lepNuWMMAX[4] = 510;
  lepNuWMMAX[5] = 575;
  lepNuWMMAX[6] = 610;
  lepNuWMMAX[7] = 665;
  
  
  
  // Define counters
  std::map<int,float> S;
  std::map<int,float> S_KF;
  
  
  // count S events
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if(i < nSigTrees) inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::string token1,token2,token3;
    
    
    if( i < nSigTrees)
    {
      std::cout << ">>>>> VBFAnalysis_countSignalEvents::signal tree in " << inputSigDirs.at(i) << " opened" << std::endl;    
      std::istringstream iss(inputSigDirs.at(i));
      getline(iss,token1,'_');
      getline(iss,token2,'_');
      getline(iss,token3,'_');
    }
    
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue; 
    
    
    // set tree branches
    tree -> SetBranchAddress("mH",           &mH);
    tree -> SetBranchAddress("totEvents",    &totEvents);
    tree -> SetBranchAddress("crossSection", &crossSection);
    tree -> SetBranchAddress("PUit_n",       &PUit_n);
    tree -> SetBranchAddress("PUoot_n",      &PUoot_n);
    tree -> SetBranchAddress("lepNuW_m",     &lepNuW_m);
    tree -> SetBranchAddress("lepNuW_m_KF",  &lepNuW_m_KF);
    
    
    for(int entry = 0; entry < tree->GetEntries(); ++entry)
    {
      tree -> GetEntry(entry);
      double weight = lumi * 1000 * 1. / totEvents * crossSection * PURescaleFactor(PUit_n,PUScale);
      
        
      // loop on the masses
      for(int iMass = 0; iMass < nMasses; ++iMass)
      {
        int mass = masses[iMass];
        
        if( (mH > 0) && (lepNuW_m >= lepNuWMMIN[iMass]) && (lepNuW_m < lepNuWMMAX[iMass]) )
        {
          if( ( (token2 == "M-250") || (token3 == "M-250") ) && (mass == 250) ) S[250] += weight;
          if( ( (token2 == "M-300") || (token3 == "M-300") ) && (mass == 300) ) S[300] += weight;
          if( ( (token2 == "M-350") || (token3 == "M-350") ) && (mass == 350) ) S[350] += weight;
          if( ( (token2 == "M-400") || (token3 == "M-400") ) && (mass == 400) ) S[400] += weight;
          if( ( (token2 == "M-450") || (token3 == "M-450") ) && (mass == 450) ) S[450] += weight;
          if( ( (token2 == "M-500") || (token3 == "M-500") ) && (mass == 500) ) S[500] += weight;
          if( ( (token2 == "M-550") || (token3 == "M-550") ) && (mass == 550) ) S[550] += weight;
          if( ( (token2 == "M-600") || (token3 == "M-600") ) && (mass == 600) ) S[600] += weight;
	}
        
        if( (mH > 0) && (lepNuW_m_KF >= lepNuWMMIN[iMass]) && (lepNuW_m_KF < lepNuWMMAX[iMass]) )
        {
          if( ( (token2 == "M-250") || (token3 == "M-250") ) && (mass == 250) ) S_KF[250] += weight;
          if( ( (token2 == "M-300") || (token3 == "M-300") ) && (mass == 300) ) S_KF[300] += weight;
          if( ( (token2 == "M-350") || (token3 == "M-350") ) && (mass == 350) ) S_KF[350] += weight;
          if( ( (token2 == "M-400") || (token3 == "M-400") ) && (mass == 400) ) S_KF[400] += weight;
          if( ( (token2 == "M-450") || (token3 == "M-450") ) && (mass == 450) ) S_KF[450] += weight;
          if( ( (token2 == "M-500") || (token3 == "M-500") ) && (mass == 500) ) S_KF[500] += weight;
          if( ( (token2 == "M-550") || (token3 == "M-550") ) && (mass == 550) ) S_KF[550] += weight;
          if( ( (token2 == "M-600") || (token3 == "M-600") ) && (mass == 600) ) S_KF[600] += weight;
        }
      }
    }  
  }
  
  
  
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::cout << ">>> Higgs mass: " << mass << "   S: " << S[mass] << "   S_KF: " << S_KF[mass] << std::endl;
  }
  
  return 0;
}
