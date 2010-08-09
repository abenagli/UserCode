import FWCore.ParameterSet.Config as cms



VBFIdSelectedMuons = cms.EDFilter(
    "VBFMuonIdSelection",
    src = cms.InputTag("muons"),
    srcVertices  = cms.InputTag("offlinePrimaryVertices"),
    normalizedChi2Max = cms.double(10),
    numberOfValidTrackerHitsMin = cms.int32(11),
    numberOfValidMuonHitsMin = cms.int32(1),
    d0Max = cms.double(2.),
    doRefCheck = cms.bool(False),
    srcMuonsRef = cms.InputTag("muons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFIdSelectedMuonsRef = cms.EDFilter(
    "VBFMuonIdSelectionRef",
    src = cms.InputTag("muons"),
    srcVertices  = cms.InputTag("offlinePrimaryVertices"),
    normalizedChi2Max = cms.double(10),
    numberOfValidTrackerHitsMin = cms.int32(11),
    numberOfValidMuonHitsMin = cms.int32(1),
    d0Max = cms.double(2.),
    doRefCheck = cms.bool(False),
    srcMuonsRef = cms.InputTag("muons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFIdSelectedMuonsSequence = cms.Sequence(
    VBFIdSelectedMuons *
    VBFIdSelectedMuonsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
