#ifndef VBFLepton3DipProducer_h
#define VBFLepton3DipProducer_h

/* Class VBFLepton3DipProducer
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
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"
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

#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

#include <vector>
#include <string>






class VBFLepton3DipProducer : public edm::EDProducer
{
 typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::GsfElectron>, float> > eleMap;
 typedef edm::AssociationMap<edm::OneToValue<std::vector<reco::Muon>, float> > muMap;

 public:
  
  //! ctor
  VBFLepton3DipProducer(const edm::ParameterSet&);
  
  //! dtor
  ~VBFLepton3DipProducer();
  
  
  
 private:
  
  //! the actual produce method
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
  
  edm::InputTag m_srcVertices;
  edm::InputTag m_srcElectrons;
  edm::InputTag m_srcMuons;
};

#endif
