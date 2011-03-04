{
  using namespace RooFit;
  
  std::string BKGPath = "/grid_mnt/data__HOME/llr/cms/abenagli/COLLISIONS7TeV/Fall10/VBFAnalysisPackage/data/VBFAnalysis_AK5PF_H160_ET30_maxDeta_minDeta_2010PU_EG_noMetCut/";
  
  std::string WJetsFolder = "2_WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/";
  std::string ZJetsFolder = "2_DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v3/";
  std::string GJets_HT40To100Folder = "2_GJets_TuneD6T_HT-40To100_7TeV-madgraph_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/";
  std::string GJets_HT100To200Folder = "2_GJets_TuneD6T_HT-100To200_7TeV-madgraph_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/";
  std::string GJets_HT200Folder = "2_GJets_TuneD6T_HT-200_7TeV-madgraph_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/";
  std::string TTJetsFolder = "3_TTJets_TuneZ2_7TeV-madgraph-tauola_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v3/";
  std::string WWFolder = "2_WWtoAnything_TuneZ2_7TeV-pythia6-tauola_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/";
  std::string WZFolder = "2_WZtoAnything_TuneZ2_7TeV-pythia6-tauola_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/";
  std::string TJets_schannelFolder = "2_TToBLNu_TuneZ2_s-channel_7TeV-madgraph_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/";
  std::string TJets_tchannelFolder = "2_TToBLNu_TuneZ2_t-channel_7TeV-madgraph_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/";
  std::string TJets_tWchannelFolder = "2_TToBLNu_TuneZ2_tW-channel_7TeV-madgraph_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v1/";

  std::string variableName = "met.Et()";
  std::string cutNameQCD = "(lep_tkIso+lep_emIso+lep_hadIso)/lep.pt() > 0.3 && (lep_tkIso+lep_emIso+lep_hadIso)/lep.pt() < 0.5";
  std::string cutName = "";

  
  int step = 6;
  char treeName[50];
  sprintf(treeName, "ntu_%d", step);
  
  float lumi = 35.;
  
  int nBins = 120;
  double xMin = 0.;
  double xMin_signal = 0.;
  double xMax = 120.;
  
  
    
  // get the QCD shape histogram
  TFile* inFile_QCDShape = TFile::Open("QCDShape.root");
  inFile_QCDShape.cd();
  
  TTree* QCDShapeTree;
  QCDShapeTree = inFile_QCDShape.GetObject(treeName, QCDShapeTree);
  
  TH1F* QCDShapeHisto = new TH1F("QCDShapeHisto", "QCDShapeHisto", nBins, xMin, xMax);
  QCDShapeTree -> Draw( (variableName+">> QCDShapeHisto").c_str(), cutNameQCD.c_str()  );
  QCDShapeHisto -> Sumw2();  
  //inFile_QCDShape.Close();
  
  
  
  // get the W shape histogram
  TFile* inFile_WShape = TFile::Open( (BKGPath+WJetsFolder+"VBFAnalysis_AK5PF.root").c_str() );
  inFile_WShape.cd();
  
  TTree* WShapeTree;
  WShapeTree = inFile_WShape.GetObject(treeName, WShapeTree);
  
  TH1F* WShapeHisto = new TH1F("WShapeHisto", "WShapeHisto", nBins, xMin, xMax);
  WShapeTree -> Draw( (variableName+">> WShapeHisto").c_str(), cutName.c_str() );
  WShapeHisto -> Sumw2();
  //inFile_WShape.Close();
  
  
  
  // get the other background shape histograms
  TH1F* BKGShapeHisto = new TH1F("BKGShapeHisto", "BKGShapeHisto", nBins, xMin, xMax);
  BKGShapeHisto -> Sumw2();
  float NBKG_MC = 0;
  
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
    
    TH1F* eventsHisto;
    eventsHisto = inFile_BKGShape.GetObject("events", eventsHisto);
    int totEvents = eventsHisto -> GetBinContent(1);
    
    float crossSection;
    BKGShapeTree -> SetBranchAddress("crossSection", &crossSection);
    BKGShapeTree -> GetEntry(0);
    
    
    char TMPvariableName[50];
    sprintf(TMPvariableName, "TMP%sHisto", BKGShortNames[i].c_str());
    TH1F* TMPShapeHisto = new TH1F(TMPvariableName, TMPvariableName, nBins, xMin, xMax);
    BKGShapeTree -> Draw( (variableName+">> "+TMPvariableName).c_str(), cutName.c_str() );
    TMPShapeHisto -> Sumw2();
    TMPShapeHisto -> Scale(1.*crossSection/totEvents);    
    
    
    BKGShapeHisto.Add(TMPShapeHisto);
    NBKG_MC += eventsHisto -> GetBinContent(step) / totEvents * crossSection * lumi;
    //inFile_WShape.Close();
  }
  
  
  
  // get the data histogram
  TFile* inFile_data = TFile::Open("data.root");
  inFile_data.cd();
  
  TTree* dataTree;
  dataTree = inFile_data.GetObject(treeName, dataTree);
  
  TH1F* dataHisto = new TH1F("dataHisto", "dataHisto", nBins, xMin, xMax);
  dataTree -> Draw( (variableName+">> dataHisto").c_str(), cutName.c_str() );
  dataHisto -> Sumw2();
  //inFile_data.Close();
  
  
  
  // define the RooRealVars
  RooRealVar x("x", variableName.c_str(), xMin, xMax);
  x.setRange("signal", xMin_signal, xMax);
  
  
  // define the RooDataHist
  RooDataHist rooDataHisto("rooDataHisto", "data", RooArgList(x), dataHisto);
  RooDataHist rooWShapeHisto("rooWShapeHisto", "W", RooArgList(x), WShapeHisto);
  RooDataHist rooBKGShapeHisto("rooBKGShapeHisto", "BKG", RooArgList(x), BKGShapeHisto);
  RooDataHist rooQCDShapeHisto("rooQCDShapeHisto", "QCD", RooArgList(x), QCDShapeHisto);
  
  
  // define the signal/bkg shapes from histograms
  RooHistPdf WPdf("WPdf", "W", x, rooWShapeHisto, 0) ;
  RooHistPdf BKGPdf("BKGPdf", "BKG", x, rooBKGShapeHisto, 0) ;
  RooHistPdf QCDPdf("QCDPdf", "QCD", x, rooQCDShapeHisto, 0) ;
  
  
  // define the total shape
  RooRealVar NW("NW", "NW", 1000., 0., 10000000.);
  RooRealVar NBKG("NBKG", "NBKG", NBKG_MC, NBKG_MC, NBKG_MC);
  RooRealVar NQCD("NQCD", "NQCD", 1000., 0., 10000000.);
  
  RooAddPdf totPdf("totPdf", "tot", RooArgList(QCDPdf,WPdf,BKGPdf), RooArgList(NQCD,NW,NBKG));
  
  
  // fit the histo
  totPdf.fitTo(rooDataHisto, RooFit::Extended(1), RooFit::Minos(1), RooFit::SumW2Error(kTRUE));
  
  
  // calculate events in signal region
  RooAbsReal* WIntegral = WPdf.createIntegral(x, NormSet(x), Range("signal")); 
  RooAbsReal* BKGIntegral = BKGPdf.createIntegral(x, NormSet(x), Range("signal")); 
  RooAbsReal* QCDIntegral = QCDPdf.createIntegral(x, NormSet(x), Range("signal"));   
  
  std::cout << "NW in signal region = " << WIntegral->getVal() * NW.getVal() << std::endl;
  std::cout << "NBKG in signal region = " << BKGIntegral->getVal() * NBKG.getVal() << std::endl;
  std::cout << "NQCD in signal region = " << QCDIntegral->getVal() * NQCD.getVal() << std::endl;
  
  
  
  // plot 
  RooPlot* rooPlot = x.frame();
  rooDataHisto.plotOn(rooPlot, DataError(RooAbsData::SumW2));
  totPdf.plotOn(rooPlot, LineColor(kRed));
  totPdf.plotOn(rooPlot, Components(QCDPdf), LineColor(kCyan+2));
  totPdf.plotOn(rooPlot, Components(WPdf), LineColor(kOrange+2));
  totPdf.plotOn(rooPlot, Components(BKGPdf), LineColor(kBlack));
  TCanvas c1;
  rooPlot->Draw();
  
  char WString[50];
  sprintf(WString, "NW = %.3f^{+%.1f}_{-%.1f}", NW.getVal(), NW.getAsymErrorHi(), NW.getAsymErrorLo());
  
  char BKGString[50];
  sprintf(BKGString, "NBKG = %.3f^{+%.1f}_{-%.1f}", NBKG.getVal(), NBKG.getAsymErrorHi(), NBKG.getAsymErrorLo());
    
  char QCDString[50];
  sprintf(QCDString, "NQCD = %.3f^{+%.1f}_{-%.1f}", NQCD.getVal(), NQCD.getAsymErrorHi(), NQCD.getAsymErrorLo());

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
