import FWCore.ParameterSet.Config as cms

process = cms.Process("EventIdFilter")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = 'INFO'


# source
process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/133/472/E6EF557C-D64A-DF11-8503-00E08178C163.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)


# output
process.out = cms.OutputModule(
    "PoolOutputModule",
    verbose = cms.untracked.bool(True),
    outputCommands = cms.untracked.vstring('keep *_*_*_*'),
    fileName = cms.untracked.string('run1333472_ev4147404.root')
    )


# Event id filter
from Egamma.LLRSkim.eventIdFilter_cfi import *
process.myEventIdFilter = eventIdFilter.clone()
process.myEventIdFilter.runId = cms.int32(133472)
process.myEventIdFilter.eventId = cms.int32(4147404)


# Paths
process.p = cms.Path(
    process.myEventIdFilter *
    process.out
    )
