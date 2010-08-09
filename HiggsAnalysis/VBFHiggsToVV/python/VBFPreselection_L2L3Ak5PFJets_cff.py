import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# ak5PFJets
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
VBFCleanedPFJetsL2L3Ak5PFJets = VBFCleanedPFJets.clone()
VBFCleanedPFJetsRefL2L3Ak5PFJets = VBFCleanedPFJetsRef.clone()

VBFCleanedPFJetsL2L3Ak5PFJets.src = cms.InputTag("VBFL2L3Ak5PFJets")
VBFCleanedPFJetsL2L3Ak5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsL2L3Ak5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedPFJetsRefL2L3Ak5PFJets.src = cms.InputTag("VBFL2L3Ak5PFJets")
VBFCleanedPFJetsRefL2L3Ak5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsRefL2L3Ak5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedPFJetsL2L3Ak5PFJets = VBFSelectedPFJets.clone()
VBFSelectedPFJetsRefL2L3Ak5PFJets = VBFSelectedPFJetsRef.clone()

VBFSelectedPFJetsL2L3Ak5PFJets.src = cms.InputTag("VBFL2L3Ak5PFJets")
VBFSelectedPFJetsL2L3Ak5PFJets.etMin = cms.double(15.)
VBFSelectedPFJetsL2L3Ak5PFJets.etaMin = cms.double(-5.0)
VBFSelectedPFJetsL2L3Ak5PFJets.etaMax = cms.double(+5.0)
VBFSelectedPFJetsL2L3Ak5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsL2L3Ak5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefL2L3Ak5PFJets")

VBFSelectedPFJetsRefL2L3Ak5PFJets.src = cms.InputTag("VBFL2L3Ak5PFJets")
VBFSelectedPFJetsRefL2L3Ak5PFJets.etMin = cms.double(15.)
VBFSelectedPFJetsRefL2L3Ak5PFJets.etaMin = cms.double(-5.0)
VBFSelectedPFJetsL2L3Ak5PFJets.etaMax = cms.double(+5.0)
VBFSelectedPFJetsRefL2L3Ak5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsRefL2L3Ak5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefL2L3Ak5PFJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinPFJetCountFilterL2L3Ak5PFJets = VBFEtMinPFJetCountFilter.clone()

VBFEtMinPFJetCountFilterL2L3Ak5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsL2L3Ak5PFJets")
VBFEtMinPFJetCountFilterL2L3Ak5PFJets.etMin = cms.double(15.)
VBFEtMinPFJetCountFilterL2L3Ak5PFJets.etaMin = cms.double(-5.0)
VBFEtMinPFJetCountFilterL2L3Ak5PFJets.etaMax = cms.double(+5.0)
VBFEtMinPFJetCountFilterL2L3Ak5PFJets.minNumber = cms.int32(2)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
#VBFFwdPFJetCountFilterL2L3Ak5PFJets = VBFFwdPFJetCountFilter.clone()

#VBFFwdPFJetCountFilterL2L3Ak5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsL2L3Ak5PFJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
VBFNtupleL2L3Ak5PFJets = SimpleNtple.clone()

VBFNtupleL2L3Ak5PFJets.EleTag = cms.InputTag("gsfElectrons")
VBFNtupleL2L3Ak5PFJets.doEleRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
VBFNtupleL2L3Ak5PFJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
VBFNtupleL2L3Ak5PFJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
VBFNtupleL2L3Ak5PFJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

VBFNtupleL2L3Ak5PFJets.MuTag = cms.InputTag("muons")
VBFNtupleL2L3Ak5PFJets.doMuRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
VBFNtupleL2L3Ak5PFJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
VBFNtupleL2L3Ak5PFJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
VBFNtupleL2L3Ak5PFJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

VBFNtupleL2L3Ak5PFJets.JetTag = cms.InputTag("VBFL2L3Ak5PFJets")
VBFNtupleL2L3Ak5PFJets.doJetRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.JetRefTag = cms.InputTag("VBFSelectedPFJetsRefL2L3Ak5PFJets")
VBFNtupleL2L3Ak5PFJets.genJetTag = cms.InputTag("ak5GenJets")

VBFNtupleL2L3Ak5PFJets.Type1MetTag = cms.InputTag("VBFType1Ak5CaloMet")

VBFNtupleL2L3Ak5PFJets.saveMu          = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveTrack       = cms.untracked.bool(False)
VBFNtupleL2L3Ak5PFJets.saveEle         = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveJet         = cms.untracked.bool(False)
VBFNtupleL2L3Ak5PFJets.savePFJet       = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveJetBTagging = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveMet         = cms.untracked.bool(True)
VBFNtupleL2L3Ak5PFJets.saveGenJet      = cms.untracked.bool(False)
VBFNtupleL2L3Ak5PFJets.saveGenMet      = cms.untracked.bool(False)
VBFNtupleL2L3Ak5PFJets.saveMC          = cms.untracked.bool(False)






VBFPreselectionSequenceL2L3Ak5PFJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    
    
    VBFL2L3Ak5PFJetsSequence +
    VBFType1Ak5CaloMetSequence +



    VBFCleanedPFJetsL2L3Ak5PFJets +
    VBFCleanedPFJetsRefL2L3Ak5PFJets +
    
    VBFSelectedPFJetsL2L3Ak5PFJets +
    VBFSelectedPFJetsRefL2L3Ak5PFJets +
    
    
    
    VBFEtMinPFJetCountFilterL2L3Ak5PFJets +
    #VBFFwdPFJetCountFilterL2L3Ak5PFJets +
    
    
    
    VBFNtupleL2L3Ak5PFJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
