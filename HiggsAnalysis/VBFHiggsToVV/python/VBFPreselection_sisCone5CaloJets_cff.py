#import FWCore.ParameterSet.Config as cms



## ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
## sisCone5CaloJets
## ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



## --- ==================== --- --- --- --- --- --- --- --- --- --- ---
## --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
## --- ==================== --- --- --- --- --- --- --- --- --- --- ---

## --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_cff import *






## --- THE JET/MET CORRECTOR --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *



## --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
#VBFCleanedCaloJetsSisCone5CaloJets = VBFCleanedCaloJets.clone()
#VBFCleanedCaloJetsRefSisCone5CaloJets = VBFCleanedCaloJetsRef.clone()

#VBFCleanedCaloJetsSisCone5CaloJets.src = cms.InputTag("sisCone5CaloJets")
#VBFCleanedCaloJetsSisCone5CaloJets.doElectronRefCheck = cms.bool(True)
#VBFCleanedCaloJetsSisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

#VBFCleanedCaloJetsRefSisCone5CaloJets.src = cms.InputTag("sisCone5CaloJets")
#VBFCleanedCaloJetsRefSisCone5CaloJets.doElectronRefCheck = cms.bool(True)
#VBFCleanedCaloJetsRefSisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


## --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
#VBFSelectedCaloJetsSisCone5CaloJets = VBFSelectedCaloJets.clone()
#VBFSelectedCaloJetsRefSisCone5CaloJets = VBFSelectedCaloJetsRef.clone()

#VBFSelectedCaloJetsSisCone5CaloJets.src = cms.InputTag("sisCone5CaloJets")
#VBFSelectedCaloJetsSisCone5CaloJets.etMin = cms.double(10.)
#VBFSelectedCaloJetsSisCone5CaloJets.etaMin = cms.double(-5.0)
#VBFSelectedCaloJetsSisCone5CaloJets.etaMax = cms.double(+5.0)
#VBFSelectedCaloJetsSisCone5CaloJets.doRefCheck = cms.bool(True)
#VBFSelectedCaloJetsSisCone5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefSisCone5CaloJets")

#VBFSelectedCaloJetsRefSisCone5CaloJets.src = cms.InputTag("sisCone5CaloJets")
#VBFSelectedCaloJetsRefSisCone5CaloJets.etMin = cms.double(10.)
#VBFSelectedCaloJetsRefSisCone5CaloJets.etaMin = cms.double(-5.0)
#VBFSelectedCaloJetsSisCone5CaloJets.etaMax = cms.double(+5.0)
#VBFSelectedCaloJetsRefSisCone5CaloJets.doRefCheck = cms.bool(True)
#VBFSelectedCaloJetsRefSisCone5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefSisCone5CaloJets")






## --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
#VBFEtMinCaloJetCountFilterSisCone5CaloJets = VBFEtMinCaloJetCountFilter.clone()

#VBFEtMinCaloJetCountFilterSisCone5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsSisCone5CaloJets")
#VBFEtMinCaloJetCountFilterSisCone5CaloJets.etMin = cms.double(10.)
#VBFEtMinCaloJetCountFilterSisCone5CaloJets.etaMin = cms.double(-5.0)
#VBFEtMinCaloJetCountFilterSisCone5CaloJets.etaMax = cms.double(+5.0)
#VBFEtMinCaloJetCountFilterSisCone5CaloJets.minNumber = cms.int32(2)


## --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

##from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
##VBFFwdCaloJetCountFilterSisCone5CaloJets = VBFFwdCaloJetCountFilter.clone()

##VBFFwdCaloJetCountFilterSisCone5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsSisCone5CaloJets")






## --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
#VBFNtupleSisCone5CaloJets = SimpleNtple.clone()

#VBFNtupleSisCone5CaloJets.EleTag = cms.InputTag("gsfElectrons")
#VBFNtupleSisCone5CaloJets.doEleRefCheck = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
#VBFNtupleSisCone5CaloJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
#VBFNtupleSisCone5CaloJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
#VBFNtupleSisCone5CaloJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

#VBFNtupleSisCone5CaloJets.MuTag = cms.InputTag("muons")
#VBFNtupleSisCone5CaloJets.doMuRefCheck = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
#VBFNtupleSisCone5CaloJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
#VBFNtupleSisCone5CaloJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
#VBFNtupleSisCone5CaloJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

#VBFNtupleSisCone5CaloJets.JetTag = cms.InputTag("sisCone5CaloJets")
#VBFNtupleSisCone5CaloJets.doJetRefCheck = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.JetRefTag = cms.InputTag("VBFSelectedCaloJetsRefSisCone5CaloJets")
#VBFNtupleSisCone5CaloJets.genJetTag = cms.InputTag("sisCone5GenJets")

#VBFNtupleSisCone5CaloJets.Type1MetTag = cms.InputTag("VBFType1SisCone5CaloMet")

#VBFNtupleSisCone5CaloJets.saveMu          = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.saveTrack       = cms.untracked.bool(False)
#VBFNtupleSisCone5CaloJets.saveEle         = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.saveJet         = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.savePFJet       = cms.untracked.bool(False)
#VBFNtupleSisCone5CaloJets.saveJetBTagging = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.saveMet         = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.saveGenJet      = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.saveGenMet      = cms.untracked.bool(True)
#VBFNtupleSisCone5CaloJets.saveMC          = cms.untracked.bool(True)






#VBFPreselectionSequenceSisCone5CaloJets = cms.Sequence(
    
    #VBFPreselectionSequenceAll +
    
    
    
    #VBFType1SisCone5CaloMetSequence +



    #VBFCleanedCaloJetsSisCone5CaloJets +
    #VBFCleanedCaloJetsRefSisCone5CaloJets +
    
    #VBFSelectedCaloJetsSisCone5CaloJets +
    #VBFSelectedCaloJetsRefSisCone5CaloJets +
    
    
    
    #VBFEtMinCaloJetCountFilterSisCone5CaloJets +
    ##VBFFwdCaloJetCountFilterSisCone5CaloJets +
    
    
    
    #VBFNtupleSisCone5CaloJets
    #)

## ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
