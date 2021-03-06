import FWCore.ParameterSet.Config as cms

process = cms.Process("SkimAnalyzer")


# initialize MessageLogger and output report
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)
process.MessageLogger.cerr.threshold = 'INFO'


# source
process.source = cms.Source(
    "PoolSource",
    skipEvents = cms.untracked.uint32(0),                       
    fileNames = cms.untracked.vstring(
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/FEBB8D80-8E45-DF11-944C-00E08178C18D.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/FC99FA82-8E45-DF11-B2B7-00E081791853.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/F800D72C-9145-DF11-8DFA-003048670A06.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/F0BE2343-8C45-DF11-86BD-00E08178C02A.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/F01AF433-8D45-DF11-BAAC-003048635B86.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/EEAE2138-8D45-DF11-88AD-002481E1510C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/EC88A2DF-9145-DF11-B006-0025B31E3D3C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/E02134D0-9145-DF11-B848-003048D476C6.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/D0B2ADA4-8B45-DF11-8F4F-002481E15000.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/BC985DCF-9345-DF11-9FAF-00304866C51E.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/AA45B342-8A45-DF11-B952-00E0817918D3.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/A00FCD08-8046-DF11-85EA-003048670A0C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/9A7B9127-8D45-DF11-BC77-002481E14D64.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/9636AE2C-9145-DF11-B731-002481E1510C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/84886DC2-8D45-DF11-B9DD-003048673FE6.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/7C0F1A7E-9245-DF11-A5D6-003048D479FC.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/5EEEF385-9045-DF11-9ADC-00E0817918C3.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/56C3B805-8945-DF11-A1E2-003048673F1C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/54B205AC-8F45-DF11-B1AD-002481E150EE.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/4ABC065B-8F45-DF11-9C8B-0025B3E0653C.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/3EF0D727-8D45-DF11-A879-00E081791859.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/3CA9ABC9-8D45-DF11-8165-00E081791819.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/3A4E7084-8E45-DF11-9635-0030486361F2.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/38F5689B-D445-DF11-B866-0025B3E06480.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/383CE239-9445-DF11-8ECA-0025B3E05CF8.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/383A957B-8845-DF11-B9B5-002481E14E62.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/34719668-8F45-DF11-9F5C-003048673F54.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/267ABE5D-7146-DF11-A703-002481E14E56.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/18A39F75-EF45-DF11-BC34-0025B3E06468.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/06824BCD-8246-DF11-9AA6-00E08178C129.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/025BBF90-8B45-DF11-9969-003048673FE6.root',
        '/store/data/Commissioning10/MinimumBias/RAW-RECO/v8/000/132/959/0219956C-8F45-DF11-B238-0025B3E05CF8.root'
    )
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)


# Skim analyzer
from Egamma.LLRSkim.skimAnalyzer_cfi import *
process.mySkimAnalyzer = skimAnalyzer.clone()


# Paths
process.p = cms.Path(
    process.mySkimAnalyzer
    )
