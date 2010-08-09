#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronIsolator.h"






VBFElectronIsolator::VBFElectronIsolator(const edm::ParameterSet& iConfig):
  m_tkIsoCut       (iConfig.getParameter<double>("tkIsoCut")), 
  m_emIsoCut       (iConfig.getParameter<double>("emIsoCut")), 
  m_hadIsoCut      (iConfig.getParameter<double>("hadIsoCut")), 
  m_useCombinedIso (iConfig.getParameter<bool>("useCombinedIso")), 
  m_combinedIsoCut (iConfig.getParameter<double>("combinedIsoCut")), 
  m_tkIsoCoeff     (iConfig.getParameter<double>("tkIsoCoeff")), 
  m_emIsoCoeff     (iConfig.getParameter<double>("tkIsoCoeff")), 
  m_hadIsoCoeff    (iConfig.getParameter<double>("tkIsoCoeff")), 
  m_dividePt       (iConfig.getParameter<bool>("dividePt")), 
  m_doRefCheck     (iConfig.getParameter<bool>("doRefCheck")),
  m_srcElectronsRef(iConfig.getParameter<edm::InputTag>("srcElectronsRef"))
{}

// ----------------------------------------------------------------






VBFElectronIsolator::~VBFElectronIsolator()
{}

// ----------------------------------------------------------------






void VBFElectronIsolator::select(edm::Handle<collection> electrons,
                             const edm::Event& iEvent,
                             const edm::EventSetup& iEventSetup)
{
  m_selected.clear();
  
  
  
  // Get the electron refs
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
    
    
    
    
    
    
    bool isTkIsoOk  = false;
    bool isEmIsoOk  = false;
    bool isHadIsoOk = false;
    bool isCombinedIsoOk = false;
    
    
    
    // get the isolation deposits
    double tkIso  = (electrons -> at(i)).dr03TkSumPt();
    double emIso  = (electrons -> at(i)).dr03EcalRecHitSumEt();
    double hadIso = (electrons -> at(i)).dr03HcalDepth1TowerSumEt();
    double combinedIso = 0.;    

    if(m_dividePt)
    {
      tkIso  /= (electrons -> at(i)).pt();
      emIso  /= (electrons -> at(i)).pt();
      hadIso /= (electrons -> at(i)).pt();
    }
    
    if(m_useCombinedIso)
    {
      combinedIso = (m_tkIsoCoeff  * tkIso) +
                    (m_emIsoCoeff  * emIso) +
                    (m_hadIsoCoeff * hadIso);
    }
    
    
    
    // do the actual cut
    if(tkIso  < m_tkIsoCut)  isTkIsoOk  = true; 
    if(emIso  < m_emIsoCut)  isEmIsoOk  = true;
    if(hadIso < m_hadIsoCut) isHadIsoOk = true; 
    
    if( (m_useCombinedIso) && (combinedIso < m_combinedIsoCut) ) isCombinedIsoOk = true;    
    
    
    
    if( (m_useCombinedIso == false && isTkIsoOk == true && isEmIsoOk == true && isHadIsoOk == true) ||
        (m_useCombinedIso == true  && isCombinedIsoOk == true ) )
      m_selected.push_back(electronRef);
    
    
  } // loop over electrons

}
