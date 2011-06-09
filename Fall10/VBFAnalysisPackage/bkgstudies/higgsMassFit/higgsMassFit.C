#include <iostream>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TRandom3.h"

#include "RooMsgService.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooKeysPdf.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooPlot.h"



void SetXSignal(double& xMin_signal, double& xMax_signal, const int& mH);



void higgsMassFit(const int& mH, const std::string& mode = "exclusion", const bool& drawPlots = false, const int& nToys = 10000)
{
  using namespace RooFit;
  RooMsgService::instance().deleteStream(0);
  RooMsgService::instance().deleteStream(1);
  
  
  
  std::string varName = "lepNuW_m";
  
  int step = 16;
  char treeName[50];
  sprintf(treeName, "ntu_%d", step);
  
  float lumi = 1000.;
  
  int nBins = 50;
  double xMin = 0.;
  double xMax = 1000.;
  double xMin_signal = 0.;
  double xMax_signal = 0.;
  SetXSignal(xMin_signal,xMax_signal,mH);
  
  RooRealVar x("x",varName.c_str(),xMin,xMax);
  x.setRange("low",   xMin,       xMin_signal);
  x.setRange("signal",xMin_signal,xMax_signal);
  x.setRange("high",  xMax_signal,xMax);
  
  RooRealVar w("w","weight",0.,1000000000.);
  
  char signalCut[50];
  sprintf(signalCut,"x > %f && x < %f",xMin_signal,xMax_signal);
  
  
  
  //-------------------
  // define the outfile
  char outFileName[50];
  sprintf(outFileName,"higgsMassFit_H%d_%s.root",mH,mode.c_str());
  TFile* outFile = new TFile(outFileName,"RECREATE");
  outFile -> cd();
  
  
  
  //-------------------
  // define the infiles
  char higgsMass[50];
  sprintf(higgsMass,"%d",mH);
  
  std::string BKGPath = "/grid_mnt/vol__vol1__u/llr/cms/abenagli/COLLISIONS7TeV/Fall10/VBFAnalysisPackage/data/VBFAnalysis_AK5PF_H" + 
                        std::string(higgsMass) + 
                        "_ET30_maxDeta_minDeta_Spring11_EGMu_noHiggsMassCut/";
  
  std::string WJetsFolder = "WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  std::string TTJetsFolder = "TTJets_TuneZ2_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/";
  //std::string ZJetsFolder = 
  //std::string GJets_HT40To100Folder 
  //std::string GJets_HT100To200Folder 
  //std::string GJets_HT200Folder 
  //std::string WWFolder 
  //std::string WZFolder 
  //std::string TJets_schannelFolder 
  //std::string TJets_tchannelFolder 
  //std::string TJets_tWchannelFolder 
  
  std::string GluGluHToLNuQQFolder   = "GluGluToHToWWToLNuQQ_M-"   + std::string(higgsMass) + "_7TeV-powheg-pythia6_Spring11-PU_S1_START311_V1G1-v1/";
  std::string GluGluHToTauNuQQFolder = "GluGluToHToWWToTauNuQQ_M-" + std::string(higgsMass) + "_7TeV-powheg-pythia6_Spring11-PU_S1_START311_V1G1-v1/";
  std::string VBFHToLNuQQFolder      = "VBF_HToWWToLNuQQ_M-"       + std::string(higgsMass) + "_7TeV-powheg-pythia6_Spring11-PU_S1_START311_V1G1-v1/";
  std::string VBFHToTauNuQQFolder    = "VBF_HToWWToTauNuQQ_M-"     + std::string(higgsMass) + "_7TeV-powheg-pythia6_Spring11-PU_S1_START311_V1G1-v1/";
  
  
  
  //---------------------------------------
  // define the background shape histograms
  int nBKG = 2;
  
  TH1F** BKGShapeHisto = new TH1F*[nBKG];
  TH1F* BKGTotShapeHisto = new TH1F("BKGTotShapeHisto","",nBins,xMin,xMax);
  THStack* BKGShapeStack = new THStack();
  RooDataSet** rooBKGDataSet = new RooDataSet*[nBKG];
  
  std::string* BKGNames = new std::string[nBKG];
  BKGNames[1]  = BKGPath+WJetsFolder+"VBFAnalysis_AK5PF.root";
  BKGNames[0]  = BKGPath+TTJetsFolder+"VBFAnalysis_AK5PF.root";
  //BKGNames[1]  = BKGPath+ZJetsFolder+"VBFAnalysis_AK5PF.root";
  //BKGNames[2]  = BKGPath+GJets_HT40To100Folder+"VBFAnalysis_AK5PF.root";
  //BKGNames[3]  = BKGPath+GJets_HT100To200Folder+"VBFAnalysis_AK5PF.root";
  //BKGNames[4]  = BKGPath+GJets_HT200Folder+"VBFAnalysis_AK5PF.root";
  //BKGNames[6]  = BKGPath+WWFolder+"VBFAnalysis_AK5PF.root";
  //BKGNames[7]  = BKGPath+WZFolder+"VBFAnalysis_AK5PF.root";
  //BKGNames[8]  = BKGPath+TJets_schannelFolder+"VBFAnalysis_AK5PF.root";
  //BKGNames[9]  = BKGPath+TJets_tchannelFolder+"VBFAnalysis_AK5PF.root";
  //BKGNames[10] = BKGPath+TJets_tWchannelFolder+"VBFAnalysis_AK5PF.root";
  
  std::string* BKGShortNames = new std::string[nBKG];
  BKGShortNames[1]  = "WJets";
  BKGShortNames[0]  = "TTJets";
  //BKGShortNames[1]  = "ZJets";
  //BKGShortNames[2]  = "GJets_HT40To100";
  //BKGShortNames[3]  = "GJets_HT100To200";
  //BKGShortNames[4]  = "GJets_HT200";
  //BKGShortNames[6]  = "WW";
  //BKGShortNames[7]  = "WZ";
  //BKGShortNames[8]  = "TJets_schannel";
  //BKGShortNames[9]  = "TJets_tchannel";
  //BKGShortNames[10] = "TJets_tWchannel";
  
  Color_t* BKGColors = new Color_t[nBKG];
  BKGColors[1]  = kOrange-708;
  BKGColors[0]  = kAzure-795;
  
  
  
  //-----------------------------------
  // define the signal shape histograms
  int nSIG = 2;
  
  TH1F* SIGShapeHisto = new TH1F("SIGShapeHisto","",4*nBins,xMin,xMax);
  SIGShapeHisto -> Sumw2();
  SIGShapeHisto -> SetLineWidth(2);
  SIGShapeHisto -> SetLineStyle(2);
  RooDataSet* rooSIGDataSet = new RooDataSet("rooSIGDataSet","",RooArgSet(x,w),WeightVar(w));
  
  std::string* SIGNames = new std::string[nSIG];
  SIGNames[0]  = BKGPath+GluGluHToLNuQQFolder+"VBFAnalysis_AK5PF.root";
  SIGNames[1]  = BKGPath+VBFHToLNuQQFolder+"VBFAnalysis_AK5PF.root";
  
  std::string* SIGShortNames = new std::string[nSIG];
  SIGShortNames[1]  = "ggH";
  SIGShortNames[0]  = "qqH";
  
  
  
  
  
  
  //----------------------
  // loop over backgrounds
  std::cout << "***********************************************************************" << std::endl;
  std::cout << ">>> Fill the background shapes" << std::endl;
    
  for(int i = 0; i < nBKG; ++i)
  {
    TFile* inFile_BKGShape = TFile::Open((BKGNames[i]).c_str());
    inFile_BKGShape -> cd();
    
    TTree* BKGShapeTree = (TTree*)(inFile_BKGShape -> Get(treeName));
    
    BKGShapeHisto[i] = new TH1F(("BKGShapeHisto_"+BKGShortNames[i]).c_str(),"",nBins,xMin,xMax);
    enum EColor color = (enum EColor)(BKGColors[i]);
    BKGShapeHisto[i] -> SetFillColor(color);
    BKGShapeHisto[i] -> Sumw2();
    
    rooBKGDataSet[i] = new RooDataSet(("rooBKGDataSet_"+BKGShortNames[i]).c_str(),"",RooArgSet(x,w),WeightVar(w)); 
    
    TH1F* eventsHisto;
    inFile_BKGShape -> GetObject("events", eventsHisto);
    float totEvents = eventsHisto -> GetBinContent(1);
    
    
    // set branch addresses
    float crossSection;
    float var;
    BKGShapeTree -> SetBranchAddress("crossSection", &crossSection);
    BKGShapeTree -> SetBranchAddress(varName.c_str(),&var);
    
    
    // loop over the entries
    for(int entry = 0; entry < BKGShapeTree->GetEntries(); ++entry)
    {
      BKGShapeTree -> GetEntry(entry);
      
      x = var;
      w = 1./totEvents*crossSection*lumi;
      
      BKGShapeHisto[i] -> Fill(var, 1./totEvents*crossSection*lumi);
      BKGTotShapeHisto    -> Fill(var, 1./totEvents*crossSection*lumi);
      rooBKGDataSet[i] -> add(RooArgSet(x,w));
    }
    
    BKGShapeStack -> Add(BKGShapeHisto[i]);
  }
  
  
  
  
  
  
  //------------------
  // loop over signals
  std::cout << ">>> Fill the signal shapes" << std::endl;
    
  for(int i = 0; i < nSIG; ++i)
  {
    TFile* inFile_SIGShape = TFile::Open((SIGNames[i]).c_str());
    inFile_SIGShape -> cd();
    
    TTree* SIGShapeTree = (TTree*)(inFile_SIGShape -> Get(treeName));
    
    TH1F* eventsHisto = (TH1F*)(inFile_SIGShape -> Get("events"));
    float totEvents = eventsHisto -> GetBinContent(1);
    
    
    // set branch addresses
    float crossSection;
    float var;
    SIGShapeTree -> SetBranchAddress("crossSection", &crossSection);
    SIGShapeTree -> SetBranchAddress(varName.c_str(),&var);
    
    
    // loop over the entries
    for(int entry = 0; entry < SIGShapeTree->GetEntries(); ++entry)
    {
      SIGShapeTree -> GetEntry(entry);
      
      x = var;
      w= 1./totEvents*crossSection*lumi;
      
      SIGShapeHisto -> Fill(var, 1./totEvents*crossSection*lumi);
      rooSIGDataSet -> add(RooArgSet(x,w));
    }
  }
  
  
  
  //-----------------------------------
  // draw the background + signal stack
  if( drawPlots )
  {
    TCanvas* c1 = new TCanvas("BKGShapeStack","BKGShapeStack");
    c1 -> SetGridx();
    c1 -> SetGridy();
    BKGShapeStack -> Draw("HIST");
    SIGShapeHisto -> Draw("HIST,same");  
    char pngFileName[50];
    sprintf(pngFileName,"BKGShapeStack_H%d_%s.png",mH,mode.c_str());
    c1 -> Print(pngFileName,"png");
  }  
  
  
  
  
  
  
  //---------------------------------
  // define the bkg shape with roofit
  std::cout << ">>> Define the background pdf" << std::endl;
    
  RooKeysPdf** rooBKGPdf = new RooKeysPdf*[nBKG];
  RooRealVar** rooNBKG = new RooRealVar*[nBKG];
  RooRealVar* rooNBKGTot = new RooRealVar("rooNBKGTot","",BKGTotShapeHisto->Integral(),0.,1000000.);
  
  for(int i = 0; i < nBKG; ++i)
  {  
    rooBKGPdf[i] = new RooKeysPdf(("rooBKGPdf_"+BKGShortNames[i]).c_str(),"",x,*rooBKGDataSet[i]);
    rooNBKG[i] = new RooRealVar(("rooNBKG_"+BKGShortNames[i]).c_str(),"",BKGShapeHisto[i]->Integral(),BKGShapeHisto[i]->Integral()),BKGShapeHisto[i]->Integral();
  }
  
  RooAddPdf* rooBKGTotPdf = new RooAddPdf("rooBKGTotPdf","",RooArgList(*rooBKGPdf[0],*rooBKGPdf[1]),RooArgList(*rooNBKG[0],*rooNBKG[1]));
  
  
  
  //---------------------------------
  // define the sig shape with roofit
  std::cout << ">>> Define the signal pdf" << std::endl;
    
  RooKeysPdf* rooSIGPdf = new RooKeysPdf("rooSIGPdf","",x,*rooSIGDataSet);
  RooRealVar* rooNSIG = new RooRealVar("rooNSIG","",1.,-1000000.,1000000.);
  
  
  
  //---------------------------------
  // define the tot shape with roofit
  std::cout << ">>> Define the total pdf" << std::endl;
  
  RooAddPdf* rooTotPdf = NULL;
  if( mode == "exclusion") rooTotPdf = new RooAddPdf("rooTotPdf","",RooArgList(*rooBKGTotPdf),RooArgList(*rooNBKGTot));
  if( mode == "discovery") rooTotPdf = new RooAddPdf("rooTotPdf","",RooArgList(*rooBKGTotPdf,*rooSIGPdf),RooArgList(*rooNBKGTot,*rooNSIG));
  
  
  
  
  
  
  //----
  // plot 
  if( drawPlots )
  {
    TCanvas* c2 = new TCanvas("rooTotPdf","rooTotPdf");
    c2 -> SetGridx();
    c2 -> SetGridy();
    
    RooPlot* rooBKGPlot = x.frame();
    rooBKGTotPdf -> plotOn(rooBKGPlot,LineColor(kBlack));
    enum EColor color = (enum EColor)(BKGColors[0]);
    rooBKGTotPdf -> plotOn(rooBKGPlot,Components(("rooBKGPdf_"+BKGShortNames[0]).c_str()),LineColor(color));
    color = (enum EColor)(BKGColors[1]);
    rooBKGTotPdf -> plotOn(rooBKGPlot,Components(("rooBKGPdf_"+BKGShortNames[1]).c_str()),LineColor(color));
    
    rooSIGPdf -> plotOn(rooBKGPlot,LineColor(kBlack),LineStyle(2));
    
    rooBKGPlot->Draw();
    
    TH1F* BKGShapeHistoNorm = (TH1F*) BKGTotShapeHisto -> Clone();
    BKGShapeHistoNorm -> Scale(1./BKGTotShapeHisto->Integral()/nBKG);
    BKGShapeHistoNorm -> Draw("HIST,same");

    char pngFileName[50];
    sprintf(pngFileName,"BKGShapeNorm_H%d_%s.png",mH,mode.c_str());
    c2 -> Print(pngFileName,"png");
  }
  
  
  
  
  
  
  //------------------------
  // generate toy experiment
  std::cout << "***********************************************************************" << std::endl;
  std::cout << ">>> 1st toy experiment - " << mode << " mode" << std::endl;  
  
  int NBKGToy = int(BKGTotShapeHisto->Integral());
  int NSIGToy = 0;
  if( mode == "discovery" ) NSIGToy = int(SIGShapeHisto->Integral());
  
  RooDataSet* rooBKGToyDataSet = rooBKGTotPdf->generate(RooArgSet(x),NBKGToy);
  RooDataSet* rooSIGToyDataSet = rooSIGPdf->generate(RooArgSet(x),NSIGToy);
  rooBKGToyDataSet -> append(*rooSIGToyDataSet);
  
  float NBKGToy_signal = rooBKGToyDataSet->sumEntries(signalCut);
  float NBKGToy_signal_fit = 0.;
  
  
  
  // fit
  if( mode == "exclusion" ) rooTotPdf -> fitTo(*rooBKGToyDataSet,Extended(kTRUE),PrintLevel(-1),Range("low,high"));
  if( mode == "discovery" ) rooTotPdf -> fitTo(*rooBKGToyDataSet,Extended(kTRUE),PrintLevel(-1));
  
  
  
  // count events
  if( mode == "exclusion" )
  {
    RooAbsReal* rooTotIntegral = rooTotPdf -> createIntegral(x,NormSet(x),Range("signal"));
    NBKGToy_signal_fit = rooTotIntegral->getVal() * rooNBKGTot->getVal();
    
    std::cout << ">>>>>> BKG toy events (true) in signal region in " << lumi << "/pb: " << NBKGToy_signal << std::endl;  
    std::cout << ">>>>>> BKG toy events (fit)  in signal region in " << lumi << "/pb: " << NBKGToy_signal_fit << std::endl;      
  }
  
  if( mode == "discovery" )
  {
    std::cout << ">>>>>> BKG toy events (true) in " << lumi << "/pb: " << NBKGToy << std::endl;  
    std::cout << ">>>>>> BKG toy events (fit)  in " << lumi << "/pb: " << rooNBKGTot->getVal() << std::endl;  
    std::cout << ">>>>>> SIG toy events (true) in " << lumi << "/pb: " << NSIGToy << std::endl;  
    std::cout << ">>>>>> SIG toy events (fit)  in " << lumi << "/pb: " << rooNSIG->getVal() << std::endl;  
  }
  
  if( drawPlots )
  {
    TCanvas* c3 = new TCanvas("TOY","TOY");
    c3 -> SetGridx();
    c3 -> SetGridy();
    
    RooPlot* rooTOYPlot = x.frame();
    rooBKGToyDataSet -> plotOn(rooTOYPlot);
    rooTotPdf -> plotOn(rooTOYPlot, LineColor(kRed));
    rooTotPdf -> plotOn(rooTOYPlot, Components("rooSIGPdf"), LineColor(kRed));
    rooTOYPlot->Draw();
    
    char pngFileName[50];
    sprintf(pngFileName,"BKGToyFit_H%d_%s.png",mH,mode.c_str());
    c3 -> Print(pngFileName,"png");
  }
  
  
  
  
  
  
  //-------------------------
  // generate toy experiments
  
  TH1F* h_BKGRes = new TH1F("h_BKGRes","",200,-400,400);
  TH1F* h_SIGRes = new TH1F("h_SIGRes","",200,-400,400);
  
  TRandom3 B;
  TRandom3 S;
  for(int j = 0; j < nToys; ++j)
  {
    if( j%100 == 0 )
      std::cout << ">>>>>> generating toy experiment " << j << std::endl;
    
    NBKGToy = B.Poisson(BKGTotShapeHisto->Integral());
    NSIGToy = 0;
    if( mode == "discovery" ) NSIGToy = S.Poisson(SIGShapeHisto->Integral());
    
    rooBKGToyDataSet = rooBKGTotPdf->generate(RooArgSet(x),NBKGToy);
    rooSIGToyDataSet = rooSIGPdf->generate(RooArgSet(x),NSIGToy);
    rooBKGToyDataSet -> append(*rooSIGToyDataSet);
    
    NBKGToy_signal = rooBKGToyDataSet->sumEntries(signalCut);
    NBKGToy_signal_fit = 0.;
    
    
    
    // fit
    if( mode == "exclusion" ) rooTotPdf -> fitTo(*rooBKGToyDataSet,Extended(kTRUE),PrintLevel(-1),Range("low,high"));
    if( mode == "discovery" ) rooTotPdf -> fitTo(*rooBKGToyDataSet,Extended(kTRUE),PrintLevel(-1));    
    
    
    
    // count events
    if( mode == "exclusion" )
    {
      RooAbsReal* rooTotIntegral = rooTotPdf -> createIntegral(x,NormSet(x),Range("signal"));
      NBKGToy_signal_fit = rooTotIntegral->getVal() * rooNBKGTot->getVal();
      
      h_BKGRes -> Fill(NBKGToy_signal_fit - NBKGToy_signal);
      h_SIGRes -> Fill(0.);
    }
    
    if( mode == "discovery" )
    {
      h_BKGRes -> Fill(rooNBKGTot->getVal() - NBKGToy);
      h_SIGRes -> Fill(rooNSIG->getVal() - NSIGToy);
    }
    
  }
  
  
  
  outFile -> cd();
  
  h_BKGRes -> Write();
  h_SIGRes -> Write();
  
  outFile -> Close();
}






void SetXSignal(double& xMin_signal, double& xMax_signal, const int& mH)
{
  if(mH == 250)
  {
    xMin_signal = 200.;
    xMax_signal = 350.;
  }
  
  else if(mH == 300)
  {
    xMin_signal = 220.;
    xMax_signal = 420.;
  }
  
  else if(mH == 400)
  {
    xMin_signal = 320.;
    xMax_signal = 520.;
  }
  
  else if(mH == 500)
  {
    xMin_signal = 400.;
    xMax_signal = 650.;
  }
  
  else if(mH == 600)
  {
    xMin_signal = 450.;
    xMax_signal = 800.;
  }

}
