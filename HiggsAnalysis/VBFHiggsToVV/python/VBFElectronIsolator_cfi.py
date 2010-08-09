import FWCore.ParameterSet.Config as cms



VBFIsolatedElectrons = cms.EDFilter(
    "VBFElectronIsolation",
    src = cms.InputTag("gsfElectrons"),
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
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFIsolatedElectronsRef = cms.EDFilter(
    "VBFElectronIsolationRef",
    src = cms.InputTag("gsfElectrons"),
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
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFIsolatedElectronsSequence = cms.Sequence(
    VBFIsolatedElectrons *
    VBFIsolatedElectronsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
