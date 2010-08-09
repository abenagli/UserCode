#ifndef VBFCaloJetIdSelector_h 
#define VBFCaloJetIdSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/JetID.h"

#include "Math/GenVector/VectorUtil.h"






class VBFCaloJetIdSelector
{
 public:
  
  typedef reco::CaloJetCollection collection;
  typedef edm::Ref<collection> jet;
  typedef edm::RefVector<collection> container;
  typedef edm::RefVector<collection>::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFCaloJetIdSelector(const edm::ParameterSet&);
  
  //!dtor
  ~VBFCaloJetIdSelector();
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin(); }
  
  //! iterator to the end of the selected collection
  const_iterator end() const { return m_selected.end(); }
  
  //! the actual selector method 
  void select(edm::Handle<collection>,const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  container m_selected;

  edm::InputTag m_srcJets_forID;
  edm::InputTag m_srcJetID;
  
  double m_EMFMin;
  int m_n90Min;
  double m_fHPDMax;
  
  bool m_doRefCheck;
  edm::InputTag m_srcJetsRef;

};

#endif
