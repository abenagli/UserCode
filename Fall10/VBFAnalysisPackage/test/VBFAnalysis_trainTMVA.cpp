#include "WplusWminusRatioVariables.h"
#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>

#include "TH1F.h"
#include "TProfile.h"
#include "TObject.h"
#include "TCut.h"

#include "TMVA/Factory.h"
#include "TMVA/Reader.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_trainMVA::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  
  //[Input]
  std::string baseDir      = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType      = gConfigParser -> readStringOption("Input::jetType");
  std::string higgsMass    = gConfigParser -> readStringOption("Input::higgsMass");
  
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
  int step        = gConfigParser -> readFloatOption("Options::step");
  
  
  
  // Define tree variables
  int totEvents;
  float mH;
  float crossSection;
  
  
  
  
  
  
  // Define the TMVA factory
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + higgsMass + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  TMVA::Factory* factory = new TMVA::Factory("TMVA", outFile);
  
  
  
  
  
  
  // add trees to the factory
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if(i < nSigTrees) inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    else              inputFullFileName = baseDir + "/" + inputBkgDirs.at(i-1) + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "tree_%d", step);
    inputFile -> GetObject(treeName, tree);
    
    
    // set tree branches
    tree -> SetBranchAddress("totEvents",    &totEvents);
    tree -> SetBranchAddress("mH",           &mH);
    tree -> SetBranchAddress("crossSection", &crossSection);
    tree -> GetEntry(0);
    
    
    // compute event weight at nth step
    double weight = 1. * tree -> GetEntries() / totEvents * crossSection; 
    
    
    // add tree to the factory
    if(i < nSigTrees)
    {
      factory -> AddSignalTree(tree, weight);
      std::cout << ">>>>> VBFAnalysis_trainMVA:: signal tree in " << inputSigDirs.at(i) << " added to the factory" << std::endl;
    }
    
    else
    {
      factory -> AddBackgroundTree(tree, weight);    
      std::cout << ">>>>> VBFAnalysis_trainMVA:: background tree in " << inputBkgDirs.at(i-1) << " added to the factory" << std::endl;
    }
    
  }
  
  
  
  
  
  
  // Define variables
  factory -> AddVariable("PV_nTracks",    'F');
  factory -> AddVariable("tagJJ_Deta",    'F');
  factory -> AddVariable("tagJJ_m",       'F');
  factory -> AddVariable("WJJ_m",         'F');
  factory -> AddVariable("lepMetW_Dphi",  'F');
  factory -> AddVariable("lepNuW_m",      'F');
  factory -> AddVariable("lep_eta",       'F');
  
  factory -> PrepareTrainingAndTestTree("", "SplitMode=Random");
  
  std::cout << "******************************************************" << std::endl;
  std::cout << "BookMethod" << std::endl;
  std::cout << "******************************************************" << std::endl;
  //factory -> BookMethod(TMVA::Types::kCuts, "kCuts");
  factory -> BookMethod(TMVA::Types::kLikelihood, "kLikelihood");
  factory -> BookMethod(TMVA::Types::kPDERS, "kPDERS");
  factory -> BookMethod(TMVA::Types::kKNN, "kKNN");
  factory -> BookMethod(TMVA::Types::kFisher, "kFisher");
  //factory -> BookMethod(TMVA::Types::kLD, "kLD");
  //factory -> BookMethod(TMVA::Types::kCFMlpANN, "kCFMlpANN");
  //factory -> BookMethod(TMVA::Types::kTMlpANN, "kTMlpANN");
  factory -> BookMethod(TMVA::Types::kMLP, "kMLP");
  factory -> BookMethod(TMVA::Types::kSVM, "kSVM");
  factory -> BookMethod(TMVA::Types::kBDT, "kBDT");

  
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

