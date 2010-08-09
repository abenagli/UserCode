import FWCore.ParameterSet.Config as cms



minLepSelector = cms.EDFilter(
    "LepMinNumFilter",
    electronsTag = cms.InputTag("gsfElectrons"),
    muonsTag = cms.InputTag ("muons"),
    ptThres = cms.double(9.), # GeV
    nLep = cms.int32(2)
    )
