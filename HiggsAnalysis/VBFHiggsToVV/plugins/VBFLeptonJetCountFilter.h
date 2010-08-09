#ifndef VBFLeptonJetCountFilter_h
#define VBFLeptonJetCountFilter_h

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
class VBFLeptonJetCountFilter : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit VBFLeptonJetCountFilter (const edm::ParameterSet&);
  
  //! dtor 
  ~VBFLeptonJetCountFilter();
  
  
  
 private:
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcJets;
  
  double m_lepPtMin1;
  double m_lepPtMin2;
  double m_lepEtaMin;
  double m_lepEtaMax;
  int m_lepMinNumber;
  
  double m_jetEtMin1;
  double m_jetEtMin2;
  double m_jetEtaMin;
  double m_jetEtaMax;
  int m_jetMinNumber;
  
  TH1F* m_totalEvents;
  TH1F* m_passedEvents;
  TH1F* m_filterEfficiency;
  
};

#endif






//! ctor
template <class TCollection>
VBFLeptonJetCountFilter<TCollection>::VBFLeptonJetCountFilter(const edm::ParameterSet& iConfig): 
  m_srcElectrons(iConfig.getParameter<edm::InputTag>("srcElectrons")),
  m_srcMuons    (iConfig.getParameter<edm::InputTag>("srcMuons")),
  m_srcJets     (iConfig.getParameter<edm::InputTag>("srcJets")),
  m_lepEtMin1   (iConfig.getParameter<double>("lepEtMin1")),
  m_lepEtMin2   (iConfig.getParameter<double>("lepEtMin2")),
  m_lepEtaMin   (iConfig.getParameter<double>("lepEtaMin")), 
  m_lepEtaMax   (iConfig.getParameter<double>("lepEtaMax")),
  m_lepMinNumber(iConfig.getParameter<int>("lepMinNumber"))
  m_jetEtMin1   (iConfig.getParameter<double>("jetEtMin1")),
  m_jetEtMin2   (iConfig.getParameter<double>("jetEtMin2")),
  m_jetEtaMin   (iConfig.getParameter<double>("jetEtaMin")), 
  m_jetEtaMax   (iConfig.getParameter<double>("jetEtaMax")),
  m_jetMinNumber(iConfig.getParameter<int>("jetMinNumber"))
{
  edm::Service<TFileService> fs;
  
  m_totalEvents = fs -> make<TH1F>("totalEvents", "totalEvents", 1,  0., 1.);
  m_passedEvents = fs -> make<TH1F>("passedEvents", "passedEvents", 1,  0., 1.);
  m_filterEfficiency = fs -> make<TH1F>("filterEfficiency", "filterEfficiency", 1,  0., 1.);
}

// ----------------------------------------------------------------






//! dtor
template <class TCollection>
VBFLeptonJetCountFilter<TCollection>::~VBFLeptonJetCountFilter()
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count jets
template <class TCollection>
bool VBFLeptonJetCountFilter<TCollection>::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  edm::Handle<reco::GsfElectronCollection> electrons;
  iEvent.getByLabel(m_srcElectrons, electrons);
  
  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(m_srcMuons, muons);
  
  edm::Handle<TCollection> jets;
  iEvent.getByLabel(m_srcJets, jets);
  
  
  
  int nEle;
  int nElePlus;
  int nEleMinus;
  std::vector<float> ele_pt;
  
  int nMu;
  int nMuPlus;
  int nMuMinus;
  std::vector<float> mu_pt;
  
  
  
  // loop over electrons
  for(unsigned int eleIt = 0; eleIt < electrons -> size(); ++eleIt)
  {
    if( (electrons->at(eleIt).eta() < m_lepEtaMax) &&
        (electrons->at(eleIt).eta() > m_lepEtaMin) &&
        (electrons->at(eleIt).pt () > m_lepPtMin2) )
    {
      ++nEle;
      if(electrons->at(eleIt).charge() > 0) ++nElePlus;
      if(electrons->at(eleIt).charge() < 0) ++nEleMinus;
      ele_pt.push_back( electrons -> at(eleIt).pt() );
    }
  } // loop over electrons
  
  
  
  // loop over muons
  for(unsigned int muIt = 0; muIt < muons -> size(); ++muIt)
  {
    if( (muons -> at(muIt).eta() < m_etaMax) &&
        (muons -> at(muIt).eta() > m_etaMin) &&
        (muons -> at(muIt).pt() > m_ptMin) )
      ++nSelected ;
  } // loop over muons
  
  
  
  // loop over jets
  for(unsigned int iJet = 0; iJet < jets -> size(); ++iJet)
  {
    if( (jets -> at(iJet).eta() > m_jetEtaMin) &&
        (jets -> at(iJet).eta() < m_jetEtaMax) &&
        (jets -> at(iJet).et () > m_jetEtMin) )
      ++nSelected;
  } // loop over jets
  
  
  
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
