#ifndef MCPtMinLeptonCountFilter_h
#define MCPtMinLeptonCountFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"




class MCPtMinLeptonCountFilter : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit MCPtMinLeptonCountFilter (const edm::ParameterSet&);
  
  //! dtor 
  ~MCPtMinLeptonCountFilter();
  
  
  
 private:
  
  void beginJob(const edm::EventSetup&);
  void endJob();
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcElectrons;
  edm::InputTag m_srcMuons;
  
  double m_ptMin;
  double m_etaMin;
  double m_etaMax;
  int m_minNumber;
};

#endif
