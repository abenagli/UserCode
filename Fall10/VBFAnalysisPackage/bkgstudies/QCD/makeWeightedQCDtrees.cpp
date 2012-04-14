/*
To compile
g++ makeWeightedQCDtrees.cpp -o makeWeightedQCDtrees `root-config --cflags --glibs` -lRooFitCore -lRooFit -lMinuit -lFoam
*/

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>

#include "TH1F.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TNtuple.h"

#include "RooMsgService.h"
#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooAbsPdf.h"
#include "RooHistPdf.h"
#include "RooGenericPdf.h"
#include "RooWorkspace.h"
#include "RooGaussian.h"
#include "RooAddPdf.h"
#include "RooPlot.h"

int main (int argc, char** argv) {

  
  std::string OldPath = "/gwteray/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_EGMu_PFlow_QCDSelection_noEleHLTcorr_NoIso_NoId_NoMET/";
  std::string NewPath = "/gwteray/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_EGMu_PFlow_noEleHLTcorr_NoMET/";
  
  std::string fileName = "VBFAnalysis_PFlow.root";

  std::vector<std::string> DATA_names;
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011A-May10ReReco-v1/");
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011A-05Aug2011-v1/");
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011A-PromptReco-v6/");
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011A-PromptReco-v4/");
  DATA_names.push_back("data_SingleEle_SingleMu_Run2011B-PromptReco-v1/");

  std::vector<float> v_lumi;
  v_lumi.push_back(211.02);
  v_lumi.push_back(136.43);
  v_lumi.push_back(818.85);
  v_lumi.push_back(1096.58);
  v_lumi.push_back(2715.89);
  
  float lumiTot = 0;
  for ( unsigned int iPeriod = 0; iPeriod < v_lumi.size(); iPeriod++ ) lumiTot += v_lumi[iPeriod];
  
  TFile* inFile_nQCD = new TFile("QCD_fit.root","READ");
  TNtuple* ntuple = (TNtuple*) inFile_nQCD -> Get("ntuple");
  float QCDtotEle;  
  float QCDtotMu;  
  ntuple ->SetBranchAddress("n_QCD_ele",&QCDtotEle);
  ntuple ->SetBranchAddress("n_QCD_mu",&QCDtotMu);
  ntuple -> GetEntry(0);
  
  float OldEventWeight;
  float NewEventWeight;
  int   totEvents;
  int   survEvents;
  int   lep_flavour;
    
  float lumi = 5000.;
  std::string s_lumi = "5000.";
    
  int step_min = 10;
  int step_max = 13;
  char treeName[50];
  
  for ( unsigned int iFile = 0; iFile < DATA_names.size(); iFile++ ) {

    TFile* oldfile = new TFile((OldPath + DATA_names[iFile] + fileName).c_str(),"READ");  
    TH1F* events = (TH1F*) oldfile -> Get("events");
    TH1F* events_PURescaled = (TH1F*) oldfile -> Get("events_PURescaled");
 
    TFile* newfile = new TFile((NewPath + "QCD_" + DATA_names[iFile] + fileName).c_str(),"RECREATE");
   
    for ( int istep = step_min; istep <= step_max; istep++ ) {
    
      sprintf(treeName, "ntu_%d", istep);
      oldfile -> cd();
      TTree* oldtree = (TTree*) oldfile -> Get(treeName);
      survEvents = oldtree->GetEntries();
      oldtree->SetBranchAddress("eventWeight",&OldEventWeight);
      oldtree->SetBranchAddress("lep_flavour",&lep_flavour);
      oldtree->SetBranchAddress("totEvents",&totEvents);

      newfile -> cd();
      TTree* newtree = oldtree -> CloneTree(0);
      newtree->SetBranchAddress("eventWeight",&NewEventWeight);
      
      TH1F* tempHisto = new TH1F("tempHisto","tempHisto",3,11,14);
      oldtree -> Draw("lep_flavour >> tempHisto","","goff");
      float nEle = tempHisto -> GetBinContent(1);
      float nMu  = tempHisto -> GetBinContent(3);
      delete tempHisto;
      
      for ( int iEntry = 0; iEntry < survEvents; iEntry++ ) {
      
        oldtree -> GetEntry(iEntry);
        if ( lep_flavour == 11 )  NewEventWeight = OldEventWeight * v_lumi[iFile]/lumiTot/lumiTot * QCDtotEle * totEvents / nEle;
        if ( lep_flavour == 13 )  NewEventWeight = OldEventWeight * v_lumi[iFile]/lumiTot/lumiTot * QCDtotMu  * totEvents / nMu;
        newtree -> Fill();
      
      } 

      newtree -> AutoSave();

      if ( istep == step_min ) {
        float nRef = events -> GetBinContent(step_min);
        events -> Scale ( 1./nRef * (QCDtotEle + QCDtotMu) * v_lumi[iFile]/lumiTot/lumiTot * totEvents );
        nRef = events_PURescaled -> GetBinContent(step_min);
        events_PURescaled -> Scale ( 1./nRef * (QCDtotEle + QCDtotMu) * v_lumi[iFile]/lumiTot/lumiTot * totEvents ); 
      }

      events -> SetBinContent(1,totEvents);
      events_PURescaled -> SetBinContent(1,totEvents);
           
      delete oldtree;  
      delete newtree;
      
    }

    events -> Write();
    events_PURescaled -> Write();
    
    delete events;
    delete events_PURescaled;

    oldfile -> Close();
    newfile -> Close();
    
    delete oldfile;
    delete newfile;
    
  
  }
  
  return 1;
}
