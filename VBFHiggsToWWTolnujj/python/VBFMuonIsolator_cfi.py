import FWCore.ParameterSet.Config as cms

VBFIsolatedMuons = cms.EDFilter(
    "VBFMuonIsolation",
    src = cms.InputTag("muons"),
    tkIsoCut  = cms.double(0.2),
    emIsoCut  = cms.double(9999.),
    hadIsoCut = cms.double(999.),
    useCombinedIso = cms.bool(False),
    combinedIsoCut = cms.double(9999.),
    tkIsoCoeff  = cms.double(1.),
    emIsoCoeff  = cms.double(1.),
    hadIsoCoeff = cms.double(1.),
    dividePt = cms.bool(True),
    doRefCheck = cms.bool(True),
    srcMuonsRef = cms.InputTag("VBFSelectedMuonsRef")
    )



VBFIsolatedMuonsRef = cms.EDFilter(
    "VBFMuonIsolationRef",
    src = cms.InputTag("muons"),
    tkIsoCut  = cms.double(0.2),
    emIsoCut  = cms.double(9999.),
    hadIsoCut = cms.double(999.),
    useCombinedIso = cms.bool(False),
    combinedIsoCut = cms.double(9999.),
    tkIsoCoeff  = cms.double(1.),
    emIsoCoeff  = cms.double(1.),
    hadIsoCoeff = cms.double(1.),
    dividePt = cms.bool(True),
    doRefCheck = cms.bool(True),
    srcMuonsRef = cms.InputTag("VBFSelectedMuonsRef")
    )
