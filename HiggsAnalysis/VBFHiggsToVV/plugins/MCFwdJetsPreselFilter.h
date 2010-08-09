#ifndef MCFwdJetsPreselFilter_h
#define MCFwdJetsPreselFilter_h

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/MakerMacros.h"

//
// class declaration
//

class MCFwdJetsPreselFilter : public edm::EDFilter {

  public:
    explicit MCFwdJetsPreselFilter (const edm::ParameterSet&) ;
    ~MCFwdJetsPreselFilter () ;

  private:
    virtual void beginJob (const edm::EventSetup&)  ;
    virtual bool filter (edm::Event&, const edm::EventSetup&) ;
    virtual void endJob ()  ;

  private:

    edm::InputTag m_jetTag ;
    double m_ptThres ;
    int m_nJets ;
    double m_DetaThres ;
    double m_MjjThres ;

} ;


#endif
