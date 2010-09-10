#ifndef VBFSameFlavourOppositeSignLeptonFilter_h
#define VBFSameFlavourOppositeSignLeptonFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"






class VBFSameFlavourOppositeSignLeptonFilter : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit VBFSameFlavourOppositeSignLeptonFilter (const edm::ParameterSet&);
  
  //! dtor 
  ~VBFSameFlavourOppositeSignLeptonFilter();
  
  
  
 private:
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcElectrons;
  edm::InputTag m_srcMuons;
  
  double m_mMin;
  
  TH1F* m_totalEvents;
  TH1F* m_passedEvents;
  TH1F* m_filterEfficiency;
  
};

#endif
