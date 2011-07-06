{
  using namespace RooFit;
  
  //pg input params
  //pg ---- ---- ----

  std::string BKGPath = "/data1/dimatteo/Higgs/Ntuples/VBFAnalysis_AK5PF_H400_ET30_maxDeta_minDeta_Spring11_EGMu_noIsoCut_noMetCut/" ;
  
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

//   std::string DATAReRecoFolder = "data_Run2011A-May10ReReco-v1/";
//   std::string DATAPromptFolder = "data_Run2011A-PromptReco-v4/";
  std::string DATAFolder = "data_Run2011A-May10thPlusPromptReco/";
  
  std::string variableName = "met.Et()";
/*  std::string cutNameQCD = "lep_flavour == 11 && (lep_tkIso+lep_emIso+lep_hadIso)/lep.pt() > 0.3 && (lep_tkIso+lep_emIso+lep_hadIso)/lep.pt() < 0.5";*/
  std::string cutNameQCD = "lep_flavour == 11 && (lep_tkIso+lep_emIso+lep_hadIso)/lep.pt() > 0.3";
  std::string cutName = "lep_flavour == 11 && (lep_tkIso+lep_emIso+lep_hadIso)/lep.pt() < 0.1";

  
  int step = 11;
  char treeName[50];
  sprintf(treeName, "ntu_%d", step);
  
  float lumi = 1000.;
  
  int nBins = 100;
  double xMin = 0.;
  double xMin_signal = 30.;
  double xMax = 200.;
  
  
  //pg test dummy
  //pg ---- ---- ---- ---- 
  
  //--> compare mc shapes w/ and w/o isolation
    
  // get the QCD shape histogram
  TFile* inFile_QCDShape = TFile::Open( (BKGPath+QCDFolder+"VBFAnalysis_AK5PF.root").c_str() );
  inFile_QCDShape.cd();
  
  TTree* QCDShapeTree;
  QCDShapeTree = inFile_QCDShape.GetObject(treeName, QCDShapeTree);
  
  TH1F* QCDShapeHisto = new TH1F("QCDShapeHisto", "QCDShapeHisto", nBins, xMin, xMax);
  QCDShapeTree -> Draw( (variableName+">> QCDShapeHisto").c_str(), cutNameQCD.c_str()  );
  QCDShapeHisto -> Sumw2();
   
  TH1F* QCDIsoShapeHisto = new TH1F("QCDIsoShapeHisto", "QCDIsoShapeHisto", nBins, xMin, xMax);
  QCDShapeTree -> Draw( (variableName+">> QCDIsoShapeHisto").c_str(), cutName.c_str()  );
  QCDIsoShapeHisto -> Sumw2();  
  float crossSection;
  QCDShapeTree -> SetBranchAddress("crossSection", &crossSection);
  QCDShapeTree -> GetEntry(0);
  TH1F* eventsHistoQCD;
  eventsHistoQCD = inFile_QCDShape.GetObject("events", eventsHistoQCD);
  int totEvents = eventsHistoQCD -> GetBinContent(1);

  float NQCD_MC = QCDIsoShapeHisto -> GetBinContent(step) / totEvents * crossSection * lumi;
  std::cout << "NQCD_MC " << NQCD_MC << std::endl;
  
   TCanvas c_QCD_shapes;
   QCDShapeHisto -> SetLineColor(kBlue);
 //   QCDShapeHisto -> Scale(1./QCDShapeHisto->GetEntries());
   QCDShapeHisto -> Draw();
   
   TCanvas c_QCDIso_shapes;
   QCDIsoShapeHisto -> SetLineColor(kRed);
 //   QCDIsoShapeHisto -> Scale(1./QCDIsoShapeHisto->GetEntries());
   QCDIsoShapeHisto -> Draw();
   
   float NonIsoIso_KS = QCDShapeHisto->KolmogorovTest( QCDIsoShapeHisto, "x");
   float IsoNonIso_KS = QCDIsoShapeHisto->KolmogorovTest( QCDShapeHisto, "x");
   
   std::cout << NonIsoIso_KS << " " << IsoNonIso_KS << std::endl;


  
  // get the W shape histogram from MC
  TFile* inFile_WShape = TFile::Open( (BKGPath+WJetsFolder+"VBFAnalysis_AK5PF.root").c_str() );
  inFile_WShape.cd();
  
  TTree* WShapeTree;
  WShapeTree = inFile_WShape.GetObject(treeName, WShapeTree);
  
  TH1F* WShapeHisto = new TH1F("WShapeHisto", "WShapeHisto", nBins, xMin, xMax);
  WShapeTree -> Draw( (variableName+">> WShapeHisto").c_str(), cutName.c_str() );
  WShapeHisto -> Sumw2();
  
  TTree* WShapeTree;
  WShapeTree = inFile_WShape.GetObject(treeName, WShapeTree);

  TH1F* eventsHisto;
  eventsHisto = inFile_WShape.GetObject("events", eventsHisto);
  totEvents = eventsHisto -> GetBinContent(1);

  WShapeTree -> SetBranchAddress("crossSection", &crossSection);
  WShapeTree -> GetEntry(0);
  float NW_MC = eventsHisto -> GetBinContent(step) / totEvents * crossSection * lumi;
  //inFile_WShape.Close();
  
  
  
  // get the other background shape histograms (sum all together)
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
    
    eventsHisto = inFile_BKGShape.GetObject("events", eventsHisto);
    totEvents = eventsHisto -> GetBinContent(1);
    
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
  TFile* inFile_data = TFile::Open( (BKGPath+DATAFolder+"VBFAnalysis_AK5PF.root").c_str() );
  inFile_data.cd();
  
  TTree* dataTree;
  dataTree = inFile_data.GetObject(treeName, dataTree);
  
  TH1F* dataHisto = new TH1F("dataHisto", "dataHisto", nBins, xMin, xMax);
  dataTree -> Draw( (variableName+">> dataHisto").c_str(), cutName.c_str() );
  dataHisto -> Sumw2();
  //inFile_data.Close();
  TH1F* QCDdataHisto = new TH1F("QCDdataHisto", "QCDdataHisto", nBins, xMin, xMax);
  dataTree -> Draw( (variableName+">> QCDdataHisto").c_str(), cutNameQCD.c_str() );
  QCDdataHisto -> Sumw2();
  
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
  //RooRealVar NW("NW", "NW", 1000., 0., 10000000.);
  RooRealVar alpha("alpha", "alpha", 1., 0.01, 10.);
  
  RooRealVar NW("NW", "NW", NW_MC, NW_MC, NW_MC);
  RooFormulaVar NWfit("NWfit", "@0*@1", RooArgSet(alpha,NW));
  
  RooRealVar NBKG("NBKG", "NBKG", NBKG_MC, NBKG_MC, NBKG_MC);
  RooFormulaVar NBKGfit("NBKGfit", "@0*@1", RooArgSet(alpha,NBKG));
  
  RooRealVar NQCDfit("NQCDfit", "NQCDfit", NQCD_MC, NQCD_MC/10., NQCD_MC*10.);

  RooAddPdf totPdf("totPdf", "tot", RooArgList(QCDDataPdf,WPdf,BKGPdf), RooArgList(NQCDfit,NWfit,NBKGfit));
//  RooAddPdf totPdf("totPdf", "tot", RooArgList(QCDPdf,WPdf), RooArgList(NQCDfit,NWfit));
  
  
  // fit the histo
  totPdf.fitTo(rooDataHisto, RooFit::Extended(1), RooFit::Minos(1), RooFit::SumW2Error(kTRUE));
//  totPdf.fitTo(rooDataHisto, RooFit::Extended(1), RooFit::Minos(1), RooFit::SumW2Error(kTRUE), Range("superSignal"));
  //pg fix the param
//  NW.setConstant(kTRUE) ;
//  totPdf.fitTo(rooDataHisto, RooFit::Extended(1), RooFit::Minos(1), RooFit::SumW2Error(kTRUE), Range("all"));
  
  
  // calculate events in signal region
  RooAbsReal* WIntegral = WPdf.createIntegral(x, NormSet(x), Range("signal")); 
  RooAbsReal* BKGIntegral = BKGPdf.createIntegral(x, NormSet(x), Range("signal")); 
  RooAbsReal* QCDIntegral = QCDDataPdf.createIntegral(x, NormSet(x), Range("signal"));   
  
  std::cout << "======>>>>>> NW in signal region = " << WIntegral->getVal() * NWfit.getVal() << std::endl;
  std::cout << "======>>>>>> NBKG in signal region = " << BKGIntegral->getVal() * NBKGfit.getVal() << std::endl;
  std::cout << "======>>>>>> NQCD in signal region = " << QCDIntegral->getVal() * NQCDfit.getVal() << std::endl;
  
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
  //sprintf(WString, "NW = %.3f^{+%.1f}_{-%.1f}", NWfit.getVal(), NWfit.getAsymErrorHi(), NWfit.getAsymErrorLo());
  
  char BKGString[50];
  //sprintf(BKGString, "NBKG = %.3f^{+%.1f}_{-%.1f}", NBKGfit.getVal(), NBKGfit.getAsymErrorHi(), NBKGfit.getAsymErrorLo());
    
  char QCDString[50];
  //sprintf(QCDString, "NQCD = %.3f^{+%.1f}_{-%.1f}", NQCDfit.getVal(), NQCDfit.getAsymErrorHi(), NQCDfit.getAsymErrorLo());

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