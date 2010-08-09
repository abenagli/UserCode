import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFCaloJetDistributions = cms.EDAnalyzer(
    "VBFCaloJetDistributions",
    srcGenParticles = cms.InputTag("genParticles"),
    srcJets         = cms.InputTag("ak5CaloJets"),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("ak5CaloJets"),
    fileName  = cms.untracked.string("VBFCaloJetDistributions"),
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFCaloJetDistributionsSequence = cms.Sequence(
    VBFCaloJetDistributions
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 






# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFPFJetDistributions = cms.EDAnalyzer(
    "VBFPFJetDistributions",
    srcGenParticles = cms.InputTag("genParticles"),
    srcJets         = cms.InputTag("ak5PFJets"),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("ak5PFJets"),
    fileName  = cms.untracked.string("VBFPFJetDistributions"),
    verbosity = cms.untracked.bool(False),
    eventType = cms.untracked.int32(0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFPFJetDistributionsSequence = cms.Sequence(
    VBFPFJetDistributions
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
