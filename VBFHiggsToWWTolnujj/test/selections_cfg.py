import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.selections = cms.PSet(
    
    printModulo  = cms.int32 (10000),
    jetAlgorithm = cms.string("sisCone5CaloJets")
)






process.inputNtuples = cms.PSet(
    maxEvents = cms.int32(-1),
    inputFiles = cms.vstring(
#         '/afs/cern.ch/user/a/abenagli/scratch0/VBF/temp/CMSSW_2_2_10/src/HiggsAnalysis/VBFHiggsToWWTolnujj/test/VBFNtuple_sisCone5CaloJets.root'
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_1.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_2.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_3.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_4.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_5.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_6.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_7.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_8.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_9.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_10.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_11.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_12.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_13.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_14.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_15.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_16.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_17.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_18.root',
        '/data/NTUPLES/VBF/qqHWW_lnujj/H200/VBFNtuple_sisCone5CaloJets_19.root'
    )
)

