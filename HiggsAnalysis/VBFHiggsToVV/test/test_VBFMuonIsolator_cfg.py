import FWCore.ParameterSet.Config as cms



process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- GENERAL THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- INPUT  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        #'file:/data/NTUPLES/VBF/qqHWW_lnujj/H200/CMSSWfile_10.root',
        'file:/gwtera5/users/data/NTUPLES/VBF/CMSSWfile_3_1_X.root'
        )
    )






# --- ==================== --- --- --- --- --- --- --- --- --- --- --- 
# --- THE VBF SELECTIONS   --- --- --- --- --- --- --- --- --- --- --- 
# --- ==================== --- --- --- --- --- --- --- --- --- --- --- 

# --- THE MUON ISOLATOR --- --- --- --- --- --- --- --- --- --- --- 

process.load("HiggsAnalysis.VBFHiggsToVV.VBFMuonIsolator_cfi")
process.VBFIsolatedMuons.tkIsoCut    = cms.double(1.)
process.VBFIsolatedMuonsRef.tkIsoCut = cms.double(1.)

process.VBFIsolatedMuons.emIsoCut    = cms.double(1.)
process.VBFIsolatedMuonsRef.emIsoCut = cms.double(1.)

process.VBFIsolatedMuons.hadIsoCut    = cms.double(1.)
process.VBFIsolatedMuonsRef.hadIsoCut = cms.double(1.)

# --- THE MUON DISTRIBUTIONS --- --- --- --- --- --- --- --- --- --- --- 

process.load("HiggsAnalysis.VBFHiggsToVV.VBFMuonDistributions_cfi")
process.VBFMuonDistributions.fileName = cms.untracked.string("VBFMuonIsolatorTEST_muonDistributions.root")
process.VBFMuonDistributions.doRefCheck = cms.bool(True)
process.VBFMuonDistributions.srcMuonsRef = cms.InputTag("VBFIsolatedMuonsRef")





# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.load("Configuration.EventContent.EventContent_cff")

process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    verbose = cms.untracked.bool(True),
    fileName = cms.untracked.string('VBFMuonIsolatorTEST.root'),
    )

process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
                               
process.p = cms.Path(
    process.VBFIsolatedMuonsSequence +
    process.VBFMuonDistributionsSequence
    )

#process.o = cms.EndPath( process.out )
