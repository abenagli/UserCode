#include "VBFAnalysisVariables.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "analysisUtils.h"
#include "readJSONFile.h"
#include "QGLikelihoodCalculator.h"
#include "VBFKinematicFit.h"

#include <iomanip>

#include "TH1F.h"
#include "TF1.h"
#include "TF2.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"
#include "Math/Vector4D.h"

#include "TMVA/Reader.h"



void SetStepNames(std::map<int, std::string>&, const std::string&, const int&, bool);
bool AcceptHLTPath(const std::vector<std::string>&, const std::vector<float>&, const std::string&);






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> CDFAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string baseDir        = gConfigParser -> readStringOption("Input::baseDir");
  std::string inputFileList  = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm   = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType        = gConfigParser -> readStringOption("Input::jetType");
  std::string higgsMass      = gConfigParser -> readStringOption("Input::higgsMass");
  std::string jsonFileName   = gConfigParser -> readStringOption("Input::jsonFileName");
  std::string MVAWeightsFile = gConfigParser -> readStringOption("Input::MVAWeightsFile");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryFIRST = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int firstSTEP = gConfigParser -> readIntOption("Options::firstSTEP");
  int verbosity = gConfigParser -> readIntOption("Options::verbosity"); 
  int trainMVA = gConfigParser -> readIntOption("Options::trainMVA"); 
  int applyMVA = gConfigParser -> readIntOption("Options::applyMVA"); 
  
  int HLTCUT = gConfigParser -> readIntOption("Cuts::HLTCUT");
  
  float elePtMIN = gConfigParser -> readFloatOption("Cuts::elePtMIN");
  float elePtMAX = gConfigParser -> readFloatOption("Cuts::elePtMAX");
  float muPtMIN = gConfigParser -> readFloatOption("Cuts::muPtMIN");
  float muPtMAX = gConfigParser -> readFloatOption("Cuts::muPtMAX");
  std::string leptonFLAVOUR = gConfigParser -> readStringOption("Cuts::leptonFLAVOUR");
  
  int isoCUT = gConfigParser -> readIntOption("Cuts::isoCUT");
  int antiIsoCUT = gConfigParser -> readIntOption("Cuts::antiIsoCUT");
  float eleCombIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleCombIsoOverPtEBMAX");
  float eleCombAntiIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleCombAntiIsoOverPtEBMAX");
  float eleCombIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleCombIsoOverPtEEMAX");
  float eleCombAntiIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleCombAntiIsoOverPtEEMAX");
  float muCombIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muCombIsoOverPtMAX");
  float muCombAntiIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muCombAntiIsoOverPtMAX");
  
  int idCUT = gConfigParser -> readIntOption("Cuts::idCUT");
  int antiIdCUT = gConfigParser -> readIntOption("Cuts::antiIdCUT");
  float eleHOverEEBMAX = gConfigParser -> readFloatOption("Cuts::eleHoverEEBMAX");
  float eleDetaInEBMAX = gConfigParser -> readFloatOption("Cuts::eleDetaInEBMAX");
  float eleDphiInEBMAX = gConfigParser -> readFloatOption("Cuts::eleDphiInEBMAX");
  float eleSigmaIetaIetaEBMAX = gConfigParser -> readFloatOption("Cuts::eleSigmaIetaIetaEBMAX");
  float eleHOverEEEMAX = gConfigParser -> readFloatOption("Cuts::eleHoverEEEMAX");
  float eleDetaInEEMAX = gConfigParser -> readFloatOption("Cuts::eleDetaInEEMAX");
  float eleDphiInEEMAX = gConfigParser -> readFloatOption("Cuts::eleDphiInEEMAX");
  float eleSigmaIetaIetaEEMAX = gConfigParser -> readFloatOption("Cuts::eleSigmaIetaIetaEEMAX");
  float ele3DipMAX = gConfigParser -> readFloatOption("Cuts::ele3DipMAX");
  
  float muDxyMAX = gConfigParser -> readFloatOption("Cuts::muDxyMAX");
  float muNormalizedChi2MAX = gConfigParser -> readFloatOption("Cuts::muNormalizedChi2MAX");
  float muNumberOfValidTrackerHitsMIN = gConfigParser -> readFloatOption("Cuts::muNumberOfValidTrackerHitsMIN");
  float muNumberOfValidMuonHitsMIN = gConfigParser -> readFloatOption("Cuts::muNumberOfValidMuonHitsMIN");
  float mu3DipMAX = gConfigParser -> readFloatOption("Cuts::mu3DipMAX");
  
  float eleAbsEtaMAX = gConfigParser -> readFloatOption("Cuts::eleAbsEtaMAX");
  float muAbsEtaMAX = gConfigParser -> readFloatOption("Cuts::muAbsEtaMAX");
  int lepMetMtCUT = gConfigParser -> readIntOption("Cuts::lepMetMtCUT");
  float lepMetMtMIN = gConfigParser -> readFloatOption("Cuts::lepMetMtMIN");
  float lepMetDphiMIN = gConfigParser -> readFloatOption("Cuts::lepMetDphiMIN");  
  float lepMetDphiMAX = gConfigParser -> readFloatOption("Cuts::lepMetDphiMAX");  
  
  int metCUT = gConfigParser -> readIntOption("Cuts::metCUT");
  float metEtMIN = gConfigParser -> readFloatOption("Cuts::metEtMIN");
  float metEtMAX = gConfigParser -> readFloatOption("Cuts::metEtMAX");
  
  float WJJMassMIN = gConfigParser -> readFloatOption("Cuts::WJJMassMIN");
  float WJJMassMAX = gConfigParser -> readFloatOption("Cuts::WJJMassMAX");
  float WJJMaxPtMIN = gConfigParser -> readFloatOption("Cuts::WJJMaxPtMIN");
  float WJJMinPtMIN = gConfigParser -> readFloatOption("Cuts::WJJMinPtMIN");
  float WJJDetaMIN = gConfigParser -> readFloatOption("Cuts::WJJDetaMIN");
  float WJJDetaMAX = gConfigParser -> readFloatOption("Cuts::WJJDetaMAX");
  float WJJDphiMIN = gConfigParser -> readFloatOption("Cuts::WJJDphiMIN");
  float WJJDphiMAX = gConfigParser -> readFloatOption("Cuts::WJJDphiMAX");
  
  float WPtMIN = gConfigParser -> readFloatOption("Cuts::WPtMIN");
  
   float lepCthetaMIN = gConfigParser -> readFloatOption("Cuts::lepCthetaMIN");
  float lepCthetaMAX = gConfigParser -> readFloatOption("Cuts::lepCthetaMAX");
  float WJ1CthetaMIN = gConfigParser -> readFloatOption("Cuts::WJ1CthetaMIN");
  float WJ1CthetaMAX = gConfigParser -> readFloatOption("Cuts::WJ1CthetaMAX");
  float lepNuCthetaMIN = gConfigParser -> readFloatOption("Cuts::lepNuCthetaMIN");
  float lepNuCthetaMAX = gConfigParser -> readFloatOption("Cuts::lepNuCthetaMAX");
  float lepWJ1DphiMIN = gConfigParser -> readFloatOption("Cuts::lepWJ1DphiMIN");
  float lepWJ1DphiMAX = gConfigParser -> readFloatOption("Cuts::lepWJ1DphiMAX");
  
  float chi2MAX = gConfigParser -> readFloatOption("Cuts::chi2MAX");
    
  float MVAMIN = gConfigParser -> readFloatOption("Cuts::MVAMIN");
  
  std::map<int, int> totalEvents = GetTotalEvents("events", inputFileList.c_str()); 
  
  
  std::cout << ">>>>> CDFAnalysis::inputFileList = " << inputFileList << std::endl;
  
  
  
  
  
  
  // Open tree
  std::string treeName = "ntu";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  

  // define map with events
  std::map<std::pair<int,std::pair<int,int> >,int> eventsMap;  
  
  
  // define out file names
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+"_"+jetAlgorithm+".root";
  TFile* outputRootFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  outputRootFile -> cd();
  
  
  
  // define event histogram
  int nStep = 18;
  
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  TH1F* events_PURescaled = new TH1F("events_PURescaled", "events_PURescaled", nStep, 0., 1.*nStep);
  TH1F* events_plus_int  = new TH1F("events_plus_int",  "events_plus_int",  nStep, 0., 1.*nStep);
  TH1F* events_minus_int = new TH1F("events_minus_int", "events_minus_int", nStep, 0., 1.*nStep);
  
  
  // define event counters
  std::map<int, TH1F*> events_plus;
  std::map<int, TH1F*> events_minus;
  for(int njetBin = 0; njetBin <= 5; ++njetBin)
  {
    char eventsPlusName[50];
    sprintf(eventsPlusName, "events_plus_%djets", njetBin);
    events_plus[njetBin] = new TH1F(eventsPlusName, eventsPlusName, nStep, 0., 1.*nStep);
    
    char eventsMinusName[50];
    sprintf(eventsMinusName, "events_minus_%djets", njetBin);
    events_minus[njetBin] = new TH1F(eventsMinusName, eventsMinusName, nStep, 0., 1.*nStep);
  }
  
  std::map<int, int> stepEvents;
  std::map<int, float> stepEvents_PURescaled;
  std::map<int, int> stepEvents_plus_int;
  std::map<int, int> stepEvents_minus_int;
  std::map<int, std::map<int, int> > stepEvents_plus;
  std::map<int, std::map<int, int> > stepEvents_minus;
  
  std::map<int, std::string> stepNames;
  
  
  // define variable/histogram container
  VBFAnalysisVariables vars;
  SetVBFPreselectionTreeBranches(vars, chain);
  
  
  
  // define clone trees
  std::map<int, TTree*> cloneTrees;
  for(int step = firstSTEP; step <= nStep; ++step)
  {
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);;
    cloneTrees[step] = chain -> CloneTree(0);
    cloneTrees[step] -> SetName(treeName); 
    AddVBFAnalysisTreeBranches(vars,cloneTrees[step]);
    InitializeVBFAnalysisTree(vars);
  }
  
  
  
  // define run/LS map from JSON file
  std::cout << ">>> VBFPreselection::Get run/LS map from JSON file" << std::endl;
  std::map<int, std::vector<std::pair<int, int> > > jsonMap;
  jsonMap = readJSONFile(jsonFileName);
  
  
  
  // define HLT paths
  std::vector<std::string>* HLT_Names = new std::vector<std::string>;
  std::vector<float>* HLT_Accept = new std::vector<float>;
  
  chain -> SetBranchAddress("HLT_Names",&HLT_Names);
  chain -> SetBranchAddress("HLT_Accept",&HLT_Accept);
  
  std::vector<std::string> HLTPathNames_e_DATA;
  std::vector<std::string> HLTPathNames_mu_DATA;
  std::vector<std::string> HLTPathNames_e_MC;
  std::vector<std::string> HLTPathNames_mu_MC;
  
  // data
  HLTPathNames_e_DATA.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1");
  HLTPathNames_e_DATA.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2");
  HLTPathNames_e_DATA.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3");
  HLTPathNames_e_DATA.push_back("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  HLTPathNames_e_DATA.push_back("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v4");
  HLTPathNames_e_DATA.push_back("HLT_Ele22_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  HLTPathNames_e_DATA.push_back("HLT_Ele22_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v4");
  HLTPathNames_e_DATA.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  
  HLTPathNames_mu_DATA.push_back("HLT_IsoMu24_v1");
  HLTPathNames_mu_DATA.push_back("HLT_IsoMu24_v2");
  HLTPathNames_mu_DATA.push_back("HLT_IsoMu24_v4");
  HLTPathNames_mu_DATA.push_back("HLT_IsoMu24_v5");
  HLTPathNames_mu_DATA.push_back("HLT_IsoMu24_v6");
  HLTPathNames_mu_DATA.push_back("HLT_IsoMu24_v7");
  HLTPathNames_mu_DATA.push_back("HLT_IsoMu24_v8");
  HLTPathNames_mu_DATA.push_back("HLT_IsoMu24_v9");
  
  // mc
  HLTPathNames_e_MC.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2");
  
  HLTPathNames_mu_MC.push_back("HLT_IsoMu24_v1");
  
  
  
  // define the quark-gluon likelihood
  QGLikelihoodCalculator* qglikeli = new QGLikelihoodCalculator("/gwpool/users/benaglia/COLLISIONS7TeV/Fall10/NtuplePackage/data/QG_QCD_Pt-15to3000_TuneZ2_Flat_7TeV_pythia6_Summe\
r11-PU_S3_START42_V11-v2.root");
  
  
  
  // define MVA reader
  TMVA::Reader* MVAReader = new TMVA::Reader();
  if( applyMVA == 1 )
  {
    MVAReader -> AddVariable("jets_bTag1",  &vars.jets_bTag1);
    MVAReader -> AddVariable("jets_bTag2",  &vars.jets_bTag2);
    MVAReader -> AddVariable("lep_eta",     &vars.lep_eta);
    MVAReader -> AddVariable("met_et",      &vars.met_et);
    MVAReader -> AddVariable("lepMet_mt",   &vars.lepMet_mt);
    MVAReader -> AddVariable("lepMet_Dphi", &vars.lepMet_Dphi);
    MVAReader -> AddVariable("WJJ_m",       &vars.WJJ_m);
    MVAReader -> AddVariable("WJJ_DR",      &vars.WJJ_DR);
    MVAReader -> AddVariable("lepMetW_Dphi",&vars.lepMetW_Dphi);
    MVAReader -> AddVariable("lepWJJ_pt1",  &vars.lepWJJ_pt1);
    MVAReader -> AddVariable("lepWJJ_pt2",  &vars.lepWJJ_pt2);
    MVAReader -> AddVariable("lepWJJ_pt3",  &vars.lepWJJ_pt3);
    MVAReader -> AddVariable("lepNuW_m",    &vars.lepNuW_m);
    MVAReader -> AddVariable("tagJJ_Deta",  &vars.tagJJ_Deta);
    MVAReader -> AddVariable("tagJJ_m",     &vars.tagJJ_m);
    MVAReader -> AddVariable("WJ1_zepp",    &vars.WJ1_zepp);
    MVAReader -> AddVariable("WJ2_zepp",    &vars.WJ2_zepp);
    MVAReader -> AddVariable("lep_zepp",    &vars.lep_zepp);
    
    MVAReader -> BookMVA("kBDT",MVAWeightsFile);
  }
  
  
  
  
  
  
  //********************                                                                                                                                                        
  // STEP 1 - All events
  int step = 1;
  stepEvents[step] = totalEvents[1];
  stepEvents_PURescaled[step] = totalEvents[1];
  stepEvents_plus_int[step] = totalEvents[1];
  stepEvents_minus_int[step] = totalEvents[1];
  for(int njetBin = 0; njetBin <= 5; ++njetBin)
  {
    std::map<int, int> tempMap;
    tempMap[step] = totalEvents[1];
    stepEvents_plus[njetBin] = tempMap;
    stepEvents_minus[njetBin] = tempMap;
  }
  stepNames[1]  = "1) All events";
  stepNames[2]  = "2) Preselection";
  stepNames[3]  = "3) HLT";
  stepNames[4]  = "4) Lepton ID+iso";
  stepNames[5]  = "5) Lepton 3D I.P.";
  stepNames[6]  = "6) Lepton pt/eta";
  stepNames[7]  = "7) Lepton mt";
  stepNames[8]  = "8) Met";
  stepNames[9]  = "9) jet veto";
  stepNames[10] = "10) W-jet pt cut";
  stepNames[11] = "11) b-tag veto";
  stepNames[12] = "12) WJJ Deta/Dphi cuts";
  stepNames[13] = "13) W pt cut";
  stepNames[14] = "14) pt2/mjj cut";
  stepNames[15] = "15) Kinematic Fit";
  stepNames[16] = "16) chi2 cut";
  stepNames[17] = "17) Helicity angles cuts";
  stepNames[18] = "18) W mass cut";
  
  
  
  
  
  
  //*********************
  // LOOP OVER THE EVENTS
  std::cout << ">>>>> CDFAnalysis::Read " << chain -> GetEntries() << " entries" << std::endl;  
  TRandom3 r;
  for(int entry = entryFIRST ; entry < chain -> GetEntries() ; ++entry)
  {
    //***********
    // DUMP EVENT
    chain -> GetEntry(entry);
    if( entry == entryMAX ) break;
    if( ((entry%entryMODULO) == 0) || (verbosity == 1) ) std::cout << ">>>>> CDFAnalysis::GetEntry " << entry << std::endl;   
    
    
    
        
    
    
    //*******************
    // SET SOME VARIABLES
    for(int iStep = firstSTEP; iStep <= nStep; ++iStep)
      InitializeVBFAnalysisTree(vars);
    
    vars.totEvents = stepEvents[1];
    vars.lep = *(vars.p_lep);
    vars.met = *(vars.p_met);
    vars.nu = *(vars.p_nu);
    vars.lepNu = *(vars.p_lep) + *(vars.p_nu);
    vars.leadingJ = *(vars.p_leadingJ);
    vars.WJ1 = *(vars.p_WJ1);
    vars.WJ2 = *(vars.p_WJ2);
    vars.WJJ = *(vars.p_WJ1) + *(vars.p_WJ2);
    vars.tagJ1 = *(vars.p_tagJ1);
    vars.tagJ2 = *(vars.p_tagJ2);
    vars.thirdJ = *(vars.p_thirdJ);
    
    GetLNuJJAngles(vars.lepNuW_cphi,vars.lepNuZ_cphi,vars.lep_ctheta,vars.WJ1_ctheta,vars.lepNu_ctheta,
                   vars.lep,vars.nu,vars.WJ1,vars.WJ2);
    
    
    
    
    
    
    //*********************************
    // JSON FILE AND DUPLIACTES IN DATA
    
    bool skipEvent = false;
    if( vars.dataFlag == 1 )
    {
      if(AcceptEventByRunAndLumiSection(vars.runId, vars.lumiId, jsonMap) == false) skipEvent = true;      
      
      std::pair<int,long long int> eventLSandID(vars.lumiId, vars.eventId);
      std::pair<int,std::pair<int,long long int> > eventRUNandLSandID(vars.runId, eventLSandID);
      if( eventsMap[eventRUNandLSandID] == 1 ) skipEvent = true;
      else eventsMap[eventRUNandLSandID] = 1;
    }
    
    if( skipEvent == true ) continue;
    
    
    
    
    
    
    //**********************
    // STEP 2 - preselection
    step = 2;
    //SetStepNames(stepNames, "preselection", step, verbosity);
    
    
    if( (leptonFLAVOUR == "e") &&  (vars.lep_flavour != 11) ) continue;
    if( (leptonFLAVOUR == "mu") && (vars.lep_flavour != 13) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP ) cloneTrees[step] -> Fill();    
    
    
    
    
    
    
    //*************
    // STEP 3 - HLT
    step += 1;
    //SetStepNames(stepNames, "HLT", step, verbosity);
    
    
    skipEvent = true;

    if( verbosity == 1)
    {
      for(unsigned int HLTIt = 0; HLTIt < (*HLT_Names).size(); ++HLTIt)
      {
        std::cout << "HLT_Bit: "       << std::setw(3)  << HLTIt
                  << "   HLT_Name: "   << std::setw(50) << (*HLT_Names).at(HLTIt)
                  << "   HLT_Accept: " << std::setw(2)  <<(*HLT_Accept).at(HLTIt)
                  << std::endl;    
      }
    }
    
    std::vector<std::string> HLTPathNames;
    if( (vars.dataFlag == 1) && (vars.lep_flavour == 11) ) HLTPathNames = HLTPathNames_e_DATA;
    if( (vars.dataFlag == 1) && (vars.lep_flavour == 13) ) HLTPathNames = HLTPathNames_mu_DATA;
    if( (vars.dataFlag == 0) && (vars.lep_flavour == 11) ) HLTPathNames = HLTPathNames_e_MC;
    if( (vars.dataFlag == 0) && (vars.lep_flavour == 13) ) HLTPathNames = HLTPathNames_mu_MC;
    
    for(unsigned int HLTIt = 0; HLTIt < HLTPathNames.size(); ++HLTIt)
    {
      if( AcceptHLTPath(*HLT_Names,*HLT_Accept,HLTPathNames.at(HLTIt)) == true )
        skipEvent = false;
    }
        
    if( (HLTCUT == 1) && (skipEvent == true) ) continue;
    
    
    // electron trigger emulation
    if( HLTCUT == 1 )
    {
      if( (vars.lep_flavour == 11) && (vars.lep.pt() < 30.) ) continue;
      if( (vars.lep_flavour == 11) && (fabs(vars.lep.eta()) > 2.5) ) continue;
      if( (vars.lep_flavour == 11) && (vars.lep_isEB == 1) )
      {
        if( (isoCUT == 1) && (vars.lep_tkIso/vars.lep.pt()  > 0.125) ) continue;
        if( (isoCUT == 1) && (vars.lep_emIso/vars.lep.pt()  > 0.125) ) continue;
        if( (isoCUT == 1) && (vars.lep_hadIso/vars.lep.pt() > 0.125) ) continue;
        
        if( vars.lep_sigmaIetaIeta > 0.011 ) continue;
        if( fabs(vars.lep_DphiIn)  > 0.070 ) continue;
        if( fabs(vars.lep_DetaIn)  > 0.008 ) continue;
        if( vars.lep_HOverE        > 0.050 ) continue;
      }
      if( (vars.lep_flavour == 11) && (vars.lep_isEB == 0) )
      {
        if( (isoCUT == 1) && (vars.lep_tkIso/vars.lep.pt()  > 0.075) ) continue;
        if( (isoCUT == 1) && (vars.lep_emIso/vars.lep.pt()  > 0.075) ) continue;
        if( (isoCUT == 1) && (vars.lep_hadIso/vars.lep.pt() > 0.075) ) continue;
        
        if( vars.lep_sigmaIetaIeta  > 0.031 ) continue;
        if( fabs(vars.lep_DphiIn)   > 0.020 ) continue;
        if( fabs(vars.lep_DetaIn)   > 0.005 ) continue;
        if( vars.lep_HOverE         > 0.025 ) continue;
      }
      if( (vars.lep_flavour == 11) && (metCUT == 1) && (vars.met.Et() < 30.) ) continue;
      if( (vars.lep_flavour == 11) && (std::max(vars.WJ1.pt(),vars.WJ2.pt()) < 35.) ) continue;
      if( (vars.lep_flavour == 11) && (std::min(vars.WJ1.pt(),vars.WJ2.pt()) < 30.) ) continue;
    }
    
    // muon trigger emulation
    if( HLTCUT == 1 )
    {
      if( (vars.lep_flavour == 13) && (vars.lep.pt() < 25.) ) continue;
      if( (vars.lep_flavour == 13) && (fabs(vars.lep.eta()) > 2.1) ) continue;
      if( (vars.lep_flavour == 13) && (isoCUT == 1) && (vars.lep_tkIso  > 1.000) ) continue;
      if( (vars.lep_flavour == 13) && (isoCUT == 1) && (vars.lep_emIso  > 1.000) ) continue;
      if( (vars.lep_flavour == 13) && (isoCUT == 1) && (vars.lep_hadIso > 1.000) ) continue;
    }
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP ) cloneTrees[step] -> Fill();    
    
    
    
    
    
    
    //**************************************
    // STEP 4 - Initial cuts - lepton ID+iso
    step += 1;
    //SetStepNames(stepNames, "lepton ID+iso", step, verbosity);
    
    
    bool isId = true;
        
    if( vars.lep_flavour == 11 )
    {
      // barrel
      if( vars.lep_isEB == 1 )
      {
        if( vars.lep_sigmaIetaIeta  > eleSigmaIetaIetaEBMAX ) isId = false;
        if( fabs(vars.lep_DphiIn)   > eleDphiInEBMAX )        isId = false;
        if( fabs(vars.lep_DetaIn)   > eleDetaInEBMAX )        isId = false;
        if( vars.lep_HOverE         > eleHOverEEBMAX )        isId = false;
      }
      
      // endcap
      else
      {
        if( vars.lep_sigmaIetaIeta  > eleSigmaIetaIetaEEMAX ) isId = false;      
        if( fabs(vars.lep_DphiIn)   > eleDphiInEEMAX )        isId = false;
        if( fabs(vars.lep_DetaIn)   > eleDetaInEEMAX )        isId = false;
        if( vars.lep_HOverE         > eleHOverEEEMAX )        isId = false;
      }    
      
    }
    
    
    
    if( vars.lep_flavour == 13 )
    {
      if( vars.lep_tracker    != 1 ) isId = false;
      if( vars.lep_standalone != 1 ) isId = false;
      if( vars.lep_global     != 1 ) isId = false;
      
      if( fabs(vars.lep_dB)                 > muDxyMAX )                      isId = false;
      if( vars.lep_normalizedChi2           > muNormalizedChi2MAX )           isId = false;
      if( vars.lep_numberOfValidTrackerHits < muNumberOfValidTrackerHitsMIN ) isId = false;
      if( vars.lep_numberOfValidMuonHits    < muNumberOfValidMuonHitsMIN )    isId = false;
    }
    
    // normal id cut
    if( (idCUT == 1) && (isId == false) ) continue;
        
    // anti id cut
    if( (antiIdCUT == 1) && (isId == true) ) continue;
    
    
    
    bool isIsolated = true;
    bool isAntiIsolated = true;
    
    float rho = vars.rhoForIsolation;
    //float rho = 0.;
    
    if( vars.lep_flavour == 11 )
    {
      // barrel
      if( vars.lep_isEB == 1)
      {
        if( (vars.lep_tkIso + std::max(float(0.),float(vars.lep_emIso)) + vars.lep_hadIso - rho*3.1415*0.3*0.3)/vars.lep.pt() > eleCombIsoOverPtEBMAX ) isIsolated = false;
        
        if( (vars.lep_tkIso + std::max(float(0.),float(vars.lep_emIso)) + vars.lep_hadIso - rho*3.1415*0.3*0.3)/vars.lep.pt() < eleCombAntiIsoOverPtEBMAX ) isAntiIsolated = false;
      }
      
      // endcap
      else
      {
        if( (vars.lep_tkIso + vars.lep_emIso + vars.lep_hadIso - rho*3.1415*0.3*0.3)/vars.lep.pt() > eleCombIsoOverPtEEMAX ) isIsolated = false;        
        
        if( (vars.lep_tkIso + vars.lep_emIso + vars.lep_hadIso - rho*3.1415*0.3*0.3)/vars.lep.pt() < eleCombAntiIsoOverPtEEMAX ) isAntiIsolated = false;
      }
    }
    
    if( vars.lep_flavour == 13 )
    {
      if( (vars.lep_tkIso + vars.lep_emIso + vars.lep_hadIso - rho*3.1415*0.3*0.3) / vars.lep.pt() > muCombIsoOverPtMAX ) isIsolated = false;
      
      if( (vars.lep_tkIso + vars.lep_emIso + vars.lep_hadIso - rho*3.1415*0.3*0.3) / vars.lep.pt() < muCombAntiIsoOverPtMAX ) isAntiIsolated = false;
    }    
    
    
    // normal isolation cut
    if( (isoCUT == 1) && (isIsolated == false) ) continue; 
    
    // inverted isolation cut
    if( (antiIsoCUT == 1) && (isAntiIsolated == false) ) continue; 
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //***************************************
    // STEP 5 - Initial cuts - lepton 3D I.P.
    step += 1;
    //SetStepNames(stepNames, "lepton 3D I.P.", step, verbosity);
    
    
    bool is3DIP = true;
    
    if( vars.lep_flavour == 11 )
    {
      if( fabs(vars.lep_dB/vars.lep_edB) > ele3DipMAX ) is3DIP = false;
    }
    
    if( vars.lep_flavour == 13 )
    {
      if( fabs(vars.lep_dB/vars.lep_edB) > mu3DipMAX ) is3DIP = false;
    }    
    
    if( is3DIP == false ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    

    //***********************
    // STEP 6 - lepton pt/eta
    step += 1;
    //SetStepNames(stepNames, "lepton pt/eta", step, verbosity);
     
    if( (vars.lep_flavour == 11) && (vars.lep.pt() < elePtMIN) ) continue;
    if( (vars.lep_flavour == 11) && (vars.lep.pt() > elePtMAX) ) continue;
    if( (vars.lep_flavour == 13) && (vars.lep.pt() < muPtMIN) ) continue;
    if( (vars.lep_flavour == 13) && (vars.lep.pt() > muPtMAX) ) continue;
    if( (vars.lep_flavour == 11) && (fabs(vars.lep.eta()) > eleAbsEtaMAX) ) continue;
    if( (vars.lep_flavour == 13) && (fabs(vars.lep.eta()) > muAbsEtaMAX) ) continue;
    
    if( vars.lep_flavour == 11 )    
    {
      if( (fabs(vars.lep_etaSC) > 1.4442) && (fabs(vars.lep_etaSC) < 1.566) ) continue;
    }
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP ) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //*******************
    // STEP 7 - lepton mt
    step += 1;
    //SetStepNames(stepNames, "lepton mt", step, verbosity);
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (lepMetMtCUT == 1) && (vars.lepMet_mt < lepMetMtMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (lepMetMtCUT == 1) && (vars.lepNu_m > 110.) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP ) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //****************************
    // STEP 8 - Initial cuts - met
    step += 1;
    //SetStepNames(stepNames, "met", step, verbosity);
    
    
    if( metCUT == 1 )
    {
      if( vars.met.Et() < metEtMIN ) continue;
      if( vars.met.Et() > metEtMAX ) continue;
    }
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
       
    
    
    
    
    //*********************************
    // STEP 9 - Initial cuts - jet veto
    step += 1;
    //SetStepNames(stepNames, "jet veto", step, verbosity);
    
    
    if( vars.nJets_pt30 > 3 ) continue;
    
    
    // Fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
        
    
    
    
    
    //*************************************
    // STEP 10 - Initial cuts - W-jet pt cut
    step += 1;
    //SetStepNames(stepNames, "W-jet pt cut", step, verbosity);
    
    
    if( (vars.WJ1.pt() <= 30.) || (vars.WJ2.pt() <= 30.) ) continue;
    if( std::max(vars.WJ1.pt(), vars.WJ2.pt()) < WJJMaxPtMIN ) continue;
    if( std::min(vars.WJ1.pt(), vars.WJ2.pt()) < WJJMinPtMIN ) continue;
    
    if( vars.WJ1.pt() > 0. )
      vars.WJ1_QGLikelihood = qglikeli -> computeQGLikelihoodPU( vars.WJ1.Pt(),vars.rhoForJets,vars.WJ1_chargedMultiplicity,vars.WJ1_neutralMultiplicity,vars.WJ1_ptD );
    if( vars.WJ2.pt() > 0. )
      vars.WJ2_QGLikelihood = qglikeli -> computeQGLikelihoodPU( vars.WJ2.Pt(),vars.rhoForJets,vars.WJ2_chargedMultiplicity,vars.WJ2_neutralMultiplicity,vars.WJ2_ptD );
    
    if( vars.WJ2.pt() > vars.WJ1.pt() )
    {
      float QGLikelihoodDummy = vars.WJ2_QGLikelihood;
      vars.WJ2_QGLikelihood = vars.WJ1_QGLikelihood;
      vars.WJ1_QGLikelihood = QGLikelihoodDummy;
    }
    
    
    // Fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //************************************
    // STEP 11 - Initial cuts - b-tag veto
    step += 1;
    //SetStepNames(stepNames, "b-tag veto", step, verbosity);
    
    
    bool isBTagged = false;
    
    if( vars.nBTag_TCHEM_pt20 > 0 ) isBTagged = true;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (isBTagged == true) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //********************************************
    // STEP 12 - Initial cuts - WJJ Deta/Dphi cuts
    step += 1;
    //SetStepNames(stepNames, "WJJ Deta/Dphi cuts", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( deltaEta(vars.WJ1.eta(),vars.WJ2.eta()) < WJJDetaMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( deltaEta(vars.WJ1.eta(),vars.WJ2.eta()) > WJJDetaMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( deltaPhi(vars.WJ1.phi(),vars.WJ2.phi()) < WJJDphiMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( deltaPhi(vars.WJ1.phi(),vars.WJ2.phi()) > WJJDphiMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //**********************************
    // STEP 13 - Initial cuts - W pt cut
    step += 1;
    //SetStepNames(stepNames, "W pt cuts", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (vars.WJ1+vars.WJ2).Pt() < WPtMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (vars.lep+vars.met).Pt() < WPtMIN) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //*************************************
    // STEP 14 - Initial cuts - pt2/mjj cut
    step += 1;
    //SetStepNames(stepNames, "pt2/mjj cut", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (std::min(vars.WJ1.Pt(),vars.WJ2.Pt())/vars.WJJ_m) < 0.3) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //***************************************
    // STEP 15 - Initial cuts - Kinematic Fit
    step += 1;
    //SetStepNames(stepNames, "Kinematic Fit", step, verbosity);
    
    
    DoKinematicFit(vars,5.,"MIB");
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //***************************************
    // STEP 16 - Initial cuts - chi2 cut
    step += 1;
    //SetStepNames(stepNames, "chi2 cut", step, verbosity);
    
    
    if( vars.chi2_KF > chi2MAX ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //*****************************************
    // STEP 17 - Initial cuts - Helicity angles
    step += 1;
    //SetStepNames(stepNames, "Helicity angles", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.lep_ctheta < lepCthetaMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.lep_ctheta > lepCthetaMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.WJ1_ctheta < WJ1CthetaMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.WJ1_ctheta > WJ1CthetaMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.lepNu_ctheta < lepNuCthetaMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.lepNu_ctheta > lepNuCthetaMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (std::min(vars.WJ1_lep_Dphi,vars.WJ2_lep_Dphi) < lepWJ1DphiMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (std::min(vars.WJ1_lep_Dphi,vars.WJ2_lep_Dphi) > lepWJ1DphiMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //************************************
    // STEP 18 - Initial cuts - W mass cut
    step += 1;
    //SetStepNames(stepNames, "W mass cut", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (vars.WJ1+vars.WJ2).mass() < WJJMassMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (vars.WJ1+vars.WJ2).mass() > WJJMassMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += PURescaleFactor(vars.PUit_n);
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //std::cout << "end of loop" << std::endl;
  } // loop over the events
  
  
  
  
  
  
  // save histograms
  outputRootFile -> cd();
  
  for(step = 1; step <= nStep; ++step)
  {
    events -> SetBinContent(step, stepEvents[step]);
    events -> GetXaxis() -> SetBinLabel(step, stepNames[step].c_str());
    
    events_PURescaled -> SetBinContent(step, stepEvents_PURescaled[step]);
    events_PURescaled -> GetXaxis() -> SetBinLabel(step, stepNames[step].c_str());
    
    
    events_plus_int -> SetBinContent(step, stepEvents_plus_int[step]);
    events_plus_int -> GetXaxis() -> SetBinLabel(step, stepNames[step].c_str());
    
    events_minus_int -> SetBinContent(step, stepEvents_minus_int[step]);
    events_minus_int -> GetXaxis() -> SetBinLabel(step, stepNames[step].c_str());
    
    
    for(int njetBin = 0; njetBin <= 5; ++njetBin)
    {
      events_plus[njetBin] -> SetBinContent(step, (stepEvents_plus[njetBin])[step]);
      events_plus[njetBin] -> GetXaxis() -> SetBinLabel(step, stepNames[step].c_str());

      events_minus[njetBin] -> SetBinContent(step, (stepEvents_minus[njetBin])[step]);
      events_minus[njetBin] -> GetXaxis() -> SetBinLabel(step, stepNames[step].c_str());      
    } 
  }
  
  
  
  for(step = firstSTEP; step <= nStep; ++step)
  {
    cloneTrees[step] -> AutoSave();
  } 
  
  
   
  events -> Write();
  events_PURescaled -> Write();
  events_plus_int -> Write();
  events_minus_int -> Write();
  for(int njetBin = 0; njetBin <= 5; ++njetBin)
  {
    events_plus[njetBin] -> Write();
    events_minus[njetBin] -> Write();
  }
  
  outputRootFile -> Close();
  delete outputRootFile;
  
  
  
  return 0;
}






void SetStepNames(std::map<int, std::string>& stepNames, const std::string& stepName, const int& step, bool verbosity)
{
  char dummy[5];
  sprintf(dummy, "%d)", step);
  stepNames[step] = std::string(dummy)+" "+stepName;

  if(verbosity)
    std::cout << ">>>>>>>>> " << stepNames[step] << std::endl;
}



bool AcceptHLTPath(const std::vector<std::string>& HLT_Names,
                   const std::vector<float>& HLT_Accept,
                   const std::string& HLTPathName)
{
  bool acceptEvent = false;
  
  for(unsigned int HLTIt = 0; HLTIt < HLT_Names.size(); ++HLTIt)
    if( (HLT_Names.at(HLTIt) == HLTPathName) &&
        (HLT_Accept.at(HLTIt) == 1) )
      acceptEvent = true;
  
  return acceptEvent;
}
