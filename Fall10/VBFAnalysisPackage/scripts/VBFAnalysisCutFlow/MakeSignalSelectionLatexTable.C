#include <iomanip>

void MakeSignalSelectionLatexTable(){
  using namespace std;

  cout.precision(3);
  cout.unsetf(std::ios::scientific);
    
//  string leptonType = "mu";
  TString leptonType = "ele";
  TString histoName = "events_PURescaled_" + leptonType;
  string InputPath = "/gwterax2/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_Fall11_v3_EGMu_Run2011AB_WithEffCorr/";

  vector<string> SignalMasses;
  const int nSignals = 3;
  SignalMasses.push_back("250");
  SignalMasses.push_back("500");
  SignalMasses.push_back("600");
  
  vector<string> SignalChannels;
  SignalChannels.push_back("GluGluToHToWWToLNuQQ_M-");
  SignalChannels.push_back("VBF_HToWWToLNuQQ_M-");
  SignalChannels.push_back("GluGluToHToWWToTauNuQQ_M-");
  SignalChannels.push_back("VBF_HToWWToTauNuQQ_M-");
  
  std::string SignalMCgenerator = "_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/";

  TH1F* theEventsHistos[nSignals][4];

  // Set up shortnames
  vector<string> SignalShortnames;
  SignalShortnames.push_back("ggH");
  SignalShortnames.push_back("qqH");
  SignalShortnames.push_back("ggH");
  SignalShortnames.push_back("qqH");

  
  string fileName = "VBFAnalysis_PFlow.root";
 
  
  // Decide which steps to plot (start from 1)
  vector<int> theSteps;
  theSteps.push_back(2);
  theSteps.push_back(3);
  theSteps.push_back(4);
  theSteps.push_back(5);
  theSteps.push_back(6);
  theSteps.push_back(7);
  theSteps.push_back(8);
  theSteps.push_back(9);
  theSteps.push_back(10);
  theSteps.push_back(11);
  theSteps.push_back(13);
  theSteps.push_back(14);
  theSteps.push_back(15);
  theSteps.push_back(16);
      
//  string outputType = "twiki";
  string outputType = "latex";

  // Set up everything to do the weighting
  float crossSection;
  int lep_flavour; 
  float lumi = 4680;
  float totEvents, weight;
  
  // Get with the right weight the signal samples
  for ( int iSignal = 0; iSignal < nSignals; iSignal++ ) {
   
    for ( int iChannel = 0; iChannel < 4; iChannel++ ) {
      string thisFile = InputPath + SignalChannels[iChannel] + SignalMasses[iSignal] + SignalMCgenerator + fileName;
      TFile* myFile = new TFile (thisFile.c_str(),"READ");

      TTree* myTree = (TTree*) myFile -> Get("ntu_15");  
      myTree -> SetBranchAddress("crossSection", &crossSection);
      myTree -> SetBranchAddress("lep_flavour", &lep_flavour);
      myTree -> GetEntry(0);

      theEventsHistos[iSignal][iChannel] = (TH1F*) myFile -> Get(histoName);
      totEvents = theEventsHistos[iSignal][iChannel] -> GetBinContent(1);
      weight = crossSection * lumi / totEvents;
      theEventsHistos[iSignal][iChannel] -> Scale(weight);

    }  
  }

  
  // Header of the table for the latex
  cout << "\\hline" << endl;
  
  cout << "Cut";                                                        
  
  for ( int iSignal = 0; iSignal < nSignals; iSignal++ ) {
    for ( int iChannel = 0; iChannel < 4; iChannel++ ) {
      cout <<  " \& " << SignalShortnames[iChannel] ;
    } 
  }
  cout <<  " \\\\";
  
  cout << endl;
  
  // now cycle on the steps and produce the latex table

  for ( int icut = 0; icut < theSteps.size(); icut++ ) {
  
    TString ThisCutName = theEventsHistos[0][0] ->GetXaxis()->GetBinLabel(theSteps[icut]);
    if ( theSteps[icut] < 10 ) ThisCutName.Remove(0,3);
    else ThisCutName.Remove(0,4);
    cout << "\\hline" << endl;
    cout <<  ThisCutName ;                                                        

    for ( int iSignal = 0; iSignal < nSignals; iSignal++ ) {
      for ( int iChannel = 0; iChannel < 4; iChannel++ ) {
      
        //Skip higgs 300 and 500
        cout << " \& " <<  theEventsHistos[iSignal][iChannel] -> GetBinContent(theSteps[icut]);
      }
    }

    cout <<  " \\\\ ";
    cout << endl;
    
  }
  cout << "\\hline" << endl;

}

