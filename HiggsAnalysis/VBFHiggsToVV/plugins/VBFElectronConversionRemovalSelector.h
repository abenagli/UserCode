#ifndef VBFElectronConversionRemovalSelector_h
#define VBFElectronConversionRemovalSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"







struct VBFElectronConversionRemovalSelector
{
  typedef reco::GsfElectronCollection collection;
  typedef reco::GsfElectronRef electron;
  typedef std::vector<reco::GsfElectronRef> container; 
  typedef container::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFElectronConversionRemovalSelector(const edm::ParameterSet& iConfig);
  
  //! dtor
  ~VBFElectronConversionRemovalSelector();
  
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin (); }
  
  //! iterator to the end of the selected collection
  const_iterator end() const { return m_selected.end (); }
  
  //! the actual select method
  void select(edm::Handle<collection> electrons, const edm::Event& iEvent, const edm::EventSetup& iSetup);
  
  
  
 private:
  
  container m_selected;
  bool IsNotConversion(edm::Ref<reco::GsfElectronCollection> eleRef);
  
  bool m_doRefCheck;
  edm::InputTag m_srcElectronsRef;
};

#endif
