import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFIdSelectedCaloJets = cms.EDFilter(
    "VBFCaloJetIdSelection",
    src = cms.InputTag("ak5CaloJets"),
    srcJets_forID = cms.InputTag("ak5CaloJets"),
    srcJetID = cms.InputTag("ak5JetID"),
    EMFMin = cms.double(0.01),
    n90Min = cms.int32(1),
    fHPDMax = cms.double(0.98),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFIdSelectedCaloJetsRef = cms.EDFilter(
    "VBFCaloJetIdSelectionRef",
    src = cms.InputTag("ak5CaloJets"),
    srcJets_forID = cms.InputTag("ak5CaloJets"),
    srcJetID = cms.InputTag("ak5JetID"),
    EMFMin = cms.double(0.01),
    n90Min = cms.int32(1),
    fHPDMax = cms.double(0.98),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFIdSelectedCaloJetsSequence = cms.Sequence(
    VBFIdSelectedCaloJets *
    VBFIdSelectedCaloJetsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
