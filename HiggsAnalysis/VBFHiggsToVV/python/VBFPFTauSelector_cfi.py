import FWCore.ParameterSet.Config as cms



VBFSelectedPFTaus = cms.EDFilter(
    "VBFPFTauSelection",
    src = cms.InputTag("shrinkingConePFTauProducer"),
    ptMin  = cms.double(5.),
    ptMax  = cms.double(7000.),    
    etaMin = cms.double(-5.),
    etaMax = cms.double(+5.),
    doRefCheck = cms.bool(False),
    srcPFTausRef = cms.InputTag("shrinkingConePFTauProducer")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSelectedPFTausRef = cms.EDFilter(
    "VBFPFTauSelectionRef",
    src = cms.InputTag("shrinkingConePFTauProducer"),
    ptMin  = cms.double(5.),
    ptMax  = cms.double(7000.),
    etaMin = cms.double(-5.),
    etaMax = cms.double(+5.),
    doRefCheck = cms.bool(False),
    srcPFTausRef = cms.InputTag("shrinkingConePFTauProducer")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFSelectedPFTausSequence = cms.Sequence(
    VBFSelectedPFTaus *
    VBFSelectedPFTausRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
