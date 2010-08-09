import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFIdSelectedPFJets = cms.EDFilter(
    "VBFPFJetIdSelection",
    src = cms.InputTag("ak5PFJets"),
    CHFMin = cms.double(0.),
    NHFMax = cms.double(1.),
    CEFMax = cms.double(1.),
    NEFMax = cms.double(1.),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFIdSelectedPFJetsRef = cms.EDFilter(
    "VBFPFJetIdSelectionRef",
    src = cms.InputTag("ak5PFJets"),
    CHFMin = cms.double(0.),
    NHFMax = cms.double(1.),
    CEFMax = cms.double(1.),
    NEFMax = cms.double(1.),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFIdSelectedPFJetsSequence = cms.Sequence(
    VBFIdSelectedPFJets *
    VBFIdSelectedPFJetsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
