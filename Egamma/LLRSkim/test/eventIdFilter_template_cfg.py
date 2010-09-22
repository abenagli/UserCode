import FWCore.ParameterSet.Config as cms

process = cms.Process("EventIdFilter")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = 'INFO'


# jet corrector
process.load("JetMETCorrections.Configuration.DefaultJEC_cff")
process.L2L3Ak5CaloJets = process.ak5CaloJetsL2L3.clone()
process.L2L3Ak5PFJets = process.ak5PFJetsL2L3.clone()


# source
process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(LISTOFFILES)
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)



# output
process.out = cms.OutputModule(
    "PoolOutputModule",
    verbose = cms.untracked.bool(True),
    outputCommands = cms.untracked.vstring('keep *_*_*_*'),
    fileName = cms.untracked.string('JOBDIR/runRUNID_evtEVTID.root'),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('p')
        )
    )


# Event id filter
from Egamma.LLRSkim.eventIdFilter_cfi import *
process.myEventIdFilter = eventIdFilter.clone()
process.myEventIdFilter.runId   = cms.int32(RUNID)
process.myEventIdFilter.eventId = cms.int32(EVTID)


# Paths
process.p = cms.Path(process.L2L3Ak5CaloJets +
                     process.L2L3Ak5PFJets +
                     process.myEventIdFilter)

process.o = cms.EndPath(process.out)
