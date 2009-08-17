#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleContent.h"
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFUtils.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <iomanip>

#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TGraphAsymmErrors.h"
#include "TF1.h"
#include "TFile.h"
#include "TProfile.h"
#include "TObject.h"
#include "TLorentzVector.h"
#include "TSystem.h"

#define PI 3.14159265






int main (int argc, char** argv)
{
  // Load ntuple
  TChain* chain = new TChain("ntu");
  VBFNtupleContent treeVars;
  SetBranchAddresses(chain, treeVars);
  // Load ntuple
  
  
  
  // Input files
  //std::string type = "cleanedIterativeCone5CaloJets";
  std::string type = "sisCone5CaloJets";
  std::string inFileName = "VBFNtuple_" + type + ".root";
  std::string inFile = "../test/" + inFileName;
  chain -> Add(inFile.c_str());
  std::cout << inFile << std::endl;
  
  int nEntries = chain -> GetEntries();
  std::cout << "FOUND " << nEntries << " ENTRIES" << std::endl;
  // Input files
  
  
  
  
  
  
  // Output file
  std::string outRootFileName = "VBF4jEfficiency_" + type + ".root";
  TFile outRootFile(outRootFileName.c_str(), "RECREATE");
  outRootFile.cd();
  // Output file
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // --------------------------------------------------------------------------------------

  TH1F* nEvents = new TH1F("nEvents", "nEvents", 3, 0., 3.);
  TH1F* nEvents_eff = new TH1F("nEvents_eff", "nEvents_eff", 3, 0., 3.);  

  TProfile* matchingEff_recoJets_genJetst_firstNj = new TProfile("matchingEff_recoJets_genJetst_firstNj", "matchingEff_recoJets_genJetst_firstNj", 27, 4., 31.);











  
  
 
  // --------------------------------------------------------------------------------------
  // Loop over entries
  // --------------------------------------------------------------------------------------
  
  
  // Loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % 100) == 0)
      std::cout << ">>>>>> VBF4jEfficiency::Event number -----> " << std::fixed << std::setw(10) << entry << std::endl;
    //if (entry > 10) break;
    chain -> GetEntry(entry);
    
    
    

    
    
    // --------------------------------------------------------------------------------------
    // CUT EVENT
    // --------------------------------------------------------------------------------------
    
    nEvents -> Fill(0);
    
    
    
    // check that 4 genJets match with the quarks    
    if( (treeVars.mcTagJet_isMatching[0] != 1) || (treeVars.mcTagJet_isMatching[1] != 1) || 
        (treeVars.mcJet_fromW2_isMatching[0] != 1) || (treeVars.mcJet_fromW2_isMatching[1] != 1) )    
      continue;
    
    nEvents -> Fill(1);    
    
    
    
    // check that there are 4 recoJets matching with the genJets
    int nRecoMatching = GetMatching_recoJets_genJets(treeVars, treeVars.nRecoJet);
    
    if(nRecoMatching != 4)
      continue;
    
    nEvents -> Fill(2);
    
    
    
    
    
    
    // --------------------------------------------------------------------------------------
    // FILL HISTOGRAMS
    // --------------------------------------------------------------------------------------
    
     for(int i = 4; i <= 30; ++i)
     {
       nRecoMatching = GetMatching_recoJets_genJets(treeVars, i);
       
       if(nRecoMatching != 4) matchingEff_recoJets_genJetst_firstNj -> Fill(i, 0);
       else                   matchingEff_recoJets_genJetst_firstNj -> Fill(i, 1);
      }


  } // Loop over entries
  




  
  // --------------------------------------------------------------------------------------
  // Save histograms
  // --------------------------------------------------------------------------------------
  
  nEvents -> Write();
  for(int bin = 1; bin <= nEvents_eff -> GetNbinsX(); ++bin)
    nEvents_eff -> SetBinContent(bin, 1. * nEvents -> GetBinContent(bin) / nEvents -> GetBinContent(1));
  nEvents_eff -> Write();
  
  matchingEff_recoJets_genJetst_firstNj -> Write();
  
  outRootFile.Close();
  
  
  return 0;
}
