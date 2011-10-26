#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TF1.h"
#include "TVirtualFitter.h"

#include "histoFunc.h"

#include "Math/GenVector/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >	XYZTVector;

XYZTVector* GetNeutrino(const XYZTVector& lep, const XYZTVector& met, const XYZTVector& WJ1, const XYZTVector& WJ2, const float& mH);

double PURescaleFactor(const double& nPU_MC);
double crystalBallHigh(double* x, double* par);
double crystalBallLowHigh(double* x, double* par);
double superGausCumCauda(double* x, double* par);


void sidebandNormalization(const int& mH, const double& xMin_signal, const double& xMax_signal, const bool& drawPlots = false)
{
  //-----------------------
  // define the vars
  std::string varName = "lepNuW_m";
  std::string varTitle = "m(lepton+neutrino+WJet1+WJet2)";
  
  int step = 17;
  char treeName[50];
  sprintf(treeName, "ntu_%d", step);
  
  float lumi = 1092;
  
  int nBins = 100;
  double xMin = 0.;
  double xMax = 1000.;
  //double xMin_signal = 420.;
  //double xMax_signal = 620.;
  double xWidth = (xMax-xMin)/nBins;
  
  double x,w;
  
  char signalCut[50];
  sprintf(signalCut,"x >= %f && x < %f",xMin_signal,xMax_signal);
  char sidebandCut[50];
  sprintf(sidebandCut,"x < %f || x >= %f",xMin_signal,xMax_signal);
  
  
  
  //-------------------
  // define the outfile
  char outFileName[50];
  sprintf(outFileName,"sidebandNormalization_mH%d.root",mH);
  TFile* outFile = new TFile(outFileName,"RECREATE");
  outFile -> cd();
  
  
  
  //-------------------
  // define the infiles
  char higgsMass[50];
  sprintf(higgsMass,"%d",mH);
  
  std::string BKGPath = "../../data/EGMu/VBFAnalysis_PFlow_H" + std::string(higgsMass) + "_PT30_maxDeta_minDeta_Summer11_EGMu_noMassDependentCuts/";
  
  std::string QCD_Pt20_MuEnrichedPtFolder = "QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6_Summer11-PU_S4_START42_V11-v1/";
  std::string QCD_Pt20to30_BCtoEFolder = "QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6_Summer11-PU_S3_START42_V11-v2/";
  std::string QCD_Pt30to80_BCtoEFolder = "QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6_Summer11-PU_S4_START42_V11-v1/";
  std::string QCD_Pt80to170_BCtoEFolder = "QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia_Summer11-PU_S4_START42_V11-v1/";
  std::string QCD_Pt20to30_EMEnrichedFolder = "QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6_Summer11-PU_S4_START42_V11-v1/";
  std::string QCD_Pt30to80_EMEnrichedFolder = "QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia_Summer11-PU_S4_START42_V11-v1/";
  std::string QCD_Pt80to170_EMEnrichedFolder = "QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6_Summer11-PU_S4_START42_V11-v1/";
  std::string ZJetsFolder = "DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string TJets_schannelFolder = "T_TuneZ2_s-channel_7TeV-powheg-tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string TJets_tchannelFolder = "T_TuneZ2_t-channel_7TeV-powheg-tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string TJets_tWchannelFolder = "T_TuneZ2_tW-channel-DR_7TeV-powheg-tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string TBarJets_schannelFolder = "Tbar_TuneZ2_s-channel_7TeV-powheg-tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string TBarJets_tchannelFolder = "Tbar_TuneZ2_t-channel_7TeV-powheg-tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string TBarJets_tWchannelFolder = "Tbar_TuneZ2_tW-channel-DR_7TeV-powheg-tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string TTJetsFolder = "TTJets_TuneZ2_7TeV-madgraph-tauola_Summer11-PU_S4_START42_V11-v2/";
  std::string WJetsFolder = "WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string WWFolder = "WW_TuneZ2_7TeV_pythia6_tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string WZFolder = "WZ_TuneZ2_7TeV_pythia6_tauola_Summer11-PU_S4_START42_V11-v1/";
  std::string ZZFolder = "ZZ_TuneZ2_7TeV_pythia6_tauola_Summer11-PU_S4_START42_V11-v1/";
  
  std::string ggHFolder = "GluGluToHToWWToLNuQQ_M-"+std::string(higgsMass)+"_7TeV-powheg-pythia6_Summer11-PU_S4_START42_V11-v1/";
  std::string qqHFolder = "VBF_HToWWToLNuQQ_M-"+std::string(higgsMass)+"_7TeV-powheg-pythia6_Summer11-PU_S4_START42_V11-v1/";
  
  std::string DataPromptRecov1Folder = "data_Run2011A-May10ReReco-v1/";
  std::string DataPromptRecov2Folder = "data_Run2011A-PromptReco-v4/";  
  
  
  int nBKG = 19;
  int nBKGSummed = 6;
  
  std::string* BKGNames = new std::string[nBKG];
  BKGNames[0]  = BKGPath+QCD_Pt20_MuEnrichedPtFolder   +"VBFAnalysis_PFlow.root";
  BKGNames[1]  = BKGPath+QCD_Pt20to30_BCtoEFolder      +"VBFAnalysis_PFlow.root";
  BKGNames[2]  = BKGPath+QCD_Pt30to80_BCtoEFolder      +"VBFAnalysis_PFlow.root";
  BKGNames[3]  = BKGPath+QCD_Pt80to170_BCtoEFolder     +"VBFAnalysis_PFlow.root";
  BKGNames[4]  = BKGPath+QCD_Pt20to30_EMEnrichedFolder +"VBFAnalysis_PFlow.root";
  BKGNames[5]  = BKGPath+QCD_Pt30to80_EMEnrichedFolder +"VBFAnalysis_PFlow.root";
  BKGNames[6]  = BKGPath+QCD_Pt80to170_EMEnrichedFolder+"VBFAnalysis_PFlow.root";
  BKGNames[7]  = BKGPath+ZJetsFolder                   +"VBFAnalysis_PFlow.root";
  BKGNames[8]  = BKGPath+TJets_schannelFolder          +"VBFAnalysis_PFlow.root";
  BKGNames[9]  = BKGPath+TJets_tchannelFolder          +"VBFAnalysis_PFlow.root";
  BKGNames[10] = BKGPath+TJets_tWchannelFolder         +"VBFAnalysis_PFlow.root";
  BKGNames[11] = BKGPath+TBarJets_schannelFolder       +"VBFAnalysis_PFlow.root";
  BKGNames[12] = BKGPath+TBarJets_tchannelFolder       +"VBFAnalysis_PFlow.root";
  BKGNames[13] = BKGPath+TBarJets_tWchannelFolder      +"VBFAnalysis_PFlow.root";
  BKGNames[14] = BKGPath+TTJetsFolder                  +"VBFAnalysis_PFlow.root";
  BKGNames[15] = BKGPath+WJetsFolder                   +"VBFAnalysis_PFlow.root";
  BKGNames[16] = BKGPath+WWFolder                      +"VBFAnalysis_PFlow.root";
  BKGNames[17] = BKGPath+WZFolder                      +"VBFAnalysis_PFlow.root";
  BKGNames[18] = BKGPath+ZZFolder                      +"VBFAnalysis_PFlow.root";
  
  std::string* BKGShortNames = new std::string[nBKG];
  BKGShortNames[0]  = "QCD";
  BKGShortNames[1]  = "QCD";
  BKGShortNames[2]  = "QCD";
  BKGShortNames[3]  = "QCD";
  BKGShortNames[4]  = "QCD";
  BKGShortNames[5]  = "QCD";
  BKGShortNames[6]  = "QCD";
  BKGShortNames[7]  = "Z+jets";
  BKGShortNames[8]  = "t+jets";
  BKGShortNames[9]  = "t+jets";
  BKGShortNames[10] = "t+jets";
  BKGShortNames[11] = "t+jets";
  BKGShortNames[12] = "t+jets";
  BKGShortNames[13] = "t+jets";
  BKGShortNames[14] = "tt+jets";
  BKGShortNames[15] = "W+jets";
  BKGShortNames[16] = "VV";
  BKGShortNames[17] = "VV";
  BKGShortNames[18] = "VV";
  
  std::string* BKGShortNamesSummed = new std::string[nBKG];
  BKGShortNamesSummed[0]  = "QCD";
  BKGShortNamesSummed[1]  = "ZJets";
  BKGShortNamesSummed[2]  = "tJets";
  BKGShortNamesSummed[3]  = "ttJets";
  BKGShortNamesSummed[4]  = "WJets";
  BKGShortNamesSummed[5]  = "VV";
  
  int* BKGIndexes = new int[nBKG];
  BKGIndexes[0]  = 0;
  BKGIndexes[1]  = 0;
  BKGIndexes[2]  = 0;
  BKGIndexes[3]  = 0;
  BKGIndexes[4]  = 0;
  BKGIndexes[5]  = 0;
  BKGIndexes[6]  = 0;
  BKGIndexes[7]  = 1;
  BKGIndexes[8]  = 2;
  BKGIndexes[9]  = 2;
  BKGIndexes[10] = 2;
  BKGIndexes[11] = 2;
  BKGIndexes[12] = 2;
  BKGIndexes[13] = 2;
  BKGIndexes[14] = 3;
  BKGIndexes[15] = 4;
  BKGIndexes[16] = 5;
  BKGIndexes[17] = 5;
  BKGIndexes[18] = 5;
  
  Color_t* BKGColors = new Color_t[nBKG];
  BKGColors[0]  = kAzure-809;
  BKGColors[1]  = kAzure-809;
  BKGColors[2]  = kAzure-809;
  BKGColors[3]  = kAzure-809;
  BKGColors[4]  = kAzure-809;
  BKGColors[5]  = kAzure-809;
  BKGColors[6]  = kAzure-809;          
  BKGColors[7]  = 8;
  BKGColors[8]  = kBlue;
  BKGColors[9]  = kBlue;
  BKGColors[10] = kBlue;
  BKGColors[11] = kBlue;
  BKGColors[12] = kBlue;
  BKGColors[13] = kBlue;
  BKGColors[14] = kAzure-795;
  BKGColors[15] = kOrange-708;
  BKGColors[16] = kOrange-705;
  BKGColors[17] = kOrange-705;
  BKGColors[18] = kOrange-705;
  
  Color_t* BKGColorsSummed = new Color_t[nBKG];
  BKGColorsSummed[0] = kAzure-809;
  BKGColorsSummed[1] = 8;
  BKGColorsSummed[2] = kBlue;
  BKGColorsSummed[3] = kAzure-795;
  BKGColorsSummed[4] = kOrange-708;
  BKGColorsSummed[5] = kOrange-705;
  
  
  
  int nSIG = 2;
  int nSIGSummed = 1;
  
  std::string* SIGNames = new std::string[nSIG];
  SIGNames[0]  = BKGPath+ggHFolder+"VBFAnalysis_PFlow.root";
  SIGNames[1]  = BKGPath+qqHFolder+"VBFAnalysis_PFlow.root";
  
  std::string* SIGShortNames = new std::string[nSIG];
  SIGShortNames[0]  = "ggH";
  SIGShortNames[1]  = "qqH";
  
  std::string* SIGShortNamesSummed = new std::string[nSIG];
  SIGShortNamesSummed[0]  = "H";
  SIGShortNamesSummed[1]  = "H";
  
  int* SIGIndexes = new int[nSIG];
  SIGIndexes[0]  = 0;
  SIGIndexes[1]  = 0;
  
  
  
  int nDATA = 2;
  
  std::string* DATANames = new std::string[nDATA];
  DATANames[0]  = BKGPath+DataPromptRecov1Folder+"VBFAnalysis_PFlow.root";
  DATANames[1]  = BKGPath+DataPromptRecov2Folder+"VBFAnalysis_PFlow.root";
  
  
  
  
  
  
  //---------------------------------------
  // define the background shape histograms
  TH1F** BKGShapeHisto   = new TH1F*[nBKGSummed];
  TH1F* BKGTotShapeHisto = new TH1F("BKGTotShapeHisto","",nBins,xMin,xMax);
  THStack* BKGShapeStack = new THStack();
  
  for(int i = 0; i < nBKGSummed; ++i)
  {
    BKGShapeHisto[i] = NULL;
  }
  
  //----------------------
  // loop over backgrounds
  std::cout << "***********************************************************************" << std::endl;
  std::cout << ">>> Fill the background shapes" << std::endl;
    
  for(int i = 0; i < nBKG; ++i)
  {
    std::cout << ">>>> i: " << std::setw(2) << i
              << "   BKGShortNames[i] = " << std::setw(10) << BKGShortNames[i]
	            << "   BKGIndexes[i] = "    << std::setw(2)  << BKGIndexes[i]
	            << std::endl;
    
    
    
    TFile* inFile_BKGShape = TFile::Open((BKGNames[i]).c_str());
    inFile_BKGShape -> cd();
    
    
    TTree* BKGShapeTree = (TTree*)(inFile_BKGShape -> Get(treeName));
    TH1F* eventsHisto = (TH1F*)(inFile_BKGShape -> Get("events"));
    
   
    if( BKGShapeHisto[BKGIndexes[i]] == NULL )
    {
      BKGShapeHisto  [BKGIndexes[i]] = new TH1F(("BKGShapeHisto_"+BKGShortNames[i]).c_str(),"",nBins,xMin,xMax);
      
      enum EColor color = (enum EColor)(BKGColors[i]);
      
      BKGShapeHisto[BKGIndexes[i]] -> SetFillColor(color);
      BKGShapeHisto[BKGIndexes[i]] -> Sumw2();
    }
    
    
    // set branch addresses
    float totEvents = eventsHisto -> GetBinContent(1);
    float crossSection;
    float var;
    float lep_charge;
    int lep_flavour;
    float lep_eta;
    int PUit_n;
    int PUoot_n;
    BKGShapeTree -> SetBranchAddress(varName.c_str(),&var);
    BKGShapeTree -> SetBranchAddress("crossSection", &crossSection);
    BKGShapeTree -> SetBranchAddress("PUit_n", &PUit_n);
    BKGShapeTree -> SetBranchAddress("PUoot_n", &PUoot_n);
    BKGShapeTree -> SetBranchAddress("lep_charge",&lep_charge);
    BKGShapeTree -> SetBranchAddress("lep_flavour",&lep_flavour);
    BKGShapeTree -> SetBranchAddress("lep_eta",&lep_eta);
    
    
    // loop over the entries
    for(int entry = 0; entry < BKGShapeTree->GetEntries(); ++entry)
    {
      BKGShapeTree -> GetEntry(entry);
      
      x = var;
      w = 1./totEvents*crossSection*lumi*PURescaleFactor(PUit_n);
      
      //if( lep_flavour != 13 ) continue;
      
      BKGShapeHisto[BKGIndexes[i]] -> Fill(x,w);
      BKGTotShapeHisto -> Fill(x,w);
    }
  }
  
  std::cout << "***********************************************************************" << std::endl;
  std::cout << ">>> Fill the summed background shapes" << std::endl;  
  for(int i = 0; i < nBKGSummed; ++i)
  {
    std::cout << ">>>> i: " << std::setw(2) << i
              << "   BKGShortNames[i] = " << std::setw(10) << BKGShortNamesSummed[i]
              << "   Nevents = "          << std::setw(10) << BKGShapeHisto[i] -> Integral()
              << std::endl;
    
    BKGShapeStack -> Add(BKGShapeHisto[i]);
  }  
  
  
  
  
  
  
  //-----------------------------------
  // define the signal shape histograms
  TH1F** SIGShapeHisto   = new TH1F*[nSIGSummed];
  TH1F* SIGTotShapeHisto = new TH1F("SIGTotShapeHisto","",nBins,xMin,xMax);
  TH1F* SIGTotShapeHisto_signal = new TH1F("SIGTotShapeHisto_signal","",nBins,xMin,xMax);
  THStack* SIGShapeStack = new THStack();
  
  for(int i = 0; i < nSIGSummed; ++i)
  {
    SIGShapeHisto[i] = NULL;
  }
  
  //------------------
  // loop over signals
  std::cout << "***********************************************************************" << std::endl;
  std::cout << ">>> Fill the signal shapes" << std::endl;
    
  for(int i = 0; i < nSIG; ++i)
  {
    std::cout << ">>>> i: " << std::setw(2) << i
              << "   SIGShortNames[i] = " << std::setw(10) << SIGShortNames[i]
	            << "   SIGIndexes[i] = "    << std::setw(2)  << SIGIndexes[i]
	            << std::endl;
    
    
    
    TFile* inFile_SIGShape = TFile::Open((SIGNames[i]).c_str());
    inFile_SIGShape -> cd();
    
    
    TTree* SIGShapeTree = (TTree*)(inFile_SIGShape -> Get(treeName));
    TH1F* eventsHisto = (TH1F*)(inFile_SIGShape -> Get("events"));
    
   
    if( SIGShapeHisto[SIGIndexes[i]] == NULL )
    {
      SIGShapeHisto  [SIGIndexes[i]] = new TH1F(("SIGShapeHisto_"+SIGShortNames[i]).c_str(),"",nBins,xMin,xMax);
      
      SIGShapeHisto[SIGIndexes[i]] -> Sumw2();
    }
    
    
    // set branch addresses
    float totEvents = eventsHisto -> GetBinContent(1);
    float crossSection;
    float var;
    float lep_charge;
    int lep_flavour;
    float lep_eta;
    int PUit_n;
    int PUoot_n;
    SIGShapeTree -> SetBranchAddress(varName.c_str(),&var);
    SIGShapeTree -> SetBranchAddress("crossSection", &crossSection);
    SIGShapeTree -> SetBranchAddress("PUit_n", &PUit_n);
    SIGShapeTree -> SetBranchAddress("PUoot_n", &PUoot_n);
    SIGShapeTree -> SetBranchAddress("lep_charge",&lep_charge);
    SIGShapeTree -> SetBranchAddress("lep_flavour",&lep_flavour);
    SIGShapeTree -> SetBranchAddress("lep_eta",&lep_eta);
    
    
    // loop over the entries
    for(int entry = 0; entry < SIGShapeTree->GetEntries(); ++entry)
    {
      SIGShapeTree -> GetEntry(entry);
      
      x = var;
      w = 1./totEvents*crossSection*lumi*PURescaleFactor(PUit_n);
      
      //if( lep_flavour != 13 ) continue;
      
      SIGShapeHisto[SIGIndexes[i]] -> Fill(x,w);
      SIGTotShapeHisto -> Fill(x,w);
      
      if( (x >= xMin_signal) && (x < xMax_signal) )
	SIGTotShapeHisto_signal -> Fill(x,w);
    }
  }
  
  std::cout << "***********************************************************************" << std::endl;
  std::cout << ">>> Fill the summed signal shapes" << std::endl;  
  for(int i = 0; i < nSIGSummed; ++i)
  {
    std::cout << ">>>> i: " << std::setw(2) << i
              << "   SIGShortNames[i] = " << std::setw(10) << SIGShortNamesSummed[i]
              << "   Nevents = "          << std::setw(10) << SIGShapeHisto[i] -> Integral()
              << std::endl;
    
    SIGShapeStack -> Add(SIGShapeHisto[i]);
  }  
  
  
  
  
  
  
  //---------------------------
  // define the data histograms
  TH1F* DATAShapeHisto   = new TH1F("DATAShapeHisto",  "",nBins,xMin,xMax);
  DATAShapeHisto -> Sumw2();
  TH1F* DATAShapeHisto_sideband   = new TH1F("DATAShapeHisto_sideband",  "",nBins,xMin,xMax);
  DATAShapeHisto_sideband -> Sumw2();
  TH1F* DATAShapeHisto_signal   = new TH1F("DATAShapeHisto_signal",  "",nBins,xMin,xMax);
  DATAShapeHisto_signal -> Sumw2();
  
  //---------------
  // loop over data
  std::cout << "***********************************************************************" << std::endl;
  std::cout << ">>> Fill the data shapes" << std::endl;
  
  for(int i = 0; i < nDATA; ++i)
  {
    TFile* inFile_DATAShape = TFile::Open((DATANames[i]).c_str());
    inFile_DATAShape -> cd();
    
    
    TTree* DATAShapeTree = (TTree*)(inFile_DATAShape -> Get(treeName));
    
    
    // set branch addresses
    float var;
    float lep_charge;
    int lep_flavour;
    XYZTVector* lep = NULL;
    XYZTVector* met = NULL;
    XYZTVector* nu  = NULL;
    XYZTVector* WJ1 = NULL;
    XYZTVector* WJ2 = NULL;
    DATAShapeTree -> SetBranchAddress(varName.c_str(),&var);
    DATAShapeTree -> SetBranchAddress("lep_charge",&lep_charge);
    DATAShapeTree -> SetBranchAddress("lep_flavour",&lep_flavour);
    DATAShapeTree -> SetBranchAddress("lep",&lep);
    DATAShapeTree -> SetBranchAddress("met",&met);
    DATAShapeTree -> SetBranchAddress("nu", &nu);
    DATAShapeTree -> SetBranchAddress("WJ1",&WJ1);
    DATAShapeTree -> SetBranchAddress("WJ2",&WJ2);
    
    
    // loop over the entries
    //double JESVariation = -0.011;
    for(int entry = 0; entry < DATAShapeTree->GetEntries(); ++entry)
    {
      DATAShapeTree -> GetEntry(entry);
      
      //XYZTVector met_new = (*met) + (-1.*JESVariation)*(*WJ1) + (-1.*JESVariation)*(*WJ2);
      //XYZTVector* nu_new  = GetNeutrino(*lep,met_new,*WJ1,*WJ2,nu->Pz());
      //var = ( (1.+JESVariation)*(*WJ1) + (1.+JESVariation)*(*WJ2) ).mass();
      //var = ((*lep) + (*nu_new) + (1.+JESVariation)*(*WJ1) + (1.+JESVariation)*(*WJ2)).mass();
      x = var;
      w = 1.;
      
      //if( lep_flavour != 13 ) continue;
      
      DATAShapeHisto -> Fill(x,w);
      if( (x < xMin_signal) || (x >= xMax_signal) ) 
        DATAShapeHisto_sideband -> Fill(x,w);
      if( (x >= xMin_signal) && (x < xMax_signal) ) 
        DATAShapeHisto_signal -> Fill(x,w);
    }
  }
  
  
  
  
  
  
  //--------------------------
  // draw the background stack
  if( drawPlots )
  {
    char axisTitle[50];
    sprintf(axisTitle,"events / %.2e",xWidth);
    
    
    
    TCanvas* c1 = new TCanvas("BKGShapeStack","BKGShapeStack");
    c1 -> SetGridx();
    c1 -> SetGridy();
    
    BKGShapeStack -> Draw("HIST");
    BKGShapeStack -> GetXaxis() -> SetTitle(varTitle.c_str());
    BKGShapeStack -> GetYaxis() -> SetTitle(axisTitle);
    
    
    DATAShapeHisto -> Draw("same");
    
    
    //char pngFileName[50];
    //sprintf(pngFileName,"BKGShapeStack_H%d.png",mH);
    //c1 -> Print(pngFileName,"png");
  }    
  
  
  
  
  
  
  //----------------------
  // define the bkg shapes
  std::cout << ">>> Define the background pdf" << std::endl;
  
  histoFunc* BKGTotHistoFunc = new histoFunc(BKGTotShapeHisto);
  
  TF1* BKGTotPdf = new TF1("BKGTotPdf",BKGTotHistoFunc,xMin,xMax,3,"histoFunc");
  BKGTotPdf -> SetNpx(10000);
  BKGTotPdf -> SetLineColor(kRed);
  BKGTotPdf -> SetParameter(0,1.);
  BKGTotPdf -> FixParameter(1,1.);
  BKGTotPdf -> FixParameter(2,0.);
        
  histoFunc** BKGHistoFunc = new histoFunc*[nBKGSummed];
  TF1** BKGPdf = new TF1*[nBKGSummed];
  
  for(int i = 0; i < nBKGSummed; ++i)
  {
    enum EColor color = (enum EColor)(BKGColorsSummed[i]);
    
    BKGHistoFunc[i] = new histoFunc(BKGShapeHisto[i]);
    char funcName[50];
    sprintf(funcName,"BKGPdf_%s",BKGShortNamesSummed[i].c_str());
    BKGPdf[i] = new TF1(funcName,BKGHistoFunc[i],xMin,xMax,3,"histoFunc");    
    BKGPdf[i] -> SetNpx(10000);
    BKGPdf[i] -> SetLineColor(color);
    BKGPdf[i] -> FixParameter(0,1.);
    BKGPdf[i] -> FixParameter(1,1.);
    BKGPdf[i] -> FixParameter(2,0.);
  }  
  
  
  
  
  //----------------------------------------
  // extrapolate events in the signal region
  std::cout << ">>> Extrapolate events in the signal region" << std::endl;
  
  TCanvas* c_data = new TCanvas("c_data","c_data");
  c_data -> cd();
  
  //BKGShapeHisto[5] -> Draw("");
  DATAShapeHisto -> GetXaxis() -> SetTitle("m(lepton+neutrino+WJets)");
  DATAShapeHisto -> GetYaxis() -> SetTitle("events / 1.0e02 GeV/c^{2}");
  DATAShapeHisto -> Draw("");
  DATAShapeHisto_signal -> SetMarkerColor(kBlue);
  DATAShapeHisto_signal -> Draw("same");
  
  //DATAShapeHisto_sideband -> Fit("BKGTotPdf","N+","",200.,1000.);
  //BKGTotPdf -> Draw("same");
  
  
  //TF1* user = new TF1("user","[0]*exp(-1.*[1]*x)",350.,xMax);
  //user -> SetParameters(32960.,0.015);
  
  //TF1* user = new TF1("user","exp([0]-1.*[1]*x)+exp([2]-1.*[3]*x)",350.,xMax);
  //user -> SetParameters(5.,0.015,1.,0.015);
  
  
  //TF1* user = new TF1("user",crystalBallHigh,300.,xMax,5);  
  //user -> SetParameters(DATAShapeHisto->GetMaximum(),350.,50.,1.,2.);
  //user -> SetParName(0,"N");
  //user -> SetParName(1,"#mu");
  //user -> SetParName(2,"#sigma");
  //user -> SetParName(3,"#alpha_{high}");
  //user -> SetParName(4,"n_{high}");
  
  
  //TF1* user = new TF1("user",crystalBallLowHigh,xMin,xMax,7);  
  //user -> SetParameters(180.,350.,50.,0.2,2.,0.5,1.);
  //user -> SetParName(0,"N");
  //user -> SetParName(1,"#mu");
  //user -> SetParName(2,"#sigma");
  //user -> SetParName(3,"#alpha_{high}");
  //user -> SetParName(4,"n_{high}");
  //user -> SetParName(5,"#alpha_{low}");
  //user -> SetParName(6,"n_{low}");



  TF1* user = new TF1("user",superGausCumCauda,150.,xMax,4);
  user -> SetParameters(14784,200.,30.68,0.33);
  user -> SetParName(0,"N");
  user -> SetParName(1,"#mu");
  user -> SetParName(2,"#sigma");
  user -> SetParName(3,"#alpha");
  
  
  
  //TF1* user = new TF1("user"," ( [0] * (1 +TMath::Erf((x-[1])/([2]*sqrt(2)))) * exp(-1.*[3]*x) * exp([3]*([1]+[2])))",0.,1000.);
  //user -> SetParameters(200,300,50,0.011,0.011);

  user -> SetNpx(10000);
  DATAShapeHisto_sideband -> Fit("user","RN+");
  //DATAShapeHisto -> Fit("user","RN+");
  //BKGShapeHisto[5] -> Fit("user","RN+");
  user -> Draw("same");
  
  
  
  //Create a histogram to hold the confidence intervals
  TH1F *hint = new TH1F("hint", "Fitted function with .68 conf.band", nBins, xMin, xMax);
  (TVirtualFitter::GetFitter())->GetConfidenceIntervals(hint, 0.68);
  //Now the "hint" histogram has the fitted function values as the 
  //bin contents and the confidence intervals as bin errors
  hint->SetStats(kFALSE);
  hint->SetMarkerSize(0);
  hint->SetFillColor(2);
  //hint->SetFillStyle(3004);
  hint->Draw("e3 same");
  DATAShapeHisto -> Draw("same");
  DATAShapeHisto_signal -> Draw("same");  
  float BandIntegral = 0;
  for ( int iBin = 1; iBin <= nBins; iBin++  ) {
   
    float thisX = DATAShapeHisto -> GetBinCenter(iBin);
    if ( thisX >= xMin_signal && thisX < xMax_signal ) BandIntegral += hint -> GetBinError(iBin);
  }
  
  
  
  float nSIG_signal = SIGTotShapeHisto_signal -> Integral();
  std::cout << "nSIG_signal     : " << nSIG_signal << std::endl;
  std::cout << SIGTotShapeHisto -> GetEntries() << std::endl;
  float nBKGTot_signal = user -> Integral(xMin_signal,xMax_signal)/xWidth;
  std::cout << "nBKGTot_signal     : " << nBKGTot_signal << " (" << 100.*BandIntegral/nBKGTot_signal <<  "\%)" << std::endl;
  float nDATA_signal = DATAShapeHisto_signal -> Integral();
  std::cout << "nDATA_signal     : " << nDATA_signal      << std::endl;
  
  
  
  outFile -> cd();
  DATAShapeHisto -> Write();
  DATAShapeHisto_sideband -> Write();
  DATAShapeHisto_signal -> Write();
  outFile -> Close();
}






XYZTVector* GetNeutrino(const XYZTVector& lep, const XYZTVector& met, const XYZTVector& WJ1, const XYZTVector& WJ2, const float& mH)
{
  // neutrino                                                                                                                                                                      
  float alpha = lep.px()*met.px() + lep.py()*met.py();

  float delta = (alpha + 0.5*80.399*80.399)*(alpha + 0.5*80.399*80.399) - lep.pt()*lep.pt()*met.pt()*met.pt();

  if( alpha - lep.pt()*met.pt() + 0.5*80.399*80.399 < 0 )
    delta = 0.;

  float pz1 = ( lep.pz()*(alpha + 0.5*80.399*80.399) + lep.energy()*sqrt(delta) ) / lep.pt() / lep.pt();
  float pz2 = ( lep.pz()*(alpha + 0.5*80.399*80.399) - lep.energy()*sqrt(delta) ) / lep.pt() / lep.pt();

  XYZTVector* nu1 = new XYZTVector(met.px(), met.py(), pz1, sqrt(met.px()*met.px() + met.py()*met.py() + pz1*pz1));
  XYZTVector* nu2 = new XYZTVector(met.px(), met.py(), pz2, sqrt(met.px()*met.px() + met.py()*met.py() + pz2*pz2));
  
  //if( fabs(mH - (lep+(*nu1)+WJ1+WJ2).mass()) < fabs(mH - (lep+(*nu2)+WJ1+WJ2).mass()) )
  if( fabs(mH - nu1->Pz()) < fabs(mH - nu2->Pz()) )
  {
    delete nu2;
    return nu1;
  }
  else
  {
    delete nu1;
    return nu2;
  }
}







double PURescaleFactor(const double& nPU_MC)
{
  if(nPU_MC < 0.)
    return 1.;
  
  else if(nPU_MC > 24.)
    return 0.;
  
  else
    {
      double distrPU_MC[] = {
	0.104109,
	0.0703573,
	0.0698445,
	0.0698254,
	0.0697054,
	0.0697907,
	0.0696751,
	0.0694486,
	0.0680332,
	0.0651044,
	0.0598036,
	0.0527395,
	0.0439513,
	0.0352202,
	0.0266714,
	0.019411,
	0.0133974,
	0.00898536,
	0.0057516,
	0.00351493,
	0.00212087,
	0.00122891,
	0.00070592,
	0.000384744,
      0.000219377
	/*0.0698146584, // 0
      0.0698146584, // 1
      0.0698146584, // 2
      0.0698146584, // 3
      0.0698146584, // 4
      0.0698146584, // 5
      0.0698146584, // 6
      0.0698146584, // 7
      0.0698146584, // 8
      0.0698146584, // 9
      0.0698146584, // 10
      0.0630151648, // 11
      0.0526654164, // 12
      0.0402754482, // 13
      0.0292988928, // 14
      0.0194384503, // 15
      0.0122016783, // 16
      0.0072070420, // 17
      0.0040036370, // 18
      0.0020278322, // 19
      0.0010739954, // 20
      0.0004595759, // 21
      0.0002229748, // 22
      0.0001028162, // 23
      0.0000458337  // 24*/
      };
    
      double distrPU_DATA[] = {
	0.0132558,
	0.0316993,
	0.0719455,
	0.115284,
	0.145239,
	0.152783,
	0.139182,
	0.112847,
	0.082904,
	0.055968,
	0.0351001,
	0.0206271,
	0.0114405,
	0.00602595,
	0.00303009,
	0.00146112,
	0.000678137,
	0.000303988,
	0.000132051,
	5.57086e-05,
	2.28897e-05,
	9.17508e-06,
	3.59522e-06,
	1.3797e-06,
      8.16915e-07
	/*0.0185058, // 0
      0.0305444, // 1
      0.0694579, // 2
      0.112061, // 3
      0.142602, // 4
      0.152644, // 5
      0.139466, // 6
      0.112813, // 7
      0.0833194, // 8
      0.0565895, // 9
      0.0358211, // 10
      0.0213224, // 11
      0.012013, // 12
      0.00644175, // 13
      0.00331013, // 14
      0.00164017, // 15
      0.000786272, // 16
      0.000366279, // 17
      0.000166604, // 18
      7.40097e-05, // 19
      3.22769e-05, // 20
      1.3882e-05, // 21
      5.84891e-06, // 22
      2.42198e-06, // 23
      1.63229e-06 // 24*/
      };
    
      return 1. * distrPU_DATA[int(nPU_MC)] / distrPU_MC[int(nPU_MC)];
    }
}






/*** single crystal ball ***/
double crystalBallHigh(double* x, double* par)
{
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  //[4] = n
  
  double xx = x[0];
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];
  double n = par[4];
  
  if( (xx-mean)/sigma > fabs(alpha) )  
  {
    double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
    double B = n/fabs(alpha) - fabs(alpha);
    
    return par[0] * A * pow(B + (xx-mean)/sigma, -1.*n);
  }
  
  else
  {
    return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigma*sigma) );
  }
}



/*** double crystal ball ***/
double crystalBallLowHigh(double* x, double* par)
{
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  //[4] = n
  //[5] = alpha2
  //[6] = n2
  //[7] = scale
  
  double xx = x[0];
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];
  double n = par[4];
  double alpha2 = par[5];
  double n2 = par[6];
  
  if( (xx-mean)/sigma > fabs(alpha) )  
    {
      double A = pow(n/fabs(alpha), n) * exp(-0.5 * alpha*alpha);
      double B = n/fabs(alpha) - fabs(alpha);
    
      return par[0] * A * pow(B + (xx-mean)/sigma, -1.*n);
    }
  
  else if( (xx-mean)/sigma < -1.*fabs(alpha2) )
    {
      double A = pow(n2/fabs(alpha2), n2) * exp(-0.5 * alpha2*alpha2);
      double B = n2/fabs(alpha2) - fabs(alpha2);
    
      return par[0] * A * pow(B - (xx-mean)/sigma, -1.*n2);
    }
  
  else
    {
      return par[0] * exp(-1. * (xx-mean)*(xx-mean) / (2*sigma*sigma) );
    } 
  
}



/*** double crystal ball ***/
double superGausCumCauda(double* x, double* par)
{
  //[0] = N
  //[1] = mean
  //[2] = sigma
  //[3] = alpha
  
  double xx = x[0];
  double mean = par[1];
  double sigma = par[2];
  double alpha = par[3];
  
  if( xx < (mean+alpha*sigma) )  
  {
    return par[0] / sigma / sqrt(2.*3.14159) * exp(-1.*(xx-mean)*(xx-mean)/(2.*sigma*sigma));
  }
  
  else
  {
    double N = par[0] / sigma / sqrt(2.*3.14159) * exp( 0.5*alpha*alpha + alpha/sigma*mean );
    double K = alpha/sigma;
    
    return N * exp(-1.*K*xx);
  }
}
