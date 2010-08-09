#ifndef VBFLeptonTipLipProducer_h
#define VBFLeptonTipLipProducer_h

/* Class VBFLeptonTipLipProducer
 *
 * Original Author:  Alexis Pompili   - Bari
 *
 * Computes for each lepton the Transverse & Longitudinal IP w.r.t to PrimaryVertex (Tip & Lip)
 */

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"

#include "TrackingTools/GeomPropagators/interface/AnalyticalPropagator.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/GsfTools/interface/GsfPropagatorAdapter.h"
#include "TrackingTools/GsfTools/interface/MultiTrajectoryStateTransform.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"

#include <vector>
#include <string>






class VBFLeptonTipLipProducer : public edm::EDProducer
{
 typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::GsfElectron>, float> > eleMap;
 typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::Muon>, float> > muMap;

 public:
  
  //! ctor
  VBFLeptonTipLipProducer(const edm::ParameterSet&);
  
  //! dtor
  ~VBFLeptonTipLipProducer();
  
  
  
 private:
  
  //! the actual produce method
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
  
  edm::InputTag m_srcVertices;
  edm::InputTag m_srcElectrons;
  edm::InputTag m_srcMuons;
};

#endif
