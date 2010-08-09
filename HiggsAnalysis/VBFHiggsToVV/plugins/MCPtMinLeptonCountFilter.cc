#include "HiggsAnalysis/VBFHiggsToVV/plugins/MCPtMinLeptonCountFilter.h"






//! ctor
MCPtMinLeptonCountFilter::MCPtMinLeptonCountFilter(const edm::ParameterSet& iConfig): 
  m_srcElectrons(iConfig.getParameter<edm::InputTag>("srcElectrons")),
  m_srcMuons    (iConfig.getParameter<edm::InputTag>("srcMuons")),
  m_ptMin       (iConfig.getParameter<double>       ("ptMin")),
  m_etaMin      (iConfig.getParameter<double>       ("etaMin")),
  m_etaMax      (iConfig.getParameter<double>       ("etaMax")),
  m_minNumber   (iConfig.getParameter<int>          ("minNumber")) 
{}

// ----------------------------------------------------------------






//! dtor
MCPtMinLeptonCountFilter::~MCPtMinLeptonCountFilter()
{}

// ----------------------------------------------------------------






void MCPtMinLeptonCountFilter::beginJob(const edm::EventSetup&) 
{}

// ----------------------------------------------------------------






void MCPtMinLeptonCountFilter::endJob() 
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count leptons
bool MCPtMinLeptonCountFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  edm::Handle<reco::GsfElectronCollection> electrons;
  iEvent.getByLabel(m_srcElectrons, electrons);
  
  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(m_srcMuons, muons);
  
  
  
  int nSelected = 0;
  
  
  //PG loop over electrons
  for(unsigned int eleIt = 0; eleIt < electrons -> size(); ++eleIt)
  {
    if( (electrons -> at(eleIt).eta() < m_etaMax) &&
        (electrons -> at(eleIt).eta() > m_etaMin) &&
        (electrons -> at(eleIt).pt () > m_ptMin) )
      ++nSelected;
  } //PG loop over electrons
  
  
  //PG loop over muons
  for(unsigned int muIt = 0; muIt < muons -> size(); ++muIt)
  {
    if( (muons -> at(muIt).eta() < m_etaMax) &&
        (muons -> at(muIt).eta() > m_etaMin) &&
        (muons -> at(muIt).pt() > m_ptMin) )
      ++nSelected ;
    } //PG loop over muons
  
  
  
  if(nSelected >= m_minNumber) return true;
  return false;
}
