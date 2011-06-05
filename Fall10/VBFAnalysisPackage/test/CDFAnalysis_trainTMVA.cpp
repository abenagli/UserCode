#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>
#include <iostream>
#include <cstdlib>

#include "TH1F.h"
#include "TProfile.h"
#include "TObject.h"
#include "TCut.h"

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/Config.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> CDFAnalysis_trainMVA::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType      = gConfigParser -> readStringOption("Input::jetType");
  
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
  int totEvents;
  float crossSection;
  
  
  
  
  
  
  // Define the TMVA factory
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  TMVA::gConfig().GetIONames().fWeightFileDir = outputRootFilePath+"/weights/";
  TMVA::Factory* factory = new TMVA::Factory("TMVA", outFile);
  
  
  // add trees to the factory
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if(i < nSigTrees) inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    else              inputFullFileName = baseDir + "/" + inputBkgDirs.at(i-nSigTrees) + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    if ( tree -> GetEntries() == 0 ) continue; 
    
    
    // set tree branches
    tree -> SetBranchAddress("totEvents",    &totEvents);
    tree -> SetBranchAddress("crossSection", &crossSection);
    tree -> GetEntry(0);
    
    
    // compute event weight at nth step
    double weight = 1. * tree -> GetEntries() / totEvents * crossSection; 
    
    
    // add tree to the factory
    if(i < nSigTrees)
    {
      factory -> AddSignalTree(tree, weight);
      std::cout << ">>>>> CDFAnalysis_trainMVA:: signal tree in " << inputSigDirs.at(i) << " added to the factory" << std::endl;
    }
    
    else
    {
      factory -> AddBackgroundTree(tree, weight);    
      std::cout << ">>>>> CDFAnalysis_trainMVA:: background tree in " << inputBkgDirs.at(i-nSigTrees) << " added to the factory" << std::endl;
    }
    
  }
  
  

  // Define variables
  factory -> AddVariable("WJJ_Deta",      'F');
  factory -> AddVariable("WJJ_Dphi",      'F');
  factory -> AddVariable("WJJ_et",        'F');
  factory -> AddVariable("WJJ_m",         'F');
  factory -> AddVariable("lepMetW_Dphi",  'F');
  factory -> AddVariable("met_et",        'F');
  factory -> AddVariable("lepWJJ_pt1",    'F');
  factory -> AddVariable("lepWJJ_pt2",    'F');
  factory -> AddVariable("lepWJJ_pt3",    'F');
  
  factory -> PrepareTrainingAndTestTree("", "SplitMode=Random:nTrain_Signal=2000:nTrain_Background=2000:nTest_Signal=0:nTest_Background=0");
  
  
  
  std::cout << "******************************************************" << std::endl;
  std::cout << "BookMethod" << std::endl;
  std::cout << "******************************************************" << std::endl;
  char methodName[50];
  //factory -> BookMethod(TMVA::Types::kCuts, "kCuts");
  factory -> BookMethod(TMVA::Types::kBDT,"kBDT");
  factory -> BookMethod(TMVA::Types::kLikelihood, "kLikelihood");
  factory -> BookMethod(TMVA::Types::kPDERS, "kPDERS");
  factory -> BookMethod(TMVA::Types::kKNN, "kKNN");
  factory -> BookMethod(TMVA::Types::kFisher, "kFisher");
  factory -> BookMethod(TMVA::Types::kLD, "kLD");
  factory -> BookMethod(TMVA::Types::kCFMlpANN, "kCFMlpANN");
  //factory -> BookMethod(TMVA::Types::kTMlpANN, "kTMlpANN");
  //factory -> BookMethod(TMVA::Types::kMLP, "kMLP");
  //factory -> BookMethod(TMVA::Types::kSVM, "kSVM");
  
  std::cout << "******************************************************" << std::endl;
  std::cout << "TrainAllMethods" << std::endl;
  std::cout << "******************************************************" << std::endl;
  factory -> TrainAllMethods();
  
  std::cout << "******************************************************" << std::endl;
  std::cout << "TestAllMethods" << std::endl;
  std::cout << "******************************************************" << std::endl;
  factory -> TestAllMethods();
  
  std::cout << "******************************************************" << std::endl;
  std::cout << "EvaluateAllMethods" << std::endl;
  std::cout << "******************************************************" << std::endl;
  factory -> EvaluateAllMethods();
  
  
  
  return 0;
}
