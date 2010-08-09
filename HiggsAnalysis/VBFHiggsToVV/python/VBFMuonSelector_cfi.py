import FWCore.ParameterSet.Config as cms



VBFSelectedMuons = cms.EDFilter(
    "VBFMuonSelection",
    src = cms.InputTag("muons"),
    ptMin  = cms.double(5.),
    ptMax  = cms.double(7000.),    
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    doRefCheck = cms.bool(False),
    srcMuonsRef = cms.InputTag("muons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSelectedMuonsRef = cms.EDFilter(
    "VBFMuonSelectionRef",
    src = cms.InputTag("muons"),
    ptMin  = cms.double(5.),
    ptMax  = cms.double(7000.),
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    doRefCheck = cms.bool(False),
    srcMuonsRef = cms.InputTag("muons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFSelectedMuonsSequence = cms.Sequence(
    VBFSelectedMuons *
    VBFSelectedMuonsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
