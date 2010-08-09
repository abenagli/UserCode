import FWCore.ParameterSet.Config as cms


process = cms.Process("TEST")



runOn35X = True
runOn36X = False
runOn37X = False



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
if runOn35X:
    process.GlobalTag.globaltag = "START3X_V25::All"
if runOn36X:
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
        #'file:/gwtera5/users/data/NTUPLES/VBF/CMSSWfile_3_1_X.root'
        #'file:/tmp/amassiro/0C5ECFC1-C445-DF11-8133-E0CB4E19F99B.root'
        '/store/data/Run2010A/EG/RECO/May27thReReco_v1/0000/129A8471-A469-DF11-A727-003048D43656.root'
        )
    )






# --- ======== --- --- --- --- --- --- --- --- --- --- --- 
# --- AK5GENJETS --- --- --- --- --- --- --- --- --- --- 
# --- ======== --- --- --- --- --- --- --- --- --- --- ---

if runOn35X:
    process.load("RecoJets.Configuration.GenJetParticles_cff")
    process.load("RecoJets.JetProducers.ak5GenJets_cfi")






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
VBFNtupleL2L3Ak5CaloJets.saveGenJet      = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveGenMet      = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveMC          = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveMCEle       = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveMCMu        = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveMCPtHat     = cms.untracked.bool(True)

if runOn35X:
    VBFNtupleL2L3Ak5CaloJets.HLTTag = cms.InputTag("TriggerResults::REDIGI")
if runOn36X:
    VBFNtupleL2L3Ak5CaloJets.HLTTag = cms.InputTag("TriggerResults::REDIGI36X")



# --- ====== --- --- --- --- --- --- --- --- --- --- ---
# ak5PFJets
# --- ====== --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFPreselection_L2L3Ak5PFJets_cff")
VBFNtupleL2L3Ak5PFJets.saveGenJet      = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveGenMet      = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveMC          = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveMCEle       = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveMCMu        = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveMCPtHat     = cms.untracked.bool(True)

if runOn35X:
    VBFNtupleL2L3Ak5PFJets.HLTTag = cms.InputTag("TriggerResults::REDIGI")
if runOn36X:
    VBFNtupleL2L3Ak5PFJets.HLTTag = cms.InputTag("TriggerResults::REDIGI36X")





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
#    fileName = cms.untracked.string('VBFPreselection_DATA.root'),
#    )
#
#process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
                               
if runOn35X:
    process.p1 = cms.Path(
        process.VBFAllPassFilterBegin*
        process.genJetParticles*
        process.ak5GenJets*
        process.VBFPreselectionSequenceL2L3Ak5CaloJets
        )

    process.p2 = cms.Path(
        process.VBFAllPassFilterBegin*
        process.genJetParticles*
        process.ak5GenJets*
        process.VBFPreselectionSequenceL2L3Ak5PFJets
        )

else :
    process.p1 = cms.Path(
        process.VBFAllPassFilterBegin*
        process.VBFPreselectionSequenceL2L3Ak5CaloJets
        )

    process.p2 = cms.Path(
        process.VBFAllPassFilterBegin*
        process.VBFPreselectionSequenceL2L3Ak5PFJets
        )

#process.o = cms.EndPath( process.out )
