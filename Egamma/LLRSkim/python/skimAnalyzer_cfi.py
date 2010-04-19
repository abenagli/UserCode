import FWCore.ParameterSet.Config as cms

skimAnalyzer = cms.EDFilter(
    "SkimAnalyzer",
    electronCollection = cms.InputTag("gsfElectrons"),
    fbremMIN = cms.double(0.),
    hcalOverEcalMAX = cms.double(0.15),
    deltaPhiSuperClusterTrackAtVtxMAX = cms.double(0.10),
    deltaEtaSuperClusterTrackAtVtxMAX = cms.double(0.02),
    sigmaIetaIetaBarrelMIN = cms.double(0.008),
    sigmaIetaIetaEndcapMIN = cms.double(0.02),
    ele_ptLow = cms.double(5.),
    ele_ptHigh = cms.double(15.),
    nEle_ptLowMIN = cms.int32(1),
    nEle_ptHighMIN = cms.int32(1)
)

