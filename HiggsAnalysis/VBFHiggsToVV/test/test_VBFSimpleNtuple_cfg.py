import FWCore.ParameterSet.Config as cms



process = cms.Process("TEST")

# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 
# --- GENERAL THINGS --- --- --- --- --- --- --- --- --- --- --- --- 
# --- ============== --- --- --- --- --- --- --- --- --- --- --- --- 

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True))

process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'MC_31X_V5::All'

process.load("Configuration.StandardSequences.Geometry_cff")
process.load('Configuration/StandardSequences/Services_cff')
process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.threshold = 'INFO'






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- INPUT  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(1000))

process.source = cms.Source(
    "PoolSource",
    debugFlag = cms.untracked.bool(True),
    debugVebosity = cms.untracked.uint32(1),
    fileNames = cms.untracked.vstring(
        #'file:/tmp/amassiro/3C91077F-31FF-DD11-A251-001A64789D6C.root',
        #'file:/tmp/amassiro/182AF539-30FC-DD11-A0A3-003048326930.root',
        #'file:/tmp/amassiro/VBFHWW2l2nuTest_9.root',
        #'file:/gwtera5/users/data/NTUPLES/VBF/CMSSWfile_3_1_X.root'
        'file:/gwtera5/users/data/NTUPLES/VBF/CMSSWfile_3_5_X.root'
        )
    )





# --- ==================== --- --- --- --- --- --- --- --- --- --- --- 
# --- THE VBF SELECTIONS   --- --- --- --- --- --- --- --- --- --- --- 
# --- ==================== --- --- --- --- --- --- --- --- --- --- --- 

# --- THE LEPTON TIP/LIP PRODUCER --- --- --- --- --- --- --- --- --- --- ---

process.load("HiggsAnalysis.VBFHiggsToVV.VBFLeptonTipLipProducer_cfi")
process.load("HiggsAnalysis.VBFHiggsToVV.VBFLepton3DipProducer_cfi")


# --- SimpleNtple --- --- --- --- --- --- --- --- --- --- --- 

process.load("HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi")

process.VBFSimpleNtpleSequence = cms.Sequence(
 process.SimpleNtple
)






# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- PATHS  --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
                               
process.SimpleNtplePath = cms.Path(
   process.VBFLeptonTipLipProducer +
   process.VBFLepton3DipProducer + 
   process.VBFSimpleNtpleSequence
)



# --- ====== --- --- --- --- --- --- --- --- --- --- --- 
# --- OUTPUT --- --- --- --- --- --- --- --- --- --- --- 
# --- ====== --- --- --- --- --- --- --- --- --- --- --- 

process.load("Configuration.EventContent.EventContent_cff")

process.out = cms.OutputModule(
    "PoolOutputModule",
    process.AODSIMEventContent,
    verbose = cms.untracked.bool(True),
    fileName = cms.untracked.string('./VBFSimpleNtpleTEST.root'),
    )

process.out.outputCommands.extend(cms.untracked.vstring('keep *_*_*_TEST'))

process.TFileService = cms.Service("TFileService", 
    fileName = cms.string("./VBFSimpleNtupleTEST.root"),
    closeFileFast = cms.untracked.bool(True),
    )

process.o = cms.EndPath( process.out )


# --- ======== --- --- --- --- --- --- --- --- --- --- --- 
# --- SCHEDULE --- --- --- --- --- --- --- --- --- --- --- 
# --- ======== --- --- --- --- --- --- --- --- --- --- --- 


process.schedule = cms.Schedule(process.SimpleNtplePath) 



