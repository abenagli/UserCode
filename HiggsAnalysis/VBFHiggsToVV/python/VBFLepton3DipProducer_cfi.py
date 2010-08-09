import FWCore.ParameterSet.Config as cms



VBFLepton3DipProducer = cms.EDProducer(
    "VBFLepton3DipProducer",
    srcVertices  = cms.InputTag("offlinePrimaryVertices"),
    srcElectrons = cms.InputTag("gsfElectrons"),
    srcMuons     = cms.InputTag("muons")
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 






VBFLepton3DipProducerSequence = cms.Sequence(
    VBFLepton3DipProducer
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
