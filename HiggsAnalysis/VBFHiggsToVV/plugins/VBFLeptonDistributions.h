#ifndef VBFLeptonDistributions_h
#define VBFLeptonDistributions_h

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






class VBFLeptonDistributions : public edm::EDAnalyzer
{
 public:
  
  //! ctor
  VBFLeptonDistributions(const edm::ParameterSet& iConfig);
  
  //! dtor
  ~VBFLeptonDistributions();
  
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
  edm::InputTag m_srcMuons;
  
  double m_tkIsoCoeff;
  double m_emIsoCoeff;
  double m_hadIsoCoeff;

  bool m_doElectronRefCheck;
  edm::InputTag m_srcElectronsRef;
  bool m_doMuonRefCheck;
  edm::InputTag m_srcMuonsRef;
  
  std::string m_fileName;
  bool m_verbosity;
  int m_eventType;
  int m_eventId;
  
  
  
  TFile* m_outFile;
  
  
  
  // electron histograms
  TH1F* recoLNDistr;
  
  TH1F* recoLPtDistr;
  TH1F* recoLEtaDistr;
    
  TH1F* recoLTkIsoDistr;
  TH1F* recoLEmIsoDistr;
  TH1F* recoLHadIsoDistr;
  TH1F* recoLCombinedIsoDistr;
  TH1F* recoLTkIsoOverPtDistr;
  TH1F* recoLEmIsoOverPtDistr;
  TH1F* recoLHadIsoOverPtDistr;
  TH1F* recoLCombinedIsoOverPtDistr;
  
  TH1F* recoEEleIdLooseDistr;
  TH1F* recoEEleIdTightDistr;
  TH1F* recoEEleIdRobustLooseDistr;
  TH1F* recoEEleIdRobustTightDistr;
  
  
  
  TH1F* matchingRecoLNDistr;
  
  TH1F* matchingRecoLPtDistr;
  TH1F* matchingRecoLEtaDistr;
    
  TH1F* matchingRecoLTkIsoDistr;
  TH1F* matchingRecoLEmIsoDistr;
  TH1F* matchingRecoLHadIsoDistr;
  TH1F* matchingRecoLCombinedIsoDistr;
  TH1F* matchingRecoLTkIsoOverPtDistr;
  TH1F* matchingRecoLEmIsoOverPtDistr;
  TH1F* matchingRecoLHadIsoOverPtDistr;
  TH1F* matchingRecoLCombinedIsoOverPtDistr;
  
  TH1F* matchingRecoEEleIdLooseDistr;
  TH1F* matchingRecoEEleIdTightDistr;
  TH1F* matchingRecoEEleIdRobustLooseDistr;
  TH1F* matchingRecoEEleIdRobustTightDistr;
  
};

#endif
