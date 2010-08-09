import FWCore.ParameterSet.Config as cms



VBFMetDistributions = cms.EDAnalyzer(
    "VBFMetDistributions",
    srcGenMetNoNuBSM        = cms.InputTag("genMetNoNuBSM"),
    srcCaloMet              = cms.InputTag("met"),
    srcMuonCorrectedCaloMet = cms.InputTag("corMetGlobalMuons"),
    srcPFMet                = cms.InputTag("pfMet"),
    fileName  = cms.untracked.string("VBFMetDistributions"),
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFMetDistributionsSequence = cms.Sequence(
    VBFMetDistributions
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
