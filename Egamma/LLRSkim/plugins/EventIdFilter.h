#ifndef EventIdFilter_h
#define EventIdFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"






class EventIdFilter : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit EventIdFilter (const edm::ParameterSet&);
  
  //! dtor 
  ~EventIdFilter();
  
  
  
 private:
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  int m_runId;
  int m_eventId;
  
};

#endif
