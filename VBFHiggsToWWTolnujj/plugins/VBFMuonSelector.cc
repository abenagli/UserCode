#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFMuonSelector.h"






VBFMuonSelector::VBFMuonSelector(const edm::ParameterSet& iConfig):
  m_ptMin (iConfig.getParameter<double>("ptMin")),
  m_etaMin(iConfig.getParameter<double>("etaMin")),
  m_etaMax(iConfig.getParameter<double>("etaMax"))
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
  
  
  
  for(unsigned int i = 0; i < muons -> size(); ++i)
  {
    if( ((muons -> at(i)).pt() < m_ptMin) ||
        ((muons -> at(i)).eta() < m_etaMin) ||
        ((muons -> at(i)).eta() > m_etaMax) )
      continue;
    
   muon muonRef(muons, i);
   m_selected.push_back(muonRef);
  }
    
}
