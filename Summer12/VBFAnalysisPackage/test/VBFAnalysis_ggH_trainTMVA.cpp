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
  int step   = gConfigParser -> readIntOption("Options::step");
  float lumi = gConfigParser -> readFloatOption("Options::lumi");
  int massDependentCUTS   = gConfigParser -> readIntOption("Options::massDependentCUTS");
  float lepNuWMMIN = gConfigParser -> readFloatOption("Options::lepNuWMMIN");
  float lepNuWMMAX = gConfigParser -> readFloatOption("Options::lepNuWMMAX");
  
  
  
  // Define tree variables
  float mH;
  int totEvents;
  float eventWeight;
  float crossSection;
  float PUWeight;
  
  

  
  
  
  // Define the TMVA factory
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_H" + higgsMass + ".root";
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
    tree -> SetBranchAddress("mH",           &mH);
    tree -> SetBranchAddress("totEvents",    &totEvents);
    tree -> SetBranchAddress("eventWeight",  &eventWeight);
    tree -> SetBranchAddress("crossSection", &crossSection);
    tree -> SetBranchAddress("PUWeight",     &PUWeight);
    tree -> GetEntry(0);
    
    
    // compute event weight at nth step
    double weight = lumi * 1000 * 1. * eventWeight / totEvents * crossSection * PUWeight;

    
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
  
  
  
  // Define variables
//  factory -> AddVariable("lepNuW_cphi", 'F');
//  factory -> AddVariable("lepNuZ_cphi", 'F');
  factory -> AddVariable("lep_ctheta",  'F');
  factory -> AddVariable("WJ1_ctheta",  'F');
  factory -> AddVariable("abs(lepNu_ctheta)",'F');
//  factory -> AddVariable("abs(WJ1_eta)",  'F');
//  factory -> AddVariable("abs(WJ2_eta)",  'F');
  factory -> AddVariable("abs(WJJ_eta)",  'F');
  factory -> AddVariable("abs(lep_eta)",  'F');
  factory -> AddVariable("lepMet_Dphi",  'F');
  factory -> AddVariable("WJJ_Dphi",  'F');
  
  TCut mycut;
  if ( massDependentCUTS == 1 ) mycut = Form("(WJJ_m > 65 && WJJ_m < 95) * (lepNuW_m_KF > %f && lepNuW_m_KF < %f )", lepNuWMMIN, lepNuWMMAX);
  else mycut = "(WJJ_m > 65 && WJJ_m < 95)";

  factory -> PrepareTrainingAndTestTree(mycut, mycut ,"SplitMode=Random");
  
  
  
  std::cout << "******************************************************" << std::endl;
  std::cout << "BookMethod" << std::endl;
  std::cout << "******************************************************" << std::endl;
  factory -> BookMethod(TMVA::Types::kCuts, "kCuts", "!H:!V:FitMethod=MC:EffSel:SampleSize=200000:VarProp=FSmart");
  //factory -> BookMethod(TMVA::Types::kCuts, "kCutsGA", "FitMethod=GA");
  //factory -> BookMethod(TMVA::Types::kLikelihood, "kLikelihood_H"+higgsMass);
  //factory -> BookMethod(TMVA::Types::kPDERS, "kPDERS");
  //factory -> BookMethod(TMVA::Types::kKNN, "kKNN_H"+higgsMass);
  //factory -> BookMethod(TMVA::Types::kFisher, "kFisher");
  //factory -> BookMethod(TMVA::Types::kLD, "kLD");
  //factory -> BookMethod(TMVA::Types::kCFMlpANN, "kCFMlpANN_H"+higgsMass);
  //factory -> BookMethod(TMVA::Types::kTMlpANN, "kTMlpANN_H"+higgsMass);
  //factory -> BookMethod(TMVA::Types::kMLP, "kMLP");
  //factory -> BookMethod(TMVA::Types::kSVM, "kSVM");
//   factory -> BookMethod( TMVA::Types::kBDT, "kBDT_H"+higgsMass,"!H:!V:VarTransform=G,N:NTrees=100:nEventsMin=10");
//  factory -> BookMethod( TMVA::Types::kBDT, "kBDT_H"+higgsMass,"!H:!V");
  
//   factory -> OptimizeAllMethods();
  
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
