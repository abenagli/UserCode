#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronConversionRemovalSelector.h"



VBFElectronConversionRemovalSelector::VBFElectronConversionRemovalSelector(const edm::ParameterSet& iConfig):
  m_doRefCheck     (iConfig.getParameter<bool>("doRefCheck")),
  m_srcElectronsRef(iConfig.getParameter<edm::InputTag>("srcElectronsRef"))
{}

// ----------------------------------------------------------------






VBFElectronConversionRemovalSelector::~VBFElectronConversionRemovalSelector()
{}

// ----------------------------------------------------------------






void VBFElectronConversionRemovalSelector::select(edm::Handle<collection> electrons,
                                             const edm::Event& iEvent,
                                             const edm::EventSetup& iSetup)
{
  m_selected.clear();
  
  
  
  edm::Handle< edm::RefVector<collection> > electronsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcElectronsRef, electronsRef);
  
  
  
  // Loop over electrons                                                                                                                                                         
  unsigned int i = 0 ;
  for(reco::GsfElectronCollection::const_iterator eleIt = electrons->begin(); eleIt != electrons->end(); ++eleIt)
  {
    // do the reference check                                                                                                                                                      
    bool isRefCheckOk = true;
    electron electronRef(electrons, i);
    if(m_doRefCheck)
      if(find(electronsRef -> begin(), electronsRef -> end(), electronRef) == electronsRef -> end())
        isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    
    
    
    if(IsNotConversion(electronRef) == true)
      m_selected.push_back(electronRef);
    ++i;
  }
}

// ----------------------------------------------------------------






bool VBFElectronConversionRemovalSelector::IsNotConversion(edm::Ref<reco::GsfElectronCollection> eleRef)
{
  bool isAmbiguous = true;
  if( eleRef->ambiguousGsfTracksSize() == 0 )
    isAmbiguous = false;
  
  
  bool isNotFromPixel = true;
  int mishits = eleRef->gsfTrack()->trackerExpectedHitsInner().numberOfHits(); 
  if( mishits == 0 )
    isNotFromPixel = false;
  
  
  if( isAmbiguous || isNotFromPixel )
    return false;
  
  return true;
}

// ----------------------------------------------------------------
