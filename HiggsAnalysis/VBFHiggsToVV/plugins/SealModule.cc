#include <FWCore/PluginManager/interface/ModuleDef.h>
#include <FWCore/Framework/interface/MakerMacros.h>

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"
#include "CommonTools/UtilAlgos/interface/SortCollectionSelector.h"






//-------------------------------------------------
// VBFElectron/Muon/Lepton/Jet/MetDistributions
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronDistributions.h"
DEFINE_FWK_MODULE(VBFElectronDistributions);
#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonDistributions.h"
DEFINE_FWK_MODULE(VBFMuonDistributions);
#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFLeptonDistributions.h"
DEFINE_FWK_MODULE(VBFLeptonDistributions);
#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMetDistributions.h"
DEFINE_FWK_MODULE(VBFMetDistributions);

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFJetDistributions.h"
typedef VBFJetDistributions<reco::CaloJetCollection> VBFCaloJetDistributions;
typedef VBFJetDistributions<reco::PFJetCollection> VBFPFJetDistributions;
DEFINE_FWK_MODULE(VBFCaloJetDistributions);
DEFINE_FWK_MODULE(VBFPFJetDistributions);






//-------------------------------------------------
// VBFElectronAmbiguityResolver
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronAmbiguityResolver.h"
typedef ObjectSelector<VBFElectronAmbiguityResolver> VBFElectronAmbiguityResolution;
typedef ObjectSelector<VBFElectronAmbiguityResolver, edm::RefVector<reco::GsfElectronCollection> > VBFElectronAmbiguityResolutionRef;
DEFINE_FWK_MODULE(VBFElectronAmbiguityResolution);
DEFINE_FWK_MODULE(VBFElectronAmbiguityResolutionRef);


//-------------------------------------------------
// VBFElectronStdPreselectionSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronStdPreselectionSelector.h"
typedef ObjectSelector<VBFElectronStdPreselectionSelector> VBFElectronStdPreselectionSelection;
typedef ObjectSelector<VBFElectronStdPreselectionSelector, edm::RefVector<reco::GsfElectronCollection> > VBFElectronStdPreselectionSelectionRef;
DEFINE_FWK_MODULE(VBFElectronStdPreselectionSelection);
DEFINE_FWK_MODULE(VBFElectronStdPreselectionSelectionRef);



//-------------------------------------------------
// VBFElectronSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronSelector.h"
typedef ObjectSelector<VBFElectronSelector> VBFElectronSelection;
typedef ObjectSelector<VBFElectronSelector, edm::RefVector<reco::GsfElectronCollection> > VBFElectronSelectionRef;
DEFINE_FWK_MODULE(VBFElectronSelection);
DEFINE_FWK_MODULE(VBFElectronSelectionRef);


//-------------------------------------------------
// VBFElectronIsolator
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronIsolator.h"
typedef ObjectSelector<VBFElectronIsolator> VBFElectronIsolation;
typedef ObjectSelector<VBFElectronIsolator, edm::RefVector<reco::GsfElectronCollection> > VBFElectronIsolationRef;
DEFINE_FWK_MODULE(VBFElectronIsolation);
DEFINE_FWK_MODULE(VBFElectronIsolationRef);


//-------------------------------------------------
// VBFElectronIdSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronIdSelector.h"
typedef ObjectSelector<VBFElectronIdSelector> VBFElectronIdSelection;
typedef ObjectSelector<VBFElectronIdSelector, edm::RefVector<reco::GsfElectronCollection> > VBFElectronIdSelectionRef;
DEFINE_FWK_MODULE(VBFElectronIdSelection);
DEFINE_FWK_MODULE(VBFElectronIdSelectionRef);



//-------------------------------------------------
// VBFElectronSpikeRemovalSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronSpikeRemovalSelector.h"
typedef ObjectSelector<VBFElectronSpikeRemovalSelector> VBFElectronSpikeRemovalSelection;
typedef ObjectSelector<VBFElectronSpikeRemovalSelector, edm::RefVector<reco::GsfElectronCollection> > VBFElectronSpikeRemovalSelectionRef;
DEFINE_FWK_MODULE(VBFElectronSpikeRemovalSelection);
DEFINE_FWK_MODULE(VBFElectronSpikeRemovalSelectionRef);



//-------------------------------------------------
// VBFElectronConversionRemovalSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronConversionRemovalSelector.h"
typedef ObjectSelector<VBFElectronConversionRemovalSelector> VBFElectronConversionRemovalSelection;
typedef ObjectSelector<VBFElectronConversionRemovalSelector, edm::RefVector<reco::GsfElectronCollection> > VBFElectronConversionRemovalSelectionRef;
DEFINE_FWK_MODULE(VBFElectronConversionRemovalSelection);
DEFINE_FWK_MODULE(VBFElectronConversionRemovalSelectionRef);






//-------------------------------------------------
// VBFMuonSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonSelector.h"
typedef ObjectSelector<VBFMuonSelector> VBFMuonSelection;
typedef ObjectSelector<VBFMuonSelector, edm::RefVector<reco::MuonCollection> > VBFMuonSelectionRef;
DEFINE_FWK_MODULE(VBFMuonSelection);
DEFINE_FWK_MODULE(VBFMuonSelectionRef);


//-------------------------------------------------
// VBFMuonIsolator
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonIsolator.h"
typedef ObjectSelector<VBFMuonIsolator> VBFMuonIsolation;
typedef ObjectSelector<VBFMuonIsolator, edm::RefVector<reco::MuonCollection> > VBFMuonIsolationRef;
DEFINE_FWK_MODULE(VBFMuonIsolation);
DEFINE_FWK_MODULE(VBFMuonIsolationRef);



//-------------------------------------------------
// VBFMuonIdSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonIdSelector.h"
typedef ObjectSelector<VBFMuonIdSelector> VBFMuonIdSelection;
typedef ObjectSelector<VBFMuonIdSelector, edm::RefVector<reco::MuonCollection> > VBFMuonIdSelectionRef;
DEFINE_FWK_MODULE(VBFMuonIdSelection);
DEFINE_FWK_MODULE(VBFMuonIdSelectionRef);






//-------------------------------------------------                                                                                             
// VBFLepton3D/TipLipProducer                                                                                                                      
//-------------------------------------------------                                                                                             

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFLepton3DipProducer.h"
DEFINE_FWK_MODULE(VBFLepton3DipProducer);

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFLeptonTipLipProducer.h"
DEFINE_FWK_MODULE(VBFLeptonTipLipProducer);






//-------------------------------------------------
// VBFJetCleaner
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFJetCleaner.h"
typedef ObjectSelector<VBFJetCleaner<reco::CaloJetCollection> > VBFCaloJetCleaning;
typedef ObjectSelector<VBFJetCleaner<reco::CaloJetCollection>, edm::RefVector<reco::CaloJetCollection> > VBFCaloJetCleaningRef;
typedef ObjectSelector<VBFJetCleaner<reco::PFJetCollection> > VBFPFJetCleaning;
typedef ObjectSelector<VBFJetCleaner<reco::PFJetCollection>, edm::RefVector<reco::PFJetCollection> > VBFPFJetCleaningRef;
DEFINE_FWK_MODULE(VBFCaloJetCleaning);
DEFINE_FWK_MODULE(VBFCaloJetCleaningRef);
DEFINE_FWK_MODULE(VBFPFJetCleaning);
DEFINE_FWK_MODULE(VBFPFJetCleaningRef);


//-------------------------------------------------
// VBFJetSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFJetSelector.h"
typedef ObjectSelector<VBFJetSelector<reco::CaloJetCollection> > VBFCaloJetSelection;
typedef ObjectSelector<VBFJetSelector<reco::CaloJetCollection>, edm::RefVector<reco::CaloJetCollection> > VBFCaloJetSelectionRef;
typedef ObjectSelector<VBFJetSelector<reco::PFJetCollection> > VBFPFJetSelection;
typedef ObjectSelector<VBFJetSelector<reco::PFJetCollection>, edm::RefVector<reco::PFJetCollection> > VBFPFJetSelectionRef;
DEFINE_FWK_MODULE(VBFCaloJetSelection);
DEFINE_FWK_MODULE(VBFCaloJetSelectionRef);
DEFINE_FWK_MODULE(VBFPFJetSelection);
DEFINE_FWK_MODULE(VBFPFJetSelectionRef);


//-------------------------------------------------
// VBFCaloJetIdSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFCaloJetIdSelector.h"
typedef ObjectSelector<VBFCaloJetIdSelector> VBFCaloJetIdSelection;
typedef ObjectSelector<VBFCaloJetIdSelector, edm::RefVector<reco::CaloJetCollection> > VBFCaloJetIdSelectionRef;
DEFINE_FWK_MODULE(VBFCaloJetIdSelection);
DEFINE_FWK_MODULE(VBFCaloJetIdSelectionRef);


//-------------------------------------------------
// VBFPFJetIdSelector
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFPFJetIdSelector.h"
typedef ObjectSelector<VBFPFJetIdSelector> VBFPFJetIdSelection;
typedef ObjectSelector<VBFPFJetIdSelector, edm::RefVector<reco::PFJetCollection> > VBFPFJetIdSelectionRef;
DEFINE_FWK_MODULE(VBFPFJetIdSelection);
DEFINE_FWK_MODULE(VBFPFJetIdSelectionRef);






//-------------------------------------------------
// VBFAllPassFilter
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFAllPassFilter.h"
DEFINE_FWK_MODULE(VBFAllPassFilter);






//-------------------------------------------------
// VBFPtMinLeptonCountFilter / VBFEtMinJetCountFilter
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFPtMinLeptonCountFilter.h"
DEFINE_FWK_MODULE(VBFPtMinLeptonCountFilter);

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFEtMinJetCountFilter.h"
typedef VBFEtMinJetCountFilter<reco::CaloJetCollection> VBFEtMinCaloJetCountFilter;
typedef VBFEtMinJetCountFilter<reco::PFJetCollection>   VBFEtMinPFJetCountFilter;
DEFINE_FWK_MODULE(VBFEtMinCaloJetCountFilter);
DEFINE_FWK_MODULE(VBFEtMinPFJetCountFilter);

#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFFwdJetCountFilter.h"
typedef VBFFwdJetCountFilter<reco::CaloJetCollection> VBFFwdCaloJetCountFilter;
typedef VBFFwdJetCountFilter<reco::PFJetCollection>   VBFFwdPFJetCountFilter;
DEFINE_FWK_MODULE(VBFFwdCaloJetCountFilter);
DEFINE_FWK_MODULE(VBFFwdPFJetCountFilter);



//-------------
// SimpleNtuple 
//-------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/SimpleNtple.h"
DEFINE_FWK_MODULE(SimpleNtple) ;










//----------------------------------
//----------- MonteCarlo -----------
//----------------------------------

//-------------------------
// MCDecayModeFilter
//-------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/MCDecayModeFilter.h"
DEFINE_FWK_MODULE(MCDecayModeFilter);



//-------------------------
// MCPtMinLeptonCountFilter 
//-------------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/MCPtMinLeptonCountFilter.h"
DEFINE_FWK_MODULE(MCPtMinLeptonCountFilter);


//----------------------
// MCFwdJetsPreselFilter 
//----------------------

#include "HiggsAnalysis/VBFHiggsToVV/plugins/MCFwdJetsPreselFilter.h"
DEFINE_FWK_MODULE(MCFwdJetsPreselFilter) ;







/*
#include "HiggsAnalysis/VBFHiggsToVV/plugins/FwdJetsPreselFilter.h"
DEFINE_FWK_MODULE(FwdJetsPreselFilter) ;

#include "HiggsAnalysis/VBFHiggsToVV/plugins/LepMinNumFilter.h"
DEFINE_FWK_MODULE(LepMinNumFilter) ;*/
