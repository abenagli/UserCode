#include "VBFAnalysisVariables.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "analysisUtils.h"
#include "PUUtils.h"
#include "readJSONFile.h"
#include "HelicityLikelihoodDiscriminant.h"
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
bool AcceptHLTPath(const std::vector<std::string>&, const std::vector<float>&, const std::string&, bool& pathFound);






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
  std::string jsonFileName   = gConfigParser -> readStringOption("Input::jsonFileName");
  std::string pileupFileName = gConfigParser -> readStringOption("Input::pileupFileName");
  std::string MVAWeightsFile = gConfigParser -> readStringOption("Input::MVAWeightsFile");
  
  std::string outputRootFilePath = gConfigParser -> readStringOption("Output::outputRootFilePath");
  std::string outputRootFileName = gConfigParser -> readStringOption("Output::outputRootFileName");  
  
  int entryFIRST = gConfigParser -> readIntOption("Options::entryFIRST");
  int entryMAX = gConfigParser -> readIntOption("Options::entryMAX");
  int entryMODULO = gConfigParser -> readIntOption("Options::entryMODULO");
  int firstSTEP = gConfigParser -> readIntOption("Options::firstSTEP");
  float crossSection = gConfigParser -> readFloatOption("Options::crossSection");
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
  float WJJDRMIN = gConfigParser -> readFloatOption("Cuts::WJJDRMIN");
  float WJJDRMAX = gConfigParser -> readFloatOption("Cuts::WJJDRMAX");
  float WJJDetaMIN = gConfigParser -> readFloatOption("Cuts::WJJDetaMIN");
  float WJJDetaMAX = gConfigParser -> readFloatOption("Cuts::WJJDetaMAX");
  float WJJDphiMIN = gConfigParser -> readFloatOption("Cuts::WJJDphiMIN");
  float WJJDphiMAX = gConfigParser -> readFloatOption("Cuts::WJJDphiMAX");
  
  float WPtMIN = gConfigParser -> readFloatOption("Cuts::WPtMIN");
  
  float lepMetWDphiMIN = gConfigParser -> readFloatOption("Cuts::lepMetWDphiMIN");
  float lepMetWDphiMAX = gConfigParser -> readFloatOption("Cuts::lepMetWDphiMAX");
  float lepNuWMMIN = gConfigParser -> readFloatOption("Cuts::lepNuWMMIN");
  float lepNuWMMAX = gConfigParser -> readFloatOption("Cuts::lepNuWMMAX");

  float lepWJJPt1MIN = gConfigParser -> readFloatOption("Cuts::lepWJJPt1MIN");  
  float lepWJJPt2MIN = gConfigParser -> readFloatOption("Cuts::lepWJJPt2MIN");
  float lepWJJPt3MIN = gConfigParser -> readFloatOption("Cuts::lepWJJPt3MIN");
  
  float lepCthetaMIN = gConfigParser -> readFloatOption("Cuts::lepCthetaMIN");
  float lepCthetaMAX = gConfigParser -> readFloatOption("Cuts::lepCthetaMAX");
  float WJ1CthetaMIN = gConfigParser -> readFloatOption("Cuts::WJ1CthetaMIN");
  float WJ1CthetaMAX = gConfigParser -> readFloatOption("Cuts::WJ1CthetaMAX");
  float lepNuCthetaMIN = gConfigParser -> readFloatOption("Cuts::lepNuCthetaMIN");
  float lepNuCthetaMAX = gConfigParser -> readFloatOption("Cuts::lepNuCthetaMAX");
  float lepWJ1DphiMIN = gConfigParser -> readFloatOption("Cuts::lepWJ1DphiMIN");
  float lepWJ1DphiMAX = gConfigParser -> readFloatOption("Cuts::lepWJ1DphiMAX");
  
  int massDependentCUTS = gConfigParser -> readIntOption("Cuts::massDependentCUTS");
  int doPDFstudy          = gConfigParser -> readIntOption("Options::doPDFstudy"); 
  
  float MVAMIN = gConfigParser -> readFloatOption("Cuts::MVAMIN");
  
  std::map<int, int> totalEvents = GetTotalEvents("events", inputFileList.c_str()); 
  
  
  std::cout << ">>>>> VBFAnalysis::inputFileList = " << inputFileList << std::endl;
  
  
  
  
  
  
  // Open tree
  std::string treeName = "ntu";
  TChain* chain = new TChain(treeName.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  
  TH1F* distrPU_MC = GetTotalHisto("nPUit", inputFileList.c_str());
  distrPU_MC -> Scale(1./distrPU_MC->Integral());
  TFile* inFile_pileup = TFile::Open(pileupFileName.c_str(),"READ");
  TH1F* distrPU_DATA = (TH1F*)( inFile_pileup->Get("pileup") );
  
  
  // define map with events
  std::map<std::pair<int,std::pair<int,int> >,int> eventsMap;  
  
  
  // define out file names
  std::string outputRootFullFileName = outputRootFilePath+outputRootFileName+"_"+jetAlgorithm+".root";
  TFile* outputRootFile = new TFile(outputRootFullFileName.c_str(), "RECREATE");
  outputRootFile -> cd();
  
  
  
  // define event histogram
  int nStep = 16;
  
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
  
  for(int step = 1; step <= nStep; ++step)
  {
    if( (step < firstSTEP) && (step != 2) ) continue;
    
    char treeName[50];
    sprintf(treeName, "ntu_%d", step);
    cloneTrees[step] = chain -> CloneTree(0);
    cloneTrees[step] -> SetName(treeName); 
    AddVBFAnalysisTreeBranches(vars,cloneTrees[step]);
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
  
  std::vector<float> HLTLumi_e;
  std::vector<float> HLTLumi_mu;
  
  std::pair<int,int> dummyHLTRunRanges;
  std::vector<std::pair<int,int> > HLTRunRanges_e;
  std::vector<std::pair<int,int> > HLTRunRanges_mu;
  
  std::vector<std::string> dummyHLTPathNames;
  std::vector<std::vector<std::string> > HLTPathNames_e_DATA;
  std::vector<std::vector<std::string> > HLTPathNames_mu_DATA;
  
  std::vector<std::vector<std::string> > HLTPathNames_e_MC;
  std::vector<std::vector<std::string> > HLTPathNames_mu_MC;
  
  
  //----------------
  // data - electron
  
  HLTLumi_e.push_back(0.);
  
  // 0-th
  HLTLumi_e.push_back(HLTLumi_e.at(0)+216.12);
  dummyHLTRunRanges.first = 160404; dummyHLTRunRanges.second = 163869; HLTRunRanges_e.push_back(dummyHLTRunRanges);
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v1");
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v2");
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3");
  HLTPathNames_e_DATA.push_back(dummyHLTPathNames);
  
  // 1-th
  HLTLumi_e.push_back(HLTLumi_e.at(1)+2248.51);
  dummyHLTRunRanges.first = 165088; dummyHLTRunRanges.second = 176309; HLTRunRanges_e.push_back(dummyHLTRunRanges);
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  dummyHLTPathNames.push_back("HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v4");
  dummyHLTPathNames.push_back("HLT_Ele22_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v1");
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v1");
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  HLTPathNames_e_DATA.push_back(dummyHLTPathNames);
  
  // 2-th
  HLTLumi_e.push_back(HLTLumi_e.at(2)+1221.00);
  dummyHLTRunRanges.first = 176461; dummyHLTRunRanges.second = 178162; HLTRunRanges_e.push_back(dummyHLTRunRanges);
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_Ele30_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30_PFMHT25_v1");
  HLTPathNames_e_DATA.push_back(dummyHLTPathNames);
  
  // 3-th
  HLTLumi_e.push_back(HLTLumi_e.at(3)+551.07);
  dummyHLTRunRanges.first = 178420; dummyHLTRunRanges.second = 180252; HLTRunRanges_e.push_back(dummyHLTRunRanges);
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_Ele27_WP80_DiCentralPFJet25_PFMHT15_v4");
  dummyHLTPathNames.push_back("HLT_Ele27_WP80_DiCentralPFJet25_PFMHT15_v5");
  HLTPathNames_e_DATA.push_back(dummyHLTPathNames);
  
  
  //------------
  // data - muon
  
  HLTLumi_mu.push_back(0.);
  
  // 0-th
  HLTLumi_mu.push_back(HLTLumi_mu.at(0)+1908.31);
  dummyHLTRunRanges.first = 160404; dummyHLTRunRanges.second = 173198; HLTRunRanges_mu.push_back(dummyHLTRunRanges);
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_IsoMu24_v1");
  dummyHLTPathNames.push_back("HLT_IsoMu24_v2");
  dummyHLTPathNames.push_back("HLT_IsoMu24_v4");
  dummyHLTPathNames.push_back("HLT_IsoMu24_v5");
  dummyHLTPathNames.push_back("HLT_IsoMu24_v6");
  dummyHLTPathNames.push_back("HLT_IsoMu24_v7");
  dummyHLTPathNames.push_back("HLT_IsoMu24_v8");
  HLTPathNames_mu_DATA.push_back(dummyHLTPathNames);
  
  // 1-th
  HLTLumi_mu.push_back(HLTLumi_mu.at(1)+2328.39);
  dummyHLTRunRanges.first = 173236; dummyHLTRunRanges.second = 180252; HLTRunRanges_mu.push_back(dummyHLTRunRanges);
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_IsoMu24_eta2p1_v3");
  dummyHLTPathNames.push_back("HLT_IsoMu24_eta2p1_v6");
  dummyHLTPathNames.push_back("HLT_IsoMu24_eta2p1_v7");
  HLTPathNames_mu_DATA.push_back(dummyHLTPathNames);
  
  
  //--------------
  // mc - electron
  
  // 0-th
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_Ele15_CaloIdT_CaloIsoVL_TrkIdT_TrkIsoVL_v2");
  HLTPathNames_e_MC.push_back(dummyHLTPathNames);
  
  // 1-th
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  HLTPathNames_e_MC.push_back(dummyHLTPathNames);
  
  // 2-th
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  HLTPathNames_e_MC.push_back(dummyHLTPathNames);
  
  // 3-th
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_PFMHT20_v2");
  HLTPathNames_e_MC.push_back(dummyHLTPathNames);
  
  
  //----------
  // mc - muon
  
  // 0-th
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_IsoMu24_v9");
  HLTPathNames_mu_MC.push_back(dummyHLTPathNames);
  
  // 1-th
  dummyHLTPathNames.clear();
  dummyHLTPathNames.push_back("HLT_Mu24_eta2p1_v1");
  HLTPathNames_mu_MC.push_back(dummyHLTPathNames);
  
  
  
  
  
  
  // define the quark-gluon likelihood
  //  QGLikelihoodCalculator* qglikeli = new QGLikelihoodCalculator();
  // define the angular likelihood
  HelicityLikelihoodDiscriminant* helicitylikeli = new HelicityLikelihoodDiscriminant();  
  
  
  
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
  stepNames[1]  = "1) all events";
  stepNames[2]  = "2) preselection";
  stepNames[3]  = "3) HLT";
  stepNames[4]  = "4) lepton ID+iso";
  stepNames[5]  = "5) lepton pt/eta";
  stepNames[6]  = "6) lepton mt";
  stepNames[7]  = "7) met";
  stepNames[8]  = "8) W-jets pt";
  stepNames[9]  = "9) b-tag veto";
  stepNames[10] = "10) jet veto";
  stepNames[11] = "11) lep-met angles";
  stepNames[12] = "12) WJ1-WJ2 angles";
  stepNames[13] = "13) W pt";
  stepNames[14] = "14) kinematic fit";
  stepNames[15] = "15) W mass";
  stepNames[16] = "16) Higgs mass";
  
  
  
  
  
  
  //*********************
  // LOOP OVER THE EVENTS
  std::cout << ">>>>> VBFAnalysis::Read " << chain -> GetEntries() << " entries" << std::endl;  
  TRandom3 r;
  for(int entry = entryFIRST ; entry < chain -> GetEntries() ; ++entry)
  {
    //***********
    // DUMP EVENT
    chain -> GetEntry(entry);
    if( entry == entryMAX ) break;
    if( ((entry%entryMODULO) == 0) || (verbosity == 1) ) std::cout << ">>>>> VBFAnalysis::GetEntry " << entry << std::endl;   
    
    
    
        
    
    
    //*******************
    // SET SOME VARIABLES
    InitializeVBFAnalysisTree(vars);
    
    vars.totEvents = stepEvents[1];
    vars.lep = *(vars.p_lep);
    vars.met = *(vars.p_met);
    vars.nu = *(vars.p_nu);
    vars.nu1 = *(vars.p_nu1);
    vars.nu2 = *(vars.p_nu2);
    vars.lepNu = *(vars.p_lep) + *(vars.p_nu);
    vars.leadingJ = *(vars.p_leadingJ);
    vars.WJ1 = *(vars.p_WJ1);
    vars.WJ2 = *(vars.p_WJ2);
    vars.WJJ = *(vars.p_WJ1) + *(vars.p_WJ2);
    vars.tagJ1 = *(vars.p_tagJ1);
    vars.tagJ2 = *(vars.p_tagJ2);
    vars.thirdJ = *(vars.p_thirdJ);

    //    if (doPDFstudy > 0) vars.PDF_weights = *(vars.p_PDF_weights) ;

    GetLNuJJAngles(vars.lepNuW_cphi,vars.lepNuZ_cphi,vars.lep_ctheta,vars.WJ1_ctheta,vars.lepNu_ctheta,
                   vars.lep,vars.nu,vars.WJ1,vars.WJ2);
    
    vars.PUWeight = PURescaleFactor(distrPU_DATA,distrPU_MC,vars.PUit_n,0);
    
    
    //*********************************
    // JSON FILE AND DUPLIACTES IN DATA
    
    bool skipEvent = false;
    if( vars.dataFlag == 1 )
    {
      if(AcceptEventByRunAndLumiSection(vars.runId, vars.lumiId, jsonMap) == false) skipEvent = true;      
      
      std::pair<int,Long64_t> eventLSandID(vars.lumiId, vars.eventId);
      std::pair<int,std::pair<int,Long64_t> > eventRUNandLSandID(vars.runId, eventLSandID);
      if( eventsMap[eventRUNandLSandID] == 1 ) skipEvent = true;
      else eventsMap[eventRUNandLSandID] = 1;
    }
    
    if( skipEvent == true ) continue;
    
    
    
    
    
    
    //**********************
    // STEP 2 - preselection
    step = 2;
    //SetStepNames(stepNames, "preselection", step, verbosity);
    
    
    if( (leptonFLAVOUR == "e")  && (vars.lep_flavour != 11) ) continue;
    if( (leptonFLAVOUR == "mu") && (vars.lep_flavour != 13) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step == 2 ) cloneTrees[step] -> Fill();
        
    
    
    
    
    
    //*************
    // STEP 3 - HLT
    step += 1;
    //SetStepNames(stepNames, "HLT", step, verbosity);
    
    
    bool pathFound = false;
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
    

    unsigned int HLTPeriod = -1;
    std::vector<float> HLTLumi;
    std::vector<std::pair<int,int> > HLTRunRanges;
    std::vector<std::string> HLTPathNames;
    
    
    // on data
    if( vars.dataFlag == 1 )
    {
      if( vars.lep_flavour == 11 ) HLTRunRanges = HLTRunRanges_e;
      if( vars.lep_flavour == 13 ) HLTRunRanges = HLTRunRanges_mu;
      
      for(HLTPeriod = 0; HLTPeriod < HLTRunRanges.size(); ++HLTPeriod)
        if( (vars.runId >= (HLTRunRanges.at(HLTPeriod)).first) &&
            (vars.runId <= (HLTRunRanges.at(HLTPeriod)).second) )
          break;
      
      if( vars.lep_flavour == 11 ) HLTPathNames = HLTPathNames_e_DATA.at(HLTPeriod);
      if( vars.lep_flavour == 13 ) HLTPathNames = HLTPathNames_mu_DATA.at(HLTPeriod);
      
      for(unsigned int HLTIt = 0; HLTIt < HLTPathNames.size(); ++HLTIt)
      {
        if( AcceptHLTPath(*HLT_Names,*HLT_Accept,HLTPathNames.at(HLTIt),pathFound) == true )
          skipEvent = false;
      }
    }
    
    
    // on mc
    else
    {
      if( vars.lep_flavour == 11 ) HLTLumi = HLTLumi_e;
      if( vars.lep_flavour == 13 ) HLTLumi = HLTLumi_mu;
      
      float dummyLumi = r.Uniform(0.,4236.7);
      
      for(HLTPeriod = 0; HLTPeriod < HLTLumi.size()-1; ++HLTPeriod)
        if( (dummyLumi >= HLTLumi.at(HLTPeriod)) &&
            (dummyLumi <=  HLTLumi.at(HLTPeriod+1) ) )
          break;
      
      if( vars.lep_flavour == 11 ) HLTPathNames = HLTPathNames_e_MC.at(HLTPeriod);
      if( vars.lep_flavour == 13 ) HLTPathNames = HLTPathNames_mu_MC.at(HLTPeriod);
      
      for(unsigned int HLTIt = 0; HLTIt < HLTPathNames.size(); ++HLTIt)
      {
        if( AcceptHLTPath(*HLT_Names,*HLT_Accept,HLTPathNames.at(HLTIt),pathFound) == true )
          skipEvent = false;
      }
    }
    
    
    
    if( pathFound == false )
      std::cout << ">>>>>>>>> AcceptHLTPath::Warning: no paths found in the HLT menu" << std::endl;  
    
    if( (HLTCUT == 1) && (skipEvent == true) ) continue;
    
    
    
    //// electron trigger emulation
    if( HLTCUT == 1 )
    {
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
      
      if( (vars.lep_flavour == 11) && (vars.lep.pt() < 30.) ) continue;
      if( (vars.lep_flavour == 11) && (HLTPeriod == 2) && (vars.lep.pt() < 33.) ) continue;
      if( (vars.lep_flavour == 11) && (fabs(vars.lep.eta()) > 2.5) ) continue;
      
      if( (vars.lep_flavour == 11) && (metCUT == 1) && (HLTPeriod == 1) && (vars.met.Et() < 30.) ) continue;
      if( (vars.lep_flavour == 11) && (metCUT == 1) && (HLTPeriod == 2) && (vars.met.Et() < 35.) ) continue;
      if( (vars.lep_flavour == 11) && (metCUT == 1) && (HLTPeriod == 3) && (vars.met.Et() < 30.) ) continue;
    }
    
    //// muon trigger emulation
    //if( HLTCUT == 1 )
    //{
    //  if( (vars.lep_flavour == 13) && (vars.lep.pt() < 25.) ) continue;
    //  if( (vars.lep_flavour == 13) && (fabs(vars.lep.eta()) > 2.1) ) continue;
    //  //if( (vars.lep_flavour == 13) && (isoCUT == 1) && (vars.lep_tkIso  > 1.000) ) continue;
    //  //if( (vars.lep_flavour == 13) && (isoCUT == 1) && (vars.lep_emIso  > 1.000) ) continue;
    //  //if( (vars.lep_flavour == 13) && (isoCUT == 1) && (vars.lep_hadIso > 1.000) ) continue;
    //}
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
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
      
      //if( fabs(vars.lep_dxy_PV)             > muDxyMAX )                      isId = false;
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
    
    
    
    // vertex compatibility
    bool is3DIP = true;
    
    if( vars.lep_flavour == 11 )
      if( fabs(vars.lep_dxy_PV/vars.lep_edxy_PV) > ele3DipMAX ) is3DIP = false;
    
    if( vars.lep_flavour == 13 )
      if( fabs(vars.lep_dxy_PV/vars.lep_edxy_PV) > mu3DipMAX ) is3DIP = false;
    
    //if( is3DIP == false ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //***********************
    // STEP 5 - lepton pt/eta
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
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP ) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //*******************
    // STEP 6 - lepton mt
    step += 1;
    //SetStepNames(stepNames, "lepton mt", step, verbosity);
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (lepMetMtCUT == 1) && (vars.lepMet_mt < lepMetMtMIN) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) ) && (lepMetMtCUT == 1) && (vars.lepNu_m > 110.) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP ) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //****************************
    // STEP 7 - Initial cuts - met
    step += 1;
    //SetStepNames(stepNames, "met", step, verbosity);
    
    
    if( metCUT == 1 )
    {
      if( vars.met.Et() < metEtMIN ) continue;
      if( vars.met.Et() > metEtMAX ) continue;
    }
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
       
    
    
    
    
    //**********************************
    // STEP 8 - Initial cuts - W-jets pt
    step += 1;
    //SetStepNames(stepNames, "W-jet pt", step, verbosity);
    
    
    if( (vars.WJ1.pt() <= 30.) || (vars.WJ2.pt() <= 30.) ) continue;
    if( std::max(vars.WJ1.pt(), vars.WJ2.pt()) < WJJMaxPtMIN ) continue;
    if( std::min(vars.WJ1.pt(), vars.WJ2.pt()) < WJJMinPtMIN ) continue;
    
    /*
    TF1* f_metTurnOn = new TF1("f_metTurnOn"," ([0] + ([1]-[0])/2 * (1 +TMath::Erf((x-[2])/([3]*sqrt(2)))))",0.,1000.);
    f_metTurnOn -> SetParameter(0,3.06396e-01);
    f_metTurnOn -> SetParameter(1,9.93169e-01);
    f_metTurnOn -> SetParameter(2,2.02097e+01);
    f_metTurnOn -> SetParameter(3,1.20391e+01);
    
    TF2* f_jetTurnOn = new TF2("j_jetTurnOn","(([0] + ([1]-[0])/2 * (1 +TMath::Erf((x-[2])/([3]*sqrt(2)))))) * ([0] + ([1]-[0])/2 * (1 +TMath::Erf((y-[2])/([3]*sqrt(2)))))");
    f_jetTurnOn -> SetParameter(0,2.28966e-01);
    f_jetTurnOn -> SetParameter(1,9.53513e-01);
    f_jetTurnOn -> SetParameter(2,2.66816e+01);
    f_jetTurnOn -> SetParameter(3,7.17539e+00);
    
    if(vars.dataFlag != 1)
      vars.eventWeight *= f_metTurnOn -> Eval(vars.met_et) * f_jetTurnOn -> Eval(vars.WJ1.pt(),vars.WJ2.pt());
    */
    
    
    // Fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //************************************
    // STEP 9 - Initial cuts - b-tag veto
    step += 1;
    //SetStepNames(stepNames, "b-tag veto", step, verbosity);
    
    
    bool isBTagged = false;
    bool isAntiBTagged = true;
    
    
    // standard b-tag veto
    if( vars.nBTag_TCHEM_pt30 > 0 ) isBTagged = true;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (vars.nJets_cnt_pt30 > 2) && (isBTagged == true) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) ) && (isBTagged == true) ) continue;
    
    
    // b-tag selection
    //if( vars.nBTag_TCHEM_pt30 == 1 ) isBTagged = true;
    //if( ( (trainMVA == 0) && (applyMVA == 0) ) && (isBTagged == false) ) continue;
    //
    //if( vars.WJ1_bTag > 3.30 ) continue;
    //if( vars.WJ2_bTag > 3.30 ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    
    //*********************************
    // STEP 10 - Initial cuts - jet veto
    step += 1;
    //SetStepNames(stepNames, "jet veto", step, verbosity);
    
    
    if( vars.nJets_cnt_pt30 > 3 ) continue;
    
    
    // Fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //****************************************
    // STEP 11 - Initial cuts - lep-met angles
    step += 1;
    //SetStepNames(stepNames, "lep-met angles", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (fabs(vars.lepMet_Dphi) < lepMetDphiMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (fabs(vars.lepMet_Dphi) > lepMetDphiMAX) ) continue;
    
    
    // fill distributions    
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //****************************************
    // STEP 12 - Initial cuts - WJ1-WJ2 angles
    step += 1;
    //SetStepNames(stepNames, "WJ1-WJ2 angle cuts", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && ( fabs(deltaR(vars.WJ1.eta(),vars.WJ1.phi(),vars.WJ2.eta(),vars.WJ2.phi())) < WJJDRMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && ( fabs(deltaR(vars.WJ1.eta(),vars.WJ1.phi(),vars.WJ2.eta(),vars.WJ2.phi())) > WJJDRMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && ( fabs(deltaEta(vars.WJ1.eta(),vars.WJ2.eta())) < WJJDetaMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && ( fabs(deltaEta(vars.WJ1.eta(),vars.WJ2.eta())) > WJJDetaMAX) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && ( fabs(deltaPhi(vars.WJ1.phi(),vars.WJ2.phi())) < WJJDphiMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && ( fabs(deltaPhi(vars.WJ1.phi(),vars.WJ2.phi())) > WJJDphiMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    ////******************************************
    //// STEP XX - Initial cuts - pt max/met  cuts
    //step += 1;
    ////SetStepNames(stepNames, "pt max cuts", step, verbosity);
    //
    //
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (fabs(vars.lepWJJ_pt1) < lepWJJPt1MIN) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (fabs(vars.lepWJJ_pt2) < lepWJJPt2MIN) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (fabs(vars.lepWJJ_pt3) < lepWJJPt3MIN) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) && (metCUT == 1) ) && (vars.met.Et() < metEtMIN) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) && (metCUT == 1) ) && (vars.met.Et() > metEtMAX) ) continue;
    //
    //
    //// fill distributions
    //stepEvents[step] += 1;
    //stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    //if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    //if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    //if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    //if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    //
    //if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //*******************************
    // STEP 13 - Initial cuts - W pt
    step += 1;
    //SetStepNames(stepNames, "W pt", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && ( (vars.WJ1+vars.WJ2).Pt() < WPtMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && ( (vars.lep+vars.met).Pt() < WPtMIN) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //***************************************
    // STEP 14 - Initial cuts - Kinematic Fit
    step += 1;
    //SetStepNames(stepNames, "Kinematic Fit", step, verbosity);
    
    
    DoKinematicFit(vars,0.1,"MIB");
    
    
    //    if( vars.WJ1.pt() > 0. )
    //      vars.WJ1_QGLikelihood = qglikeli -> computeQGLikelihoodPU( vars.WJ1.Pt(),vars.rhoForIsolation,vars.WJ1_chargedMultiplicity,vars.WJ1_neutralMultiplicity,vars.WJ1_ptD );
    //    if( vars.WJ2.pt() > 0. )
    //      vars.WJ2_QGLikelihood = qglikeli -> computeQGLikelihoodPU( vars.WJ2.Pt(),vars.rhoForIsolation,vars.WJ2_chargedMultiplicity,vars.WJ2_neutralMultiplicity,vars.WJ2_ptD );
    if( vars.WJ2.pt() > vars.WJ1.pt() )
    {
      float QGLikelihoodDummy = vars.WJ2_QGLikelihood;
      vars.WJ2_QGLikelihood = vars.WJ1_QGLikelihood;
      vars.WJ1_QGLikelihood = QGLikelihoodDummy;
    }
    
    TLorentzVector lep_tlv(vars.lep_KF.Px(),vars.lep_KF.Py(),vars.lep_KF.Pz(),vars.lep_KF.E());
    TLorentzVector nu_tlv ( vars.nu_KF.Px(), vars.nu_KF.Py(), vars.nu_KF.Pz(), vars.nu_KF.E());
    TLorentzVector WJ1_tlv(vars.WJ1_KF.Px(),vars.WJ1_KF.Py(),vars.WJ1_KF.Pz(),vars.WJ1_KF.E());
    TLorentzVector WJ2_tlv(vars.WJ2_KF.Px(),vars.WJ2_KF.Py(),vars.WJ2_KF.Pz(),vars.WJ2_KF.E());
        
    HelicityLikelihoodDiscriminant::HelicityAngles hangles;
    if( vars.lep_charge < 0 ) hangles = helicitylikeli -> computeHelicityAngles(lep_tlv,nu_tlv,WJ1_tlv,WJ2_tlv);
    else                      hangles = helicitylikeli -> computeHelicityAngles(nu_tlv,lep_tlv,WJ1_tlv,WJ2_tlv);
    helicitylikeli -> setMeasurables(hangles);
    double sProb = helicitylikeli -> getSignalProbability();
    double bProb = helicitylikeli -> getBkgdProbability();
    vars.helicityLikelihood = sProb/(sProb+bProb);  
    
    
    //if( vars.helicityLikelihood < 0.6 ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    ////*****************************************
    //// STEP XX - Initial cuts - Helicity angles
    //step += 1;
    ////SetStepNames(stepNames, "Helicity angles", step, verbosity);
    //
    //
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (vars.lep_ctheta < lepCthetaMIN) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (vars.lep_ctheta > lepCthetaMAX) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (vars.WJ1_ctheta < WJ1CthetaMIN) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (vars.WJ1_ctheta > WJ1CthetaMAX) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (vars.lepNu_ctheta < lepNuCthetaMIN) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (vars.lepNu_ctheta > lepNuCthetaMAX) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (std::min(vars.WJ1_lep_Dphi,vars.WJ2_lep_Dphi) < lepWJ1DphiMIN) ) continue;
    //if( ( (trainMVA == 0) && (applyMVA == 0) && (massDependentCUTS == 1) ) && (std::min(vars.WJ1_lep_Dphi,vars.WJ2_lep_Dphi) > lepWJ1DphiMAX) ) continue;
    //
    //
    //// fill distributions
    //stepEvents[step] += 1;
    //stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    //if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    //if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    //if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    //if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    //
    //if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //********************************
    // STEP 15 - Initial cuts - W mass
    step += 1;
    //SetStepNames(stepNames, "W mass", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (vars.WJ1+vars.WJ2).mass() < WJJMassMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && ( (vars.WJ1+vars.WJ2).mass() > WJJMassMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
    if( vars.lep_charge > 0. ) stepEvents_plus_int[step] += 1;
    if( vars.lep_charge < 0. ) stepEvents_minus_int[step] += 1;
    if( vars.lep_charge > 0. ) (stepEvents_plus[vars.nJets])[step] += 1;
    if( vars.lep_charge < 0. ) (stepEvents_minus[vars.nJets])[step] += 1;
    
    if( step >= firstSTEP) cloneTrees[step] -> Fill();
    
    
    
    
    
    
    //************************************
    // STEP 16 - Initial cuts - Higgs mass
    step += 1;
    //SetStepNames(stepNames, "Higgs mass cut", step, verbosity);
    
    
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepNuW_m_KF) < lepNuWMMIN) ) continue;
    if( ( (trainMVA == 0) && (applyMVA == 0) ) && (fabs(vars.lepNuW_m_KF) > lepNuWMMAX) ) continue;
    
    
    // fill distributions
    stepEvents[step] += 1;
    stepEvents_PURescaled[step] += vars.eventWeight * vars.PUWeight;
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
  
  
  
  for(step = 1; step <= nStep; ++step)
  {
    if( (step < firstSTEP) && (step != 2) ) continue;
    cloneTrees[step] -> AutoSave();
  } 
  
  
  
  distrPU_MC -> Write();
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
                   const std::string& HLTPathName,
                   bool& pathFound)
{
  bool acceptEvent = false;
  
  for(unsigned int HLTIt = 0; HLTIt < HLT_Names.size(); ++HLTIt)
  {
    if( HLT_Names.at(HLTIt) == HLTPathName )
    {
      pathFound = true;
      if( HLT_Accept.at(HLTIt) == 1 )
        acceptEvent = true;
    }
  }
  
  return acceptEvent;
}
