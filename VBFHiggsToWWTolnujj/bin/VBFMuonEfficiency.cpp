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
  std::string outRootFileName = "VBFMuonEfficiency.root";
  TFile outRootFile(outRootFileName.c_str(), "RECREATE");
  outRootFile.cd();
  // Output file
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // --------------------------------------------------------------------------------------
  
  TH1F* nMcMuDistr = new TH1F("nMcMuDistr", "nMcMuDistr", 100, 0., 100);
  TH1F* nMcMuDistr_accept = new TH1F("nMcMuDistr_accept", "nMcMuDistr_accept", 100, 0., 100);
  
  TH1F* nRecoMuDistr = new TH1F("nRecoMuDistr", "nRecoMuDistr", 100, 0., 100);
  
  
  
  TH1F* nMcMuTotDistr = new TH1F("nMcMuTotDistr", "nMcMuTotDistr", 2, 0., 2);
  TH1F* nMcMuTotDistr_fromW1 = new TH1F("nMcMuTotDistr_fromW1", "nMcMuTotDistr_fromW1", 2, 0., 2);
  
  
  
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
  
  int nMcMuTot = 0;
  int nMcMuTot_accept = 0;
  
  int nMcMuTot_fromW1 = 0;
  int nMcMuTot_fromW1_accept = 0;
  
  
  
  // Loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % printModulo) == 0)
      std::cout << ">>>>>> VBFMuonEfficiency::Event number -----> " << std::fixed << std::setw(10) << entry << std::endl;
    //if (entry > 10) break;
    chain -> GetEntry(entry);
    
    
    

    
    
    // --------------------------------------------------------------------------------------
    // FILL HISTOGRAMS
    // --------------------------------------------------------------------------------------
    
    int nMcMu_accept = 0;
    for(unsigned int i = 0; i < treeVars.nMcMu; ++i)
    {
      if( (treeVars.mcMu_pt[i] > 5.) && (fabs(treeVars.mcMu_eta[i]) < 2.4) )
        ++nMcMu_accept;
    }
    
    nMcMuDistr        -> Fill(treeVars.nMcMu);
    nMcMuDistr_accept -> Fill(nMcMu_accept);    
    
    nRecoMuDistr      -> Fill(treeVars.nRecoMu);
    
    
    
    nMcMuTot += treeVars.nMcMu;
    nMcMuTot_accept += nMcMu_accept;
    
    
    
    
    
    
    std::vector<int> matchRecoMuIt;
    GetMatching_recoMu_mcMu(treeVars, treeVars.nMcMu, &matchRecoMuIt);
    
    for(unsigned int i = 0; i < matchRecoMuIt.size(); ++i)
    {
      if( (matchRecoMuIt.at(i) != -1) && 
          (treeVars.mcMu_pt[i] > 5.) && (fabs(treeVars.mcMu_eta[i]) < 2.4) )
      {
        efficiency_vsPt     -> Fill(     treeVars.mcMu_pt[i],   1.);
        efficiency_vsPhi    -> Fill(     treeVars.mcMu_phi[i],  1.);
        efficiency_vsEta    -> Fill(     treeVars.mcMu_eta[i],  1.);
        efficiency_vsAbsEta -> Fill(fabs(treeVars.mcMu_eta[i]), 1.);
      }
      
      if( (matchRecoMuIt.at(i) == -1) && 
          (treeVars.mcMu_pt[i] > 5.) && (fabs(treeVars.mcMu_eta[i]) < 2.4) )
      {
        efficiency_vsPt     -> Fill(     treeVars.mcMu_pt[i],   0.);
        efficiency_vsPhi    -> Fill(     treeVars.mcMu_phi[i],  0.);
        efficiency_vsEta    -> Fill(     treeVars.mcMu_eta[i],  0.);
        efficiency_vsAbsEta -> Fill(fabs(treeVars.mcMu_eta[i]), 0.);
      }

    }
    
    
    
    //---------------------------
    // select only muons from W1
    //---------------------------
    
    if(abs(treeVars.mcF_fromW1_pdgId[0]) != 13)
      continue;
    
    
    
    ++nMcMuTot_fromW1;
    if( (treeVars.mcF_fromW1_pt[0] > 5.) && (fabs(treeVars.mcF_fromW1_eta[0]) < 2.4) )
      ++nMcMuTot_fromW1_accept;
    
    
    
    std::vector<const reco::Particle*> recoMuons;
    for(unsigned int i = 0; i < treeVars.nRecoMu; ++i)
      recoMuons.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.recoMu_px[i],
                                                                        treeVars.recoMu_py[i],
                                                                        treeVars.recoMu_pz[i],
                                                                        treeVars.recoMu_energy[i])));


    std::vector<const reco::Particle*> mcMu_fromW1;
    mcMu_fromW1.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcF_fromW1_px[0],
                                                                        treeVars.mcF_fromW1_py[0],
                                                                        treeVars.mcF_fromW1_pz[0],
                                                                        treeVars.mcF_fromW1_energy[0])));

    
    std::vector<int> matchRecoMuIt_fromW1;
    GetMatching(recoMuons, mcMu_fromW1, 0.05, true, false, &matchRecoMuIt_fromW1);

    for(unsigned int i = 0; i < matchRecoMuIt_fromW1.size(); ++i)
      {
	if( (matchRecoMuIt_fromW1.at(i) != -1) &&
            (treeVars.mcF_fromW1_pt[i] > 5.) && (fabs(treeVars.mcF_fromW1_eta[i]) < 2.4) )
	  {
	    efficiency_fromW1_vsPt     -> Fill(     treeVars.mcF_fromW1_pt[i],   1.);
	    efficiency_fromW1_vsPhi    -> Fill(     treeVars.mcF_fromW1_phi[i],  1.);
	    efficiency_fromW1_vsEta    -> Fill(     treeVars.mcF_fromW1_eta[i],  1.);
	    efficiency_fromW1_vsAbsEta -> Fill(fabs(treeVars.mcF_fromW1_eta[i]), 1.);
	  }

	if( (matchRecoMuIt_fromW1.at(i) == -1) &&
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
  
  nMcMuDistr -> Write();
  nMcMuDistr_accept -> Write();
    
  nRecoMuDistr -> Write();
  
  
  
  nMcMuTotDistr -> SetBinContent(1, nMcMuTot);
  nMcMuTotDistr -> SetBinContent(2, nMcMuTot_accept);
  nMcMuTotDistr -> Write();
  
  nMcMuTotDistr_fromW1 -> SetBinContent(1, nMcMuTot_fromW1);
  nMcMuTotDistr_fromW1 -> SetBinContent(2, nMcMuTot_fromW1_accept);
  nMcMuTotDistr_fromW1 -> Write();
  
  
  
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
