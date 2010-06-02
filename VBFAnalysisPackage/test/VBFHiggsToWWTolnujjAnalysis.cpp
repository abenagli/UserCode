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
    std::cerr << ">>>>> VBFHiggsToWWTolnujjAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
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
  
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  float mH = gConfigParser -> readFloatOption("Options::mH");
  float crossSection = gConfigParser -> readFloatOption("Options::crossSection");
  bool TMVA4jSelection = gConfigParser -> readBoolOption("Options::TMVA4jSelection"); 
  bool TMVA4jTraining = gConfigParser -> readBoolOption("Options::TMVA4jTraining");
  
  int jetNMIN = gConfigParser -> readIntOption("Cuts::jetNMIN");
  int jetNMAX = gConfigParser -> readIntOption("Cuts::jetNMAX");
  float jetEtMIN = gConfigParser -> readFloatOption("Cuts::jetEtMIN");
  float jetEtMIN1 = gConfigParser -> readFloatOption("Cuts::jetEtMIN1");
  float jetEtMIN2 = gConfigParser -> readFloatOption("Cuts::jetEtMIN2");
  float jetEtMIN3 = gConfigParser -> readFloatOption("Cuts::jetEtMIN3");
  float jetEtMIN4 = gConfigParser -> readFloatOption("Cuts::jetEtMIN4");
  
  int lepNMIN = gConfigParser -> readIntOption("Cuts::lepNMIN");
  float lepPtMIN = gConfigParser -> readFloatOption("Cuts::lepPtMIN");
  float lepPtMAX = gConfigParser -> readFloatOption("Cuts::lepPtMAX");
  float lepTkIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::lepTkIsoOverPtMAX");
  float eleIdValueMIN = gConfigParser -> readFloatOption("Cuts::eleIdValueMIN");
  float lepZeppMAX = gConfigParser -> readFloatOption("Cuts::lepZeppMAX");
  float lepEtaGapMIN = gConfigParser -> readFloatOption("Cuts::lepEtaGapMIN");
  float lepAbsEtaMAX = gConfigParser -> readFloatOption("Cuts::lepAbsEtaMAX");
  float lepTipSigMAX = gConfigParser -> readFloatOption("Cuts::lepTipSigMAX");
  float lepMetMtMIN = gConfigParser -> readFloatOption("Cuts::lepMetMtMIN");
  float lepMetMtMAX = gConfigParser -> readFloatOption("Cuts::lepMetMtMAX");
  
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
  float WJetZeppMAX = gConfigParser -> readFloatOption("Cuts::WJetZeppMAX");
  float WJetDRMAX = gConfigParser -> readFloatOption("Cuts::WJetDRMAX");
  
  float metEtMIN = gConfigParser -> readFloatOption("Cuts::metEtMIN");
  float metEtMAX = gConfigParser -> readFloatOption("Cuts::metEtMAX");
  
  float ptMaxMIN = gConfigParser -> readFloatOption("Cuts::ptMaxMIN");

  float tagWJetEt1MIN = gConfigParser -> readFloatOption("Cuts::tagWJetEt1MIN");
  float tagWJetEt2MIN = gConfigParser -> readFloatOption("Cuts::tagWJetEt2MIN");
  float tagWJetEt3MIN = gConfigParser -> readFloatOption("Cuts::tagWJetEt3MIN");
  float tagWJetEt4MIN = gConfigParser -> readFloatOption("Cuts::tagWJetEt4MIN");
  
  float lepMet_W_DphiMAX = gConfigParser -> readFloatOption("Cuts::lepMetWDphiMAX");
  float lepMetWMtMIN = gConfigParser -> readFloatOption("Cuts::lepMetWMtMIN");
  float lepMetWMtMAX = gConfigParser -> readFloatOption("Cuts::lepMetWMtMAX");
  
  std::map<int, int> totalEvents = GetTotalEvents("events", inputFileList.c_str()); 
  
  
  
  
  
  
  // Open tree
  std::string treeName = "SimpleNtple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain));  
  
  
  
  // define event histogram
  int nStep = 13;
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  TProfile* purity_tag = new TProfile("purity_tag", "purity_tag", 3, 0., 3.);
  TProfile* purity_W = new TProfile("purity_W", "purity_W", 3, 0., 3.);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepName;
  
  
  
  int step = 0;
  stepEvents[step] = totalEvents[1];
  stepName[step] = "0) total events";
  
  step = 1;
  stepEvents[step] = totalEvents[2];
  stepName[step] = "1) Preselection >= 1 lepton";
  
  
  
  
  
  
  // define out file names
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+"_"+jetAlgorithm+".root";
  std::string outputTreeFileName = outputRootFilePath+"/tree.root";
  
  // histograms
  TH2F* controlRegion_lepMetW_Dphi_vs_tagJJ_Deta = new TH2F("controlRegion_lepMetW_Dphi_vs_tagJJ_Deta", "", 200, 0., 10., 100, 0., 5.);
  
  
  hFactory* histograms = new hFactory(outputRootFullFileName);
  h2Factory* histograms2 = new h2Factory(outputRootFullFileName);

  histograms -> add_h1("tagJJ_etaProd",  "",  2000, -25., 25., nStep);  
  histograms -> add_h1("tagJJ_bTag",  "",  2000, -100., 100., nStep);
  histograms -> add_h1("WJJ_bTag",     "", 2000,  -100., 100., nStep);
  histograms -> add_h1("tagWJ_et1", "",  3000, 0.,  3000., nStep);   
  histograms -> add_h1("tagWJ_et2", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("tagWJ_et3", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("tagWJ_et4", "",  3000, 0.,  3000., nStep);

  histograms -> add_h1("WJJ_matched_right_m", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("WJJ_matched_wrong_m", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("WJJ_nonMatched_m", "",  3000, 0.,  3000., nStep);

  histograms -> add_h1("ele_tkIso",  "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("ele_emIso",  "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("ele_hadIso", "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("mu_tkIso",   "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("mu_emIso",   "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("mu_hadIso",  "", 1000, 0., 0.5, nStep);
  histograms -> add_h1("lep_lipSig",  "", 2000, -50., 50., nStep);
  histograms -> add_h1("lep_tipSig",  "", 1000,   0., 50., nStep);  
  histograms -> add_h1("lep_3DipSig", "", 2000, -50., 50., nStep);
  
  histograms -> add_h1("lepMet_mt",  "", 3000, 0., 3000., nStep);
  histograms -> add_h1("lepMetW_mt", "", 3000, 0., 3000., nStep);  
  
  histograms -> add_h1("lepWJ_pt1", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("lepWJ_pt2", "",  3000, 0.,  3000., nStep);
  histograms -> add_h1("lepWJ_pt3", "",  3000, 0.,  3000., nStep);
  
  histograms2 -> add_h2("tagWJJ_m_vs_Deta", "", 200,  0., 10., 3000, 0., 3000., nStep);
  histograms2 -> add_h2("tagWJJ_matched_right_m_vs_Deta", "", 200,  0., 10., 3000, 0., 3000., nStep);
  histograms2 -> add_h2("tagWJJ_matched_wrong_m_vs_Deta", "", 200,  0., 10., 3000, 0., 3000., nStep);
  histograms2 -> add_h2("tagWJJ_matched_all_m_vs_Deta", "", 200,  0., 10., 3000, 0., 3000., nStep);
  histograms2 -> add_h2("lepWJ_pt1_vs_tagJJ_pt", "", 500, 0., 500., 500, 0., 500., nStep);
  
  histograms -> add_h1("TMVA_kBDT",  "",  1000, -1., 1., nStep);
  
  // stdHistograms
  stdHisto* stdHistograms = new stdHisto(nStep, outputRootFullFileName);
  
  stdHistograms -> Add2("tagJJ",    nStep);
  stdHistograms -> Add2("WJJ",      nStep, true);
  stdHistograms -> Add1("allJ",   nStep);
  stdHistograms -> Add1("met",      nStep);
  stdHistograms -> Add1("lep",      nStep, true);
  stdHistograms -> Add2("lepMet",   nStep, true);
  stdHistograms -> Add2("lepMetW",  nStep, true);
  stdHistograms -> Add1("otherJ_et15", nStep, true);
  stdHistograms -> Add1("otherJ_et20", nStep, true);
  stdHistograms -> Add1("otherJ_et25", nStep, true);
  stdHistograms -> Add1("otherJ_et30", nStep, true);
  stdHistograms -> Add1("centralJ_et15", nStep, true);
  stdHistograms -> Add1("centralJ_et20", nStep, true);
  stdHistograms -> Add1("centralJ_et25", nStep, true);
  stdHistograms -> Add1("centralJ_et30", nStep, true);
  
  
  
  
  
  
  // Define tree variables
  int type = -1;
  float tagJJ_Deta;
  float tagJJ_etaProd;
  float tagJJ_m;
  float tagJJ_max_e;
  float tagJJ_min_e;
  float tagJJ_max_et;
  float tagJJ_min_et;
  float tagJJ_max_eta;
  float tagJJ_min_eta;
  float tagJ1_bTag;
  float tagJ2_bTag;
  float WJJ_pt;
  float WJJ_eta;
  float WJJ_zepp;
  float WJJ_Deta;
  float WJJ_Dphi;
  float WJJ_DR;
  float WJJ_etaProd;
  float WJJ_m;
  float WJJ_max_et;
  float WJJ_min_et;
  float WJJ_max_eta;
  float WJJ_min_eta;
  float WJJ_max_zepp;
  float WJJ_min_zepp;
  float WJ1_bTag;
  float WJ2_bTag;
  int lep_n;
  float lep_pt;
  float lep_eta;
  float lep_zepp;
  float lep_eleId;
  float lep_tkIso;
  float lep_emIso;
  float lep_hadIso;
  float lep_tipSig = -1.;
  float lep_lipSig = -1.;
  float lep_3DipSig = -1.;
  float lepWJJ_pt1;
  float lepWJJ_pt2;
  float lepWJJ_pt3;
  float met_et;
  float lepMet_mt;
  float lepMetW_mt;
  float lepMetW_Dphi;
  int nOtherJets_et15;
  int nOtherJets_et20;
  int nOtherJets_et25;
  int nOtherJets_et30;
  int nCentralJets_et15;
  int nCentralJets_et20;
  int nCentralJets_et25;
  int nCentralJets_et30;
  
  // Define tree
  std::vector<TTree*> tree;
  std::vector<TTree*> treeEvents;
  
  for(int i = 0; i < nStep; ++i)
  { 
    char treeName[50];
    sprintf(treeName, "tree_%d", i);
    tree.push_back(new TTree(treeName, treeName));
    tree.at(i) -> SetDirectory(0);
    
    tree.at(i) -> Branch("mH",                &mH,                               "mH/F");
    tree.at(i) -> Branch("totEvents",         &stepEvents[0],             "totEvents/I");
    tree.at(i) -> Branch("crossSection",      &crossSection,           "crossSection/F");
    tree.at(i) -> Branch("type",              &type,                           "type/I");
    tree.at(i) -> Branch("tagJJ_Deta",        &tagJJ_Deta,               "tagJJ_Deta/F");
    tree.at(i) -> Branch("tagJJ_etaProd",     &tagJJ_etaProd,         "tagJJ_etaProd/F");
    tree.at(i) -> Branch("tagJJ_m",           &tagJJ_m,                     "tagJJ_m/F");  
    tree.at(i) -> Branch("tagJJ_max_e",       &tagJJ_max_e,             "tagJJ_max_e/F");   
    tree.at(i) -> Branch("tagJJ_min_e",       &tagJJ_min_e,             "tagJJ_min_e/F");
    tree.at(i) -> Branch("tagJJ_max_et",      &tagJJ_max_et,           "tagJJ_max_et/F");   
    tree.at(i) -> Branch("tagJJ_min_et",      &tagJJ_min_et,           "tagJJ_min_et/F"); 
    tree.at(i) -> Branch("tagJJ_max_eta",     &tagJJ_max_eta,         "tagJJ_max_eta/F");   
    tree.at(i) -> Branch("tagJJ_min_eta",     &tagJJ_min_eta,         "tagJJ_min_eta/F");
    tree.at(i) -> Branch("tagJ1_bTag",        &tagJ1_bTag,               "tagJ1_bTag/F");
    tree.at(i) -> Branch("tagJ2_bTag",        &tagJ2_bTag,               "tagJ2_bTag/F");
    tree.at(i) -> Branch("WJJ_pt",            &WJJ_pt,                       "WJJ_pt/F");
    tree.at(i) -> Branch("WJJ_eta",           &WJJ_eta,                     "WJJ_eta/F");
    tree.at(i) -> Branch("WJJ_zepp",          &WJJ_zepp,                   "WJJ_zepp/F");
    tree.at(i) -> Branch("WJJ_Deta",          &WJJ_Deta,                   "WJJ_Deta/F");
    tree.at(i) -> Branch("WJJ_Dphi",          &WJJ_Dphi,                   "WJJ_Dphi/F");
    tree.at(i) -> Branch("WJJ_DR",            &WJJ_DR,                       "WJJ_DR/F");
    tree.at(i) -> Branch("WJJ_etaProd",       &WJJ_etaProd,             "WJJ_etaProd/F");
    tree.at(i) -> Branch("WJJ_m",             &WJJ_m,                         "WJJ_m/F");  
    tree.at(i) -> Branch("WJJ_max_et",        &WJJ_max_et,               "WJJ_max_et/F");   
    tree.at(i) -> Branch("WJJ_min_et",        &WJJ_min_et,               "WJJ_min_et/F");
    tree.at(i) -> Branch("WJJ_max_eta",       &WJJ_max_eta,             "WJJ_max_eta/F");   
    tree.at(i) -> Branch("WJJ_min_eta",       &WJJ_min_eta,             "WJJ_min_eta/F");
    tree.at(i) -> Branch("WJJ_max_zepp",      &WJJ_max_zepp,           "WJJ_max_zepp/F");   
    tree.at(i) -> Branch("WJJ_min_zepp",      &WJJ_min_zepp,           "WJJ_min_zepp/F");
    tree.at(i) -> Branch("WJ1_bTag",          &WJ1_bTag,                   "WJ1_bTag/F");
    tree.at(i) -> Branch("WJ2_bTag",          &WJ2_bTag,                   "WJ2_bTag/F");
    tree.at(i) -> Branch("lep_n",             &lep_n,                         "lep_n/I");
    tree.at(i) -> Branch("lep_pt",            &lep_pt,                       "lep_pt/F");
    tree.at(i) -> Branch("lep_eta",           &lep_eta,                     "lep_eta/F");
    tree.at(i) -> Branch("lep_zepp",          &lep_zepp,                   "lep_zepp/F");
    tree.at(i) -> Branch("lep_eleId",         &lep_eleId,                 "lep_eleId/F");
    tree.at(i) -> Branch("lep_tkIso",         &lep_tkIso,                 "lep_tkIso/F");
    tree.at(i) -> Branch("lep_emIso",         &lep_emIso,                 "lep_emIso/F");
    tree.at(i) -> Branch("lep_hadIso",        &lep_hadIso,               "lep_hadIso/F");
    tree.at(i) -> Branch("lep_tipSig",        &lep_tipSig,               "lep_tipSig/F");
    tree.at(i) -> Branch("lep_lipSig",        &lep_lipSig,               "lep_lipSig/F");
    tree.at(i) -> Branch("lep_3DipSig",       &lep_3DipSig,             "lep_3DipSig/F");
    tree.at(i) -> Branch("lepWJJ_pt1",        &lepWJJ_pt1,               "lepWJJ_pt1/F");
    tree.at(i) -> Branch("lepWJJ_pt2",        &lepWJJ_pt2,               "lepWJJ_pt2/F");
    tree.at(i) -> Branch("lepWJJ_pt3",        &lepWJJ_pt3,               "lepWJJ_pt3/F");
    tree.at(i) -> Branch("met_et",            &met_et,                       "met_et/F");
    tree.at(i) -> Branch("lepMet_mt",         &lepMet_mt,                 "lepMet_mt/F");
    tree.at(i) -> Branch("lepMetW_mt",        &lepMetW_mt,               "lepMetW_mt/F");
    tree.at(i) -> Branch("lepMetW_Dphi",      &lepMetW_Dphi,           "lepMetW_Dphi/F");
    tree.at(i) -> Branch("nOtherJets_et15",   &nOtherJets_et15,     "nOtherJets_et15/I");
    tree.at(i) -> Branch("nOtherJets_et20",   &nOtherJets_et20,     "nOtherJets_et20/I");
    tree.at(i) -> Branch("nOtherJets_et25",   &nOtherJets_et25,     "nOtherJets_et25/I");
    tree.at(i) -> Branch("nOtherJets_et30",   &nOtherJets_et30,     "nOtherJets_et30/I");        
    tree.at(i) -> Branch("nCentralJets_et15", &nCentralJets_et15, "nCentralJets_et15/I");
    tree.at(i) -> Branch("nCentralJets_et20", &nCentralJets_et20, "nCentralJets_et20/I");
    tree.at(i) -> Branch("nCentralJets_et25", &nCentralJets_et25, "nCentralJets_et25/I");
    tree.at(i) -> Branch("nCentralJets_et30", &nCentralJets_et30, "nCentralJets_et30/I");
    
    
    
    char treeEventsName[50];
    sprintf(treeEventsName, "treeEvents_%d", i);
    treeEvents.push_back(new TTree(treeEventsName, treeEventsName));
    
    treeEvents.at(i) -> Branch("mH",           &mH,                     "mH/F");
    treeEvents.at(i) -> Branch("totEvents",    &stepEvents[0],   "totEvents/I");
    treeEvents.at(i) -> Branch("crossSection", &crossSection, "crossSection/F");
    treeEvents.at(i) -> Branch("events",       &stepEvents[i],      "events/I");
  }
  
  
  
  
  
  
  
  
  
  
  
  
  // Select jets with TMVA
  TMVA::Reader* TMVAReader = new TMVA::Reader();
  
  if(TMVA4jSelection == true)
  {
    TMVAReader -> AddVariable("tagJJ_Deta",    &tagJJ_Deta);
    TMVAReader -> AddVariable("tagJJ_max_eta", &tagJJ_max_eta);  
    TMVAReader -> AddVariable("tagJJ_min_eta", &tagJJ_min_eta);    
    TMVAReader -> AddVariable("tagJJ_etaProd", &tagJJ_etaProd);
    TMVAReader -> AddVariable("tagJJ_m",       &tagJJ_m);  
    TMVAReader -> AddVariable("tagJJ_max_et",  &tagJJ_max_et);
    TMVAReader -> AddVariable("tagJJ_min_et",  &tagJJ_min_et);
    TMVAReader -> AddVariable("WJJ_Deta",    &WJJ_Deta);
    TMVAReader -> AddVariable("WJJ_max_eta", &WJJ_max_eta);  
    TMVAReader -> AddVariable("WJJ_min_eta", &WJJ_min_eta);    
    TMVAReader -> AddVariable("WJJ_etaProd", &WJJ_etaProd);
    TMVAReader -> AddVariable("WJJ_m",       &WJJ_m);  
    TMVAReader -> AddVariable("WJJ_max_et",  &WJJ_max_et);
    TMVAReader -> AddVariable("WJJ_min_et",  &WJJ_min_et);
    
    TMVAReader -> BookMVA("kBDT", "../VBF4jSelection_TMVA/weights/example_kBDT.weights.xml");
  }
  
  
  
  
  
  
  
  
  
  
  
  
  //*********************
  // LOOP OVER THE EVENTS
  
  std::cout << ">>>>> VBFHiggsToWWTolnujjAnalysis::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    reader.GetEntry(entry);
    if((entry%entryMODULO) == 0) std::cout << ">>>>> VBFHiggsToWWTolnujjAnalysis::GetEntry " << entry << std::endl;   
    if(entry == entryMAX) break;
    
    
    
    
    
    
    //**************
    // DUMP MC TRUTH
    //std::cout << ">>> Dump MC truth" << std::endl;    
    
    ROOT::Math::XYZTVector mcH;
    ROOT::Math::XYZTVector mcW1;
    ROOT::Math::XYZTVector mcW2;
    ROOT::Math::XYZTVector mcQ1_tag;
    ROOT::Math::XYZTVector mcQ2_tag;
    ROOT::Math::XYZTVector mcQ1_W;
    ROOT::Math::XYZTVector mcQ2_W;
    ROOT::Math::XYZTVector mcLepton;
    //ROOT::Math::XYZTVector mcMet;
    std::vector<ROOT::Math::XYZTVector> mcQuarks;
    std::vector<ROOT::Math::XYZTVector> mcQuarks_tag;
    std::vector<ROOT::Math::XYZTVector> mcQuarks_W;
    
    if(mH > 0)
    {
      mcH = reader.Get4V("mc_H")->at(0);
      mcW1 = reader.Get4V("mcV1")->at(0);
      mcW2 = reader.Get4V("mcV2")->at(0);
      mcQ1_tag = (reader.Get4V("mcQ1_tag"))->at(0);
      mcQ2_tag = (reader.Get4V("mcQ2_tag"))->at(0);
      mcQ1_W   = (reader.Get4V("mcF1_fromV2"))->at(0);
      mcQ2_W   = (reader.Get4V("mcF2_fromV2"))->at(0);
      mcLepton = (reader.Get4V("mcF1_fromV1"))->at(0);
      if( (fabs(reader.GetFloat("mcF1_fromV1_pdgId")->at(0)) == 12) ||
          (fabs(reader.GetFloat("mcF1_fromV1_pdgId")->at(0)) == 14) ||
          (fabs(reader.GetFloat("mcF1_fromV1_pdgId")->at(0)) == 16) )
      mcLepton = (reader.Get4V("mcF2_fromV1"))->at(0);
      //mcMet = (reader.Get4V("genMet"))->at(0);
      
      mcQuarks.push_back( mcQ1_tag );
      mcQuarks.push_back( mcQ2_tag );    
      mcQuarks.push_back( mcQ1_W );
      mcQuarks.push_back( mcQ2_W );
      
      mcQuarks_tag.push_back( mcQ1_tag );
      mcQuarks_tag.push_back( mcQ2_tag );    
      mcQuarks_W.push_back( mcQ1_W );
      mcQuarks_W.push_back( mcQ2_W );
    }
    
    
    
    
    
    
    //************
    // SELECT JETS
    //std::cout << ">>> Select Jets" << std::endl;
    
    std::vector<ROOT::Math::XYZTVector> jets;
    std::vector<ROOT::Math::XYZTVector> electrons_jetCleaning;
    std::vector<ROOT::Math::XYZTVector> muons_jetCleaning;
    std::vector<float> jets_bTag;
    
    // build the collection of leptons for jet cleaning
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.5 ) continue;
      if( (reader.GetFloat("electrons_IdRobustLoose")->at(eleIt)) < 1. ) continue;
      
      electrons_jetCleaning.push_back( reader.Get4V("electrons")->at(eleIt) );
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < 10. ) continue;
      if( (reader.GetFloat("muons_tkIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.5 ) continue;
      
      muons_jetCleaning.push_back( reader.Get4V("muons")->at(muIt) );
    }
    
    // jets
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      if( jet.Et() < jetEtMIN ) continue;
      
      // clean jets
      bool skipJet = false;
      for(unsigned int eleIt = 0; eleIt < electrons_jetCleaning.size(); ++eleIt)
      {
        ROOT::Math::XYZTVector ele = electrons_jetCleaning.at(eleIt);
        if( deltaR(jet.eta(), jet.phi(), ele.eta(), ele.phi()) < 0.3 )
          skipJet = true;
      }
      for(unsigned int muIt = 0; muIt < muons_jetCleaning.size(); ++muIt)
      {
        ROOT::Math::XYZTVector mu = muons_jetCleaning.at(muIt);
        if( deltaR(jet.eta(), jet.phi(), mu.eta(), mu.phi()) < 0.3 )
          skipJet = true;
      }
      
      if(skipJet == true) continue;
      
      jets.push_back( reader.Get4V("jets")->at(jetIt) );
      jets_bTag.push_back( reader.GetFloat("jets_trackCountingHighEffBJetTags")->at(jetIt) );
    }
    
    if( (int)(jets.size()) < jetNMIN ) continue;
    if( (int)(jets.size()) > jetNMAX ) continue;
    
    
    
    
    
    
    //**************
    // MATCH WITH MC
    //std::cout << ">>> Match with MC" << std::endl;    
    
    std::vector<int> matchIt;    
    std::vector<int> matchIt_tag;
    std::vector<int> matchIt_W;
    int nMatching = -1;
    int nMatching_tag = -1;
    int nMatching_W = -1;
    
    if(mH > 0)
    {
      nMatching = GetMatching(jets, mcQuarks, 0.3, 0.5, 1.5, &matchIt);
      nMatching_tag = GetMatching(jets, mcQuarks_tag, 0.3, 0.5, 1.5, &matchIt_tag);
      nMatching_W = GetMatching(jets, mcQuarks_W, 0.3, 0.01, 100., &matchIt_W);
      
      if( (nMatching == 4) && (matchIt.at(0) > matchIt.at(1)) )
      {
        float dummy = matchIt.at(0);
        matchIt.at(0) = matchIt.at(1);
        matchIt.at(1) = dummy;
      }
      
      if( (nMatching == 4) && (matchIt.at(2) > matchIt.at(3)) )
      {
        float dummy = matchIt.at(2);
        matchIt.at(2) = matchIt.at(3);
        matchIt.at(3) = dummy;
      }
      
      std::sort(matchIt_tag.begin(), matchIt_tag.end());
      std::sort(matchIt_W.begin(), matchIt_W.end());
    }
    
    
    
    
    
    //************************
    // STEP 2 - Preselection
    step = 2;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "2) Preselection >= 4 jets";      
    
    std::vector<float> jets_et;
    for(unsigned int jetIt = 0; jetIt < jets.size(); ++jetIt)
      jets_et.push_back(jets.at(jetIt).Et());
    
    std::sort(jets_et.begin(), jets_et.end(), maggiore);
    
    if(jets_et.at(0) < jetEtMIN1) continue;
    if(jets_et.at(1) < jetEtMIN2) continue;
    if(jets_et.at(2) < jetEtMIN3) continue;
    if(jets_et.at(3) < jetEtMIN4) continue;
    
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    stdHistograms -> Fill1(jets, "allJ", step);
    
    
    
    
    
    
    //********************
    // STEP 3 - 4j et cuts
    step = 3;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "3) select 4 jets";
    
    
    // select jets
    std::vector<int> selectIt_tag;
    selectIt_tag.push_back(-1);
    selectIt_tag.push_back(-1);
    
    std::vector<int> selectIt_W;
    selectIt_W.push_back(-1);
    selectIt_W.push_back(-1);
    
    
    
    if( (TMVA4jSelection == false) && (TMVA4jTraining == false) )
    {    
      SelectTagJets(selectIt_tag, jets, jetEtMIN, 2., 200.);
      std::sort(selectIt_tag.begin(), selectIt_tag.end());
      
      std::vector<int> blacklistIt_tag;
      blacklistIt_tag.push_back(selectIt_tag.at(0));
      blacklistIt_tag.push_back(selectIt_tag.at(1));
      SelectWJets(selectIt_W, jets, jetEtMIN, 2., 200., &blacklistIt_tag);
      std::sort(selectIt_W.begin(), selectIt_W.end());    
    }  
    
    
    
    double bestMvaValue = -999999;
    if( (TMVA4jSelection == true) && (TMVA4jTraining == false) )
    {
      std::vector<std::vector<int> > combinations;
      int nCombinations = Build4JetCombinations(combinations, jets.size());
      
      std::vector<int> bestCombination;

      for(int combIt = 0; combIt < nCombinations; ++combIt)
      {
        std::vector<int> tempCombination = combinations.at(combIt);
        
        ROOT::Math::XYZTVector j1_tag = jets.at(tempCombination.at(0));
        ROOT::Math::XYZTVector j2_tag = jets.at(tempCombination.at(1));
        ROOT::Math::XYZTVector j1_W = jets.at(tempCombination.at(2));
        ROOT::Math::XYZTVector j2_W = jets.at(tempCombination.at(3));                  
        
        tagJJ_Deta = deltaEta(j1_tag.eta(), j2_tag.eta());
        tagJJ_etaProd = j1_tag.eta()*j2_tag.eta()/fabs(j1_tag.eta()*j2_tag.eta());
        tagJJ_max_eta = std::max(j1_tag.eta(), j2_tag.eta());
        tagJJ_min_eta = std::min(j1_tag.eta(), j2_tag.eta());      
        tagJJ_m = (j1_tag + j2_tag).mass();      
        tagJJ_max_et = std::max(j1_tag.Et(), j2_tag.Et());    
        tagJJ_min_et = std::min(j1_tag.Et(), j2_tag.Et());          
        WJJ_Deta = deltaEta(j1_W.eta(), j2_W.eta());
        WJJ_etaProd = j1_W.eta()*j2_W.eta();
        WJJ_max_eta = std::max(j1_W.eta(), j2_W.eta());
        WJJ_min_eta = std::min(j1_W.eta(), j2_W.eta());            
        WJJ_m = (j1_W + j2_W).mass();
        WJJ_max_et = std::max(j1_W.Et(), j2_W.Et());    
        WJJ_min_et = std::min(j1_W.Et(), j2_W.Et());
        
        double mvaValue = TMVAReader -> EvaluateMVA("kBDT");
        
        if(mvaValue > bestMvaValue)
        {
          bestMvaValue = mvaValue;
          bestCombination = tempCombination;
        }
      }
      
      selectIt_tag.clear();
      selectIt_tag.push_back(bestCombination.at(0));
      selectIt_tag.push_back(bestCombination.at(1));
      
      selectIt_W.clear();
      selectIt_W.push_back(bestCombination.at(2));
      selectIt_W.push_back(bestCombination.at(3));
    }
    
    
    
    if( TMVA4jTraining == true )
    {
      if(nMatching == 4)
      {
        std::vector<std::vector<int> > combinations;
        int nCombinations = Build4JetCombinations(combinations, jets.size());
        
        for(int combIt = 0; combIt < nCombinations; ++combIt)
        {
          std::vector<int> tempCombination = combinations.at(combIt);
          
          ROOT::Math::XYZTVector j1_tag = jets.at(tempCombination.at(0));
          ROOT::Math::XYZTVector j2_tag = jets.at(tempCombination.at(1));
          ROOT::Math::XYZTVector j1_W = jets.at(tempCombination.at(2));
          ROOT::Math::XYZTVector j2_W = jets.at(tempCombination.at(3));                  
          
          tagJJ_Deta = deltaEta(j1_tag.eta(), j2_tag.eta());
          tagJJ_etaProd = j1_tag.eta()*j2_tag.eta()/fabs(j1_tag.eta()*j2_tag.eta());
          tagJJ_max_eta = std::max(j1_tag.eta(), j2_tag.eta());
          tagJJ_min_eta = std::min(j1_tag.eta(), j2_tag.eta());      
          tagJJ_m = (j1_tag + j2_tag).mass();      
          tagJJ_max_et = std::max(j1_tag.Et(), j2_tag.Et());    
          tagJJ_min_et = std::min(j1_tag.Et(), j2_tag.Et());          
          WJJ_Deta = deltaEta(j1_W.eta(), j2_W.eta());
          WJJ_etaProd = j1_W.eta()*j2_W.eta();
          WJJ_max_eta = std::max(j1_W.eta(), j2_W.eta());
          WJJ_min_eta = std::min(j1_W.eta(), j2_W.eta());            
          WJJ_m = (j1_W + j2_W).mass();
          WJJ_max_et = std::max(j1_W.Et(), j2_W.Et());    
          WJJ_min_et = std::min(j1_W.Et(), j2_W.Et());
          if(tempCombination == matchIt) type = 0;
          else type = 1;
          
          tree.at(step) -> Fill();
        }
      }
    }
    
    
    
    //std::cout << "tag: " << selectIt_tag.at(0) << " " << selectIt_tag.at(1) << std::endl;
    //std::cout << "  W: " << selectIt_W.at(0) << " " << selectIt_W.at(1) << std::endl;
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
    
    float eta1_tag = jet1_tag.eta();
    float eta2_tag = jet2_tag.eta();
    
    float etaMin_tag = std::min(jet1_tag.eta(), jet2_tag.eta());
    float etaMax_tag = std::max(jet1_tag.eta(), jet2_tag.eta());
    
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
    
    
    
    std::vector<float> tagWJ_et;
    tagWJ_et.push_back(jet1_tag.Et());
    tagWJ_et.push_back(jet2_tag.Et());
    tagWJ_et.push_back(jet1_W.Et());
    tagWJ_et.push_back(jet2_W.Et());
    sort(tagWJ_et.begin(), tagWJ_et.end(), maggiore);
    
    
    
    //**************
    // match with mc    

    if(mH > 0.)
    {
      if(nMatching == 4)
      {
        // 2 tag matching
        if( ( (matchIt.at(0) == selectIt_tag.at(0)) && 
              (matchIt.at(1) == selectIt_tag.at(1)) ) ||
            ( (matchIt.at(0) == selectIt_tag.at(1)) && 
              (matchIt.at(1) == selectIt_tag.at(0)) ) )
        {
          purity_tag -> Fill(0.5, 0.);
          purity_tag -> Fill(1.5, 0.);
          purity_tag -> Fill(2.5, 1.);
        }
        // 1 tag matching
        else if( (matchIt.at(0) == selectIt_tag.at(0)) ||
                 (matchIt.at(0) == selectIt_tag.at(1)) ||
                 (matchIt.at(1) == selectIt_tag.at(0)) ||
                 (matchIt.at(1) == selectIt_tag.at(1)) )
        
        {
          purity_tag -> Fill(0.5, 0.);
          purity_tag -> Fill(1.5, 1.);
          purity_tag -> Fill(2.5, 0.);
        }
        // 0 tag matching
        else
        {
          purity_tag -> Fill(0.5, 1.);
          purity_tag -> Fill(1.5, 0.);
          purity_tag -> Fill(2.5, 0.);      
        }
        
        
        
        // 2 W matching
        if( ( (matchIt.at(2) == selectIt_W.at(0)) && 
              (matchIt.at(3) == selectIt_W.at(1)) ) || 
            ( (matchIt.at(2) == selectIt_W.at(1)) && 
              (matchIt.at(3) == selectIt_W.at(0)) ) )
        {
          purity_W -> Fill(0.5, 0.);
          purity_W -> Fill(1.5, 0.);
          purity_W -> Fill(2.5, 1.);
        }
        // 1 W matching
        else if( (matchIt.at(2) == selectIt_W.at(0)) || 
                 (matchIt.at(2) == selectIt_W.at(1)) ||
                 (matchIt.at(3) == selectIt_W.at(0)) ||
                 (matchIt.at(3) == selectIt_W.at(1)) )
        {
          purity_W -> Fill(0.5, 0.);
          purity_W -> Fill(1.5, 1.);
          purity_W -> Fill(2.5, 0.);
        }
        // 0 W matching
        else
        {
          purity_W -> Fill(0.5, 1.);
          purity_W -> Fill(1.5, 0.);
          purity_W -> Fill(2.5, 0.);          
        }
      }
    }
    
    
    
    
    
    
    
    //***********************
    // other and central jets
    
    std::vector<ROOT::Math::XYZTVector> otherJets_et15;
    std::vector<ROOT::Math::XYZTVector> otherJets_et20;
    std::vector<ROOT::Math::XYZTVector> otherJets_et25;
    std::vector<ROOT::Math::XYZTVector> otherJets_et30;
    std::vector<ROOT::Math::XYZTVector> centralJets_et15;
    std::vector<ROOT::Math::XYZTVector> centralJets_et20;
    std::vector<ROOT::Math::XYZTVector> centralJets_et25;
    std::vector<ROOT::Math::XYZTVector> centralJets_et30;
    for(unsigned int jetIt = 0; jetIt < jets.size(); ++jetIt)
    {
      if( jetIt == (unsigned int)(selectIt_tag.at(0)) ) continue;
      if( jetIt == (unsigned int)(selectIt_tag.at(1)) ) continue;
      if( jetIt == (unsigned int)(selectIt_W.at(0)) ) continue;
      if( jetIt == (unsigned int)(selectIt_W.at(1)) ) continue;      
      

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
      if( reader.Get4V("electrons")->at(eleIt).pt() < 10. ) continue;
      if( (reader.GetFloat("electrons_tkIso")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.1 ) continue;
      if( (reader.GetFloat("electrons_hadIso03_1")->at(eleIt)) / reader.Get4V("electrons")->at(eleIt).pt() > 0.1 ) continue;
      if( (reader.GetFloat("electrons_IdRobustTight")->at(eleIt)) < eleIdValueMIN ) continue;
      
      electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
      leptons.push_back( reader.Get4V("electrons")->at(eleIt) );      
      leptonCharges.push_back( reader.GetFloat("electrons_charge")->at(eleIt) );
      leptonFlavours.push_back("electron");
      leptons_tkIso.push_back(reader.GetFloat("electrons_tkIso")->at(eleIt));
      leptons_emIso.push_back(reader.GetFloat("electrons_emIso03")->at(eleIt));
      leptons_hadIso.push_back(reader.GetFloat("electrons_hadIso03_1")->at(eleIt));    
      leptons_eleIdRobustTight.push_back(reader.GetFloat("electrons_IdRobustTight")->at(eleIt));
      leptons_lipSig.push_back(reader.GetFloat("electrons_lipSignificance")->at(eleIt));
      leptons_tipSig.push_back(reader.GetFloat("electrons_tipSignificance")->at(eleIt));
      leptons_3DipSig.push_back(reader.GetFloat("electrons_3DipSignificance")->at(eleIt));
    }
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      if( reader.Get4V("muons")->at(muIt).pt() < 10. ) continue;
      if( (reader.GetFloat("muons_emIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.1 ) continue;
      if( (reader.GetFloat("muons_hadIsoR03")->at(muIt)) / reader.Get4V("muons")->at(muIt).pt() > 0.1 ) continue;
      
      muons.push_back( reader.Get4V("muons")->at(muIt) );
      leptons.push_back( reader.Get4V("muons")->at(muIt) );      
      leptonCharges.push_back( reader.GetFloat("muons_charge")->at(muIt) );
      leptonFlavours.push_back("muon");
      leptons_tkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
      leptons_emIso.push_back(reader.GetFloat("muons_emIsoR03")->at(muIt));
      leptons_hadIso.push_back(reader.GetFloat("muons_hadIsoR03")->at(muIt));
      leptons_eleIdRobustTight.push_back(1.);
      leptons_lipSig.push_back(reader.GetFloat("muons_lipSignificance")->at(muIt));
      leptons_tipSig.push_back(reader.GetFloat("muons_tipSignificance")->at(muIt));
      leptons_3DipSig.push_back(reader.GetFloat("muons_3DipSignificance")->at(muIt));
    }
    
    if( (int)(leptons.size()) < lepNMIN ) continue;
    
    
    
    // select lepton
    int selectIt_lep = SelectLepton(leptons, "maxPt", 10.);    
    ROOT::Math::XYZTVector lepton = leptons.at(selectIt_lep);
    if(selectIt_lep == -1) continue;
    
    float lep_etaGap1 = deltaEta(lepton.eta(), jet1_tag.eta());
    float lep_etaGap2 = deltaEta(lepton.eta(), jet2_tag.eta());
    
    
    
    
    ROOT::Math::XYZTVector met;
    if(jetType == "Calo")    
      met = reader.Get4V("met")->at(0);
    if(jetType == "PF")    
      met = reader.Get4V("PFMet")->at(0);
    ROOT::Math::XYZTVector lepMet = lepton + met;
    ROOT::Math::XYZTVector lepMetW = lepMet + jet12_W;
    ROOT::Math::XYZTVector lepW = lepton + jet12_W;
    ROOT::Math::XYZTVector lepMetWTag = lepMetW + jet12_tag;
    
    
    
    std::vector<float> lepWJ_pt;
    lepWJ_pt.push_back( lepton.pt() );
    lepWJ_pt.push_back( jet1_W.Et() );
    lepWJ_pt.push_back( jet2_W.Et() );
    std::sort(lepWJ_pt.begin(), lepWJ_pt.end(), maggiore);
    
    
    
    
    
    
    //***********************
    // CUT VARIABLES FOR TREE
    
    tagJJ_Deta   = deltaEta(jet1_tag.eta(), jet2_tag.eta());
    tagJJ_etaProd =jet1_tag.eta()*jet2_tag.eta();
    tagJJ_m      = jet12_tag.mass();
    tagJJ_max_e = std::max(jet1_tag.energy(), jet2_tag.energy());
    tagJJ_min_e = std::min(jet1_tag.energy(), jet2_tag.energy());
    tagJJ_max_et = std::max(jet1_tag.Et(), jet2_tag.Et());
    tagJJ_min_et = std::min(jet1_tag.Et(), jet2_tag.Et());
    tagJJ_max_eta = fabs(jet1_tag.eta()) > fabs(jet2_tag.eta()) ? jet1_tag.eta() : jet2_tag.eta();
    tagJJ_min_eta = fabs(jet1_tag.eta()) < fabs(jet2_tag.eta()) ? jet1_tag.eta() : jet2_tag.eta();
    tagJ1_bTag = jet1_tag_bTag;
    tagJ2_bTag = jet2_tag_bTag;
    WJJ_pt       = jet12_W.pt();
    WJJ_eta      = jet12_W.eta();
    WJJ_zepp     = jet12_W_zepp;
    WJJ_Deta     = deltaEta(jet1_W.eta(), jet2_W.eta());
    WJJ_Dphi     = deltaPhi(jet1_W.phi(), jet2_W.phi());
    WJJ_DR       = deltaR(jet1_W.eta(), jet1_W.phi(), jet2_W.eta(), jet2_W.phi());
    WJJ_etaProd  = jet1_W.eta()*jet2_W.eta();
    WJJ_m        = jet12_W.mass();
    WJJ_max_et   = std::max(jet1_W.Et(), jet2_W.Et());
    WJJ_min_et   = std::min(jet1_W.Et(), jet2_W.Et());
    WJJ_max_eta  = fabs(jet1_W.eta()) > fabs(jet2_W.eta()) ? jet1_W.eta() : jet2_W.eta();
    WJJ_min_eta  = fabs(jet1_W.eta()) < fabs(jet2_W.eta()) ? jet1_W.eta() : jet2_W.eta();
    WJJ_max_zepp = fabs(jet1_W_zepp) > fabs(jet2_W_zepp) ? fabs(jet1_W_zepp) : fabs(jet2_W_zepp);
    WJJ_min_zepp = fabs(jet1_W_zepp) < fabs(jet2_W_zepp) ? fabs(jet1_W_zepp) : fabs(jet2_W_zepp);
    WJ1_bTag = jet1_W_bTag;
    WJ2_bTag = jet2_W_bTag;
    lep_n = leptons.size();
    lep_pt = lepton.pt();
    lep_eta = lepton.eta();
    lep_eleId = leptons_eleIdRobustTight.at(selectIt_lep);
    lep_zepp = (lepton.eta() - avgEta_tag)/absDeta_tag;    
    lep_tkIso = leptons_tkIso.at(selectIt_lep);
    lep_emIso = leptons_emIso.at(selectIt_lep);
    lep_hadIso = leptons_hadIso.at(selectIt_lep);
    lep_lipSig = leptons_lipSig.at(selectIt_lep);
    lep_tipSig = leptons_tipSig.at(selectIt_lep);
    lep_3DipSig = leptons_3DipSig.at(selectIt_lep);
    lepWJJ_pt1 = lepWJ_pt.at(0);
    lepWJJ_pt2 = lepWJ_pt.at(1);
    lepWJJ_pt3 = lepWJ_pt.at(2);    
    met_et = met.Et();
    lepMetW_Dphi = deltaPhi(lepMet.phi(), jet12_W.phi());
    lepMet_mt = sqrt( lepton.mass()*lepton.mass() + 2.*lepton.pt()*met.pt()*(1-cos(deltaPhi(lepton.phi(), met.phi()))));    
    lepMetW_mt = sqrt( lepW.mass()*lepW.mass() + 2.*lepW.pt()*met.pt()*(1-cos(deltaPhi(lepW.phi(), met.phi()))));
    nOtherJets_et15 = (int)(otherJets_et15.size());
    nOtherJets_et20 = (int)(otherJets_et20.size());
    nOtherJets_et25 = (int)(otherJets_et25.size());
    nOtherJets_et30 = (int)(otherJets_et30.size());        
    nCentralJets_et15 = (int)(centralJets_et15.size());
    nCentralJets_et20 = (int)(centralJets_et20.size());
    nCentralJets_et25 = (int)(centralJets_et25.size());
    nCentralJets_et30 = (int)(centralJets_et30.size());
    
    
    
    if(mH > 0.)
    {      
      if( (nMatching_tag == 2) && (nMatching_W == 2) )
      {
        std::vector<std::vector<int> > combinations;
        int nCombinations = Build4JetCombinations(combinations, jets.size());
        
        // loop on combinations
        for(int combIt = 0; combIt < nCombinations; ++combIt)
        {
          std::vector<int> tempCombination = combinations.at(combIt);
          
          ROOT::Math::XYZTVector j1_tag = jets.at(tempCombination.at(0));
          ROOT::Math::XYZTVector j2_tag = jets.at(tempCombination.at(1));
          ROOT::Math::XYZTVector j1_W = jets.at(tempCombination.at(2));
          ROOT::Math::XYZTVector j2_W = jets.at(tempCombination.at(3));                  

          histograms2 -> Fill("tagWJJ_matched_all_m_vs_Deta", step, deltaEta(j1_tag.eta(), j2_tag.eta()), (j1_tag+j2_tag).mass());
          histograms2 -> Fill("tagWJJ_matched_all_m_vs_Deta", step, deltaEta(j1_W.eta(), j2_W.eta()), (j1_W+j2_W).mass());          
          
          if(tempCombination == matchIt)
          {
            histograms2 -> Fill("tagWJJ_matched_right_m_vs_Deta", step, deltaEta(j1_tag.eta(), j2_tag.eta()), (j1_tag+j2_tag).mass());
            histograms2 -> Fill("tagWJJ_matched_right_m_vs_Deta", step, deltaEta(j1_W.eta(), j2_W.eta()), (j1_W+j2_W).mass());
          }
          
          else
          {
            histograms2 -> Fill("tagWJJ_matched_wrong_m_vs_Deta", step, deltaEta(j1_tag.eta(), j2_tag.eta()), (j1_tag+j2_tag).mass());
            histograms2 -> Fill("tagWJJ_matched_wrong_m_vs_Deta", step, deltaEta(j1_W.eta(), j2_W.eta()), (j1_W+j2_W).mass());
          }
        } // loop on combinations
      }
     
    
      if(nMatching_W == 2)
      {
        // 2 W matching
        if( ( (matchIt_W.at(0) == selectIt_W.at(0)) && 
              (matchIt_W.at(1) == selectIt_W.at(1)) ) || 
            ( (matchIt_W.at(0) == selectIt_W.at(1)) && 
              (matchIt_W.at(1) == selectIt_W.at(0)) ) )
        {
          histograms -> Fill("WJJ_matched_right_m", step, jet12_W.mass());
        }
        else
        {          
          histograms -> Fill("WJJ_matched_wrong_m", step, jet12_W.mass()); 
        }
      }
      
      else
      {
        //std::cout << "******************************************" << std::endl;
        //std::cout << "q1:   ";
        //Print4V(mcQ1_W);
        //std::cout << "q2:   ";
        //Print4V(mcQ2_W);
        //std::cout << "--- --- --- " << std::endl;
        //std::cout << "matching = " << matchIt_W.at(0) << "," << matchIt_W.at(1) << std::endl;
        //
        //for(unsigned int jetIt = 0; jetIt < jets.size(); ++jetIt)
        //{
        //  std::cout << "jet:  ";
        //  Print4V(jets.at(jetIt));
        //}

        histograms -> Fill("WJJ_nonMatched_m", step, jet12_W.mass());
      }
    }
    
    if(mH < 0.)
    {      
      std::vector<std::vector<int> > combinations;
      int nCombinations = Build4JetCombinations(combinations, jets.size());
      
      // loop on combinations
      for(int combIt = 0; combIt < nCombinations; ++combIt)
      {
        std::vector<int> tempCombination = combinations.at(combIt);
        
        ROOT::Math::XYZTVector j1_tag = jets.at(tempCombination.at(0));
        ROOT::Math::XYZTVector j2_tag = jets.at(tempCombination.at(1));
        ROOT::Math::XYZTVector j1_W = jets.at(tempCombination.at(2));
        ROOT::Math::XYZTVector j2_W = jets.at(tempCombination.at(3));                  
        
        histograms2 -> Fill("tagWJJ_matched_all_m_vs_Deta", step, deltaEta(j1_tag.eta(), j2_tag.eta()), (j1_tag+j2_tag).mass());
        histograms2 -> Fill("tagWJJ_matched_all_m_vs_Deta", step, deltaEta(j1_W.eta(), j2_W.eta()), (j1_W+j2_W).mass());
      }
    }
    
    
    
    // fill distributions
    stepEvents[step] += 1;

    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass()); 
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    controlRegion_lepMetW_Dphi_vs_tagJJ_Deta -> Fill(deltaEta(jet1_tag.eta(), jet2_tag.eta()), deltaPhi(lepMet.phi(), jet12_W.phi()));
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //*********************************
    // STEP 4 - Initial cuts - tag jets
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "4) tag jet cuts";
    
    if( deltaEta(jet1_tag.eta(), jet2_tag.eta()) < tagJetDetaMIN ) continue;
    if( jet12_tag.mass() < tagJetMjjMIN ) continue;
    if( jet1_tag.Et() < tagJet1EtMIN ) continue;
    if( jet1_tag.Et() > tagJet1EtMAX ) continue;    
    if( jet2_tag.Et() < tagJet2EtMIN ) continue;
    if( jet2_tag.Et() > tagJet2EtMAX ) continue;
    
    
    
    // fill distributions
    stepEvents[step] += 1;

    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(2));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(0));
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //*******************************
    // STEP 5 - Initial cuts - W jets
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "5) W jet cuts";
    
    if( jet12_W.mass() < WJetMjjMIN ) continue;
    if( jet12_W.mass() > WJetMjjMAX ) continue;
    if( jet1_W.Et() < WJet1EtMIN ) continue;
    if( jet1_W.Et() > WJet1EtMAX ) continue;
    if( jet2_W.Et() < WJet2EtMIN ) continue;
    if( jet2_W.Et() > WJet2EtMAX ) continue;
    if( fabs(jet12_W_zepp) > WJetZeppMAX ) continue;
    if( deltaR(jet1_W.eta(), jet1_W.phi(), jet2_W.eta(), jet2_W.phi()) > WJetDRMAX) continue;
    
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
        
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //*******************************
    // STEP 6 - Initial cuts - CJV
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "6) CJV";
    
    if(centralJets_et25.size() > 0) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //*******************************
    // STEP 7 - Initial cuts - lepton
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "7) lepton cuts";
    
    if( leptons.size() > 1 ) continue;
    if( lepton.pt() < lepPtMIN ) continue;
    if( lepton.pt() > lepPtMAX ) continue;
    if( leptons_tkIso.at(selectIt_lep) / lepton.pt() > lepTkIsoOverPtMAX) continue;
    if( (leptonFlavours.at(selectIt_lep) == "electron") &&
        (leptons_eleIdRobustTight.at(selectIt_lep) < eleIdValueMIN) ) continue;
    if( fabs(lep_zepp) > lepZeppMAX ) continue;
    if( lep_tipSig > lepTipSigMAX ) continue; 
    if( fabs(lepton.eta()) > lepAbsEtaMAX) continue;
    if( (lepMet_mt < lepMetMtMIN) || (lepMet_mt > lepMetMtMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //**********************************
    // STEP 8 - Initial cuts - b-tagging
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "8) b-tagging";
    
    if( jet1_tag_bTag > tagJetBTagMAX ) continue; 
    if( jet2_tag_bTag > tagJetBTagMAX ) continue;
    if( jet1_W_bTag > WJetBTagMAX ) continue; 
    if( jet2_W_bTag > WJetBTagMAX ) continue;
    
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //**********************************
    // STEP 9 - Initial cuts - met
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "9) met";
    
    if( met.Et() < metEtMIN ) continue; 
    if( met.Et() > metEtMAX ) continue;    
    
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //***************************************
    // STEP 10 - Initial cuts - lepMet - W cut
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "10) lepMet-W Dphi";
    
    if( deltaPhi(lepMet.phi(), jet12_W.phi()) > lepMet_W_DphiMAX ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    //*******************************
    // STEP 11 - Initial cuts - pt max
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "11) pt max";
    
    if( lepWJ_pt.at(0) < ptMaxMIN ) continue;
    if( tagWJ_et.at(0) < tagWJetEt1MIN ) continue;
    if( tagWJ_et.at(1) < tagWJetEt2MIN ) continue;
    if( tagWJ_et.at(2) < tagWJetEt3MIN ) continue;
    if( tagWJ_et.at(3) < tagWJetEt4MIN ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    tree.at(step) -> Fill();
    
    
    
    
    
    
    ////*******************************
    //// STEP 12 - Initial cuts - rapGap
    //step += 1;
    ////std::cout << ">>> step: " << step << std::endl;
    //stepName[step] = "12) rapGap";
    //
    //if( std::min(lep_etaGap1, lep_etaGap2) < lepEtaGapMIN ) continue;
    //
    //
    //
    //// fill distributions
    //stepEvents[step] += 1;
    //
    //histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());    
    //histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    //histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    //histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    //histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    //
    //histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    //histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    //
    //if(leptonFlavours.at(selectIt_lep) == "electron")
    //{
    //  histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
    //  histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
    //  histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    //}
    //if(leptonFlavours.at(selectIt_lep) == "muon")
    //{
    //  histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
    //  histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
    //  histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    //}
    //
    //histograms -> Fill("lep_lipSig", step, lep_lipSig);
    //histograms -> Fill("lep_tipSig", step, lep_tipSig);
    //histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    //
    //histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    //histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    //histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    //histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    //histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    //histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    //histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    //
    //histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    //
    //histograms -> Fill("lepMet_mt", step, lepMet_mt);
    //histograms -> Fill("lepMetW_mt", step, lepMetW_mt);
    //
    //histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    //
    //
    //stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    //stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    //stdHistograms -> Fill1(jets, "allJ", step);
    //stdHistograms -> Fill1(met, "met", step);
    //stdHistograms -> Fill1(lepton, "lep", step, &eta1_tag, &eta2_tag);
    //stdHistograms -> Fill2(lepton, met, "lepMet", step, &eta1_tag, &eta2_tag);
    //stdHistograms -> Fill2(lepMet, jet12_W, "lepMetW", step, &eta1_tag, &eta2_tag);
    //stdHistograms -> Fill1(otherJets_et15, "otherJ_et15", step);
    //stdHistograms -> Fill1(otherJets_et20, "otherJ_et20", step);
    //stdHistograms -> Fill1(otherJets_et25, "otherJ_et25", step);
    //stdHistograms -> Fill1(otherJets_et30, "otherJ_et30", step);
    //stdHistograms -> Fill1(centralJets_et15, "centralJ_et15", step);    
    //stdHistograms -> Fill1(centralJets_et20, "centralJ_et20", step);
    //stdHistograms -> Fill1(centralJets_et25, "centralJ_et25", step);
    //stdHistograms -> Fill1(centralJets_et30, "centralJ_et30", step);
    //
    //tree.at(step) -> Fill();
    
    
    
    
    
    
    //**********************************************
    // STEP 12 - Initial cuts - higgs transverse mass
    step += 1;
    //std::cout << ">>> step: " << step << std::endl;
    stepName[step] = "12) Higgs m2";
    
    if( lepMetW_mt < lepMetWMtMIN ) continue;
    if( lepMetW_mt > lepMetWMtMAX ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    
    histograms -> Fill("tagJJ_etaProd", step, jet1_tag.eta()*jet2_tag.eta());    
    histograms -> Fill("tagJJ_bTag", step, jet1_tag_bTag);
    histograms -> Fill("tagJJ_bTag", step, jet2_tag_bTag);
    histograms -> Fill("WJJ_bTag", step, jet1_W_bTag);
    histograms -> Fill("WJJ_bTag", step, jet2_W_bTag);
    
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_W.eta(),   jet2_W.eta()),   (jet1_W+jet2_W).mass()); 
    histograms2 -> Fill("tagWJJ_m_vs_Deta", step, deltaEta(jet1_tag.eta(), jet2_tag.eta()), (jet1_tag+jet2_tag).mass());
    
    if(leptonFlavours.at(selectIt_lep) == "electron")
    {
      histograms -> Fill("ele_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("ele_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("ele_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    if(leptonFlavours.at(selectIt_lep) == "muon")
    {
      histograms -> Fill("mu_tkIso", step, leptons_tkIso.at(selectIt_lep)/lepton.pt());        
      histograms -> Fill("mu_emIso", step, leptons_emIso.at(selectIt_lep)/lepton.Et());
      histograms -> Fill("mu_hadIso", step, leptons_hadIso.at(selectIt_lep)/lepton.Et());
    }
    
    histograms -> Fill("lep_lipSig", step, lep_lipSig);
    histograms -> Fill("lep_tipSig", step, lep_tipSig);
    histograms -> Fill("lep_3DipSig", step, lep_3DipSig);
    
    histograms -> Fill("tagWJ_et1", step, tagWJ_et.at(0));
    histograms -> Fill("tagWJ_et2", step, tagWJ_et.at(1));
    histograms -> Fill("tagWJ_et3", step, tagWJ_et.at(2));
    histograms -> Fill("tagWJ_et4", step, tagWJ_et.at(3));
    histograms -> Fill("lepWJ_pt1", step, lepWJ_pt.at(0));
    histograms -> Fill("lepWJ_pt2", step, lepWJ_pt.at(1));
    histograms -> Fill("lepWJ_pt3", step, lepWJ_pt.at(2));
    
    histograms2 -> Fill("lepWJ_pt1_vs_tagJJ_pt", step, jet12_tag.pt(), lepWJ_pt.at(0));
    
    histograms -> Fill("lepMet_mt", step, lepMet_mt);
    histograms -> Fill("lepMetW_mt", step, lepMetW_mt);    
    
    histograms -> Fill("TMVA_kBDT",  step, bestMvaValue);
    
    
    stdHistograms -> Fill2(jet1_tag, jet2_tag, "tagJJ", step);
    stdHistograms -> Fill2(jet1_W, jet2_W, "WJJ", step, &eta1_tag, &eta2_tag);
    stdHistograms -> Fill1(jets, "allJ", step);
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
    
    tree.at(step) -> Fill();
    
    
    
  } // loop over the events
  
  
  
  
  
  
  // save tree
  TFile* outputTreeFile = new TFile(outputTreeFileName.c_str(), "RECREATE");
  outputTreeFile -> cd();
  
  for(step = 0; step < nStep; ++step)
    tree.at(step) -> Write();
  
  for(step = 0; step < nStep; ++step)
  {
    treeEvents.at(step) -> Fill();
    treeEvents.at(step) -> Write();
  }  
  
  outputTreeFile -> Close();
  delete outputTreeFile;
  
  
  
  // save histograms
  TFile* outputRootFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  outputRootFile -> cd();
  
  for(step = 0; step < nStep; ++step)
  {
    events -> SetBinContent(step+1, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step+1, stepName[step].c_str());
  }
  
  events -> Write();
  purity_tag -> Write();
  purity_W -> Write();
  
  controlRegion_lepMetW_Dphi_vs_tagJJ_Deta -> Write();
  
  outputRootFile -> Close(); 
  
  
  
  delete histograms;
  delete histograms2;
  delete stdHistograms;
  delete outputRootFile;
  
  
  
  return 0;
}
