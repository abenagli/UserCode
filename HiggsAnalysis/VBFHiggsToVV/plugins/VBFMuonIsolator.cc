#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonIsolator.h"






VBFMuonIsolator::VBFMuonIsolator(const edm::ParameterSet& iConfig):
  m_tkIsoCut      (iConfig.getParameter<double>("tkIsoCut")), 
  m_emIsoCut      (iConfig.getParameter<double>("emIsoCut")), 
  m_hadIsoCut     (iConfig.getParameter<double>("hadIsoCut")), 
  m_useCombinedIso(iConfig.getParameter<bool>("useCombinedIso")), 
  m_combinedIsoCut(iConfig.getParameter<double>("combinedIsoCut")), 
  m_tkIsoCoeff    (iConfig.getParameter<double>("tkIsoCoeff")), 
  m_emIsoCoeff    (iConfig.getParameter<double>("tkIsoCoeff")), 
  m_hadIsoCoeff   (iConfig.getParameter<double>("tkIsoCoeff")), 
  m_dividePt      (iConfig.getParameter<bool>("dividePt")), 
  m_doRefCheck    (iConfig.getParameter<bool>("doRefCheck")),
  m_srcMuonsRef   (iConfig.getParameter<edm::InputTag>("srcMuonsRef"))
{}

// ----------------------------------------------------------------






VBFMuonIsolator::~VBFMuonIsolator()
{}

// ----------------------------------------------------------------






void VBFMuonIsolator::select(edm::Handle<collection> muons,
                             const edm::Event& iEvent,
                             const edm::EventSetup& iEventSetup)
{
  m_selected.clear();
  
  
  
  edm::Handle< edm::RefVector<collection> > muonsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcMuonsRef, muonsRef);
  
  
  
  // loop over muons
  for(unsigned i = 0; i < muons -> size(); ++i)
  { 
    // do the reference check 
    bool isRefCheckOk = true;
    muon muonRef(muons, i);
    if(m_doRefCheck)
      if(find(muonsRef -> begin(), muonsRef -> end(), muonRef) == muonsRef -> end())
        isRefCheckOk = false;
    
    if(!isRefCheckOk) continue;
    
    
    
    
    
    
    bool isTkIsoOk  = false;
    bool isEmIsoOk  = false;
    bool isHadIsoOk = false;
    bool isCombinedIsoOk = false;
    
    
    
    // get the isolation deposits
    double tkIso =  ((muons -> at(i)).isolationR03()).sumPt;
    double emIso =  ((muons -> at(i)).isolationR03()).emEt;
    double hadIso = ((muons -> at(i)).isolationR03()).hadEt;
    double combinedIso = 0.;    

    if(m_dividePt)
    {
      tkIso  /= (muons -> at(i)).pt();
      emIso  /= (muons -> at(i)).pt();
      hadIso /= (muons -> at(i)).pt();
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
      m_selected.push_back(muonRef);
    
    
  } // loop over muons

}
