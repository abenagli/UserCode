#ifndef VBFElectronIsolator_h 
#define VBFElectronIsolator_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"





class VBFElectronIsolator
{
 public:
  
  typedef reco::GsfElectronCollection collection;
  typedef reco::GsfElectronRef electron;
  typedef std::vector<electron> container;
  typedef container::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFElectronIsolator(const edm::ParameterSet&);
  
  //! dtor
  ~VBFElectronIsolator();
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin(); }

  //! iterator to the end of the selected collection
  const_iterator end() const { return m_selected.end(); }
  
  //! the actual selector method
  void select(edm::Handle<collection>, const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  container m_selected;        
  
  double m_tkIsoCut;
  double m_emIsoCut;
  double m_hadIsoCut;
  
  bool m_useCombinedIso;
  double m_combinedIsoCut;
  double m_tkIsoCoeff;
  double m_emIsoCoeff;
  double m_hadIsoCoeff;
  
  bool m_dividePt;
  
  bool m_doRefCheck;
  edm::InputTag m_srcElectronsRef;

};


#endif
