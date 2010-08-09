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

#from HiggsAnalysis.VBFHiggsToVV.VBFJetCorrector_cff import *
#from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *


## --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
#VBFCleanedCaloJetsL2L3SisCone5CaloJets = VBFCleanedCaloJets.clone()
#VBFCleanedCaloJetsRefL2L3SisCone5CaloJets = VBFCleanedCaloJetsRef.clone()

#VBFCleanedCaloJetsL2L3SisCone5CaloJets.src = cms.InputTag("VBFL2L3SisCone5CaloJets")
#VBFCleanedCaloJetsL2L3SisCone5CaloJets.doElectronRefCheck = cms.bool(True)
#VBFCleanedCaloJetsL2L3SisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

#VBFCleanedCaloJetsRefL2L3SisCone5CaloJets.src = cms.InputTag("VBFL2L3SisCone5CaloJets")
#VBFCleanedCaloJetsRefL2L3SisCone5CaloJets.doElectronRefCheck = cms.bool(True)
#VBFCleanedCaloJetsRefL2L3SisCone5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


## --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
#VBFSelectedCaloJetsL2L3SisCone5CaloJets = VBFSelectedCaloJets.clone()
#VBFSelectedCaloJetsRefL2L3SisCone5CaloJets = VBFSelectedCaloJetsRef.clone()

#VBFSelectedCaloJetsL2L3SisCone5CaloJets.src = cms.InputTag("VBFL2L3SisCone5CaloJets")
#VBFSelectedCaloJetsL2L3SisCone5CaloJets.etMin = cms.double(15.)
#VBFSelectedCaloJetsL2L3SisCone5CaloJets.etaMin = cms.double(-5.0)
#VBFSelectedCaloJetsL2L3SisCone5CaloJets.etaMax = cms.double(+5.0)
#VBFSelectedCaloJetsL2L3SisCone5CaloJets.doRefCheck = cms.bool(True)
#VBFSelectedCaloJetsL2L3SisCone5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefL2L3SisCone5CaloJets")

#VBFSelectedCaloJetsRefL2L3SisCone5CaloJets.src = cms.InputTag("VBFL2L3SisCone5CaloJets")
#VBFSelectedCaloJetsRefL2L3SisCone5CaloJets.etMin = cms.double(15.)
#VBFSelectedCaloJetsRefL2L3SisCone5CaloJets.etaMin = cms.double(-5.0)
#VBFSelectedCaloJetsL2L3SisCone5CaloJets.etaMax = cms.double(+5.0)
#VBFSelectedCaloJetsRefL2L3SisCone5CaloJets.doRefCheck = cms.bool(True)
#VBFSelectedCaloJetsRefL2L3SisCone5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefL2L3SisCone5CaloJets")






## --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
#VBFEtMinCaloJetCountFilterL2L3SisCone5CaloJets = VBFEtMinCaloJetCountFilter.clone()

#VBFEtMinCaloJetCountFilterL2L3SisCone5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsL2L3SisCone5CaloJets")
#VBFEtMinCaloJetCountFilterL2L3SisCone5CaloJets.etMin = cms.double(15.)
#VBFEtMinCaloJetCountFilterL2L3SisCone5CaloJets.etaMin = cms.double(-5.0)
#VBFEtMinCaloJetCountFilterL2L3SisCone5CaloJets.etaMax = cms.double(+5.0)
#VBFEtMinCaloJetCountFilterL2L3SisCone5CaloJets.minNumber = cms.int32(2)


## --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

##from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
##VBFFwdCaloJetCountFilterL2L3SisCone5CaloJets = VBFFwdCaloJetCountFilter.clone()

##VBFFwdCaloJetCountFilterL2L3SisCone5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsL2L3SisCone5CaloJets")






## --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
#VBFNtupleL2L3SisCone5CaloJets = SimpleNtple.clone()

#VBFNtupleL2L3SisCone5CaloJets.EleTag = cms.InputTag("gsfElectrons")
#VBFNtupleL2L3SisCone5CaloJets.doEleRefCheck = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
#VBFNtupleL2L3SisCone5CaloJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
#VBFNtupleL2L3SisCone5CaloJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
#VBFNtupleL2L3SisCone5CaloJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

#VBFNtupleL2L3SisCone5CaloJets.MuTag = cms.InputTag("muons")
#VBFNtupleL2L3SisCone5CaloJets.doMuRefCheck = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
#VBFNtupleL2L3SisCone5CaloJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
#VBFNtupleL2L3SisCone5CaloJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
#VBFNtupleL2L3SisCone5CaloJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

#VBFNtupleL2L3SisCone5CaloJets.JetTag = cms.InputTag("VBFL2L3SisCone5CaloJets")
#VBFNtupleL2L3SisCone5CaloJets.doJetRefCheck = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.JetRefTag = cms.InputTag("VBFSelectedCaloJetsRefL2L3SisCone5CaloJets")
#VBFNtupleL2L3SisCone5CaloJets.genJetTag = cms.InputTag("sisCone5GenJets")

#VBFNtupleL2L3SisCone5CaloJets.Type1MetTag = cms.InputTag("VBFType1SisCone5CaloMet")

#VBFNtupleL2L3SisCone5CaloJets.saveMu          = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.saveTrack       = cms.untracked.bool(False)
#VBFNtupleL2L3SisCone5CaloJets.saveEle         = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.saveJet         = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.savePFJet       = cms.untracked.bool(False)
#VBFNtupleL2L3SisCone5CaloJets.saveJetBTagging = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.saveMet         = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.saveGenJet      = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.saveGenMet      = cms.untracked.bool(True)
#VBFNtupleL2L3SisCone5CaloJets.saveMC          = cms.untracked.bool(True)






#VBFPreselectionSequenceL2L3SisCone5CaloJets = cms.Sequence(
    
    #VBFPreselectionSequenceAll +
    
    
    
    #VBFL2L3SisCone5CaloJetsSequence +
    #VBFType1SisCone5CaloMetSequence +

    #VBFCleanedCaloJetsL2L3SisCone5CaloJets +
    #VBFCleanedCaloJetsRefL2L3SisCone5CaloJets +
    
    #VBFSelectedCaloJetsL2L3SisCone5CaloJets +
    #VBFSelectedCaloJetsRefL2L3SisCone5CaloJets +
    
    
    
    #VBFEtMinCaloJetCountFilterL2L3SisCone5CaloJets +
    ##VBFFwdCaloJetCountFilterL2L3SisCone5CaloJets +
    
    
    
    #VBFNtupleL2L3SisCone5CaloJets
    #)

## ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
