#include "VBFAnalysisVariables.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>

#include "TH1F.h"
#include "TProfile.h"
#include "TObject.h"
#include "TRandom3.h"

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
  
  std::string baseDir = gConfigParser -> readStringOption("Input::baseDir");
  std::string inputFileList = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm  = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType       = gConfigParser -> readStringOption("Input::jetType");
  std::string higgsMass     = gConfigParser -> readStringOption("Input::higgsMass");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryFIRST = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int firstSTEP = gConfigParser -> readIntOption("Options::firstSTEP");
  int dataFlag = gConfigParser -> readIntOption("Options::dataFlag");
  float crossSection = gConfigParser -> readFloatOption("Options::crossSection");
  int verbosity = gConfigParser -> readIntOption("Options::verbosity"); 
  
  int jetNMIN = gConfigParser -> readIntOption("Cuts::jetNMIN");
  int jetNMAX = gConfigParser -> readIntOption("Cuts::jetNMAX");
  
  float lepPtMIN = gConfigParser -> readFloatOption("Cuts::lepPtMIN");
  float lepPtMAX = gConfigParser -> readFloatOption("Cuts::lepPtMAX");
  std::string leptonFLAVOUR = gConfigParser -> readStringOption("Cuts::leptonFLAVOUR");
  
  int isoCUT = gConfigParser -> readIntOption("Cuts::isoCUT");
  int antiIsoCUT = gConfigParser -> readIntOption("Cuts::antiIsoCUT");
  //float eleTkIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleTkIsoOverPtEBMAX");
  //float eleEmIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleEmIsoOverPtEBMAX");
  //float eleHadIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleHadIsoOverPtEBMAX");
  float eleCombIsoOverPtEBMAX = gConfigParser -> readFloatOption("Cuts::eleCombIsoOverPtEBMAX");
  //float eleTkIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleTkIsoOverPtEEMAX");
  //float eleEmIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleEmIsoOverPtEEMAX");
  //float eleHadIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleHadIsoOverPtEEMAX");
  float eleCombIsoOverPtEEMAX = gConfigParser -> readFloatOption("Cuts::eleCombIsoOverPtEEMAX");
  //float muTkIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muTkIsoOverPtMAX");
  //float muEmIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muEmIsoOverPtMAX");
  //float muHadIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muHadIsoOverPtMAX");
  float muCombIsoOverPtMAX = gConfigParser -> readFloatOption("Cuts::muCombIsoOverPtMAX");
  
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
  float WJJMaxEtMIN = gConfigParser -> readFloatOption("Cuts::WJJMaxEtMIN");
  float WJJMinEtMIN = gConfigParser -> readFloatOption("Cuts::WJJMinEtMIN");
  float WJJDRMIN = gConfigParser -> readFloatOption("Cuts::WJJDRMIN");
  float WJJDRMAX = gConfigParser -> readFloatOption("Cuts::WJJDRMAX");
  
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
  int nStep = 20;
  
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
    vars.mva = -99.;
    vars.lep = *(vars.p_lep);
    vars.met = *(vars.p_met);
    vars.leadingJ = *(vars.p_leadingJ);
    vars.WJ1 = *(vars.p_WJ1);
    vars.WJ2 = *(vars.p_WJ2);
    vars.tagJ1 = *(vars.p_tagJ1);
    vars.tagJ2 = *(vars.p_tagJ2);

    
    
    
    
    
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
    
    if( vars.lep.pt() < lepPtMIN ) continue;
    if( vars.lep.pt() > lepPtMAX ) continue;
    if( (leptonFLAVOUR == "e") &&  (vars.lep_flavour != 11) ) continue;
    if( (leptonFLAVOUR == "mu") && (vars.lep_flavour != 11) ) continue;
    if( (vars.lep_flavour == 11) && (fabs(vars.lep.eta()) > eleAbsEtaMAX) ) continue;
    if( (vars.lep_flavour == 13) && (fabs(vars.lep.eta()) > muAbsEtaMAX) ) continue;
    
    if( vars.lep_flavour == 11 )    
    {
      if( (fabs(vars.lep.eta()) > 1.4442) && (fabs(vars.lep.eta()) < 1.566) ) continue;
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
      if( vars.lep_simpleEleId80cIso < 4 ) isId = false;
      
    }
    
    
    
    if( vars.lep_flavour == 13 )
    {
      if( vars.lep_tracker    != 1 ) isId = false;
      if( vars.lep_standalone != 1 ) isId = false;
      if( vars.lep_global     != 1 ) isId = false;
      if( fabs(vars.lep_dxy)                > muDxyMAX )                      isId = false;
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
    
    if( vars.lep_flavour == 11 )
    {
      // barrel
      if( vars.lep_isEB == 1)
      {
        //if( vars.lep_tkIso/vars.lep.pt()  > eleTkIsoOverPtEBMAX ) isIsolated = false;
        //if( vars.lep_emIso/vars.lep.pt() > eleEmIsoOverPtEBMAX ) isIsolated = false;
        //if( vars.lep_hadIso/vars.lep.pt() > eleHadIsoOverPtEBMAX ) isIsolated = false;
        if( (vars.lep_tkIso+vars.lep_emIso+vars.lep_hadIso)/vars.lep.pt() > eleCombIsoOverPtEBMAX ) isIsolated = false;
        
        //if( vars.lep_tkIso/vars.lep.pt()  < eleTkIsoOverPtEBMAX ) isAntiIsolated = false;
        //if( vars.lep_emIso/vars.lep.pt() < eleEmIsoOverPtEBMAX ) isAntiIsolated = false;
        //if( vars.lep_hadIso/vars.lep.pt() < eleHadIsoOverPtEBMAX ) isAntiIsolated = false;
        if( (vars.lep_tkIso+vars.lep_emIso+vars.lep_hadIso)/vars.lep.pt() < eleCombIsoOverPtEBMAX ) isAntiIsolated = false;
      }
      
      // endcap
      else
      {
        //if( vars.lep_tkIso  / vars.lep.pt() > eleTkIsoOverPtEEMAX )  isIsolated = false;
	// if( vars.lep_emIso  / vars.lep.pt() > eleEmIsoOverPtEEMAX )  isIsolated = false;
        //if( vars.lep_hadIso / vars.lep.pt() > eleHadIsoOverPtEEMAX ) isIsolated = false;
        if( (vars.lep_tkIso+vars.lep_emIso+vars.lep_hadIso)/vars.lep.pt() > eleCombIsoOverPtEEMAX ) isIsolated = false;        
        
        //if( vars.lep_tkIso  / vars.lep.pt() < eleTkIsoOverPtEEMAX )  isAntiIsolated = false;
	//if( vars.lep_emIso  / vars.lep.pt() < eleEmIsoOverPtEEMAX )  isAntiIsolated = false;
        //if( vars.lep_hadIso / vars.lep.pt() < eleHadIsoOverPtEEMAX ) isAntiIsolated = false;
        if( (vars.lep_tkIso+vars.lep_emIso+vars.lep_hadIso)/vars.lep.pt() < eleCombIsoOverPtEEMAX ) isAntiIsolated = false;
      }
    }
    
    if( vars.lep_flavour == 13 )
    {
      //isolation
      //if( vars.lep_tkIso  / vars.lep.pt() > muTkIsoOverPtMAX )  isIsolated = false;
      //if( vars.lep_emIso  / vars.lep.pt() > muEmIsoOverPtMAX )  isIsolated = false;
      //if( vars.lep_hadIso / vars.lep.pt() > muHadIsoOverPtMAX ) isIsolated = false;
      if( (vars.lep_tkIso+vars.lep_emIso+vars.lep_hadIso) / vars.lep.pt() > muCombIsoOverPtMAX ) isIsolated = false;
      
      //if( vars.lep_tkIso  / vars.lep.pt() < muTkIsoOverPtMAX )  isAntiIsolated = false;
      //if( vars.lep_emIso  / vars.lep.pt() < muEmIsoOverPtMAX )  isAntiIsolated = false;
      //if( vars.lep_hadIso / vars.lep.pt() < muHadIsoOverPtMAX ) isAntiIsolated = false;
      if( (vars.lep_tkIso+vars.lep_emIso+vars.lep_hadIso) / vars.lep.pt() < muCombIsoOverPtMAX ) isAntiIsolated = false;
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
      if( fabs(vars.lep_dB/vars.lep_edB) > ele3DipMAX ) is3DIP = false;
    }
    
    if( vars.lep_flavour == 13 )
    {
      if( fabs(vars.lep_dB/vars.lep_edB) > mu3DipMAX ) is3DIP = false;
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
    
    
    if( vars.met.Et() < 20. ) continue;
    
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
    
    
    // at least 4 jets
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
    if( (vars.nJets >=2) && (vars.jets_bTag2 > 1.50) ) isBTagged = true;
    if( isBTagged == true) continue;
    
    
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
    
    
    if( vars.met.Et() < metEtMIN ) continue;
    if( vars.met.Et() > metEtMAX ) continue;
    if( vars.lepMet_mt < lepMetMtMIN ) continue;
    if( fabs(vars.lepMet_Dphi) < lepMetDphiMIN ) continue;
    if( fabs(vars.lepMet_Dphi) > lepMetDphiMAX ) continue;
    
    
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
    if( (vars.WJ1+vars.WJ2).mass() < WJJMassMIN ) continue;
    if( (vars.WJ1+vars.WJ2).mass() > WJJMassMAX ) continue;
    if( std::max(vars.WJ1.Et(), vars.WJ2.Et()) < WJJMaxEtMIN ) continue;
    if( std::min(vars.WJ1.Et(), vars.WJ2.Et()) < WJJMinEtMIN ) continue;
    if( fabs(deltaR(vars.WJ1.eta(),vars.WJ1.phi(),vars.WJ2.eta(),vars.WJ2.phi())) < WJJDRMIN ) continue;
    if( fabs(deltaR(vars.WJ1.eta(),vars.WJ1.phi(),vars.WJ2.eta(),vars.WJ2.phi())) > WJJDRMAX ) continue;
    
    
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
    
    
    if( fabs(vars.lepMetW_Dphi) < lepMetWDphiMIN ) continue;
    if( fabs(vars.lepMetW_Dphi) > lepMetWDphiMAX ) continue;
    
    
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
    
    
    if( fabs(vars.lepWJJ_pt1) < lepWJJPt1MIN ) continue;
    if( fabs(vars.lepWJJ_pt2) < lepWJJPt2MIN ) continue;
    if( fabs(vars.lepWJJ_pt3) < lepWJJPt3MIN ) continue;
    
    
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
    
    
    if( fabs(vars.lepNuW_m) < lepNuWMMIN ) continue;    
    if( fabs(vars.lepNuW_m) > lepNuWMMAX ) continue;
    
    
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
    
    
    if( fabs(deltaEta(vars.tagJ1.eta(),vars.tagJ2.eta())) < tagJJDetaMIN ) continue;
    if( (vars.tagJ1+vars.tagJ2).mass() < tagJJMassMIN ) continue;
    if( std::max(vars.tagJ1.Et(),vars.tagJ2.Et()) < tagJJMaxEtMIN ) continue;
    if( std::min(vars.tagJ1.Et(),vars.tagJ2.Et()) < tagJJMinEtMIN ) continue;
    if( std::max(fabs(vars.tagJ1.eta()),fabs(vars.tagJ2.eta())) < tagJJMaxEtaMIN ) continue;
    if( std::min(fabs(vars.tagJ1.eta()),fabs(vars.tagJ2.eta())) < tagJJMinEtaMIN ) continue;
    
    
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
    
    
    if( fabs(vars.lep_zepp) > lepZeppMAX ) continue;
    if( std::max(fabs(vars.WJ1_zepp),fabs(vars.WJ2_zepp)) > WJJMaxZeppMAX ) continue;
    if( std::min(fabs(vars.WJ1_zepp),fabs(vars.WJ2_zepp)) > WJJMinZeppMAX ) continue;
    
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
