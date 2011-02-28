#include "VBFPreselectionVariables.h"
#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "readJSONFile.h"

#include <iomanip>

#include "TH1F.h"



void SetStepNames(std::map<int, std::string>&, const std::string&, const int&, const int&);
bool AcceptHLTPath(treeReader&, const std::string&);





int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFPreselection::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile(argv[1]);
  
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType       = gConfigParser -> readStringOption("Input::jetType");
  std::string jsonFileName  = gConfigParser -> readStringOption("Input::jsonFileName");
  std::string higgsMass     = gConfigParser -> readStringOption("Input::higgsMass");
    
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + ".root";
  
  int entryFIRST       = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX         = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO      = gConfigParser -> readIntOption("Options::entryMODULO");
  int verbosity        = gConfigParser -> readIntOption("Options::verbosity");
  int dataFlag         = gConfigParser -> readIntOption("Options::dataFlag");
  float crossSection   = gConfigParser -> readFloatOption("Options::crossSection");
  int TMVA4JetTraining = gConfigParser -> readIntOption("Options::TMVA4JetTraining");

  
  int nJetCntMIN = gConfigParser -> readIntOption("Cuts::nJetCntMIN");
  int nJetMAX = gConfigParser -> readIntOption("Cuts::nJetMAX");
  float jetEtMIN  = gConfigParser -> readFloatOption("Cuts::jetEtMIN");
  float jetEtaCNT = gConfigParser -> readFloatOption("Cuts::jetEtaCNT");
  float jetEtaFWD = gConfigParser -> readFloatOption("Cuts::jetEtaFWD");
    
  int nLepMIN = gConfigParser -> readIntOption("Cuts::nLepMIN");
  int nLepMAX = gConfigParser -> readIntOption("Cuts::nLepMAX");
  
  std::string leptonFLAVOUR = gConfigParser -> readStringOption("Cuts::leptonFLAVOUR");  
  
  
  
  
  
  
  // Get number of events
  std::cout << ">>> VBFPreselection::Get number of events" << std::endl;
  std::string stepName;
  
  stepName = "AllEvents/totalEvents";
  std::map<int,int> totalEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  stepName = "NonScrapedEvents/passedEvents";
  std::map<int,int> nonScrapedEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  stepName = "GoodVtxEvents/passedEvents";
  std::map<int,int> goodVtxEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  stepName = "LeptonsFilterEvents/passedEvents";
  std::map<int,int> leptonFilterEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  stepName = "JetFilterAK5PFEvents/passedEvents";
  std::map<int,int> jetFilterEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  
  
  
  
  
  // Get run/LS map from JSON file
  std::cout << ">>> VBFPreselection::Get run/LS map from JSON file" << std::endl;
  std::map<int, std::vector<std::pair<int, int> > > jsonMap;
  jsonMap = readJSONFile(jsonFileName);
  
  
  
  
  
  
  // Open old tree
  std::cout << ">>> VBFPreselection::Open old tree" << std::endl;
  std::string treeName = "MiBiCommonNTOneLeptonTwoJets"+jetAlgorithm+"/SimpleNtuple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain), false);
  
  
  
  
  
  
  // define histograms
  std::cout << ">>> VBFPreselection::Define histograms" << std::endl;
  int nStep = 9;
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepNames;
  
  
  
  // define variable container
  VBFPreselectionVariables vars;
  InitializeVBFPreselectionTree(vars, outputRootFullFileName);
  vars.mH = atof(higgsMass.c_str());
  vars.dataFlag = dataFlag;
  vars.crossSection = crossSection;
  vars.TMVA4Jet = -1;
  
  
  
  // define HLT paths
  std::vector<std::string> HLTPathNames;
  
  if( (leptonFLAVOUR == "e") || (leptonFLAVOUR == "emu") )
  {
    HLTPathNames.push_back("HLT_Photon15_L1R");
    HLTPathNames.push_back("HLT_Ele15_LW_L1R");
    HLTPathNames.push_back("HLT_Ele15_SW_L1R");
    HLTPathNames.push_back("HLT_Ele15_SW_CaloEleId_L1R");
    HLTPathNames.push_back("HLT_Ele17_SW_CaloEleId_L1R");
    HLTPathNames.push_back("HLT_Ele17_SW_TightEleIdIsol_L1R_v1");
    HLTPathNames.push_back("HLT_Ele17_SW_TighterEleIdIsol_L1R_v1");
    HLTPathNames.push_back("HLT_Ele17_SW_TighterEleIdIsol_L1R_v2");
    HLTPathNames.push_back("HLT_Ele17_SW_TighterEleIdIsol_L1R_v3");
  }
  if( (leptonFLAVOUR == "mu") || (leptonFLAVOUR == "emu") )
  {
    HLTPathNames.push_back("HLT_Mu9");
    HLTPathNames.push_back("HLT_Mu11");
    HLTPathNames.push_back("HLT_Mu15_v1");
    HLTPathNames.push_back("HLT_IsoMu13_v3");
    HLTPathNames.push_back("HLT_IsoMu13_v4");
  }
    
    
  
  
  
  
  //********************
  // STEP 1 - all events
  int step = 1;
  SetStepNames(stepNames, "All events", step, verbosity);
  stepEvents[step] = totalEvents[1];
  
                                                                                                                                                           
  
  //*********************
  // STEP 2 - no scraping
  step = 2;
  SetStepNames(stepNames, "Skim: no scraping", step, verbosity);
  stepEvents[step] = nonScrapedEvents[1];
  
  
  //*********************
  // STEP 3 - good vertex
  step = 3;
  SetStepNames(stepNames, "Skim: good vertex", step, verbosity);
  stepEvents[step] = goodVtxEvents[1];
  
  
  //*********************
  // STEP 4 - >= 1 lepton
  step = 4;
  SetStepNames(stepNames, "Skim: >= 1 lepton", step, verbosity);
  stepEvents[step] = leptonFilterEvents[1];
  
  
  //*******************
  // STEP 5 - >= 2 jets
  step = 5;
  SetStepNames(stepNames, "Skim: >= 2 jets", step, verbosity);
  stepEvents[step] = jetFilterEvents[1];
  
  
  
  
  
  
  //*********************
  // LOOP OVER THE EVENTS
  
  std::cout << ">>>>> VBFPreselection::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = entryFIRST ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    ClearVBFPreselectionVariables(vars);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> VBFPreselection::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    
    

    //**************
    // DUMP MC TRUTH
    
    ROOT::Math::XYZTVector mcH;
    ROOT::Math::XYZTVector mcW1;
    ROOT::Math::XYZTVector mcW2;
    ROOT::Math::XYZTVector mcQ1_tag;
    ROOT::Math::XYZTVector mcQ2_tag;
    ROOT::Math::XYZTVector mcQ1_W;
    ROOT::Math::XYZTVector mcQ2_W;
    ROOT::Math::XYZTVector mcLepton;
    std::vector<ROOT::Math::XYZTVector> mcQuarks;
    std::vector<ROOT::Math::XYZTVector> mcQuarks_tag;
    std::vector<ROOT::Math::XYZTVector> mcQuarks_W;
    
    //if( vars.mH > 0 )
    //{
    //  mcH = reader.Get4V("mc_H")->at(0);
    //  mcW1 = reader.Get4V("mcV1")->at(0);
    //  mcW2 = reader.Get4V("mcV2")->at(0);
    //  mcQ1_tag = (reader.Get4V("mcQ1_tag"))->at(0);
    //  mcQ2_tag = (reader.Get4V("mcQ2_tag"))->at(0);
    //  mcQ1_W   = (reader.Get4V("mcF1_fromV2"))->at(0);
    //  mcQ2_W   = (reader.Get4V("mcF2_fromV2"))->at(0);
    //  mcLepton = (reader.Get4V("mcF1_fromV1"))->at(0);
    //  if( (fabs(reader.GetFloat("mcF1_fromV1_pdgId")->at(0)) == 12) ||
    //      (fabs(reader.GetFloat("mcF1_fromV1_pdgId")->at(0)) == 14) ||
    //      (fabs(reader.GetFloat("mcF1_fromV1_pdgId")->at(0)) == 16) )
    //  mcLepton = (reader.Get4V("mcF2_fromV1"))->at(0);
    //  
    //  mcQuarks.push_back( mcQ1_tag );
    //  mcQuarks.push_back( mcQ2_tag );
    //  mcQuarks.push_back( mcQ1_W );
    //  mcQuarks.push_back( mcQ2_W );
    //
    //  mcQuarks_tag.push_back( mcQ1_tag );
    //  mcQuarks_tag.push_back( mcQ2_tag );
    //  mcQuarks_W.push_back( mcQ1_W );
    //  mcQuarks_W.push_back( mcQ2_W );
    //}
    
    
    
    
    
    
    //*******************
    // STEP 6 - 1! lepton
    step = 6;
    SetStepNames(stepNames, "1! lepton", step, verbosity);
    
    SetPVVariables(vars, reader);
    
    int nLep = 0;
    int nEle = 0;
    int nMu = 0;
    
    int nLep_loose = 0;
    int nEle_loose = 0;
    int nMu_loose = 0;
    
    // loop on electrons
    if( verbosity == 1)
      std::cout << "ele begin" << std::endl;
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      float pt = reader.Get4V("electrons")->at(eleIt).pt();
      
      float tkIso  = reader.GetFloat("electrons_tkIsoR03")->at(eleIt); 
      float emIso  = reader.GetFloat("electrons_emIsoR03")->at(eleIt);
      float hadIso = reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt) + 
                     reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt);
      
      float sigmaIetaIeta = reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt);
      float DphiIn = fabs(reader.GetFloat("electrons_deltaPhiIn")->at(eleIt));
      float DetaIn = fabs(reader.GetFloat("electrons_deltaEtaIn")->at(eleIt));
      float HOverE = reader.GetFloat("electrons_hOverE")->at(eleIt);
      
      
      if( pt < 10. ) continue;
      
      
      
      //-------------------
      // standard selection
      
      if( (reader.GetInt("electrons_isEB")->at(eleIt)) == 1 )
      {
        // isol loose
        if(  (tkIso+emIso+hadIso) / pt > 0.5 ) continue;
        
        // eleId VBTF 95% 
        if( sigmaIetaIeta < 0.004 ) continue;
        if( sigmaIetaIeta > 0.010 ) continue;
        if(        DphiIn > 0.800 ) continue;
        if(        DetaIn > 0.007 ) continue;
        if(        HOverE > 0.150 ) continue;
      }
      else
      {
        // isol loose
        if(  (tkIso+emIso+hadIso) / pt > 0.5 ) continue;
        
        // eleId VBTF 95%
        if( sigmaIetaIeta > 0.030 ) continue;
        if(        DphiIn > 0.700 ) continue;
        if(        DetaIn > 0.010 ) continue;
        if(        HOverE > 0.070 ) continue;
      }
      
      SetElectronVariables(vars, reader, eleIt);
            
      
      
      //----------------
      // loose selection
      
      if( (reader.GetInt("electrons_isEB")->at(eleIt)) == 1 )
      {
        // isol VBTF 95% 
        if(  tkIso / pt > 0.15 ) continue;
        if(  emIso / pt > 2.00 ) continue;
        if( hadIso / pt > 0.12 ) continue;
      }
      else
      {
        // eleId VBTF 95%
        if(  tkIso / pt > 0.08 ) continue;
        if(  emIso / pt > 0.06 ) continue;
        if( hadIso / pt > 0.05 ) continue;
      }

      vars.electrons_loose.push_back( reader.Get4V("electrons")->at(eleIt) );
      
    } // loop on electrons

    nLep += vars.electrons.size();
    nEle  = vars.electrons.size();
    
    nLep_loose += vars.electrons_loose.size();
    nEle_loose  = vars.electrons_loose.size();
    
    if( verbosity == 1)    
      std::cout << "ele end" << std::endl;
    
    
    
    
    // loop on muons

    if( verbosity == 1)
      std::cout << "mu begin" << std::endl;
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      
      float pt = reader.Get4V("muons")->at(muIt).pt();
      
      float tkIso  = reader.GetFloat("muons_tkIsoR03")->at(muIt); 
      float emIso  = reader.GetFloat("muons_emIsoR03")->at(muIt);
      float hadIso = reader.GetFloat("muons_hadIsoR03")->at(muIt);
      
      int tracker    = reader.GetInt("muons_tracker")->at(muIt);
      int standalone = reader.GetInt("muons_standalone")->at(muIt);
      int global     = reader.GetInt("muons_global")->at(muIt);
      
      float dxy                    = reader.GetFloat("muons_dxy_PV")->at(muIt);
      float normalizedChi2         = reader.GetFloat("muons_normalizedChi2")->at(muIt);
      int numberOfValidTrackerHits = reader.GetInt("muons_numberOfValidTrackerHits")->at(muIt);
      int numberOfValidMuonHits    = reader.GetInt("muons_numberOfValidMuonHits")->at(muIt);
      
      if( pt < 10. ) continue;
      
      if( tracker    != 1 ) continue;
      if( standalone != 1 ) continue;
      if( global     != 1 ) continue;
      if( fabs(dxy)                > 0.2 ) continue;
      if( normalizedChi2           > 10. ) continue;
      if( numberOfValidTrackerHits < 11)   continue;
      if( numberOfValidMuonHits    < 1 )   continue;
      
      
      
      //-------------------
      // standard selection
      
      if(  (tkIso+emIso+hadIso) / pt > 0.500 ) continue;
      
      SetMuonVariables(vars, reader, muIt);
      
      
      
      //----------------
      // loose selection
      
      if(  (tkIso+emIso+hadIso) / pt > 0.150 ) continue;
      
      vars.muons_loose.push_back( reader.Get4V("muons")->at(muIt) );
      
    } // loop on muons
    
    nLep += vars.muons.size();
    nMu   = vars.muons.size();
    
    nLep_loose += vars.muons_loose.size();
    nMu_loose   = vars.muons_loose.size();

    if( verbosity == 1)
      std::cout << "mu end" << std::endl;
    
    
    
    
    
    
    //**********
    // 1! lepton
    if( nLep < nLepMIN ) continue;
    if( nLep_loose > nLepMAX ) continue;
    if( (leptonFLAVOUR == "e")   && ( nEle      < nLepMIN) ) continue;
    if( (leptonFLAVOUR == "mu")  && (      nMu  < nLepMIN) ) continue;
    if( (leptonFLAVOUR == "emu") && ((nEle+nMu) < nLepMIN) ) continue;
    
    
    
    //**************
    // select lepton
    vars.selectIt_lep = SelectLepton(vars.leptons, "maxPt", 15.);
    if(vars.selectIt_lep == -1) continue;
    SetLeptonVariables(vars, reader);
    
    
    // fIll event counters
    stepEvents[step] += 1;
    
    
    
    
    
    
    //*************************
    // STEP 7 -  >= 2 cnt jets
    step += 1;
    char stepName[50]; sprintf(stepName, ">= %d cnt jet(s)", nJetCntMIN);
    SetStepNames(stepNames, std::string(stepName), step, verbosity);
    
    //*****************
    // met and neutrino
    SetMetVariables(vars, reader, jetType);
    
    
    
    //************
    // select jets

    if( verbosity == 1)
      std::cout << "jet begin" << std::endl;
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      
      // jet et min
      if( jet.Et() < jetEtMIN ) continue;
      
      // clean jets from selected lepton
      if( deltaR(jet.eta(), jet.phi(), vars.lep.eta(), vars.lep.phi()) < 0.5 ) continue;
      
      // jetID
      if(jetType == "Calo")
      {
        if( (fabs(jet.eta()) < 2.6) && (reader.GetFloat("jets_emEnergyFraction")->at(jetIt) < 0.01) ) continue;
        if( reader.GetFloat("jets_emEnergyFraction")->at(jetIt) >= 0.98 ) continue;
        if( reader.GetFloat("jets_n90Hits")->at(jetIt) < 2 ) continue;
        if( reader.GetFloat("jets_fHPD")->at(jetIt) >= 0.98 ) continue;
      }
      
      if(jetType == "PF")
      {
        
        if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_neutralHadronEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_chargedHadronEnergyFraction")->at(jetIt) <= 0.) ) continue;
        if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_chargedEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) < 2.4) && ( ( reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) +
                                           reader.GetFloat("jets_chargedEmEnergyFraction")->at(jetIt) ) >= 0.99) ) continue;
        if( (fabs(jet.eta()) < 2.4) && (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) <= 0) ) continue;
        if( (fabs(jet.eta()) < 2.4) && (reader.GetInt("jets_neutralMultiplicity")->at(jetIt) <= 0) ) continue;
        if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_muonEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        
        if( (fabs(jet.eta()) >= 2.4) && (reader.GetFloat("jets_neutralHadronEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) >= 2.4) && (reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) >= 2.4) && (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) + reader.GetInt("jets_neutralMultiplicity")->at(jetIt) <= 1) ) continue;
      }
      
      SetJetVariables(vars, reader, jetIt, jetType, jetEtaCNT, jetEtaFWD);
      
    } // loop on jets
    if( verbosity == 1)
      std::cout << "jet end" << std::endl;
    
    std::sort(vars.jets_etOrdered.begin(), vars.jets_etOrdered.end(), maggiore);
    std::sort(vars.jets_bTagOrdered.begin(), vars.jets_bTagOrdered.end(), maggiore);
      if( vars.jets_bTagOrdered.size() > 0 )
    vars.jets_bTag1 = vars.jets_bTagOrdered.at(0);
      if( vars.jets_bTagOrdered.size() > 1 )
    vars.jets_bTag2 = vars.jets_bTagOrdered.at(1);
    
    
    
    //**************
    // >= n cnt jets 
    if( vars.nJets_cnt < nJetCntMIN ) continue;
    if( vars.nJets >  nJetMAX ) continue;
    
    SetLeadingJetVariables(vars, reader, jetEtaCNT);
    
    
    
    
    
    
    //**************
    // TMVA training 
    /*
    if( TMVA4JetTraining == 1 )
    {
      // match with MC
      std::vector<int> matchIt;
      std::vector<int> matchIt_tag;
      std::vector<int> matchIt_W;
      int nMatching = -1;
      int nMatching_tag = -1;
      int nMatching_W = -1;
      
      if( vars.mH > 0 )
      {
        nMatching     = GetMatching(vars.jets, mcQuarks,     0.3, 0.5, 1.5, &matchIt);
        nMatching_tag = GetMatching(vars.jets, mcQuarks_tag, 0.3, 0.5, 1.5, &matchIt_tag);
        nMatching_W   = GetMatching(vars.jets, mcQuarks_W,   0.3, 0.5, 1.5, &matchIt_W);
        
	std::cout << "nMatching = " << nMatching << std::endl;
        if( (nMatching == 4) && (matchIt.at(0) > matchIt.at(1)) )
        {
          int dummy = matchIt.at(0);
          matchIt.at(0) = matchIt.at(1);
          matchIt.at(1) = dummy;
        }
  
        if( (nMatching == 4) && (matchIt.at(2) > matchIt.at(3)) )
        {
          int dummy = matchIt.at(2);
          matchIt.at(2) = matchIt.at(3);
          matchIt.at(3) = dummy;
        }
  
        std::sort(matchIt_tag.begin(), matchIt_tag.end());
        std::sort(matchIt_W.begin(), matchIt_W.end());
      }
      
      // fill the ntuple with combinations
      if(nMatching == 4)
      {
        std::vector<std::vector<int> > combinations;
        int nCombinations = Build4JetCombinations(combinations,vars.nJets);
        
        for(int combIt = 0; combIt < nCombinations; ++combIt)
        {
          std::vector<int> tempCombination = combinations.at(combIt);
          
          vars.selectIt_tag.at(0) = tempCombination.at(0);
          vars.selectIt_tag.at(1) = tempCombination.at(1);
          vars.selectIt_W.at(0) = tempCombination.at(2);
          vars.selectIt_W.at(1) = tempCombination.at(3);
	        
          SetWJJVariables(vars, reader);
          SetTagJJVariables(vars, reader);
          SetHVariables(vars, reader);
          
          if(tempCombination == matchIt) vars.TMVA4Jet = 1;
          else                           vars.TMVA4Jet = 0;          
	        
	        
	        // Fill reduced tree
	        FillVBFPreselectionTree(vars);
        }
        
        // fIll event counters
        stepEvents[step] += 1;
      }
    }
    */
    
    
    
    
    
    
    //******************
    // select W/tag jets
    if( TMVA4JetTraining == 0 )
    {
      if( vars.nJets < 3 )
      {
        SelectWJets(vars.selectIt_W, vars.jets, "minDeta", jetEtMIN, jetEtaCNT, 3., 300.);
        
        std::vector<int> blacklistIt_W;
        blacklistIt_W.push_back(vars.selectIt_W.at(0));
        blacklistIt_W.push_back(vars.selectIt_W.at(1));
        SelectTagJets(vars.selectIt_tag, vars.jets, jetEtMIN, 1., 200., &blacklistIt_W);
      }
      if( vars.nJets == 3 )
      {
        SelectTagJet(vars.selectIt_tag, vars.jets, jetEtMIN, jetEtaCNT);
        
        std::vector<int> blacklistIt_tag;
        blacklistIt_tag.push_back(vars.selectIt_tag.at(0));
        blacklistIt_tag.push_back(vars.selectIt_tag.at(1));
        SelectWJets(vars.selectIt_W, vars.jets, "minDeta", jetEtMIN, jetEtaCNT, 3., 300., &blacklistIt_tag);
      }
      if( vars.nJets > 3 )
      {
        SelectTagJets(vars.selectIt_tag, vars.jets, jetEtMIN, 1., 200.);
        
        std::vector<int> blacklistIt_tag;
        blacklistIt_tag.push_back(vars.selectIt_tag.at(0));
        blacklistIt_tag.push_back(vars.selectIt_tag.at(1));
        SelectWJets(vars.selectIt_W, vars.jets, "minDeta", jetEtMIN, jetEtaCNT, 3., 300., &blacklistIt_tag);
      }
      
      
      std::sort(vars.selectIt_W.begin(), vars.selectIt_W.end());
      std::sort(vars.selectIt_tag.begin(), vars.selectIt_tag.end());
      
      SetWJJVariables(vars, reader);
      SetTagJJVariables(vars, reader);
      SetThirdJetVariables(vars, reader);
      SetHVariables(vars, reader);
      
      
      // fIll event counters
      stepEvents[step] += 1;
      
      
      // Fill reduced tree
      FillVBFPreselectionTree(vars);
    }
  
    
    
    
    
    
    //**************************
    // STEP 8 - run/LS selection
    step = step + 1;
    SetStepNames(stepNames, "Run/LS selection", step, verbosity);
    
    
    vars.runId   = reader.GetInt("runId")->at(0);
    vars.lumiId  = reader.GetInt("lumiId")->at(0);
    vars.eventId = reader.GetInt("eventId")->at(0);
    
    bool skipEvent = false;
    if( vars.dataFlag == 1 )
    {
      if(AcceptEventByRunAndLumiSection(vars.runId, vars.lumiId, jsonMap) == false) skipEvent = true;      
      
      // HCAL noise
      //if( reader.GetInt("HCAL_noise")->at(0) == 0 ) skipEvent = true;
    }
    
    if( skipEvent == true ) continue;
    
    
    // fill event counters
    stepEvents[step] += 1;
    
    
    
    
    
        
    //***********************
    // STEP 9 - HLT selection
    step = step+1;
    SetStepNames(stepNames, "HLT", step, verbosity);
    
    
    skipEvent = true;
    
    for(unsigned int HLTIt = 0; HLTIt < HLTPathNames.size(); ++HLTIt)
      if( AcceptHLTPath(reader, HLTPathNames.at(HLTIt)) == true )
        skipEvent = false;
    
    //if( skipEvent == true ) continue;
    
    
    // fill event counters
    stepEvents[step] += 1;
    
    
    
    
    
    
    
  } // loop over the events
  
  
  
  // Save tree
  DeleteVBFPreselectionVariables(vars);
  
  
  
  // save histograms
  std::cout << ">>> VBFPreselection::Open output root file " << outputRootFullFileName << std::endl;
  TFile outputRootFile((outputRootFullFileName).c_str(), "UPDATE");
  outputRootFile.cd();
  
  for(step = 1; step <= nStep; ++step)
  {
    events -> SetBinContent(step, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step, stepNames[step].c_str());
  }
  
  events -> Write();
  outputRootFile.Close();
  
  
  
  return 0;
}






void SetStepNames(std::map<int, std::string>& stepNames, const std::string& stepName, const int& step, const int& verbosity)
{
  char dummy[5];
  sprintf(dummy, "%d)", step);
  stepNames[step] = std::string(dummy)+" "+stepName;
  
  if(verbosity)
    std::cout << ">>>>>>>>> " << stepNames[step] << std::endl;
}






bool AcceptHLTPath(treeReader& reader, const std::string& HLTPathName)
{
  bool acceptEvent = false;
  
  std::vector<std::string> HLT_names = *(reader.GetString("HLT_Names"));
  for(unsigned int HLTIt = 0; HLTIt < HLT_names.size(); ++HLTIt)
    if( (reader.GetString("HLT_Names")->at(HLTIt) == HLTPathName) &&
        (reader.GetFloat("HLT_Accept")->at(HLTIt) == 1) )
      acceptEvent = true;
  
  return acceptEvent;
}
