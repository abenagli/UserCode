#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFPtMinLeptonCountFilter.h"






//! ctor
VBFPtMinLeptonCountFilter::VBFPtMinLeptonCountFilter(const edm::ParameterSet& iConfig): 
  m_srcElectrons(iConfig.getParameter<edm::InputTag>("srcElectrons")),
  m_srcMuons    (iConfig.getParameter<edm::InputTag>("srcMuons")),
  m_ptMin       (iConfig.getParameter<double>       ("ptMin")),
  m_etaMin      (iConfig.getParameter<double>       ("etaMin")),
  m_etaMax      (iConfig.getParameter<double>       ("etaMax")),
  m_minNumber   (iConfig.getParameter<int>          ("minNumber")) 
{}

// ----------------------------------------------------------------






//! dtor
VBFPtMinLeptonCountFilter::~VBFPtMinLeptonCountFilter()
{}

// ----------------------------------------------------------------






void VBFPtMinLeptonCountFilter::beginJob(const edm::EventSetup&) 
{}

// ----------------------------------------------------------------






void VBFPtMinLeptonCountFilter::endJob() 
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count leptons
bool VBFPtMinLeptonCountFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  edm::Handle<reco::GsfElectronCollection> electronsHandle;
  iEvent.getByLabel(m_srcElectrons, electronsHandle);
  
  edm::Handle<reco::MuonCollection> muonsHandle;
  iEvent.getByLabel(m_srcMuons, muonsHandle);
  
  
  
  int nSelected = 0;
  
  
  //PG loop over electrons
  for(unsigned int iEle = 0; iEle < electronsHandle -> size(); ++iEle)
  {
    if( (electronsHandle -> at(iEle).eta() < m_etaMax) &&
        (electronsHandle -> at(iEle).eta() > m_etaMin) &&
        (electronsHandle -> at(iEle).pt () > m_ptMin) )
      ++nSelected;
  } //PG loop over electrons
  
  
  //PG loop over muons
  for(unsigned int iMu = 0; iMu < muonsHandle -> size(); ++iMu)
  {
    if( (muonsHandle -> at(iMu).eta() < m_etaMax) &&
        (muonsHandle -> at(iMu).eta() > m_etaMin) &&
        (muonsHandle -> at(iMu).pt() > m_ptMin) )
      ++nSelected ;
    } //PG loop over muons
  
  
  
  if(nSelected >= m_minNumber) return true;
  return false;
}
