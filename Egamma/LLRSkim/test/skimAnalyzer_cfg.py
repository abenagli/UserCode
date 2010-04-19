import FWCore.ParameterSet.Config as cms

process = cms.Process("SkimAnalyzer")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.threshold = 'INFO'


# source
process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/472/E6EF557C-D64A-DF11-8503-00E08178C163.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/472/08D8AF7D-D04A-DF11-9A0C-003048D47A40.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/472/FC8A9960-D54A-DF11-A2A8-002481E15104.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


# Skim analyzer
from Egamma.LLRSkim.skimAnalyzer_cfi import *
process.mySkimAnalyzer = skimAnalyzer.clone()


# Paths
process.p = cms.Path(
    process.mySkimAnalyzer
    )
