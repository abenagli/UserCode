#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFElectronSelector.h"






VBFElectronSelector::VBFElectronSelector(const edm::ParameterSet& iConfig):
  m_ptMin (iConfig.getParameter<double>("ptMin")),
  m_etaMin(iConfig.getParameter<double>("etaMin")),
  m_etaMax(iConfig.getParameter<double>("etaMax"))
{}

// ----------------------------------------------------------------






VBFElectronSelector::~VBFElectronSelector()
{}

// ----------------------------------------------------------------






void VBFElectronSelector::select(edm::Handle<collection> electrons,
                                 const edm::Event& iEvent,
                                 const edm::EventSetup& iEventSetup)
{
  m_selected.clear();
  
  
  
  for(unsigned int i = 0; i < electrons -> size(); ++i)
  {
    if( ((electrons -> at(i)).pt() < m_ptMin) ||
        ((electrons -> at(i)).eta() < m_etaMin) ||
        ((electrons -> at(i)).eta() > m_etaMax) )
      continue;
    
   electron electronRef(electrons, i);
   m_selected.push_back(electronRef);
  }
    
}

// ----------------------------------------------------------------
