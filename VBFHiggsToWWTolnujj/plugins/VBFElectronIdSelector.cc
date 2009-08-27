#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFElectronIdSelector.h"






VBFElectronIdSelector::VBFElectronIdSelector(const edm::ParameterSet& iConfig):
  m_srcElectronIdValues(iConfig.getParameter<edm::InputTag>("srcElectronIdValues")),
  m_doRefCheck         (iConfig.getParameter<bool>("doRefCheck")),
  m_srcElectronsRef    (iConfig.getParameter<edm::InputTag>("srcElectronsRef"))
{}

// ----------------------------------------------------------------






VBFElectronIdSelector::~VBFElectronIdSelector()
{}

// ----------------------------------------------------------------






void VBFElectronIdSelector::select(edm::Handle<collection> electrons,
                             const edm::Event& iEvent,
                             const edm::EventSetup& iEventSetup)
{
  m_selected.clear();
  
  
  
  // Get the isolation maps
  edm::Handle<edm::ValueMap<float> > electronIdValues;
  iEvent.getByLabel(m_srcElectronIdValues, electronIdValues);
  
  
  
  edm::Handle< edm::RefVector<collection> > electronsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcElectronsRef, electronsRef);
  
  
  
  // loop over electrons
  for(unsigned i = 0; i < electrons -> size(); ++i)
  { 
    // do the reference check 
    bool isRefCheckOk = true;
    electron electronRef(electrons, i);
    if(m_doRefCheck)
      if(find(electronsRef -> begin(), electronsRef -> end(), electronRef) == electronsRef -> end())
        isRefCheckOk = false;
    
    if(!isRefCheckOk) continue;
    
    
    
    
    
    
    bool isEleIdOk  = false;
    
    
    // get the isolation deposits
    double eleId  = (*electronIdValues)[electronRef];
    
    
    // do the actual cut
    if(eleId > 0)  isEleIdOk  = true; 
    
        
    if( (isEleIdOk == true ) )
      m_selected.push_back(electronRef);
    
    
  } // loop over electrons

}
