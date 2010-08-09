#ifndef VBFElectronStdPreselectionSelector_h 
#define VBFElectronStdPreselectionSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"





class VBFElectronStdPreselectionSelector
{
 public: 
  
  typedef reco::GsfElectronCollection collection;
  typedef reco::GsfElectronRef electron;
  typedef std::vector<electron> container;
  typedef container::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFElectronStdPreselectionSelector(const edm::ParameterSet&);
  
  //! dtor
  ~VBFElectronStdPreselectionSelector();
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin(); }

  //! iterator to the end of the selected collection
  const_iterator end() const { return m_selected.end(); }
  
  //! the actual selector method
  void select(edm::Handle<collection>, const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  container m_selected;        
  
  bool StdPreselection(edm::Ref<reco::GsfElectronCollection> eleRef);
  
  bool m_doRefCheck;
  edm::InputTag m_srcElectronsRef;

};

#endif
