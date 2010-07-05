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

#include "TMVA/Reader.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> WplusWminusRatioAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string baseDir = gConfigParser -> readStringOption("Input::baseDir");
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType       = gConfigParser -> readStringOption("Input::jetType");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryFIRST = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int dataFlag = gConfigParser -> readFloatOption("Options::dataFlag");
  float crossSection = gConfigParser -> readFloatOption("Options::crossSection");
  
  int jetNMIN = gConfigParser -> readIntOption("Cuts::jetNMIN");
  int jetNMAX = gConfigParser -> readIntOption("Cuts::jetNMAX");
  float jetEtMIN = gConfigParser -> readFloatOption("Cuts::jetEtMIN");

  
  int lepNMIN = gConfigParser -> readIntOption("Cuts::lepNMIN");
  int lepNMAX = gConfigParser -> readIntOption("Cuts::lepNMAX");
  int eleNMIN = gConfigParser -> readIntOption("Cuts::eleNMIN");
  int muNMIN = gConfigParser -> readIntOption("Cuts::muNMIN");
  float lepPtMIN = gConfigParser -> readFloatOption("Cuts::lepPtMIN");
  float lepPtMAX = gConfigParser -> readFloatOption("Cuts::lepPtMAX");
  float eleTkIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleTkIsoOverPtEBMAX");
  float eleEmIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleEmIsoOverPtEBMAX");
  float eleHadIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleHadIsoOverPtEBMAX");
  float eleTkIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleTkIsoOverPtEEMAX");
  float eleEmIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleEmIsoOverPtEEMAX");
  float eleHadIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleHadIsoOverPtEEMAX");
  float muTkIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muTkIsoOverPtMAX");
  float muEmIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muEmIsoOverPtMAX");
  float muHadIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muHadIsoOverPtMAX");
  float eleHoverEEBMAX = gConfigParser -> readFloatOption("Cuts::eleHoverEEBMAX");
  float eleDetaInEBMAX = gConfigParser -> readFloatOption("Cuts::eleDetaInEBMAX");
  float eleDphiInEBMAX = gConfigParser -> readFloatOption("Cuts::eleDphiInEBMAX");
  float eleSigmaIetaIetaEBMAX = gConfigParser -> readFloatOption("Cuts::eleSigmaIetaIetaEBMAX");
  float eleHoverEEEMAX = gConfigParser -> readFloatOption("Cuts::eleHoverEEEMAX");
  float eleDetaInEEMAX = gConfigParser -> readFloatOption("Cuts::eleDetaInEEMAX");
  float eleDphiInEEMAX = gConfigParser -> readFloatOption("Cuts::eleDphiInEEMAX");
  float eleSigmaIetaIetaEEMAX = gConfigParser -> readFloatOption("Cuts::eleSigmaIetaIetaEEMAX");
  float lepAbsEtaMAX = gConfigParser -> readFloatOption("Cuts::lepAbsEtaMAX");
  float lepMetMtMIN = gConfigParser -> readFloatOption("Cuts::lepMetMtMIN");
  float lepMetMtMAX = gConfigParser -> readFloatOption("Cuts::lepMetMtMAX");
  float lepMetDphiMIN = gConfigParser -> readFloatOption("Cuts::lepMetDphiMIN");  
  
  float metEtMIN = gConfigParser -> readFloatOption("Cuts::metEtMIN");
  float metEtMAX = gConfigParser -> readFloatOption("Cuts::metEtMAX");
  
  std::map<int, int> totalEvents = GetTotalEvents("events", inputFileList.c_str()); 
  
  
  std::cout << ">>>>> WplusWminusRatioAnalysis::inputFileList = " << inputFileList << std::endl;
  
  
  
  
  
  
  // Open tree
  std::string treeName = "SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain));  
  
  
  
  // define out file names
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+"_"+jetAlgorithm+".root";
  std::string outputTreeFileName = outputRootFilePath+"/tree.root";
  
  
  
  // define event histogram
  int nStep = 11;
  
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  
  std::map<int, TH1F*> events_plus;
  std::map<int, TH1F*> events_minus;
  for(int njetBin = jetNMIN; njetBin <= jetNMAX; ++njetBin)
  {
    char eventsPlusName[50];
    sprintf(eventsPlusName, "events_plus_%djets", njetBin);
    events_plus[njetBin] = new TH1F(eventsPlusName, eventsPlusName, nStep, 0., 1.*nStep);
    
    char eventsMinusName[50];
    sprintf(eventsMinusName, "events_minus_%djets", njetBin);
    events_minus[njetBin] = new TH1F(eventsMinusName, eventsMinusName, nStep, 0., 1.*nStep);
  }
  
  std::map<int, int> stepEvents;
  std::map<int, std::map<int, int> > stepEvents_plus;
  std::map<int, std::map<int, int> > stepEvents_minus;
  std::map<int, std::string> stepName;
  
  
  int step = 1;
  stepEvents[step] = totalEvents[1];
  for(int njetBin = jetNMIN; njetBin <= jetNMAX; ++njetBin)
  {
    std::map<int, int> tempMap;
    tempMap[step] = totalEvents[1];
    stepEvents_plus[njetBin] = tempMap;
    stepEvents_minus[njetBin] = tempMap;
  }
  stepName[step] = "1) total events";
  
  step = 2;
  stepEvents[step] = totalEvents[5];
  for(int njetBin = 2; njetBin <= jetNMAX; ++njetBin)
  {
    (stepEvents_plus[njetBin])[step] = totalEvents[5];
    (stepEvents_minus[njetBin])[step] = totalEvents[5];
  }
  stepName[step] = "2) Preselection";
  
  
  
  // define variable/histogram container
  WplusWminusRatioVariables vars;
  InitializeWplusWminusRatioVariables(vars, outputRootFullFileName, nStep);
  
  
  
  
  
  
  
  
  
  
  
  
  //*********************
  // LOOP OVER THE EVENTS
  
  std::cout << ">>>>> WplusWminusRatioAnalysis::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = entryFIRST ; entry < chain -> GetEntries() ; ++entry)
  {
    //***********
    // DUMP EVENT
    reader.GetEntry(entry);
    if(entry == entryMAX) break;
    if((entry%entryMODULO) == 0) std::cout << ">>>>> WplusWminusRatioAnalysis::GetEntry " << entry << std::endl;   
    ClearWplusWminusRatioVariables(vars);
    
    
    
    
    
    
    //********************************
    // STEP 3 - Initial cuts - Trigger
    step = 3;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "3) HLT";
    
    
    bool skipEvent = true;
    
    std::vector<std::string> HLT_names = *(reader.GetString("HLT_Names"));
    int HLT_Photon10_L1R_bit = -1;
    int HLT_Ele10_LW_L1R_bit = -1;
    int HLT_Mu9_bit = -1;
    for(unsigned int HLTIt = 0; HLTIt < HLT_names.size(); ++HLTIt)
    {
      if( reader.GetString("HLT_Names")->at(HLTIt) == "HLT_Photon10_L1R" )
        HLT_Photon10_L1R_bit = HLTIt;
      if( reader.GetString("HLT_Names")->at(HLTIt) == "HLT_Ele10_LW_L1R" )
        HLT_Ele10_LW_L1R_bit = HLTIt;
      if( reader.GetString("HLT_Names")->at(HLTIt) == "HLT_Mu9" )
        HLT_Mu9_bit = HLTIt;
    }
    
    //HLT_Photon10_L1R
    if( HLT_Photon10_L1R_bit == -1 ) continue; 
    if( reader.GetFloat("HLT_Accept")->at(HLT_Photon10_L1R_bit) == 1 ) skipEvent = false;
    
    //HLT_Ele10_LW_L1R
    if( HLT_Ele10_LW_L1R_bit == -1 ) continue; 
    if( reader.GetFloat("HLT_Accept")->at(HLT_Ele10_LW_L1R_bit) == 1 ) skipEvent = false;
        
    //HLT_Mu9
    if( HLT_Mu9_bit == -1 ) continue; 
    if( reader.GetFloat("HLT_Accept")->at(HLT_Mu9_bit) == 1 ) skipEvent = false;
    
    
    if( skipEvent == true ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    for(int njetBin = 2; njetBin <= jetNMAX; ++njetBin)
    {
      (stepEvents_plus[njetBin])[step] += 1;
      (stepEvents_minus[njetBin])[step] += 1;
    }
    
    
    
    
    
    
    //***********************
    // STEP 4 - select lepton
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "4) select lepton";
    
    
    int nEle = 0;
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
      
      if( (reader.GetInt("electrons_isEB")->at(eleIt)) == 1 )
      {      
        if( (reader.GetFloat("electrons_tkIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.15 ) continue;
        if( (reader.GetFloat("electrons_emIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 2.00 ) continue;
        if( ( (reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt)) +
              (reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt)) ) / reader.Get4V("electrons")->at(eleIt).pt() > 0.12 ) continue;
        if( (reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt)) > 0.01 ) continue;
        if( fabs((reader.GetFloat("electrons_deltaPhiIn")->at(eleIt))) > 0.08 ) continue;
        if( fabs((reader.GetFloat("electrons_deltaEtaIn")->at(eleIt))) > 0.007 ) continue;
        if( (reader.GetFloat("electrons_hOverE")->at(eleIt)) > 0.1 ) continue;
      }

      else
      {      
        if( (reader.GetFloat("electrons_tkIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.08 ) continue;
        if( (reader.GetFloat("electrons_emIsoR03")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.06 ) continue;
        if( ( (reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt)) +
              (reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt)) ) / reader.Get4V("electrons")->at(eleIt).pt() > 0.05 ) continue;
        if( (reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt)) > 0.03 ) continue;
        if( fabs((reader.GetFloat("electrons_deltaPhiIn")->at(eleIt))) > 0.07 ) continue;
        if( fabs((reader.GetFloat("electrons_deltaEtaIn")->at(eleIt))) > 0.01 ) continue;
        if( (reader.GetFloat("electrons_hOverE")->at(eleIt)) > 0.07 ) continue;
      }

              
      vars.electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      vars.leptons.push_back( reader.Get4V("electrons")->at(eleIt) );      
      vars.leptonCharges.push_back( reader.GetFloat("electrons_charge")->at(eleIt) );
      vars.leptonFlavours.push_back("electron");
      
      vars.leptons_tkIso.push_back(reader.GetFloat("electrons_tkIsoR03")->at(eleIt));
      vars.leptons_emIso.push_back(reader.GetFloat("electrons_emIsoR03")->at(eleIt));
      vars.leptons_hadIso.push_back(reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt)+
                                    reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt));          
      vars.leptons_dxy.push_back(reader.GetFloat("electrons_dxy")->at(eleIt));
      vars.leptons_dz.push_back(reader.GetFloat("electrons_dz")->at(eleIt));
      vars.leptons_lipSig.push_back(reader.GetFloat("electrons_lipSignificance")->at(eleIt));
      vars.leptons_tipSig.push_back(reader.GetFloat("electrons_tipSignificance")->at(eleIt));
      vars.leptons_3DipSig.push_back(reader.GetFloat("electrons_3DipSignificance")->at(eleIt));
      
      vars.electrons_isEB.push_back(reader.GetInt("electrons_isEB")->at(eleIt));      
      vars.electrons_fbrem.push_back(reader.GetFloat("electrons_fbrem")->at(eleIt));
      vars.electrons_HoverE.push_back(reader.GetFloat("electrons_hOverE")->at(eleIt));
      vars.electrons_DphiIn.push_back(reader.GetFloat("electrons_deltaPhiIn")->at(eleIt));
      vars.electrons_DetaIn.push_back(reader.GetFloat("electrons_deltaEtaIn")->at(eleIt));
      vars.electrons_sigmaIetaIeta.push_back(reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt));
      
      ++nEle;
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < 10. ) continue;
      
      vars.muons.push_back( reader.Get4V("muons")->at(muIt) );
      vars.leptons.push_back( reader.Get4V("muons")->at(muIt) );      
      vars.leptonCharges.push_back( reader.GetFloat("muons_charge")->at(muIt) );
      vars.leptonFlavours.push_back("muon");
      vars.leptons_tkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
      vars.leptons_emIso.push_back(reader.GetFloat("muons_emIsoR03")->at(muIt));
      vars.leptons_hadIso.push_back(reader.GetFloat("muons_hadIsoR03")->at(muIt));
      vars.leptons_dxy.push_back(reader.GetFloat("muons_dxy")->at(muIt));
      vars.leptons_dz.push_back(reader.GetFloat("muons_dz")->at(muIt));
      vars.leptons_lipSig.push_back(reader.GetFloat("muons_lipSignificance")->at(muIt));
      vars.leptons_tipSig.push_back(reader.GetFloat("muons_tipSignificance")->at(muIt));
      vars.leptons_3DipSig.push_back(reader.GetFloat("muons_3DipSignificance")->at(muIt));
      
      vars.muons_tracker.push_back(reader.GetInt("muons_tracker")->at(muIt));
      vars.muons_standalone.push_back(reader.GetInt("muons_standalone")->at(muIt));
      vars.muons_global.push_back(reader.GetInt("muons_global")->at(muIt));
      vars.muons_normalizedChi2.push_back(reader.GetFloat("muons_normalizedChi2")->at(muIt));
      vars.muons_numberOfValidTrackerHits.push_back(reader.GetInt("muons_numberOfValidTrackerHits")->at(muIt));
      vars.muons_numberOfValidMuonHits.push_back(reader.GetInt("muons_numberOfValidMuonHits")->at(muIt));
    }
    
    if( (int)(vars.leptons.size()) < lepNMIN ) continue;
    if( (int)(vars.leptons.size()) > lepNMAX ) continue;
    if( (int)(vars.electrons.size()) < eleNMIN ) continue;
    if( (int)(vars.muons.size()) < muNMIN ) continue;
    
    
    
    // select lepton
    vars.selectIt_lep = SelectLepton(vars.leptons, "maxPt", 15.);
    vars.lepton = vars.leptons.at(vars.selectIt_lep);
    if(vars.selectIt_lep == -1) continue;
    
    
    vars.selectIt_ele = -1;
    if(vars.leptonFlavours.at(vars.selectIt_lep) == "electron")
      for(unsigned int eleIt = 0; eleIt < vars.electrons.size(); ++eleIt)
      {
        ROOT::Math::XYZTVector ele = vars.electrons.at(eleIt);
        if( deltaR(ele.eta(), ele.phi(), vars.lepton.eta(), vars.lepton.phi()) < 0.0001 )
          vars.selectIt_ele = eleIt;
      }
    
    vars.selectIt_mu = -1;
    if(vars.leptonFlavours.at(vars.selectIt_lep) == "muon")
      for(unsigned int muIt = 0; muIt < vars.muons.size(); ++muIt)
      {
        ROOT::Math::XYZTVector mu = vars.muons.at(muIt);
        if(deltaR(mu.eta(), mu.phi(), vars.lepton.eta(), vars.lepton.phi()) < 0.0001)
          vars.selectIt_mu = muIt;
      }
    
    
    
    //****
    // met

    vars.caloMet = reader.Get4V("met")->at(0);
    vars.type1Met = reader.Get4V("type1Met")->at(0);
    vars.pfMet = reader.Get4V("PFMet")->at(0);
    vars.genMet = reader.Get4V("genMet")->at(0);
    
    if(jetType == "Calo")    
      vars.met = reader.Get4V("type1Met")->at(0);
    if(jetType == "PF")    
      vars.met = reader.Get4V("PFMet")->at(0);
    
    vars.lepMet = vars.lepton + vars.met;
    vars.lepMet_mt = sqrt( vars.lepton.mass() * vars.lepton.mass() +
                           2. * vars.lepton.pt() * vars.met.pt() * ( 1 - cos(deltaPhi(vars.lepton.phi(), vars.met.phi()) ) ) );
    
    
    
    // fill distributions
    stepEvents[step] += 1;
    for(int njetBin = 2; njetBin <= jetNMAX; ++njetBin)
    {
      if( vars.leptonCharges.at(vars.selectIt_lep) > 0. ) (stepEvents_plus[njetBin])[step] += 1;
      if( vars.leptonCharges.at(vars.selectIt_lep) < 0. ) (stepEvents_minus[njetBin])[step] += 1;
    }
    FillLeptonHistograms(vars, step);
    FillMetHistograms(vars, step);
    

    
    
    
    
    
    
    //**********************
    // STEP 5 - select jets
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "5) select jets";
    
    
    // jets
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      
      if( jet.Et() < jetEtMIN ) continue;
      if( reader.GetFloat("jets_emEnergyFraction")->at(jetIt) > 0.98 ) continue;
      
      
      // clean jets from electrons
      bool skipJet = false;
      for(unsigned int eleIt = 0; eleIt < vars.electrons.size(); ++eleIt)
      {
        ROOT::Math::XYZTVector ele = vars.electrons.at(eleIt);
        if( deltaR(jet.eta(), jet.phi(), ele.eta(), ele.phi()) < 0.5 )
          skipJet = true;
      }
      
      // clean jets from muons
      for(unsigned int muIt = 0; muIt < vars.muons.size(); ++muIt)
      {
        ROOT::Math::XYZTVector mu = vars.muons.at(muIt);
        if( deltaR(jet.eta(), jet.phi(), mu.eta(), mu.phi()) < 0.5 )
          skipJet = true;
      }
      
      if(skipJet == true) continue;
      
      
      vars.jets.push_back( reader.Get4V("jets")->at(jetIt) );
      vars.jets_bTag.push_back( reader.GetFloat("jets_trackCountingHighEffBJetTags")->at(jetIt) );
      vars.jets_emEnergyFraction.push_back( reader.GetFloat("jets_emEnergyFraction")->at(jetIt) );
      vars.jets_etaEtaMoment.push_back( reader.GetFloat("jets_etaetaMoment")->at(jetIt) );
      vars.jets_phiPhiMoment.push_back( reader.GetFloat("jets_phiphiMoment")->at(jetIt) );
      vars.jets_etaPhiMoment.push_back( reader.GetFloat("jets_etaphiMoment")->at(jetIt) );
      vars.jets_fHPD.push_back( reader.GetFloat("jets_fHPD")->at(jetIt) );
      vars.jets_fRBX.push_back( reader.GetFloat("jets_fRBX")->at(jetIt) );
      vars.jets_n90Hits.push_back( reader.GetFloat("jets_n90Hits")->at(jetIt) );
      vars.jets_etOrdered.push_back( reader.Get4V("jets")->at(jetIt).Et() );
      vars.jets_bTagOrdered.push_back( reader.GetFloat("jets_trackCountingHighEffBJetTags")->at(jetIt) );
    }
    
    
    if( (int)(vars.jets.size()) < jetNMIN ) continue;
    if( (int)(vars.jets.size()) > jetNMAX ) continue;
    
    
    vars.nJets = vars.jets.size();
    std::sort(vars.jets_etOrdered.begin(), vars.jets_etOrdered.end(), maggiore);
    std::sort(vars.jets_bTagOrdered.begin(), vars.jets_bTagOrdered.end(), maggiore);

    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    FillLeptonHistograms(vars, step);
    FillMetHistograms(vars, step);
    FillJetHistograms(vars, step);
    
    
    
    
    
    
    //**************************************
    // STEP 6 - Initial cuts - lepton pt/eta
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "6) lepton pt/eta";
    
    if( vars.lepton.pt() < lepPtMIN ) continue;
    if( vars.lepton.pt() > lepPtMAX ) continue;
    if( fabs(vars.lepton.eta()) > lepAbsEtaMAX ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    FillLeptonHistograms(vars, step);
    FillMetHistograms(vars, step);
    FillJetHistograms(vars, step);
    
    
    
    
    
    
    
    //*****************************************
    // STEP 7 - Initial cuts - lepton isolation
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "7) lepton isolation";
    
    
    if( vars.leptonFlavours.at(vars.selectIt_lep) == "electron" )
    {
      // barrel
      if( vars.electrons_isEB.at(vars.selectIt_ele) == 1)
      {
        if( vars.leptons_tkIso.at(vars.selectIt_lep)/vars.lepton.pt()  > eleTkIsoOverPtEBMAX ) continue;
        if( vars.leptons_emIso.at(vars.selectIt_lep)/vars.lepton.pt() > eleEmIsoOverPtEBMAX ) continue;
        if( vars.leptons_hadIso.at(vars.selectIt_lep)/vars.lepton.pt() > eleHadIsoOverPtEBMAX ) continue;
      }
      
      // endcap
      else
      {
        if( vars.leptons_tkIso.at(vars.selectIt_lep)/vars.lepton.pt() > eleTkIsoOverPtEEMAX ) continue;
        if( vars.leptons_emIso.at(vars.selectIt_lep)/vars.lepton.pt() > eleEmIsoOverPtEEMAX ) continue;
        if( vars.leptons_hadIso.at(vars.selectIt_lep)/vars.lepton.pt() > eleHadIsoOverPtEEMAX ) continue;
      }
    }
    
    if( vars.leptonFlavours.at(vars.selectIt_lep) == "muon")
    {
      //isolation
      if( vars.leptons_tkIso.at(vars.selectIt_lep)/vars.lepton.pt()  > muTkIsoOverPtMAX ) continue;
      if( vars.leptons_emIso.at(vars.selectIt_lep)/vars.lepton.pt() > muEmIsoOverPtMAX ) continue;
      if( vars.leptons_hadIso.at(vars.selectIt_lep)/vars.lepton.pt() > muHadIsoOverPtMAX ) continue;
    }    

    // fill distributions
    stepEvents[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    FillLeptonHistograms(vars, step);
    FillMetHistograms(vars, step);
    FillJetHistograms(vars, step);
    
    
    
    
    
    
    //**********************************
    // STEP 8 - Initial cuts - lepton ID
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "8) lepton ID";
    
    if( vars.leptonFlavours.at(vars.selectIt_lep) == "electron" )
    {
      // barrel
      if( vars.electrons_isEB.at(vars.selectIt_ele) == 1)
      {
        if( vars.electrons_HoverE.at(vars.selectIt_ele) > eleHoverEEBMAX ) continue;
        if( fabs(vars.electrons_DphiIn.at(vars.selectIt_ele)) > eleDphiInEBMAX ) continue;
        if( fabs(vars.electrons_DetaIn.at(vars.selectIt_ele)) > eleDetaInEBMAX ) continue;
        if( vars.electrons_sigmaIetaIeta.at(vars.selectIt_ele)  > eleSigmaIetaIetaEBMAX ) continue;
      }
      
      // endcap
      {
        if( vars.electrons_HoverE.at(vars.selectIt_ele) > eleHoverEEEMAX ) continue;
        if( fabs(vars.electrons_DphiIn.at(vars.selectIt_ele)) > eleDphiInEEMAX ) continue;
        if( fabs(vars.electrons_DetaIn.at(vars.selectIt_ele)) > eleDetaInEEMAX ) continue;
        if( vars.electrons_sigmaIetaIeta.at(vars.selectIt_ele)  > eleSigmaIetaIetaEEMAX ) continue;      
      }    
    }
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    FillLeptonHistograms(vars, step);
    FillMetHistograms(vars, step);
    FillJetHistograms(vars, step);
    
    
    
    
    
    
    //****************************
    // STEP 9 - Initial cuts - met
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "9) met";
    
    if( vars.met.Et() < metEtMIN ) continue;
    if( vars.met.Et() > metEtMAX ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    FillLeptonHistograms(vars, step);
    FillMetHistograms(vars, step);
    FillJetHistograms(vars, step);
    
    
    
    
    
    
    //*******************************************
    // STEP 10 - Initial cuts - lepton met mt/dphi
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "10) lepton+met mt/dphi";
    
    if( (vars.lepMet_mt < lepMetMtMIN) || (vars.lepMet_mt > lepMetMtMAX) ) continue;
    if( fabs(deltaPhi(vars.lepton.phi(), vars.met.phi())) < lepMetDphiMIN ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    FillLeptonHistograms(vars, step);
    FillMetHistograms(vars, step);
    FillJetHistograms(vars, step);

    
    
    
    
    
    //**********************************
    // STEP 11 - Initial cuts - b-tagging
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "11) b-tagging";
    
    if( vars.jets_bTagOrdered.at(0) > 2.5 ) continue;
    if( vars.jets_bTagOrdered.at(1) > 2.0 ) continue;
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.leptonCharges.at(vars.selectIt_lep) < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    FillLeptonHistograms(vars, step);
    FillMetHistograms(vars, step);
    FillJetHistograms(vars, step);
    
    
    
  } // loop over the events
  
  
  
  
  
  
  // save tree
  //for(step = 1; step <= nStep; ++step)
  //  tree[step] -> Write();
  //
  //for(step = 1; step <= nStep; ++step)
  //{
  //  treeEvents[step] -> Fill();
  //  treeEvents[step] -> Write();
  //}  
  //
  //outputTreeFile -> Close();
  //delete outputTreeFile;
  
  
  
  // save histograms
  TFile* outputRootFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  outputRootFile -> cd();
  
  for(step = 1; step <= nStep; ++step)
  {
    events -> SetBinContent(step, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step, stepName[step].c_str());
    
    for(int njetBin = jetNMIN; njetBin <= jetNMAX; ++njetBin)
    {
      events_plus[njetBin] -> SetBinContent(step, (stepEvents_plus[njetBin])[step]);
      events_plus[njetBin] -> GetXaxis() -> SetBinLabel(step, stepName[step].c_str());

      events_minus[njetBin] -> SetBinContent(step, (stepEvents_minus[njetBin])[step]);
      events_minus[njetBin] -> GetXaxis() -> SetBinLabel(step, stepName[step].c_str());      
    } 
  }
  
  events -> Write();
  for(int njetBin = jetNMIN; njetBin <= jetNMAX; ++njetBin)
  {
    events_plus[njetBin] -> Write();
    events_minus[njetBin] -> Write();
  }
  outputRootFile -> Close(); 
  
  
  
  DeleteWplusWminusRatioVariables(vars);
  delete outputRootFile;
  
  return 0;
}

