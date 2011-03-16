#include "VBFAnalysisVariables.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>

#include "TH1F.h"
#include "TProfile.h"
#include "TGraph.h"
#include "TF1.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis_optimization::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string baseDir = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType       = gConfigParser -> readStringOption("Input::jetType");
  std::string step          = gConfigParser -> readStringOption("Input::step");
  
  std::string inputFileName = gConfigParser -> readStringOption("Input::inputFileName");
  std::vector<std::string> inputSigDirs = gConfigParser -> readStringListOption("Input::inputSigDirs");
  std::vector<std::string> inputBkgDirs = gConfigParser -> readStringListOption("Input::inputBkgDirs");

  unsigned int nSigTrees = inputSigDirs.size();
  unsigned int nBkgTrees = inputBkgDirs.size();
  unsigned int nTotTrees = nSigTrees + nBkgTrees;

  std::vector<std::string> inputTotDirs;
  for(unsigned int i = 0; i < nSigTrees; ++ i) inputTotDirs.push_back(inputSigDirs.at(i));
  for(unsigned int i = 0; i < nBkgTrees; ++ i) inputTotDirs.push_back(inputBkgDirs.at(i));
  
  
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+"_"+jetAlgorithm+".root";
  
    
  int entryFIRST = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  
  
  
  
  
  
  // Define tree variables
  int totEvents;
  float crossSection;
  float jets_bTag1;  
  float jets_bTag2;  
  
  
  
  // define set of cuts
  std::vector<std::vector<float> > cuts;
  for(float bTag1 = 20.; bTag1 > -10.; bTag1-=0.20)
    for(float bTag2 = bTag1; bTag2 > -10.; bTag2-=0.20)
    {  
      std::vector<float> dummy;
      dummy.push_back(bTag1);
      dummy.push_back(bTag2);
      
      cuts.push_back(dummy);  
    }
  
  
  
  // define event counters
  std::vector<std::vector<float>* > nSigEvents;
  std::vector<std::vector<float>* > nBkgEvents;
  
  for(unsigned int cutsIt = 0; cutsIt < cuts.size(); ++cutsIt)
  {
    nSigEvents.push_back( new std::vector<float>(nSigTrees) );
    nBkgEvents.push_back( new std::vector<float>(nBkgTrees) );
  }
  
  
  
  
  
  
  // loop over samples
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if(i < nSigTrees) inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    else              inputFullFileName = baseDir + "/" + inputBkgDirs.at(i-nSigTrees) + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << ">>> VBFAnalysis_optimization::Open " << inputFullFileName << std::endl;
    
    
    // get the tree at nth step
    TTree* tree = NULL;
    std::string treeName = "ntu_"+step;    
    inputFile -> GetObject(treeName.c_str(), tree);
    
    
    // get the events histogram
    TH1F* events;
    inputFile -> GetObject("events", events);     
    totEvents = events->GetBinContent(1);
    
    // set tree branches
    tree -> SetBranchAddress("crossSection", &crossSection);
    tree -> SetBranchAddress("jets_bTag1", &jets_bTag1);
    tree -> SetBranchAddress("jets_bTag2", &jets_bTag2);
    
    
    // compute event weight
    tree -> GetEntry(0);
    double weight = 1000. / totEvents * crossSection;
  
    
    // loop over the events
    for(int entry = 0; entry < tree->GetEntries(); ++entry)
    {
      tree -> GetEntry(entry);
      
      
      for(unsigned int cutsIt = 0; cutsIt < cuts.size(); ++cutsIt)
      {
        if( jets_bTag1 > (cuts.at(cutsIt)).at(0) ) continue;
        if( jets_bTag2 > (cuts.at(cutsIt)).at(1) ) continue;

        if( i < nSigTrees ) (nSigEvents.at(cutsIt))->at(i) += 1. * weight;
        else                (nBkgEvents.at(cutsIt))->at(i-nSigTrees) += 1. * weight;
      }
    }
    
  } // loop over samples
  
  
  
  
  
  // fill output tree
  TFile* outputRootFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  outputRootFile -> cd();

  TTree* outputTree = new TTree("ntu", "ntu");
  
  float cut_bTag1;
  float cut_bTag2;
  float nSig;
  float nBkg;

  outputTree -> Branch("cut_bTag1", &cut_bTag1);
  outputTree -> Branch("cut_bTag2", &cut_bTag2);
  outputTree -> Branch("nSig", &nSig);
  outputTree -> Branch("nBkg", &nBkg);
  
  for(unsigned int cutsIt = 0; cutsIt < cuts.size(); ++cutsIt)
  {
    cut_bTag1 = (cuts.at(cutsIt)).at(0);
    cut_bTag2 = (cuts.at(cutsIt)).at(1);
    
    nSig = 0.;
    for(unsigned int i = 0; i < nSigTrees; ++i)
      nSig += (nSigEvents.at(cutsIt)) -> at(i);
    
    nBkg = 0.;
    for(unsigned int i = 0; i < nBkgTrees; ++i)
      nBkg += (nBkgEvents.at(cutsIt)) -> at(i);
    
    outputTree -> Fill();
  }
  
  
  
  
  
  
  // define iso-significance curves
  TF1* sig_01 = new TF1("sig_01", "(x*x)/(0.1*0.1)", 0., 100.);
  sig_01 -> SetLineStyle(2);
  sig_01 -> SetLineWidth(1);
  
  TF1* sig_025 = new TF1("sig_025", "(x*x)/(0.25*0.25)", 0., 100.);
  sig_025 -> SetLineStyle(2);
  sig_025 -> SetLineWidth(1);
  
  TF1* sig_05 = new TF1("sig_05", "(x*x)/(0.5*0.5)", 0., 100.);
  sig_05 -> SetLineStyle(2);
  sig_05 -> SetLineWidth(1);
  
  TF1* sig_075 = new TF1("sig_075", "(x*x)/(0.75*0.75)", 0., 100.);
  sig_075 -> SetLineStyle(2);
  sig_075 -> SetLineWidth(1);
  
  TF1* sig_1 = new TF1("sig_1", "(x*x)/(1.0*1.0)", 0., 100.);
  sig_1 -> SetLineColor(kRed);
  sig_1 -> SetLineStyle(1);
  sig_1 -> SetLineWidth(1);
  
  TF1* sig_15 = new TF1("sig_15", "(x*x)/(1.5*1.5)", 0., 100.);
  sig_15 -> SetLineStyle(2);
  sig_15 -> SetLineWidth(1);
  
  TF1* sig_2 = new TF1("sig_2", "(x*x)/(2.0*2.0)", 0., 100.);
  sig_2 -> SetLineStyle(2);
  sig_2 -> SetLineWidth(1);
  
  TF1* sig_3 = new TF1("sig_3", "(x*x)/(3.0*3.0)", 0., 100.);
  sig_3 -> SetLineStyle(2);
  sig_3 -> SetLineWidth(1);
  
  
  
  
  
  // save histograms
  outputTree -> Write();
  sig_01 -> Write();
  sig_025 -> Write();
  sig_05 -> Write();
  sig_075 -> Write();
  sig_1 -> Write();
  sig_15 -> Write();
  sig_2 -> Write();
  sig_3 -> Write();
  
  outputRootFile -> Close();
  
  delete outputRootFile;
  return 0;
}
