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


int main(int argc, char** argv)
{
  
  // Parse the config file
  parseConfigFile (argv[1]) ;
  
  std::string inputFileList  = gConfigParser -> readStringOption("Input::inputFileList");
  std::string inputTree  = gConfigParser -> readStringOption("Input::inputTree");
  std::string dataEra    = gConfigParser -> readStringOption("Input::dataEra");
  std::string outputFolder  = gConfigParser -> readStringOption("Input::outputFolder");

  int runMin = gConfigParser -> readIntOption("Options::runMin");
  int runMax = gConfigParser -> readIntOption("Options::runMax");
  int HLTperiod = gConfigParser -> readIntOption("Options::HLTperiod");

  // Open tree
  TChain* chain = new TChain(inputTree.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  
  // initialize histograms
  int nBins = 200;
  float jetPtMin = 0;
  float jetPtMax = 200;
   
  TH1F* h_num25_all = new TH1F("h_num25_all","h_num25_all",nBins,jetPtMin,jetPtMax);
  TH1F* h_den_all = new TH1F("h_den_all","h_den_all",nBins,jetPtMin,jetPtMax);
  TH1F* h_eff25_all = new TH1F("h_eff25_all","h_eff25_all",nBins,jetPtMin,jetPtMax);  
  // divide everything in eta bins
  const int nEtaBins = 4;
  TH1F* h_num25[nEtaBins];
  TH1F* h_den[nEtaBins];
  TH1F* h_eff25[nEtaBins];  
  for ( int iEta = 0; iEta < nEtaBins; iEta++ ) {
  
    TString s_iEta = "";
    s_iEta += iEta;
    h_num25[iEta] = new TH1F("h_num25_"+s_iEta,"h_num25_"+s_iEta,nBins,jetPtMin,jetPtMax);
    h_den[iEta] = new TH1F("h_den_"+s_iEta,"h_den_"+s_iEta,nBins,jetPtMin,jetPtMax);
    h_eff25[iEta] = new TH1F("h_eff25_"+s_iEta,"h_eff25_"+s_iEta,nBins,jetPtMin,jetPtMax);
  
  }
  
  std::string outfileName = outputFolder + "/" + dataEra + "_" + inputTree + "_JetTurnOn.root";
  TFile* outfile = new TFile(outfileName.c_str(),"RECREATE");
  
  // initialize variables
  // HLT variables
  std::vector<std::string>* HLT_Names = new std::vector<std::string>;
  std::vector<float>* HLT_Accept = new std::vector<float>;
  chain -> SetBranchAddress("HLT_Names", &HLT_Names);
  chain -> SetBranchAddress("HLT_Accept", &HLT_Accept);
  
  // Met variables
  float met_et;
  chain-> SetBranchAddress("met_et", &met_et);
  
  // Jet variables
  ROOT::Math::XYZTVector* p_jet1;
  ROOT::Math::XYZTVector* p_jet2;
  ROOT::Math::XYZTVector* p_jet3;
  ROOT::Math::XYZTVector* p_jet4;
  ROOT::Math::XYZTVector* p_jet5;
  ROOT::Math::XYZTVector* p_jet6;
   
  int jet1_hltMatched;
  int jet2_hltMatched;
  int jet3_hltMatched;
  int jet4_hltMatched;
  int jet5_hltMatched;
  int jet6_hltMatched;
  
  float jet1_hltMatchPt;
  float jet2_hltMatchPt;
  float jet3_hltMatchPt;
  float jet4_hltMatchPt;
  float jet5_hltMatchPt;
  float jet6_hltMatchPt;
 

  p_jet1 = new ROOT::Math::XYZTVector; 
  p_jet2 = new ROOT::Math::XYZTVector; 
  p_jet3 = new ROOT::Math::XYZTVector; 
  p_jet4 = new ROOT::Math::XYZTVector; 
  p_jet5 = new ROOT::Math::XYZTVector; 
  p_jet6 = new ROOT::Math::XYZTVector; 

  chain-> SetBranchAddress("jet1", &p_jet1);
  chain-> SetBranchAddress("jet2", &p_jet2);
  chain-> SetBranchAddress("jet3", &p_jet3);
  chain-> SetBranchAddress("jet4", &p_jet4);
  chain-> SetBranchAddress("jet5", &p_jet5);
  chain-> SetBranchAddress("jet6", &p_jet6);

  chain-> SetBranchAddress("jet1_hltMatched", &jet1_hltMatched);
  chain-> SetBranchAddress("jet2_hltMatched", &jet2_hltMatched);
  chain-> SetBranchAddress("jet3_hltMatched", &jet3_hltMatched);
  chain-> SetBranchAddress("jet4_hltMatched", &jet4_hltMatched);
  chain-> SetBranchAddress("jet5_hltMatched", &jet5_hltMatched);
  chain-> SetBranchAddress("jet6_hltMatched", &jet6_hltMatched);

  chain-> SetBranchAddress("jet1_hltMatchPt", &jet1_hltMatchPt);
  chain-> SetBranchAddress("jet2_hltMatchPt", &jet2_hltMatchPt);
  chain-> SetBranchAddress("jet3_hltMatchPt", &jet3_hltMatchPt);
  chain-> SetBranchAddress("jet4_hltMatchPt", &jet4_hltMatchPt);
  chain-> SetBranchAddress("jet5_hltMatchPt", &jet5_hltMatchPt);
  chain-> SetBranchAddress("jet6_hltMatchPt", &jet6_hltMatchPt);

  
  // Other variables
  int lep_flavour;
  chain-> SetBranchAddress("lep_flavour", &lep_flavour);

  ROOT::Math::XYZTVector* lep = new ROOT::Math::XYZTVector; 
  chain-> SetBranchAddress("lep", &lep);
  
  int runId;
  chain-> SetBranchAddress("runId", &runId);

  // ---------------------- REF and TEST HLT -------------------------- //
  std::vector<std::string> refHLTPathNames;
  std::vector<std::string> testHLT25PathNames;

  // MC   
  if ( HLTperiod == -1 ) {
    refHLTPathNames.push_back("HLT_Ele25_CaloIdL_CaloIsoVL_TrkIdVL_TrkIsoVL_v5");
    testHLT25PathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_v1");
  }

  // 11A
  if ( HLTperiod == 1 ) {
    refHLTPathNames.push_back("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v3");
    refHLTPathNames.push_back("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v4");
    refHLTPathNames.push_back("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v5");
    refHLTPathNames.push_back("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v6");
    refHLTPathNames.push_back("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v6");
    refHLTPathNames.push_back("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v7");
    
    testHLT25PathNames.push_back("HLT_Ele17_CaloIdVT_TrkIdT_CentralJet30_CentralJet25_v1");
    testHLT25PathNames.push_back("HLT_Ele17_CaloIdVT_TrkIdT_CentralJet30_CentralJet25_v4");
    testHLT25PathNames.push_back("HLT_Ele22_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_v2");
    testHLT25PathNames.push_back("HLT_Ele22_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_v3");
    testHLT25PathNames.push_back("HLT_Ele22_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_v3");
    testHLT25PathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_v1");
  }
  	
  // 11B no PF - normal
  if ( HLTperiod == 2 ) {
    refHLTPathNames.push_back("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v7");
    testHLT25PathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_v1");
  }

  // 11B no PF - backup
  if ( HLTperiod == 3 ) {
    refHLTPathNames.push_back("HLT_Ele32_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_v7");
    testHLT25PathNames.push_back("HLT_Ele27_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30_CentralJet25_v1");
  }

  // 11B with PF
  if ( HLTperiod == 4 ) {
    refHLTPathNames.push_back("HLT_Ele32_WP70_v2");
    refHLTPathNames.push_back("HLT_Ele32_WP70_v3");
    testHLT25PathNames.push_back("HLT_Ele27_WP80_DiCentralPFJet25_v4");
    testHLT25PathNames.push_back("HLT_Ele27_WP80_DiCentralPFJet25_v5");
  }
  
  bool refHLT, testHLT25;

  // LOOP OVER THE EVENTS
  std::cout << ">>>>> VBFAnalysis::Read " << chain -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < chain -> GetEntries() ; ++entry)
  {
    //***********
    // DUMP EVENT
    chain -> GetEntry(entry);
    if( entry == chain -> GetEntries() ) break;
    
    // skip runs outside the interesting HLT Era
    if ( runId < runMin || runId > runMax ) continue;
        
    // skip muons
    if ( lep_flavour == 13 ) continue;

    //===================== Dump relevant HLT info ====================//
    refHLT = false;
    testHLT25 = false;
    for(unsigned int HLTit = 0; HLTit < HLT_Names->size(); ++HLTit) {
    
      std::string thisHLTname = HLT_Names->at(HLTit);
      if ( (int) HLT_Accept->at(HLTit) == 0 ) continue;
      //REF 
      for ( unsigned int iRef = 0; iRef < refHLTPathNames.size(); iRef++ )
        if ( thisHLTname == refHLTPathNames.at(iRef) ) refHLT = true; 
      //TEST25 
      for ( unsigned int iTest = 0; iTest < testHLT25PathNames.size(); iTest++ )
        if ( thisHLTname == testHLT25PathNames.at(iTest) ) testHLT25 = true; 
    
    }
    
    //===================== Dump relevant Jet info ====================//
    std::vector<float> jets_pt;
    std::vector<float> jets_eta;
    std::vector<float> jets_match_pt;
    std::vector<int> jets_match;
    if ( fabs(p_jet1 -> eta()) < 2.4 ) {
      jets_pt.push_back( p_jet1 -> pt() );
      jets_eta.push_back( p_jet1 -> eta() );
      jets_match_pt.push_back( jet1_hltMatchPt );
      jets_match.push_back( jet1_hltMatched );
    }
    if ( fabs(p_jet2 -> eta()) < 2.4 ) {
      jets_pt.push_back( p_jet2 -> pt() );
      jets_eta.push_back( p_jet2 -> eta() );
      jets_match_pt.push_back( jet2_hltMatchPt );
      jets_match.push_back( jet2_hltMatched );
    }
    if ( fabs(p_jet3 -> eta()) < 2.4 ) {
      jets_pt.push_back( p_jet3 -> pt() );
      jets_eta.push_back( p_jet3 -> eta() );
      jets_match_pt.push_back( jet3_hltMatchPt );
      jets_match.push_back( jet3_hltMatched );
    }
    if ( fabs(p_jet4 -> eta()) < 2.4 ) {
      jets_pt.push_back( p_jet4 -> pt() );
      jets_eta.push_back( p_jet4 -> eta() );
      jets_match_pt.push_back( jet4_hltMatchPt );
      jets_match.push_back( jet4_hltMatched );
    }
    if ( fabs(p_jet5 -> eta()) < 2.4 ) {
      jets_pt.push_back( p_jet5 -> pt() );
      jets_eta.push_back( p_jet5 -> eta() );
      jets_match_pt.push_back( jet5_hltMatchPt );
      jets_match.push_back( jet5_hltMatched );
    }
    if ( fabs(p_jet6 -> eta()) < 2.4 ) {
      jets_pt.push_back( p_jet6 -> pt() );
      jets_eta.push_back( p_jet6 -> eta() );
      jets_match_pt.push_back( jet6_hltMatchPt );
      jets_match.push_back( jet6_hltMatched );
    }
       

    //===================== UNbias the sample ====================//
    if ( jets_pt.size() < 2 ) continue;
    if ( !refHLT ) continue;
    if ( lep->pt() < 33. ) continue;

    if ( jets_pt.at(0) < 40. ) continue;
    if ( jets_pt.size() > 2 && jets_pt.at(2) > 10. ) continue;

    // Find the second jet eta bin
    int etaBin = -1; 
    if ( jets_eta.at(1) > -2.4 && jets_eta.at(1) < -1. ) etaBin = 0;
    if ( jets_eta.at(1) > -1.0 && jets_eta.at(1) < 0. ) etaBin = 1;
    if ( jets_eta.at(1) > 0. && jets_eta.at(1) < 1. ) etaBin = 2;
    if ( jets_eta.at(1) > 1. && jets_eta.at(1) < 2.4 ) etaBin = 3;
    
    //===================== Turn on calculation ====================//
    h_den_all -> Fill ( jets_pt.at(1) ) ;
    h_den[etaBin] -> Fill ( jets_pt.at(1) ) ;

    if ( testHLT25 ) {
      h_num25_all -> Fill ( jets_pt.at(1) );
      h_num25[etaBin] -> Fill ( jets_pt.at(1) ) ;
    }
    
  }
  
  //===================== Turn on calculations ====================//
  outfile -> cd();
  h_num25_all -> Write();
  h_den_all -> Write();
  h_eff25_all -> Divide(h_num25_all,h_den_all);
  h_eff25_all -> Write();
  for ( int iEta = 0; iEta < nEtaBins; iEta++ ) {
    h_num25[iEta] -> Write();
    h_den[iEta] -> Write();
    h_eff25[iEta] -> Divide(h_num25[iEta],h_den[iEta]);
    h_eff25[iEta] -> Write();
  }
    
}
