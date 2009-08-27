#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"



//define this as a plug-in
DEFINE_SEAL_MODULE();






//-------------------------------------------------
// VBFLeptonDistributions
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFLeptonDistributions.h"
DEFINE_ANOTHER_FWK_MODULE(VBFLeptonDistributions);












//-------------------------------------------------
// VBFElectronAmbiguityResolver
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFElectronAmbiguityResolver.h"
typedef ObjectSelector<VBFElectronAmbiguityResolver> VBFElectronAmbiguityResolution;
typedef ObjectSelector<VBFElectronAmbiguityResolver, edm::RefVector<reco::GsfElectronCollection> > VBFElectronAmbiguityResolutionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFElectronAmbiguityResolution);
DEFINE_ANOTHER_FWK_MODULE(VBFElectronAmbiguityResolutionRef);


//-------------------------------------------------
// VBFElectronSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFElectronSelector.h"
typedef ObjectSelector<VBFElectronSelector> VBFElectronSelection;
typedef ObjectSelector<VBFElectronSelector, edm::RefVector<reco::GsfElectronCollection> > VBFElectronSelectionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFElectronSelection);
DEFINE_ANOTHER_FWK_MODULE(VBFElectronSelectionRef);


//-------------------------------------------------
// VBFElectronIsolator
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFElectronIsolator.h"
typedef ObjectSelector<VBFElectronIsolator> VBFElectronIsolation;
typedef ObjectSelector<VBFElectronIsolator, edm::RefVector<reco::GsfElectronCollection> > VBFElectronIsolationRef;
DEFINE_ANOTHER_FWK_MODULE(VBFElectronIsolation);
DEFINE_ANOTHER_FWK_MODULE(VBFElectronIsolationRef);


//-------------------------------------------------
// VBFElectronIdSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFElectronIdSelector.h"
typedef ObjectSelector<VBFElectronIdSelector> VBFElectronIdSelection;
typedef ObjectSelector<VBFElectronIdSelector, edm::RefVector<reco::GsfElectronCollection> > VBFElectronIdSelectionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFElectronIdSelection);
DEFINE_ANOTHER_FWK_MODULE(VBFElectronIdSelectionRef);






//-------------------------------------------------
// VBFMuonSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFMuonSelector.h"
typedef ObjectSelector<VBFMuonSelector> VBFMuonSelection;
typedef ObjectSelector<VBFMuonSelector, edm::RefVector<reco::MuonCollection> > VBFMuonSelectionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFMuonSelection);
DEFINE_ANOTHER_FWK_MODULE(VBFMuonSelectionRef);


//-------------------------------------------------
// VBFMuonIsolator
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFMuonIsolator.h"
typedef ObjectSelector<VBFMuonIsolator> VBFMuonIsolation;
typedef ObjectSelector<VBFMuonIsolator, edm::RefVector<reco::MuonCollection> > VBFMuonIsolationRef;
DEFINE_ANOTHER_FWK_MODULE(VBFMuonIsolation);
DEFINE_ANOTHER_FWK_MODULE(VBFMuonIsolationRef);






//-------------------------------------------------
// VBFJetSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFJetSelector.h"
typedef ObjectSelector<VBFJetSelector<reco::CaloJetCollection> > VBFCaloJetSelection;
typedef ObjectSelector<VBFJetSelector<reco::CaloJetCollection>, edm::RefVector<reco::CaloJetCollection> > VBFCaloJetSelectionRef;
DEFINE_ANOTHER_FWK_MODULE(VBFCaloJetSelection);
DEFINE_ANOTHER_FWK_MODULE(VBFCaloJetSelectionRef);






//-------------------------------------------------
// VBFPtMinLeptonCountFilter / VBFEtMinJetCountFilter
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFPtMinLeptonCountFilter.h"
DEFINE_ANOTHER_FWK_MODULE(VBFPtMinLeptonCountFilter);

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFEtMinJetCountFilter.h"
typedef VBFEtMinJetCountFilter<reco::CaloJetCollection> VBFEtMinCaloJetCountFilter;
typedef VBFEtMinJetCountFilter<reco::PFJetCollection>   VBFEtMinPFJetCountFilter;
DEFINE_ANOTHER_FWK_MODULE(VBFEtMinCaloJetCountFilter);
DEFINE_ANOTHER_FWK_MODULE(VBFEtMinPFJetCountFilter);






//-------------------------------------------------
// VBFNtuple
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFNtupleCaloJets.h"
DEFINE_ANOTHER_FWK_MODULE(VBFNtupleCaloJets);
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFNtuplePFJets.h"
DEFINE_ANOTHER_FWK_MODULE(VBFNtuplePFJets);

