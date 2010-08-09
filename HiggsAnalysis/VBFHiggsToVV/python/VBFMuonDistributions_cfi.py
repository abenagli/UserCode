import FWCore.ParameterSet.Config as cms



VBFMuonDistributions = cms.EDAnalyzer(
    "VBFMuonDistributions",
    srcGenParticles = cms.InputTag("genParticles"),
    srcMuons        = cms.InputTag("muons"),
    tkIsoCoeff  = cms.double(1.),
    emIsoCoeff  = cms.double(1.),
    hadIsoCoeff = cms.double(1.),
    doRefCheck = cms.bool(False),
    srcMuonsRef = cms.InputTag("muons"),
    fileName  = cms.untracked.string("VBFMuonDistributions"),
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFMuonDistributionsSequence = cms.Sequence(
    VBFMuonDistributions
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
