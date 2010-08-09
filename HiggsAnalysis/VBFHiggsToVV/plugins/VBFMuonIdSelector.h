#ifndef VBFMuonIdSelector_h 
#define VBFMuonIdSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

#include "RecoVertex/PrimaryVertexProducer/interface/PrimaryVertexSorter.h"






class VBFMuonIdSelector
{
 public:
  
  typedef reco::MuonCollection collection;
  typedef reco::MuonRef muon;
  typedef std::vector<muon> container;
  typedef container::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFMuonIdSelector(const edm::ParameterSet&);
  
  //!dtor
  ~VBFMuonIdSelector();
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin(); }
  
  //! iterator to the end of the selected collection
  const_iterator end() const { return m_selected.end(); }
  
  //! the actual selector method 
  void select(edm::Handle<collection>,const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  container m_selected;
  
  edm::InputTag m_srcVertices;
  double m_normalizedChi2Max;
  int m_numberOfValidTrackerHitsMin;
  int m_numberOfValidMuonHitsMin;
  double m_d0Max;
  
  bool m_doRefCheck;
  edm::InputTag m_srcMuonsRef;

};

#endif
