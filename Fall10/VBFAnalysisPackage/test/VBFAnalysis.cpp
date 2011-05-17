#include "VBFAnalysisVariables.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "kalanand.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>

#include "TH1F.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"
#include "Math/Vector4D.h"

#include "TMVA/Reader.h"



void SetStepNames(std::map<int, std::string>&, const std::string&, const int&, bool);






int main(int argc, char** argv)
{
  //Check if all nedeed arguments to parse are there
  if(argc != 2)
  {
    std::cerr << ">>>>> VBFAnalysis::usage: " << argv[0] << " configFileName" << std::endl ;
    return 1;
  }
  
  
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string baseDir        = gConfigParser -> readStringOption("Input::baseDir");
  std::string inputFileList  = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm   = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType        = gConfigParser -> readStringOption("Input::jetType");
  std::string higgsMass      = gConfigParser -> readStringOption("Input::higgsMass");
  std::string MVAWeightsFile = gConfigParser -> readStringOption("Input::MVAWeightsFile");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryFIRST = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int firstSTEP = gConfigParser -> readIntOption("Options::firstSTEP");
  int dataFlag = gConfigParser -> readIntOption("Options::dataFlag");
  float crossSection = gConfigParser -> readFloatOption("Options::crossSection");
  int verbosity = gConfigParser -> readIntOption("Options::verbosity"); 
  int trainMVA = gConfigParser -> readIntOption("Options::trainMVA"); 
  int applyMVA = gConfigParser -> readIntOption("Options::applyMVA"); 
  
  int jetNMIN = gConfigParser -> readIntOption("Cuts::jetNMIN");
  int jetNMAX = gConfigParser -> readIntOption("Cuts::jetNMAX");
  
  float elePtMIN = gConfigParser -> readFloatOption("Cuts::elePtMIN");
  float elePtMAX = gConfigParser -> readFloatOption("Cuts::elePtMAX");
  float muPtMIN = gConfigParser -> readFloatOption("Cuts::muPtMIN");
  float muPtMAX = gConfigParser -> readFloatOption("Cuts::muPtMAX");
  std::string leptonFLAVOUR = gConfigParser -> readStringOption("Cuts::leptonFLAVOUR");
  
  int isoCUT = gConfigParser -> readIntOption("Cuts::isoCUT");
  int antiIsoCUT = gConfigParser -> readIntOption("Cuts::antiIsoCUT");
  //float eleTkIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleTkIsoOverPtEBMAX");
  //float eleEmIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleEmIsoOverPtEBMAX");
  //float eleHadIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleHadIsoOverPtEBMAX");
  float eleCombIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleCombIsoOverPtEBMAX");
  float eleCombAntiIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleCombAntiIsoOverPtEBMAX");
  //float eleTkIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleTkIsoOverPtEEMAX");
  //float eleEmIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleEmIsoOverPtEEMAX");
  //float eleHadIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleHadIsoOverPtEEMAX");
  float eleCombIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleCombIsoOverPtEEMAX");
  float eleCombAntiIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleCombAntiIsoOverPtEEMAX");
  //float muTkIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muTkIsoOverPtMAX");
  //float muEmIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muEmIsoOverPtMAX");
  //float muHadIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muHadIsoOverPtMAX");
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
  float lepMetMtMIN = gConfigParser -> readFloatOption("Cuts::lepMetMtMIN");
  float lepMetDphiMIN = gConfigParser -> readFloatOption("Cuts::lepMetDphiMIN");  
  float lepMetDphiMAX = gConfigParser -> readFloatOption("Cuts::lepMetDphiMAX");  
  
  int metCUT = gConfigParser -> readIntOption("Cuts::metCUT");
  float metEtMIN = gConfigParser -> readFloatOption("Cuts::metEtMIN");
  float metEtMAX = gConfigParser -> readFloatOption("Cuts::metEtMAX");
  
  float tagJJMassMIN = gConfigParser -> readFloatOption("Cuts::tagJJMassMIN");
  float tagJJDetaMIN = gConfigParser -> readFloatOption("Cuts::tagJJDetaMIN");
  float tagJJMaxEtMIN = gConfigParser -> readFloatOption("Cuts::tagJJMaxEtMIN");
  float tagJJMinEtMIN = gConfigParser -> readFloatOption("Cuts::tagJJMinEtMIN");
  float tagJJMaxEtaMIN = gConfigParser -> readFloatOption("Cuts::tagJJMaxEtaMIN");
  float tagJJMinEtaMIN = gConfigParser -> readFloatOption("Cuts::tagJJMinEtaMIN");
  
  float WJJMassMIN = gConfigParser -> readFloatOption("Cuts::WJJMassMIN");
  float WJJMassMAX = gConfigParser -> readFloatOption("Cuts::WJJMassMAX");
  float WJJEtMIN = gConfigParser -> readFloatOption("Cuts::WJJEtMIN");
  float WJJMaxEtMIN = gConfigParser -> readFloatOption("Cuts::WJJMaxEtMIN");
  float WJJMinEtMIN = gConfigParser -> readFloatOption("Cuts::WJJMinEtMIN");
  float WJJDRMIN = gConfigParser -> readFloatOption("Cuts::WJJDRMIN");
  float WJJDRMAX = gConfigParser -> readFloatOption("Cuts::WJJDRMAX");
  float WJJDetaMIN = gConfigParser -> readFloatOption("Cuts::WJJDetaMIN");
  float WJJDetaMAX = gConfigParser -> readFloatOption("Cuts::WJJDetaMAX");
  float WJJDphiMIN = gConfigParser -> readFloatOption("Cuts::WJJDphiMIN");
  float WJJDphiMAX = gConfigParser -> readFloatOption("Cuts::WJJDphiMAX");
  
  float lepZeppMAX = gConfigParser -> readFloatOption("Cuts::lepZeppMAX");
  float WJJMaxZeppMAX = gConfigParser -> readFloatOption("Cuts::WJJMaxZeppMAX");
  float WJJMinZeppMAX = gConfigParser -> readFloatOption("Cuts::WJJMinZeppMAX");
  
  float lepMetWDphiMIN = gConfigParser -> readFloatOption("Cuts::lepMetWDphiMIN");
  float lepMetWDphiMAX = gConfigParser -> readFloatOption("Cuts::lepMetWDphiMAX");
  float lepNuWMMIN = gConfigParser -> readFloatOption("Cuts::lepNuWMMIN");
  float lepNuWMMAX = gConfigParser -> readFloatOption("Cuts::lepNuWMMAX");

  float lepWJJPt1MIN = gConfigParser -> readFloatOption("Cuts::lepWJJPt1MIN");  
  float lepWJJPt2MIN = gConfigParser -> readFloatOption("Cuts::lepWJJPt2MIN");
  float lepWJJPt3MIN = gConfigParser -> readFloatOption("Cuts::lepWJJPt3MIN");
  
  float MVAMIN = gConfigParser -> readFloatOption("Cuts::MVAMIN");
  
  std::map<int, int> totalEvents = GetTotalEvents("events", inputFileList.c_str()); 
  
  
  std::cout << ">>>>> VBFAnalysis::inputFileList = " << inputFileList << std::endl;
  
  
  
  
  
  
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
  int nStep = 21;
  
  TH1F* events = new TH1F("events", "events", nStep, 0., 1.*nStep);
  TH1F* events_plus_int  = new TH1F("events_plus_int",  "events_plus_int",  nStep, 0., 1.*nStep);
  TH1F* events_minus_int = new TH1F("events_minus_int", "events_minus_int", nStep, 0., 1.*nStep);
  
  
  // define event counters
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
  }
  
  
  
  
  
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
  stepEvents_plus_int[step] = totalEvents[1];
  stepEvents_minus_int[step] = totalEvents[1];
  for(int njetBin = jetNMIN; njetBin <= jetNMAX; ++njetBin)
  {
    std::map<int, int> tempMap;
    tempMap[step] = totalEvents[1];
    stepEvents_plus[njetBin] = tempMap;
    stepEvents_minus[njetBin] = tempMap;
  }
  stepNames[step] = "1) All events";
  
  
  
  //*******************************
  // STEP 2 - Skim: 1 lepton 2 jets
  step = 2;
  stepEvents[step] = totalEvents[7];
  stepEvents_plus_int[step] = totalEvents[7];
  stepEvents_minus_int[step] = totalEvents[7];
  for(int njetBin = jetNMIN; njetBin <= jetNMAX; ++njetBin)
  {
    (stepEvents_plus[njetBin])[step] = totalEvents[7];
    (stepEvents_minus[njetBin])[step] = totalEvents[7];
  }
  stepNames[step] = "2) 1! lepton && >= 2 cnt jets";
  
  
  
  //*************
  // STEP 3 - HLT
  step = 3;
  stepEvents[step] = totalEvents[9];
  stepEvents_plus_int[step] = totalEvents[9];
  stepEvents_minus_int[step] = totalEvents[9];
  for(int njetBin = jetNMIN; njetBin <= jetNMAX; ++njetBin)
  {
    (stepEvents_plus[njetBin])[step] = totalEvents[9];
    (stepEvents_minus[njetBin])[step] = totalEvents[9];
  }
  stepNames[step] = "3) HLT";
  
  
  
  
  
  
  //*********************
  // LOOP OVER THE EVENTS
  std::cout << ">>>>> VBFAnalysis::Read " << chain -> GetEntries() << " entries" << std::endl;  
  TRandom3 r;
  for(int entry = entryFIRST ; entry < chain -> GetEntries() ; ++entry)
  {
    //***********
    // DUMP EVENT
    chain -> GetEntry(entry);
    if(entry == entryMAX) break;
    if((entry%entryMODULO) == 0) std::cout << ">>>>> VBFAnalysis::GetEntry " << entry << std::endl;   
    
    
    
        
    
    
    //*******************
    // SET SOME VARIABLES
    
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
    GetJacksonAngle(vars.lepNu_ctheta,
                    vars.lep,vars.nu);
    GetJacksonAngle(vars.WJJ_ctheta,
                    vars.WJ1,vars.WJ2);
    GetJacksonAngle(vars.lepNuW_ctheta,
                    vars.lepNu,vars.WJJ);
    GetLNuJJAngles(vars.lepNuW_cphi,vars.lep_ctheta,vars.WJ1_ctheta,
                   vars.lep,vars.nu,vars.WJ1,vars.WJ2);
    
    
    // check with kalanand functions
    TLorentzVector k_lep(vars.lep.Px(),vars.lep.Py(),vars.lep.Pz(),vars.lep.E());
    TLorentzVector k_nu(vars.nu.Px(),vars.nu.Py(),vars.nu.Pz(),vars.nu.E());
    TLorentzVector k_WJ1(vars.WJ1.Px(),vars.WJ1.Py(),vars.WJ1.Pz(),vars.WJ1.E());
    TLorentzVector k_WJ2(vars.WJ2.Px(),vars.WJ2.Py(),vars.WJ2.Pz(),vars.WJ2.E());
    TLorentzVector k_lepNu = k_lep+k_nu;
    TLorentzVector k_WJJ = k_WJ1+k_WJ2;
    
    float k_lepNu_ctheta = JacksonAngle(k_lep,k_nu);
    float k_WJJ_ctheta = JacksonAngle(k_WJ1,k_WJ2);
    float k_lepNuW_ctheta = JacksonAngle(k_lepNu,k_WJJ);

    float k_lepNuW_cphi;
    float k_lep_ctheta;
    float k_WJ1_ctheta;
    dg_kin_Wuv_Wjj(k_lep,k_nu,k_WJ1,k_WJ2,k_lepNuW_cphi,k_lep_ctheta,k_WJ1_ctheta);
    
    std::cout << "lepNu_ctheta:    io: " << vars.lepNu_ctheta  << "   kalanand: " << k_lepNu_ctheta << std::endl;
    std::cout << "WJJ_ctheta:      io: " << vars.WJJ_ctheta    << "   kalanand: " << k_WJJ_ctheta << std::endl;
    std::cout << "lepNuW_ctheta:   io: " << vars.lepNuW_ctheta << "   kalanand: " << k_lepNuW_ctheta << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "lepNuW_cphi:   io: " << vars.lepNuW_cphi  << "   kalanand: " << k_lepNuW_cphi << std::endl;
    std::cout << "lep_ctheta:    io: " << vars.lep_ctheta   << "   kalanand: " << k_lep_ctheta << std::endl;
    std::cout << "WJ1_ctheta:    io: " << vars.WJ1_ctheta   << "   kalanand: " << k_WJ1_ctheta << std::endl;
     
    
    
    
    
    
    //**************************
    // REMOVE DUPLICATES IN DATA 
    if( dataFlag == 1 )
    {
      std::pair<int,int> eventLSandID(vars.lumiId, vars.eventId);
      std::pair<int,std::pair<int,int> > eventRUNandLSandID(vars.runId, eventLSandID);
      
      if( eventsMap[eventRUNandLSandID] == 1 ) continue;
      else eventsMap[eventRUNandLSandID] = 1;
    }
    
    
    
    
    
    
    
    
    //***********************
    // STEP 4 - lepton pt/eta
    step = 4;
    SetStepNames(stepNames, "lepton pt/eta", step, verbosity);
    
    if( (vars.lep_flavour == 11) && (vars.lep.pt() < elePtMIN) ) continue;
    if( (vars.lep_flavour == 11) && (vars.lep.pt() > elePtMAX) ) continue;
    if( (vars.lep_flavour == 13) && (vars.lep.pt() < muPtMIN) ) continue;
    if( (vars.lep_flavour == 13) && (vars.lep.pt() > muPtMAX) ) continue;
    if( (leptonFLAVOUR == "e") &&  (vars.lep_flavour != 11) ) continue;
    if( (leptonFLAVOUR == "mu") && (vars.lep_flavour != 13) ) continue;
    if( (vars.lep_flavour == 11) && (fabs(vars.lep.eta()) > eleAbsEtaMAX) ) continue;
    if( (vars.lep_flavour == 13) && (fabs(vars.lep.eta()) > muAbsEtaMAX) ) continue;
    
    if( vars.lep_flavour == 11 )    
    {
      if( (fabs(vars.lep_etaSC) > 1.4442) && (fabs(vars.lep_etaSC) < 1.566) ) continue;
    }
    
    // correct for mu trigger efficiency
    //if( (dataFlag == 0) && (vars.lep_flavour == 13) )
    //{
    //  float effMax = 1.; 
    //  if( (fabs(vars.lep.eta()) < 0.9) ) effMax = 0.977;
    //  if( (fabs(vars.lep.eta()) >= 0.9) && (fabs(vars.lep.eta()) < 1.2) ) effMax = 0.825;
    //  if( (fabs(vars.lep.eta()) >= 1.2) && (fabs(vars.lep.eta()) < 2.1) ) effMax = 0.945;
    //  
    //  float eff = r.Uniform(0., 1.);
    //  if( eff > effMax) continue; 
    //}
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP ) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //**********************************
    // STEP 5 - Initial cuts - lepton ID
    step += 1;
    SetStepNames(stepNames, "lepton ID", step, verbosity);
    
    
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
      
      // conversion removal
      //FIXME
      //if( vars.lep_simpleEleId80cIso < 4 ) isId = false;
      
    }
    
    
    
    if( vars.lep_flavour == 13 )
    {
      if( vars.lep_tracker    != 1 ) isId = false;
      if( vars.lep_standalone != 1 ) isId = false;
      if( vars.lep_global     != 1 ) isId = false;
      
      if( fabs(vars.lep_dxy_PV)             > muDxyMAX )                      isId = false;
      if( vars.lep_normalizedChi2           > muNormalizedChi2MAX )           isId = false;
      if( vars.lep_numberOfValidTrackerHits < muNumberOfValidTrackerHitsMIN ) isId = false;
      if( vars.lep_numberOfValidMuonHits    < muNumberOfValidMuonHitsMIN )    isId = false;
    }
    
    // normal id cut
    if( (idCUT == 1) && (isId == false) ) continue;
        
    // anti id cut
    if( (antiIdCUT == 1) && (isId == true) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //******************************************
    // STEP 6 - Initial cuts - lepton isolation
    step += 1;
    SetStepNames(stepNames, "lepton isolation", step, verbosity);
    
    
    bool isIsolated = true;
    bool isAntiIsolated = true;
    
    float rho = vars.rhoForIsolation;
    
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
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //***************************************
    // STEP 7 - Initial cuts - lepton 3D I.P.
    step += 1;
    SetStepNames(stepNames, "lepton 3D I.P.", step, verbosity);
    
    
    bool is3DIP = true;
    
    if( vars.lep_flavour == 11 )
    {
      //FIXME
      if( fabs(vars.lep_dxy_PV/vars.lep_edxy_PV) > ele3DipMAX ) is3DIP = false;
    }
    
    if( vars.lep_flavour == 13 )
    {
      //FIXME
      if( fabs(vars.lep_dxy_PV/vars.lep_edxy_PV) > mu3DipMAX ) is3DIP = false;
    }    
    
    
    // normal isolation cut
    if( is3DIP == false ) continue; 
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //****************************
    // STEP 8 - Initial cuts - met
    step += 1;
    SetStepNames(stepNames, "met", step, verbosity);
    
    
    if( metCUT == 1 )
      if( vars.met.Et() < 25. ) continue;
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
       
    
    
    
    
    //**********************************
    // STEP 9 - Initial cuts - 2 W-jets
    step += 1;
    SetStepNames(stepNames, "2 W-jets", step, verbosity);
    
    
    //if( vars.nJets > 3 ) continue;
    //if( vars.nJets_cnt > 2 ) continue;
    if( (vars.WJ1.Et() <= 0.) || (vars.WJ2.Et() <= 0.) ) continue;
      
    // Fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //************************************
    // STEP 10 - Initial cuts - b-tag veto
    step += 1;
    SetStepNames(stepNames, "b-tag veto", step, verbosity);
    
    
    bool isBTagged = false;
    //for(unsigned int jetIt = 0; jetIt < vars.nJets; ++jetIt)
    //{
    //  if( vars.jets_bTag.at(jetIt) > 2.5 ) isBTagged = true;
    //}
    
    if( (vars.nJets >=1) && (vars.jets_bTag1 > 2.50) ) isBTagged = true;
    if( (vars.nJets >=2) && (vars.jets_bTag2 > 2.00) ) isBTagged = true;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (isBTagged == true) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //*************************************
    // STEP 11 - Initial cuts - lep. W cuts
    step += 1;
    SetStepNames(stepNames, "lep. W cuts", step, verbosity);
    

    if( metCUT == 1 )    
    {
      if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.met.Et() < metEtMIN) ) continue;
      if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.met.Et() > metEtMAX) ) continue;
    }
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.lepMet_mt < lepMetMtMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepMet_Dphi) < lepMetDphiMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepMet_Dphi) > lepMetDphiMAX) ) continue;
    
    //float phi = vars.WJ1.phi();
    //if( vars.WJ2.Et() > vars.WJ1.Et() )
    //  phi = vars.WJ2.phi();    
    //
    //if( deltaPhi(vars.met.phi(),phi) < 0.4 ) continue;
    
    // fill distributions    
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //*************************************
    // STEP 12 - Initial cuts - had. W cuts
    step += 1;
    SetStepNames(stepNames, "had. W cuts", step, verbosity);
    
    
    // mjj cut
    if( std::max(vars.WJ1.Et(), vars.WJ2.Et()) < WJJMaxEtMIN ) continue;
    if( std::min(vars.WJ1.Et(), vars.WJ2.Et()) < WJJMinEtMIN ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (vars.WJ1+vars.WJ2).mass() < WJJMassMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (vars.WJ1+vars.WJ2).mass() > WJJMassMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (vars.WJ1+vars.WJ2).Et() < WJJEtMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( fabs(deltaR(vars.WJ1.eta(),vars.WJ1.phi(),vars.WJ2.eta(),vars.WJ2.phi())) < WJJDRMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( fabs(deltaR(vars.WJ1.eta(),vars.WJ1.phi(),vars.WJ2.eta(),vars.WJ2.phi())) > WJJDRMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( fabs(deltaEta(vars.WJ1.eta(),vars.WJ2.eta())) < WJJDetaMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( fabs(deltaEta(vars.WJ1.eta(),vars.WJ2.eta())) > WJJDetaMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( fabs(deltaPhi(vars.WJ1.phi(),vars.WJ2.phi())) < WJJDphiMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( fabs(deltaPhi(vars.WJ1.phi(),vars.WJ2.phi())) > WJJDphiMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //**********************************
    // STEP 13 - Initial cuts - W-W cuts
    step += 1;
    SetStepNames(stepNames, "W-W cuts", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepMetW_Dphi) < lepMetWDphiMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepMetW_Dphi) > lepMetWDphiMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //**************************************
    // STEP 14 - Initial cuts - pt max  cuts
    step += 1;
    SetStepNames(stepNames, "pt max cuts", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepWJJ_pt1) < lepWJJPt1MIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepWJJ_pt2) < lepWJJPt2MIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepWJJ_pt3) < lepWJJPt3MIN) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //************************************
    // STEP 15 - Initial cuts - Higgs mass
    step += 1;
    SetStepNames(stepNames, "Higgs mass", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepNuW_m) < lepNuWMMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepNuW_m) > lepNuWMMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //**********************************
    // STEP 16 - Initial cuts - < 4 jets
    step += 1;
    SetStepNames(stepNames, "< 4 jets", step, verbosity);
    
    
    if( vars.nJets < 4 )
    {
      // fill distributions
      stepEvents[step] += 1;
      if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
      if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
      if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
      if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
      
      if( step >= firstSTEP) cloneTrees[step] -> Fill();
    }
    
    
    
    
    
    
    //************************************
    // STEP 17 - Initial cuts - 2 tag-jets
    step += 1;
    SetStepNames(stepNames, "2 tag-jets", step, verbosity);
    
    
    // at least 1 tag jet
    //if( (vars.selectIt_tag.at(0) == -1) && (vars.selectIt_tag.at(1) == -1) ) continue;
    
    // at least 2 tag jets
    if( (vars.tagJ1.Et() <= 0.) || (vars.tagJ2.Et() <= 0.) ) continue;
    
    
    // Fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    

    //**************************************
    // STEP 18 - Initial cuts - tag-jet cuts
    step += 1;
    SetStepNames(stepNames, "tag-jet cuts", step, verbosity);
    

    if( std::max(vars.tagJ1.Et(),vars.tagJ2.Et()) < tagJJMaxEtMIN ) continue;
    if( std::min(vars.tagJ1.Et(),vars.tagJ2.Et()) < tagJJMinEtMIN ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(deltaEta(vars.tagJ1.eta(),vars.tagJ2.eta())) < tagJJDetaMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ((vars.tagJ1+vars.tagJ2).mass() < tagJJMassMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (std::max(fabs(vars.tagJ1.eta()),fabs(vars.tagJ2.eta())) < tagJJMaxEtaMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (std::min(fabs(vars.tagJ1.eta()),fabs(vars.tagJ2.eta())) < tagJJMinEtaMIN) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //*****************************************
    // STEP 19 - Initial cuts - zeppenfeld cuts
    step += 1;
    SetStepNames(stepNames, "zeppenfeld cuts", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lep_zepp) > lepZeppMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (std::max(fabs(vars.WJ1_zepp),fabs(vars.WJ2_zepp)) > WJJMaxZeppMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (std::min(fabs(vars.WJ1_zepp),fabs(vars.WJ2_zepp)) > WJJMinZeppMAX) ) continue;
    
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //**********************************
    // STEP 20 - Initial cuts - jet veto
    step += 1;
    SetStepNames(stepNames, "jet veto", step, verbosity);
    
    
    if( (vars.nJets_btw_et20) > 1 ) continue;    
    
    
    // read MVA
    if( applyMVA == 1 )
    {
      vars.mva = MVAReader -> EvaluateMVA("kBDT");    
    }  
    
        
    // fill distributions
    stepEvents[step] += 1;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //*****************************
    // STEP 21 - Initial cuts - MVA
    step += 1;
    SetStepNames(stepNames, "MVA", step, verbosity);
    
    
    if( vars.mva < MVAMIN ) continue;    
    

    // fill distributions
    stepEvents[step] += 1;
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
    
    
    events_plus_int -> SetBinContent(step, stepEvents_plus_int[step]);
    events_plus_int -> GetXaxis() -> SetBinLabel(step, stepNames[step].c_str());
    
    events_minus_int -> SetBinContent(step, stepEvents_minus_int[step]);
    events_minus_int -> GetXaxis() -> SetBinLabel(step, stepNames[step].c_str());
    
    
    for(int njetBin = jetNMIN; njetBin <= jetNMAX; ++njetBin)
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
  events_plus_int -> Write();
  events_minus_int -> Write();
  for(int njetBin = jetNMIN; njetBin <= jetNMAX; ++njetBin)
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

//  LocalWords:  flavours
