#ifndef VBFJetCleaner_h
#define VBFJetCleaner_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/Math/interface/deltaR.h"

#include <iostream>
#include <Math/VectorUtil.h>






template <class TCollection>
class VBFJetCleaner
{
 public:

  typedef reco::GsfElectronCollection electronCollection;
  typedef reco::GsfElectronRef electron;

  typedef reco::MuonCollection muonCollection;
  typedef reco::MuonRef muon;
  
  typedef TCollection collection;
  typedef edm::Ref<collection> jet;
  typedef edm::RefVector<collection> container;
  typedef typename edm::RefVector<collection>::const_iterator const_iterator;  
  
  
 public:
  
  //! ctor
  VBFJetCleaner(const edm::ParameterSet& conf);
  
  //! dtor
  ~VBFJetCleaner();
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin(); }
  
  //! iterator to the end of the selected collection
  const_iterator end () const { return m_selected.end(); }
  
  //! the actual selector method 
  void select (edm::Handle<collection>, const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  container m_selected;
  
  edm::InputTag m_srcElectronsRef;
  edm::InputTag m_srcMuonsRef;
  
  double m_maxDeltaR;
  
  bool m_doJetRefCheck;
  edm::InputTag m_srcJetsRef;
};
  





template <class TCollection>
VBFJetCleaner<TCollection>::VBFJetCleaner(const edm::ParameterSet& iConfig):
  m_srcElectronsRef(iConfig.getParameter<edm::InputTag>("srcElectronsRef")),
  m_srcMuonsRef    (iConfig.getParameter<edm::InputTag>("srcMuonsRef")),
  m_maxDeltaR      (iConfig.getParameter<double>("maxDeltaR")),
  m_doJetRefCheck  (iConfig.getParameter<bool>("doJetRefCheck")),
  m_srcJetsRef     (iConfig.getParameter<edm::InputTag>("srcJetsRef"))
{}  

// ----------------------------------------------------------------------------






template <class TCollection>
VBFJetCleaner<TCollection>::~VBFJetCleaner()
{}

// ----------------------------------------------------------------------------





  
template <class TCollection> 
void VBFJetCleaner<TCollection>::select(edm::Handle<VBFJetCleaner<TCollection>::collection> jets, 
                                        const edm::Event& iEvent,
                                        const edm::EventSetup& iSetup)
{
  m_selected.clear ();
  
  
  
  //Get the collections
  edm::Handle< edm::RefVector<electronCollection> > electronsRef;
  iEvent.getByLabel(m_srcElectronsRef, electronsRef);

  edm::Handle< edm::RefVector<muonCollection> > muonsRef;
  iEvent.getByLabel(m_srcMuonsRef, muonsRef);
  
  edm::Handle< edm::RefVector<collection> > jetsRef;
  if(m_doJetRefCheck)
    iEvent.getByLabel(m_srcJetsRef, jetsRef);  
  
  
  
  
  
  
  // loop over jets
  for(unsigned int j = 0; j < jets -> size(); ++j)
  {
    // do the reference check 
    bool isJetRefCheckOk = true;
    jet jetRef(jets, j);
    if(m_doJetRefCheck)
      if(find(jetsRef -> begin(), jetsRef -> end(), jetRef) == jetsRef -> end())
        isJetRefCheckOk = false;
    
    if(!isJetRefCheckOk) continue;
    
    
    
    int counter = 0;
    
    // loop over electrons    
    for(unsigned int i = 0; i < electronsRef -> size(); ++i)
    {
      electron electronRef = electronsRef->at(i);
      double dR = deltaR(electronRef->eta(), electronRef->phi(),
                         jetRef->eta(), jetRef->phi());
      
      if(dR < m_maxDeltaR)
        ++counter;
    } // loop over electrons
    
    // loop over muons    
    for(unsigned int i = 0; i < muonsRef -> size(); ++i)
    {
      muon muonRef = muonsRef->at(i);
      double dR = deltaR(muonRef->eta(), muonRef->phi(),
                         jetRef->eta(), jetRef->phi());
      
      if(dR < m_maxDeltaR)
        ++counter;
    } // loop over muons
    
    
    
    if(counter)
      continue;
    
    
    m_selected.push_back(jetRef);
 } // loop over jets   

}

// ----------------------------------------------------------------------------

#endif
