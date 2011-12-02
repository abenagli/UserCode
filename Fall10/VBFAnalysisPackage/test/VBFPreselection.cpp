#include "VBFPreselectionVariables.h"
#include "HiggsPtKFactors.h"
#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"


#include <iomanip>

#include "TH1F.h"
#include "TH2F.h"



void SetStepNames(std::map<int, std::string>&, const std::string&, const int&, const int&);






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
  
  std::string inputFileList      = gConfigParser -> readStringOption("Input::inputFileList");
  std::string jetAlgorithm       = gConfigParser -> readStringOption("Input::jetAlgorithm");
  std::string jetType            = gConfigParser -> readStringOption("Input::jetType");
  std::string higgsMass          = gConfigParser -> readStringOption("Input::higgsMass");
  std::string JECUncertaintyFile = gConfigParser -> readStringOption("Input::JECUncertaintyFile");
    
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  std::string outputRootFullFileName = outputRootFilePath + "/" + outputRootFileName + "_" + jetAlgorithm + ".root";
  
  int entryFIRST          = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX            = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO         = gConfigParser -> readIntOption("Options::entryMODULO");
  int verbosity           = gConfigParser -> readIntOption("Options::verbosity");
  int dataFlag            = gConfigParser -> readIntOption("Options::dataFlag");
  int MCFlag              = gConfigParser -> readIntOption("Options::MCFlag");
  float crossSection      = gConfigParser -> readFloatOption("Options::crossSection");
  int TMVA4JetTraining    = gConfigParser -> readIntOption("Options::TMVA4JetTraining");
  float JESScaleVariation = gConfigParser -> readFloatOption("Options::JESScaleVariation");
  
  
  int nJetCntMIN    = gConfigParser -> readIntOption("Cuts::nJetCntMIN");
  float lepJetDRMIN = gConfigParser -> readFloatOption("Cuts::lepJetDRMIN");
  float jetPtMIN    = gConfigParser -> readFloatOption("Cuts::jetPtMIN");
  float jetEtaCNT   = gConfigParser -> readFloatOption("Cuts::jetEtaCNT");
  float jetEtaFWD   = gConfigParser -> readFloatOption("Cuts::jetEtaFWD");
  
  
  std::string WSelectionMETHOD   = gConfigParser -> readStringOption("Cuts::WSelectionMETHOD");
  std::string tagSelectionMETHOD = gConfigParser -> readStringOption("Cuts::tagSelectionMETHOD");
  int tagFIRST = gConfigParser -> readIntOption("Cuts::tagFIRST");
  
  int nLepMIN = gConfigParser -> readIntOption("Cuts::nLepMIN");
  
  std::string leptonFLAVOUR = gConfigParser -> readStringOption("Cuts::leptonFLAVOUR");  
  
  
  
  
  
  
  // Get number of events
  std::cout << ">>> VBFPreselection::Get number of events" << std::endl;
  std::string stepName;
  
  stepName = "AllEvents/totalEvents";
  std::map<int,int> totalEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  //stepName = "NonScrapedEvents/passedEvents";
  //std::map<int,int> nonScrapedEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  //stepName = "GoodVtxEvents/passedEvents";
  //std::map<int,int> goodVtxEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  stepName = "LeptonsFilterEvents/totalEvents";
  std::map<int,int> leptonFilterEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  stepName = "JetFilter"+jetAlgorithm+"Events/totalEvents";
  std::map<int,int> jetFilterEvents = GetTotalEvents(stepName.c_str(), inputFileList.c_str());  
  
  
  
  
  
  
  // Get the JEC uncertainty file, if needed
  TH2F* JECUncertainty = NULL; 
  if( JESScaleVariation != 0. )
  {
    TFile* temp = TFile::Open(JECUncertaintyFile.c_str(),"READ");
    JECUncertainty = (TH2F*)( temp -> Get("histo") );
  }
  
  
  
  
  
  
  // Open old tree
  std::cout << ">>> VBFPreselection::Open old tree" << std::endl;
  std::string treeName = "MiBiCommonNTOneLeptonTwoJets"+jetAlgorithm+"/SimpleNtuple";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  treeReader reader((TTree*)(chain), false);
  
  
  
  
  
  
  // define histograms
  std::cout << ">>> VBFPreselection::Define histograms" << std::endl;
  int nStep = 12;
  
  TH1F* nPU    = GetTotalHisto("PUDumper/nPU",   inputFileList.c_str());
  TH1F* nPUit  = GetTotalHisto("PUDumper/nPUit", inputFileList.c_str());
  TH1F* nPUoot = GetTotalHisto("PUDumper/nPUoot",inputFileList.c_str());
  
  TH1F* events = new TH1F("events","events",nStep,0.,1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepNames;
  
  
  
  // define variable container
  VBFPreselectionVariables vars;
  InitializeVBFPreselectionTree(vars, outputRootFullFileName);
  vars.mH = atof(higgsMass.c_str());
  vars.dataFlag = dataFlag;
  vars.MCFlag = MCFlag;
  vars.totEvents = totalEvents[1];
  vars.crossSection = crossSection;
  vars.TMVA4Jet = -1;
  vars.eventNaiveId = -1;
  
  
  
  
    
    
  //********************
  // STEP 1 - all events
  int step = 1;
  SetStepNames(stepNames, "All events", step, verbosity);
  stepEvents[step] = totalEvents[1];
  
                                                                                                                                                           
  
  //*********************
  // STEP 2 - no scraping
  step = 2;
  SetStepNames(stepNames, "Skim: no scraping", step, verbosity);
  //stepEvents[step] = nonScrapedEvents[1];
  stepEvents[step] = totalEvents[1];
  
  
  //*********************
  // STEP 3 - good vertex
  step = 3;
  SetStepNames(stepNames, "Skim: good vertex", step, verbosity);
  //stepEvents[step] = goodVtxEvents[1];
  stepEvents[step] = totalEvents[1];
  
  
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
    if( (entry%entryMODULO) == 0 ) std::cout << ">>>>> VBFPreselection::GetEntry " << entry << std::endl;   
    if( entry == entryMAX ) break;
    
    
    
    
    
    
    //*****************************
    // STEP 5 - SET EVENT VARIABLES
        
    vars.runId   = reader.GetInt("runId")->at(0);
    vars.lumiId  = reader.GetInt("lumiId")->at(0);
    
    vars.eventId = reader.GetLongLongInt("eventId")->at(0);
    
    vars.eventNaiveId += 1;
    
    vars.eventWeight = 1.;
    
    SetPUVariables(vars, reader, dataFlag);
    SetHLTVariables(vars, reader);
    SetPVVariables(vars, reader);
    
    //if( vars.eventId != 177044 ) continue;
    
    
        
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
    
    
    
    
    
    
    //********************
    // STEP 6 - HCAL noise
    step = 6;
    SetStepNames(stepNames, "HCAL noise", step, verbosity);
    
    
    //if( reader.GetInt("HCAL_noise")->at(0) == 0 ) continue;
    
    
    // fill event counters
    stepEvents[step] += 1;
    
    
    
    
    
    
    //*****************
    // STEP 7 - Good PV
    step += 1;
    SetStepNames(stepNames, "Good PV", step, verbosity);
    
    
    if( vars.PV_ndof     <  4  ) continue;
    if( fabs(vars.PV_z)  > 24. ) continue;
    if( fabs(vars.PV_d0) >  2. ) continue;
    
    
    // fill event counters
    stepEvents[step] += 1;
    
    
    
    
    
        
    //*********************
    // STEP 8 - >= 1 lepton
    step += 1;
    SetStepNames(stepNames, ">= 1 lepton", step, verbosity);
    
    int nLep = 0;
    int nEle = 0;
    int nMu = 0;
    
    int nLep_loose = 0;
    int nEle_loose = 0;
    int nMu_loose = 0;
    
    
    //------------------
    // loop on electrons
    if( verbosity == 1)
      std::cout << "ele begin" << std::endl;
    
    for(unsigned int eleIt = 0; eleIt < (reader.Get4V("electrons")->size()); ++eleIt)
    {
      float pt = reader.Get4V("electrons")->at(eleIt).pt();
      
      float eta = reader.Get4V("electrons")->at(eleIt).eta();
      float etaSC = reader.Get3PV("electrons_positionSC")->at(eleIt).eta();
      
      float tkIso  = reader.GetFloat("electrons_tkIsoR03")->at(eleIt); 
      float emIso  = reader.GetFloat("electrons_emIsoR03")->at(eleIt);
      float hadIso = reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt) + 
                     reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt);
      float combIso = tkIso + emIso + hadIso;
      
      float dxy = reader.GetFloat("electrons_dxy_BS")->at(eleIt);
      
      int isEB = reader.GetInt("electrons_isEB")->at(eleIt);
      float sigmaIetaIeta = reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt);
      float DphiIn = fabs(reader.GetFloat("electrons_deltaPhiIn")->at(eleIt));
      float DetaIn = fabs(reader.GetFloat("electrons_deltaEtaIn")->at(eleIt));
      float HOverE = reader.GetFloat("electrons_hOverE")->at(eleIt);
      
      int mishits = reader.GetInt("electrons_mishits")->at(eleIt);
      float dist = reader.GetFloat("electrons_dist")->at(eleIt);
      float dcot = reader.GetFloat("electrons_dcot")->at(eleIt);
      
      /*std::cout << "eleIt: " << eleIt
                << "   pt: " << pt
                << "   eta: " << eta
                << "   etaSC: " << etaSC
                << "   tkIso: " << tkIso
                << "   dxy: " << dxy
                << "   sigmaIetaIeta: " << sigmaIetaIeta
                << "   DphiIn: " << DphiIn
                << "   DetaIn: " << DetaIn
                << "   HOverE: " << HOverE
                << "   mishits: " << HOverE
                << "   dist: " << dist
                << "   dcot: " << dcot
                << std::endl;*/
      
      
      // ---------------
      // tight selection
      bool isTightElectron = false;
      if( ( pt > 20.)  &&
          ( fabs(eta) < 2.5 ) &&
          ( (fabs(etaSC) < 1.4442) || (fabs(etaSC) > 1.5660) ) &&
          ( fabs(dxy) < 0.02 ) &&
          ( tkIso/pt < 0.1 ) &&   // standard
          //( tkIso/pt < 0.5 ) &&    // loose for QCD studies
          ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&  // standard
          ( ( (isEB == 1) && (DphiIn < 0.060) )        || ( (isEB == 0) && (DphiIn < 0.030) ) ) &&         // standard
          ( ( (isEB == 1) && (DetaIn < 0.004) )        || ( (isEB == 0) && (DetaIn < 0.007) ) ) &&         // standard
          ( ( (isEB == 1) && (HOverE < 0.040) )        || ( (isEB == 0) && (HOverE < 0.025) ) ) &&         // standard
          //( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&   // loose for QCD studies
          //( ( (isEB == 1) && (DphiIn < 0.800) )        || ( (isEB == 0) && (DphiIn < 0.700) ) ) &&          // loose for QCD studies
          //( ( (isEB == 1) && (DetaIn < 0.007) )        || ( (isEB == 0) && (DetaIn < 0.010) ) ) &&          // loose for QCD studies
          //( ( (isEB == 1) && (HOverE < 0.150) )        || ( (isEB == 0) && (HOverE < 0.070) ) ) &&          // loose for QCD studies
          ( mishits == 0 ) &&
          ( ( fabs(dist) > 0.02 ) || ( fabs(dcot) > 0.02 ) ) )
      {
        isTightElectron = true;
        SetElectronVariables(vars, reader, eleIt);
        
        nLep += 1;
        nEle += 1;
      }
      
      
      // ---------------
      // loose selection
      if( isTightElectron == true ) continue;
      
      bool isLooseElectron = false;
      if( ( pt > 15. ) &&
          ( fabs(eta) < 2.5 ) &&
          ( (fabs(etaSC) < 1.4442) || (fabs(etaSC) > 1.5660) ) &&
	  ( tkIso/pt < 0.2) &&   // standard
	  //( tkIso/pt < 0.5) &&     // loose for QCD studies
          ( ( (isEB == 1) && (sigmaIetaIeta < 0.010) ) || ( (isEB == 0) && (sigmaIetaIeta < 0.030) ) ) &&
          ( ( (isEB == 1) && (DphiIn < 0.800) )        || ( (isEB == 0) && (DphiIn < 0.700) ) ) &&
          ( ( (isEB == 1) && (DetaIn < 0.007) )        || ( (isEB == 0) && (DetaIn < 0.010) ) ) &&
          ( ( (isEB == 1) && (HOverE < 0.150) )        || ( (isEB == 0) && (HOverE < 0.070) ) ) )
      {
        isLooseElectron = true;
	vars.electrons_loose.push_back( reader.Get4V("electrons")->at(eleIt) );
        
        nLep_loose += 1;
        nEle_loose += 1;
      }
      
      
    }
    
    if( verbosity == 1)    
      std::cout << "ele end" << std::endl;
    
    
    
    //--------------
    // loop on muons
    if( verbosity == 1)
      std::cout << "mu begin" << std::endl;
    
    for(unsigned int muIt = 0; muIt < (reader.Get4V("muons")->size()); ++muIt)
    {
      float pt = reader.Get4V("muons")->at(muIt).pt();
      float eta = reader.Get4V("muons")->at(muIt).eta();
      
      float tkIso  = reader.GetFloat("muons_tkIsoR03")->at(muIt); 
      float emIso  = reader.GetFloat("muons_emIsoR03")->at(muIt);
      float hadIso = reader.GetFloat("muons_hadIsoR03")->at(muIt);
      float combIso = tkIso + emIso + hadIso;
      
      int tracker    = reader.GetInt("muons_tracker")->at(muIt);
      int standalone = reader.GetInt("muons_standalone")->at(muIt);
      int global     = reader.GetInt("muons_global")->at(muIt);
      
      float z                      = reader.GetFloat("muons_z")->at(muIt);
      float dxy                    = reader.GetFloat("muons_dxy_BS")->at(muIt);
      float normalizedChi2         = reader.GetFloat("muons_normalizedChi2")->at(muIt);
      int numberOfMatches            = reader.GetInt("muons_numberOfMatches")->at(muIt);
      int numberOfValidTrackerHits   = reader.GetInt("muons_numberOfValidTrackerHits")->at(muIt);
      int numberOfValidMuonHits      = reader.GetInt("muons_numberOfValidMuonHits")->at(muIt);
      int pixelLayersWithMeasurement = reader.GetInt("muons_pixelLayersWithMeasurement")->at(muIt);
      
      /*std::cout << "muIt: " << muIt
                << "   pt: " << pt
                << "   eta: " << eta
                << "   combIso: " << combIso
                << "   tracker: " << tracker
                << "   standalone: " << standalone
                << "   global: " << global
                << "   z: " << z
                << "   dxy: " << dxy
                << "   normalizedChi2: " << normalizedChi2
                << "   numberOfMatches: " << numberOfMatches
                << "   numberOfValidTrackerHits: " << numberOfValidTrackerHits
                << "   numberOfValidMuonHits: " << numberOfValidMuonHits
                << "   pixelLayersWithMeasurement: " << pixelLayersWithMeasurement
                << std::endl;*/
      
            
      // ---------------
      // tight selection
      bool isTightMuon = false;
      if( ( pt > 20. ) &&
          ( fabs(eta) < 2.1 ) &&
	  ( tkIso/pt < 0.05 ) &&   // standard
	  //( tkIso/pt < 0.50 ) &&   // loose for QCD studies
          ( fabs(z-vars.PV_z) < 1. ) && 
          ( fabs(dxy) < 0.02 ) &&
          ( tracker == 1 ) &&
          //( standalone == 1 ) &&
          ( global == 1 ) &&
          ( normalizedChi2 < 10. ) &&
          ( pixelLayersWithMeasurement > 0 ) &&
          ( numberOfValidTrackerHits > 10 ) &&
          ( numberOfValidMuonHits > 0 ) &&
          ( numberOfMatches > 1 ) 
        ) 
      {
        isTightMuon = true;
        SetMuonVariables(vars, reader, muIt);
        
        nLep += 1;
        nMu  += 1;
      }
      
      
      // ---------------
      // loose selection
      if( isTightMuon == true ) continue;
      
      bool isLooseMuon = false;
      if( ( pt > 10. ) &&
          ( fabs(eta) < 2.5 ) &&
	  ( tkIso/pt < 0.20 ) &&
          ( global == 1) )
      {
        isLooseMuon = true;
        vars.muons_loose.push_back( reader.Get4V("muons")->at(muIt) );        
        
        nLep_loose += 1;
        nMu_loose  += 1;
      }
      
      
    } // loop on muons
    
    if( verbosity == 1)
      std::cout << "mu end" << std::endl;
    
    
    
    //std::cout << "nMu: " << nMu << "   nEle: " << nEle << "   nMu_loose: " << nMu_loose << "   nEle_loose: " << nEle_loose << std::endl;
    // >= 1 lepton
    if( nLep < nLepMIN ) continue;
    if( (leptonFLAVOUR == "e")   && ( nEle      < nLepMIN) ) continue;
    if( (leptonFLAVOUR == "mu")  && (      nMu  < nLepMIN) ) continue;
    if( (leptonFLAVOUR == "emu") && ((nEle+nMu) < nLepMIN) ) continue;
    if( (leptonFLAVOUR == "emu") && ((nEle+nMu) > nLepMIN) ) continue;
    
    
    // select lepton
    vars.selectIt_lep = SelectLepton(vars.leptons, "maxPt", 20.);
    if(vars.selectIt_lep == -1) continue;
    SetLeptonVariables(vars, reader);
    
    
    // fIll event counters
    stepEvents[step] += 1;
    
    
    
    
    
    
    //*******************
    // STEP 9 - muon veto
    
    step += 1;
    SetStepNames(stepNames, "muon veto", step, verbosity);

    if( nMu_loose > 0 ) continue;
    if( (leptonFLAVOUR == "e") && (nMu > 0) ) continue;
    if( (leptonFLAVOUR == "mu") && (nMu > 1) ) continue;
    
    
    // fIll event counters
    stepEvents[step] += 1;
    
    
    
    
    
    
    //***********************
    // STEP 10 - electron veto
    step += 1;
    SetStepNames(stepNames, "electron veto", step, verbosity);
    
    if( nEle_loose > 0 ) continue;
    if( (leptonFLAVOUR == "e") && (nEle > 1) ) continue;
    if( (leptonFLAVOUR == "mu") && (nEle > 0) ) continue;
    
    
    // fIll event counters
    stepEvents[step] += 1;
    
    
    
    
    
    
    /*if( leptonFLAVOUR == "mu")
    {
      std::cout << std::fixed << std::setprecision(6)
                << vars.eventId << " "
	        << vars.lep_pt << " "
	        << vars.lep_eta << " "
	        << vars.lep_tkIso << " "
	        << vars.lep_global << " "
	        << vars.lep_tracker << " "
	        << vars.lep_dxy_BS << " "
	        << fabs(vars.lep_z-vars.PV_z) << " "
	        << vars.lep_normalizedChi2 << " "
	        << vars.lep_numberOfMatches << " " 
	        << vars.lep_numberOfValidTrackerHits << " "
	        << vars.lep_numberOfValidMuonHits << " "
	        << vars.lep_pixelLayersWithMeasurement << " " 
	        << std::endl;    
    }
    
    if( leptonFLAVOUR == "e")
    {
      std::cout << std::fixed << std::setprecision(6)
                << vars.eventId << " "
	        << vars.lep_pt << " "
	        << vars.lep_eta << " "
	        << vars.lep_tkIso << " "
	        << vars.lep_dxy_BS << " "
	        << vars.lep_sigmaIetaIeta << " " 
	        << vars.lep_DphiIn << " " 
	        << vars.lep_DetaIn << " " 
	        << vars.lep_HOverE << " " 
	        << vars.lep_mishits << " " 
	        << vars.lep_dist << " " 
	        << vars.lep_dcot << " " 
	        << std::endl;
     }*/
    
    
    
    
    
    //*************************
    // STEP 11 -  >= 2 cnt jets
    step += 1;
    char stepName[50]; 
    sprintf(stepName, ">= %d cnt jet(s)", nJetCntMIN);
    SetStepNames(stepNames, std::string(stepName), step, verbosity);
    
    //*****************
    // met and neutrino
    SetMetVariables(vars, reader, jetType, JESScaleVariation, JECUncertainty);
    
    //****************
    SetBTagVariables(vars, reader, jetType, jetEtaCNT);
    
    
    //************
    // select jets

    if( verbosity == 1)
      std::cout << "jet begin" << std::endl;
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      
      // jet et min
      if( jet.pt() < jetPtMIN ) continue;
      
      // clean jets from selected lepton
      float DR = deltaR(jet.eta(), jet.phi(), vars.lep.eta(), vars.lep.phi());
      
      if( (leptonFLAVOUR == "e")  && (DR < lepJetDRMIN) ) continue;
      if( (leptonFLAVOUR == "emu") && (vars.lep_flavour == 11) && (DR < lepJetDRMIN) ) continue;
      
      if( (leptonFLAVOUR == "mu") && (DR < lepJetDRMIN) ) continue;
      if( (leptonFLAVOUR == "emu") && (vars.lep_flavour == 13) && (DR < lepJetDRMIN) ) continue;
      
      
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
        if( (fabs(jet.eta()) < 2.4) && (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) <= 0) ) continue;
        //if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_muonEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        
        if( (fabs(jet.eta()) >= 2.4) && (reader.GetFloat("jets_neutralHadronEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) >= 2.4) && (reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) >= 2.4) && (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) + reader.GetInt("jets_neutralMultiplicity")->at(jetIt) <= 1) ) continue;
      }
      
      SetJetVariables(vars, reader, jetIt, jetType, jetEtaCNT, jetEtaFWD, JESScaleVariation, JECUncertainty);
      
    } // loop on jets
    if( verbosity == 1)
      std::cout << "jet end" << std::endl;
    
    std::sort(vars.jets_ptOrdered.begin(), vars.jets_ptOrdered.end(), maggiore);
    std::sort(vars.jets_bTagOrdered.begin(), vars.jets_bTagOrdered.end(), maggiore);
      if( vars.jets_bTagOrdered.size() > 0 )
    vars.jets_bTag1 = vars.jets_bTagOrdered.at(0);
      if( vars.jets_bTagOrdered.size() > 1 )
    vars.jets_bTag2 = vars.jets_bTagOrdered.at(1);
    




    /*std::cout << std::fixed << std::setprecision(6)
              << "eventId: " << vars.eventId << " "
              << vars.lep_pt << " "
              << vars.lep_eta << " "
              << std::endl;
    
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      
      // jet et min
      if( jet.pt() < 10. ) continue;
      
      // clean jets from selected lepton
      float DR = deltaR(jet.eta(), jet.phi(), vars.lep.eta(), vars.lep.phi());
      
      if( (leptonFLAVOUR == "e")  && (DR < 0.3) ) continue;
      if( (leptonFLAVOUR == "emu") && (vars.lep_flavour == 11) && (DR < 0.3) ) continue;
      
      if( (leptonFLAVOUR == "mu") && (DR < 0.3) ) continue;
      if( (leptonFLAVOUR == "emu") && (vars.lep_flavour == 13) && (DR < 0.3) ) continue;
      
      
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
        if( (fabs(jet.eta()) < 2.4) && (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) <= 0) ) continue;
        //if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_muonEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        
        if( (fabs(jet.eta()) >= 2.4) && (reader.GetFloat("jets_neutralHadronEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) >= 2.4) && (reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) >= 2.4) && (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) + reader.GetInt("jets_neutralMultiplicity")->at(jetIt) <= 1) ) continue;
      }
      
      std::cout << std::fixed << std::setprecision(6)
                << ">>> jet: " << jetIt << " "
		<< jet.pt() << " "
		<< jet.eta() << " "
		<< jet.phi() << " "
		<< DR << " "
		<< std::endl;
      
    } // loop on jets
    */




    
    
    //**************
    // >= n cnt jets 
    if( vars.nJets_cnt < nJetCntMIN ) continue;
    
    SetLeadingJetVariables(vars, reader, jetEtaCNT);
    
    
    // fIll event counters
    stepEvents[step] += 1;
    
    
    
    
    
    
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
    
    
    
    
    
    
    //***********************
    // STEP 12 -  select jets
    step += 1;
    sprintf(stepName, "select jets");
    SetStepNames(stepNames, std::string(stepName), step, verbosity);
    
    
    
    //******************
    // select W/tag jets
    if( TMVA4JetTraining == 0 )
    {
      if( vars.nJets < 4 )
      {
        SelectWJets(vars.selectIt_W, vars.jets, WSelectionMETHOD, jetPtMIN, jetEtaCNT, 5, 9999.);
        
        std::vector<int> blacklistIt_W;
        blacklistIt_W.push_back(vars.selectIt_W.at(0));
        blacklistIt_W.push_back(vars.selectIt_W.at(1));
        SelectTagJets(vars.selectIt_tag, vars.jets, tagSelectionMETHOD, jetPtMIN, 0., 0., &blacklistIt_W);
      }
      
      if( vars.nJets >= 4 )
      {
        if( tagFIRST == 1)
        {
          SelectTagJets(vars.selectIt_tag, vars.jets, tagSelectionMETHOD, jetPtMIN, 0., 0.);
          
          std::vector<int> blacklistIt_tag;
          blacklistIt_tag.push_back(vars.selectIt_tag.at(0));
          blacklistIt_tag.push_back(vars.selectIt_tag.at(1));
          SelectWJets(vars.selectIt_W, vars.jets, WSelectionMETHOD, jetPtMIN, jetEtaCNT, 5., 9999., &blacklistIt_tag);
        }
        else
        {
          SelectWJets(vars.selectIt_W, vars.jets, WSelectionMETHOD, jetPtMIN, jetEtaCNT, 9, 9999.);
          
          std::vector<int> blacklistIt_W;
          blacklistIt_W.push_back(vars.selectIt_W.at(0));
          blacklistIt_W.push_back(vars.selectIt_W.at(1));
          SelectTagJets(vars.selectIt_tag, vars.jets, tagSelectionMETHOD, jetPtMIN, 0., 0., &blacklistIt_W);
        }
      }
      
      std::sort(vars.selectIt_W.begin(), vars.selectIt_W.end());
      std::sort(vars.selectIt_tag.begin(), vars.selectIt_tag.end());
      
      SetWJJVariables(vars, reader);
      SetTagJJVariables(vars, reader);
      SetThirdJetVariables(vars, reader);
      SetHVariables(vars, reader);
      if(vars.mH > 0.) SetMCVariables(vars, reader);
      if( (vars.mH > 0.) && (vars.mH < 1000.) ) vars.eventWeight = HiggsPtKFactors(vars.mc_H_pt,vars.mH);
      
      
      // fIll event counters
      stepEvents[step] += 1;
    }
    
    
    
    
    
    
    //***************************
    // FILL THE PRESELECTION TREE
    
    // Fill reduced tree
    FillVBFPreselectionTree(vars);
    
    
    
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
  
  nPU    -> Write();
  nPUit  -> Write();
  nPUoot -> Write();
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
