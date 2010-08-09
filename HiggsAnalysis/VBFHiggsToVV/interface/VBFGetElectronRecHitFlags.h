#ifndef VBFGetElectronRecHitFlags_h
#define VBFGetElectronRecHitFlags_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/Records/interface/CaloTopologyRecord.h"

#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloClusterFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronSeedFwd.h"
#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"





struct VBFGetElectronRecHitFlags
{
  typedef reco::GsfElectronCollection collection;
  typedef reco::GsfElectronRef electron;
  typedef std::vector<reco::GsfElectronRef> container; 
  typedef container::const_iterator const_iterator;
  
  
  
 public:
 
 //! ctor
 explicit VBFGetElectronRecHitFlags(const edm::Event& iEvent, const edm::EventSetup& iSetup, const reco::GsfElectronRef electronRef);
  
  //! dtor
  ~VBFGetElectronRecHitFlags();
  
  //! methods
  std::string GetSeedFlag() { return m_seed_flag; } ;
  std::string GetSeedSeverityLevel() { return m_seed_severityLevel; } ;
  std::string GetOtherFlag() { return m_other_flag; } ;
  std::string GetOtherSeverityLevel() { return m_other_severityLevel; } ;
  
  
 private:
  
  const CaloTopology* m_topology;
  edm::Handle<EcalRecHitCollection> m_EBRecHits;
  edm::Handle<EcalRecHitCollection> m_EERecHits;
  
  std::string m_seed_flag;
  std::string m_seed_severityLevel;
  std::string m_other_flag;
  std::string m_other_severityLevel;  
  
};

#endif
