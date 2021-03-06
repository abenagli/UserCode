#include <iomanip>

void MakeSelectionLatexTable(){
  using namespace std;

  cout.precision(3);
  cout.unsetf(std::ios::scientific);
  const int nSample = 9;
  TH1F* theEventsHistos[nSample];
  TH1F* theData;
    
//  string leptonType = "mu";
  TString leptonType = "ele";
  TString histoName = "events_PURescaled_" + leptonType;
  string InputPath = "/gwteraz/users/benaglia/data/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_Fall11_v3_EGMu_Run2011AB_effCorr/";

  vector<string> DATAFolders;
  DATAFolders.push_back("data_SingleEle_SingleMu_Run2011A-May10ReReco-v1/");
  DATAFolders.push_back("data_SingleEle_SingleMu_Run2011A-PromptReco-v4/");
  DATAFolders.push_back("data_SingleEle_SingleMu_Run2011A-05Aug2011-v1/");
  DATAFolders.push_back("data_SingleEle_SingleMu_Run2011A-PromptReco-v6/");
  DATAFolders.push_back("data_SingleEle_SingleMu_Run2011B-PromptReco-v1/");
/*  DATAFolders.push_back("ElectronHad_abenagli-SQWaT_PAT_42X_2011A-05Aug2011_v3/");
  DATAFolders.push_back("ElectronHad_abenagli-SQWaT_PAT_42X_2011A-PromptReco-v4_v3/");
  DATAFolders.push_back("ElectronHad_abenagli-SQWaT_PAT_42X_2011A-PromptReco-v6_v3/");
  DATAFolders.push_back("ElectronHad_abenagli-SQWaT_PAT_42X_2011B-PromptReco-v1_v3/");
  DATAFolders.push_back("SingleMu_abenagli-SQWaT_PAT_42X_2011A_May10ReReco-v1_v3/");
  DATAFolders.push_back("SingleMu_abenagli-SQWaT_PAT_42X_2011A-PromptReco-v4_v3/");
  DATAFolders.push_back("SingleMu_abenagli-SQWaT_PAT_42X_2011A-PromptReco-v6_v3/");
  DATAFolders.push_back("SingleMu_abenagli-SQWaT_PAT_42X_2011B-PromptReco-v1_v3/");*/

  vector<string> QCDFolders;
  QCDFolders.push_back("QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  QCDFolders.push_back("QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  //QCDFolders.push_back("QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  QCDFolders.push_back("QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  QCDFolders.push_back("QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  QCDFolders.push_back("QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  QCDFolders.push_back("QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");

  vector<string> Higgs300Folders;
  Higgs300Folders.push_back("GluGluToHToWWToLNuQQ_M-300_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  Higgs300Folders.push_back("VBF_HToWWToLNuQQ_M-300_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  Higgs300Folders.push_back("GluGluToHToWWToTauNuQQ_M-300_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  Higgs300Folders.push_back("VBF_HToWWToTauNuQQ_M-300_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  
  vector<string> Higgs400Folders;
  Higgs400Folders.push_back("GluGluToHToWWToLNuQQ_M-400_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  Higgs400Folders.push_back("VBF_HToWWToLNuQQ_M-400_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  Higgs400Folders.push_back("GluGluToHToWWToTauNuQQ_M-400_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  Higgs400Folders.push_back("VBF_HToWWToTauNuQQ_M-400_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");

  vector<string> Higgs500Folders;
  Higgs500Folders.push_back("GluGluToHToWWToLNuQQ_M-500_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  Higgs500Folders.push_back("VBF_HToWWToLNuQQ_M-500_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  Higgs500Folders.push_back("GluGluToHToWWToTauNuQQ_M-500_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  Higgs500Folders.push_back("VBF_HToWWToTauNuQQ_M-500_7TeV-powheg-pythia6_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  
  vector<string> VVFolders;
  VVFolders.push_back("WW_TuneZ2_7TeV_pythia6_tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  VVFolders.push_back("WZ_TuneZ2_7TeV_pythia6_tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  VVFolders.push_back("ZZ_TuneZ2_7TeV_pythia6_tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  string WJetsFolder = "WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_govoni-SQWaT_PAT_42X_Fall11_v2/";
  string ZJetsFolder = "DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_govoni-SQWaT_PAT_42X_Fall11_v3_2/";
  string TTJetsFolder = "TTJets_TuneZ2_7TeV-madgraph-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/";
  vector<string> TFolders;
  TFolders.push_back("Tbar_TuneZ2_s-channel_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  TFolders.push_back("Tbar_TuneZ2_t-channel_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  TFolders.push_back("T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  TFolders.push_back("T_TuneZ2_s-channel_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  TFolders.push_back("T_TuneZ2_t-channel_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  TFolders.push_back("T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola_abenagli-SQWaT_PAT_42X_Fall11_v3/");
  
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
//  theSteps.push_back(14);
//  theSteps.push_back(15);
  theSteps.push_back(16);
    
  // Set up shortnames
  vector<string> shortnames;
  shortnames.push_back("H(300)");
  shortnames.push_back("H(400)");
  shortnames.push_back("H(500)");
  shortnames.push_back("VV");
  shortnames.push_back("W+jets");
  shortnames.push_back("Z+jets");
  shortnames.push_back("ttbar");
  shortnames.push_back("s.top");
  shortnames.push_back("QCD");
  
//  string outputType = "twiki";
  string outputType = "latex";

  // Set up everything to do the weighting
  float crossSection;
  int lep_flavour; 
  float weight, PUWeight, eventWeight;
  int totEvents; 
  float lumi = 4970;
  

  int thisFlavour;
  if ( leptonType == "ele") thisFlavour = 11;
  else thisFlavour = 13;
  TString s_weight;

  TFile* myFileW = new TFile ((InputPath + WJetsFolder + fileName).c_str(),"READ");
  TTree* myTreeW = (TTree*) myFileW -> Get("ntu_13");  
  myTreeW -> SetBranchAddress("crossSection", &crossSection);
  myTreeW -> SetBranchAddress("lep_flavour", &lep_flavour);
  myTreeW -> SetBranchAddress("totEvents", &totEvents);
  myTreeW -> SetBranchAddress("eventWeight", &eventWeight);
  myTreeW -> SetBranchAddress("PUWeight", &PUWeight);
  myTreeW -> GetEntry(0);
  s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65) * ( crossSection * %f / totEvents) * (eventWeight * PUWeight)", thisFlavour, lumi);
  myTreeW -> Draw("lep_flavour >> tmpHisto(20,0,20)",s_weight);

  theEventsHistos[4] = (TH1F*) myFileW -> Get(histoName);
  float weight = crossSection * lumi / totEvents;
  theEventsHistos[4] -> Scale(weight);
  theEventsHistos[4] -> SetBinContent(16, tmpHisto -> Integral());
    
  TFile* myFileZ = new TFile ((InputPath + ZJetsFolder + fileName).c_str(),"READ");
  TTree* myTreeZ = (TTree*) myFileZ -> Get("ntu_13");  
  myTreeZ -> SetBranchAddress("crossSection", &crossSection);
  myTreeZ -> SetBranchAddress("lep_flavour", &lep_flavour);
  myTreeZ -> SetBranchAddress("totEvents", &totEvents);
  myTreeZ -> SetBranchAddress("eventWeight", &eventWeight);
  myTreeZ -> SetBranchAddress("PUWeight", &PUWeight);
  myTreeZ -> GetEntry(0);
  s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65) * ( crossSection * %f / totEvents) * (eventWeight * PUWeight)", thisFlavour, lumi);
  myTreeZ -> Draw("lep_flavour >> tmpHisto(20,0,20)",s_weight);

  theEventsHistos[5] = (TH1F*) myFileZ -> Get(histoName);
  weight = crossSection * lumi / totEvents;
  theEventsHistos[5] -> Scale(weight);
  theEventsHistos[5] -> SetBinContent(16, tmpHisto -> Integral());

  TFile* myFileTT = new TFile ((InputPath + TTJetsFolder + fileName).c_str(),"READ");
  TTree* myTreeTT = (TTree*) myFileTT -> Get("ntu_13");  
  myTreeTT -> SetBranchAddress("crossSection", &crossSection);
  myTreeTT -> SetBranchAddress("lep_flavour", &lep_flavour);
  myTreeTT -> SetBranchAddress("totEvents", &totEvents);
  myTreeTT -> SetBranchAddress("eventWeight", &eventWeight);
  myTreeTT -> SetBranchAddress("PUWeight", &PUWeight);
  myTreeTT -> GetEntry(0);
  s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65) * ( crossSection * %f / totEvents) * (eventWeight * PUWeight)", thisFlavour, lumi);
  myTreeTT -> Draw("lep_flavour >> tmpHisto(20,0,20)",s_weight);

  theEventsHistos[6] = (TH1F*) myFileTT -> Get(histoName);
  weight = crossSection * lumi / totEvents;
  theEventsHistos[6] -> Scale(weight);
  theEventsHistos[6] -> SetBinContent(16, tmpHisto -> Integral());


  // Add up with the right weight the DATA samples
  for ( int iDATA = 0; iDATA < DATAFolders.size(); iDATA++ ) {
   
    string thisFile = InputPath + DATAFolders[iDATA] + fileName;
    TFile* myFile = new TFile (thisFile.c_str(),"READ");
    TH1F* theTempHisto = (TH1F*) myFile -> Get(histoName); 

    TH1F* tmpHisto = new TH1F("tmpHisto","tmpHisto",20,0,20);

    TTree* myTree = (TTree*) myFile -> Get("ntu_13");  
    myTree -> SetBranchAddress("lep_flavour", &lep_flavour);
    myTree -> GetEntry(0);

    int thisFlavour;
    if ( leptonType == "ele") thisFlavour = 11;
    else thisFlavour = 13;
    TString s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65)", thisFlavour);
    myTree -> Draw("lep_flavour >> tmpHisto",s_weight);
           
    if (iDATA == 0) { 
      theData = (TH1F*) myFile -> Get(histoName);
      theData -> SetBinContent(16, tmpHisto -> Integral());
    }
    else {
      theTempHisto -> SetBinContent(16, tmpHisto -> Integral());
      theData -> Add(theTempHisto);
    }
    delete tmpHisto;
        
  }


  // Add up with the right weight the QCD samples
  for ( int iQCD = 0; iQCD < QCDFolders.size(); iQCD++ ) {
   
    string thisFile = InputPath + QCDFolders[iQCD] + fileName;
    TFile* myFile = new TFile (thisFile.c_str(),"READ");

    TH1F* tmpHisto = new TH1F("tmpHisto","tmpHisto",20,0,20);

    TTree* myTree = (TTree*) myFile -> Get("ntu_13");  
    myTree -> SetBranchAddress("crossSection", &crossSection);
    myTree -> SetBranchAddress("lep_flavour", &lep_flavour);
    myTree -> SetBranchAddress("totEvents", &totEvents);
    myTree -> SetBranchAddress("eventWeight", &eventWeight);
    myTree -> SetBranchAddress("PUWeight", &PUWeight);
    myTree -> GetEntry(0);

    int thisFlavour;
    if ( leptonType == "ele") thisFlavour = 11;
    else thisFlavour = 13;
    TString s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65) * ( crossSection * %f / totEvents) * (eventWeight * PUWeight)", thisFlavour, lumi);
    myTree -> Draw("lep_flavour >> tmpHisto",s_weight);

    TH1F* theTempHisto = (TH1F*) myFile -> Get(histoName); 
    
    float weight = crossSection * lumi / totEvents;
       
    if (iQCD == 0) { 
      theEventsHistos[8] = (TH1F*) myFile -> Get(histoName);
      theEventsHistos[8] -> Scale(weight);
      theEventsHistos[8] -> SetBinContent(16, tmpHisto -> Integral());
    }
    else { 
      theTempHisto -> SetBinContent(16, tmpHisto -> Integral()/weight);
      theEventsHistos[8] -> Add(theTempHisto, weight);
    }
    delete tmpHisto;
        
  }

  // Add up with the right weight the VV samples
  for ( int iVV = 0; iVV < VVFolders.size(); iVV++ ) {
   
    string thisFile = InputPath + VVFolders[iVV] + fileName;
    TFile* myFile = new TFile (thisFile.c_str(),"READ");

    TH1F* tmpHisto = new TH1F("tmpHisto","tmpHisto",20,0,20);

    TTree* myTree = (TTree*) myFile -> Get("ntu_13");  
    myTree -> SetBranchAddress("crossSection", &crossSection);
    myTree -> SetBranchAddress("lep_flavour", &lep_flavour);
    myTree -> SetBranchAddress("totEvents", &totEvents);
    myTree -> SetBranchAddress("eventWeight", &eventWeight);
    myTree -> SetBranchAddress("PUWeight", &PUWeight);
    myTree -> GetEntry(0);

    int thisFlavour;
    if ( leptonType == "ele") thisFlavour = 11;
    else thisFlavour = 13;
    TString s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65) * ( crossSection * %f / totEvents) * (eventWeight * PUWeight)", thisFlavour, lumi);
    myTree -> Draw("lep_flavour >> tmpHisto",s_weight);
    
    TH1F* theTempHisto = (TH1F*) myFile -> Get(histoName); 

    float weight = crossSection * lumi / totEvents;
       
    if (iVV == 0) { 
      theEventsHistos[3] = (TH1F*) myFile -> Get(histoName);
      theEventsHistos[3] -> Scale(weight);
      theEventsHistos[3] -> SetBinContent(16, tmpHisto -> Integral());
    }
    else { 
      theTempHisto -> SetBinContent(16, tmpHisto -> Integral()/weight);
      theEventsHistos[3] -> Add(theTempHisto, weight);
    }
    delete tmpHisto;
        
  }

  // Add up with the right weight the single t samples
  for ( int iT = 0; iT < TFolders.size(); iT++ ) {
   
    string thisFile = InputPath + TFolders[iT] + fileName;
    TFile* myFile = new TFile (thisFile.c_str(),"READ");

    TH1F* tmpHisto = new TH1F("tmpHisto","tmpHisto",20,0,20);

    TTree* myTree = (TTree*) myFile -> Get("ntu_13");  
    myTree -> SetBranchAddress("crossSection", &crossSection);
    myTree -> SetBranchAddress("lep_flavour", &lep_flavour);
    myTree -> SetBranchAddress("totEvents", &totEvents);
    myTree -> SetBranchAddress("eventWeight", &eventWeight);
    myTree -> SetBranchAddress("PUWeight", &PUWeight);
    myTree -> GetEntry(0);

    int thisFlavour;
    if ( leptonType == "ele") thisFlavour = 11;
    else thisFlavour = 13;
    TString s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65) * ( crossSection * %f / totEvents) * (eventWeight * PUWeight)", thisFlavour, lumi);
    myTree -> Draw("lep_flavour >> tmpHisto",s_weight);
    
    TH1F* theTempHisto = (TH1F*) myFile -> Get(histoName); 
    
    float weight = crossSection * lumi / totEvents;
       
    if (iT == 0) { 
      theEventsHistos[7] = (TH1F*) myFile -> Get(histoName);
      theEventsHistos[7] -> Scale(weight);
      theEventsHistos[7] -> SetBinContent(16, tmpHisto -> Integral());
    }
    else { 
      theTempHisto -> SetBinContent(16, tmpHisto -> Integral()/weight);
      theEventsHistos[7] -> Add(theTempHisto, weight);
    }
    delete tmpHisto;
        
  }
    
  // Add up with the right weight the Higgs samples
  for ( int iHiggs = 0; iHiggs < Higgs300Folders.size(); iHiggs++ ) {
    
    string thisFile = InputPath + Higgs300Folders[iHiggs] + fileName;
    TFile* myFile = new TFile (thisFile.c_str(),"READ");
    
    TH1F* tmpHisto = new TH1F("tmpHisto","tmpHisto",20,0,20);

    TTree* myTree = (TTree*) myFile -> Get("ntu_13");  
    myTree -> SetBranchAddress("crossSection", &crossSection);
    myTree -> SetBranchAddress("lep_flavour", &lep_flavour);
    myTree -> SetBranchAddress("totEvents", &totEvents);
    myTree -> SetBranchAddress("eventWeight", &eventWeight);
    myTree -> SetBranchAddress("PUWeight", &PUWeight);
    myTree -> GetEntry(0);

    int thisFlavour;
    if ( leptonType == "ele") thisFlavour = 11;
    else thisFlavour = 13;
    TString s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65) * ( crossSection * %f / totEvents) * (eventWeight * PUWeight)", thisFlavour, lumi);
    myTree -> Draw("lep_flavour >> tmpHisto",s_weight);
    
    TH1F* theTempHisto = (TH1F*) myFile -> Get(histoName); 
    
    float weight = crossSection * lumi / totEvents;
    
    if (iHiggs == 0) { 
      theEventsHistos[0] = (TH1F*) myFile -> Get(histoName);
      theEventsHistos[0] -> Scale(weight);
      theEventsHistos[0] -> SetBinContent(16, tmpHisto -> Integral());
    }
    else { 
      theTempHisto -> SetBinContent(16, tmpHisto -> Integral()/weight);
      theEventsHistos[0] -> Add(theTempHisto, weight);
    }
    delete tmpHisto;
    
  }
  
  for ( int iHiggs = 0; iHiggs < Higgs400Folders.size(); iHiggs++ ) {
    
    string thisFile = InputPath + Higgs400Folders[iHiggs] + fileName;
    TFile* myFile = new TFile (thisFile.c_str(),"READ");
    
    TH1F* tmpHisto = new TH1F("tmpHisto","tmpHisto",20,0,20);

    TTree* myTree = (TTree*) myFile -> Get("ntu_13");  
    myTree -> SetBranchAddress("crossSection", &crossSection);
    myTree -> SetBranchAddress("lep_flavour", &lep_flavour);
    myTree -> SetBranchAddress("totEvents", &totEvents);
    myTree -> SetBranchAddress("eventWeight", &eventWeight);
    myTree -> SetBranchAddress("PUWeight", &PUWeight);
    myTree -> GetEntry(0);

    int thisFlavour;
    if ( leptonType == "ele") thisFlavour = 11;
    else thisFlavour = 13;
    TString s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65) * ( crossSection * %f / totEvents) * (eventWeight * PUWeight)", thisFlavour, lumi);
    myTree -> Draw("lep_flavour >> tmpHisto",s_weight);
    
    TH1F* theTempHisto = (TH1F*) myFile -> Get(histoName); 
    
    float weight = crossSection * lumi / totEvents;
    
    if (iHiggs == 0) { 
      theEventsHistos[1] = (TH1F*) myFile -> Get(histoName);
      theEventsHistos[1] -> Scale(weight);
      theEventsHistos[1] -> SetBinContent(16, tmpHisto -> Integral());
    }
    else { 
      theTempHisto -> SetBinContent(16, tmpHisto -> Integral()/weight);
      theEventsHistos[1] -> Add(theTempHisto, weight);
    }
    delete tmpHisto;
    
  }

  for ( int iHiggs = 0; iHiggs < Higgs500Folders.size(); iHiggs++ ) {
    
    string thisFile = InputPath + Higgs500Folders[iHiggs] + fileName;
    TFile* myFile = new TFile (thisFile.c_str(),"READ");
    
    TH1F* tmpHisto = new TH1F("tmpHisto","tmpHisto",20,0,20);

    TTree* myTree = (TTree*) myFile -> Get("ntu_13");  
    myTree -> SetBranchAddress("crossSection", &crossSection);
    myTree -> SetBranchAddress("lep_flavour", &lep_flavour);
    myTree -> SetBranchAddress("totEvents", &totEvents);
    myTree -> SetBranchAddress("eventWeight", &eventWeight);
    myTree -> SetBranchAddress("PUWeight", &PUWeight);
    myTree -> GetEntry(0);

    int thisFlavour;
    if ( leptonType == "ele") thisFlavour = 11;
    else thisFlavour = 13;
    TString s_weight = Form("(lep_flavour == %d && WJJ_m < 95 && WJJ_m > 65) * ( crossSection * %f / totEvents) * (eventWeight * PUWeight)", thisFlavour, lumi);
    myTree -> Draw("lep_flavour >> tmpHisto",s_weight);
    
    TH1F* theTempHisto = (TH1F*) myFile -> Get(histoName); 
    
    float weight = crossSection * lumi / totEvents;
    
    if (iHiggs == 0) { 
      theEventsHistos[2] = (TH1F*) myFile -> Get(histoName);
      theEventsHistos[2] -> Scale(weight);
      theEventsHistos[2] -> SetBinContent(16, tmpHisto -> Integral());
    }
    else { 
      theTempHisto -> SetBinContent(16, tmpHisto -> Integral()/weight);
      theEventsHistos[2] -> Add(theTempHisto, weight);
    }
    delete tmpHisto;
    
  }
      


  // Now produce the background sample
  TH1F* theBackground = (TH1F*) theEventsHistos[3] -> Clone("Background");
  theBackground -> Add ( theEventsHistos[4] );
  theBackground -> Add ( theEventsHistos[5] );
  theBackground -> Add ( theEventsHistos[6] );
  theBackground -> Add ( theEventsHistos[7] );
  theBackground -> Add ( theEventsHistos[8] );
  
  // Header of the table for the latex
  cout << "\\hline" << endl;
  
  cout << "Cut \&";                                                        
  
  for ( int iSample = 0; iSample < nSample; iSample++ ) {
   
    //Skip higgs 300 and 500
    if ( iSample == 0 || iSample == 2 ) continue;
    cout <<  " " << shortnames[iSample] << " &";
    
  }

  cout <<  " Bkg^{TOT} \&";
  cout <<  " Data \&";
  cout <<  " Data/Bkg^{TOT} \\\\";
  
  cout << endl;
  
  // now cycle on the steps and produce the latex table

  for ( int icut = 0; icut < theSteps.size(); icut++ ) {
  
    TString ThisCutName = theBackground ->GetXaxis()->GetBinLabel(theSteps[icut]);
    if ( theSteps[icut] < 10 ) ThisCutName.Remove(0,3);
    else ThisCutName.Remove(0,4);
    cout << "\\hline" << endl;
    cout <<  ThisCutName << " \&";                                                        

    for ( int iSample = 0; iSample < nSample; iSample++ ) {
      
      //Skip higgs 300 and 500
      if ( iSample == 0 || iSample == 2 ) continue;
      cout << theEventsHistos[iSample] -> GetBinContent(theSteps[icut]) << " \& ";
      
    }
    cout << theBackground -> GetBinContent(theSteps[icut]) << " \& ";
    cout << theData -> GetBinContent(theSteps[icut]) << " \& ";
    
    float errD, errB, intD, intB, dataOverMC, errDataOverMC;
    intB = theBackground -> GetBinContent(theSteps[icut]);
    intD = theData -> GetBinContent(theSteps[icut]);
    errB = theBackground -> GetBinError(theSteps[icut]);
    errD = theData -> GetBinError(theSteps[icut]);
    dataOverMC = intD/intB;
    errDataOverMC = intD/intB * sqrt(errB*errB/intB/intB + errD*errD/intD/intD);
    
    cout << dataOverMC << " \\pm " << errDataOverMC << "  \\\\";
    cout << endl;
    
  }
  cout << "\\hline" << endl;
  
}

