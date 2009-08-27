import FWCore.ParameterSet.Config as cms

VBFSelectedMuons = cms.EDFilter(
    "VBFMuonSelection",
    src = cms.InputTag("muons"),
    ptMin  = cms.double(5.),
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    doRefCheck = cms.bool(False),
    srcMuonsRef = cms.InputTag("muonsRef")
    )



VBFSelectedMuonsRef = cms.EDFilter(
    "VBFMuonSelectionRef",
    src = cms.InputTag("muons"),
    ptMin  = cms.double(5.),
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    doRefCheck = cms.bool(False),
    srcMuonsRef = cms.InputTag("muonsRef")
    )
