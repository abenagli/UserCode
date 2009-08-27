#!/usr/bin/env cmsRun
import FWCore.ParameterSet.Config as cms

process = cms.Process("VBFPreselection")

process.load("SimGeneral.HepPDTESSource.pythiapdt_cfi")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'DEBUG'






# Source
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        #'rfio:/castor/cern.ch/user/a/amassiro/VBF/MySample/qqHWW_lnujj/H200/CMSSWfile_10.root'
        'file:/tmp/abenagli/CMSSWfile_10.root'
       	)
    )






###################################
## Electron ambiguity resolving  ##
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFElectronAmbiguityResolver_cfi")

process.VBFAmbiguityResolvedElectronsSequence = cms.Sequence(
    process.VBFAmbiguityResolvedElectrons *
    process.VBFAmbiguityResolvedElectronsRef 
    )

#######################
## Electron selector ##
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFElectronSelector_cfi")

process.VBFSelectedElectronsSequence = cms.Sequence(
    process.VBFSelectedElectrons *
    process.VBFSelectedElectronsRef 
    )

#######################
## Electron isolator ##
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFElectronIsolator_cfi")

process.VBFIsolatedElectronsSequence = cms.Sequence(
    process.VBFIsolatedElectrons *
    process.VBFIsolatedElectronsRef 
    )

#######################
## Electron isolator ##
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFElectronIdSelector_cfi")

process.VBFIdSelectedElectronsSequence = cms.Sequence(
    process.VBFIdSelectedElectrons *
    process.VBFIdSelectedElectronsRef 
    )






###################
## Muon selector ##
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFMuonSelector_cfi")

process.VBFSelectedMuonsSequence = cms.Sequence(
    process.VBFSelectedMuons *
    process.VBFSelectedMuonsRef 
    )

###################
## Muon isolator ##
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFMuonIsolator_cfi")

process.VBFIsolatedMuonsSequence = cms.Sequence(
    process.VBFIsolatedMuons *
    process.VBFIsolatedMuonsRef 
    )





##################
## Jet selector ##
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFCaloJetSelector_cfi")

process.VBFSelectedCaloJetsSequence = cms.Sequence(
    process.VBFSelectedCaloJets *
    process.VBFSelectedCaloJetsRef 
    )






#############################
## Lepton/jet count filter ##
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFPtMinLeptonCountFilter_cfi")
process.load("HiggsAnalysis.VBFHiggsToWWTolnujj.VBFEtMinCaloJetCountFilter_cfi")

process.VBFLeptonJetCountFilterSequence = cms.Sequence(
    process.VBFPtMinLeptonCountFilter *
    process.VBFEtMinCaloJetCountFilter
    )






##################
## CMS sequence ##

process.VBFPreselectionSequence = cms.Sequence(
    process.VBFAmbiguityResolvedElectronsSequence *  # electron ambiguity resolver
    process.VBFSelectedElectronsSequence *           # electron selector
    process.VBFIsolatedElectronsSequence *           # electron isolator
    process.VBFIdSelectedElectronsSequence *         # electron id selector
    process.VBFSelectedMuonsSequence *               # muon selector
    process.VBFIsolatedMuonsSequence *               # muon isolator
    process.VBFSelectedCaloJetsSequence *            # jet selector
    process.VBFLeptonJetCountFilterSequence          # count leptons / jets
    )

process.p0 = cms.Path(process.VBFPreselectionSequence)






process.schedule = cms.Schedule(process.p0)
