import FWCore.ParameterSet.Config as cms



VBFPtMinLeptonCountFilter = cms.EDFilter(
    "VBFPtMinLeptonCountFilter",
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcMuons     = cms.InputTag("muons"),
    ptMin  = cms.double(10.),
    etaMin = cms.double(-2.7),
    etaMax = cms.double(+2.7),
    minNumber = cms.int32(1)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
