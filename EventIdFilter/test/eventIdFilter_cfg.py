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
        'file:/gwtera5/users/data/RECO/B647CAD9-0E3C-DF11-886F-0030487CD716.root'
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
    fileName = cms.untracked.string('run132440_ev3950385.root')
    )


# Event id filter
from CommonTools.EventIdFilter.eventIdFilter_cfi import *
process.myEventIdFilter = eventIdFilter.clone()
process.myEventIdFilter.runId = cms.int32(132440)
process.myEventIdFilter.eventId = cms.int32(3950385)


# Paths
process.p = cms.Path(
    process.myEventIdFilter *
    process.out
    )
