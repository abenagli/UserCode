#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFPFTauSelector.h"






VBFPFTauSelector::VBFPFTauSelector(const edm::ParameterSet& iConfig):
  m_ptMin (iConfig.getParameter<double>("ptMin")),
  m_ptMax (iConfig.getParameter<double>("ptMax")),
  m_etaMin(iConfig.getParameter<double>("etaMin")),
  m_etaMax(iConfig.getParameter<double>("etaMax")),
  m_doRefCheck  (iConfig.getParameter<bool>("doRefCheck")),
  m_srcPFTausRef(iConfig.getParameter<edm::InputTag>("srcPFTausRef"))
{}

// ----------------------------------------------------------------






VBFPFTauSelector::~VBFPFTauSelector()
{}

// ----------------------------------------------------------------






void VBFPFTauSelector::select(edm::Handle<collection> taus,
                              const edm::Event& iEvent,
                              const edm::EventSetup& iEventSetup)
{
  m_selected.clear();
  
  
  edm::Handle< edm::RefVector<collection> > tausRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcPFTausRef, tausRef);  
  
  
  for(unsigned int i = 0; i < taus -> size(); ++i)
  {
    // do the reference check
    bool isRefCheckOk = true;
    tau tauRef(taus, i);
    if(m_doRefCheck)
      if(find(tausRef -> begin(), tausRef -> end(), tauRef) == tausRef -> end())
	isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    
    
    if( ((taus -> at(i)).pt() < m_ptMin) ||
        ((taus -> at(i)).pt() > m_ptMax) ||
        ((taus -> at(i)).eta() < m_etaMin) ||
        ((taus -> at(i)).eta() > m_etaMax) )
      continue;
    
    
    m_selected.push_back(tauRef);
  }
    
}
