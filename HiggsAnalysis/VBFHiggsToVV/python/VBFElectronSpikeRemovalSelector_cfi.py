import FWCore.ParameterSet.Config as cms



VBFSpikeRemovalSelectedElectrons = cms.EDFilter(
    "VBFElectronSpikeRemovalSelection",
    src = cms.InputTag("gsfElectrons"),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSpikeRemovalSelectedElectronsRef = cms.EDFilter(
    "VBFElectronSpikeRemovalSelectionRef",
    src = cms.InputTag("gsfElectrons"),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFSpikeRemovalSelectedElectronsSequence = cms.Sequence(
    VBFSpikeRemovalSelectedElectrons *
    VBFSpikeRemovalSelectedElectronsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
