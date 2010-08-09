#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronSpikeRemovalSelector.h"



VBFElectronSpikeRemovalSelector::VBFElectronSpikeRemovalSelector(const edm::ParameterSet& iConfig):
  m_topology       (0),
  m_doRefCheck     (iConfig.getParameter<bool>("doRefCheck")),
  m_srcElectronsRef(iConfig.getParameter<edm::InputTag>("srcElectronsRef"))
{}

// ----------------------------------------------------------------






VBFElectronSpikeRemovalSelector::~VBFElectronSpikeRemovalSelector()
{}

// ----------------------------------------------------------------






void VBFElectronSpikeRemovalSelector::select(edm::Handle<collection> electrons,
                                             const edm::Event& iEvent,
                                             const edm::EventSetup& iSetup)
{
  m_selected.clear();
  
  
  
  edm::Handle< edm::RefVector<collection> > electronsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcElectronsRef, electronsRef);
  
  
  
  //calo topology                                                                                                                                                                
  unsigned long long cacheIDTopo = 0;
  edm::ESHandle<CaloTopology> theCaloTopology;
  if(cacheIDTopo != iSetup.get<CaloTopologyRecord>().cacheIdentifier())
    {
      cacheIDTopo = iSetup.get<CaloTopologyRecord>().cacheIdentifier();
      iSetup.get<CaloTopologyRecord>().get(theCaloTopology);
    }
  m_topology = theCaloTopology.product();
    
    
  edm::ESHandle<EcalChannelStatus> theChannelStatus;
  iSetup.get<EcalChannelStatusRcd>().get(theChannelStatus);
  
  
  
  // reduced rechits                                                                                                                                                             
  iEvent.getByLabel( edm::InputTag("ecalRecHit:EcalRecHitsEB"), m_EBRecHits );
  iEvent.getByLabel( edm::InputTag("ecalRecHit:EcalRecHitsEE"), m_EERecHits );


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
    
    
    
    if( electronRef -> isEB() )
    {
      // kOutOfTime
      const edm::Ptr<reco::CaloCluster>& seedCluster = (electronRef->superCluster())->seed();
      std::pair<DetId, float> id = EcalClusterTools::getMaximum(seedCluster->hitsAndFractions(), (m_EBRecHits.product()));
      
      int flag = 0;
      EcalRecHitCollection::const_iterator it = (m_EBRecHits.product())->find(id.first);
      
      if( it != m_EBRecHits.product()->end() )
      {
        const EcalRecHit& rh = (*it);
        flag = rh.recoFlag();
      }
      
      if( flag == EcalRecHit::kOutOfTime ) continue;
      
      
      
      // kWeird
      int sev = EcalSeverityLevelAlgo::severityLevel(id.first, *m_EBRecHits, *(theChannelStatus.product()), 5., EcalSeverityLevelAlgo::kSwissCross, 0.95);
      if( sev > 2 ) continue;
    }
    
    
    m_selected.push_back(electronRef);
    ++i;
  }
}

// ----------------------------------------------------------------
