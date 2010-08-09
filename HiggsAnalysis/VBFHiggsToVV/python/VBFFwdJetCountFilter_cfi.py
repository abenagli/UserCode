import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFFwdCaloJetCountFilter = cms.EDFilter(
    "VBFFwdCaloJetCountFilter",
    srcJets = cms.InputTag("ak5CaloJets"),
    etMin  = cms.double(15.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    DetaMin = cms.double(2.0),
    mJJMin = cms.double(250.0),
    minNumber = cms.int32(1)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 






# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

VBFFwdPFJetCountFilter = cms.EDFilter(
    "VBFFwdPFJetCountFilter",
    srcJets = cms.InputTag("ak5PFJets"),
    etMin  = cms.double(15.),
    etaMin = cms.double(-5.0),
    etaMax = cms.double(+5.0),
    DetaMin = cms.double(2.0),
    mJJMin = cms.double(250.0),
    minNumber = cms.int32(1)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
