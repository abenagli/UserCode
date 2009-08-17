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
  std::string fileName(argv[1]);
  boost::shared_ptr<edm::ProcessDesc> processDesc = edm::readConfigFile(fileName);
  boost::shared_ptr<edm::ParameterSet> parameterSet = processDesc->getProcessPSet();

  edm::ParameterSet subPSetSelections =  parameterSet -> getParameter<edm::ParameterSet>("selections");
  int printModulo          = subPSetSelections.getParameter<int>        ("printModulo");

  
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
  std::string outRootFileName = "VBFElectronEfficiency.root";
  TFile outRootFile(outRootFileName.c_str(), "RECREATE");
  outRootFile.cd();
  // Output file
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // --------------------------------------------------------------------------------------
  
  TH1F* nMcEDistr = new TH1F("nMcEDistr", "nMcEDistr", 100, 0., 100);
  TH1F* nMcEDistr_accept = new TH1F("nMcEDistr_accept", "nMcEDistr_accept", 100, 0., 100);

  TH1F* nRecoEDistr = new TH1F("nRecoEDistr", "nRecoEDistr", 100, 0., 100);
  
  TH1F* nMcETotDistr = new TH1F("nMcETotDistr", "nMcETotDistr", 2, 0., 2);
  TH1F* nMcETotDistr_fromW1 = new TH1F("nMcETotDistr_fromW1", "nMcETotDistr_fromW1", 2, 0., 2);
  
  
  
  TH1F* DRDistr = new TH1F("DRDistr", "DRDistr", 1000, 0., 10.);
  TH1F* ptRatioDistr = new TH1F("ptRatioDistr", "ptRatioDistr", 1000, 0., 2.);

  TH1F* DRMatchedDistr = new TH1F("DRMatchedDistr", "DRMatchedDistr", 1000, 0., 10.);
  TH1F* ptRatioMatchedDistr = new TH1F("ptRatioMatchedDistr", "ptRatioMatchedDistr", 1000, 0., 2.);
  
  
  
  TProfile* efficiency_vsPt = new TProfile("efficiency_vsPt", "efficiency_vsPt", 500, 0., 500.);
  TProfile* efficiency_vsPhi = new TProfile("efficiency_vsPhi", "efficiency_vsPhi", 200, -5., 5.);
  TProfile* efficiency_vsEta = new TProfile("efficiency_vsEta", "efficiency_vsEta", 200, -5., 5.);
  TProfile* efficiency_vsAbsEta = new TProfile("efficiency_vsAbsEta", "efficiency_vsAbsEta", 100, 0., 5.);
  
  
  
  TProfile* efficiency_fromW1_vsPt = new TProfile("effciency_fromW1_vsPt", "effciency_fromW1_vsPt", 500, 0., 500.);
  TProfile* efficiency_fromW1_vsPhi = new TProfile("effciency_fromW1_vsPhi", "effciency_fromW1_vsPhi", 200, -5., 5.);
  TProfile* efficiency_fromW1_vsEta = new TProfile("effciency_fromW1_vsEta", "effciency_fromW1_vsEta", 200, -5., 5.);
  TProfile* efficiency_fromW1_vsAbsEta = new TProfile("effciency_fromW1_vsAbsEta", "effciency_fromW1_vsAbsEta", 100, 0., 5.);
  
  
  
  
  
  
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Loop over entries
  // --------------------------------------------------------------------------------------

  int nMcETot = 0;
  int nMcETot_accept = 0;
  
  int nMcETot_fromW1 = 0;
  int nMcETot_fromW1_accept = 0;
  
  
  
  // Loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % printModulo) == 0)
      std::cout << ">>>>>> VBFElectronEfficiency::Event number -----> " << std::fixed << std::setw(10) << entry << std::endl;
    //if (entry > 10) break;
    chain -> GetEntry(entry);
    
    
    

    
    
    // --------------------------------------------------------------------------------------
    // FILL HISTOGRAMS
    // --------------------------------------------------------------------------------------

    int nMcE_accept = 0;
    for(unsigned int i = 0; i < treeVars.nMcE; ++i)
    {
      if( (treeVars.mcE_pt[i] > 5.) && (fabs(treeVars.mcE_eta[i]) < 2.4) )
        ++nMcE_accept;
    }
    
    nMcEDistr        -> Fill(treeVars.nMcE);
    nMcEDistr_accept -> Fill(nMcE_accept);
    
    nRecoEDistr      -> Fill(treeVars.nRecoE);
    
    
    
    nMcETot += treeVars.nMcE;
    nMcETot_accept += nMcE_accept;
    
    
    
    
    
    
    for(unsigned int i = 0; i < treeVars.nMcE; ++ i)
    {
      for(unsigned int j = 0; j < treeVars.nRecoE; ++j)
      {
        float DR = deltaR(treeVars.mcE_eta[i], treeVars.mcE_phi[i],
                          treeVars.recoE_eta[j], treeVars.recoE_phi[j]);
        
        DRDistr -> Fill(DR);
        ptRatioDistr -> Fill(1. * treeVars.recoE_pt[j] / treeVars.mcE_pt[i]);
      }
    }
    
    
    
    
    
    
    std::vector<int> matchRecoEIt;
    GetMatching_recoE_mcE(treeVars, treeVars.nMcE, &matchRecoEIt);
    
    for(unsigned int i = 0; i < matchRecoEIt.size(); ++i)
    {
      if( (matchRecoEIt.at(i) != -1) && 
          (treeVars.mcE_pt[i] > 5.) && (fabs(treeVars.mcE_eta[i]) < 2.4) )
      {
        efficiency_vsPt     -> Fill(     treeVars.mcE_pt[i],   1.);
        efficiency_vsPhi    -> Fill(     treeVars.mcE_phi[i],  1.);
        efficiency_vsEta    -> Fill(     treeVars.mcE_eta[i],  1.);
        efficiency_vsAbsEta -> Fill(fabs(treeVars.mcE_eta[i]), 1.);
      }
      
      if( (matchRecoEIt.at(i) == -1) && 
          (treeVars.mcE_pt[i] > 5.) && (fabs(treeVars.mcE_eta[i]) < 2.4) )
      {
        efficiency_vsPt     -> Fill(     treeVars.mcE_pt[i],   0.);
        efficiency_vsPhi    -> Fill(     treeVars.mcE_phi[i],  0.);
        efficiency_vsEta    -> Fill(     treeVars.mcE_eta[i],  0.);
        efficiency_vsAbsEta -> Fill(fabs(treeVars.mcE_eta[i]), 0.);
      }

    }
    
    
    
    //------------------------------
    // select only electrons from W1
    //------------------------------
    
    if(abs(treeVars.mcF_fromW1_pdgId[0]) != 11)
      continue;
    
    
    
    ++nMcETot_fromW1;
    if( (treeVars.mcF_fromW1_pt[0] > 5.) && (fabs(treeVars.mcF_fromW1_eta[0]) < 2.4) )
      ++nMcETot_fromW1_accept;
    
    
    
    std::vector<const reco::Particle*> recoElectrons;
    for(unsigned int i = 0; i < treeVars.nRecoE; ++i)
      recoElectrons.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.recoE_px[i],
                                                                            treeVars.recoE_py[i],
                                                                            treeVars.recoE_pz[i],
                                                                            treeVars.recoE_energy[i])));
    
    
    std::vector<const reco::Particle*> mcE_fromW1;
    mcE_fromW1.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcF_fromW1_px[0],
                                                                       treeVars.mcF_fromW1_py[0],
                                                                       treeVars.mcF_fromW1_pz[0],
                                                                       treeVars.mcF_fromW1_energy[0])));

    
    std::vector<int> matchRecoEIt_fromW1;
    GetMatching(recoElectrons, mcE_fromW1, 0.05, true, false, &matchRecoEIt_fromW1);
    
    for(unsigned int i = 0; i < matchRecoEIt_fromW1.size(); ++i)
    {
      if( (matchRecoEIt_fromW1.at(i) != -1) &&
          (treeVars.mcF_fromW1_pt[i] > 5.) && (fabs(treeVars.mcF_fromW1_eta[i]) < 2.4) )
      {
        efficiency_fromW1_vsPt     -> Fill(     treeVars.mcF_fromW1_pt[i],   1.);
        efficiency_fromW1_vsPhi    -> Fill(     treeVars.mcF_fromW1_phi[i],  1.);
        efficiency_fromW1_vsEta    -> Fill(     treeVars.mcF_fromW1_eta[i],  1.);
        efficiency_fromW1_vsAbsEta -> Fill(fabs(treeVars.mcF_fromW1_eta[i]), 1.);
      }
      
      if( (matchRecoEIt_fromW1.at(i) == -1) &&
          (treeVars.mcF_fromW1_pt[i] > 5.) && (fabs(treeVars.mcF_fromW1_eta[i]) < 2.4) )
      {
        efficiency_fromW1_vsPt     -> Fill(     treeVars.mcF_fromW1_pt[i],   0.);
        efficiency_fromW1_vsPhi    -> Fill(     treeVars.mcF_fromW1_phi[i],  0.);
        efficiency_fromW1_vsEta    -> Fill(     treeVars.mcF_fromW1_eta[i],  0.);
        efficiency_fromW1_vsAbsEta -> Fill(fabs(treeVars.mcF_fromW1_eta[i]), 0.);
      }

    }
    
  } // Loop over entries
  




  
  // --------------------------------------------------------------------------------------
  // Save histograms
  // --------------------------------------------------------------------------------------
  
  nMcEDistr -> Write();
  nMcEDistr_accept -> Write();
    
  nRecoEDistr -> Write();
  
  
  
  nMcETotDistr -> SetBinContent(1, nMcETot);
  nMcETotDistr -> SetBinContent(2, nMcETot_accept);
  nMcETotDistr -> Write();
  
  nMcETotDistr_fromW1 -> SetBinContent(1, nMcETot_fromW1);
  nMcETotDistr_fromW1 -> SetBinContent(2, nMcETot_fromW1_accept);
  nMcETotDistr_fromW1 -> Write();
  
  
  
  DRDistr -> Write();
  ptRatioDistr -> Write();
  
  
  
  efficiency_vsPt -> Write();
  efficiency_vsPhi -> Write();
  efficiency_vsEta -> Write();
  efficiency_vsAbsEta -> Write();
  
  
  efficiency_fromW1_vsPt -> Write();
  efficiency_fromW1_vsPhi -> Write();
  efficiency_fromW1_vsEta -> Write();
  efficiency_fromW1_vsAbsEta -> Write();
  
  
  outRootFile.Close();
  
  
  return 0;
}
