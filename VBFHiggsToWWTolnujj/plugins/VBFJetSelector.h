#ifndef VBFJetSelector_h 
#define VBFJetSelector_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"





template <class TCollection>
class VBFJetSelector
{
 public:
  
  typedef TCollection collection;
  typedef edm::Ref<collection> jet;
  typedef edm::RefVector<collection> container;
  typedef typename edm::RefVector<collection>::const_iterator const_iterator;
  
  
  
 public:
  
  //! ctor
  explicit VBFJetSelector(const edm::ParameterSet&);
  
  //!dtor
  ~VBFJetSelector();
  
  //! iterator to the begin of the selected collection
  const_iterator begin() const { return m_selected.begin(); }
  
  //! iterator to the end of the selected collection
  const_iterator end() const { return m_selected.end(); }
  
  //! the actual selector method 
  void select(edm::Handle<collection>,const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  container m_selected;
  
  double m_etMin;
  double m_etaMin;
  double m_etaMax;
  
  bool m_doRefCheck;
  edm::InputTag m_srcJetsRef;

};






template <class TCollection>
VBFJetSelector<TCollection>::VBFJetSelector(const edm::ParameterSet& iConfig):
  m_etMin (iConfig.getParameter<double>("etMin")),
  m_etaMin(iConfig.getParameter<double>("etaMin")),
  m_etaMax(iConfig.getParameter<double>("etaMax")),
  m_doRefCheck (iConfig.getParameter<bool>("doRefCheck")),
  m_srcJetsRef(iConfig.getParameter<edm::InputTag>("srcJetsRef"))
{}

// ----------------------------------------------------------------






template <class TCollection>
VBFJetSelector<TCollection>::~VBFJetSelector()
{}

// ----------------------------------------------------------------






template <class TCollection>
void VBFJetSelector<TCollection>::select(edm::Handle<collection> jets,
                             const edm::Event& iEvent,
                             const edm::EventSetup& iEventSetup)
{
  m_selected.clear();
  
  
  edm::Handle< edm::RefVector<collection> > jetsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcJetsRef, jetsRef);  
  
  
  for(unsigned int i = 0; i < jets -> size(); ++i)
  {
    // do the reference check
    bool isRefCheckOk = true;
    jet jetRef(jets, i);
    if(m_doRefCheck)
      if(find(jetsRef -> begin(), jetsRef -> end(), jetRef) == jetsRef -> end())
	isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    
    
    if( ((jets -> at(i)).et() < m_etMin) ||
        ((jets -> at(i)).eta() < m_etaMin) ||
        ((jets -> at(i)).eta() > m_etaMax) )
      continue;
    
    
    m_selected.push_back(jetRef);
  }
    
}

#endif
