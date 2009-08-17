#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleContent.h"
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFUtils.h"

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/ParameterSet/interface/MakeParameterSets.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <algorithm>
#include <vector>
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






struct decreasingOrder:
  public std::binary_function<int, int, bool>
{
  bool operator() (float x, float y)
  {
    return x > y;
  }
};





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
  std::string outRootFileName = "VBFMcJetDistr.root";
  TFile outRootFile(outRootFileName.c_str(), "RECREATE");
  outRootFile.cd();
  // Output file
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Define histograms
  // --------------------------------------------------------------------------------------
  
  TH1F* mc4JetPtDistr_1 = new TH1F("mc4JetPtDistr_1", "mc4JetPtDistr_1", 1000, 0., 1000.);
  TH1F* mc4JetPtDistr_2 = new TH1F("mc4JetPtDistr_2", "mc4JetPtDistr_2", 1000, 0., 1000.);
  TH1F* mc4JetPtDistr_3 = new TH1F("mc4JetPtDistr_3", "mc4JetPtDistr_3", 1000, 0., 1000.);
  TH1F* mc4JetPtDistr_4 = new TH1F("mc4JetPtDistr_4", "mc4JetPtDistr_4", 1000, 0., 1000.);
  
  
  
  TH1F* mcTagJetMaxPtDistr = new TH1F("mcTagJetMaxPtDistr", "mcTagJetMaxPtDistr", 1000, 0., 1000.);
  TH1F* mcTagJetMinPtDistr = new TH1F("mcTagJetMinPtDistr", "mcTagJetMinPtDistr", 1000, 0., 1000.);
  TH1F* mcWJetMaxPtDistr = new TH1F("mcWJetMaxPtDistr", "mcWJetMaxPtDistr", 1000, 0., 1000.);
  TH1F* mcWJetMinPtDistr = new TH1F("mcWJetMinPtDistr", "mcWJetMinPtDistr", 1000, 0., 1000.);
  
  TH2F* mcTagJetMaxPt_vsMcWJetMinPt = new TH2F("mcTagJetMaxPt_vsMcWJetMinPt", "mcTagJetMaxPt_vsMcWJetMinPt", 1000, 0, 1000., 1000, 0., 1000.);
  TH2F* mcTagJetMinPt_vsMcWJetMaxPt = new TH2F("mcTagJetMinPt_vsMcWJetMaxPt", "mcTagJetMinPt_vsMcWJetMaxPt", 1000, 0, 1000., 1000, 0., 1000.);
  
  
  
  TH1F* mcTagJetSumPtDistr = new TH1F("mcTagJetSumPtDistr", "mcTagJetSumPtDistr", 1000, 0., 1000);
  TH1F* mcWJetSumPtDistr = new TH1F("mcWJetSumPtDistr", "mcWJetSumPtDistr", 1000, 0., 1000);
  
  TH2F* mcTagJetSumPt_vsMcWJetSumPt = new TH2F("mcTagJetSumPt_vsMcWJetSumPt", "mcTagJetSumPt_vsMcWJetSumPt", 1000, 0., 1000., 1000, 0., 1000.);
  TH1F* resolvingPower_sumPt = new TH1F("resolvingPower_sumPt", "resolvingPower_sumPt", 3, -1., 2.);
 
  
  
  TH1F* mcTagJetJetPtDistr = new TH1F("mcTagJetJetPtDistr", "mcTagJetJetPtDistr", 1000, 0., 1000.);
  TH1F* mcWJetJetPtDistr = new TH1F("mcWJetJetPtDistr", "mcWJetJetPtDistr", 1000, 0., 1000.);
  
  TH2F* mcTagJetJetPt_vsMcWJetJetPt = new TH2F("mcTagJetJetPt_vsMcWJetJetPt", "mcTagJetJetPt_vsMcWJetJetPt", 1000, 0., 1000., 1000, 0., 1000.);
  TH1F* resolvingPower_pt = new TH1F("resolvingPower_pt", "resolvingPower_pt", 3, -1., 2.);
  
  
  
  TH1F* mcTagJetJetMassDistr = new TH1F("mcTagJetJetMassDistr", "mcTagJetJetMassDistr", 10000, 0., 10000.);
  TH1F* mcWJetJetMassDistr = new TH1F("mcWJetJetMassDistr", "mcWJetJetMassDistr", 1000, 0., 1000.);
  
  TH2F* mcTagJetJetMass_vsMcWJetJetMass = new TH2F("mcTagJetJetMass_vsMcWJetJetMass", "mcTagJetJetMass_vsMcWJetJetMass", 1000, 0., 1000., 10000, 0., 10000.);
  TH1F* resolvingPower_mass = new TH1F("resolvingPower_mass", "resolvingPower_mass", 3, -1., 2.);
  
  
  
  TH1F* mcTagJetJetDetaDistr = new TH1F("mcTagJetJetDetaDistr", "mcTagJetJetDetaDistr", 1000, 0., 10.);
  TH1F* mcWJetJetDetaDistr = new TH1F("mcWJetJetDetaDistr", "mcWJetJetDetaDistr", 1000, 0., 10.);
  
  TH2F* mcTagJetJetDeta_vsMcWJetJetDeta = new TH2F("mcTagJetJetDeta_vsMcWJetJetDeta", "mcTagJetJetDeta_vsMcWJetJetDeta", 1000, 0., 10., 1000, 0., 10.);
  TH1F* resolvingPower_Deta = new TH1F("resolvingPower_Deta", "resolvingPower_Deta", 3, -1., 2.);
  
  
  
  
  
  
  // --------------------------------------------------------------------------------------
  // Loop over entries
  // --------------------------------------------------------------------------------------
  
  int nTot = 0;

  int nTagJetSumPt = 0;
  int nWJetSumPt = 0;
  
  int nTagJetJetPt = 0;
  int nWJetJetPt = 0;
  
  int nTagJetJetMass = 0;
  int nWJetJetMass = 0;

  int nTagJetJetDeta = 0;
  int nWJetJetDeta = 0;
  
  
  
  // Loop over entries
  for (int entry = 0; entry < nEntries; ++entry)
  {
    if ((entry % printModulo) == 0)
      std::cout << ">>>>>> VBFMcJetDistr::Event number -----> " << std::fixed << std::setw(10) << entry << std::endl;
    //if (entry > 10) break;
    chain -> GetEntry(entry);
    
    
    

    
    
    // --------------------------------------------------------------------------------------
    // FILL HISTOGRAMS
    // --------------------------------------------------------------------------------------
    
    // check that 4 genJets match with the quarks    
    if( (treeVars.mcTagJet_isMatching[0] != 1) || (treeVars.mcTagJet_isMatching[1] != 1) || 
        (treeVars.mcJet_fromW2_isMatching[0] != 1) || (treeVars.mcJet_fromW2_isMatching[1] != 1) )    
      continue;
    
    ++nTot;
    
    
    
    
    
    
    std::vector<float> pt4jSorted;
    pt4jSorted.push_back(treeVars.mcTagJet_pt[0]);
    pt4jSorted.push_back(treeVars.mcTagJet_pt[1]);
    pt4jSorted.push_back(treeVars.mcJet_fromW2_pt[0]);
    pt4jSorted.push_back(treeVars.mcJet_fromW2_pt[1]);
    std::sort(pt4jSorted.begin(), pt4jSorted.end(), decreasingOrder());    
    
    mc4JetPtDistr_1 -> Fill(pt4jSorted.at(0));
    mc4JetPtDistr_2 -> Fill(pt4jSorted.at(1));
    mc4JetPtDistr_3 -> Fill(pt4jSorted.at(2));
    mc4JetPtDistr_4 -> Fill(pt4jSorted.at(3));
    
    
    
    
    
    
    mcTagJetMaxPtDistr -> Fill(std::max(treeVars.mcTagJet_pt[0], treeVars.mcTagJet_pt[1]));
    mcTagJetMinPtDistr -> Fill(std::min(treeVars.mcTagJet_pt[0], treeVars.mcTagJet_pt[1]));
    
    mcWJetMaxPtDistr -> Fill(std::max(treeVars.mcJet_fromW2_pt[0], treeVars.mcJet_fromW2_pt[1]));
    mcWJetMinPtDistr -> Fill(std::min(treeVars.mcJet_fromW2_pt[0], treeVars.mcJet_fromW2_pt[1]));
    
    mcTagJetMaxPt_vsMcWJetMinPt -> Fill(std::min(treeVars.mcJet_fromW2_pt[0], treeVars.mcJet_fromW2_pt[1]),
                                        std::max(treeVars.mcTagJet_pt[0], treeVars.mcTagJet_pt[1]));
    mcTagJetMinPt_vsMcWJetMaxPt -> Fill(std::max(treeVars.mcJet_fromW2_pt[0], treeVars.mcJet_fromW2_pt[1]),
                                        std::min(treeVars.mcTagJet_pt[0], treeVars.mcTagJet_pt[1]));
    
    
    
    
    
    //--------
    // sum pt
    //--------
    
    mcTagJetSumPtDistr -> Fill(treeVars.mcTagJet_pt[0] + treeVars.mcTagJet_pt[1]);
    mcWJetSumPtDistr -> Fill(treeVars.mcJet_fromW2_pt[0] + treeVars.mcJet_fromW2_pt[1]);
    
    mcTagJetSumPt_vsMcWJetSumPt -> Fill(treeVars.mcJet_fromW2_pt[0] + treeVars.mcJet_fromW2_pt[1],
                                        treeVars.mcTagJet_pt[0] + treeVars.mcTagJet_pt[1]);
    
    if( (treeVars.mcTagJet_pt[0] + treeVars.mcTagJet_pt[1]) >
        (treeVars.mcJet_fromW2_pt[0] + treeVars.mcJet_fromW2_pt[1]) ) ++ nTagJetSumPt;
    else ++ nWJetSumPt;
    
    
    
    
    
    //-----------
    // jet-jet pt
    //-----------
    
    const reco::Particle* mcTagJetJet = 
      new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcTagJet_px[0] + treeVars.mcTagJet_px[1],
                                                    treeVars.mcTagJet_py[0] + treeVars.mcTagJet_py[1],
                                                    treeVars.mcTagJet_pz[0] + treeVars.mcTagJet_pz[1],
                                                    treeVars.mcTagJet_energy[0] + treeVars.mcTagJet_energy[1]));
    
    const reco::Particle* mcJetJet_fromW2 = 
      new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcJet_fromW2_px[0] + treeVars.mcJet_fromW2_px[1],
                                                    treeVars.mcJet_fromW2_py[0] + treeVars.mcJet_fromW2_py[1],
                                                    treeVars.mcJet_fromW2_pz[0] + treeVars.mcJet_fromW2_pz[1],
                                                    treeVars.mcJet_fromW2_energy[0] + treeVars.mcJet_fromW2_energy[1]));
    
    mcTagJetJetPtDistr -> Fill(mcTagJetJet -> pt());
    mcWJetJetPtDistr -> Fill(mcJetJet_fromW2 -> pt());
    
    mcTagJetJetPt_vsMcWJetJetPt -> Fill(mcJetJet_fromW2 -> pt(), mcTagJetJet -> pt());
    
    if( mcTagJetJet -> pt() > mcJetJet_fromW2 -> pt() ) ++ nTagJetJetPt;
    else ++ nWJetJetPt;
    
    
    
    
    
    
    //--------------
    // jet-jet mass
    //--------------
    
    mcTagJetJetMassDistr -> Fill(mcTagJetJet -> mass());
    mcWJetJetMassDistr -> Fill(mcJetJet_fromW2 -> mass());
    
    mcTagJetJetMass_vsMcWJetJetMass -> Fill(mcJetJet_fromW2 -> mass(), mcTagJetJet -> mass());
    
    if( mcTagJetJet -> mass() > mcJetJet_fromW2 -> mass() ) ++ nTagJetJetMass;
    else ++ nWJetJetMass;
    
    
    
    
    
    
    //-------------------
    // jet-jet Delta eta
    //------------------
    
    mcTagJetJetDetaDistr -> Fill(fabs(treeVars.mcTagJet_eta[0] - treeVars.mcTagJet_eta[1]));
    mcWJetJetDetaDistr -> Fill(fabs(treeVars.mcJet_fromW2_eta[0] - treeVars.mcJet_fromW2_eta[1]));
    
    mcTagJetJetDeta_vsMcWJetJetDeta -> Fill(fabs(treeVars.mcJet_fromW2_eta[0] - treeVars.mcJet_fromW2_eta[1]), 
                                            fabs(treeVars.mcTagJet_eta[0] - treeVars.mcTagJet_eta[1]));
    
    if( fabs(treeVars.mcTagJet_eta[0] - treeVars.mcTagJet_eta[1]) > 
        fabs(treeVars.mcJet_fromW2_eta[0] - treeVars.mcJet_fromW2_eta[1]) ) ++ nTagJetJetDeta;
    else ++ nWJetJetDeta;
    
    
    
  } // Loop over entries
  




  
  // --------------------------------------------------------------------------------------
  // Save histograms
  // --------------------------------------------------------------------------------------
  
  mc4JetPtDistr_1 -> Write();
  mc4JetPtDistr_2 -> Write();
  mc4JetPtDistr_3 -> Write();
  mc4JetPtDistr_4 -> Write();
  
  
  
  
  
  
  mcTagJetMaxPtDistr -> Write();
  mcTagJetMinPtDistr -> Write();
  
  mcWJetMaxPtDistr -> Write();
  mcWJetMinPtDistr -> Write();
  
  mcTagJetMaxPt_vsMcWJetMinPt -> Write();
  mcTagJetMinPt_vsMcWJetMaxPt -> Write();
  
  
  
  
  
  
  mcTagJetSumPtDistr -> Write();
  mcWJetSumPtDistr -> Write();
  
  mcTagJetSumPt_vsMcWJetSumPt -> Write();
  
  resolvingPower_sumPt -> SetBinContent(1, 1. * nWJetSumPt / nTot);
  resolvingPower_sumPt -> SetBinContent(3, 1. * nTagJetSumPt / nTot);
  resolvingPower_sumPt -> Write();
  
  
  
  
  
  
  mcTagJetJetPtDistr -> Write();
  mcWJetJetPtDistr -> Write();
  
  mcTagJetJetPt_vsMcWJetJetPt -> Write();
  
  resolvingPower_pt -> SetBinContent(1, 1. * nWJetJetPt / nTot);
  resolvingPower_pt -> SetBinContent(3, 1. * nTagJetJetPt / nTot);
  resolvingPower_pt -> Write();
  
  
  
  
  
  
  mcTagJetJetMassDistr -> Write();
  mcWJetJetMassDistr -> Write();
  
  mcTagJetJetMass_vsMcWJetJetMass -> Write();
  
  resolvingPower_mass -> SetBinContent(1, 1. * nWJetJetMass / nTot);
  resolvingPower_mass -> SetBinContent(3, 1. * nTagJetJetMass / nTot);
  resolvingPower_mass -> Write();
  
  
  
  
  
  
  mcTagJetJetDetaDistr -> Write();
  mcWJetJetDetaDistr -> Write();
  
  mcTagJetJetDeta_vsMcWJetJetDeta -> Write();
  
  resolvingPower_Deta -> SetBinContent(1, 1. * nWJetJetDeta / nTot);
  resolvingPower_Deta -> SetBinContent(3, 1. * nTagJetJetDeta / nTot);
  resolvingPower_Deta -> Write();
  
  
  
  
  
  
  TF1* bisector = new TF1("bisector", "x", 0., 1000.);
  bisector -> SetNpx(10000);
  bisector -> Write();
  
  
  outRootFile.Close();
  
  
  return 0;
}
