import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFSelectedCaloJets = cms.EDFilter(
    "VBFCaloJetSelection",
    src = cms.InputTag("ak5CaloJets"),
    etMin  = cms.double(10.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSelectedCaloJetsRef = cms.EDFilter(
    "VBFCaloJetSelectionRef",
    src = cms.InputTag("akCaloJets"),
    etMin  = cms.double(10.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFSelectedCaloJetsSequence = cms.Sequence(
    VBFSelectedCaloJets *
    VBFSelectedCaloJetsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----






# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFSelectedPFJets = cms.EDFilter(
    "VBFPFJetSelection",
    src = cms.InputTag("ak5PFJets"),
    etMin  = cms.double(10.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



VBFSelectedPFJetsRef = cms.EDFilter(
    "VBFPFJetSelectionRef",
    src = cms.InputTag("ak5PFJets"),
    etMin  = cms.double(10.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jets")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



VBFSelectedPFJetsSequence = cms.Sequence(
    VBFSelectedPFJets *
    VBFSelectedPFJetsRef
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
