#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFPFJetIdSelector.h"






VBFPFJetIdSelector::VBFPFJetIdSelector(const edm::ParameterSet& iConfig):
  m_CHFMin  (iConfig.getParameter<double>("CHFMin")),
  m_NHFMax  (iConfig.getParameter<double>("NHFMax")),
  m_CEFMax  (iConfig.getParameter<double>("CEFMax")),
  m_NEFMax  (iConfig.getParameter<double>("NEFMax")),
  m_doRefCheck(iConfig.getParameter<bool>("doRefCheck")),
  m_srcJetsRef(iConfig.getParameter<edm::InputTag>("srcJetsRef"))
{}

// ----------------------------------------------------------------






VBFPFJetIdSelector::~VBFPFJetIdSelector()
{}

// ----------------------------------------------------------------






void VBFPFJetIdSelector::select(edm::Handle<collection> jets,
                                const edm::Event& iEvent,
                                const edm::EventSetup& iEventSetup)
{
  m_selected.clear();
  
  
  
  edm::Handle< edm::RefVector<collection> > jetsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcJetsRef, jetsRef);  
  
  
  
  for(unsigned int i = 0; i < jets -> size(); ++i)
  {
    // do the reference check
    bool isRefCheckOk = true;
    jet jetRef(jets, i);
    if(m_doRefCheck)
      if(find(jetsRef -> begin(), jetsRef -> end(), jetRef) == jetsRef -> end())
	isRefCheckOk = false;
    
    if(!isRefCheckOk) continue;
    
    
    
    bool isJetIdOk = true;
    
    
    
    if( (fabs(jetRef->eta()) < 2.4) && (jetRef->chargedHadronEnergyFraction() < m_CHFMin) ) continue;
    if( jetRef->neutralHadronEnergyFraction() > m_NHFMax ) continue;
    if( jetRef->chargedEmEnergyFraction() > m_CEFMax ) continue;
    if( jetRef->neutralEmEnergyFraction() > m_NEFMax ) continue;
    
    
    
    if( (isJetIdOk == true ) )
      m_selected.push_back(jetRef);
  }
    
}
