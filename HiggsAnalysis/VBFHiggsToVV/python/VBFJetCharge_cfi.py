import FWCore.ParameterSet.Config as cms



VBFCaloJetsCharge = cms.EDProducer(
    "JetChargeProducer",
    var = cms.string('Pt'),
    src = cms.InputTag("ak5JetTracksAssociatorAtVertex"),
    exp = cms.double(1.0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----




VBFPFJetsCharge = cms.EDProducer(
    "JetChargeProducer",
    var = cms.string('Pt'),
    src = cms.InputTag("ak5JetTracksAssociatorAtVertex"),
    exp = cms.double(1.0)
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
