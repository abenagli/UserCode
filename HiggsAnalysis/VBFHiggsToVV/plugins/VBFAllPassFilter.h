#ifndef VBFAllPassFilter_h
#define VBFAllPassFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"






class VBFAllPassFilter : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit VBFAllPassFilter (const edm::ParameterSet&);
  
  //! dtor 
  ~VBFAllPassFilter();
  
  
  
 private:
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  TH1F* m_totalEvents;
  TH1F* m_passedEvents;
  TH1F* m_filterEfficiency;
  
};

#endif
