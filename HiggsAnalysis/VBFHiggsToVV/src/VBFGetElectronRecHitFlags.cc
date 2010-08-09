#include "HiggsAnalysis/VBFHiggsToVV/interface/VBFGetElectronRecHitFlags.h"



VBFGetElectronRecHitFlags::VBFGetElectronRecHitFlags(const edm::Event& iEvent, const edm::EventSetup& iSetup, const reco::GsfElectronRef electronRef):
  m_topology(0),
  m_seed_flag(""),
  m_seed_severityLevel(""),
  m_all_flag(""),
  m_all_severityLevel("")
{
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
  
  
  
  // RecHits                                                                                                                                                             
  iEvent.getByLabel( edm::InputTag("ecalRecHit:EcalRecHitsEB"), m_EBRecHits );
  iEvent.getByLabel( edm::InputTag("ecalRecHit:EcalRecHitsEE"), m_EERecHits );
  
  
  
  // get rechit flags
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
    
    char name1[3];
    sprintf(name1, "%d", flag);
    m_seed_flag += name1;
    
    
    
    // kWeird
    int sev = EcalSeverityLevelAlgo::severityLevel(id.first, *m_EBRecHits, *(theChannelStatus.product()), 5., EcalSeverityLevelAlgo::kSwissCross, 0.95);

    char name2[3];
    sprintf(name2, "%d", sev);
    m_seed_severityLevel += name2;
    
    
    
    // Loop on all rechits of all clusters
    for(reco::CaloCluster_iterator bc = electronRef->superCluster()->clustersBegin(); 
	bc != electronRef->superCluster()->clustersEnd(); ++bc)
    {
      
      //if( seedCluster == (*bc) ) continue;
      std::vector<std::pair<DetId, float> > ids = (*bc)->hitsAndFractions();
      
      
      // loop on rechits
      for(std::vector<std::pair<DetId, float> >::const_iterator idIt = ids.begin(); idIt != ids.end(); ++idIt)
      {
        EcalRecHitCollection::const_iterator it = (m_EBRecHits.product())->find(idIt->first);
        
        int tempFlag = 0;
        if( it != m_EBRecHits.product()->end() )
        {
          const EcalRecHit& rh = (*it);
          tempFlag = rh.recoFlag();
        }
        
        char name11[3];
        sprintf(name11, "%d,", tempFlag);
        m_all_flag += name11;
        
        
        
        // kWeird
        sev = EcalSeverityLevelAlgo::severityLevel(id.first, *m_EBRecHits, *(theChannelStatus.product()), 5., EcalSeverityLevelAlgo::kSwissCross, 0.95);
        
        char name22[3];
        sprintf(name22, "%d,", sev);
        m_all_severityLevel += name22;
        
      } // loop on rechits
      
      m_all_flag += " ";
      m_all_severityLevel += " ";
      
    } // Loop on clusters 
    
  } // if isBarrel
 
}

// ----------------------------------------------------------------






VBFGetElectronRecHitFlags::~VBFGetElectronRecHitFlags()
{}

// ----------------------------------------------------------------
