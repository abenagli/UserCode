import FWCore.ParameterSet.Config as cms



VBFLeptonTipLipProducer = cms.EDProducer(
    "VBFLeptonTipLipProducer",
    srcVertices  = cms.InputTag("offlinePrimaryVertices"),
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcMuons     = cms.InputTag("muons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 






VBFLeptonTipLipProducerSequence = cms.Sequence(
    VBFLeptonTipLipProducer
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
