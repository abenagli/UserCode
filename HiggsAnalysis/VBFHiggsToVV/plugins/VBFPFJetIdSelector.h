#ifndef VBFPFJetIdSelector_h 
#define VBFPFJetIdSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"






class VBFPFJetIdSelector
{
 public:
  
  typedef reco::PFJetCollection collection;
  typedef edm::Ref<collection> jet;
  typedef edm::RefVector<collection> container;
  typedef edm::RefVector<collection>::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFPFJetIdSelector(const edm::ParameterSet&);
  
  //!dtor
  ~VBFPFJetIdSelector();
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin(); }
  
  //! iterator to the end of the selected collection
  const_iterator end() const { return m_selected.end(); }
  
  //! the actual selector method 
  void select(edm::Handle<collection>,const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  container m_selected;

  edm::InputTag m_srcJetID;
  
  double m_CHFMin;
  double m_NHFMax;
  double m_CEFMax;
  double m_NEFMax;
  
  bool m_doRefCheck;
  edm::InputTag m_srcJetsRef;

};

#endif
