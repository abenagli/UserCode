#ifndef VBFMuonDistributions_h
#define VBFMuonDistributions_h

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






class VBFMuonDistributions : public edm::EDAnalyzer
{
 public:
  
  //! ctor
  VBFMuonDistributions(const edm::ParameterSet& iConfig);
  
  //! dtor
  ~VBFMuonDistributions();
  
  virtual void beginJob(const edm::EventSetup&);
  virtual void endJob();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcGenParticles;
  edm::InputTag m_srcMuons;
  
  double m_tkIsoCoeff;
  double m_emIsoCoeff;
  double m_hadIsoCoeff;

  bool m_doRefCheck;
  edm::InputTag m_srcMuonsRef;
  
  std::string m_fileName;
  bool m_verbosity;
  int m_eventType;
  int m_eventId;
  
  
  
  TFile* m_outFile;
  
  
  
  // muon histograms
  TH1F* recoMuNDistr;
  
  TH1F* recoMuPtDistr;
  TH1F* recoMuEtaDistr;
    
  TH1F* recoMuTkIsoDistr;
  TH1F* recoMuEmIsoDistr;
  TH1F* recoMuHadIsoDistr;
  TH1F* recoMuCombinedIsoDistr;
  TH1F* recoMuTkIsoOverPtDistr;
  TH1F* recoMuEmIsoOverPtDistr;
  TH1F* recoMuHadIsoOverPtDistr;
  TH1F* recoMuCombinedIsoOverPtDistr;
  
  
  
  TH1F* matchingRecoMuNDistr;
  
  TH1F* matchingRecoMuPtDistr;
  TH1F* matchingRecoMuEtaDistr;
    
  TH1F* matchingRecoMuTkIsoDistr;
  TH1F* matchingRecoMuEmIsoDistr;
  TH1F* matchingRecoMuHadIsoDistr;
  TH1F* matchingRecoMuCombinedIsoDistr;
  TH1F* matchingRecoMuTkIsoOverPtDistr;
  TH1F* matchingRecoMuEmIsoOverPtDistr;
  TH1F* matchingRecoMuHadIsoOverPtDistr;
  TH1F* matchingRecoMuCombinedIsoOverPtDistr;
  
};

#endif
