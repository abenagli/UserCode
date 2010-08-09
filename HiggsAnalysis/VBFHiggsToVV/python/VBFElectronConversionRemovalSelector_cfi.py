import FWCore.ParameterSet.Config as cms



VBFConversionRemovalSelectedElectrons = cms.EDFilter(
    "VBFElectronConversionRemovalSelection",
    src = cms.InputTag("gsfElectrons"),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFConversionRemovalSelectedElectronsRef = cms.EDFilter(
    "VBFElectronConversionRemovalSelectionRef",
    src = cms.InputTag("gsfElectrons"),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFConversionRemovalSelectedElectronsSequence = cms.Sequence(
    VBFConversionRemovalSelectedElectrons *
    VBFConversionRemovalSelectedElectronsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
