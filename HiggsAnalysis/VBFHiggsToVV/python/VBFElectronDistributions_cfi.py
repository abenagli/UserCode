import FWCore.ParameterSet.Config as cms



VBFElectronDistributions = cms.EDAnalyzer(
    "VBFElectronDistributions",
    srcGenParticles                = cms.InputTag("genParticles"),
    srcElectrons                   = cms.InputTag("gsfElectrons"),
    srcElectronTkIsoValues         = cms.InputTag("eleIsoFromDepsTk"),
    srcElectronEmIsoValues         = cms.InputTag("eleIsoFromDepsEcalFromHits"),
    srcElectronHadIsoValues        = cms.InputTag("eleIsoFromDepsHcalFromHits"),
    srcElectronIdLooseValues       = cms.InputTag("eidLoose"),
    srcElectronIdTightValues       = cms.InputTag("eidTight"),
    srcElectronIdRobustLooseValues = cms.InputTag("eidRobustLoose"),
    srcElectronIdRobustTightValues = cms.InputTag("eidRobustTight"),
    tkIsoCoeff  = cms.double(1.),
    emIsoCoeff  = cms.double(1.),
    hadIsoCoeff = cms.double(1.),
    doRefCheck = cms.bool(False),
    srcElectronsRef = cms.InputTag("gsfElectrons"),
    fileName  = cms.untracked.string("VBFElectronDistributions"),
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFElectronDistributionsSequence = cms.Sequence(
    VBFElectronDistributions
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
