#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronIdSelector.h"






VBFElectronIdSelector::VBFElectronIdSelector(const edm::ParameterSet& iConfig):
  m_fbremMIN                         (iConfig.getParameter<double>("fbremMIN")),
  m_hcalOverEcalMAX                  (iConfig.getParameter<double>("hcalOverEcalMAX")),
  m_deltaPhiSuperClusterTrackAtVtxMAX(iConfig.getParameter<double>("deltaPhiSuperClusterTrackAtVtxMAX")),
  m_deltaEtaSuperClusterTrackAtVtxMAX(iConfig.getParameter<double>("deltaEtaSuperClusterTrackAtVtxMAX")),
  m_sigmaIetaIetaMAX                 (iConfig.getParameter<double>("sigmaIetaIetaMAX")),
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
    
    
    
    
    
    
    bool isEleIdOk = true;
    
    // loose eleId cuts
    if( electronRef->fbrem() < m_fbremMIN ) isEleIdOk = false;
    if( electronRef->hcalOverEcal() > m_hcalOverEcalMAX ) isEleIdOk = false;
    if( fabs(electronRef->deltaPhiSuperClusterTrackAtVtx()) > m_deltaPhiSuperClusterTrackAtVtxMAX ) isEleIdOk = false;
    if( fabs(electronRef->deltaEtaSuperClusterTrackAtVtx()) > m_deltaEtaSuperClusterTrackAtVtxMAX ) isEleIdOk = false;
    if( electronRef->sigmaIetaIeta() > m_sigmaIetaIetaMAX ) isEleIdOk = false; 
    
    
    
    if( (isEleIdOk == true ) )
      m_selected.push_back(electronRef);
    
  } // loop over electrons

}
