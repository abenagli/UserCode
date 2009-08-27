import FWCore.ParameterSet.Config as cms

VBFPtMinLeptonCountFilter = cms.EDFilter(
    "VBFPtMinLeptonCountFilter",
    srcElectrons = cms.InputTag("VBFIdSelectedElectrons"),
    srcMuons     = cms.InputTag("VBFIsolatedMuons"),
    ptMin  = cms.double(10.),
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    minNumber = cms.int32(1)
    )
