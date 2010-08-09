import FWCore.ParameterSet.Config as cms



VBFIdSelectedElectrons = cms.EDFilter(
    "VBFElectronIdSelection",
    src = cms.InputTag("gsfElectrons"),
    fbremMIN = cms.double(0.),
    hcalOverEcalMAX = cms.double(0.1),
    deltaPhiSuperClusterTrackAtVtxMAX = cms.double(0.10),
    deltaEtaSuperClusterTrackAtVtxMAX = cms.double(0.015),
    sigmaIetaIetaMAX = cms.double(0.1),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFIdSelectedElectronsRef = cms.EDFilter(
    "VBFElectronIdSelectionRef",
    src = cms.InputTag("gsfElectrons"),
    fbremMIN = cms.double(0.),
    hcalOverEcalMAX = cms.double(0.1),
    deltaPhiSuperClusterTrackAtVtxMAX = cms.double(0.10),
    deltaEtaSuperClusterTrackAtVtxMAX = cms.double(0.015),
    sigmaIetaIetaMAX = cms.double(0.1),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFIdSelectedElectronsSequence = cms.Sequence(
    VBFIdSelectedElectrons *
    VBFIdSelectedElectronsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
