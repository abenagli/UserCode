#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFSameFlavourOppositeSignLeptonFilter.h"






//! ctor
VBFSameFlavourOppositeSignLeptonFilter::VBFSameFlavourOppositeSignLeptonFilter(const edm::ParameterSet& iConfig): 
  m_srcElectrons(iConfig.getParameter<edm::InputTag>("srcElectrons")),
  m_srcMuons    (iConfig.getParameter<edm::InputTag>("srcMuons")),
  m_mMin        (iConfig.getParameter<double>       ("mMin"))
{
  edm::Service<TFileService> fs;
  
  m_totalEvents = fs -> make<TH1F>("totalEvents", "totalEvents", 1,  0., 1.);
  m_passedEvents = fs -> make<TH1F>("passedEvents", "passedEvents", 1,  0., 1.);
  m_filterEfficiency = fs -> make<TH1F>("filterEfficiency", "filterEfficiency", 1,  0., 1.);
}

// ----------------------------------------------------------------






//! dtor
VBFSameFlavourOppositeSignLeptonFilter::~VBFSameFlavourOppositeSignLeptonFilter()
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count leptons
bool VBFSameFlavourOppositeSignLeptonFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  edm::Handle<reco::GsfElectronCollection> electrons;
  iEvent.getByLabel(m_srcElectrons, electrons);
  
  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(m_srcMuons, muons);
  
  
  
  int nSelected = 0;
  
  
  // loop over electrons
  for(unsigned int eleIt = 0; eleIt < electrons -> size(); ++eleIt)
    for(unsigned int eleIt2 = eleIt+1; eleIt2 < electrons -> size(); ++eleIt2)
    {
      if( ( (electrons -> at(eleIt).charge() * electrons -> at(eleIt2).charge()) < 0. ) && 
          ( (electrons -> at(eleIt).p4() + electrons -> at(eleIt2).p4()).mass() > m_mMin ) )
        ++nSelected;
    } // loop over electrons
  
  
  
  // loop over muons
  for(unsigned int muIt = 0; muIt < muons -> size(); ++muIt)
    for(unsigned int muIt2 = muIt+1; muIt2 < muons -> size(); ++muIt2)
    {
      if( ( (muons -> at(muIt).charge() * muons -> at(muIt2).charge()) < 0. ) && 
          ( (muons -> at(muIt).p4() + muons -> at(muIt2).p4()).mass() > m_mMin ) )
        ++nSelected;
    } // loop over muons
  
  
  
  int nTotalEvents = static_cast<int>(m_totalEvents -> GetBinContent(1));
  int nPassedEvents = static_cast<int>(m_passedEvents -> GetBinContent(1));
  
  if(nSelected >= 1)
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
