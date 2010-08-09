#ifndef VBFMetDistributions_h
#define VBFMetDistributions_h

#include "HiggsAnalysis/VBFHiggsToVV/interface/VBFUtils.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/METReco/interface/CaloMET.h"
#include "DataFormats/METReco/interface/CaloMETCollection.h"
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
// #include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h" //---- CMSSW_2_X_Y
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h" //---- CMSSW_3_X_Y


#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TH1F.h"
#include "TProfile.h"
#include "TDirectory.h"

#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Vector/ThreeVector.h"






class VBFMetDistributions : public edm::EDAnalyzer
{
 public:
  
  //! ctor
  VBFMetDistributions(const edm::ParameterSet& iConfig);
  
  //! dtor
  ~VBFMetDistributions();
  
  virtual void beginJob(const edm::EventSetup&);
  virtual void endJob();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcGenMetNoNuBSM;
  edm::InputTag m_srcCaloMet;
  edm::InputTag m_srcMuonCorrectedCaloMet;
  edm::InputTag m_srcPFMet;
  
  std::string m_fileName;
  bool m_verbosity;
  int m_eventType;
  int m_eventId;
  
  
  
  TFile* m_outFile;
  
  
  
  // met histograms
  TH1F* genMetNoNuBSMDistr;
  
  TH1F* caloMetDistr;
  TH1F* caloMetRes;
  
  TH1F* muonCorrectedCaloMetDistr;
  TH1F* muonCorrectedCaloMetRes;
  
  TH1F* PFMetDistr;
  TH1F* PFMetRes;
  
};

#endif
