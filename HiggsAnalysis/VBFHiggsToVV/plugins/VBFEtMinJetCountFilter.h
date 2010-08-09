#ifndef VBFEtMinJetCountFilter_h
#define VBFEtMinJetCountFilter_h

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"






template <class TCollection>
class VBFEtMinJetCountFilter : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit VBFEtMinJetCountFilter (const edm::ParameterSet&);
  
  //! dtor 
  ~VBFEtMinJetCountFilter();
  
  
  
 private:
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcJets;
  
  double m_etMin;
  double m_etaMin;
  double m_etaMax;
  int m_minNumber;
  
  TH1F* m_totalEvents;
  TH1F* m_passedEvents;
  TH1F* m_filterEfficiency;
  
};

#endif






//! ctor
template <class TCollection>
VBFEtMinJetCountFilter<TCollection>::VBFEtMinJetCountFilter(const edm::ParameterSet& iConfig): 
  m_srcJets  (iConfig.getParameter<edm::InputTag>("srcJets")),
  m_etMin    (iConfig.getParameter<double>("etMin")),
  m_etaMin   (iConfig.getParameter<double>("etaMin")), 
  m_etaMax   (iConfig.getParameter<double>("etaMax")),
  m_minNumber(iConfig.getParameter<int>("minNumber"))
{
  edm::Service<TFileService> fs;
  
  m_totalEvents = fs -> make<TH1F>("totalEvents", "totalEvents", 1,  0., 1.);
  m_passedEvents = fs -> make<TH1F>("passedEvents", "passedEvents", 1,  0., 1.);
  m_filterEfficiency = fs -> make<TH1F>("filterEfficiency", "filterEfficiency", 1,  0., 1.);
}

// ----------------------------------------------------------------






//! dtor
template <class TCollection>
VBFEtMinJetCountFilter<TCollection>::~VBFEtMinJetCountFilter()
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count jets
template <class TCollection>
bool VBFEtMinJetCountFilter<TCollection>::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  edm::Handle<TCollection> jets;
  iEvent.getByLabel(m_srcJets, jets);
  
  
  
  int nSelected = 0;
  
  
  //PG loop over jets
  for(unsigned int iJet = 0; iJet < jets -> size(); ++iJet)
  {
    if( (jets -> at(iJet).eta() > m_etaMin) &&
        (jets -> at(iJet).eta() < m_etaMax) &&
        (jets -> at(iJet).et () > m_etMin) )
      ++nSelected;
  } //PG loop over jets
  
  
  
  int nTotalEvents = static_cast<int>(m_totalEvents -> GetBinContent(1));
  int nPassedEvents = static_cast<int>(m_passedEvents -> GetBinContent(1));
  
  if(nSelected >= m_minNumber)
  {
    m_totalEvents -> Fill(0.5);
    m_passedEvents -> Fill(0.5);
    m_filterEfficiency -> SetBinContent(1, 1.*(nPassedEvents+1)/(nTotalEvents+1));
    
    return true;
  }
  
  else
  {
    m_totalEvents -> Fill(0.5);
    m_filterEfficiency -> SetBinContent(1, 1.*(nPassedEvents)/(nTotalEvents+1)); 
    
    return false;
  }
  
}
