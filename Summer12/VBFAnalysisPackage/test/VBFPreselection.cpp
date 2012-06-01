#include "VBFPreselectionVariables.h"
#include "HiggsPtKFactors.h"
#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"


#include <iomanip>
#include <algorithm>

#include "TH1F.h"
#include "TH2F.h"
#include "TRandom3.h"



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
  int correctMet          = gConfigParser -> readIntOption("Options::correctMet");
  float JESScaleVariation = gConfigParser -> readFloatOption("Options::JESScaleVariation");
  int ttSelection         = gConfigParser -> readIntOption("Options::ttSelection"); 
  int QCDSelection        = gConfigParser -> readIntOption("Options::QCDSelection"); 
  int doTnP               = gConfigParser -> readIntOption("Options::doTnP"); 
  int doPDFstudy          = gConfigParser -> readIntOption("Options::doPDFstudy");
  float intLumi           = gConfigParser -> readFloatOption("Input::intLumi");
    
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
  
  stepName = "LeptonsFilter"+jetAlgorithm+"Events/totalEvents";
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
  
  TH1F* nPUtrue      = GetTotalHisto("PUDumper/nPUtrue",     inputFileList.c_str());
  TH1F* nPUit        = GetTotalHisto("PUDumper/nPUit",       inputFileList.c_str());
  TH1F* nPUoot_early = GetTotalHisto("PUDumper/nPUoot_early",inputFileList.c_str());
  TH1F* nPUoot_late  = GetTotalHisto("PUDumper/nPUoot_late", inputFileList.c_str());
  
  TH1F* events = new TH1F("events","events",nStep,0.,1.*nStep);
  std::map<int, int> stepEvents;
  std::map<int, std::string> stepNames;
  
  
  
  // define variable container
  VBFPreselectionVariables vars;
  InitializeVBFPreselectionTree(vars, outputRootFullFileName, doTnP);
  vars.mH = atof(higgsMass.c_str());
  vars.dataFlag = dataFlag;
  vars.MCFlag = MCFlag;
  vars.totEvents = totalEvents[1];
  vars.crossSection = crossSection;
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
  TRandom3 r;
  
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
    
    SetGenVariables(vars, reader, dataFlag, verbosity);
    SetPUVariables(vars, reader, dataFlag, verbosity);
    SetHLTVariables(vars, reader, verbosity);
    if (doPDFstudy) SetPDFVariables(vars, reader, verbosity);
    SetPVVariables(vars, reader, verbosity);
    
        
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
      
      float chIso  = reader.GetFloat("electrons_chargedHadronIso")->at(eleIt); 
      float nhIso  = reader.GetFloat("electrons_neutralHadronIso")->at(eleIt);
      float phIso  = reader.GetFloat("electrons_photonIso")->at(eleIt);
      float puIso  = reader.GetFloat("electrons_PUIso")->at(eleIt);
      float effArea = GetEffArea(11, etaSC);
      // EA correction
      float combIso = chIso + std::max((double) (nhIso + phIso - vars.rhoForIsolationPFlow * effArea), 0.);
      
      // // DB correction
      // float combIso = chIso + std::max((double) (nhIso + phIso - 0.5 * puIso), 0.);
            
      float mvaTrig = reader.GetFloat("electrons_mvaTrig")->at(eleIt);
      float mvaNonTrig = reader.GetFloat("electrons_mvaNonTrig")->at(eleIt);
      bool isWP70       = GetEleId(pt,etaSC,mvaTrig   ,combIso,70,true);  //tag lepton
      bool isWP90noTrig = GetEleId(pt,etaSC,mvaNonTrig,combIso,90,false); //veto lepton
      bool isWP95noIso  = GetEleId(pt,etaSC,mvaTrig   ,-1     ,95,true);  //qcd lepton

      /*std::cout << "event --> " << vars.eventId << "\n"
                << "iEle  --> " << eleIt << "\n"
                << "chIso --> " << chIso << "\n"
                << "nhIso --> " << nhIso << "\n" 
                << "phIso --> " << phIso << "\n" 
                << "rho   --> " << vars.rhoForIsolationPFlow << "\n" 
                << "eta   --> " << etaSC << "\n" 
                << "pt    --> " << pt << "\n" 
                << "EA    --> " << effArea << "\n" 
                << "mvaTri--> " << mvaTrig << "\n" 
                << "mvaNonTri--> " << mvaNonTrig << "\n" 
                << std::endl;*/

      float z   = reader.GetFloat("electrons_z")->at(eleIt);      
      float dxy = reader.GetFloat("electrons_dB")->at(eleIt);
      
      int isEB = reader.GetInt("electrons_isEB")->at(eleIt);
      float sigmaIetaIeta = reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt);
      float DphiIn = fabs(reader.GetFloat("electrons_deltaPhiIn")->at(eleIt));
      float DetaIn = fabs(reader.GetFloat("electrons_deltaEtaIn")->at(eleIt));
      float HOverE = reader.GetFloat("electrons_hOverE")->at(eleIt);
      
      // Old WP if old ntuples (signals)  
      bool isNewNT = false;
      int pos = -1;
      pos = outputRootFilePath.find("PAT_52X");
      if ( pos >= 0 ) isNewNT = true; 
      if ( !isNewNT ) {
      
        bool isTight = true;
        bool isLoose = true;
        
        // barrel
        if( isEB == 1 )
        {
          if( sigmaIetaIeta  > 0.01 ) isTight = false;
          if( DphiIn         > 0.03 ) isTight = false;
          if( DetaIn         > 0.004) isTight = false;
          if( HOverE         > 0.12 ) isTight = false;
          if( combIso/pt     > 0.10 ) isTight = false;

          if( sigmaIetaIeta  > 0.01 ) isLoose = false;
          if( DphiIn         > 0.06 ) isLoose = false;
          if( DetaIn         > 0.004) isLoose = false;
          if( HOverE         > 0.12 ) isLoose = false;
          if( combIso/pt     > 0.15 ) isLoose = false;
        }
        // endcap
        if( isEB != 1 )
        {
          if( sigmaIetaIeta  > 0.03 ) isTight = false;
          if( DphiIn         > 0.02 ) isTight = false;
          if( DetaIn         > 0.005) isTight = false;
          if( HOverE         > 0.10 ) isTight = false;
          if( combIso/pt     > 0.10 ) isTight = false;

          if( sigmaIetaIeta  > 0.03 ) isLoose = false;
          if( DphiIn         > 0.03 ) isLoose = false;
          if( DetaIn         > 0.007) isLoose = false;
          if( HOverE         > 0.10 ) isLoose = false;
          if( combIso/pt     > 0.15 ) isLoose = false;
        }

        isWP70       = isTight;
        isWP90noTrig = isLoose;
      
      }

            
      int mishits = reader.GetInt("electrons_mishits")->at(eleIt);
      float dist = reader.GetFloat("electrons_dist")->at(eleIt);
      float dcot = reader.GetFloat("electrons_dcot")->at(eleIt);
      
      //NOTALEO
      //int passConvVeto = reader.GetInt("electrons_passConvVeto")->at(eleIt);
      bool passConvVeto = false;
      if ( ( mishits == 0 ) && ( ( fabs(dist) > 0.02 ) || ( fabs(dcot) > 0.02 ) ) ) passConvVeto = true;
      
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
          ( fabs(z-vars.PV_z) < 0.1 ) && 
          ( fabs(dxy) < 0.02 ) &&
          ( passConvVeto ) &&
          ( (QCDSelection == 0 && isWP70 ) ||   // standard
            (QCDSelection == 1 && isWP95noIso && combIso/pt > 0.1 )) // QCD selection
        )
      {
        isTightElectron = true;
        SetElectronVariables(vars, reader, eleIt, verbosity);

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
          ( fabs(z-vars.PV_z) < 0.2 ) && 
          ( fabs(dxy) < 0.04 ) &&
          ( passConvVeto ) &&
	        ( isWP90noTrig )
	      )
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
      
      float chIso  = reader.GetFloat("muons_chargedHadronIso")->at(muIt); 
      float nhIso  = reader.GetFloat("muons_neutralHadronIso")->at(muIt);
      float phIso  = reader.GetFloat("muons_photonIso")->at(muIt);
      float puIso  = reader.GetFloat("muons_PUIso")->at(muIt);
      float effArea = GetEffArea(13, eta);
      
      // // EA correction
      // float combIso = chIso + std::max((double) (nhIso + phIso - vars.rhoForIsolationPFlowMuons * effArea), 0.);
      // DB correction
      float combIso = chIso + std::max((double) (nhIso + phIso - 0.5 * puIso), 0.);
      
      int tracker    = reader.GetInt("muons_tracker")->at(muIt);
      int standalone = reader.GetInt("muons_standalone")->at(muIt);
      int global     = reader.GetInt("muons_global")->at(muIt);
      int PF         = reader.GetInt("muons_pfMuon")->at(muIt);
      
      float z                        = reader.GetFloat("muons_z")->at(muIt);
      float dxy                      = reader.GetFloat("muons_dB")->at(muIt);
      float normalizedChi2           = reader.GetFloat("muons_normalizedChi2")->at(muIt);
      int numberOfMatches            = reader.GetInt("muons_numberOfMatches")->at(muIt);
      int numberOfValidTrackerHits   = reader.GetInt("muons_numberOfValidTrackerHits")->at(muIt);
      int numberOfValidMuonHits      = reader.GetInt("muons_numberOfValidMuonHits")->at(muIt);
      int numberOfValidPixelHits      = reader.GetInt("muons_numberOfValidPixelHits")->at(muIt);
      int pixelLayersWithMeasurement = reader.GetInt("muons_pixelLayersWithMeasurement")->at(muIt);
      int trackerLayersWithMeasurement = reader.GetInt("muons_trackerLayersWithMeasurement")->at(muIt);

      /*std::cout << "event --> " << vars.eventId << "\n"
                << "iMu   --> " << muIt << "\n"
                << "chIso --> " << chIso << "\n"
                << "nhIso --> " << nhIso << "\n" 
                << "phIso --> " << phIso << "\n" 
                << "PUIso --> " << puIso << "\n" 
                << "eta   --> " << eta   << "\n" 
                << "pt    --> " << pt << "\n" 
                << "global--> " << global << "\n"
                << "PF    --> " << PF << "\n"
                << std::endl;*/

      
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
          ( (QCDSelection == 0 && combIso/pt < 0.12)  ||   // standard
            (QCDSelection == 1 && combIso/pt > 0.12) ) &&   // QCD selection
          ( fabs(z-vars.PV_z) < 0.5 ) && 
          ( fabs(dxy) < 0.02 ) &&
          ( global == 1 ) &&
          ( PF == 1 ) &&
          ( normalizedChi2 < 10. ) &&
          ( numberOfValidMuonHits > 0 ) &&
          ( numberOfMatches > 1 ) && 
          ( numberOfValidPixelHits > 0 ) &&
          ( trackerLayersWithMeasurement > 5 )
        )
      {
        isTightMuon = true;
        SetMuonVariables(vars, reader, muIt, verbosity);
        
        nLep += 1;
        nMu  += 1;
      }
      
      
      // ---------------
      // loose selection
      if( isTightMuon == true ) continue;
      
      bool isLooseMuon = false;
      if( ( pt > 10. ) &&
          ( fabs(eta) < 2.5 ) &&
      	  ( combIso/pt < 0.20 ) &&
          ( global == 1 || tracker == 1 )  &&
          ( PF == 1 )
        )
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
    SetLeptonVariables(vars, reader, verbosity);
    
    if( doTnP == 1 )
    {
      std::vector<int> blacklistIt_lep;
      blacklistIt_lep.push_back(vars.selectIt_lep);
      
      vars.selectIt_lep2 = SelectLepton(vars.leptons, "maxPt", 20.,&blacklistIt_lep);
      if(vars.selectIt_lep2 == -1) continue;
      SetLepton2Variables(vars, reader, verbosity);
      if( ((vars.lep+vars.lep2).mass() < 50.) || ((vars.lep+vars.lep2).mass() > 130.) ) continue;
    }
    
    // fIll event counters
    stepEvents[step] += 1;
    
    
    
    
    
    
    //*******************
    // STEP 9 - muon veto
    step += 1;
    SetStepNames(stepNames, "muon veto", step, verbosity);

    if( nMu_loose > 0 ) continue;
    if( (leptonFLAVOUR == "e") && (nMu > 0) && (QCDSelection == 0) ) continue;
    if( (leptonFLAVOUR == "mu") && (doTnP == 0) && (nMu > 1) && (QCDSelection == 0) ) continue;
    
    
    // fIll event counters
    stepEvents[step] += 1;
    
    
    
    
    
    
    //***********************
    // STEP 10 - electron veto
    step += 1;
    SetStepNames(stepNames, "electron veto", step, verbosity);
    
    if( nEle_loose > 0 ) continue;
    if( (leptonFLAVOUR == "e") && (doTnP == 0) && (nEle > 1) && (QCDSelection == 0) ) continue;
    if( (leptonFLAVOUR == "mu") && (nEle > 0) && (QCDSelection == 0) ) continue;
    
    
    // fIll event counters
    stepEvents[step] += 1;
    
    
    //*************************
    // STEP 11 -  >= 2 cnt jets
    step += 1;
    char stepName[50]; 
    sprintf(stepName, ">= %d cnt jet(s)", nJetCntMIN);
    SetStepNames(stepNames, std::string(stepName), step, verbosity);
    
    //*****************
    // met and neutrino
    std::string run;
    if( vars.dataFlag == 1 )
    {
      if( vars.runId <= 191930 ) run = "2012A";
      else                       run = "2012B";
      
    }
    if( vars.dataFlag == 0 )
    {
      float val = r.Uniform(0.,intLumi);
      if( val <= 531.579 ) run = "2012A";
      else                 run = "2012B";
    }
    
    SetMetVariables(vars, reader, jetType, correctMet, run, JESScaleVariation, JECUncertainty, verbosity);
    
    //****************
    SetBTagVariables(vars, reader, jetType, jetEtaCNT, verbosity);
    
    
    //************
    // select jets

    if( verbosity == 1)
      std::cout << "jet begin" << std::endl;
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      
      ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
      
      // jet et min
      if( jet.pt() < 15. ) continue;
      
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
        if( (reader.GetFloat("jets_neutralHadronEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) + reader.GetInt("jets_neutralMultiplicity")->at(jetIt) <= 1) ) continue;
        if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_chargedHadronEnergyFraction")->at(jetIt) <= 0.) ) continue;
        if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_chargedEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
        if( (fabs(jet.eta()) < 2.4) && (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) <= 0) ) continue;
      }
      
      SetJetVariables(vars, reader, jetIt, jetType, jetEtaCNT, jetEtaFWD, JESScaleVariation, JECUncertainty, verbosity);
      
    } // loop on jets
    if( verbosity == 1)
      std::cout << "jet end" << std::endl;
    
    std::sort(vars.jets_ptOrdered.begin(), vars.jets_ptOrdered.end(), maggiore);
    std::sort(vars.jets_bTagOrdered.begin(), vars.jets_bTagOrdered.end(), maggiore);
      if( vars.jets_bTagOrdered.size() > 0 )
    vars.jets_bTag1 = vars.jets_bTagOrdered.at(0);
      if( vars.jets_bTagOrdered.size() > 1 )
    vars.jets_bTag2 = vars.jets_bTagOrdered.at(1);
    

    
    
    //**************
    // >= n cnt jets 
    if( vars.nJets_cnt_pt30 < nJetCntMIN ) continue;
    
    if( vars.nJets_cnt > 0 )
      SetLeadingJetVariables(vars, reader, jetEtaCNT, verbosity);
    
    
    // fIll event counters
    stepEvents[step] += 1;
    
    
    
    
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
        if( ttSelection == 0 )
          SelectWJets(vars.selectIt_W, vars.jets, WSelectionMETHOD, jetPtMIN, jetEtaCNT, 5, 9999.);
        else
          SelectWJets(vars.selectIt_W, vars.jets, WSelectionMETHOD, jetPtMIN, jetEtaCNT, 5, 9999., 0, &(vars.jets_bTag));
        
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

          if( ttSelection == 0 )
            SelectWJets(vars.selectIt_W, vars.jets, WSelectionMETHOD, jetPtMIN, jetEtaCNT, 5, 9999.);
          else
            SelectWJets(vars.selectIt_W, vars.jets, WSelectionMETHOD, jetPtMIN, jetEtaCNT, 5, 9999., 0, &(vars.jets_bTag));
        }
        else
        {
          if( ttSelection == 0 )
            SelectWJets(vars.selectIt_W, vars.jets, WSelectionMETHOD, jetPtMIN, jetEtaCNT, 5, 9999.);
          else
            SelectWJets(vars.selectIt_W, vars.jets, WSelectionMETHOD, jetPtMIN, jetEtaCNT, 5, 9999., 0, &(vars.jets_bTag));
          
          std::vector<int> blacklistIt_W;
          blacklistIt_W.push_back(vars.selectIt_W.at(0));
          blacklistIt_W.push_back(vars.selectIt_W.at(1));
          SelectTagJets(vars.selectIt_tag, vars.jets, tagSelectionMETHOD, jetPtMIN, 0., 0., &blacklistIt_W);
        }
      }
      
      std::sort(vars.selectIt_W.begin(), vars.selectIt_W.end());
      std::sort(vars.selectIt_tag.begin(), vars.selectIt_tag.end());
      
      SetWJJVariables(vars, reader, verbosity);
      SetTagJJVariables(vars, reader, verbosity);
      SetThirdJetVariables(vars, reader, verbosity);
      SetHVariables(vars, reader, verbosity);
      if(vars.mH > 0.) SetMCVariables(vars, reader, verbosity);
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
  
  nPUtrue      -> Write();
  nPUit        -> Write();
  nPUoot_early -> Write();
  nPUoot_late  -> Write();
  
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
    std::cout << ">>>>>> " << stepNames[step] << std::endl;
}
