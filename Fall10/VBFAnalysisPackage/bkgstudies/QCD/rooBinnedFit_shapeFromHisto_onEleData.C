
{
  using namespace RooFit;
  

  std::string BKGPath = "/home/dimatteo/PHD_MiBi/VBF/Ntuples/VBFAnalysis_AK5PF_H400_PT30_maxDeta_minDeta_Spring11_EGMu_noHLT_noIso_noMet/" ;

  std::string WJetsFolder = "WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string ZJetsFolder = "DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string GJets_HT40To100Folder = "GJets_TuneD6T_HT-40To100_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string GJets_HT100To200Folder = "GJets_TuneD6T_HT-100To200_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string GJets_HT200Folder = "GJets_TuneD6T_HT-200_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string TTJetsFolder = "TTJets_TuneZ2_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string WWFolder = "WWtoAnything_TuneZ2_7TeV-pythia6-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string WZFolder = "WZtoAnything_TuneZ2_7TeV-pythia6-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string TJets_schannelFolder = "TToBLNu_TuneZ2_s-channel_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string TJets_tchannelFolder = "TToBLNu_TuneZ2_t-channel_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string TJets_tWchannelFolder = "TToBLNu_TuneZ2_tW-channel_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string QCDFolder = "QCD_Pt-20to170_EMEnriched_TuneZ2_7TeV-pythia6_Spring11-PU_S1_START311_V1G1-v1/";
//   std::string QCDFolder = "QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6_Spring11-PU_S1_START311_V1G1-v1/";

//   std::string DATAReRecoFolder = "data_Run2011A-May10ReReco-v1/";
//   std::string DATAPromptFolder = "data_Run2011A-PromptReco-v4/";
  std::string DATAFolder = "data_Run2011A-May10thPlusPromptReco/";
//   std::string DATAFolder = "data_Run2011A-May10ReReco-v1/";
  
  std::string variableName = "met_et";
  bool onlyBarrel = true ;
  // Fit just the May10th (no PFMHT HLT)
  int runMin = 163870 ;
  
  int step = 10;
  char treeName[50];
  sprintf(treeName, "ntu_%d", step);
  int stepLess = 10;
  char treeNameStepLess[50];
  sprintf(treeNameStepLess, "ntu_%d", stepLess);
  
  // Define the control region
  float isoMin = 0.15;
  float isoMax = 0.3;
  // Define the Lepton type
  int sel_flavour = 11;
 
  // Define the variable
  float theVariable;
  
  float lumi = 1000.;
  
  int nBins = 50;
  double xMin = 0.;
  double xMin_signal = 30.;
  double xMax = 200.;
    
  /// ---------------------------------------------------------------------------------------------- ///
  /// -------------------------------------------QCD------------------------------------------------ ///
  /// ---------------------------------------------------------------------------------------------- ///
  
  // Prepare the stack for the isolation variable distribution
  TH1F* h_QCDIso = new TH1F("h_QCDIso", "h_QCDIso", 50., 0., 0.3);
  TH1F* h_WIso = new TH1F("h_WIso", "h_WIso", 50., 0., 0.3);
  TH1F* h_BKGIso = new TH1F("h_BKGIso", "h_BKGIso", 50., 0., 0.3);
  
  // Prepare the file to put the histo for the AN
  TFile* outFile = new TFile("forAn_centralLep.root","RECREATE");
  
  // get the QCD shape histogram
  TFile* inFile_QCDShape = TFile::Open( (BKGPath+QCDFolder+"VBFAnalysis_AK5PF.root").c_str() );
  inFile_QCDShape.cd();
  
  TTree* QCDShapeTree;
  QCDShapeTree = inFile_QCDShape.GetObject(treeNameStepLess, QCDShapeTree);
  
  // Prepare the variable shape for Iso and Non Iso region
  TH1F* QCDShapeHisto = new TH1F("QCDShapeHisto", "QCDShapeHisto", nBins, xMin, xMax);
  TH1F* QCDIsoShapeHisto = new TH1F("QCDIsoShapeHisto", "QCDIsoShapeHisto", nBins, xMin, xMax);
  
  // Set up the variables for the TTree analysis
  float crossSection;
  float tkIso, emIso, hadIso, pt, rho, eta; 
  int isEB, flavour;
  QCDShapeTree -> SetBranchAddress("lep_tkIso", &tkIso);
  QCDShapeTree -> SetBranchAddress("lep_emIso", &emIso);
  QCDShapeTree -> SetBranchAddress("lep_hadIso", &hadIso);
  QCDShapeTree -> SetBranchAddress("lep_pt", &pt);
  QCDShapeTree -> SetBranchAddress("lep_eta", &eta);
  QCDShapeTree -> SetBranchAddress("lep_isEB", &isEB);
  QCDShapeTree -> SetBranchAddress("lep_flavour", &flavour);
  QCDShapeTree -> SetBranchAddress("rhoForIsolation", &rho);
  QCDShapeTree -> SetBranchAddress(variableName.c_str(), &theVariable);
  
  for ( int iEntry = 0; iEntry < QCDShapeTree->GetEntries(); iEntry++ ) {
    
    QCDShapeTree -> GetEntry(iEntry);
    if ( flavour != sel_flavour ) continue;
                    
    // solve problems related to selective readout
    if ( sel_flavour == 11 && emIso < 0 ) emIso = 0.; 
    // central lepton cut
    if ( onlyBarrel && fabs(eta) > 1.5 ) continue;
    
    h_QCDIso -> Fill ( (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt );

    if ( isEB == 1 && 
         tkIso/pt < 0.075 && 
         emIso/pt < 0.075 && 
         hadIso/pt < 0.075 && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < 0.05 
       ) QCDIsoShapeHisto -> Fill (theVariable);
    
    if ( isEB == 0 && 
         tkIso/pt < 0.125 && 
         emIso/pt < 0.125 && 
         hadIso/pt < 0.125 && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < 0.05 
       ) QCDIsoShapeHisto -> Fill (theVariable);
       
    if ( isEB == 1 && (
         tkIso/pt > 0.075 || 
         emIso/pt > 0.075 || 
         hadIso/pt > 0.075) && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt > isoMin && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < isoMax 
       ) QCDShapeHisto -> Fill (theVariable);

    if ( isEB == 0 && (
         tkIso/pt > 0.125 || 
         emIso/pt > 0.125 || 
         hadIso/pt > 0.125) && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt > isoMin && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < isoMax 
       ) QCDShapeHisto -> Fill (theVariable);
    
    
  }
    
  // Now prepare everything to get the expected MC events
  QCDShapeTree -> SetBranchAddress("crossSection", &crossSection);
  QCDShapeTree -> GetEntry(0);
  TH1F* eventsHistoQCD;
  eventsHistoQCD = inFile_QCDShape.GetObject("events", eventsHistoQCD);
  int totEvents = eventsHistoQCD -> GetBinContent(1);
  float NQCD_MC = QCDIsoShapeHisto -> GetEntries() / totEvents * crossSection * lumi;
  

  // Set up the key histograms and save them
  
  h_QCDIso -> Scale(1./totEvents * crossSection * lumi);
  outFile -> cd();
  
  TCanvas c_QCDIso_shapes;
  QCDIsoShapeHisto -> SetLineColor(kRed);
  QCDIsoShapeHisto -> Draw();
  QCDIsoShapeHisto -> Write();

  TCanvas c_QCD_shapes;
  QCDShapeHisto -> SetLineColor(kBlue);
  QCDShapeHisto -> Draw();
  QCDShapeHisto -> Write();
  
  h_QCDIso -> Write();
  
  /// ---------------------------------------------------------------------------------------------- ///
  /// -----------------------------------------W+jets----------------------------------------------- ///
  /// ---------------------------------------------------------------------------------------------- ///
  
  // get the W shape histogram from MC
  TFile* inFile_WShape = TFile::Open( (BKGPath+WJetsFolder+"VBFAnalysis_AK5PF.root").c_str() );
  inFile_WShape.cd();
  
  TTree* WShapeTree;
  WShapeTree = inFile_WShape.GetObject(treeName, WShapeTree);
  
  TH1F* WShapeHisto = new TH1F("WShapeHisto", "WShapeHisto", nBins, xMin, xMax);
  WShapeHisto -> Sumw2();
  TH1F* WShapeNonIsoHisto = new TH1F("WShapeNonIsoHisto", "WShapeNonIsoHisto", nBins, xMin, xMax);
  WShapeNonIsoHisto -> Sumw2();
  
  TTree* WShapeTree;
  WShapeTree = inFile_WShape.GetObject(treeName, WShapeTree);
  
  WShapeTree -> SetBranchAddress("lep_tkIso", &tkIso);
  WShapeTree -> SetBranchAddress("lep_emIso", &emIso);
  WShapeTree -> SetBranchAddress("lep_hadIso", &hadIso);
  WShapeTree -> SetBranchAddress("lep_pt", &pt);
  WShapeTree -> SetBranchAddress("lep_eta", &eta);
  WShapeTree -> SetBranchAddress("lep_isEB", &isEB);
  WShapeTree -> SetBranchAddress("lep_flavour", &flavour);
  WShapeTree -> SetBranchAddress(variableName.c_str(), &theVariable);
  
  for ( int iEntry = 0; iEntry < WShapeTree->GetEntries(); iEntry++ ) {
    
    WShapeTree -> GetEntry(iEntry);
    if ( flavour != sel_flavour ) continue;
        
    // solve problems related to selective readout
    if ( sel_flavour == 11 && emIso < 0 ) emIso = 0.; 
    // central lepton cut
    if ( onlyBarrel && fabs(eta) > 1.5 ) continue;
    

    h_WIso -> Fill ( (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt );

    if ( isEB == 1 && 
         tkIso/pt < 0.075 && 
         emIso/pt < 0.075 && 
         hadIso/pt < 0.075 && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < 0.05 
       ) WShapeHisto -> Fill (theVariable);

    if ( isEB == 0 && 
         tkIso/pt < 0.125 && 
         emIso/pt < 0.125 && 
         hadIso/pt < 0.125 && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < 0.05 
       ) WShapeHisto -> Fill (theVariable);
    
    if ( isEB == 1 && (
         tkIso/pt > 0.075 || 
         emIso/pt > 0.075 || 
         hadIso/pt > 0.075) && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt > isoMin && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < isoMax 
       ) WShapeNonIsoHisto -> Fill (theVariable);

    if ( isEB == 0 && (
         tkIso/pt > 0.125 || 
         emIso/pt > 0.125 || 
         hadIso/pt > 0.125) && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt > isoMin && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < isoMax 
       ) WShapeNonIsoHisto -> Fill (theVariable);
    
    
  }

  
  // Now prepare everything to get the expected MC events
  TH1F* eventsHisto;
  eventsHisto = inFile_WShape.GetObject("events", eventsHisto);
  totEvents = eventsHisto -> GetBinContent(1);
  WShapeTree -> SetBranchAddress("crossSection", &crossSection);
  WShapeTree -> GetEntry(0);
  float NW_MC = eventsHisto -> GetBinContent(step) / totEvents * crossSection * lumi;


  h_WIso -> Scale(1./totEvents * crossSection * lumi);

  // Write the key histograms in the outPut
  float NWNonIso_MC = WShapeNonIsoHisto -> GetEntries() / totEvents * crossSection * lumi;
  TCanvas c_WNonIso_shapes;
  WShapeNonIsoHisto -> Draw();
  outFile -> cd();
  h_WIso -> Write();
  
  /// ---------------------------------------------------------------------------------------------- ///
  /// --------------------------------------otherBKG------------------------------------------------ ///
  /// ---------------------------------------------------------------------------------------------- ///
  
  // get the other background shape histograms (sum all together)
  TH1F* BKGShapeHisto = new TH1F("BKGShapeHisto", "BKGShapeHisto", nBins, xMin, xMax);
  BKGShapeHisto -> Sumw2();
  float NBKG_MC = 0;
  float NBKG_MC_NonIso = 0;
  
  int nBKG = 10;
  std::string* BKGNames = new std::string[nBKG];
  BKGNames[0] = BKGPath+ZJetsFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[1] = BKGPath+GJets_HT40To100Folder+"VBFAnalysis_AK5PF.root";
  BKGNames[2] = BKGPath+GJets_HT100To200Folder+"VBFAnalysis_AK5PF.root";
  BKGNames[3] = BKGPath+GJets_HT200Folder+"VBFAnalysis_AK5PF.root";
  BKGNames[4] = BKGPath+TTJetsFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[5] = BKGPath+WWFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[6] = BKGPath+WZFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[7] = BKGPath+TJets_schannelFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[8] = BKGPath+TJets_tchannelFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[9] = BKGPath+TJets_tWchannelFolder+"VBFAnalysis_AK5PF.root";
  
  std::string* BKGShortNames = new std::string[nBKG];
  BKGShortNames[0] = "ZJets";
  BKGShortNames[1] = "GJets_HT40To100";
  BKGShortNames[2] = "GJets_HT100To200";
  BKGShortNames[3] = "GJets_HT200";
  BKGShortNames[4] = "TTJets";
  BKGShortNames[5] = "WW";
  BKGShortNames[6] = "WZ";
  BKGShortNames[7] = "TJets_schannel";
  BKGShortNames[8] = "TJets_tchannel";
  BKGShortNames[9] = "TJets_tWchannel";
  
  for(int i = 0; i < nBKG; ++i)
{
    TFile* inFile_BKGShape = TFile::Open((BKGNames[i]).c_str());
    inFile_BKGShape.cd();
    
    TTree* BKGShapeTree;
    BKGShapeTree = inFile_BKGShape.GetObject(treeName, BKGShapeTree);
    
    eventsHisto = inFile_BKGShape.GetObject("events", eventsHisto);
    totEvents = eventsHisto -> GetBinContent(1);
    
    char TMPvariableName[50];
    sprintf(TMPvariableName, "TMP%sHisto", BKGShortNames[i].c_str());
    TH1F* TMPShapeHisto = new TH1F(TMPvariableName, TMPvariableName, nBins, xMin, xMax);
    TMPShapeHisto -> Sumw2();
    
    char TMPvariableNameNonIso[50];
    sprintf(TMPvariableNameNonIso, "TMP%sHistoNonIso", BKGShortNames[i].c_str());
    TH1F* TMPShapeHistoNonIso = new TH1F(TMPvariableNameNonIso, TMPvariableNameNonIso, nBins, xMin, xMax);
    TMPShapeHistoNonIso -> Sumw2();
    
    char TMPBKGvariableName[50];
    sprintf(TMPBKGvariableName, "BKG%sIso", BKGShortNames[i].c_str());
    TH1F* TMPBKGIso = new TH1F(TMPBKGvariableName, TMPBKGvariableName, 50., 0., 1.);
    
    BKGShapeTree -> SetBranchAddress("lep_tkIso", &tkIso);
    BKGShapeTree -> SetBranchAddress("lep_emIso", &emIso);
    BKGShapeTree -> SetBranchAddress("lep_hadIso", &hadIso);
    BKGShapeTree -> SetBranchAddress("lep_pt", &pt);
    BKGShapeTree -> SetBranchAddress("lep_eta", &eta);
    BKGShapeTree -> SetBranchAddress("lep_isEB", &isEB);
    BKGShapeTree -> SetBranchAddress("lep_flavour", &flavour);
    BKGShapeTree -> SetBranchAddress(variableName.c_str(), &theVariable);
  
    for ( int iEntry = 0; iEntry < BKGShapeTree->GetEntries(); iEntry++ ) {
    
      BKGShapeTree -> GetEntry(iEntry);
      if ( flavour != sel_flavour ) continue;
                  
      // solve problems related to selective readout
      if ( sel_flavour == 11 && emIso < 0 ) emIso = 0.; 
      // central lepton cut
      if ( onlyBarrel && fabs(eta) > 1.5 ) continue;
      
      
      TMPBKGIso -> Fill ( (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt );

      if ( isEB == 1 && 
           tkIso/pt < 0.075 && 
           emIso/pt < 0.075 && 
           hadIso/pt < 0.075 && 
           (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < 0.05 
         ) TMPShapeHisto -> Fill (theVariable);

      if ( isEB == 0 && 
           tkIso/pt < 0.125 && 
           emIso/pt < 0.125 && 
           hadIso/pt < 0.125 && 
           (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < 0.05 
         ) TMPShapeHisto -> Fill (theVariable);
       
      if ( isEB == 1 && (
           tkIso/pt > 0.075 || 
           emIso/pt > 0.075 || 
           hadIso/pt > 0.075) && 
           (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt > isoMin && 
           (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < isoMax 
         ) TMPShapeHistoNonIso -> Fill (theVariable);

      if ( isEB == 0 && (
           tkIso/pt > 0.125 || 
           emIso/pt > 0.125 || 
           hadIso/pt > 0.125) && 
           (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt > isoMin && 
           (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < isoMax 
         ) TMPShapeHistoNonIso -> Fill (theVariable);
    
    
    }
    
    
    BKGShapeTree -> SetBranchAddress("crossSection", &crossSection);
    BKGShapeTree -> GetEntry(0);
    
    
    TMPShapeHisto -> Scale(1.*crossSection/totEvents);    
    NBKG_MC_NonIso += TMPShapeHistoNonIso -> GetEntries() / totEvents * crossSection * lumi;
    TMPBKGIso -> Scale(1.*crossSection/totEvents*lumi);
        
    BKGShapeHisto.Add(TMPShapeHisto);
    NBKG_MC += eventsHisto -> GetBinContent(step) / totEvents * crossSection * lumi;
    h_BKGIso -> Add ( TMPBKGIso );

}

  outFile -> cd();
  h_BKGIso -> Write();

  /// ---------------------------------------------------------------------------------------------- ///
  /// ----------------------------------------DATA-------------------------------------------------- ///
  /// ---------------------------------------------------------------------------------------------- ///
  
  // get the data histogram
  TFile* inFile_data = TFile::Open( (BKGPath+DATAFolder+"VBFAnalysis_AK5PF.root").c_str() );
  inFile_data.cd();
  
  TTree* dataTree;
  dataTree = inFile_data.GetObject(treeName, dataTree);
  
  TH1F* dataHisto = new TH1F("dataHisto", "dataHisto", nBins, xMin, xMax);
  dataHisto -> Sumw2();
  //inFile_data.Close();
  
  int run;
  dataTree -> SetBranchAddress("lep_tkIso", &tkIso);
  dataTree -> SetBranchAddress("lep_emIso", &emIso);
  dataTree -> SetBranchAddress("lep_hadIso", &hadIso);
  dataTree -> SetBranchAddress("lep_pt", &pt);
  dataTree -> SetBranchAddress("lep_eta", &eta);
  dataTree -> SetBranchAddress("lep_isEB", &isEB);
  dataTree -> SetBranchAddress("lep_flavour", &flavour);
  dataTree -> SetBranchAddress("runId", &run);
  dataTree -> SetBranchAddress(variableName.c_str(), &theVariable);
  
  for ( int iEntry = 0; iEntry < dataTree->GetEntries(); iEntry++ ) {
    
    dataTree -> GetEntry(iEntry);
    if ( flavour != sel_flavour ) continue;
        
    if ( run >= runMin ) continue;
    
    // solve problems related to selective readout
    if ( sel_flavour == 11 && emIso < 0 ) emIso = 0.; 
    // central lepton cut
    if ( onlyBarrel && fabs(eta) > 1.5 ) continue;
    
    if ( isEB == 1 && 
         tkIso/pt < 0.075 && 
         emIso/pt < 0.075 && 
         hadIso/pt < 0.075 && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < 0.05 
       ) dataHisto -> Fill (theVariable);

    if ( isEB == 0 && 
         tkIso/pt < 0.125 && 
         emIso/pt < 0.125 && 
         hadIso/pt < 0.125 && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < 0.05 
       ) dataHisto -> Fill (theVariable);
    
  }

  
  TTree* dataTreeStepLess;
  dataTreeStepLess = inFile_data.GetObject(treeNameStepLess, dataTreeStepLess);
      
  TH1F* QCDdataHisto = new TH1F("QCDdataHisto", "QCDdataHisto", nBins, xMin, xMax);
  
  dataTreeStepLess -> SetBranchAddress("lep_tkIso", &tkIso);
  dataTreeStepLess -> SetBranchAddress("lep_emIso", &emIso);
  dataTreeStepLess -> SetBranchAddress("lep_hadIso", &hadIso);
  dataTreeStepLess -> SetBranchAddress("lep_pt", &pt);
  dataTreeStepLess -> SetBranchAddress("lep_eta", &eta);
  dataTreeStepLess -> SetBranchAddress("lep_isEB", &isEB);
  dataTreeStepLess -> SetBranchAddress("lep_flavour", &flavour);
  dataTreeStepLess -> SetBranchAddress(variableName.c_str(), &theVariable);
  
  for ( int iEntry = 0; iEntry < dataTreeStepLess->GetEntries(); iEntry++ ) {
    
    dataTreeStepLess -> GetEntry(iEntry);
    if ( flavour != sel_flavour ) continue;
        
    // solve problems related to selective readout
    if ( sel_flavour == 11 && emIso < 0 ) emIso = 0.; 
    // central lepton cut
    if ( onlyBarrel && fabs(eta) > 1.5 ) continue;
    

    if ( isEB == 1 && (
         tkIso/pt > 0.075 || 
         emIso/pt > 0.075 || 
         hadIso/pt > 0.075) && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt > isoMin && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < isoMax 
       ) QCDdataHisto -> Fill (theVariable);

    if ( isEB == 0 && (
         tkIso/pt > 0.125 || 
         emIso/pt > 0.125 || 
         hadIso/pt > 0.125) && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt > isoMin && 
         (tkIso+emIso+hadIso-rho*3.1415*0.3*0.3)/pt < isoMax 
       ) QCDdataHisto -> Fill (theVariable);
    
  }

  outFile -> cd();
  QCDdataHisto -> Sumw2();
  QCDdataHisto -> Write();
  dataHisto -> Write();
  BKGShapeHisto -> Write();
  WShapeHisto -> Write();
  
  // define the RooRealVars
  RooRealVar x("x", variableName.c_str(), xMin, xMax);
  x.setRange("superSignal", 50., xMax);
  x.setRange("signal", xMin_signal, xMax);
  x.setRange("bkg", 0., xMin_signal);
  x.setRange("all", 0., xMax);
  
  
  // define the RooDataHist
  RooDataHist rooDataHisto("rooDataHisto", "data", RooArgList(x), dataHisto); // fitto questo
  
  RooDataHist rooWShapeHisto("rooWShapeHisto", "W", RooArgList(x), WShapeHisto);                     // per arrivare alle funzioni di fit
  RooDataHist rooBKGShapeHisto("rooBKGShapeHisto", "BKG", RooArgList(x), BKGShapeHisto);             // per arrivare alle funzioni di fit
  RooDataHist rooQCDShapeHisto("rooQCDShapeHisto", "QCD", RooArgList(x), QCDShapeHisto);             // per arrivare alle funzioni di fit
  RooDataHist rooQCDIsoShapeHisto("rooQCDIsoShapeHisto", "QCDIso", RooArgList(x), QCDIsoShapeHisto); // per arrivare alle funzioni di fit
  RooDataHist rooQCDDataShapeHisto("rooQCDDataShapeHisto", "QCDData", RooArgList(x), QCDdataHisto); // per arrivare alle funzioni di fit

  // define the signal/bkg shapes from histograms
  RooHistPdf WPdf("WPdf", "W", x, rooWShapeHisto, 0) ;                     // le funzioni di fit
  RooHistPdf BKGPdf("BKGPdf", "BKG", x, rooBKGShapeHisto, 0) ;             // le funzioni di fit
  RooHistPdf QCDPdf("QCDPdf", "QCD", x, rooQCDShapeHisto, 0) ;             // le funzioni di fit
  RooHistPdf QCDIsoPdf("QCDIsoPdf", "QCDIso", x, rooQCDIsoShapeHisto, 0) ; // le funzioni di fit
  RooHistPdf QCDDataPdf("QCDDataPdf", "QCDDataPdf", x, rooQCDDataShapeHisto, 0) ; // le funzioni di fit
  
  
  // define the total shape
  RooRealVar alpha("alpha", "alpha", 1., 0.01, 10.);
  
  RooRealVar NW("NW", "NW", NW_MC, NW_MC, NW_MC);
  RooFormulaVar NWfit("NWfit", "@0*@1", RooArgSet(alpha,NW));
  
  RooRealVar NBKG("NBKG", "NBKG", NBKG_MC, NBKG_MC, NBKG_MC);
  RooFormulaVar NBKGfit("NBKGfit", "@0*@1", RooArgSet(alpha,NBKG));
  
  RooRealVar NQCDfit("NQCDfit", "NQCDfit", NQCD_MC, NQCD_MC/10., NQCD_MC*10.);

  // events in the entire region BEFORE the fit
  std::cout << "======>>>>>> MC NW in whole region = " << NW.getVal() << std::endl;
  std::cout << "======>>>>>> MC NBKG in whole region = " << NBKG.getVal() << std::endl;
  std::cout << "======>>>>>> MC NQCD in whole region = " << NQCDfit.getVal() << std::endl;
  
  
  RooAddPdf totPdf("totPdf", "tot", RooArgList(QCDDataPdf,WPdf,BKGPdf), RooArgList(NQCDfit,NWfit,NBKGfit));
  
  // fit the histo
  totPdf.fitTo(rooDataHisto, RooFit::Extended(1), RooFit::Minos(1), RooFit::SumW2Error(kTRUE));
  
  // calculate events in signal region
  RooAbsReal* WIntegral = WPdf.createIntegral(x, NormSet(x), Range("signal")); 
  RooAbsReal* BKGIntegral = BKGPdf.createIntegral(x, NormSet(x), Range("signal")); 
  RooAbsReal* QCDIntegral = QCDDataPdf.createIntegral(x, NormSet(x), Range("signal"));   
  
  std::cout << "======>>>>>> NW in signal region = " << WIntegral->getVal() * NWfit.getVal() << std::endl;
  std::cout << "======>>>>>> NBKG in signal region = " << BKGIntegral->getVal() * NBKGfit.getVal() << std::endl;
  std::cout << "======>>>>>> NQCD in signal region = " << QCDIntegral->getVal() * NQCDfit.getVal() << std::endl;

  // events in the entire region
  std::cout << "======>>>>>> NW in whole region = " << NWfit.getVal() << std::endl;
  std::cout << "======>>>>>> NBKG in whole region = " << NBKGfit.getVal() << std::endl;
  std::cout << "======>>>>>> NQCD in whole region = " << NQCDfit.getVal() << " +- "<< NQCDfit.getError() << std::endl;
  
  // plot 
  RooPlot* rooPlot = x.frame();
  rooDataHisto.plotOn(rooPlot, DataError(RooAbsData::SumW2));
  totPdf.plotOn(rooPlot, LineColor(kRed), Range("all"));
  totPdf.plotOn(rooPlot, Components(QCDDataPdf), LineColor(kCyan+2),Range ("all"));
  totPdf.plotOn(rooPlot, Components(WPdf), LineColor(kOrange+2),Range ("all"));
  totPdf.plotOn(rooPlot, Components(BKGPdf), LineColor(kBlack),Range ("all"));
  TCanvas c1;
  rooPlot->Draw();
  
  char WString[50];
//   sprintf(WString, "NW = %.3f^{+%.1f}_{-%.1f}", NWfit.getVal(), NWfit.getAsymErrorHi(), NWfit.getAsymErrorLo());
  
  char BKGString[50];
//   sprintf(BKGString, "NBKG = %.3f^{+%.1f}_{-%.1f}", NBKGfit.getVal(), NBKGfit.getAsymErrorHi(), NBKGfit.getAsymErrorLo());
    
  char QCDString[50];
//   sprintf(QCDString, "NQCD = %.3f^{+%.1f}_{-%.1f}", NQCDfit.getVal(), NQCDfit.getAsymErrorHi(), NQCDfit.getAsymErrorLo());

  TLatex WLatex(0.65, 0.85, WString);
  WLatex.SetNDC();
  WLatex.SetTextFont(42);
  WLatex.SetTextSize(0.03);
  WLatex.Draw("same");
  
  TLatex BKGLatex(0.65, 0.75, BKGString);
  BKGLatex.SetNDC();
  BKGLatex.SetTextFont(42);
  BKGLatex.SetTextSize(0.03);
  BKGLatex.Draw("same");
  
  TLatex QCDLatex(0.65, 0.65, QCDString);
  QCDLatex.SetNDC();
  QCDLatex.SetTextFont(42);
  QCDLatex.SetTextSize(0.03);
  QCDLatex.Draw("same");
}
