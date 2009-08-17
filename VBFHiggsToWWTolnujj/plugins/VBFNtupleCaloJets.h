#ifndef VBFNtupleCaloJets_h
#define VBFNtupleCaloJets_h

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleContent.h"
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/MCAnalysis.h"
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/MCUtils.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"

#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"

#define MCEMAX 25
#define MCMUMAX 25
#define MCJETMAX 75

#define RECOEMAX 25
#define RECOMUMAX 25
#define RECOJETMAX 75






class VBFNtupleCaloJets : public edm::EDAnalyzer
{
 public:
  
  VBFNtupleCaloJets(const edm::ParameterSet& iConfig);
  ~VBFNtupleCaloJets();
  
  virtual void beginJob(const edm::EventSetup&);
  virtual void endJob();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag srcGenParticles_p;
  edm::InputTag srcGenJets_p;
  edm::InputTag srcRecoJets_p;
  edm::InputTag srcRecoElectrons_p;
  edm::InputTag srcRecoElectronTkIsolations_p;
  edm::InputTag srcRecoElectronEmIsolations_p;
  edm::InputTag srcRecoElectronHadIsolations_p;
  edm::InputTag srcRecoElectronIdLoose_p;
  edm::InputTag srcRecoElectronIdTight_p;
  edm::InputTag srcRecoElectronIdRobustLoose_p;
  edm::InputTag srcRecoElectronIdRobustTight_p;
  edm::InputTag srcRecoMuons_p;
  
  std::string fileName_p;
  bool verbosity_p;
  int eventsToPrint_p;
  
  int eventType_p;
  int eventId_p;
  
  
  
  TFile* outFile_p;
  TTree* tree_p;
  
  
  VBFNtupleContent treeVars_p;
};

#endif
