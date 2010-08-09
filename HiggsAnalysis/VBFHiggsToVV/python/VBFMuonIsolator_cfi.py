import FWCore.ParameterSet.Config as cms



VBFIsolatedMuons = cms.EDFilter(
    "VBFMuonIsolation",
    src = cms.InputTag("muons"),
    tkIsoCut  = cms.double(9999.),
    emIsoCut  = cms.double(9999.),
    hadIsoCut = cms.double(9999.),
    useCombinedIso = cms.bool(False),
    combinedIsoCut = cms.double(9999.),
    tkIsoCoeff  = cms.double(1.),
    emIsoCoeff  = cms.double(1.),
    hadIsoCoeff = cms.double(1.),
    dividePt = cms.bool(True),
    doRefCheck = cms.bool(False),
    srcMuonsRef = cms.InputTag("muons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFIsolatedMuonsRef = cms.EDFilter(
    "VBFMuonIsolationRef",
    src = cms.InputTag("muons"),
    tkIsoCut  = cms.double(9999.),
    emIsoCut  = cms.double(9999.),
    hadIsoCut = cms.double(9999.),
    useCombinedIso = cms.bool(False),
    combinedIsoCut = cms.double(9999.),
    tkIsoCoeff  = cms.double(1.),
    emIsoCoeff  = cms.double(1.),
    hadIsoCoeff = cms.double(1.),
    dividePt = cms.bool(True),
    doRefCheck = cms.bool(False),
    srcMuonsRef = cms.InputTag("muons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFIsolatedMuonsSequence = cms.Sequence(
    VBFIsolatedMuons *
    VBFIsolatedMuonsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
