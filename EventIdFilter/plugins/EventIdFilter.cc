#include "CommonTools/EventIdFilter/plugins/EventIdFilter.h"






//! ctor
EventIdFilter::EventIdFilter(const edm::ParameterSet& iConfig): 
  m_runId(iConfig.getParameter<int>("runId")),
  m_eventId(iConfig.getParameter<int>("eventId"))
{}

// ----------------------------------------------------------------






//! dtor
EventIdFilter::~EventIdFilter()
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count leptons
bool EventIdFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  if( (int)(iEvent.id().run()) != m_runId ) return false;
  if( (int)(iEvent.id().event()) != m_eventId ) return false;
  
  return true;
}
