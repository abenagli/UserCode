#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "stdHisto.h"

#include <iomanip>

#include "TH1F.h"
#include "TObject.h"
#include "RooFitResult.h"
#include "RooWorkspace.h"
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooAbsPdf.h"
#include "RooAddPdf.h"
#include "RooGaussian.h"





int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFHiggsToWWTolnujjAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  float mH = gConfigParser -> readFloatOption("Options::mH");
  float crossSection = gConfigParser -> readFloatOption("Options::crossSection");
  
  int jetNMIN = gConfigParser -> readIntOption("Cuts::jetNMIN");
  float jetEtMIN = gConfigParser -> readFloatOption("Cuts::jetEtMIN");
  
  int lepNMIN = gConfigParser -> readIntOption("Cuts::lepNMIN");
  float lepPtMIN = gConfigParser -> readFloatOption("Cuts::lepPtMIN");
  float lepPtMAX = gConfigParser -> readFloatOption("Cuts::lepPtMAX");
  float lepTkIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::lepTkIsoOverPtMAX");
  float eleIdValueMIN = gConfigParser -> readFloatOption("Cuts::eleIdValueMIN");
  float lepZeppMAX = gConfigParser -> readFloatOption("Cuts::lepZeppMAX");
  float lepTipSigMAX = gConfigParser -> readFloatOption("Cuts::lepTipSigMAX");
  
  float tagJetDetaMIN = gConfigParser -> readFloatOption("Cuts::tagJetDetaMIN");
  float tagJetMjjMIN = gConfigParser -> readFloatOption("Cuts::tagJetMjjMIN");
  float tagJet1EtMIN = gConfigParser -> readFloatOption("Cuts::tagJet1EtMIN");
  float tagJet1EtMAX = gConfigParser -> readFloatOption("Cuts::tagJet1EtMAX");
  float tagJet2EtMIN = gConfigParser -> readFloatOption("Cuts::tagJet2EtMIN");
  float tagJet2EtMAX = gConfigParser -> readFloatOption("Cuts::tagJet2EtMAX");
  float tagJetBTagMAX = gConfigParser -> readFloatOption("Cuts::tagJetBTagMAX");

  float WJetMjjMIN = gConfigParser -> readFloatOption("Cuts::WJetMjjMIN");
  float WJetMjjMAX = gConfigParser -> readFloatOption("Cuts::WJetMjjMAX");
  float WJet1EtMIN = gConfigParser -> readFloatOption("Cuts::WJet1EtMIN");
  float WJet1EtMAX = gConfigParser -> readFloatOption("Cuts::WJet1EtMAX");
  float WJet2EtMIN = gConfigParser -> readFloatOption("Cuts::WJet2EtMIN");
  float WJet2EtMAX = gConfigParser -> readFloatOption("Cuts::WJet2EtMAX");
  float WJetBTagMAX = gConfigParser -> readFloatOption("Cuts::WJetBTagMAX");
  
  float metEtMIN = gConfigParser -> readFloatOption("Cuts::metEtMIN");
  float metEtMAX = gConfigParser -> readFloatOption("Cuts::metEtMAX");
  
  float lepMet_W_DphiMAX = gConfigParser -> readFloatOption("Cuts::lepMetWDphiMAX");
  float lepMetWMtMAX = gConfigParser -> readFloatOption("Cuts::lepMetWMtMAX");
  
  std::map<int, int> totalEvents = GetTotalEvents("events", inputFileList.c_str()); 
  
  
  
  
  
  
  // Open tree
  std::string treeName = "SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain));  
  
  
  
  // define histograms
  int nStep = 10;
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepName;
  
  
  
  int step = 0;
  stepEvents[step] = totalEvents[1];
  stepName[step] = "total events";
  
  step = 1;
  stepEvents[step] = totalEvents[2];
  stepName[step] = "VBFHiggsToWWTolnujjPreselection";
  
  
  
  // define histograms
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+"_"+jetAlgorithm+".root";
  
  // histograms
  hFactory* histograms = new hFactory(outputRootFullFileName);
  
  histograms -> add_h1("tagJJ_bTag",  "",  2000, -100., 100., nStep);

  histograms -> add_h1("WJJ_zepp",     "", 2000,    -1.,   1., nStep);  
  histograms -> add_h1("WJJ_max_zepp", "", 2000,    -1.,   1., nStep);
  histograms -> add_h1("WJJ_min_zepp", "", 2000,    -1.,   1., nStep);
  histograms -> add_h1("WJJ_bTag",     "", 2000,  -100., 100., nStep);

  histograms -> add_h1("otherJ_zepp",   "", 2000, -1.,   1., nStep);
  histograms -> add_h1("centralJ_zepp", "", 2000, -1.,   1., nStep);
  
  histograms -> add_h1("lep_zepp",    "", 2000,  -1.,  1., nStep);  
  histograms -> add_h1("lep_lipSig",  "", 2000, -50., 50., nStep);
  histograms -> add_h1("lep_tipSig",  "", 1000,   0., 50., nStep);  
  histograms -> add_h1("lep_3DipSig", "", 2000, -50., 50., nStep);
  
  
  // stdHistograms
  stdHisto* stdHistograms = new stdHisto(nStep, outputRootFullFileName);
  
  stdHistograms -> Add2("tagJJ",    nStep);
  stdHistograms -> Add2("WJJ",      nStep);
  stdHistograms -> Add1("otherJ",   nStep);
  stdHistograms -> Add1("centralJ", nStep);
  stdHistograms -> Add1("met",      nStep);
  stdHistograms -> Add1("lep",      nStep);
  stdHistograms -> Add2("lepMet",   nStep);
  stdHistograms -> Add2("lepMetW",  nStep);
  
  
  
  // define roofit variables
  //RooRealVar var_tagJJ_Deta("var_tagJJ_Deta", "var_tagJJ_Deta", 0., 10.) ; 
  //RooRealVar var_WJJ_mass("var_WJJ_mass", "var_WJJ_mass", 0., 500.) ; 
  //RooRealVar var_lepMet_W_Dphi("var_lepMet_W_Dphi", "var_lepMet_W_Dphi", 0., 3.) ;
  //RooArgSet var_set(var_tagJJ_Deta, var_WJJ_mass);
    
  // Define tree variables
  float tagJJ_Deta;
  float tagJJ_m;
  float tagJJ_max_et;
  float tagJJ_min_et;
  float WJJ_Deta;
  float WJJ_m;
  float WJJ_max_et;
  float WJJ_min_et;
  float lepMetW_Dphi;
  
  // Define tree
  std::string outputTreeFileName = outputRootFilePath+"../tree.root";
  TFile* outputTreeFile = new TFile(outputTreeFileName.c_str(), "UPDATE");
  
  std::string outputTreeEventsFileName = outputRootFilePath+"../treeEvents.root";
  TFile* outputTreeEventsFile = new TFile(outputTreeEventsFileName.c_str(), "UPDATE");
  
  std::vector<TTree*> tree;
  std::vector<TTree*> treeEvents;
  for(int i = 0; i < nStep; ++i)
  {
    tree.push_back(NULL);
    treeEvents.push_back(NULL);
  }
  
  for(int i = 0; i < nStep; ++i)
  { 
    char treeName[50];
    sprintf(treeName, "tree_%d", i);
    tree.at(i) = (TTree*)(outputTreeFile -> Get(treeName));
    
    char treeEventsName[50];
    sprintf(treeEventsName, "treeEvents_%d", i);
    treeEvents.at(i) = (TTree*)(outputTreeEventsFile -> Get(treeEventsName));
    
    if(tree.at(i) == NULL)
    {
      tree.at(i) = new TTree(treeName, treeName);
      
      tree.at(i) -> Branch("mH",           &mH,                     "mH/F");
      tree.at(i) -> Branch("totEvents",    &stepEvents[0],   "totEvents/I");
      tree.at(i) -> Branch("crossSection", &crossSection, "crossSection/F");
      tree.at(i) -> Branch("tagJJ_Deta",   &tagJJ_Deta,     "tagJJ_Deta/F");
      tree.at(i) -> Branch("tagJJ_m",      &tagJJ_m,           "tagJJ_m/F");  
      tree.at(i) -> Branch("tagJJ_max_et", &tagJJ_max_et, "tagJJ_max_et/F");   
      tree.at(i) -> Branch("tagJJ_min_et", &tagJJ_min_et, "tagJJ_min_et/F"); 
      tree.at(i) -> Branch("WJJ_Deta",     &WJJ_Deta,         "WJJ_Deta/F");
      tree.at(i) -> Branch("WJJ_m",        &WJJ_m,               "WJJ_m/F");  
      tree.at(i) -> Branch("WJJ_max_et",   &WJJ_max_et,     "WJJ_max_et/F");   
      tree.at(i) -> Branch("WJJ_min_et",   &WJJ_min_et,     "WJJ_min_et/F");
      tree.at(i) -> Branch("lepMetW_Dphi", &lepMetW_Dphi, "lepMetW_Dphi/F");
      
      
      treeEvents.at(i) = new TTree(treeEventsName, treeEventsName);
      
      treeEvents.at(i) -> Branch("mH",           &mH,                     "mH/F");
      treeEvents.at(i) -> Branch("totEvents",    &stepEvents[0],   "totEvents/I");
      treeEvents.at(i) -> Branch("crossSection", &crossSection, "crossSection/F");
      treeEvents.at(i) -> Branch("events",       &stepEvents[i],      "events/I");
    }
    else
    {
      tree.at(i) -> SetBranchAddress("mH",           &mH);
      tree.at(i) -> SetBranchAddress("totEvents",    &stepEvents[0]);
      tree.at(i) -> SetBranchAddress("crossSection", &crossSection);
      tree.at(i) -> SetBranchAddress("tagJJ_Deta",   &tagJJ_Deta);
      tree.at(i) -> SetBranchAddress("tagJJ_m",      &tagJJ_m);  
      tree.at(i) -> SetBranchAddress("tagJJ_max_et", &tagJJ_max_et);   
      tree.at(i) -> SetBranchAddress("tagJJ_min_et", &tagJJ_min_et); 
      tree.at(i) -> SetBranchAddress("WJJ_Deta",     &WJJ_Deta);
      tree.at(i) -> SetBranchAddress("WJJ_m",        &WJJ_m);  
      tree.at(i) -> SetBranchAddress("WJJ_max_et",   &WJJ_max_et);   
      tree.at(i) -> SetBranchAddress("WJJ_min_et",   &WJJ_min_et);
      tree.at(i) -> SetBranchAddress("lepMetW_Dphi", &lepMetW_Dphi);
      
      
      treeEvents.at(i) -> SetBranchAddress("mH",           &mH);
      treeEvents.at(i) -> SetBranchAddress("totEvents",    &stepEvents[0]);
      treeEvents.at(i) -> SetBranchAddress("crossSection", &crossSection);
      treeEvents.at(i) -> SetBranchAddress("events",       &stepEvents[i]);
    }
  }
  
  
  
  
  
  
  
  
  
  
  
  //*********************
  // LOOP OVER THE EVENTS
  
  std::cout << ">>>>> VBFHiggsToWWTolnujjAnalysis::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> VBFHiggsToWWTolnujjPreselection::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    //************
    // SELECT JETS
    
    std::vector<ROOT::Math::XYZTVector> jets;
    std::vector<ROOT::Math::XYZTVector> otherJets;
    std::vector<ROOT::Math::XYZTVector> centralJets;
    std::vector<ROOT::Math::XYZTVector> electrons_jetCleaning;
    std::vector<float> jets_bTag;
    
    // build the collection of electros for jet cleaning
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 5. ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.5 ) continue;
      if( (reader.GetFloat("electrons_IdRobustLoose")->at(eleIt)) < 1. ) continue;
      
      electrons_jetCleaning.push_back( reader.Get4V("electrons")->at(eleIt) );
    }
    
    // clean jets
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      if( jet.Et() < jetEtMIN ) continue;
      
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
    
    if( (int)(jets.size()) < jetNMIN ) continue;
    
    
    
    // select jets    
    std::vector<int> selectIt_tag;
    std::vector<int> blacklistIt_tag;
    std::vector<int> selectIt_W;
    
    SelectJets(selectIt_tag, jets, "maxMJJ", jetEtMIN);
    
    blacklistIt_tag.push_back(selectIt_tag.at(0));
    blacklistIt_tag.push_back(selectIt_tag.at(1));
    SelectJets(selectIt_W, jets, "maxPt", jetEtMIN, &blacklistIt_tag);
    
    if( (selectIt_tag.at(0) == -1) || (selectIt_tag.at(1) == -1) ||
        (selectIt_W.at(0)   == -1) || (selectIt_W.at(1)   == -1) )
      continue;
    
    
    
    //*********
    // tag jets
    
    ROOT::Math::XYZTVector jet1_tag = jets.at(selectIt_tag.at(0));
    ROOT::Math::XYZTVector jet2_tag = jets.at(selectIt_tag.at(1));    
    float jet1_tag_bTag = jets_bTag.at(selectIt_tag.at(0));
    float jet2_tag_bTag = jets_bTag.at(selectIt_tag.at(1));
    
    ROOT::Math::XYZTVector jet12_tag = jet1_tag + jet2_tag;
    
    float avgEta_tag = 0.5 * (jet1_tag.eta() + jet2_tag.eta());
    float absDeta_tag = deltaEta(jet1_tag.eta(), jet2_tag.eta());
    
    
    
    //*******
    // W jets
        
    ROOT::Math::XYZTVector jet1_W = jets.at(selectIt_W.at(0));
    ROOT::Math::XYZTVector jet2_W = jets.at(selectIt_W.at(1));        
    float jet1_W_bTag = jets_bTag.at(selectIt_W.at(0));
    float jet2_W_bTag = jets_bTag.at(selectIt_W.at(1));        
    ROOT::Math::XYZTVector jet12_W = jet1_W + jet2_W;
    
    float jet12_W_zepp = (jet12_W.eta() - avgEta_tag)/absDeta_tag;
    float jet1_W_zepp  = (jet1_W.eta()  - avgEta_tag)/absDeta_tag;
    float jet2_W_zepp  = (jet2_W.eta()  - avgEta_tag)/absDeta_tag;    
    
    
    
    //***********************
    // other and central jets
    
    for(unsigned int jetIt = 0; jetIt < jets.size(); ++jetIt)
    {
      if( jetIt == (unsigned int)(selectIt_tag.at(0)) ) continue;
      if( jetIt == (unsigned int)(selectIt_tag.at(1)) ) continue;
      if( jetIt == (unsigned int)(selectIt_W.at(0)) ) continue;
      if( jetIt == (unsigned int)(selectIt_W.at(1)) ) continue;      
      
      otherJets.push_back(jets.at(jetIt));
      
      if( fabs(jets.at(jetIt).eta()) < fabs(avgEta_tag) )
        centralJets.push_back(jets.at(jetIt));
    }
    
    
    
    
    
    
    //**************
    // SELECT LEPTON
    
    std::vector<ROOT::Math::XYZTVector> electrons;
    std::vector<ROOT::Math::XYZTVector> muons;
    std::vector<ROOT::Math::XYZTVector> leptons;
    std::vector<std::string> leptonFlavours;    
    std::vector<float> leptons_tkIso;
    std::vector<float> leptons_lipSig;
    std::vector<float> leptons_tipSig;
    std::vector<float> leptons_3DipSig;
        
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.5 ) continue;
      if( (reader.GetFloat("electrons_IdRobustTight")->at(eleIt)) < eleIdValueMIN ) continue;
      
      electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      leptons.push_back( reader.Get4V("electrons")->at(eleIt) );      
      leptonFlavours.push_back("electron");
      leptons_tkIso.push_back(reader.GetFloat("electrons_tkIso")->at(eleIt));
      leptons_lipSig.push_back(reader.GetFloat("electrons_lipSignificance")->at(eleIt));
      leptons_tipSig.push_back(reader.GetFloat("electrons_tipSignificance")->at(eleIt));
      leptons_3DipSig.push_back(reader.GetFloat("electrons_3DipSignificance")->at(eleIt));
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < 10. ) continue;
      if( (reader.GetFloat("muons_tkIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.5 ) continue;
      
      muons.push_back( reader.Get4V("muons")->at(muIt) );
      leptons.push_back( reader.Get4V("muons")->at(muIt) );      
      leptonFlavours.push_back("muon");
      leptons_tkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
      leptons_lipSig.push_back(reader.GetFloat("muons_lipSignificance")->at(muIt));
      leptons_tipSig.push_back(reader.GetFloat("muons_tipSignificance")->at(muIt));
      leptons_3DipSig.push_back(reader.GetFloat("muons_3DipSignificance")->at(muIt));
    }
    
    if( (int)(leptons.size()) < lepNMIN ) continue;
    
    
    
    // select lepton
    int selectIt_lep = SelectLepton(leptons, "maxPt", 10.);    
    ROOT::Math::XYZTVector lepton = leptons.at(selectIt_lep);
    if(selectIt_lep == -1) continue;
    
    float lep_zepp = (lepton.eta() - avgEta_tag)/absDeta_tag;
    float lep_lipSig = leptons_lipSig.at(selectIt_lep);
    float lep_tipSig = leptons_tipSig.at(selectIt_lep);
    float lep_3DipSig = leptons_3DipSig.at(selectIt_lep);
    
    ROOT::Math::XYZTVector met = reader.Get4V("met")->at(0);
    ROOT::Math::XYZTVector lepMet = lepton + met;
    ROOT::Math::XYZTVector lepMetW = lepMet + jet12_W;
    ROOT::Math::XYZTVector lepMetWTag = lepMetW + jet12_tag;
    
    
    
    
    
    
    //***********************
    // CUT VARIABLES FOR TREE
    
    tagJJ_Deta   = deltaEta(jet1_tag.eta(), jet2_tag.eta());
    tagJJ_m      = jet12_tag.mass();
    tagJJ_max_et = std::max(jet1_tag.Et(), jet2_tag.Et());
    tagJJ_min_et = std::min(jet1_tag.Et(), jet2_tag.Et());
    WJJ_Deta     = deltaEta(jet1_W.eta(), jet2_W.eta());
    WJJ_m        = jet12_W.mass();
    WJJ_max_et   = std::max(jet1_W.Et(), jet2_W.Et());
    WJJ_min_et   = std::min(jet1_W.Et(), jet2_W.Et());
    lepMetW_Dphi = deltaPhi(lepMet.phi(), jet12_W.phi());
    
    
    
    
    
    
    
    
    
    
    
    
    //**********************
    // STEP 2 - Preselection
    step = 2;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "VBFHiggsToWWTolnujjPreselection";
    
        
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_zepp", step, jet12_W_zepp);
    histograms -> Fill("WJJ_max_zepp", step, fabs(jet1_W_zepp) > fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_min_zepp", step, fabs(jet1_W_zepp) < fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );    
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    for(unsigned int jetIt = 0; jetIt < otherJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector otherJet = otherJets.at(jetIt);
      float otherJet_zepp = (otherJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("otherJ_zepp", step, otherJet_zepp);
    }

    for(unsigned int jetIt = 0; jetIt < centralJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector centralJet = centralJets.at(jetIt);
      float centralJet_zepp = (centralJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("centralJ_zepp", step, centralJet_zepp);
    }
    
    histograms -> Fill("lep_zepp", step, lep_zepp);    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //*********************************
    // STEP 3 - Initial cuts - tag jets
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "tag jet cuts";
    
    if( deltaEta(jet1_tag.eta(), jet2_tag.eta()) < tagJetDetaMIN ) continue;
    if( jet12_tag.mass() < tagJetMjjMIN ) continue;
    if( jet1_tag.Et() < tagJet1EtMIN ) continue;
    if( jet1_tag.Et() > tagJet1EtMAX ) continue;    
    if( jet2_tag.Et() < tagJet2EtMIN ) continue;
    if( jet2_tag.Et() > tagJet2EtMAX ) continue;
    if( jet1_tag_bTag > tagJetBTagMAX ) continue; 
    if( jet2_tag_bTag > tagJetBTagMAX ) continue; 
    
    
    // fill distributions
    stepEvents[step] += 1;

    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_zepp", step, jet12_W_zepp);
    histograms -> Fill("WJJ_max_zepp", step, fabs(jet1_W_zepp) > fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_min_zepp", step, fabs(jet1_W_zepp) < fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    for(unsigned int jetIt = 0; jetIt < otherJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector otherJet = otherJets.at(jetIt);
      float otherJet_zepp = (otherJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("otherJ_zepp", step, otherJet_zepp);
    }

    for(unsigned int jetIt = 0; jetIt < centralJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector centralJet = centralJets.at(jetIt);
      float centralJet_zepp = (centralJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("centralJ_zepp", step, centralJet_zepp);
    }
    
    histograms -> Fill("lep_zepp", step, lep_zepp);    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
            
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //*******************************
    // STEP 4 - Initial cuts - W jets
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "W jet cuts";
    
    if( jet12_W.mass() < WJetMjjMIN ) continue;
    if( jet12_W.mass() > WJetMjjMAX ) continue;
    if( jet1_W.Et() < WJet1EtMIN ) continue;
    if( jet1_W.Et() > WJet1EtMAX ) continue;
    if( jet2_W.Et() < WJet2EtMIN ) continue;
    if( jet2_W.Et() > WJet2EtMAX ) continue;
    if( jet1_W_bTag > WJetBTagMAX ) continue; 
    if( jet2_W_bTag > WJetBTagMAX ) continue; 
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_zepp", step, jet12_W_zepp);
    histograms -> Fill("WJJ_max_zepp", step, fabs(jet1_W_zepp) > fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_min_zepp", step, fabs(jet1_W_zepp) < fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    for(unsigned int jetIt = 0; jetIt < otherJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector otherJet = otherJets.at(jetIt);
      float otherJet_zepp = (otherJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("otherJ_zepp", step, otherJet_zepp);
    }

    for(unsigned int jetIt = 0; jetIt < centralJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector centralJet = centralJets.at(jetIt);
      float centralJet_zepp = (centralJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("centralJ_zepp", step, centralJet_zepp);
    }
    
    histograms -> Fill("lep_zepp", step, lep_zepp);    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //*******************************
    // STEP 5 - Initial cuts - lepton
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "lepton cuts";
    
    if( lepton.pt() < lepPtMIN ) continue;
    if( lepton.pt() > lepPtMAX ) continue;
    if( leptons_tkIso.at(selectIt_lep) / lepton.pt() > lepTkIsoOverPtMAX) continue;
    if( fabs(lep_zepp) > lepZeppMAX ) continue;
    if( lep_tipSig < lepTipSigMAX ) continue; 
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_zepp", step, jet12_W_zepp);
    histograms -> Fill("WJJ_max_zepp", step, fabs(jet1_W_zepp) > fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_min_zepp", step, fabs(jet1_W_zepp) < fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    for(unsigned int jetIt = 0; jetIt < otherJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector otherJet = otherJets.at(jetIt);
      float otherJet_zepp = (otherJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("otherJ_zepp", step, otherJet_zepp);
    }

    for(unsigned int jetIt = 0; jetIt < centralJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector centralJet = centralJets.at(jetIt);
      float centralJet_zepp = (centralJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("centralJ_zepp", step, centralJet_zepp);
    }
    
    histograms -> Fill("lep_zepp", step, lep_zepp);    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //*******************************
    // STEP 6 - Initial cuts - met
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "met cuts";
    
    if( met.Et() < metEtMIN ) continue;
    if( met.Et() > metEtMAX ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_zepp", step, jet12_W_zepp);
    histograms -> Fill("WJJ_max_zepp", step, fabs(jet1_W_zepp) > fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_min_zepp", step, fabs(jet1_W_zepp) < fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    for(unsigned int jetIt = 0; jetIt < otherJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector otherJet = otherJets.at(jetIt);
      float otherJet_zepp = (otherJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("otherJ_zepp", step, otherJet_zepp);
    }

    for(unsigned int jetIt = 0; jetIt < centralJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector centralJet = centralJets.at(jetIt);
      float centralJet_zepp = (centralJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("centralJ_zepp", step, centralJet_zepp);
    }
    
    histograms -> Fill("lep_zepp", step, lep_zepp);    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //***************************************
    // STEP 7 - Initial cuts - lepMet - W cut
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "lepMet-W Dphi";
    
    if( deltaPhi(lepMet.phi(), jet12_W.phi()) > lepMet_W_DphiMAX ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_zepp", step, jet12_W_zepp);
    histograms -> Fill("WJJ_max_zepp", step, fabs(jet1_W_zepp) > fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_min_zepp", step, fabs(jet1_W_zepp) < fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    for(unsigned int jetIt = 0; jetIt < otherJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector otherJet = otherJets.at(jetIt);
      float otherJet_zepp = (otherJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("otherJ_zepp", step, otherJet_zepp);
    }

    for(unsigned int jetIt = 0; jetIt < centralJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector centralJet = centralJets.at(jetIt);
      float centralJet_zepp = (centralJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("centralJ_zepp", step, centralJet_zepp);
    }
    
    histograms -> Fill("lep_zepp", step, lep_zepp);    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //***************************************
    // STEP 8 - Initial cuts - CJV
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "CJV";
    
    
    if( otherJets.size() > 0 ) continue;
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_zepp", step, jet12_W_zepp);
    histograms -> Fill("WJJ_max_zepp", step, fabs(jet1_W_zepp) > fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_min_zepp", step, fabs(jet1_W_zepp) < fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    for(unsigned int jetIt = 0; jetIt < otherJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector otherJet = otherJets.at(jetIt);
      float otherJet_zepp = (otherJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("otherJ_zepp", step, otherJet_zepp);
    }

    for(unsigned int jetIt = 0; jetIt < centralJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector centralJet = centralJets.at(jetIt);
      float centralJet_zepp = (centralJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("centralJ_zepp", step, centralJet_zepp);
    }
    
    histograms -> Fill("lep_zepp", step, lep_zepp);    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //**********************************************
    // STEP 9 - Initial cuts - higgs transverse mass
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "Higgs mt";
    
    if( lepMetW.mt() > lepMetWMtMAX ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_zepp", step, jet12_W_zepp);
    histograms -> Fill("WJJ_max_zepp", step, fabs(jet1_W_zepp) > fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_min_zepp", step, fabs(jet1_W_zepp) < fabs(jet2_W_zepp) ? jet1_W_zepp : jet2_W_zepp );
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    for(unsigned int jetIt = 0; jetIt < otherJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector otherJet = otherJets.at(jetIt);
      float otherJet_zepp = (otherJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("otherJ_zepp", step, otherJet_zepp);
    }

    for(unsigned int jetIt = 0; jetIt < centralJets.size(); ++jetIt)
    {
      ROOT::Math::XYZTVector centralJet = centralJets.at(jetIt);
      float centralJet_zepp = (centralJet.eta() - avgEta_tag)/absDeta_tag;
      
      histograms -> Fill("centralJ_zepp", step, centralJet_zepp);
    }
    
    histograms -> Fill("lep_zepp", step, lep_zepp);    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    tree.at(step) -> Fill();
    
    
    
  } // loop over the events
  
  
  
  
  
  
  // clone tree
  /*std::vector<TTree*> newTree;
  for(step = 0; step < nStep; ++step)
    newTree.push_back( (TTree*)(tree.at(step)->Clone()) );
  outputTreeFile -> Close(); 
  */
  // save tree in a new file
  //outputTreeFile = new TFile(outputTreeFileName.c_str(), "RECREATE");
  outputTreeFile -> cd();
  for(step = 0; step < nStep; ++step)
    tree.at(step) -> Write("", TObject::kOverwrite);
  outputTreeFile -> Close();
  
  
  

  outputTreeEventsFile -> cd();
  for(step = 0; step < nStep; ++step)
  {
    treeEvents.at(step) -> Fill();
    treeEvents.at(step) -> Write("", TObject::kOverwrite);
  }  
  outputTreeEventsFile -> Close();
  
  
  
  
  
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
  
  //RooDataSet data("data", "data", &ntu, var_set) ;
  //data.Write();
  
  delete histograms;
  delete stdHistograms;
  
  
  return 0;
}
