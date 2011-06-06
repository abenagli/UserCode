#include <map>
#include <vector>


void WplusWminusAsymmetry_OnMC()
{
  using namespace RooFit;
  
  std::string BKGPath = "/data1/dimatteo/VBF/AnaNtuple/VBFAnalysis_AK5PF_H160_ET30_maxDeta_maxSumEt_Spring11_Mu/";
  
  std::string WJetsFolder = "WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/";
//   std::string ZJetsFolder = "2_DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Fall10-E7TeV_ProbDist_2010Data_BX156_START38_V12-v3/"; this sample is missing
  std::string GJets_HT40To100Folder = "GJets_TuneD6T_HT-40To100_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string GJets_HT100To200Folder = "GJets_TuneD6T_HT-100To200_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string GJets_HT200Folder = "GJets_TuneD6T_HT-200_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string TTJetsFolder = "TTJets_TuneZ2_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string WWFolder = "WWtoAnything_TuneZ2_7TeV-pythia6-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string WZFolder = "WZtoAnything_TuneZ2_7TeV-pythia6-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string ZZFolder = "ZZtoAnything_TuneZ2_7TeV-pythia6-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string TJets_schannelFolder = "TToBLNu_TuneZ2_s-channel_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string TJets_tchannelFolder = "TToBLNu_TuneZ2_t-channel_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";
  std::string TJets_tWchannelFolder = "TToBLNu_TuneZ2_tW-channel_7TeV-madgraph_Spring11-PU_S1_START311_V1G1-v1/";

  std::string variableName = "WJJ_m";
  std::string cutNameQCD = "";
  std::string cutName = "";

  
  int step = 11;
  char treeName[50];
  sprintf(treeName, "ntu_%d", step);
  
  float lumi = 300.;
  
  int nBins = 30;
  double xMin = 0.;
  double xMax = 300.;
  
  
  
  
  
    
  // get the background shape histograms
  TH1F* BKGShapeHisto = new TH1F("BKGShapeHisto", "BKGShapeHisto", nBins, xMin, xMax);
  BKGShapeHisto -> Sumw2();

  TH1F* BKGShapeHisto_test = new TH1F("BKGShapeHisto_test", "BKGShapeHisto_test", nBins, xMin, xMax);
  BKGShapeHisto_test -> Sumw2();
  
  TH1F* BKGShapeHisto_p = new TH1F("BKGShapeHisto_p", "BKGShapeHisto_p", nBins, xMin, xMax);
  BKGShapeHisto_p -> Sumw2();
  TH1F* WShapeHisto_p = new TH1F("WShapeHisto_p", "WShapeHisto_p", nBins, xMin, xMax);
  WShapeHisto_p -> Sumw2();
  
  TH1F* BKGShapeHisto_m = new TH1F("BKGShapeHisto_m", "BKGShapeHisto_m", nBins, xMin, xMax);
  BKGShapeHisto_m -> Sumw2();
  TH1F* WShapeHisto_m = new TH1F("WShapeHisto_m", "WShapeHisto_m", nBins, xMin, xMax);
  WShapeHisto_m -> Sumw2();
  
  TH1F* BKGShapeHisto_diff = new TH1F("BKGShapeHisto_diff", "BKGShapeHisto_diff", nBins, xMin, xMax);
  BKGShapeHisto_diff -> Sumw2();
  
  TH1F* BKGShapeHisto_diff_corr = new TH1F("BKGShapeHisto_diff_corr", "BKGShapeHisto_diff_corr", nBins, xMin, xMax);
  BKGShapeHisto_diff_corr -> Sumw2();
  
  
  int nBKG = 11;
  std::string* BKGNames = new std::string[nBKG];
  BKGNames[0]  = BKGPath+WJetsFolder+"VBFAnalysis_AK5PF.root";
//  BKGNames[1]  = BKGPath+ZJetsFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[1]  = BKGPath+GJets_HT40To100Folder+"VBFAnalysis_AK5PF.root";
  BKGNames[2]  = BKGPath+GJets_HT100To200Folder+"VBFAnalysis_AK5PF.root";
  BKGNames[3]  = BKGPath+GJets_HT200Folder+"VBFAnalysis_AK5PF.root";
  BKGNames[4]  = BKGPath+TTJetsFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[5]  = BKGPath+WWFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[6]  = BKGPath+WZFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[7]  = BKGPath+ZZFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[8]  = BKGPath+TJets_schannelFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[9]  = BKGPath+TJets_tchannelFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[10] = BKGPath+TJets_tWchannelFolder+"VBFAnalysis_AK5PF.root";
  
  
  std::string* BKGShortNames = new std::string[nBKG];
  BKGShortNames[0]  = "WJets";
  BKGShortNames[1]  = "GJets_HT40To100";
  BKGShortNames[2]  = "GJets_HT100To200";
  BKGShortNames[3]  = "GJets_HT200";
  BKGShortNames[4]  = "TTJets";
  BKGShortNames[5]  = "WW";
  BKGShortNames[6]  = "WZ";
  BKGShortNames[7]  = "ZZ";
  BKGShortNames[8]  = "TJets_schannel";
  BKGShortNames[9]  = "TJets_tchannel";
  BKGShortNames[10] = "TJets_tWchannel";
  
  
  for(int i = 0; i < nBKG; ++i)
  {
    TFile* inFile_BKGShape = TFile::Open((BKGNames[i]).c_str());
    inFile_BKGShape -> cd();
    
    TTree* BKGShapeTree;
    BKGShapeTree = inFile_BKGShape -> GetObject(treeName, BKGShapeTree);
    
    TH1F* eventsHisto;
    inFile_BKGShape -> GetObject("events", eventsHisto);
    float totEvents = eventsHisto -> GetBinContent(1);
    
    
    // set branch addresses
    float crossSection;
    float theVariable;
    float lep_charge;
    BKGShapeTree -> SetBranchAddress("crossSection",       &crossSection);
    BKGShapeTree -> SetBranchAddress(variableName.c_str(), &theVariable);
    BKGShapeTree -> SetBranchAddress("lep_charge",         &lep_charge);
    
    
    
    // loop over the entries
    for(int entry = 0; entry < BKGShapeTree->GetEntries(); ++entry)
    {
      BKGShapeTree -> GetEntry(entry);
      
      
      if( entry < int(BKGShapeTree->GetEntries()/2) )
      {
        // all events
        BKGShapeHisto -> Fill(theVariable, 1./totEvents*crossSection*lumi);
        
        // + events
        if( lep_charge > 0. )
          BKGShapeHisto_p -> Fill(theVariable, 1./totEvents*crossSection*lumi);
        if( (lep_charge > 0.) && (BKGShortNames[i] == "WJets") )
          WShapeHisto_p -> Fill(theVariable, 1./totEvents*crossSection*lumi);
        
        // - events
        if( lep_charge < 0. )
          BKGShapeHisto_m -> Fill(theVariable, 1./totEvents*crossSection*lumi);
        if( (lep_charge < 0.) && (BKGShortNames[i] == "WJets") )
          WShapeHisto_m -> Fill(theVariable, 1./totEvents*crossSection*lumi);
      }
      
      
      if( (entry > int(BKGShapeTree->GetEntries()/2)) &&
          (BKGShortNames[i] == "WJets") )
      {
        // all events - test
        BKGShapeHisto_test -> Fill(theVariable, 1./totEvents*crossSection*lumi);
      }
      
      
    }
  }
  
  
  
  //-----------------------
  // get the bin population
  std::vector<float> events_p;
  for(int bin = 1; bin <= WShapeHisto_p->GetNbinsX(); ++bin)
  {
    events_p.push_back( WShapeHisto_p->GetBinContent(bin) );
  }
  
  std::vector<float> events_m;
  for(int bin = 1; bin <= WShapeHisto_m->GetNbinsX(); ++bin)
  {
    events_m.push_back( WShapeHisto_m->GetBinContent(bin) );
  }
  
  
  
  //----------------------
  // subtract plus - minus
  BKGShapeHisto_diff -> Add(BKGShapeHisto_p,BKGShapeHisto_m,1.,-1.);
  
  
  
  
  //-----------------------------
  // apply the correction factors
  BKGShapeHisto_diff_corr = (TH1F*)(BKGShapeHisto_diff -> Clone());
  for(int bin = 1; bin <= BKGShapeHisto_diff_corr->GetNbinsX(); ++bin)
  {
    float old = BKGShapeHisto_diff_corr -> GetBinContent(bin);
    
    float R = -1.;
    if( (events_p[bin-1] > 0.) && (events_m[bin-1] > 0.) )
      R = events_p[bin-1]/events_m[bin-1];
    float newval;

    if ( (events_p[bin-1] > 0.) && (events_m[bin-1] > 0.) && (events_p[bin-1] == events_m[bin-1]) ) newval = old;
    else newval = old * (1 + 2. / (R - 1.) );
    std::cout << " new is " << newval<< std::endl;
    BKGShapeHisto_diff_corr -> SetBinContent(bin, newval);
  }
  
  
  
  //--------------------
  // draw the histograms
  TCanvas* c1 = new TCanvas(variableName.c_str(),variableName.c_str());
  BKGShapeHisto -> SetFillColor(kBlue);
  BKGShapeHisto -> SetLineColor(kBlue);
  BKGShapeHisto -> SetFillStyle(3003);
  BKGShapeHisto -> Draw("HIST");
  
  TCanvas* c1_p = new TCanvas((variableName + "_plus").c_str(),(variableName + "_plus").c_str());
  BKGShapeHisto_p -> Draw();
  
  TCanvas* c1_m = new TCanvas((variableName + "_minus").c_str(),(variableName + "_minus").c_str());
  BKGShapeHisto_m -> Draw();
  
  TCanvas* c1_diff = new TCanvas((variableName + "_diff").c_str(),(variableName + "_diff").c_str());
  BKGShapeHisto_diff -> Draw();
  
  TCanvas* c1_diff_corr = new TCanvas((variableName + "_diff_corr").c_str(),(variableName + "_diff_corr").c_str());
  BKGShapeHisto_diff_corr -> SetMarkerColor(kRed);
  BKGShapeHisto_diff_corr -> SetLineColor(kRed);
  BKGShapeHisto -> Draw("HIST");
  BKGShapeHisto_diff_corr -> Draw("same");
  
  TCanvas* c1_diff_test = new TCanvas((variableName + "_diff_test").c_str(),(variableName + "_diff_test").c_str());
  BKGShapeHisto_test -> SetFillColor(kBlue);
  BKGShapeHisto_test -> SetFillStyle(3003);
  BKGShapeHisto_test -> Draw("HIST");
  BKGShapeHisto_diff_corr -> Draw("same");
  

}
