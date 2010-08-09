import FWCore.ParameterSet.Config as cms



VBFStdPreselectionSelectedElectrons = cms.EDFilter(
    "VBFElectronStdPreselectionSelection",
    src = cms.InputTag("gsfElectrons"),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFStdPreselectionSelectedElectronsRef = cms.EDFilter(
    "VBFElectronStdPreselectionSelectionRef",
    src = cms.InputTag("gsfElectrons"),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFStdPreselectionSelectedElectronsSequence = cms.Sequence(
    VBFStdPreselectionSelectedElectrons *
    VBFStdPreselectionSelectedElectronsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
