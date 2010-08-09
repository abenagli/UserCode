import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- GENERAL THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
#process.GlobalTag.globaltag = "MC_31X_V5::All"
process.GlobalTag.globaltag = "START3X_V26::All"

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
#process.MessageLogger = cms.Service(
#    "MessageLogger",
#    #destinations = cms.untracked.vstring('detailedInfo'),
#    #detailedInfo = cms.untracked.PSet(
#    #FwkReport = cms.untracked.PSet(
#    #    reportEvery = cms.untracked.int32(5000),
#    #    limit = cms.untracked.int32(10000000)
#    #    )
#    #),
#    default = cms.untracked.PSet(
#        FwkReport = cms.untracked.PSet(
#            reportEvery = cms.untracked.int32(1000),
#            limit = cms.untracked.int32(10000000)
#            )
#        )
#    )


process.TFileService = cms.Service(
    "TFileService",
    fileName = cms.string("./VBFPreselection.root"),
    closeFileFast = cms.untracked.bool(True)
    )






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- INPUT  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))

process.source = cms.Source(
    "PoolSource",
#    debugFlag = cms.untracked.bool(True),
#    debugVebosity = cms.untracked.uint32(1),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
        #'file:/data/NTUPLES/VBF/qqHWW_lnujj/H200/CMSSWfile_10.root',
        'file:/gwtera5/users/data/NTUPLES/VBF/CMSSWfile_3_1_X.root'
        )
    )




## --- ====== --- --- --- --- --- --- --- --- --- --- ---
## --- GenJetProduction   --- --- --- --- --- --- --- ---
## --- ====== --- --- --- --- --- --- --- --- --- --- ---


process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.JetProducers.ak5GenJets_cfi")
#-----------------------------

process.p0 = cms.Path(process.genParticlesForJets * process.ak5GenJets )





# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PRESELECTION  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_cff")
process.MCDecayModeFilterAll.eventType = cms.untracked.int32(0)


# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# sisCone5CaloJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

#process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_sisCone5CaloJets_cff")
#process.VBFNtupleSisCone5CaloJets.eventType  = cms.untracked.int32(0)
#process.VBFNtupleSisCone5CaloJets.saveGenJet = cms.untracked.bool(True)
#process.VBFNtupleSisCone5CaloJets.saveGenMet = cms.untracked.bool(True)

#process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_L2L3SisCone5CaloJets_cff")
#process.VBFNtupleL2L3SisCone5CaloJets.eventType  = cms.untracked.int32(0)
#process.VBFNtupleL2L3SisCone5CaloJets.saveGenJet = cms.untracked.bool(True)
#process.VBFNtupleL2L3SisCone5CaloJets.saveGenMet = cms.untracked.bool(True)


# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# antikt5CaloJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_antikt5CaloJets_cff")
process.VBFNtupleAntikt5CaloJets.eventType  = cms.untracked.int32(0)
process.VBFNtupleAntikt5CaloJets.saveGenJet = cms.untracked.bool(True)
process.VBFNtupleAntikt5CaloJets.saveGenMet = cms.untracked.bool(True)

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_L2L3Antikt5CaloJets_cff")
process.VBFNtupleL2L3Antikt5CaloJets.eventType  = cms.untracked.int32(0)
process.VBFNtupleL2L3Antikt5CaloJets.saveGenJet = cms.untracked.bool(True)
process.VBFNtupleL2L3Antikt5CaloJets.saveGenMet = cms.untracked.bool(True)


# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# antikt5PFJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_antikt5PFJets_cff")
process.VBFNtupleAntikt5PFJets.eventType  = cms.untracked.int32(0)
process.VBFNtupleAntikt5PFJets.saveGenJet = cms.untracked.bool(True)
process.VBFNtupleAntikt5PFJets.saveGenMet = cms.untracked.bool(True)

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_L2L3Antikt5PFJets_cff")
process.VBFNtupleL2L3Antikt5PFJets.eventType  = cms.untracked.int32(0)
process.VBFNtupleL2L3Antikt5PFJets.saveGenJet = cms.untracked.bool(True)
process.VBFNtupleL2L3Antikt5PFJets.saveGenMet = cms.untracked.bool(True)






# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- ---
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("Configuration.EventContent.EventContent_cff")

process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    verbose = cms.untracked.bool(True),
    fileName = cms.untracked.string('VBFPreselection_signalTEST.root'),
    )

process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
                               
#process.p1 = cms.Path(
#    process.VBFPreselectionSequenceSisCone5CaloJets
#    )

#process.p2 = cms.Path(
#    process.VBFPreselectionSequenceL2L3SisCone5CaloJets
#    )

process.p3 = cms.Path(
    process.VBFPreselectionSequenceAntikt5CaloJets
    )

process.p4 = cms.Path(
    process.VBFPreselectionSequenceL2L3Antikt5CaloJets
    )

process.p5 = cms.Path(
    process.VBFPreselectionSequenceAntikt5PFJets
    )

process.p6 = cms.Path(
    process.VBFPreselectionSequenceL2L3Antikt5PFJets
    )

#process.o = cms.EndPath( process.out )
