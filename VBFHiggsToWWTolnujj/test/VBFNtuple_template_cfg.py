#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFNtupleCaloJets")

process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFNtupleCaloJets_cfi")
process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")

process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'WARNING'






# Source
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        FILENAMES
       	)
    )






###################################
## Electron ambiguity resolving  ##

process.ambiguityResolvedElectrons = cms.EDFilter(
    "VBFElectronAmbiguityResolution",
    src = cms.InputTag("pixelMatchGsfElectrons"),
    )

process.ambiguityResolvedElectronsSequence = cms.Sequence(
    process.ambiguityResolvedElectrons 
    )


###################
## Create NTuple ##

process.VBFNtupleCaloJets.verbosity = True
process.VBFNtupleCaloJets.eventsToPrint = 0
process.VBFNtupleCaloJets.eventType = 0     # 0 ---> signal,    1 ---> background

process.VBFNtupleCaloJets.srcElectrons = cms.InputTag("ambiguityResolvedElectrons")
process.VBFNtupleCaloJets.srcRecoJets = cms.InputTag("JETALGORITHM")

process.VBFNtupleCaloJets.fileName = "VBFNtuple_JETALGORITHM_JOBIT"




##################
## CMS sequence ##

process.VBFNtupleSequence = cms.Sequence(
    process.ambiguityResolvedElectronsSequence *  # electron ambiguity resolver
    process.VBFNtupleCaloJets                    # make the ntuple
    )

process.p0 = cms.Path(process.VBFNtupleSequence)






process.schedule = cms.Schedule(process.p0)
