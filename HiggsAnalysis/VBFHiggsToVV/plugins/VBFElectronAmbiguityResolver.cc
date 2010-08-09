#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronAmbiguityResolver.h"






VBFElectronAmbiguityResolver::VBFElectronAmbiguityResolver(const edm::ParameterSet& iConfig):
  m_doRefCheck     (iConfig.getParameter<bool>("doRefCheck")),
  m_srcElectronsRef(iConfig.getParameter<edm::InputTag>("srcElectronsRef"))
{}

// ------------------------------------------------------------






 VBFElectronAmbiguityResolver::~VBFElectronAmbiguityResolver()
{}

// ------------------------------------------------------------






void VBFElectronAmbiguityResolver::select(edm::Handle<collection> electrons,
                                          const edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  m_selected.clear();
  
  //PG get the actual product
  dump(iEvent, &m_selected, electrons);
  
  container::iterator ShorterEnd;
  
  sort(m_selected.begin(), m_selected.end(), TKeEoPSorting()); 
  ShorterEnd = unique(m_selected.begin(), m_selected.end(), equalTk()); 
  m_selected.erase(ShorterEnd, m_selected.end());
  
  sort(m_selected.begin(), m_selected.end(), SCEeEoPSorting()); 
  ShorterEnd = unique(m_selected.begin(), m_selected.end(), equalSC()); 
  m_selected.erase(ShorterEnd, m_selected.end());
  
  return;
}       

// ------------------------------------------------------------






void VBFElectronAmbiguityResolver::dump(const edm::Event& iEvent,
                                        container* output, const edm::Handle<collection>& electrons) 
{
  edm::Handle< edm::RefVector<collection> > electronsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcElectronsRef, electronsRef);
  
  
  for(unsigned int ii = 0; ii != electrons -> size(); ++ii)
  {
    // do the reference check
    bool isRefCheckOk = true;
    electron electronRef(electrons, ii);
    if(m_doRefCheck)
      if(find(electronsRef -> begin(), electronsRef -> end(), electronRef) == electronsRef -> end())
        isRefCheckOk = false;
    
    if(!isRefCheckOk) continue;
    
    
    output -> push_back(electronRef);
  }   
 
 return;
}
