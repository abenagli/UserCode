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
process.load("RecoTracker.TrackProducer.TrackRefitters_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "GR_R_36X_V12::All"
#process.GlobalTag.globaltag = "GR10_P_V7::All"

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
        'file:/gwtera5/users/data/NTUPLES/VBF/DATA_361p4.root'
        )
    )






# --- ======== --- --- --- --- --- --- --- --- --- --- --- 
# --- ALL PASS FILTER --- --- --- --- --- --- --- --- --- --- 
# --- ======== --- --- --- --- --- --- --- --- --- --- ---

process.load('HiggsAnalysis.VBFHiggsToVV.VBFAllPassFilter_cfi')

process.VBFAllPassFilterBegin = process.VBFAllPassFilter.clone()





## --- ======== --- --- --- --- --- --- --- --- --- --- --- 
## --- GOODCOLL  --- --- --- --- --- --- --- --- --- --- --- 
## --- ======== --- --- --- --- --- --- --- --- --- --- ---

## Technical trigger bit filter
#process.load('L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff')
#process.load('HLTrigger/HLTfilters/hltLevel1GTSeed_cfi')
#process.L1T1coll = process.hltLevel1GTSeed.clone()
#process.L1T1coll.L1TechTriggerSeeding = cms.bool(True)
#process.L1T1coll.L1SeedsLogicalExpression = cms.string('0 AND (40 OR 41) AND NOT (36 OR 37 OR 38 OR 39) AND NOT ((42 AND NOT 43) OR (43 AND NOT 42))')
#process.l1tcollpath = cms.Sequence(process.L1T1coll)

## Good vertex filter
#process.primaryVertexFilter = cms.EDFilter(
#    "VertexSelector",
#    src = cms.InputTag("offlinePrimaryVertices"),
#    cut = cms.string("!isFake && ndof > 4 && abs(z) <= 15 && position.Rho <= 2"), # tracksSize() > 3 for the older cut
#    filter = cms.bool(True),                                                      # otherwise it won't filter the events, just produce an empty vertex collection.
#    )

## No scraping filter
#process.noscraping = cms.EDFilter(
#    "FilterOutScraping",
#    applyfilter = cms.untracked.bool(True),
#    debugOn = cms.untracked.bool(False),
#    numtrack = cms.untracked.uint32(10),
#    thresh = cms.untracked.double(0.25)
#    )
#
#process.goodvertex = cms.Sequence(process.primaryVertexFilter*process.noscraping)

#process.goodcoll = cms.Sequence(process.l1tcollpath + process.goodvertex)






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PRESELECTION  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_cff")



# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# ak5CaloJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_L2L3Ak5CaloJets_cff")



# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# ak5PFJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_L2L3Ak5PFJets_cff")






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
#    fileName = cms.untracked.string('VBFPreselection_EDM.root'),
#    )
#
#process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
                               
process.p1 = cms.Path(
    process.VBFAllPassFilterBegin*
    process.VBFPreselectionSequenceL2L3Ak5CaloJets
    )


process.p2 = cms.Path(
    process.VBFAllPassFilterBegin*
    process.VBFPreselectionSequenceL2L3Ak5PFJets
    )

#process.o = cms.EndPath( process.out )
