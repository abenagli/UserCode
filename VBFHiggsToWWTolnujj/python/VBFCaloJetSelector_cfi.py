import FWCore.ParameterSet.Config as cms

VBFSelectedCaloJets = cms.EDFilter(
    "VBFCaloJetSelection",
    src = cms.InputTag("sisCone5CaloJets"),
    etMin  = cms.double(10.),
    etaMin = cms.double(-5.),
    etaMax = cms.double(+5.),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jetsRef")
    )



VBFSelectedCaloJetsRef = cms.EDFilter(
    "VBFCaloJetSelectionRef",
    src = cms.InputTag("sisCone5CaloJets"),
    etMin  = cms.double(10.),
    etaMin = cms.double(-5.),
    etaMax = cms.double(+5.),
    doRefCheck = cms.bool(False),
    srcJetsRef = cms.InputTag("jetsRef")
    )
