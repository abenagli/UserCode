#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "readJSONFile.h"

#include <iomanip>

#include "TH1F.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there                                                                                                                               
  if(argc != 2)
  {
    std::cerr << ">>>>> Collisions7TeVPreselection::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file                                                                                                                                                          
  parseConfigFile (argv[1]) ;
  
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryMAX    = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int dataFlag    = gConfigParser -> readIntOption("Options::dataFlag");
  
  int nJetMIN = gConfigParser -> readIntOption("Cuts::nJetMIN");
  float jetEtMIN = gConfigParser -> readFloatOption("Cuts::jetEtMIN");
  
  int nLepMIN = gConfigParser -> readIntOption("Cuts::nLepMIN");
  float lepPtMIN = gConfigParser -> readFloatOption("Cuts::lepPtMIN");
  
  
  
  
  
  
  // Get number of events
  std::cout << ">>> Collisions7TeVPreselection::Get number of events" << std::endl;
  std::map<int,int> totalEvents = GetTotalEvents("VBFAllPassFilterBegin/totalEvents", inputFileList.c_str());  

  std::string GoodVertexPreselectionName = "VBFAllPassFilterGoodVertex/totalEvents"; 
  std::map<int,int> GoodVertexPreselectionEvents = GetTotalEvents(GoodVertexPreselectionName.c_str(), inputFileList.c_str());  

  std::string VBFPreselectionName = "VBFEtMinCaloJetCountFilter" + jetAlgorithm+"/passedEvents"; 
  std::map<int,int> VBFPreselectionEvents = GetTotalEvents(VBFPreselectionName.c_str(), inputFileList.c_str());  
  
  
  
  
  
  
  // Get run/LS map from JSON file
  //std::cout << ">>> Collisions7TeVPreselection::Get run/LS map from JSON file" << std::endl;
  //std::map<int, std::vector<std::pair<int, int> > > jsonMap;
  //if(dataFlag == 1) jsonMap = readJSONFile(jsonFile);
  
  
  
  
  
  
  // Open old tree
  std::cout << ">>> Collisions7TeVPreselection::Open old tree" << std::endl;
  std::string treeName = "VBFNtuple" + jetAlgorithm + "/SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain), false);
  
  
  
  // Open output root file for clone tree
  outputRootFileName += "_" + jetAlgorithm + ".root";
  TFile outputRootFile((outputRootFilePath+outputRootFileName).c_str(), "RECREATE");
  outputRootFile.cd();
  
  TTree* cloneTree = chain -> CloneTree(0);
  
  
  
  // define histograms
  std::cout << ">>> Collisions7TeVPreselection::Define histograms" << std::endl;
  int nStep = 7;
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepName;

  int step = 0;
  stepEvents[step] = totalEvents[1];
  stepName[step] = "total events";

  step = 1;
  stepEvents[step] = GoodVertexPreselectionEvents[1];
  stepName[step] = "GoodVertexPreselection";
  
  step = 2;
  stepEvents[step] = VBFPreselectionEvents[1];
  stepName[step] = "VBFPreselection";
  
  
  
  
  
  
  // Loop over events
  std::cout << ">>>>> Collisions7TeVPreselection::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> Collisions7TeVPreselection::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    //************************
    // STEP 3 - cut on leptons
    step = 3;
    stepName[step] = ">= 1 lepton";
    
    int nLep = 0;
    std::vector<ROOT::Math::XYZTVector> electrons;
    std::vector<ROOT::Math::XYZTVector> muons;
    std::vector<ROOT::Math::XYZTVector> leptons;
    std::vector<std::string> leptonFlavours;    
        
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < lepPtMIN ) continue;
      ++nLep;
      
      electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      leptons.push_back( reader.Get4V("electrons")->at(eleIt) );      
      leptonFlavours.push_back("electron");
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < lepPtMIN ) continue;
      ++nLep;
      
      muons.push_back( reader.Get4V("muons")->at(muIt) );
      leptons.push_back( reader.Get4V("muons")->at(muIt) );      
      leptonFlavours.push_back("muon");
    }
    
    if( nLep < nLepMIN ) continue;
    stepEvents[step] += 1;
    
    
    
    //*********************
    // STEP 4 - cut on jets
    step = step+1;
    stepName[step] = ">= 2 jets";
    
    int nJet = 0;
    std::vector<ROOT::Math::XYZTVector> jets;
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      if( jet.Et() < jetEtMIN ) continue;
      
      ++nJet;
      jets.push_back( reader.Get4V("jets")->at(jetIt) );
    }
    
    if( nJet < nJetMIN ) continue;
    stepEvents[step] += 1;
    
    
    
    
    
    
    //**************************
    // STEP 5 - run/LS selection
    //step = step+1;
    //stepName[step] = "run/LS";    
    //
    //bool skipEvent = false;
    //if(dataFlag == 1)
    //{
    //  int runId  = reader.GetInt("runId")->at(0);
    //  int lumiId = reader.GetInt("lumiId")->at(0);
    //  if(AcceptEventByRunAndLumiSection(runId, lumiId, jsonMap) == false) skipEvent = true;      
    //}
    //
    //if( skipEvent == true ) continue;
    //stepEvents[step] += 1;
    
    
    
    
    
    
    //***********************
    // STEP 5 - HLT selection
    step = step+1;
    stepName[step] = "HLT";
    
    bool skipEvent = true;
    
    std::vector<std::string> HLT_names = *(reader.GetString("HLT_Names"));
    int HLT_Photon10_L1R_bit = -1;
    int HLT_Mu9_bit = -1;
    for(unsigned int HLTIt = 0; HLTIt < HLT_names.size(); ++HLTIt)
    {
      if( reader.GetString("HLT_Names")->at(HLTIt) == "HLT_Photon10_L1R" )
        HLT_Photon10_L1R_bit = HLTIt;
      if( reader.GetString("HLT_Names")->at(HLTIt) == "HLT_Mu9" )
        HLT_Mu9_bit = HLTIt;
    }
    
    //HLT_Photon10_L1R
    if( HLT_Photon10_L1R_bit == -1 ) continue; 
    if( reader.GetFloat("HLT_Accept")->at(HLT_Photon10_L1R_bit) == 1 ) skipEvent = false;
    
    //HLT_Mu9
    if( HLT_Mu9_bit == -1 ) continue; 
    if( reader.GetFloat("HLT_Accept")->at(HLT_Mu9_bit) == 1 ) skipEvent = false;
    
    if( skipEvent == true ) continue;
    stepEvents[step] += 1;    
    
    
    
    
    
    
    cloneTree -> Fill();
    
    
  } // loop over the events
  
  
  
  
  
  
  // save histograms
  for(step = 0; step < nStep; ++step)
  {
    events -> SetBinContent(step+1, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step+1, stepName[step].c_str());
  }
  
  events -> Write();
  
  cloneTree -> AutoSave();
  outputRootFile.Close();
  
  
  
  return 0;
}
