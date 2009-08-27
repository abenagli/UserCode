import FWCore.ParameterSet.Config as cms

VBFIdSelectedElectrons = cms.EDFilter(
    "VBFElectronIdSelection",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    srcElectronIdValues = cms.InputTag("eidRobustLoose"),
    doRefCheck = cms.bool(True),
    srcElectronsRef = cms.InputTag("VBFIsolatedElectronsRef")
    )



VBFIdSelectedElectronsRef = cms.EDFilter(
    "VBFElectronIdSelectionRef",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    srcElectronIdValues = cms.InputTag("eidRobustLoose"),
    doRefCheck = cms.bool(True),
    srcElectronsRef = cms.InputTag("VBFIsolatedElectronsRef")
    )
