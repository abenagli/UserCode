import FWCore.ParameterSet.Config as cms

VBFEtMinCaloJetCountFilter = cms.EDFilter(
    "VBFEtMinCaloJetCountFilter",
    srcJets = cms.InputTag("VBFSelectedCaloJets"),
    etMin  = cms.double(10.),
    etaMin = cms.double(-5.),
    etaMax = cms.double(+5.),
    minNumber = cms.int32(2)
    )
