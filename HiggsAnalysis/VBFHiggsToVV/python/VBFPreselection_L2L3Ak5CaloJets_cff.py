import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# ak5CaloJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_cff import *






# --- THE JET CORRECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCorrector_cff import *
from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *



# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedCaloJetsL2L3Ak5CaloJets = VBFCleanedCaloJets.clone()
VBFCleanedCaloJetsRefL2L3Ak5CaloJets = VBFCleanedCaloJetsRef.clone()

VBFCleanedCaloJetsL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")
VBFCleanedCaloJetsL2L3Ak5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsL2L3Ak5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedCaloJetsRefL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")
VBFCleanedCaloJetsRefL2L3Ak5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsRefL2L3Ak5CaloJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedCaloJetsL2L3Ak5CaloJets = VBFSelectedCaloJets.clone()
VBFSelectedCaloJetsRefL2L3Ak5CaloJets = VBFSelectedCaloJetsRef.clone()

VBFSelectedCaloJetsL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")
VBFSelectedCaloJetsL2L3Ak5CaloJets.etMin = cms.double(15.)
VBFSelectedCaloJetsL2L3Ak5CaloJets.etaMin = cms.double(-5.0)
VBFSelectedCaloJetsL2L3Ak5CaloJets.etaMax = cms.double(+5.0)
VBFSelectedCaloJetsL2L3Ak5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsL2L3Ak5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefL2L3Ak5CaloJets")

VBFSelectedCaloJetsRefL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")
VBFSelectedCaloJetsRefL2L3Ak5CaloJets.etMin = cms.double(15.)
VBFSelectedCaloJetsRefL2L3Ak5CaloJets.etaMin = cms.double(-5.0)
VBFSelectedCaloJetsL2L3Ak5CaloJets.etaMax = cms.double(+5.0)
VBFSelectedCaloJetsRefL2L3Ak5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsRefL2L3Ak5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefL2L3Ak5CaloJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets = VBFEtMinCaloJetCountFilter.clone()

VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsL2L3Ak5CaloJets")
VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets.etMin = cms.double(15.)
VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets.etaMin = cms.double(-5.0)
VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets.etaMax = cms.double(+5.0)
VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets.minNumber = cms.int32(2)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
#VBFFwdCaloJetCountFilterL2L3Ak5CaloJets = VBFFwdCaloJetCountFilter.clone()

#VBFFwdCaloJetCountFilterL2L3Ak5CaloJets.srcJets = cms.InputTag("VBFSelectedCaloJetsL2L3Ak5CaloJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
VBFNtupleL2L3Ak5CaloJets = SimpleNtple.clone()

VBFNtupleL2L3Ak5CaloJets.EleTag = cms.InputTag("gsfElectrons")
VBFNtupleL2L3Ak5CaloJets.doEleRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
VBFNtupleL2L3Ak5CaloJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
VBFNtupleL2L3Ak5CaloJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
VBFNtupleL2L3Ak5CaloJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

VBFNtupleL2L3Ak5CaloJets.MuTag = cms.InputTag("muons")
VBFNtupleL2L3Ak5CaloJets.doMuRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
VBFNtupleL2L3Ak5CaloJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
VBFNtupleL2L3Ak5CaloJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
VBFNtupleL2L3Ak5CaloJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

VBFNtupleL2L3Ak5CaloJets.JetTag = cms.InputTag("VBFL2L3Ak5CaloJets")
VBFNtupleL2L3Ak5CaloJets.doJetRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.JetRefTag = cms.InputTag("VBFSelectedCaloJetsRefL2L3Ak5CaloJets")
VBFNtupleL2L3Ak5CaloJets.genJetTag = cms.InputTag("ak5GenJets")

VBFNtupleL2L3Ak5CaloJets.Type1MetTag = cms.InputTag("VBFType1Ak5CaloMet")

VBFNtupleL2L3Ak5CaloJets.saveMu          = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveTrack       = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveEle         = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveJet         = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.savePFJet       = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveJetBTagging = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveMet         = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveGenJet      = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveGenMet      = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveMC          = cms.untracked.bool(False)






VBFPreselectionSequenceL2L3Ak5CaloJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    
    
    VBFL2L3Ak5CaloJetsSequence +
    VBFType1Ak5CaloMetSequence +



    VBFCleanedCaloJetsL2L3Ak5CaloJets +
    VBFCleanedCaloJetsRefL2L3Ak5CaloJets +
    
    VBFSelectedCaloJetsL2L3Ak5CaloJets +
    VBFSelectedCaloJetsRefL2L3Ak5CaloJets +
    
    
    
    VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets +
    #VBFFwdCaloJetCountFilterL2L3Ak5CaloJets +
    
    
    
    VBFNtupleL2L3Ak5CaloJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
