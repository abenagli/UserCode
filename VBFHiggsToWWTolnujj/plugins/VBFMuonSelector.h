#ifndef VBFMuonSelector_h 
#define VBFMuonSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"






class VBFMuonSelector
{
 public:
  
  typedef reco::MuonCollection collection;
  typedef reco::MuonRef muon;
  typedef std::vector<muon> container;
  typedef container::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFMuonSelector(const edm::ParameterSet&);
  
  //!dtor
  ~VBFMuonSelector();
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin(); }
  
  //! iterator to the end of the selected collection
  const_iterator end() const { return m_selected.end(); }
  
  //! the actual selector method 
  void select(edm::Handle<collection>,const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  container m_selected;
  
  double m_ptMin;
  double m_etaMin;
  double m_etaMax;
  
  bool m_doRefCheck;
  edm::InputTag m_srcMuonsRef;

};

#endif
