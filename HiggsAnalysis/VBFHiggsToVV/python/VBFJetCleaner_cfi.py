import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFCleanedCaloJets = cms.EDFilter(
    "VBFCaloJetCleaning",
    src = cms.InputTag("ak5CaloJets"),
    srcElectronsRef = cms.InputTag("gsfElectrons"),
    srcMuonsRef = cms.InputTag("muons"),
    maxDeltaR = cms.double(0.3),
    doJetRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("ak5CaloJets"),
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFCleanedCaloJetsRef = cms.EDFilter(
    "VBFCaloJetCleaningRef",
    src = cms.InputTag("ak5CaloJets"),
    srcElectronsRef = cms.InputTag("gsfElectrons"),
    srcMuonsRef = cms.InputTag("muons"),
    maxDeltaR = cms.double(0.3),
    doJetRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("ak5CaloJets"),
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFCleanedCaloJetsSequence = cms.Sequence(
    VBFCleanedCaloJets *
    VBFCleanedCaloJetsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----






# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFCleanedPFJets = cms.EDFilter(
    "VBFPFJetCleaning",
    src = cms.InputTag("ak5PFJets"),
    srcElectronsRef = cms.InputTag("gsfElectrons"),
    srcMuonsRef = cms.InputTag("muons"),
    maxDeltaR = cms.double(0.3),
    doJetRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("ak5PFJets"),
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFCleanedPFJetsRef = cms.EDFilter(
    "VBFPFJetCleaningRef",
    src = cms.InputTag("ak5PFJets"),
    srcElectronsRef = cms.InputTag("gsfElectrons"),
    srcMuonsRef = cms.InputTag("muons"),
    maxDeltaR = cms.double(0.3),
    doJetRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("ak5PFJets"),
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFCleanedPFJetsSequence = cms.Sequence(
    VBFCleanedPFJets *
    VBFCleanedPFJetsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
