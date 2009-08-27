#ifndef VBFElectronSelector_h 
#define VBFElectronSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"






class VBFElectronSelector
{
 public:
  
  typedef reco::GsfElectronCollection collection;
  typedef reco::GsfElectronRef electron;
  typedef std::vector<electron> container;
  typedef container::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFElectronSelector(const edm::ParameterSet&);
  
  //!dtor
  ~VBFElectronSelector();
  
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
  edm::InputTag m_srcElectronsRef;
  
};

#endif
