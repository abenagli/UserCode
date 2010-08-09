import FWCore.ParameterSet.Config as cms

looseVBFselect = cms.EDFilter("FwdJetsPreselFilter",
  jetTag = cms.InputTag ("ak5CaloJets") ,
  ptThres = cms.double (20.) , # GeV
  nJets = cms.int32 (2) ,
  DetaThres = cms.double (1.) ,
  MjjThres = cms.double (300.) # GeV
)
