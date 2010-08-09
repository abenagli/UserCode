// -*- C++ -*-
//
// Package:    GenFilters
// Class:      MCFwdJetsPreselFilter
// 
/**\class MCFwdJetsPreselFilter MCFwdJetsPreselFilter.cc GeneratorInterface/GenFilters/src/MCFwdJetsPreselFilter.cc

 Description: mild VBF selections: 
 the event is required con contain a minimum number (m_nJets) of gen jets (m_jetTag)
 with a minimum pT (m_ptThres); out of any possible combination of pairs of gen jets that can be built with those, 
 , at least one is required to have at least a given  delta eta (m_DetaThres)
 and invariant mass (m_MjjThres)

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  P. Govoni
//         Created:  Mon Apr 20 15:02:55 CEST 2009
// $Id: MCFwdJetsPreselFilter.cc,v 1.1 2009/09/23 10:21:49 abenagli Exp $
//
//


// system include files
//#include <vector>
//#include <boost/format.hpp>

// user include files
#include "HiggsAnalysis/VBFHiggsToVV/plugins/MCFwdJetsPreselFilter.h"

#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/Math/interface/LorentzVector.h"


MCFwdJetsPreselFilter::MCFwdJetsPreselFilter (const edm::ParameterSet& iConfig) : 
  m_jetTag (iConfig.getParameter<edm::InputTag> ("jetTag")) ,
  m_ptThres (iConfig.getParameter<double> ("ptThres")) ,
  m_nJets (iConfig.getParameter<int> ("nJets")) ,
  m_DetaThres (iConfig.getParameter<double> ("DetaThres")) ,
  m_MjjThres (iConfig.getParameter<double> ("MjjThres"))
{
  if (m_nJets < 2)
    {
      //PG issue a warning message here
      m_nJets = 2 ;
    }
}


// ----------------------------------------------------------------


MCFwdJetsPreselFilter::~MCFwdJetsPreselFilter () {}


// ----------------------------------------------------------------


void MCFwdJetsPreselFilter::beginJob (const edm::EventSetup&) 
{
}


// ----------------------------------------------------------------


void MCFwdJetsPreselFilter::endJob () 
{
//PG  cout << boost::format ("Pass: %9d %9d %9d") % pass22 % pass23 %  (pass22+pass23)
//PG       << endl ;
}


// ----------------------------------------------------------------


bool 
MCFwdJetsPreselFilter::filter (
    edm::Event& iEvent,
    const edm::EventSetup& iSetup) 
{
  edm::Handle<reco::CaloJetCollection> jetsHandle ;
  iEvent.getByLabel (m_jetTag, jetsHandle) ;

  std::vector<int> goodJets ;
  for (unsigned int iGj = 0 ; iGj < jetsHandle->size () ; ++iGj)
    {
      if (jetsHandle->at (iGj).pt () > m_ptThres) goodJets.push_back (iGj) ;    
    }
  if (goodJets.size () < static_cast<unsigned int>(m_nJets)) return false ;

  //PG loop over jet pairs
  for (unsigned int iGj = 0 ; iGj < goodJets.size () - 1 ; ++iGj)
    for (unsigned int jGj = iGj + 1 ; jGj < goodJets.size () ; ++jGj)
      {
        double dEta = fabs (jetsHandle->at (iGj).eta () - 
                            jetsHandle->at (jGj).eta ()) ;
        if (dEta > m_DetaThres)
          {
            math::XYZTLorentzVector sum = jetsHandle->at (iGj).p4 () +
                                          jetsHandle->at (jGj).p4 () ;
            if (sum.M () > m_MjjThres) return true ; 
          }
      } //PG loop over jet pairs
      
  return false ;
}
