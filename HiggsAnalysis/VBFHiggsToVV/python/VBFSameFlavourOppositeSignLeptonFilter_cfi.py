import FWCore.ParameterSet.Config as cms



VBFSameFlavourOppositeSignLeptonFilter = cms.EDFilter(
    "VBFSameFlavourOppositeSignLeptonFilter",
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcMuons     = cms.InputTag("muons"),
    mMin  = cms.double(12.)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
