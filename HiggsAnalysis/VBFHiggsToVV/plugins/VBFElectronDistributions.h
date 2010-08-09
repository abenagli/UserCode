#ifndef VBFElectronDistributions_h
#define VBFElectronDistributions_h

#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumperVBF.h"
#include "HiggsAnalysis/VBFHiggsToVV/interface/VBFUtils.h"

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
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h" //---- CMSSW_2_X_Y
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h" //---- CMSSW_3_X_Y
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"

#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TH1F.h"






class VBFElectronDistributions : public edm::EDAnalyzer
{
 public:
  
  //! ctor
  VBFElectronDistributions(const edm::ParameterSet& iConfig);
  
  //! dtor
  ~VBFElectronDistributions();
  
  virtual void beginJob(const edm::EventSetup&);
  virtual void endJob();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcGenParticles;
  edm::InputTag m_srcElectrons;
  edm::InputTag m_srcElectronTkIsoValues;
  edm::InputTag m_srcElectronEmIsoValues;
  edm::InputTag m_srcElectronHadIsoValues;
  edm::InputTag m_srcElectronIdLooseValues;
  edm::InputTag m_srcElectronIdTightValues;
  edm::InputTag m_srcElectronIdRobustLooseValues;
  edm::InputTag m_srcElectronIdRobustTightValues;
  
  double m_tkIsoCoeff;
  double m_emIsoCoeff;
  double m_hadIsoCoeff;

  bool m_doRefCheck;
  edm::InputTag m_srcElectronsRef;
  
  std::string m_fileName;
  bool m_verbosity;
  int m_eventType;
  int m_eventId;
  
  
  
  TFile* m_outFile;
  
  
  
  // electron histograms
  TH1F* recoENDistr;
  
  TH1F* recoEPtDistr;
  TH1F* recoEEtaDistr;
    
  TH1F* recoETkIsoDistr;
  TH1F* recoEEmIsoDistr;
  TH1F* recoEHadIsoDistr;
  TH1F* recoECombinedIsoDistr;
  TH1F* recoETkIsoOverPtDistr;
  TH1F* recoEEmIsoOverPtDistr;
  TH1F* recoEHadIsoOverPtDistr;
  TH1F* recoECombinedIsoOverPtDistr;
  
  TH1F* recoEEleIdLooseDistr;
  TH1F* recoEEleIdTightDistr;
  TH1F* recoEEleIdRobustLooseDistr;
  TH1F* recoEEleIdRobustTightDistr;
  
  
  
  TH1F* matchingRecoENDistr;
  
  TH1F* matchingRecoEPtDistr;
  TH1F* matchingRecoEEtaDistr;
    
  TH1F* matchingRecoETkIsoDistr;
  TH1F* matchingRecoEEmIsoDistr;
  TH1F* matchingRecoEHadIsoDistr;
  TH1F* matchingRecoECombinedIsoDistr;
  TH1F* matchingRecoETkIsoOverPtDistr;
  TH1F* matchingRecoEEmIsoOverPtDistr;
  TH1F* matchingRecoEHadIsoOverPtDistr;
  TH1F* matchingRecoECombinedIsoOverPtDistr;
  
  TH1F* matchingRecoEEleIdLooseDistr;
  TH1F* matchingRecoEEleIdTightDistr;
  TH1F* matchingRecoEEleIdRobustLooseDistr;
  TH1F* matchingRecoEEleIdRobustTightDistr;
  
};

#endif
