import FWCore.ParameterSet.Config as cms

VBFLeptonDistributions = cms.EDAnalyzer(
    "VBFLeptonDistributions",
    srcGenParticles              = cms.InputTag("genParticles"),
    srcRecoElectrons             = cms.InputTag("pixelMatchGsfElectrons"),
    srcRecoElectronTkIsolations  = cms.InputTag("eleIsoFromDepsTk"),
    srcRecoElectronEmIsolations  = cms.InputTag("eleIsoFromDepsEcalFromHits"),
    srcRecoElectronHadIsolations = cms.InputTag("eleIsoFromDepsHcalFromHits"),
    srcRecoElectronIdLoose       = cms.InputTag("eidLoose"),
    srcRecoElectronIdTight       = cms.InputTag("eidTight"),
    srcRecoElectronIdRobustLoose = cms.InputTag("eidRobustLoose"),
    srcRecoElectronIdRobustTight = cms.InputTag("eidRobustTight"),    
    srcRecoMuons                 = cms.InputTag("muons"),
    fileName                     = cms.untracked.string("VBFNtuple"),
    verbosity                    = cms.untracked.bool(True),
    eventsToPrint                = cms.untracked.int32(1),
    eventType                    = cms.untracked.int32(0)
    )
