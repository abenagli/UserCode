import FWCore.ParameterSet.Config as cms



VBFLeptonDistributions = cms.EDAnalyzer(
    "VBFLeptonDistributions",
    srcGenParticles                = cms.InputTag("genParticles"),
    srcElectrons                   = cms.InputTag("gsfElectrons"),
    srcElectronTkIsoValues         = cms.InputTag("eleIsoFromDepsTk"),
    srcElectronEmIsoValues         = cms.InputTag("eleIsoFromDepsEcalFromHits"),
    srcElectronHadIsoValues        = cms.InputTag("eleIsoFromDepsHcalFromHits"),
    srcElectronIdLooseValues       = cms.InputTag("eidLoose"),
    srcElectronIdTightValues       = cms.InputTag("eidTight"),
    srcElectronIdRobustLooseValues = cms.InputTag("eidRobustLoose"),
    srcElectronIdRobustTightValues = cms.InputTag("eidRobustTight"),
    srcMuons                       = cms.InputTag("muons"),
    tkIsoCoeff  = cms.double(1.),
    emIsoCoeff  = cms.double(1.),
    hadIsoCoeff = cms.double(1.),
    doElectronRefCheck = cms.bool(False),
    srcElectronsRef    = cms.InputTag("gsfElectrons"),
    doMuonRefCheck     = cms.bool(False),
    srcMuonsRef        = cms.InputTag("muons"),
    fileName  = cms.untracked.string("VBFLeptonDistributions"),
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFLeptonDistributionsSequence = cms.Sequence(
    VBFLeptonDistributions
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
