#ifndef VBFFwdJetCountFilter_h
#define VBFFwdJetCountFilter_h

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
#include "TLorentzVector.h"






template <class TCollection>
class VBFFwdJetCountFilter : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit VBFFwdJetCountFilter (const edm::ParameterSet&);
  
  //! dtor 
  ~VBFFwdJetCountFilter();
  
  
  
 private:
  
  void beginJob(const edm::EventSetup&);
  void endJob();
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcJets;
  
  double m_etMin;
  double m_etaMin;
  double m_etaMax;
  double m_DetaMin;
  double m_mJJMin;
  int m_minNumber;
  
  TH1F* m_totalEvents;
  TH1F* m_passedEvents;
  TH1F* m_filterEfficiency;
  
};

#endif






//! ctor
template <class TCollection>
VBFFwdJetCountFilter<TCollection>::VBFFwdJetCountFilter(const edm::ParameterSet& iConfig): 
  m_srcJets  (iConfig.getParameter<edm::InputTag>("srcJets")),
  m_etMin    (iConfig.getParameter<double>("etMin")),
  m_etaMin   (iConfig.getParameter<double>("etaMin")), 
  m_etaMax   (iConfig.getParameter<double>("etaMax")),
  m_DetaMin  (iConfig.getParameter<double>("DetaMin")),
  m_mJJMin   (iConfig.getParameter<double>("mJJMin")),
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
VBFFwdJetCountFilter<TCollection>::~VBFFwdJetCountFilter()
{}

// ----------------------------------------------------------------






template <class TCollection>
void VBFFwdJetCountFilter<TCollection>::beginJob(const edm::EventSetup&) 
{}

// ----------------------------------------------------------------






template <class TCollection>
void VBFFwdJetCountFilter<TCollection>::endJob() 
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count jets
template <class TCollection>
bool VBFFwdJetCountFilter<TCollection>::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  edm::Handle<TCollection> jets;
  iEvent.getByLabel(m_srcJets, jets);
  
  
  
  int nSelected = 0;
  
  
  //PG loop over jets
  for(unsigned int iJet1 = 0; iJet1 < jets -> size(); ++iJet1)
  {
    if( (jets -> at(iJet1).eta() < m_etaMin) ||
        (jets -> at(iJet1).eta() > m_etaMax) ||
        (jets -> at(iJet1).et () < m_etMin) )
      continue;
    
    for(unsigned int iJet2 = iJet1+1; iJet2 < jets -> size(); ++iJet2)
    {
      if( (jets -> at(iJet2).eta() < m_etaMin) ||
          (jets -> at(iJet2).eta() > m_etaMax) ||
          (jets -> at(iJet2).et () < m_etMin) )
        continue;
      
      
      TLorentzVector jet1(jets -> at(iJet1).px(),
                          jets -> at(iJet1).py(),
                          jets -> at(iJet1).pz(),
                          jets -> at(iJet1).energy());
      
      TLorentzVector jet2(jets -> at(iJet2).px(),
                          jets -> at(iJet2).py(),
                          jets -> at(iJet2).pz(),
                          jets -> at(iJet2).energy());
      
      
      if( ( fabs(jet1.Eta() - jet2.Eta()) > m_DetaMin ) &&
          ( (jet1 + jet2).M() > m_mJJMin ) )
        ++nSelected;
    }
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
