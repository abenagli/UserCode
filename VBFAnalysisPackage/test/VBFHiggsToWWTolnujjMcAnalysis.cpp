#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>

#include "TH1F.h"
#include "TObject.h"






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFHiggsToWWTolnujjMcAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType       = gConfigParser -> readStringOption("Input::jetType");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  float mH = gConfigParser -> readFloatOption("Options::mH");
  float crossSection = gConfigParser -> readFloatOption("Options::crossSection");
  
  std::map<int,int> totalEvents = GetTotalEvents("MCDecayModeFilterAll/totalEvents", inputFileList.c_str());
  
  std::string VBFPreselectionName;
  if(jetType == "Calo")
    VBFPreselectionName = "VBFEtMinCaloJetCountFilter" + jetAlgorithm+"/passedEvents";
  if(jetType == "PF")
    VBFPreselectionName = "VBFEtMinPFJetCountFilter" + jetAlgorithm+"/passedEvents";
  std::map<int,int> VBFPreselectionEvents = GetTotalEvents(VBFPreselectionName.c_str(), inputFileList.c_str());
  
  
  
  
  
  
  // Open tree
  std::string treeName = "VBFNtuple"+jetAlgorithm+"/SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain));  
  
  
  
  // define histograms
  int nStep = 4;
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepName;
  
  
  
  int step = 0;
  stepEvents[step] = totalEvents[1];
  stepName[step] = "total events";
  
  
  
  
  
  
  // define histograms
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+"_"+jetAlgorithm+".root";
  
  // histograms
  hFactory* histograms = new hFactory(outputRootFullFileName);
  h2Factory* histograms2 = new h2Factory(outputRootFullFileName);
  
  histograms -> add_h1("mcLepMet_mt",  "", 3000, 0., 3000., nStep);
  histograms -> add_h1("mcLepMetW_mt", "", 3000, 0., 3000., nStep);
  histograms -> add_h1("mcTagQQ_sumE", "",  3000, 0., 3000., nStep);
  histograms -> add_h1("mcWQQ_sumE",   "",  3000, 0., 3000., nStep);
  histograms -> add_h1("mcTagWQ_pt1", "",  3000, 0.,  3000., nStep);   
  histograms -> add_h1("mcTagWQ_pt2", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("mcTagWQ_pt3", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("mcTagWQ_pt4", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("mcLepWQ_pt1", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("mcLepWQ_pt2", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("mcLepWQ_pt3", "",  3000, 0.,  3000., nStep);

  histograms2 -> add_h2("mcTagWQQ_m_vs_Deta", "", 200,  0., 10., 3000, 0., 3000., nStep);  
  histograms2 -> add_h2("mcLepW_pt1_vs_mcTagQQ_pt", "", 500, 0., 500., 500, 0., 500., nStep);

  histograms -> add_h1("jets_et1",  "",  3000, 0.,  3000., nStep);   
  histograms -> add_h1("jets_et2",  "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("jets_et3",  "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("jets_et4",  "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("ele_tkIso",  "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("ele_emIso",  "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("ele_hadIso", "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("mu_tkIso",   "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("mu_emIso",   "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("mu_hadIso",  "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("lepMet_mt",  "", 3000, 0., 3000., nStep);
  histograms -> add_h1("lepMetW_mt", "", 3000, 0., 3000., nStep);
  histograms -> add_h1("tagJJ_sumE", "", 3000, 0.,  3000., nStep);
  histograms -> add_h1("WJJ_sumE",   "", 3000, 0.,  3000., nStep);
  histograms -> add_h1("tagWJ_et1", "",  3000, 0.,  3000., nStep);   
  histograms -> add_h1("tagWJ_et2", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("tagWJ_et3", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("tagWJ_et4", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("lepWJ_pt1", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("lepWJ_pt2", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("lepWJ_pt3", "",  3000, 0.,  3000., nStep);
  
  histograms2 -> add_h2("tagWJJ_m_vs_Deta", "", 200,  0., 10., 3000, 0., 3000., nStep);
  histograms2 -> add_h2("lepWJ_pt1_vs_tagJJ_pt", "", 500, 0., 500., 500, 0., 500., nStep);  
  
  // stdHistograms
  stdHisto* stdHistograms = new stdHisto(nStep, outputRootFullFileName);

  stdHistograms -> Add1("mcH",       nStep, true);  
  stdHistograms -> Add2("mcWW",      nStep, true);
  stdHistograms -> Add2("mcTagQQ",   nStep);
  stdHistograms -> Add2("mcWQQ",     nStep, true);
  stdHistograms -> Add1("mcMet",     nStep);
  stdHistograms -> Add1("mcLep",     nStep, true);
  stdHistograms -> Add2("mcLepMet",  nStep, true);
  stdHistograms -> Add2("mcLepMetW", nStep, true);
  
  stdHistograms -> Add2("tagJJ",   nStep);
  stdHistograms -> Add2("WJJ",     nStep, true);
  stdHistograms -> Add1("met",     nStep);
  stdHistograms -> Add1("lep",     nStep, true);
  stdHistograms -> Add2("lepMet",  nStep, true);
  stdHistograms -> Add2("lepMetW", nStep, true);  
  stdHistograms -> Add1("otherJ_et15", nStep, false);
  stdHistograms -> Add1("otherJ_et20", nStep, false);
  stdHistograms -> Add1("otherJ_et25", nStep, false);
  stdHistograms -> Add1("otherJ_et30", nStep, false);
  stdHistograms -> Add1("centralJ_et15", nStep, false);  
  stdHistograms -> Add1("centralJ_et20", nStep, false);
  stdHistograms -> Add1("centralJ_et25", nStep, false);
  stdHistograms -> Add1("centralJ_et30", nStep, false);
  
  
  
  
  
  
  
  
  
  
  
  //*********************
  // LOOP OVER THE EVENTS
  
  std::cout << ">>>>> VBFHiggsToWWTolnujjMcAnalysis::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> VBFHiggsToWWTolnujjMcAnalysis::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    
    
    
    //**************
    // DUMP MC TRUTH
    ROOT::Math::XYZTVector mcH = reader.Get4V("mc_H")->at(0);
    ROOT::Math::XYZTVector mcW1 = reader.Get4V("mcV1")->at(0);
    ROOT::Math::XYZTVector mcW2 = reader.Get4V("mcV2")->at(0);
    ROOT::Math::XYZTVector mcQ1_tag = (reader.Get4V("mcQ1_tag"))->at(0);
    ROOT::Math::XYZTVector mcQ2_tag = (reader.Get4V("mcQ2_tag"))->at(0);
    ROOT::Math::XYZTVector mcQ12_tag = mcQ1_tag + mcQ2_tag;
    ROOT::Math::XYZTVector mcQ1_W   = (reader.Get4V("mcF1_fromV2"))->at(0);
    ROOT::Math::XYZTVector mcQ2_W   = (reader.Get4V("mcF2_fromV2"))->at(0);
    ROOT::Math::XYZTVector mcQ12_W   = mcQ1_W + mcQ2_W;
    ROOT::Math::XYZTVector mcLepton = (reader.Get4V("mcF1_fromV1"))->at(0);
    if( (fabs(reader.GetFloat("mcF1_fromV1_pdgId")->at(0)) == 12) ||
        (fabs(reader.GetFloat("mcF1_fromV1_pdgId")->at(0)) == 14) ||
        (fabs(reader.GetFloat("mcF1_fromV1_pdgId")->at(0)) == 16) )
      mcLepton = (reader.Get4V("mcF2_fromV1"))->at(0);
    ROOT::Math::XYZTVector mcMet = (reader.Get4V("genMet"))->at(0);        
    ROOT::Math::XYZTVector mcLepMet = mcLepton + mcMet;
    ROOT::Math::XYZTVector mcLepMetW = mcLepMet + mcQ12_W;
    ROOT::Math::XYZTVector mcLepW = mcLepton + mcQ12_W;

    float mcLepMet_mt = sqrt( mcLepton.mass()*mcLepton.mass() + 2.*mcLepton.pt()*mcMet.pt()*(1-cos(deltaPhi(mcLepton.phi(), mcMet.phi()))));    
    float mcLepMetW_mt = sqrt( mcLepW.mass()*mcLepW.mass() + 2.*mcLepW.pt()*mcMet.pt()*(1-cos(deltaPhi(mcLepW.phi(), mcMet.phi()))));
    
    float mcEta1_tag = mcQ1_tag.eta();
    float mcEta2_tag = mcQ2_tag.eta();
    float mcAvgEta_tag = 0.5 * (mcQ1_tag.eta() + mcQ2_tag.eta());
    float mcAbsDeta_tag = deltaEta(mcQ1_tag.eta(), mcQ2_tag.eta());
    
    std::vector<float> mcTagWQ_pt;
    mcTagWQ_pt.push_back(mcQ1_tag.pt());
    mcTagWQ_pt.push_back(mcQ2_tag.pt());
    mcTagWQ_pt.push_back(mcQ1_W.pt());
    mcTagWQ_pt.push_back(mcQ2_W.pt());
    sort(mcTagWQ_pt.begin(), mcTagWQ_pt.end(), maggiore);
    
    std::vector<float> mcLepWQ_pt;
    mcLepWQ_pt.push_back(mcQ1_W.pt());
    mcLepWQ_pt.push_back(mcQ2_W.pt());
    mcLepWQ_pt.push_back(mcLepton.pt());
    sort(mcLepWQ_pt.begin(), mcLepWQ_pt.end(), maggiore);    
    
    
    
    
    
    
    //************
    // SELECT JETS
    
    std::vector<ROOT::Math::XYZTVector> jets;
    std::vector<ROOT::Math::XYZTVector> otherJets_et15;
    std::vector<ROOT::Math::XYZTVector> otherJets_et20;
    std::vector<ROOT::Math::XYZTVector> otherJets_et25;
    std::vector<ROOT::Math::XYZTVector> otherJets_et30;        
    std::vector<ROOT::Math::XYZTVector> centralJets_et15;
    std::vector<ROOT::Math::XYZTVector> centralJets_et20;
    std::vector<ROOT::Math::XYZTVector> centralJets_et25;
    std::vector<ROOT::Math::XYZTVector> centralJets_et30;
    std::vector<ROOT::Math::XYZTVector> electrons_jetCleaning;
    std::vector<float> jets_bTag;
    
    // build the collection of electros for jet cleaning
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.5 ) continue;
      if( (reader.GetFloat("electrons_IdTight")->at(eleIt)) < 1. ) continue;
      
      electrons_jetCleaning.push_back( reader.Get4V("electrons")->at(eleIt) );
    }
    
    // clean jets
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      if( jet.Et() < 15. ) continue;
      
      bool skipJet = false;
      for(unsigned int eleIt = 0; eleIt < electrons_jetCleaning.size(); ++eleIt)
      {
        ROOT::Math::XYZTVector ele = electrons_jetCleaning.at(eleIt);
        if( deltaR(jet.eta(), jet.phi(), ele.eta(), ele.phi()) < 0.3 )
          skipJet = true;
      }
      if(skipJet == true) continue;
      
      jets.push_back( reader.Get4V("jets")->at(jetIt) );
      jets_bTag.push_back( reader.GetFloat("jets_trackCountingHighEffBJetTags")->at(jetIt) );
    }
    
    
    
    
    
    
    //**************
    // SELECT LEPTON
    
    std::vector<ROOT::Math::XYZTVector> electrons;
    std::vector<ROOT::Math::XYZTVector> muons;
    std::vector<ROOT::Math::XYZTVector> leptons;
    std::vector<float> leptonCharges;
    std::vector<std::string> leptonFlavours;    
    std::vector<float> leptons_tkIso;
    std::vector<float> leptons_emIso;
    std::vector<float> leptons_hadIso;
    std::vector<float> leptons_eleIdRobustTight;
    std::vector<float> leptons_lipSig;
    std::vector<float> leptons_tipSig;
    std::vector<float> leptons_3DipSig;
        
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 15. ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.1 ) continue;
      if( (reader.GetFloat("electrons_IdRobustTight")->at(eleIt)) < 1. ) continue;
      
      electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      leptons.push_back( reader.Get4V("electrons")->at(eleIt) );      
      leptonCharges.push_back( reader.GetFloat("electrons_charge")->at(eleIt) );
      leptonFlavours.push_back("electron");
      leptons_tkIso.push_back(reader.GetFloat("electrons_tkIso")->at(eleIt));
      leptons_emIso.push_back(reader.GetFloat("electrons_emIso")->at(eleIt));
      leptons_hadIso.push_back(reader.GetFloat("electrons_hadIso_1")->at(eleIt));
      leptons_eleIdRobustTight.push_back(reader.GetFloat("electrons_IdRobustTight")->at(eleIt));
      leptons_lipSig.push_back(reader.GetFloat("electrons_lipSignificance")->at(eleIt));
      leptons_tipSig.push_back(reader.GetFloat("electrons_tipSignificance")->at(eleIt));
      leptons_3DipSig.push_back(reader.GetFloat("electrons_3DipSignificance")->at(eleIt));
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < 15. ) continue;
      if( (reader.GetFloat("muons_tkIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.1 ) continue;
      
      muons.push_back( reader.Get4V("muons")->at(muIt) );
      leptons.push_back( reader.Get4V("muons")->at(muIt) );      
      leptonCharges.push_back( reader.GetFloat("muons_charge")->at(muIt) );
      leptonFlavours.push_back("muon");
      leptons_tkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
      leptons_emIso.push_back(reader.GetFloat("muons_emIsoR03")->at(muIt));
      leptons_hadIso.push_back(reader.GetFloat("muons_hadIsoR03")->at(muIt));
      leptons_lipSig.push_back(reader.GetFloat("muons_lipSignificance")->at(muIt));
      leptons_tipSig.push_back(reader.GetFloat("muons_tipSignificance")->at(muIt));
      leptons_3DipSig.push_back(reader.GetFloat("muons_3DipSignificance")->at(muIt));
    }
    
    
    
    
    
    
    
    
    
    
    
    
    // **********************
    // STEP 1 - No selections
    step = 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "no selections";
    
    // fill distributions
    stepEvents[step] += 1;

    histograms -> Fill("mcTagQQ_sumE", step, mcQ1_tag.energy() + mcQ2_tag.energy());    
    histograms -> Fill("mcWQQ_sumE",   step, mcQ1_W.energy()   + mcQ2_W.energy());
    histograms -> Fill("mcTagWQ_pt1", step, mcTagWQ_pt.at(0));
    histograms -> Fill("mcTagWQ_pt2", step, mcTagWQ_pt.at(1));
    histograms -> Fill("mcTagWQ_pt3", step, mcTagWQ_pt.at(2));
    histograms -> Fill("mcTagWQ_pt4", step, mcTagWQ_pt.at(3));
    histograms -> Fill("mcLepWQ_pt1", step, mcLepWQ_pt.at(0));
    histograms -> Fill("mcLepWQ_pt2", step, mcLepWQ_pt.at(1));
    histograms -> Fill("mcLepWQ_pt3", step, mcLepWQ_pt.at(2));
    
    histograms -> Fill("mcLepMet_mt", step, mcLepMet_mt);
    histograms -> Fill("mcLepMetW_mt", step, mcLepMetW_mt);
    
    histograms2 -> Fill("mcTagWQQ_m_vs_Deta", step, deltaEta(mcQ1_tag.eta(), mcQ2_tag.eta()), mcQ12_tag.mass());  
    histograms2 -> Fill("mcTagWQQ_m_vs_Deta", step, deltaEta(mcQ1_W.eta(),   mcQ2_W.eta()),   mcQ12_W.mass());
    histograms2 -> Fill("mcLepW_pt1_vs_mcTagQQ_pt", step, mcQ12_tag.pt(), mcLepWQ_pt.at(0));
    
    stdHistograms -> Fill1(mcH, "mcH", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill2(mcW1, mcW2, "mcWW", step, &mcEta1_tag, &mcEta2_tag);    
    stdHistograms -> Fill2(mcQ1_tag, mcQ2_tag, "mcTagQQ", step);
    stdHistograms -> Fill2(mcQ1_W, mcQ2_W, "mcWQQ", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill1(mcMet, "mcMet", step);
    stdHistograms -> Fill1(mcLepton, "mcLep", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill2(mcLepton, mcMet, "mcLepMet", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill2(mcLepMet, mcQ12_W, "mcLepMetW", step, &mcEta1_tag, &mcEta2_tag);
    
    
    
    
    
    
    
    
    
    
    
    
    // **************************************
    // STEP 2 - there exist 4 jets + 1 lepton
    step = step+1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "exist 4 jet / 1 lepton";
    
    if( (int)(jets.size()) < 4 ) continue;
    if( (int)(leptons.size()) < 1 ) continue;
    
    
    std::vector<float> jets_et;
    for(unsigned int jetIt = 0; jetIt < jets.size(); ++jetIt)
      jets_et.push_back( jets.at(jetIt).Et() );
    sort(jets_et.begin(), jets_et.end(), maggiore);
    
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("mcTagQQ_sumE", step, mcQ1_tag.energy() + mcQ2_tag.energy());    
    histograms -> Fill("mcWQQ_sumE",   step, mcQ1_W.energy()   + mcQ2_W.energy());
    histograms -> Fill("mcTagWQ_pt1", step, mcTagWQ_pt.at(0));
    histograms -> Fill("mcTagWQ_pt2", step, mcTagWQ_pt.at(1));
    histograms -> Fill("mcTagWQ_pt3", step, mcTagWQ_pt.at(2));
    histograms -> Fill("mcTagWQ_pt4", step, mcTagWQ_pt.at(3));
    histograms -> Fill("mcLepWQ_pt1", step, mcLepWQ_pt.at(0));
    histograms -> Fill("mcLepWQ_pt2", step, mcLepWQ_pt.at(1));
    histograms -> Fill("mcLepWQ_pt3", step, mcLepWQ_pt.at(2));
    
    histograms -> Fill("mcLepMet_mt", step, mcLepMet_mt);
    histograms -> Fill("mcLepMetW_mt", step, mcLepMetW_mt);
    
    histograms2 -> Fill("mcTagWQQ_m_vs_Deta", step, deltaEta(mcQ1_tag.eta(), mcQ2_tag.eta()), mcQ12_tag.mass());
    histograms2 -> Fill("mcTagWQQ_m_vs_Deta", step, deltaEta(mcQ1_W.eta(),   mcQ2_W.eta()),   mcQ12_W.mass());
    histograms2 -> Fill("mcLepW_pt1_vs_mcTagQQ_pt", step, mcQ12_tag.pt(), mcLepWQ_pt.at(0));
    
    histograms -> Fill("jets_et1", step, jets_et.at(0));
    histograms -> Fill("jets_et2", step, jets_et.at(1));
    histograms -> Fill("jets_et3", step, jets_et.at(2));
    histograms -> Fill("jets_et4", step, jets_et.at(3));    

    stdHistograms -> Fill1(mcH, "mcH", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill2(mcW1, mcW2, "mcWW", step, &mcEta1_tag, &mcEta2_tag);    
    stdHistograms -> Fill2(mcQ1_tag, mcQ2_tag, "mcTagQQ", step);
    stdHistograms -> Fill2(mcQ1_W, mcQ2_W, "mcWQQ", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill1(mcMet, "mcMet", step);
    stdHistograms -> Fill1(mcLepton, "mcLep", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill2(mcLepton, mcMet, "mcLepMet", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill2(mcLepMet, mcQ12_W, "mcLepMetW", step, &mcEta1_tag, &mcEta2_tag);
    
    
    
    
    
    
    
    
    
    
    
    
    // *******************************************************
    // STEP 3 - there exist 4 jets + 1 lepton matching with mc
    step = step+1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "exist 4 jet / 1 lepton matching";
    
    if( (int)(jets.size()) < 4 ) continue;
    if( (int)(leptons.size()) < 1 ) continue;
    
    
    
    std::vector<ROOT::Math::XYZTVector> mcQuarks;
    mcQuarks.push_back( mcQ1_tag );
    mcQuarks.push_back( mcQ2_tag );    
    mcQuarks.push_back( mcQ1_W );
    mcQuarks.push_back( mcQ2_W );
    
    std::vector<int> matchIt;
    if( GetMatching(jets, mcQuarks, 0.3, 0.5, 1.5, &matchIt) < 4 ) continue;

    
    
    //*********
    // tag jets
    ROOT::Math::XYZTVector jet1_tag = jets.at(matchIt.at(0));
    ROOT::Math::XYZTVector jet2_tag = jets.at(matchIt.at(1));    
    
    ROOT::Math::XYZTVector jet12_tag = jet1_tag + jet2_tag;
    
    float eta1_tag = jet1_tag.eta();
    float eta2_tag = jet2_tag.eta();
    
    float etaMin_tag = std::min(jet1_tag.eta(), jet2_tag.eta());
    float etaMax_tag = std::max(jet1_tag.eta(), jet2_tag.eta());
    
    float avgEta_tag = 0.5 * (jet1_tag.eta() + jet2_tag.eta());
    float absDeta_tag = deltaEta(jet1_tag.eta(), jet2_tag.eta());
    
    
    
    //*******
    // W jets
    ROOT::Math::XYZTVector jet1_W = jets.at(matchIt.at(2));
    ROOT::Math::XYZTVector jet2_W = jets.at(matchIt.at(3));        
    ROOT::Math::XYZTVector jet12_W = jet1_W + jet2_W;
    
    float jet12_W_zepp = (jet12_W.eta() - avgEta_tag)/absDeta_tag;
    float jet1_W_zepp  = (jet1_W.eta()  - avgEta_tag)/absDeta_tag;
    float jet2_W_zepp  = (jet2_W.eta()  - avgEta_tag)/absDeta_tag;
    
    
    
    std::vector<float> tagWJ_et;
    tagWJ_et.push_back(jet1_tag.Et());
    tagWJ_et.push_back(jet2_tag.Et());
    tagWJ_et.push_back(jet1_W.Et());
    tagWJ_et.push_back(jet2_W.Et());
    sort(tagWJ_et.begin(), tagWJ_et.end(), maggiore);
    
    
    
    //***********************
    // other and central jets
    for(unsigned int jetIt = 0; jetIt < jets.size(); ++jetIt)
    {
      if( jetIt == (unsigned int)(matchIt.at(0)) ) continue;
      if( jetIt == (unsigned int)(matchIt.at(1)) ) continue;
      if( jetIt == (unsigned int)(matchIt.at(2)) ) continue;
      if( jetIt == (unsigned int)(matchIt.at(3)) ) continue;      
      
      if( jets.at(jetIt).Et() > 15. )
        otherJets_et15.push_back(jets.at(jetIt));
      if( jets.at(jetIt).Et() > 20. )
        otherJets_et20.push_back(jets.at(jetIt));
      if( jets.at(jetIt).Et() > 25. )
        otherJets_et25.push_back(jets.at(jetIt));
      if( jets.at(jetIt).Et() > 30. )
        otherJets_et30.push_back(jets.at(jetIt));
      
      if( (jets.at(jetIt).eta() > etaMin_tag) &&
          (jets.at(jetIt).eta() < etaMax_tag) )
      {
        if( jets.at(jetIt).Et() > 15. )
          centralJets_et15.push_back(jets.at(jetIt));
        if( jets.at(jetIt).Et() > 20. )
          centralJets_et20.push_back(jets.at(jetIt));
        if( jets.at(jetIt).Et() > 25. )
          centralJets_et25.push_back(jets.at(jetIt));
        if( jets.at(jetIt).Et() > 30. )
          centralJets_et30.push_back(jets.at(jetIt));
      }
    }
    
    
    
    
    
    
    std::vector<ROOT::Math::XYZTVector> mcLeptons;
    mcLeptons.push_back( mcLepton );
    
    std::vector<int> matchIt_lepton;
    if( GetMatching(leptons, mcLeptons, 0.05, 0.9, 1.1, &matchIt_lepton) < 1 ) continue;    
    
    ROOT::Math::XYZTVector lepton = leptons.at(matchIt_lepton.at(0));
    float lep_zepp = (lepton.eta() - avgEta_tag)/absDeta_tag;
    
    ROOT::Math::XYZTVector met;
    if(jetType == "Calo")    
      met = reader.Get4V("met")->at(0);
    if(jetType == "PF")    
      met = reader.Get4V("PFMet")->at(0);
    ROOT::Math::XYZTVector lepMet = lepton + met;
    ROOT::Math::XYZTVector lepMetW = lepMet + jet12_W;
    ROOT::Math::XYZTVector lepW = lepton + jet12_W;
    ROOT::Math::XYZTVector lepMetWTag = lepMetW + jet12_tag;
    
    float lepMet_mt = sqrt( lepton.mass()*lepton.mass() + 2.*lepton.pt()*met.pt()*(1-cos(deltaPhi(lepton.phi(), met.phi()))));    
    float lepMetW_mt = sqrt( lepW.mass()*lepW.mass() + 2.*lepW.pt()*met.pt()*(1-cos(deltaPhi(lepW.phi(), met.phi()))));
    
    
    
    std::vector<float> lepWJ_pt;
    lepWJ_pt.push_back(jet1_W.Et());
    lepWJ_pt.push_back(jet2_W.Et());
    lepWJ_pt.push_back(lepton.pt());
    sort(lepWJ_pt.begin(), lepWJ_pt.end(), maggiore);
    
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("mcTagQQ_sumE", step, mcQ1_tag.energy() + mcQ2_tag.energy());    
    histograms -> Fill("mcWQQ_sumE",   step, mcQ1_W.energy()   + mcQ2_W.energy());
    histograms -> Fill("mcTagWQ_pt1", step, mcTagWQ_pt.at(0));
    histograms -> Fill("mcTagWQ_pt2", step, mcTagWQ_pt.at(1));
    histograms -> Fill("mcTagWQ_pt3", step, mcTagWQ_pt.at(2));
    histograms -> Fill("mcTagWQ_pt4", step, mcTagWQ_pt.at(3));
    histograms -> Fill("mcLepWQ_pt1", step, mcLepWQ_pt.at(0));
    histograms -> Fill("mcLepWQ_pt2", step, mcLepWQ_pt.at(1));
    histograms -> Fill("mcLepWQ_pt3", step, mcLepWQ_pt.at(2));
    
    histograms -> Fill("mcLepMet_mt", step, mcLepMet_mt);
    histograms -> Fill("mcLepMetW_mt", step, mcLepMetW_mt);
    
    histograms2 -> Fill("mcTagWQQ_m_vs_Deta", step, deltaEta(mcQ1_tag.eta(), mcQ2_tag.eta()), mcQ12_tag.mass());
    histograms2 -> Fill("mcTagWQQ_m_vs_Deta", step, deltaEta(mcQ1_W.eta(),   mcQ2_W.eta()),   mcQ12_W.mass());
    histograms2 -> Fill("mcLepW_pt1_vs_mcTagQQ_pt", step, mcQ12_tag.pt(), mcLepWQ_pt.at(0));
    
    stdHistograms -> Fill1(mcH, "mcH", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill2(mcW1, mcW2, "mcWW", step, &mcEta1_tag, &mcEta2_tag);    
    stdHistograms -> Fill2(mcQ1_tag, mcQ2_tag, "mcTagQQ", step);
    stdHistograms -> Fill2(mcQ1_W, mcQ2_W, "mcWQQ", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill1(mcMet, "mcMet", step);
    stdHistograms -> Fill1(mcLepton, "mcLep", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill2(mcLepton, mcMet, "mcLepMet", step, &mcEta1_tag, &mcEta2_tag);
    stdHistograms -> Fill2(mcLepMet, mcQ12_W, "mcLepMetW", step, &mcEta1_tag, &mcEta2_tag);
    
    
    
    histograms -> Fill("jets_et1", step, jets_et.at(0));
    histograms -> Fill("jets_et2", step, jets_et.at(1));
    histograms -> Fill("jets_et3", step, jets_et.at(2));
    histograms -> Fill("jets_et4", step, jets_et.at(3));    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    
    if(leptonFlavours.at(matchIt_lepton.at(0)) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(matchIt_lepton.at(0))/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(matchIt_lepton.at(0))/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(matchIt_lepton.at(0))/lepton.Et());
    }
    if(leptonFlavours.at(matchIt_lepton.at(0)) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(matchIt_lepton.at(0))/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(matchIt_lepton.at(0))/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(matchIt_lepton.at(0))/lepton.Et());
    }
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), jet12_tag.mass());    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   jet12_W.mass());
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill2(lepton, met, "lepMet", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(otherJets_et15, "otherJ_et15", step);
    stdHistograms -> Fill1(otherJets_et20, "otherJ_et20", step);
    stdHistograms -> Fill1(otherJets_et25, "otherJ_et25", step);
    stdHistograms -> Fill1(otherJets_et30, "otherJ_et30", step);
    stdHistograms -> Fill1(centralJets_et15, "centralJ_et15", step);
    stdHistograms -> Fill1(centralJets_et20, "centralJ_et20", step);
    stdHistograms -> Fill1(centralJets_et25, "centralJ_et25", step);
    stdHistograms -> Fill1(centralJets_et30, "centralJ_et30", step);
        
    
    
  } // loop over the events
  
  
  
  
  
  
  // save event histogram
  TFile* outputRootFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  outputRootFile -> cd();
  for(step = 0; step < nStep; ++step)
  {
    events -> SetBinContent(step+1, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step+1, stepName[step].c_str());
  }
  
  events -> Write(); 
  outputRootFile -> Close(); 
  
  delete histograms;
  delete histograms2;
  delete stdHistograms;
  
  
  return 0;
}
