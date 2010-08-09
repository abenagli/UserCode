import FWCore.ParameterSet.Config as cms



VBFSelectedElectrons = cms.EDFilter(
    "VBFElectronSelection",
    src = cms.InputTag("gsfElectrons"),
    ptMin  = cms.double(5.),
    ptMax  = cms.double(7000.),    
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSelectedElectronsRef = cms.EDFilter(
    "VBFElectronSelectionRef",
    src = cms.InputTag("gsfElectrons"),
    ptMin  = cms.double(5.),
    ptMax  = cms.double(7000.),    
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSelectedElectronsSequence = cms.Sequence(
    VBFSelectedElectrons *
    VBFSelectedElectronsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
