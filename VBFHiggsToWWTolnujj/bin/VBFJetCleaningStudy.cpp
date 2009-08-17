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

#define maxDeltaR 0.2
#define minEleOverJetEratio 0.9
#define maxHEoverEmE 0.2





int main (int argc, char** argv)
{
  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName);
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet();

  edm::ParameterSet subPSetSelections =  parameterSet -> getParameter<edm::ParameterSet>("selections");
  int printModulo          = subPSetSelections.getParameter<int>        ("printModulo");
  std::string jetAlgorithm = subPSetSelections.getParameter<std::string>("jetAlgorithm");

  
  edm::ParameterSet subPSetInput = parameterSet -> getParameter<edm::ParameterSet>("inputNtuples");
  std::vector<std::string> inputFiles = subPSetInput.getParameter<std::vector<std::string> > ("inputFiles");
  
  
  
  
  
  
  // Load ntuple
  TChain* chain = new TChain("ntu");
  VBFNtupleContent treeVars;
  SetBranchAddresses(chain, treeVars);
  // Load ntuple
  
  
  
  // Input files
  for(std::vector<std::string>::const_iterator listIt = inputFiles.begin();
      listIt != inputFiles.end(); ++listIt)
  {
    chain -> Add (listIt -> c_str());
    std::cout << *listIt << std::endl;
  }
  
  int nEntries = chain -> GetEntries();
  std::cout << "FOUND " << nEntries << " ENTRIES" << std::endl;
  // Input files
  
  
  
  
  
  
  // Output file
  std::string outRootFileName = "VBFJetCleaningStudy_" + jetAlgorithm + ".root";
  TFile outRootFile(outRootFileName.c_str(), "RECREATE");
  outRootFile.cd();
  // Output file
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // --------------------------------------------------------------------------------------
  
  TH1F* nRecoJetDistr = new TH1F("nRecoJetDistr", "nRecoJetDistr", 100, 0., 100);

  TH1F* nRecoJetDistr_cleaned1 = new TH1F("nRecoJetDistr_cleaned1", "nRecoJetDistr_cleaned1", 100, 0., 100);
  TH1F* nRecoJetDistr_removed1 = new TH1F("nRecoJetDistr_removed1", "nRecoJetDistr_removed1", 100, 0., 100);
  TH1F* nErrors_removed1 = new TH1F("nErrors_removed1", "nErrors_removed1", 10, 0., 10);
  TH1F* nErrors_kept1 = new TH1F("nErrors_kept1", "nErrors_kept1", 10, 0., 10);
  TH1F* n4jRemoved1 = new TH1F("n4jRemoved1", "n4jRemoved1", 5, 0., 5);

  TH1F* nRecoJetDistr_cleaned2 = new TH1F("nRecoJetDistr_cleaned2", "nRecoJetDistr_cleaned2", 100, 0., 100);
  TH1F* nRecoJetDistr_removed2 = new TH1F("nRecoJetDistr_removed2", "nRecoJetDistr_removed2", 100, 0., 100);
  TH1F* nErrors_removed2 = new TH1F("nErrors_removed2", "nErrors_removed2", 10, 0., 10);
  TH1F* nErrors_kept2 = new TH1F("nErrors_kept2", "nErrors_kept2", 10, 0., 10);  
  TH1F* n4jRemoved2 = new TH1F("n4jRemoved2", "n4jRemoved2", 5, 0., 5);  

  TH1F* nRecoJetDistr_cleaned3 = new TH1F("nRecoJetDistr_cleaned3", "nRecoJetDistr_cleaned3", 100, 0., 100);
  TH1F* nRecoJetDistr_removed3 = new TH1F("nRecoJetDistr_removed3", "nRecoJetDistr_removed3", 100, 0., 100);
  TH1F* nErrors_removed3 = new TH1F("nErrors_removed3", "nErrors_removed3", 10, 0., 10);
  TH1F* nErrors_kept3 = new TH1F("nErrors_kept3", "nErrors_kept3", 10, 0., 10);  
  TH1F* n4jRemoved3 = new TH1F("n4jRemoved3", "n4jRemoved3", 5, 0., 5);    

  
  
  
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Loop over entries
  // --------------------------------------------------------------------------------------
  
  
  // Loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % printModulo) == 0)
      std::cout << ">>>>>> VBFJetCleaningStudy::Event number -----> " << std::fixed << std::setw(10) << entry << std::endl;
    //if (entry > 10) break;
    chain -> GetEntry(entry);
    
    
    

    
    
    // --------------------------------------------------------------------------------------
    // FILL HISTOGRAMS
    // --------------------------------------------------------------------------------------
    
    nRecoJetDistr -> Fill(treeVars.nRecoJet);






    // -------------------------------------------------------------------------
    
    std::vector<int> recoJetIt_mcE_removed;
    for(unsigned int jetIt = 0; jetIt < treeVars.nRecoJet; ++jetIt)
    {
      double electronsEnergy = 0.;
      
      int counter = 0;
      for(unsigned int eleIt = 0; eleIt < treeVars.nMcE; ++eleIt)
      {
        if( (treeVars.mcE_pt[eleIt] < 5.) || (fabs(treeVars.mcE_eta[eleIt]) > 2.4) )
          continue;
         
    
        double DR = deltaR(treeVars.mcE_eta[eleIt],   treeVars.mcE_phi[eleIt],
                           treeVars.recoJet_eta[jetIt], treeVars.recoJet_phi[jetIt]);
        
        if(DR < maxDeltaR)
        {
          ++counter;
          electronsEnergy += treeVars.mcE_energy[eleIt];
        }          
      }
      
      
      if ( (counter) && ((electronsEnergy / treeVars.recoJet_energy[jetIt]) > minEleOverJetEratio) )
      {
        recoJetIt_mcE_removed.push_back(jetIt);
        continue;
      }      
    }

    // -------------------------------------------------------------------------




    
    
    
    
    
         
    // -------------------
    // 1st method (old one)
    // -------------------

    int nRecoJet_cleaned = 0;
    std::vector<int> recoJetIt_recoE_removed1;
    for(unsigned int jetIt = 0; jetIt < treeVars.nRecoJet; ++jetIt)
    {
      double electronsEnergy = 0.;
      
      int counter = 0;
      for(unsigned int eleIt = 0; eleIt < treeVars.nRecoE; ++eleIt)
      {
        //PG this is not an electron
        if(fabs(treeVars.recoE_HOverEm[eleIt]) > maxHEoverEmE)
          continue;
        if(fabs(treeVars.recoE_tkIso[eleIt] / treeVars.recoE_p[eleIt]) > 0.2)
          continue;
        
        
        double DR = deltaR(treeVars.recoE_eta[eleIt],   treeVars.recoE_phi[eleIt],
                           treeVars.recoJet_eta[jetIt], treeVars.recoJet_phi[jetIt]);
        
        if(DR < maxDeltaR)
        {
          ++counter;
          electronsEnergy += treeVars.recoE_SuperClusterEnergy[eleIt];
        }          
      }
      
      
      if ( (counter) && ((electronsEnergy / treeVars.recoJet_energy[jetIt]) > minEleOverJetEratio) )
      {
        recoJetIt_recoE_removed1.push_back(jetIt);
        continue;
      }
      
      ++nRecoJet_cleaned;
    }
    
    nRecoJetDistr_cleaned1 -> Fill(nRecoJet_cleaned);
    nRecoJetDistr_removed1 -> Fill(treeVars.nRecoJet - nRecoJet_cleaned);
    
    
    
    int nKept = 0;
    for(unsigned int i = 0; i < recoJetIt_mcE_removed.size(); ++i)
    {
      int it1 = recoJetIt_mcE_removed.at(i);
      
      bool found = false;
      for(unsigned int j = 0; j < recoJetIt_recoE_removed1.size(); ++j)
        if(recoJetIt_recoE_removed1.at(j) == it1) found = true;
      
      if(found == false)
        ++nKept;
    }
    if(recoJetIt_mcE_removed.size() > 0)
      nErrors_kept1 -> Fill(nKept);
    
    int nRemoved = 0;
    for(unsigned int i = 0; i < recoJetIt_recoE_removed1.size(); ++i)
    {
      int it1 = recoJetIt_recoE_removed1.at(i);
      
      bool found = false;
      for(unsigned int j = 0; j < recoJetIt_mcE_removed.size(); ++j)
        if(recoJetIt_mcE_removed.at(j) == it1) found = true;
      
      if(found == false)
        ++nRemoved;
    }
    if(recoJetIt_recoE_removed1.size() > 0)
      nErrors_removed1 -> Fill(nRemoved); 
    
    
    
    
    
    
    // ------------------------
    // 2st method - eleId loose
    // ------------------------

    nRecoJet_cleaned = 0;
    std::vector<int> recoJetIt_recoE_removed2;
    for(unsigned int jetIt = 0; jetIt < treeVars.nRecoJet; ++jetIt)
    {
      double electronsEnergy = 0.;
      
      int counter = 0;
      for(unsigned int eleIt = 0; eleIt < treeVars.nRecoE; ++eleIt)
      {
        if(fabs(treeVars.recoE_tkIso[eleIt] / treeVars.recoE_p[eleIt]) > 0.2)
          continue;
        if(treeVars.recoE_eleIdRobustLoose[eleIt] == 0)
          continue;
        
        
        double DR = deltaR(treeVars.recoE_eta[eleIt],   treeVars.recoE_phi[eleIt],
                           treeVars.recoJet_eta[jetIt], treeVars.recoJet_phi[jetIt]);      
        
        if(DR < maxDeltaR)
        {
          ++counter;
          electronsEnergy += treeVars.recoE_SuperClusterEnergy[eleIt];
        }          
      }
      
      
      if ( (counter) && ((electronsEnergy / treeVars.recoJet_energy[jetIt]) > minEleOverJetEratio) )
      {
        recoJetIt_recoE_removed2.push_back(jetIt);
        continue;
      }
      
      ++nRecoJet_cleaned;
      
    }
    
    nRecoJetDistr_cleaned2 -> Fill(nRecoJet_cleaned);
    nRecoJetDistr_removed2 -> Fill(treeVars.nRecoJet - nRecoJet_cleaned);
    
    
    
    nKept = 0;
    for(unsigned int i = 0; i < recoJetIt_mcE_removed.size(); ++i)
    {
      int it1 = recoJetIt_mcE_removed.at(i);
      
      bool found = false;
      for(unsigned int j = 0; j < recoJetIt_recoE_removed2.size(); ++j)
        if(recoJetIt_recoE_removed2.at(j) == it1) found = true;
      
      if(found == false)
        ++nKept;
    }
    if(recoJetIt_mcE_removed.size() > 0)
      nErrors_kept2 -> Fill(nKept); 
    
    nRemoved = 0;
    for(unsigned int i = 0; i < recoJetIt_recoE_removed2.size(); ++i)
    {
      int it1 = recoJetIt_recoE_removed2.at(i);
      
      bool found = false;
      for(unsigned int j = 0; j < recoJetIt_mcE_removed.size(); ++j)
        if(recoJetIt_mcE_removed.at(j) == it1) found = true;
      
      if(found == false)
        ++nRemoved;
    }
    if(recoJetIt_recoE_removed2.size() > 0)
      nErrors_removed2 -> Fill(nRemoved); 
    
    
    
    
    
    
    // ------------------------
    // 3st method - eleId tight
    // ------------------------

    nRecoJet_cleaned = 0;
    std::vector<int> recoJetIt_recoE_removed3;
    for(unsigned int jetIt = 0; jetIt < treeVars.nRecoJet; ++jetIt)
    {
      double electronsEnergy = 0.;
      
      int counter = 0;
      for(unsigned int eleIt = 0; eleIt < treeVars.nRecoE; ++eleIt)
      {
        if(fabs(treeVars.recoE_tkIso[eleIt] / treeVars.recoE_p[eleIt]) > 0.2)
          continue;
        if(treeVars.recoE_eleIdRobustTight[eleIt] == 0)
          continue;
        
        
        double DR = deltaR(treeVars.recoE_eta[eleIt],   treeVars.recoE_phi[eleIt],
                           treeVars.recoJet_eta[jetIt], treeVars.recoJet_phi[jetIt]);      
        
        if(DR < maxDeltaR)
        {
          ++counter;
          electronsEnergy += treeVars.recoE_SuperClusterEnergy[eleIt];
        }          
      }
      
      
      if ( (counter) && ((electronsEnergy / treeVars.recoJet_energy[jetIt]) > minEleOverJetEratio) )
      {
        recoJetIt_recoE_removed3.push_back(jetIt);
        continue;
      }
      
      ++nRecoJet_cleaned;
      
    }
    
    nRecoJetDistr_cleaned3 -> Fill(nRecoJet_cleaned);
    nRecoJetDistr_removed3 -> Fill(treeVars.nRecoJet - nRecoJet_cleaned);
    
    
    
    nKept = 0;
    for(unsigned int i = 0; i < recoJetIt_mcE_removed.size(); ++i)
    {
      int it1 = recoJetIt_mcE_removed.at(i);
      
      bool found = false;
      for(unsigned int j = 0; j < recoJetIt_recoE_removed3.size(); ++j)
        if(recoJetIt_recoE_removed3.at(j) == it1) found = true;
      
      if(found == false)
        ++nKept;
    }
    if(recoJetIt_mcE_removed.size() > 0)
      nErrors_kept3 -> Fill(nKept); 
    
    nRemoved = 0;
    for(unsigned int i = 0; i < recoJetIt_recoE_removed3.size(); ++i)
    {
      int it1 = recoJetIt_recoE_removed3.at(i);
      
      bool found = false;
      for(unsigned int j = 0; j < recoJetIt_mcE_removed.size(); ++j)
        if(recoJetIt_mcE_removed.at(j) == it1) found = true;
      
      if(found == false)
        ++nRemoved;
    }
    if(recoJetIt_recoE_removed3.size() > 0)
      nErrors_removed3 -> Fill(nRemoved);     
    
    
    
    
    
    
    
    
    
    
    
    
    if( (treeVars.mcTagJet_isMatching[0] != 1) || (treeVars.mcTagJet_isMatching[1] != 1) ||
        (treeVars.mcJet_fromW2_isMatching[0] != 1) || (treeVars.mcJet_fromW2_isMatching[1] != 1) )
      continue;
    
    
    std::vector<const reco::Particle*> genJets;
    genJets.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcTagJet_px[0],
								    treeVars.mcTagJet_py[0],
								    treeVars.mcTagJet_pz[0],
								    treeVars.mcTagJet_energy[0])));
    genJets.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcTagJet_px[1],
								    treeVars.mcTagJet_py[1],
								    treeVars.mcTagJet_pz[1],
								    treeVars.mcTagJet_energy[1])));
    genJets.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcJet_fromW2_px[0],
								    treeVars.mcJet_fromW2_py[0],
								    treeVars.mcJet_fromW2_pz[0],
								    treeVars.mcJet_fromW2_energy[0])));
    genJets.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcJet_fromW2_px[1],
								    treeVars.mcJet_fromW2_py[1],
								    treeVars.mcJet_fromW2_pz[1],
								    treeVars.mcJet_fromW2_energy[1])));
    
    std::vector<const reco::Particle*> recoJets_removed1;
    for(unsigned int i = 0; i < recoJetIt_recoE_removed1.size(); ++i)
      recoJets_removed1.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.recoJet_px[recoJetIt_recoE_removed1.at(i)],
                                                                                treeVars.recoJet_py[recoJetIt_recoE_removed1.at(i)],
                                                                                treeVars.recoJet_pz[recoJetIt_recoE_removed1.at(i)],
                                                                                treeVars.recoJet_energy[recoJetIt_recoE_removed1.at(i)])));
    
    int nMatching1 = GetMatching(recoJets_removed1, genJets, 0.3, false, false);
    n4jRemoved1 -> Fill(nMatching1);    
    
    
    
    std::vector<const reco::Particle*> recoJets_removed2;
    for(unsigned int i = 0; i < recoJetIt_recoE_removed2.size(); ++i)
      recoJets_removed2.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.recoJet_px[recoJetIt_recoE_removed2.at(i)],
                                                                                treeVars.recoJet_py[recoJetIt_recoE_removed2.at(i)],
                                                                                treeVars.recoJet_pz[recoJetIt_recoE_removed2.at(i)],
                                                                                treeVars.recoJet_energy[recoJetIt_recoE_removed2.at(i)])));
    
    int nMatching2 = GetMatching(recoJets_removed2, genJets, 0.3, false, false);
    n4jRemoved2 -> Fill(nMatching2);
    
    
    
    std::vector<const reco::Particle*> recoJets_removed3;
    for(unsigned int i = 0; i < recoJetIt_recoE_removed3.size(); ++i)
      recoJets_removed3.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.recoJet_px[recoJetIt_recoE_removed3.at(i)],
                                                                                treeVars.recoJet_py[recoJetIt_recoE_removed3.at(i)],
                                                                                treeVars.recoJet_pz[recoJetIt_recoE_removed3.at(i)],
                                                                                treeVars.recoJet_energy[recoJetIt_recoE_removed3.at(i)])));
    int nMatching3 = GetMatching(recoJets_removed3, genJets, 0.3, false, false);
    n4jRemoved3 -> Fill(nMatching3);
  } // Loop over entries
  




  
  // --------------------------------------------------------------------------------------
  // Save histograms
  // --------------------------------------------------------------------------------------
  
  nRecoJetDistr -> Write();
  
  nRecoJetDistr_cleaned1 -> Write();
  nRecoJetDistr_removed1 -> Write();
  nErrors_kept1    -> Write();
  nErrors_removed1 -> Write();
  n4jRemoved1 -> Scale(1. / n4jRemoved2 -> GetEntries());
  n4jRemoved1 -> Write();
  
  nRecoJetDistr_cleaned2 -> Write();
  nRecoJetDistr_removed2 -> Write();
  nErrors_kept2    -> Write();
  nErrors_removed2 -> Write();
  n4jRemoved2 -> Scale(1. / n4jRemoved2 -> GetEntries());
  n4jRemoved2 -> Write();   

  nRecoJetDistr_cleaned3 -> Write();
  nRecoJetDistr_removed3 -> Write();
  nErrors_kept3    -> Write();
  nErrors_removed3 -> Write();
  n4jRemoved3 -> Scale(1. / n4jRemoved3 -> GetEntries());
  n4jRemoved3 -> Write(); 
  
  
  outRootFile.Close();
  
  
  return 0;
}
