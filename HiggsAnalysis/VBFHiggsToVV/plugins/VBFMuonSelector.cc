#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonSelector.h"






VBFMuonSelector::VBFMuonSelector(const edm::ParameterSet& iConfig):
  m_ptMin (iConfig.getParameter<double>("ptMin")),
  m_ptMax (iConfig.getParameter<double>("ptMax")),
  m_etaMin(iConfig.getParameter<double>("etaMin")),
  m_etaMax(iConfig.getParameter<double>("etaMax")),
  m_doRefCheck (iConfig.getParameter<bool>("doRefCheck")),
  m_srcMuonsRef(iConfig.getParameter<edm::InputTag>("srcMuonsRef"))
{}

// ----------------------------------------------------------------






VBFMuonSelector::~VBFMuonSelector()
{}

// ----------------------------------------------------------------






void VBFMuonSelector::select(edm::Handle<collection> muons,
                             const edm::Event& iEvent,
                             const edm::EventSetup& iEventSetup)
{
  m_selected.clear();
  
  
  edm::Handle< edm::RefVector<collection> > muonsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcMuonsRef, muonsRef);  
  
  
  for(unsigned int i = 0; i < muons -> size(); ++i)
  {
    // do the reference check
    bool isRefCheckOk = true;
    muon muonRef(muons, i);
    if(m_doRefCheck)
      if(find(muonsRef -> begin(), muonsRef -> end(), muonRef) == muonsRef -> end())
	isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    
    
    if( ((muons -> at(i)).pt() < m_ptMin) ||
        ((muons -> at(i)).pt() > m_ptMax) ||
        ((muons -> at(i)).eta() < m_etaMin) ||
        ((muons -> at(i)).eta() > m_etaMax) )
      continue;
    
    
    m_selected.push_back(muonRef);
  }
    
}
