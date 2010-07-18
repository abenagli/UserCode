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
  std::string jetType       = gConfigParser -> readStringOption("Input::jetType");
    
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryMAX    = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int dataFlag    = gConfigParser -> readIntOption("Options::dataFlag");
  
  int nJetMIN = gConfigParser -> readIntOption("Cuts::nJetMIN");
  float jetEtMIN = gConfigParser -> readFloatOption("Cuts::jetEtMIN");
  
  int nLepMIN = gConfigParser -> readIntOption("Cuts::nLepMIN");
  int nEleMIN = gConfigParser -> readIntOption("Cuts::nEleMIN");
  int nMuMIN = gConfigParser -> readIntOption("Cuts::nMuMIN");
  float lepPtMIN = gConfigParser -> readFloatOption("Cuts::lepPtMIN");
  
  std::string HLTBitNAME = gConfigParser -> readStringOption("Cuts::HLTBitNAME");  
  
  
  
  
  
  
  // Get number of events
  std::cout << ">>> Collisions7TeVPreselection::Get number of events" << std::endl;
  std::map<int,int> totalEvents = GetTotalEvents("VBFAllPassFilterBegin/totalEvents", inputFileList.c_str());  

  std::string GoodVertexPreselectionName = "VBFAllPassFilterGoodVertex/totalEvents"; 
  std::map<int,int> GoodVertexPreselectionEvents = GetTotalEvents(GoodVertexPreselectionName.c_str(), inputFileList.c_str());  

  std::string VBFPreselectionName;
  if(jetType == "Calo")
    VBFPreselectionName = "VBFEtMinCaloJetCountFilter" + jetAlgorithm+"/passedEvents"; 
  if(jetType == "PF")
    VBFPreselectionName = "VBFEtMinPFJetCountFilter" + jetAlgorithm+"/passedEvents"; 
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
  int nStep = 6;
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepName;

  int step = 1;
  stepEvents[step] = totalEvents[1];
  stepName[step] = "total events";

  step = 2;
  stepEvents[step] = GoodVertexPreselectionEvents[1];
  stepName[step] = "GoodVertexPreselection";
  
  step = 3;
  stepEvents[step] = VBFPreselectionEvents[1];
  stepName[step] = "Preselection";
  
  
  
  
  
  
  // Loop over events
  std::cout << ">>>>> Collisions7TeVPreselection::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> Collisions7TeVPreselection::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    //************************
    // STEP 4 - cut on leptons
    step = 4;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = ">= 1 lepton";
    
    int nLep = 0;
    int nEle = 0;
    int nMu = 0;
    std::vector<ROOT::Math::XYZTVector> electrons;
    std::vector<ROOT::Math::XYZTVector> muons;
    std::vector<ROOT::Math::XYZTVector> leptons;
    std::vector<std::string> leptonFlavours;    
        
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
      
      //if( (reader.GetInt("electrons_isEB")->at(eleIt)) == 1 )
      //{      
      //  if( (reader.GetFloat("electrons_tkIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.15 ) continue;
      //  if( (reader.GetFloat("electrons_emIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 2.00 ) continue;
      //  if( ( (reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt)) +
      //        (reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt)) ) / reader.Get4V("electrons")->at(eleIt).pt() > 0.12 ) continue;
      //  if( (reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt)) > 0.01 ) continue;
      //  if( fabs((reader.GetFloat("electrons_deltaPhiIn")->at(eleIt))) > 0.08 ) continue;
      //  if( fabs((reader.GetFloat("electrons_deltaEtaIn")->at(eleIt))) > 0.007 ) continue;
      //  if( (reader.GetFloat("electrons_hOverE")->at(eleIt)) > 0.1 ) continue;
      //}
      
      //else
      //{      
      //  if( (reader.GetFloat("electrons_tkIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.08 ) continue;
      //  if( (reader.GetFloat("electrons_emIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.06 ) continue;
      //  if( ( (reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt)) +
      //        (reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt)) ) / reader.Get4V("electrons")->at(eleIt).pt() > 0.05 ) continue;
      //  if( (reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt)) > 0.03 ) continue;
      //  if( fabs((reader.GetFloat("electrons_deltaPhiIn")->at(eleIt))) > 0.07 ) continue;
      //  if( fabs((reader.GetFloat("electrons_deltaEtaIn")->at(eleIt))) > 0.01 ) continue;
      //  if( (reader.GetFloat("electrons_hOverE")->at(eleIt)) > 0.07 ) continue;
      //}
      
      electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      leptons.push_back( reader.Get4V("electrons")->at(eleIt) );
      
      if( reader.Get4V("electrons")->at(eleIt).pt() >= lepPtMIN )
      {
        ++nLep;
        ++nEle;
      }
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < 10. ) continue;
      
      muons.push_back( reader.Get4V("muons")->at(muIt) );
      leptons.push_back( reader.Get4V("muons")->at(muIt) ); 
      
      if( reader.Get4V("muons")->at(muIt).pt() >= lepPtMIN )
      {
        ++nLep;
        ++nMu;
      }
    }
    
    if( nLep < nLepMIN ) continue;
    if( nEle < nEleMIN ) continue;
    if( nMu < nMuMIN ) continue;
    stepEvents[step] += 1;
    
    
    
    
    
    
    //*********************
    // STEP 5 - cut on jets
    step = step+1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = ">= 2 jets";
    
    int nJet = 0;
    std::vector<ROOT::Math::XYZTVector> jets;
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      
      if( jet.Et() < jetEtMIN ) continue;
      if( reader.GetFloat("jets_emEnergyFraction")->at(jetIt) > 0.98 ) continue;
      
      
      // clean jets from electrons
      bool skipJet = false;
      for(unsigned int eleIt = 0; eleIt < electrons.size(); ++eleIt)
      {
        ROOT::Math::XYZTVector ele = electrons.at(eleIt);
        if( deltaR(jet.eta(), jet.phi(), ele.eta(), ele.phi()) < 0.5 )
          skipJet = true;
      }
      
      // clean jets from muons
      for(unsigned int muIt = 0; muIt < muons.size(); ++muIt)
      {
        ROOT::Math::XYZTVector mu = muons.at(muIt);
        if( deltaR(jet.eta(), jet.phi(), mu.eta(), mu.phi()) < 0.5 )
          skipJet = true;
      }
      
      if(skipJet == true) continue;  
      
      
      ++nJet;
      jets.push_back( reader.Get4V("jets")->at(jetIt) );
    }
    
    if( nJet < nJetMIN ) continue;
    stepEvents[step] += 1;
    
    
    
    
    
    
    //***********************
    // STEP 6 - HLT selection
    step = step+1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "HLT";
    
    bool skipEvent = true;
    
    std::vector<std::string> HLT_names = *(reader.GetString("HLT_Names"));
    int HLT_bit = -1;
    for(unsigned int HLTIt = 0; HLTIt < HLT_names.size(); ++HLTIt)
    {
      if( reader.GetString("HLT_Names")->at(HLTIt) == HLTBitNAME )
        HLT_bit = HLTIt;
    }
    
    if( HLT_bit == -1 ) continue; 
    if( reader.GetFloat("HLT_Accept")->at(HLT_bit) == 1 ) skipEvent = false;
    
    if( skipEvent == true ) continue;
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
    
    
    
    cloneTree -> Fill();
    
    
  } // loop over the events
  
  
  
  
  
  
  // save histograms
  for(step = 1; step <= nStep; ++step)
  {
    events -> SetBinContent(step, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step, stepName[step].c_str());
  }
  
  events -> Write();
  
  cloneTree -> AutoSave();
  outputRootFile.Close();
  
  
  
  return 0;
}
