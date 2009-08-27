#ifndef VBFJetCleaner_h
#define VBFJetCleaner_h

#include "DataFormats/Common/interface/Handle.h" 
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include <iostream>
#include <algorithm>
#include <Math/VectorUtil.h>






template <class TCollection>
class VBFJetCleaner
{
  typedef edm::View<reco::PixelMatchGsfElectron> electronCollection;
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
  
  edm::InputTag m_srcElectrons;
  double m_maxDeltaR;
  double m_minEleOverJetEratio;
  double m_maxHEoverEmE;
};
  





template <class TCollection>
VBFJetCleaner<TCollection>::VBFJetCleaner(const edm::ParameterSet& iConfig) :
  m_srcElectrons        (iConfig.getParameter<edm::InputTag>("srcElectrons")),
  m_maxDeltaR           (iConfig.getParameter<double>       ("maxDeltaR")),
  m_minEleOverJetEratio (iConfig.getParameter<double>       ("minEleOJetEratio")),
  m_maxHEoverEmE        (iConfig.getParameter<double>       ("maxHEoverEmE"))
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
  
  
  //PG get the GSF electrons collection
  edm::Handle<electronCollection> GSFHandle; 
  iEvent.getByLabel(m_srcElectrons, GSFHandle); 
  
  
  //PG loop over jets
  for(typename collection::const_iterator jetIt = jets -> begin();
      jetIt != jets -> end(); ++jetIt)
  {
    double electronsEnergy = 0.;
    
    
    //PG loop over electrons    
    int counter = 0;
    for(electronCollection::const_iterator eleIt = GSFHandle -> begin();
       eleIt != GSFHandle ->end(); ++eleIt)
    {
      //PG this is not an electron
      if(fabs(eleIt -> hadronicOverEm()) > m_maxHEoverEmE)
        continue;          
      
      
      double deltaR = deltaR(*eleIt, *jetIt);
      
      if (deltaR < m_maxDeltaR)
      {
        ++counter;
        electronsEnergy += eleIt -> caloEnergy();
      }          
    } //PG loop over electrons
    
    
    if ( (counter) && ((electronsEnergy / jetIt ->energy()) > m_minEleOverJetEratio) )
      continue;
    
    
    m_selected.push_back(jet(jets, jetIt - jets -> begin()));
 } //PG loop over jets   

}

#endif
 


