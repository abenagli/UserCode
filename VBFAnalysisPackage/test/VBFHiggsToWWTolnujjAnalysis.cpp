#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "stdHisto.h"

#include <iomanip>

#include "TH1F.h"
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
  
  int jetNMIN = gConfigParser -> readIntOption("Cuts::jetNMIN");
  float jetEtMIN = gConfigParser -> readFloatOption("Cuts::jetEtMIN");
  
  int lepNMIN = gConfigParser -> readIntOption("Cuts::lepNMIN");
  float lepPtMIN = gConfigParser -> readFloatOption("Cuts::lepPtMIN");
  float lepPtMAX = gConfigParser -> readFloatOption("Cuts::lepPtMAX");
  float lepTkIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::lepTkIsoOverPtMAX");
  float eleIdValueMIN = gConfigParser -> readFloatOption("Cuts::eleIdValueMIN");
  float lepZeppMAX = gConfigParser -> readFloatOption("Cuts::lepZeppMAX");
  
  float tagJetDetaMIN = gConfigParser -> readFloatOption("Cuts::tagJetDetaMIN");
  float tagJetMjjMIN = gConfigParser -> readFloatOption("Cuts::tagJetMjjMIN");
  float tagJet1EtMIN = gConfigParser -> readFloatOption("Cuts::tagJet1EtMIN");
  float tagJet1EtMAX = gConfigParser -> readFloatOption("Cuts::tagJet1EtMAX");
  float tagJet2EtMIN = gConfigParser -> readFloatOption("Cuts::tagJet2EtMIN");
  float tagJet2EtMAX = gConfigParser -> readFloatOption("Cuts::tagJet2EtMAX");

  float WJetMjjMIN = gConfigParser -> readFloatOption("Cuts::WJetMjjMIN");
  float WJetMjjMAX = gConfigParser -> readFloatOption("Cuts::WJetMjjMAX");
  float WJet1EtMIN = gConfigParser -> readFloatOption("Cuts::WJet1EtMIN");
  float WJet1EtMAX = gConfigParser -> readFloatOption("Cuts::WJet1EtMAX");
  float WJet2EtMIN = gConfigParser -> readFloatOption("Cuts::WJet2EtMIN");
  float WJet2EtMAX = gConfigParser -> readFloatOption("Cuts::WJet2EtMAX");
  
  float metEtMIN = gConfigParser -> readFloatOption("Cuts::metEtMIN");
  float metEtMAX = gConfigParser -> readFloatOption("Cuts::metEtMAX");
  
  float lepMet_W_DphiMAX = gConfigParser -> readFloatOption("Cuts::lepMetWDphiMAX");
  
  int totalEvents = GetTotalEvents("events", inputFileList.c_str()); 
  
  
  
  
  
  
  // Open tree
  std::string treeName = "SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain));  
  
  
  
  // define histograms
  int nStep = 8;
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepName;
  
  
  
  int step = 0;
  stepEvents[step] = totalEvents;
  stepName[step] = "total events";
  
  
  
  // define histograms
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+"_"+jetAlgorithm+".root";
  stdHisto* stdHistograms = new stdHisto(nStep, outputRootFullFileName);
  //hFactory* histograms = new hFactory(outputRootFullFileName);
  
  
  
  // tag jets
  stdHistograms -> Add2("tagJJ", nStep);
  stdHistograms -> Add2("WJJ", nStep);
  stdHistograms -> Add1("otherJ", nStep);
  stdHistograms -> Add1("centralJ", nStep);
  stdHistograms -> Add1("met", nStep);
  stdHistograms -> Add1("lep", nStep);
  stdHistograms -> Add2("lepMet", nStep);
  stdHistograms -> Add2("lepMetW", nStep);
  
  //// other jets
  //histograms -> add_h1("otherJet_n",    "",  10,  0.,  10., nStep);
  //histograms -> add_h1("otherJet_et",   "", 500,  0., 500., nStep);
  //histograms -> add_h1("otherJet_zepp", "", 400, -1.,   1., nStep);
  //
  //// central jets
  //histograms -> add_h1("centralJet_n",    "",  10,  0.,  10., nStep);
  //histograms -> add_h1("centralJet_et",   "", 500,  0., 500., nStep);
  //histograms -> add_h1("centralJet_zepp", "", 400, -1.,   1., nStep);
  //
  //// lepton
  //histograms -> add_h1("lep_pt",         "", 500,  0., 500., nStep);
  //histograms -> add_h1("lep_zepp",       "", 400, -1.,   1., nStep);
  //histograms -> add_h1("lep_W_Deta",     "", 400,  0.,  10., nStep);  
  //histograms -> add_h1("lep_W_Dphi",     "", 200,  0.,   5., nStep);  
  //histograms -> add_h1("lep_tag_Dphi",   "", 200,  0.,   5., nStep);
  //histograms -> add_h1("lepMet_mt",      "", 500,  0., 500., nStep);
  //histograms -> add_h1("lepMet_W_Dphi",  "", 200,  0.,   5., nStep);
  //  
  //// met
  //histograms -> add_h1("met_et",       "", 500, 0., 500., nStep);
  //histograms -> add_h1("met_lep_Dphi", "", 200, 0.,   5., nStep);
  //histograms -> add_h1("met_W_Dphi",   "", 200, 0.,   5., nStep);
  //histograms -> add_h1("met_tag_Dphi", "", 200, 0.,   5., nStep);  
  //
  //// higgs  
  //histograms -> add_h1("lepMetW_pt",       "",  500, 0.,  500., nStep);
  //histograms -> add_h1("lepMetW_mt",       "", 1000, 0., 1000., nStep);  
  //histograms -> add_h1("lepMetW_tag_Dphi", "",  200, 0.,    5., nStep);  
  //
  //// pt tot  
  //histograms -> add_h1("lepMetWTag_pt", "", 500, 0., 500., nStep);  
  
  
  
  
  
  
  // Loop over events
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
    
    // build the collection of electros for jet cleaning
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
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
    
    ROOT::Math::XYZTVector jet1_tag = jets.at(selectIt_tag.at(0));
    ROOT::Math::XYZTVector jet2_tag = jets.at(selectIt_tag.at(1));    
    ROOT::Math::XYZTVector jet12_tag = jet1_tag + jet2_tag;
    
    ROOT::Math::XYZTVector jet1_W = jets.at(selectIt_W.at(0));
    ROOT::Math::XYZTVector jet2_W = jets.at(selectIt_W.at(1));        
    ROOT::Math::XYZTVector jet12_W = jet1_W + jet2_W;
    
    float avgEta_tag = 0.5 * (jet1_tag.eta() + jet2_tag.eta());
    float absDeta_tag = deltaEta(jet1_tag.eta(), jet2_tag.eta());
    
    
    
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
    
    
    
    std::vector<ROOT::Math::XYZTVector> electrons;
    std::vector<ROOT::Math::XYZTVector> muons;
    std::vector<ROOT::Math::XYZTVector> leptons;
    std::vector<std::string> leptonFlavours;    
    std::vector<float> leptonTkIso;
        
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.5 ) continue;
      if( (reader.GetFloat("electrons_IdRobustTight")->at(eleIt)) < eleIdValueMIN ) continue;
      
      electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      leptons.push_back( reader.Get4V("electrons")->at(eleIt) );      
      leptonFlavours.push_back("electron");
      leptonTkIso.push_back(reader.GetFloat("electrons_tkIso")->at(eleIt));
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < 10. ) continue;
      if( (reader.GetFloat("muons_tkIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.5 ) continue;
      
      muons.push_back( reader.Get4V("muons")->at(muIt) );
      leptons.push_back( reader.Get4V("muons")->at(muIt) );      
      leptonFlavours.push_back("muon");
      leptonTkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
    }
    
    if( (int)(leptons.size()) < lepNMIN ) continue;
    
    
    // select lepton
    int selectIt_lep = SelectLepton(leptons, "maxPt", 10.);    
    ROOT::Math::XYZTVector lepton = leptons.at(selectIt_lep);
    if(selectIt_lep == -1) continue;
    
    
    
    ROOT::Math::XYZTVector met = reader.Get4V("met")->at(0);
    ROOT::Math::XYZTVector lepMet = lepton + met;
    ROOT::Math::XYZTVector lepMetW = lepMet + jet12_W;
    ROOT::Math::XYZTVector lepMetWTag = lepMetW + jet12_tag;
    
    
    
    
    
    
    //**********************
    // STEP 1 - Preselection
    step = 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "preselection";
    
        
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    
    
    
    
    
    //*********************************
    // STEP 2 - Initial cuts - tag jets
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "tag jet cuts";
    
    if( deltaEta(jet1_tag.eta(), jet2_tag.eta()) < tagJetDetaMIN ) continue;
    if( jet12_tag.mass() < tagJetMjjMIN ) continue;
    if( jet1_tag.Et() < tagJet1EtMIN ) continue;
    if( jet1_tag.Et() > tagJet1EtMAX ) continue;    
    if( jet2_tag.Et() < tagJet2EtMIN ) continue;
    if( jet2_tag.Et() > tagJet2EtMAX ) continue;
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    
    
    
    
    
    //*******************************
    // STEP 3 - Initial cuts - W jets
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "W jet cuts";
    
    if( jet12_W.mass() < WJetMjjMIN ) continue;
    if( jet12_W.mass() > WJetMjjMAX ) continue;
    if( jet1_W.Et() < WJet1EtMIN ) continue;
    if( jet1_W.Et() > WJet1EtMAX ) continue;
    if( jet2_W.Et() < WJet2EtMIN ) continue;
    if( jet2_W.Et() > WJet2EtMAX ) continue;
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    
    
    
    
    
    //*******************************
    // STEP 4 - Initial cuts - lepton
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "lepton cuts";
    
    if( lepton.pt() < lepPtMIN ) continue;
    if( lepton.pt() > lepPtMAX ) continue;
    if( leptonTkIso.at(selectIt_lep) / lepton.pt() > lepTkIsoOverPtMAX) continue;
    if( fabs((lepton.eta() - avgEta_tag)/absDeta_tag) > lepZeppMAX ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    
    
    
    
    
    //*******************************
    // STEP 5 - Initial cuts - met
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "met cuts";
    
    
    if( met.Et() < metEtMIN ) continue;
    if( met.Et() > metEtMAX ) continue;
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    
    
    
    
    
    //***************************************
    // STEP 6 - Initial cuts - lepMet - W cut
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "lepMet-W Dphi";
    
    if( deltaPhi(lepMet.phi(), jet12_W.phi()) > lepMet_W_DphiMAX ) continue;
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    
    
    
    
    
    //***************************************
    // STEP 7 - Initial cuts - CJV
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "CJV";
    
    if( centralJets.size() > 0 ) continue;
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step);
    stdHistograms -> Fill1(otherJets, "otherJ", step);
    stdHistograms -> Fill1(centralJets, "centralJ", step);
    stdHistograms -> Fill1(met, "met", step);
    stdHistograms -> Fill1(lepton, "lep", step);
    stdHistograms -> Fill2(lepton, met, "lepMet", step);
    stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step);
    
    
    
    
    //// tag jets
    //float etJ1_tag = jet1_tag.Et();
    //histograms -> Fill("tag_etJ1", step, etJ1_tag);
    //
    //float etJ2_tag = jet2_tag.Et();
    //histograms -> Fill("tag_etJ2", step, etJ2_tag);
    //
    //float etJJ_tag = jet12_tag.Et();
    //histograms -> Fill("tag_etJJ", step, etJJ_tag);
    //    
    //float mJJ_tag = jet12_tag.mass();
    //histograms -> Fill("tag_mJJ", step, mJJ_tag);
    //
    //float DetaJJ_tag = deltaEta(jet1_tag.eta(), jet2_tag.eta());
    //histograms -> Fill("tag_DetaJJ", step, DetaJJ_tag);
    //
    //float DphiJJ_tag = deltaPhi(jet1_tag.phi(), jet2_tag.phi());
    //histograms -> Fill("tag_DphiJJ", step, DphiJJ_tag);    
    //
    //
    //
    //// W jets
    //float etJ1_W = jet1_W.Et();
    //histograms -> Fill("W_etJ1", step, etJ1_W);
    //    
    //float etJ2_W = jet2_W.Et();
    //histograms -> Fill("W_etJ2", step, etJ2_W);
    //
    //float etJJ_W = jet12_W.Et();
    //histograms -> Fill("W_etJJ", step, etJJ_W);
    //    
    //float mJJ_W = jet12_W.mass();
    //histograms -> Fill("W_mJJ", step, mJJ_W);
    //    
    //float mtJJ_W = jet12_W.mt();
    //histograms -> Fill("W_mtJJ", step, mtJJ_W);
    //
    //float DetaJJ_W = deltaEta(jet1_W.eta(), jet2_W.eta());
    //histograms -> Fill("W_DetaJJ", step, DetaJJ_W);
    //
    //float DphiJJ_W = deltaPhi(jet1_W.phi(), jet2_W.phi());
    //histograms -> Fill("W_DphiJJ", step, DphiJJ_W);
    //
    //float zeppJ1_W = (jet1_W.eta() - avgEta_tag) / absDeta_tag;
    //histograms -> Fill("W_zeppJ1", step, zeppJ1_W);
    //    
    //float zeppJ2_W = (jet2_W.eta() - avgEta_tag) / absDeta_tag;
    //histograms -> Fill("W_zeppJ2", step, zeppJ2_W);
    //
    //float zeppJJ_W = (jet12_W.eta() - avgEta_tag) / absDeta_tag;
    //histograms -> Fill("W_zeppJJ", step, zeppJJ_W);
    //
    //float Dphi_W_tag = deltaPhi(jet12_W.phi(), jet12_tag.phi());
    //histograms -> Fill("W_tag_Dphi", step, Dphi_W_tag);    
    //
    //// other jets
    //unsigned int nOtherJet = otherJets.size();
    //histograms -> Fill("otherJet_n", step, nOtherJet);
    //
    //for(unsigned int jetIt = 0; jetIt < nOtherJet; ++jetIt)
    //{
    //  float etOtherJet = otherJets.at(jetIt).Et();
    //  histograms -> Fill("otherJet_et", step, etOtherJet);
    //
    //  float zeppOtherJet = (otherJets.at(jetIt).eta() - avgEta_tag)/absDeta_tag;
    //  histograms -> Fill("otherJet_zepp", step, zeppOtherJet);      
    //}
    //
    //// central jets
    //unsigned int nCentralJet = centralJets.size();
    //histograms -> Fill("centralJet_n", step, nCentralJet);
    //
    //for(unsigned int jetIt = 0; jetIt < nCentralJet; ++jetIt)
    //{
    //  float etCentralJet = centralJets.at(jetIt).Et();
    //  histograms -> Fill("centralJet_et", step, etCentralJet);
    //
    //  float zeppCentralJet = (centralJets.at(jetIt).eta() - avgEta_tag)/absDeta_tag;
    //  histograms -> Fill("centralJet_zepp", step, zeppCentralJet);      
    //}
    //
    //// lepton
    //float ptLep = lepton.pt();
    //histograms -> Fill("lep_pt", step, ptLep);
    //
    //float zeppLep = (lepton.eta() - avgEta_tag) / absDeta_tag;
    //histograms -> Fill("lep_zepp", step, zeppLep);
    //    
    //float DetaLep_W = deltaEta(lepton.eta(), jet12_W.eta());
    //histograms -> Fill("lep_W_Deta", step, DetaLep_W); 
    //
    //float DphiLep_W = deltaPhi(lepton.phi(), jet12_W.phi());
    //histograms -> Fill("lep_W_Dphi", step, DphiLep_W);
    //
    //float DphiLep_tag = deltaPhi(lepton.phi(), jet12_tag.phi());
    //histograms -> Fill("lep_tag_Dphi", step, DphiLep_tag);
    //
    //float mtLepMet = lepMet.mt();
    //histograms -> Fill("lepMet_mt", step, mtLepMet);
    //    
    //float DphiLepMet_W = deltaPhi(lepMet.phi(), jet12_W.phi());
    //histograms -> Fill("lepMet_W_Dphi", step, DphiLepMet_W);
    //
    //
    //
    //// met
    //float etMet = met.Et();
    //histograms -> Fill("met_et", step, etMet);
    //
    //float DphiMet_lep = deltaPhi(met.phi(), lepton.phi());
    //histograms -> Fill("met_lep_Dphi", step, DphiMet_lep);
    //
    //float DphiMet_W = deltaPhi(met.phi(), jet12_W.phi());
    //histograms -> Fill("met_W_Dphi", step, DphiMet_W);
    //
    //float DphiMet_tag = deltaPhi(met.phi(), jet12_tag.phi());
    //histograms -> Fill("met_tag_Dphi", step, DphiMet_tag);    
    //
    //
    //
    //// higgs
    //float ptLepMetW = lepMetW.pt();
    //histograms -> Fill("lepMetW_pt", step, ptLepMetW);
    //    
    //float mtLepMetW = lepMetW.mt();
    //histograms -> Fill("lepMetW_mt", step, mtLepMetW);
    //
    //float DphiLepMetW_tag = deltaPhi(lepMetW.phi(), jet12_tag.phi());
    //histograms -> Fill("lepMetW_tag_Dphi", step, DphiLepMetW_tag);    
    //
    //
    //
    //// pt tot
    //float ptLepMetWTag = lepMetWTag.pt();
    //histograms -> Fill("lepMetWTag_pt", step, ptLepMetWTag);
    
    
    
    
    
    
  } // loop over the events
  
  
  
  
  
  
  // Open output root file
  TFile* outputRootFile = new TFile(outputRootFullFileName.c_str(), "recreate");
  outputRootFile -> cd();
  
  
  
  for(step = 0; step < nStep; ++step)
  {
    events -> SetBinContent(step+1, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step+1, stepName[step].c_str());
  }
  
  events -> Write(); 
  outputRootFile -> Close();
  
  delete stdHistograms;
  //delete histograms;
  
  return 0;
}
