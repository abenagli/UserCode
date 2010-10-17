import FWCore.ParameterSet.Config as cms

from PhysicsTools.PatAlgos.tools.metTools import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.tools.coreTools import *
from PhysicsTools.PatAlgos.tools.pfTools import *



process = cms.Process("EventIdFilter")

# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1)
process.MessageLogger.cerr.threshold = 'INFO'
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR10_P_V9::All"




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




# Pat sequences
process.load("PhysicsTools.PatAlgos.patSequences_cff")
process.load("PhysicsTools.PatAlgos.tools.pfTools")
postfix = "PFlow"
usePF2PAT(process, runPF2PAT=True, jetAlgo='AK5', runOnMC=False, postfix=postfix)
removeMCMatching(process, ['All'])
process.patJets.addTagInfos = cms.bool(False)




# Paths
process.p = cms.Path(process.myEventIdFilter *
                     getattr(process,"patPF2PATSequence"+postfix)
                     )

process.out.outputCommands = cms.untracked.vstring( 'keep *' )
process.o = cms.EndPath(process.out)
