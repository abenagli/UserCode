import FWCore.ParameterSet.Config as cms

process = cms.Process("EventIdFilter")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.threshold = 'INFO'


# source
process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


# Output
process.out = cms.OutputModule(
    "PoolOutputModule",
    verbose = cms.untracked.bool(True),
    outputCommands = cms.untracked.vstring('keep *_*_*_*'),
    fileName = cms.untracked.string('runRUNID_evEVID.root')
    )


# Event id filter
from CommonTools.EventIdFilter.eventIdFilter_cfi import *
process.myEventIdFilter = eventIdFilter.clone()
process.myEventIdFilter.runId = cms.int32(RUNID)
process.myEventIdFilter.eventId = cms.int32(EVID)


# Paths
process.p = cms.Path(
    process.myEventIdFilter *
    process.out
    )
