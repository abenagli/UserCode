#include "Egamma/LLRSkim/plugins/SkimAnalyzer.h"






//! ctor
SkimAnalyzer::SkimAnalyzer(const edm::ParameterSet& iConfig):
  electronCollection_               (iConfig.getParameter<edm::InputTag>("electronCollection")),
  fbremMIN_                         (iConfig.getParameter<double>("fbremMIN")),
  hcalOverEcalMAX_                  (iConfig.getParameter<double>("hcalOverEcalMAX")),
  deltaPhiSuperClusterTrackAtVtxMAX_(iConfig.getParameter<double>("deltaPhiSuperClusterTrackAtVtxMAX")),
  deltaEtaSuperClusterTrackAtVtxMAX_(iConfig.getParameter<double>("deltaEtaSuperClusterTrackAtVtxMAX")),
  sigmaIetaIetaBarrelMIN_           (iConfig.getParameter<double>("sigmaIetaIetaBarrelMIN")),
  sigmaIetaIetaEndcapMIN_           (iConfig.getParameter<double>("sigmaIetaIetaEndcapMIN")),
  ele_ptLow_                        (iConfig.getParameter<double>("ele_ptLow")),
  ele_ptHigh_                       (iConfig.getParameter<double>("ele_ptHigh")),
  nEle_ptLowMIN_                    (iConfig.getParameter<int>("nEle_ptLowMIN")),
  nEle_ptHighMIN_                   (iConfig.getParameter<int>("nEle_ptHighMIN"))
{}



//! dtor
SkimAnalyzer::~SkimAnalyzer()
{}




//! analyze method
void SkimAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  int runId = iEvent.id().run();
  int lumiId = iEvent.luminosityBlock();
  int eventId = iEvent.id().event();
  
  
  
  edm::Handle<reco::GsfElectronCollection> EleHandle;
  iEvent.getByLabel(electronCollection_.label(), EleHandle);
  
  
  
  // loop on electrons
  int nEle_ptLow = 0;
  int nEle_ptHigh = 0;
  for(unsigned int eleIt = 0; eleIt < EleHandle->size(); ++eleIt)
  {
    
    reco::GsfElectronRef eleRef(EleHandle, eleIt);
    
    // loose eleId cuts
    if( eleRef->fbrem() < fbremMIN_ ) continue;
    if( eleRef->hcalOverEcal() > hcalOverEcalMAX_ ) continue;
    if( fabs(eleRef->deltaPhiSuperClusterTrackAtVtx()) > deltaPhiSuperClusterTrackAtVtxMAX_ ) continue;
    if( fabs(eleRef->deltaEtaSuperClusterTrackAtVtx()) > deltaEtaSuperClusterTrackAtVtxMAX_ ) continue;
    if( (eleRef->isEB()) && (eleRef->sigmaIetaIeta() < sigmaIetaIetaBarrelMIN_) ) continue; 
    if( (eleRef->isEE()) && (eleRef->sigmaIetaIeta() < sigmaIetaIetaEndcapMIN_) ) continue; 
    
    // count electrons
    if( eleRef->pt() > ele_ptLow_) ++nEle_ptLow;
    if( eleRef->pt() > ele_ptHigh_) ++nEle_ptHigh;
    
  } // loop on electrons
  
  
  
  // print out results
  if( nEle_ptLow >= nEle_ptLowMIN_ )
    std::cout << ">>>SkimAnalyzer::run=" << runId << "::lumi=" 
                                         << lumiId << "::eventId=" 
                                         << eventId << "::Found at least "
                                         << nEle_ptLowMIN_ << " electrons with pt > " << ele_ptLow_ << std::endl; 
  if( nEle_ptHigh >= nEle_ptHighMIN_ )
    std::cout << ">>>SkimAnalyzer::run=" << runId << "::lumi=" 
                                         << lumiId << "::eventId=" 
                                         << eventId << "::Found at least "
                                         << nEle_ptHighMIN_ << " electrons with pt > " << ele_ptHigh_ << std::endl; 
}
