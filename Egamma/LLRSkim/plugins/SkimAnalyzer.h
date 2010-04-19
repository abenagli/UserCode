#ifndef SkimAnalyzer_h
#define SkimAnalyzer_h

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include <iostream>
#include <memory>



//
// class declaration
//

class SkimAnalyzer : public edm::EDAnalyzer 
{
 public:
  
  explicit SkimAnalyzer(const edm::ParameterSet&);
  ~SkimAnalyzer();
  
  
  
 private:
	
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  edm::InputTag electronCollection_;
  //edm::InputTag clusterCollection_;
  //edm::InputTag gsftrackCollection_;
  
  double fbremMIN_;
  double hcalOverEcalMAX_;
  double deltaPhiSuperClusterTrackAtVtxMAX_;
  double deltaEtaSuperClusterTrackAtVtxMAX_;
  double sigmaIetaIetaBarrelMIN_;
  double sigmaIetaIetaEndcapMIN_;
  double ele_ptLow_;
  double ele_ptHigh_;
  double nEle_ptLowMIN_;
  double nEle_ptHighMIN_;
  
};

#endif
