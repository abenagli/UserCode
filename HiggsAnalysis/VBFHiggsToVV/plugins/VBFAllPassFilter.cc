#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFAllPassFilter.h"






//! ctor
VBFAllPassFilter::VBFAllPassFilter(const edm::ParameterSet& iConfig)
{
  edm::Service<TFileService> fs;
  
  m_totalEvents = fs -> make<TH1F>("totalEvents", "totalEvents", 1,  0., 1.);
  m_passedEvents = fs -> make<TH1F>("passedEvents", "passedEvents", 1,  0., 1.);
  m_filterEfficiency = fs -> make<TH1F>("filterEfficiency", "filterEfficiency", 1,  0., 1.);
}

// ----------------------------------------------------------------






//! dtor
VBFAllPassFilter::~VBFAllPassFilter()
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count leptons
bool VBFAllPassFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  int nTotalEvents = static_cast<int>(m_totalEvents -> GetBinContent(1));
  int nPassedEvents = static_cast<int>(m_passedEvents -> GetBinContent(1));
  
  m_totalEvents -> Fill(0.5);
  m_passedEvents -> Fill(0.5);
  m_filterEfficiency -> SetBinContent(1, 1.*(nPassedEvents+1)/(nTotalEvents+1));
  
  return true;
}
