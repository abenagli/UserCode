#ifndef VBFElectronSpikeRemovalSelector_h
#define VBFElectronSpikeRemovalSelector_h

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





struct VBFElectronSpikeRemovalSelector
{
  typedef reco::GsfElectronCollection collection;
  typedef reco::GsfElectronRef electron;
  typedef std::vector<reco::GsfElectronRef> container; 
  typedef container::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFElectronSpikeRemovalSelector(const edm::ParameterSet& iConfig);
  
  //! dtor
  ~VBFElectronSpikeRemovalSelector();
  
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin (); }
  
  //! iterator to the end of the selected collection
  const_iterator end() const { return m_selected.end (); }
  
  //! the actual select method
  void select(edm::Handle<collection> electrons, const edm::Event& iEvent, const edm::EventSetup& iSetup);
  
  
    
 private:
  
  container m_selected;
  const CaloTopology* m_topology;
  edm::Handle<EcalRecHitCollection> m_EBRecHits;
  edm::Handle<EcalRecHitCollection> m_EERecHits;
  
  
  bool m_doRefCheck;
  edm::InputTag m_srcElectronsRef;
};

#endif
