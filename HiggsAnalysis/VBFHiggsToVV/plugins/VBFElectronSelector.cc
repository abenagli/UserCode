#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronSelector.h"






VBFElectronSelector::VBFElectronSelector(const edm::ParameterSet& iConfig):
  m_ptMin (iConfig.getParameter<double>("ptMin")),
  m_ptMax (iConfig.getParameter<double>("ptMax")),
  m_etaMin(iConfig.getParameter<double>("etaMin")),
  m_etaMax(iConfig.getParameter<double>("etaMax")),
  m_doRefCheck     (iConfig.getParameter<bool>("doRefCheck")),
  m_srcElectronsRef(iConfig.getParameter<edm::InputTag>("srcElectronsRef"))
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
  
  
  edm::Handle< edm::RefVector<collection> > electronsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcElectronsRef, electronsRef);  
  
  
  for(unsigned int i = 0; i < electrons -> size(); ++i)
  {
    // do the reference check
    bool isRefCheckOk = true;
    electron electronRef(electrons, i);
    if(m_doRefCheck)
      if(find(electronsRef -> begin(), electronsRef -> end(), electronRef) == electronsRef -> end())
        isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    
    
    
    if( ((electrons -> at(i)).pt() < m_ptMin) ||
        ((electrons -> at(i)).pt() > m_ptMax) ||
        ((electrons -> at(i)).eta() < m_etaMin) ||
        ((electrons -> at(i)).eta() > m_etaMax) )
      continue;
   
   
   m_selected.push_back(electronRef);
  }
    
}

// ----------------------------------------------------------------
