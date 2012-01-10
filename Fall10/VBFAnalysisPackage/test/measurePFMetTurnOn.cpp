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

  int runMin = gConfigParser -> readIntOption("Options::runMin");
  int runMax = gConfigParser -> readIntOption("Options::runMax");
  int HLTperiod = gConfigParser -> readIntOption("Options::HLTperiod");


  // Open tree
  TChain* chain = new TChain(inputTree.c_str());
  if(!FillChain(*chain, inputFileList.c_str())) return 1;
  
  // initialize histograms
  int nBins = 200;
  float metPtMin = 0;
  float metPtMax = 200;
   
  TH1F* h_num = new TH1F("h_num","h_num",nBins,metPtMin,metPtMax);
  TH1F* h_den = new TH1F("h_den","h_den",nBins,metPtMin,metPtMax);
  TH1F* h_eff = new TH1F("h_eff","h_eff",nBins,metPtMin,metPtMax);
  
  std::string outfileName = dataEra + "_" + inputTree + "_MetTurnOn.root";
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
  
  // Other variables
  int lep_flavour;
  chain-> SetBranchAddress("lep_flavour", &lep_flavour);

  ROOT::Math::XYZTVector* lep = new ROOT::Math::XYZTVector; 
  chain-> SetBranchAddress("lep", &lep);
  
  int runId;
  chain-> SetBranchAddress("runId", &runId);

  // REF and TEST HLT
  std::vector<std::string> refHLTPathNames;
  std::vector<std::string> testHLTPathNames;

  if ( HLTperiod == 4 ) {
    refHLTPathNames.push_back("HLT_Ele27_WP80_DiCentralPFJet25_v4");
    refHLTPathNames.push_back("HLT_Ele27_WP80_DiCentralPFJet25_v5");
    testHLTPathNames.push_back("HLT_Ele27_WP80_DiCentralPFJet25_PFMHT15_v4");
    testHLTPathNames.push_back("HLT_Ele27_WP80_DiCentralPFJet25_PFMHT15_v5");
  }
  
  bool refHLT, testHLT;

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
    testHLT = false;
    for(unsigned int HLTit = 0; HLTit < HLT_Names->size(); ++HLTit) {
    
      std::string thisHLTname = HLT_Names->at(HLTit);
      if ( (int) HLT_Accept->at(HLTit) == 0 ) continue;
      //REF 
      for ( unsigned int iRef = 0; iRef < refHLTPathNames.size(); iRef++ )
        if ( thisHLTname == refHLTPathNames.at(iRef) ) refHLT = true; 
      //TEST 
      for ( unsigned int iTest = 0; iTest < testHLTPathNames.size(); iTest++ )
        if ( thisHLTname == testHLTPathNames.at(iTest) ) testHLT = true; 
    
    }

    //===================== UNbias the sample ====================//
    if ( !refHLT ) continue;
    h_den -> Fill ( met_et ) ;

    //===================== Turn on calculation ====================//
    if ( !testHLT ) continue;
    h_num -> Fill ( met_et ) ;
    
  }
  
  //===================== Turn on calculations ====================//
  outfile -> cd();
  h_num -> Write();
  h_den -> Write();
  h_eff -> Divide(h_num,h_den);
  h_eff -> Write();
    
}
