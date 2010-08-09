import FWCore.ParameterSet.Config as cms



VBFAmbiguityResolvedElectrons = cms.EDFilter(
    "VBFElectronAmbiguityResolution",
    src = cms.InputTag("gsfElectrons"),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFAmbiguityResolvedElectronsRef = cms.EDFilter(
    "VBFElectronAmbiguityResolutionRef",
    src = cms.InputTag("gsfElectrons"),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFAmbiguityResolvedElectronsSequence = cms.Sequence(
    VBFAmbiguityResolvedElectrons *
    VBFAmbiguityResolvedElectronsRef 
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
