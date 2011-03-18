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
  std::string baseDir           = gConfigParser -> readStringOption("Input::baseDir");
  std::string jetAlgorithm      = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType           = gConfigParser -> readStringOption("Input::jetType");
  std::string higgsMass         = gConfigParser -> readStringOption("Input::higgsMass");
  std::string inputweightfolder = gConfigParser -> readStringOption("Input::inputweightfolder");
  
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
  float lumiInt   = gConfigParser -> readIntOption("Options::lumiInt"); //in pb-1
  
  
  // Define tree variables
  int totEvents;
  float mH;
  float crossSection;
  
  //Prepare variables to read the tree
  Float_t jets_bTag1, 
  jets_bTag2,
  lep_eta,
  met_et,
  lepMet_mt,
  lepMet_Dphi,
  WJJ_m,
  WJJ_DR,
  lepMetW_Dphi,
  lepWJJ_pt1,
  lepWJJ_pt2,
  lepWJJ_pt3,
  lepNuW_m,
  tagJJ_Deta,
  tagJJ_m,
  WJ1_zepp,
  WJ2_zepp,
  lep_zepp;
  
  // define MVA reader
  TMVA::Reader* MVAReader = new TMVA::Reader();
  MVAReader -> AddVariable("jets_bTag1"  ,&jets_bTag1);
  MVAReader -> AddVariable("jets_bTag2"  ,&jets_bTag2);
  MVAReader -> AddVariable("lep_eta"     ,&lep_eta);
  MVAReader -> AddVariable("met_et"      ,&met_et);
  MVAReader -> AddVariable("lepMet_mt"   ,&lepMet_mt);
  MVAReader -> AddVariable("lepMet_Dphi" ,&lepMet_Dphi);
  MVAReader -> AddVariable("WJJ_m"       ,&WJJ_m);
  MVAReader -> AddVariable("WJJ_DR"      ,&WJJ_DR);
  
  MVAReader -> AddVariable("lepMetW_Dphi",&lepMetW_Dphi);
  MVAReader -> AddVariable("lepWJJ_pt1"  ,&lepWJJ_pt1);
  MVAReader -> AddVariable("lepWJJ_pt2"  ,&lepWJJ_pt2);
  MVAReader -> AddVariable("lepWJJ_pt3"  ,&lepWJJ_pt3);
  MVAReader -> AddVariable("lepNuW_m"    ,&lepNuW_m);
  
  MVAReader -> AddVariable("tagJJ_Deta"  ,&tagJJ_Deta);
  MVAReader -> AddVariable("tagJJ_m"     ,&tagJJ_m);
  
  MVAReader -> AddVariable("WJ1_zepp"    ,&WJ1_zepp);
  MVAReader -> AddVariable("WJ2_zepp"    ,&WJ2_zepp);
  MVAReader -> AddVariable("lep_zepp"    ,&lep_zepp);  
  
  TString methodName = "kBDT method";
  std::string weightfile = inputweightfolder + "/TMVA_kBDT_H" + higgsMass + ".weights.xml";
  
  MVAReader->BookMVA( methodName, weightfile.c_str() );
  
  // define the MVA interest range and step
  float theMvaValMin      = -0.4;
  float theMvaValMax      =  0.4;
  const int theMvaValBins = 100;
  float theMvaValStep     = (theMvaValMax - theMvaValMin)/theMvaValBins;
  
  // Define the output file
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + higgsMass + ".root";
  TFile* outFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  
  // Define the scaled number of signal and background events after the mva selection
  float n_signal[theMvaValBins] = { 0 };
  float n_background[theMvaValBins] = { 0 };
  
  // Define the output histogram
  TH1F* h_signal = new TH1F("h_signal", "h_signal", theMvaValBins, theMvaValMin, theMvaValMax); 
  TH1F* h_background = new TH1F("h_background", "h_background", theMvaValBins, theMvaValMin, theMvaValMax); 
  TH1F* h_significance = new TH1F("h_significance", "h_significance", theMvaValBins, theMvaValMin, theMvaValMax); 
  
  // add trees to the reader
  for(unsigned int i = 0; i < nTotTrees; ++i)
  {
    // open root file
    std::string inputFullFileName;
    if(i < nSigTrees) inputFullFileName = baseDir + "/" + inputSigDirs.at(i)   + "/" + inputFileName + ".root";
    else              inputFullFileName = baseDir + "/" + inputBkgDirs.at(i-nSigTrees) + "/" + inputFileName + ".root";
    TFile* inputFile = TFile::Open(inputFullFileName.c_str());
    std::cout << "\n\n sto leggendo il file --->> " << inputFullFileName.c_str() << "\n\n" << std::endl;
    
    // get the tree at nth step
    TTree* tree = NULL;
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    inputFile -> GetObject(treeName, tree);
    
    // Get the number of events BEFORE the selection
    TH1F* events = (TH1F*) inputFile -> Get("events");
    totEvents = events -> GetBinContent(1);
    
    // set tree branches
    tree -> SetBranchAddress("mH",           &mH);
    tree -> SetBranchAddress("crossSection", &crossSection);
    //    tree -> SetBranchAddress("totEvents",    &totEvents);
    tree -> GetEntry(0);
    
    // Do not consider this contribution IF at the level of the AN before the MVA step no events are surviving
    if ( tree -> GetEntries() == 0 ) continue; 
    
    // compute event weight at nth step
    double weight = 1. / totEvents * crossSection * lumiInt; 
    
    
    tree -> SetBranchAddress("jets_bTag1", &jets_bTag1);
    tree -> SetBranchAddress("jets_bTag2", &jets_bTag2);
    tree -> SetBranchAddress("lep_eta", &lep_eta);
    tree -> SetBranchAddress("met_et", &met_et);
    tree -> SetBranchAddress("lepMet_mt", &lepMet_mt);
    tree -> SetBranchAddress("lepMet_Dphi", &lepMet_Dphi);
    tree -> SetBranchAddress("WJJ_m", &WJJ_m);
    tree -> SetBranchAddress("WJJ_DR", &WJJ_DR);
    tree -> SetBranchAddress("lepMetW_Dphi", &lepMetW_Dphi);
    tree -> SetBranchAddress("lepWJJ_pt1", &lepWJJ_pt1);
    tree -> SetBranchAddress("lepWJJ_pt2", &lepWJJ_pt2);
    tree -> SetBranchAddress("lepWJJ_pt3", &lepWJJ_pt3);
    tree -> SetBranchAddress("lepNuW_m", &lepNuW_m);
    tree -> SetBranchAddress("tagJJ_Deta", &tagJJ_Deta);
    tree -> SetBranchAddress("tagJJ_m", &tagJJ_m);
    tree -> SetBranchAddress("WJ1_zepp", &WJ1_zepp);
    tree -> SetBranchAddress("WJ2_zepp", &WJ2_zepp);
    tree -> SetBranchAddress("lep_zepp", &lep_zepp);
    
    int tree_entries = tree -> GetEntries();
    std::cout << "leggo tot eventi " << tree_entries << std::endl;
    
    for ( int jentry=0; jentry< tree_entries; jentry++)
    {
      tree -> GetEntry(jentry);
      
      float theMvaVal = MVAReader -> EvaluateMVA("kBDT method");  
      
      //now cycle on different mva cut values:
      float theMvaCutVal = theMvaValMin;
      int theBin = 0;
      while ( theMvaCutVal < theMvaValMax ){
	
	if( theMvaVal >= theMvaCutVal ) 
	{
	  // Case 1: signal
	  if(i < nSigTrees) n_signal[theBin] += weight;
	  
	  // Case 2: background
	  else n_background[theBin] += weight;
	}
	
	theBin++;
	theMvaCutVal += theMvaValStep;
	
      }//end cycle on mva cut values
      
    }//end cycle on events
    
    
  }//end cycle on samples
  
  
  // Now get the significance for each bin and fill the histos
  for ( int ibin = 1; ibin <= theMvaValBins; ibin++ )
  {
    float Significance = n_signal[ibin-1] / sqrt(n_background[ibin-1]) ;
    if (n_background[ibin-1] == 0) continue; 
    h_significance -> SetBinContent(ibin,Significance);
    h_signal -> SetBinContent(ibin,n_signal[ibin-1]);
    h_background -> SetBinContent(ibin,n_background[ibin-1]);
    
    std::cout << "cut val " << (ibin-1)*theMvaValStep + theMvaValMin 
              << " Significance " << Significance 
              << " signal " << n_signal[ibin-1] 
              << " bkg " << n_background[ibin-1] << std::endl;
  }
  
  outFile -> cd();
  h_significance -> Write();
  h_signal -> Write();
  h_background -> Write();
  outFile -> Close();
  
  return 0;
}

