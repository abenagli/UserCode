import FWCore.ParameterSet.Config as cms

VBFAmbiguityResolvedElectrons = cms.EDFilter(
    "VBFElectronAmbiguityResolution",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("pixelMatchGsfElectrons")
    )



VBFAmbiguityResolvedElectronsRef = cms.EDFilter(
    "VBFElectronAmbiguityResolutionRef",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("pixelMatchGsfElectrons")
    )
