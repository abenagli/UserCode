import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFEtMinCaloJetCountFilter = cms.EDFilter(
    "VBFEtMinCaloJetCountFilter",
    srcJets = cms.InputTag("ak5CaloJets"),
    etMin  = cms.double(10.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    minNumber = cms.int32(0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 






# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFEtMinPFJetCountFilter = cms.EDFilter(
    "VBFEtMinPFJetCountFilter",
    srcJets = cms.InputTag("ak5PFJets"),
    etMin  = cms.double(10.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    minNumber = cms.int32(0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
