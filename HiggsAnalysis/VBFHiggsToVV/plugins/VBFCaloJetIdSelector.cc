#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFCaloJetIdSelector.h"






VBFCaloJetIdSelector::VBFCaloJetIdSelector(const edm::ParameterSet& iConfig):
  m_srcJets_forID(iConfig.getParameter<edm::InputTag>("srcJets_forID")),
  m_srcJetID     (iConfig.getParameter<edm::InputTag>("srcJetID")),
  m_EMFMin       (iConfig.getParameter<double>("EMFMin")),
  m_n90Min       (iConfig.getParameter<int>("n90Min")),
  m_fHPDMax      (iConfig.getParameter<double>("fHPDMax")),
  m_doRefCheck(iConfig.getParameter<bool>("doRefCheck")),
  m_srcJetsRef(iConfig.getParameter<edm::InputTag>("srcJetsRef"))
{}

// ----------------------------------------------------------------






VBFCaloJetIdSelector::~VBFCaloJetIdSelector()
{}

// ----------------------------------------------------------------






void VBFCaloJetIdSelector::select(edm::Handle<collection> jets,
                                  const edm::Event& iEvent,
                                  const edm::EventSetup& iEventSetup)
{
  m_selected.clear();
  
  
  
  edm::Handle< edm::RefVector<collection> > jetsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcJetsRef, jetsRef);  
  
  edm::Handle<reco::CaloJetCollection> JetHandle_forID;
  iEvent.getByLabel (m_srcJets_forID, JetHandle_forID);
  
  edm::Handle<reco::JetIDValueMap> jetIDHandle;
  iEvent.getByLabel(m_srcJetID, jetIDHandle);  
  
  
  
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
    
    
    
    float DRMin = 0.01;
    int jMin = -1;
    
    for(unsigned int j=0; j<JetHandle_forID->size(); ++j)
    {
      reco::CaloJetRef jetRef_forID(JetHandle_forID, j);
      
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(jetRef->p4(), jetRef_forID->p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    
    
    if(jMin == -1) isJetIdOk = false;
    
    reco::CaloJetRef jetRef_match(JetHandle_forID, jMin);
    
    if( (fabs(jetRef->eta()) < 2.6) && (jetRef->emEnergyFraction() < m_EMFMin) ) isJetIdOk = false;
    if( ((*jetIDHandle)[jetRef_match]).n90Hits < m_n90Min ) isJetIdOk = false;
    if( ((*jetIDHandle)[jetRef_match]).fHPD > m_fHPDMax ) isJetIdOk = false;
    
    
    
    if( (isJetIdOk == true ) )
      m_selected.push_back(jetRef);
  }
    
}
