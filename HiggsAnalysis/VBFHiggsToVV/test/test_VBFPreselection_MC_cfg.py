import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- General THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration.StandardSequences.Services_cff')
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "START36_V10::All"

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

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.source = cms.Source(
    "PoolSource",
    #debugFlag = cms.untracked.bool(True),
    #debugVebosity = cms.untracked.uint32(1),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
    fileNames = cms.untracked.vstring(
        #'file:/data/NTUPLES/VBF/qqHWW_lnujj/H200/CMSSWfile_10.root',
        'file:/gwtera5/users/data/NTUPLES/VBF/WJets_3_6_X.root'
        #'file:/gwtera5/users/data/NTUPLES/VBF/WJets_3_5_X.root'
        #'file:/tmp/amassiro/0C5ECFC1-C445-DF11-8133-E0CB4E19F99B.root'
        #'/store/data/Commissioning10/MinimumBias/RECO/v9/000/133/863/3C17F2A4-714F-DF11-A722-0025B3E05D50.root'
        #'/store/mc/Spring10/MinBias_TuneD6T_7TeV-pythia6/GEN-SIM-RECO/START3X_V26B-v1/0008/005CDDB7-025D-DF11-8641-00151724CE06.root'
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_1.root',
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_2.root',
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_3.root',
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_4.root',
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_5.root',
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_6.root',
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_7.root',
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_8.root',
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_9.root',
        #'file:/gwtera5/users/data/EDM/PYTHIA6_singleElectron/PYTHIA6_singleElectron_GEN_FASTSIM_10.root'
        )
    )






# --- ======== --- --- --- --- --- --- --- --- --- --- ---
# --- ALL PASS FILTER --- --- --- --- --- --- --- --- --- ---
# --- ======== --- --- --- --- --- --- --- --- --- --- ---

process.load('HiggsAnalysis.VBFHiggsToVV.VBFAllPassFilter_cfi')

process.VBFAllPassFilterBegin = process.VBFAllPassFilter.clone()
process.VBFAllPassFilterGoodVertex = process.VBFAllPassFilter.clone()





# --- ======== --- --- --- --- --- --- --- --- --- --- --- 
# --- GOODCOLL  --- --- --- --- --- --- --- --- --- --- --- 
# --- ======== --- --- --- --- --- --- --- --- --- --- ---

# Technical trigger bit filter
#process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
#process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
#process.L1T1coll = process.hltLevel1GTSeed.clone()
#process.L1T1coll.L1TechTriggerSeeding = cms.bool(True)
#process.L1T1coll.L1SeedsLogicalExpression = cms.string('(40 OR 41)')
#process.l1tcollpath = cms.Sequence(process.L1T1coll)

# Good vertex filter
process.primaryVertexFilter = cms.EDFilter(
    "VertexSelector",
    src = cms.InputTag("offlinePrimaryVertices"),
    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"), # tracksSize() > 3 for the older cut
    filter = cms.bool(True),                                                      # otherwise it won't filter the events, just produce an empty vertex collection.
    )

# No scraping filter
process.noscraping = cms.EDFilter(
    "FilterOutScraping",
    applyfilter = cms.untracked.bool(True),
    debugOn = cms.untracked.bool(False),
    numtrack = cms.untracked.uint32(10),
    thresh = cms.untracked.double(0.25)
    )

process.goodvertex = cms.Sequence(process.primaryVertexFilter*process.noscraping)

#process.goodcoll = cms.Sequence(process.l1tcollpath + process.goodvertex)






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PRESELECTION  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("RecoJets.Configuration.GenJetParticles_cff")
process.load("RecoJets.JetProducers.ak5GenJets_cfi")

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_MC_cff")


# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# ak5CaloJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_ak5CaloJets_MC_cff")
process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_L2L3Ak5CaloJets_MC_cff")



# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# ak5PFJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_ak5PFJets_MC_cff")
process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_L2L3Ak5PFJets_MC_cff")






# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- ---
# --- ====== --- --- --- --- --- --- --- --- --- --- ---
#
#process.load("Configuration.EventContent.EventContent_cff")
#
#process.out = cms.OutputModule(
#    "PoolOutputModule",
#    process.AODSIMEventContent,
#    verbose = cms.untracked.bool(True),
#    fileName = cms.untracked.string('VBFPreselection_MC.root'),
#    )
#
#process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
                               
#process.p1 = cms.Path(
#    process.VBFAllPassFilter*
#    process.goodcoll*
#    process.VBFPreselectionSequenceAk5CaloJets
#    )

process.p2 = cms.Path(
    process.VBFAllPassFilterBegin*
    process.goodvertex*    
    process.VBFAllPassFilterGoodVertex*
    process.genJetParticles*
    process.ak5GenJets*
    process.VBFPreselectionSequenceL2L3Ak5CaloJets
    )


#process.p3 = cms.Path(
#    process.VBFAllPassFilter*
#    process.goodcoll*    
#    process.VBFPreselectionSequenceAk5PFJets
#    )

process.p4 = cms.Path(
    process.VBFAllPassFilterBegin*
    process.goodvertex*
    process.VBFAllPassFilterGoodVertex*    
    process.genJetParticles*
    process.ak5GenJets*
    process.VBFPreselectionSequenceL2L3Ak5PFJets
    )

#process.o = cms.EndPath( process.out )
