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
    std::cerr << ">>>>> VBFAnalysis_optimizeHiggsMassWindow::usage: " << argv[0] << " configFileName" << std::endl ;
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
  std::vector<std::string> inputBkgDirs = gConfigParser -> readStringListOption("Input::inputBkgDirs");
  
  unsigned int nSigTrees = inputSigDirs.size();
  unsigned int nBkgTrees = inputBkgDirs.size();
  unsigned int nTotTrees = nSigTrees + nBkgTrees;
  
  std::vector<std::string> inputTotDirs;
  for(unsigned int i = 0; i < nSigTrees; ++ i) inputTotDirs.push_back(inputSigDirs.at(i));
  for(unsigned int i = 0; i < nBkgTrees; ++ i) inputTotDirs.push_back(inputBkgDirs.at(i));
  
  
  //[Output]
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  
  //[Options]
  int entryFIRST  = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX    = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int step        = gConfigParser -> readIntOption("Options::step");
  
  
  
  // Define tree variables
  float mH;
  int totEvents;
  float crossSection;
  int PUit_n;
  int PUoot_n;
  float lepNuW_m;
  float lepNuW_m_KF;
  
  
  
  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
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
  
  
  std::map<int,TGraph*> g_significance;
  std::map<int,TGraph*> g_significance_KF;
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    g_significance[mass] = new TGraph();
    g_significance_KF[mass] = new TGraph();
  }
  
  
  
  // Define counters
  int nPoints = 250;
  std::map<int,std::map<float,float> > S;
  std::map<int,std::map<float,float> > B;
  std::map<int,std::map<float,float> > S_KF;
  std::map<int,std::map<float,float> > B_KF;
  
  // count S and B events
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if(i < nSigTrees) inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    else              inputFullFileName = baseDir + "/" + inputBkgDirs.at(i-nSigTrees) + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::string token1,token2,token3;
    
    
    if( i < nSigTrees)
    {
      std::cout << ">>>>> VBFAnalysis_optimizeHiggsMassWindow::signal tree in " << inputSigDirs.at(i) << " opened" << std::endl;    
      std::istringstream iss(inputSigDirs.at(i));
      getline(iss,token1,'_');
      getline(iss,token2,'_');
      getline(iss,token3,'_');
    }
    else
    {
      std::cout << ">>>>> VBFAnalysis_optimizeHiggsMassWindow::background tree in " << inputBkgDirs.at(i-nSigTrees) << " opened" << std::endl;
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
      double weight = lumi * 1000 * 1. / totEvents * crossSection * PURescaleFactor(PUit_n); 
      
      for(int point = 0; point < nPoints; ++point)
      {
        float window = 0. + point*250./nPoints;
        
        // loop on the masses
        for(int iMass = 0; iMass < nMasses; ++iMass)
        {
          int mass = masses[iMass];
          
          if( (lepNuW_m >= mass-window) && (lepNuW_m < mass+window) )
          {
            if( mH > 0 )
            {
              if( ( (token2 == "M-250") || (token3 == "M-250") ) && (mass == 250) ) (S[250])[window] += weight;
              if( ( (token2 == "M-300") || (token3 == "M-300") ) && (mass == 300) ) (S[300])[window] += weight;
              if( ( (token2 == "M-350") || (token3 == "M-350") ) && (mass == 350) ) (S[350])[window] += weight;
              if( ( (token2 == "M-400") || (token3 == "M-400") ) && (mass == 400) ) (S[400])[window] += weight;
              if( ( (token2 == "M-450") || (token3 == "M-450") ) && (mass == 450) ) (S[450])[window] += weight;
              if( ( (token2 == "M-500") || (token3 == "M-500") ) && (mass == 500) ) (S[500])[window] += weight;
              if( ( (token2 == "M-550") || (token3 == "M-550") ) && (mass == 550) ) (S[550])[window] += weight;
              if( ( (token2 == "M-600") || (token3 == "M-600") ) && (mass == 600) ) (S[600])[window] += weight;
	    }
            else (B[mass])[window] += weight;
          }
          
          if( (lepNuW_m_KF >= mass-window) && (lepNuW_m_KF < mass+window) )
          {
            if( mH > 0 )
            {
              if( ( (token2 == "M-250") || (token3 == "M-250") ) && (mass == 250) ) (S_KF[250])[window] += weight;
              if( ( (token2 == "M-300") || (token3 == "M-300") ) && (mass == 300) ) (S_KF[300])[window] += weight;
              if( ( (token2 == "M-350") || (token3 == "M-350") ) && (mass == 350) ) (S_KF[350])[window] += weight;
              if( ( (token2 == "M-400") || (token3 == "M-400") ) && (mass == 400) ) (S_KF[400])[window] += weight;
              if( ( (token2 == "M-450") || (token3 == "M-450") ) && (mass == 450) ) (S_KF[450])[window] += weight;
              if( ( (token2 == "M-500") || (token3 == "M-500") ) && (mass == 500) ) (S_KF[500])[window] += weight;
              if( ( (token2 == "M-550") || (token3 == "M-550") ) && (mass == 550) ) (S_KF[550])[window] += weight;
              if( ( (token2 == "M-600") || (token3 == "M-600") ) && (mass == 600) ) (S_KF[600])[window] += weight;
	    }
            else (B_KF[mass])[window] += weight;
	  }
        }
      }
    }    
  }
  
  
  
  outFile -> cd();
  
  for(int iMass = 0; iMass < nMasses; ++iMass)
  {
    int mass = masses[iMass];
    
    std::map<float,float>::const_iterator Sit = (S[mass]).begin();
    std::map<float,float>::const_iterator Bit = (B[mass]).begin();
    std::map<float,float>::const_iterator Sit_KF = (S_KF[mass]).begin();
    std::map<float,float>::const_iterator Bit_KF = (B_KF[mass]).begin();
    
    
    int point = 0;
    while( (Sit != (S[mass]).end()) && (Bit != (B[mass]).end()) )
    {
      g_significance[mass] -> SetPoint(point,Sit->first,Sit->second/sqrt(Sit->second+Bit->second));
      g_significance_KF[mass] -> SetPoint(point,Sit_KF->first,Sit_KF->second/sqrt(Sit_KF->second+Bit_KF->second));
      ++Sit;
      ++Bit;
      ++Sit_KF;
      ++Bit_KF;
      ++point;
    }
    
    std::stringstream graphName;
    graphName << "g_significance_" << mass; 
    g_significance[mass] -> Write(graphName.str().c_str());

    std::stringstream graphName_KF;
    graphName_KF << "g_significance_KF_" << mass; 
    g_significance_KF[mass] -> Write(graphName_KF.str().c_str());
  }
  
  outFile -> Close();
  
  
  return 0;
}
