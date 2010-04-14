import FWCore.ParameterSet.Config as cms

process = cms.Process("EventIdFilter")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(100)
process.MessageLogger.cerr.threshold = 'INFO'


# source
process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v7/000/132/440/FCCA95EA-4A3C-DF11-B108-00E0817918BF.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
)


# output
process.out = cms.OutputModule(
    "PoolOutputModule",
    process.FEVTEventContent,
    verbose = cms.untracked.bool(True),
    fileName = cms.untracked.string('Run132440_ev1.root'),
    )

process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))


# Event id filter
from CommonTools.EventIdFilter.eventIdFilter_cfi import *
process.myEventIdFilter = eventIdFilter.clone()
process.myEventIdFilter.runId = cms.int32(132440)
process.myEventIdFilter.eventId = cms.int32(1)


# Paths
process.p = cms.Path(
    process.myEventIdFilter
    )

process.o = cms.OutPath(
    process.out
    )
