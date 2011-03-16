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
  int step        = gConfigParser -> readIntOption("Options::step");
  
  
  
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
    else              inputFullFileName = baseDir + "/" + inputBkgDirs.at(i-nSigTrees) + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << "\n\n sto leggendo il file --->> " << inputFullFileName.c_str() << "\n\n" << std::endl;
    
    // Get the number of events BEFORE the selection
    TH1F* events = (TH1F*) inputFile -> Get("events");
    totEvents = events -> GetBinContent(1);
  
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    
    // set tree branches
    tree -> SetBranchAddress("mH",           &mH);
    tree -> SetBranchAddress("crossSection", &crossSection);
    tree -> GetEntry(0);
    
    // Do not consider this contribution IF at the level of the AN before the MVA step no events are surviving
    if ( tree -> GetEntries() == 0 ) continue; 

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
      std::cout << ">>>>> VBFAnalysis_trainMVA:: background tree in " << inputBkgDirs.at(i-nSigTrees) << " added to the factory" << std::endl;
    }
    
  }
  
  std::cout << "sono qua 1" << std::endl;

  // Define variables
  factory -> AddVariable("jets_bTag1",    'F');
  factory -> AddVariable("jets_bTag1",    'F');
  factory -> AddVariable("met.Et()",      'F');
  factory -> AddVariable("lepMet_mt",     'F');
  factory -> AddVariable("lepMet_Dphi",   'F');
  factory -> AddVariable("WJJ_m",         'F');
  factory -> AddVariable("WJJ_DR",        'F');

  factory -> AddVariable("lepMetW_Dphi",  'F');
  factory -> AddVariable("lepWJJ_pt1",    'F');
  factory -> AddVariable("lepWJJ_pt2",    'F');
  factory -> AddVariable("lepWJJ_pt3",    'F');
  factory -> AddVariable("lepNuW_m",      'F');

  factory -> AddVariable("tagJJ_Deta",    'F');
  factory -> AddVariable("tagJJ_m",       'F');

  factory -> AddVariable("WJ1_zepp",      'F');
  factory -> AddVariable("WJ2_zepp",      'F');
  factory -> AddVariable("lep_zepp",      'F');
  
  factory -> AddVariable("lep.Eta()",     'F');

  std::cout << "sono qua 2" << std::endl;
  factory -> PrepareTrainingAndTestTree("", "SplitMode=Random");
  std::cout << "sono qua 3" << std::endl;

  std::cout << "******************************************************" << std::endl;
  std::cout << "BookMethod" << std::endl;
  std::cout << "******************************************************" << std::endl;
  //factory -> BookMethod(TMVA::Types::kCuts, "kCuts");
//  factory -> BookMethod(TMVA::Types::kLikelihood, "kLikelihood");
//  factory -> BookMethod(TMVA::Types::kPDERS, "kPDERS");
//  factory -> BookMethod(TMVA::Types::kKNN, "kKNN");
//  factory -> BookMethod(TMVA::Types::kFisher, "kFisher");
  //factory -> BookMethod(TMVA::Types::kLD, "kLD");
  //factory -> BookMethod(TMVA::Types::kCFMlpANN, "kCFMlpANN");
  //factory -> BookMethod(TMVA::Types::kTMlpANN, "kTMlpANN");
//  factory -> BookMethod(TMVA::Types::kMLP, "kMLP");
//  factory -> BookMethod(TMVA::Types::kSVM, "kSVM");
  factory -> BookMethod(TMVA::Types::kBDT, "kBDT");

  std::cout << "sono qua 4" << std::endl;

  std::cout << "******************************************************" << std::endl;
  std::cout << "TrainAllMethods" << std::endl;
  std::cout << "******************************************************" << std::endl;
  factory -> TrainAllMethods();
  
  std::cout << "sono qua 5" << std::endl;

  std::cout << "******************************************************" << std::endl;
  std::cout << "TestAllMethods" << std::endl;
  std::cout << "******************************************************" << std::endl;
  factory -> TestAllMethods();
  std::cout << "sono qua 6" << std::endl;
  
  std::cout << "******************************************************" << std::endl;
  std::cout << "EvaluateAllMethods" << std::endl;
  std::cout << "******************************************************" << std::endl;
  factory -> EvaluateAllMethods();
  std::cout << "sono qua 7" << std::endl;

  
  return 0;
}

