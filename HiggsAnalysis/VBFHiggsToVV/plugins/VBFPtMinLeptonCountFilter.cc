#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFPtMinLeptonCountFilter.h"






//! ctor
VBFPtMinLeptonCountFilter::VBFPtMinLeptonCountFilter(const edm::ParameterSet& iConfig): 
  m_srcElectrons(iConfig.getParameter<edm::InputTag>("srcElectrons")),
  m_srcMuons    (iConfig.getParameter<edm::InputTag>("srcMuons")),
  m_ptMin       (iConfig.getParameter<double>       ("ptMin")),
  m_etaMin      (iConfig.getParameter<double>       ("etaMin")),
  m_etaMax      (iConfig.getParameter<double>       ("etaMax")),
  m_minNumber   (iConfig.getParameter<int>          ("minNumber")) 
{
  edm::Service<TFileService> fs;
  
  m_totalEvents = fs -> make<TH1F>("totalEvents", "totalEvents", 1,  0., 1.);
  m_passedEvents = fs -> make<TH1F>("passedEvents", "passedEvents", 1,  0., 1.);
  m_filterEfficiency = fs -> make<TH1F>("filterEfficiency", "filterEfficiency", 1,  0., 1.);
}

// ----------------------------------------------------------------






//! dtor
VBFPtMinLeptonCountFilter::~VBFPtMinLeptonCountFilter()
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count leptons
bool VBFPtMinLeptonCountFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
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
  
  
  
  int nTotalEvents = static_cast<int>(m_totalEvents -> GetBinContent(1));
  int nPassedEvents = static_cast<int>(m_passedEvents -> GetBinContent(1));
  
  if(nSelected >= m_minNumber)
  {
    m_totalEvents -> Fill(0.5);
    m_passedEvents -> Fill(0.5);
    m_filterEfficiency -> SetBinContent(1, 1.*(nPassedEvents+1)/(nTotalEvents+1));
    
    return true;
  }
  
  else
  {
    m_totalEvents -> Fill(0.5);
    m_filterEfficiency -> SetBinContent(1, 1.*(nPassedEvents)/(nTotalEvents+1)); 
    
    return false;
  }
  
}
