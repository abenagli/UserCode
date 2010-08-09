import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# ak5PFJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_DATA_cff import *






# --- THE JET/MET CORRECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *



# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedPFJetsAk5PFJets = VBFCleanedPFJets.clone()
VBFCleanedPFJetsRefAk5PFJets = VBFCleanedPFJetsRef.clone()

VBFCleanedPFJetsAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFCleanedPFJetsAk5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsAk5PFJets.srcElectronsRef = cms.InputTag("VBFSpikeRemovalSelectedElectronsRefAll")
VBFCleanedPFJetsAk5PFJets.srcMuonsRef = cms.InputTag("VBFIdSelectedMuonsRefAll")

VBFCleanedPFJetsRefAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFCleanedPFJetsRefAk5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsRefAk5PFJets.srcElectronsRef = cms.InputTag("VBFSpikeRemovalSelectedElectronsRefAll")
VBFCleanedPFJetsRefAk5PFJets.srcMuonsRef = cms.InputTag("VBFIdSelectedMuonsRefAll")



# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedPFJetsAk5PFJets = VBFSelectedPFJets.clone()
VBFSelectedPFJetsRefAk5PFJets = VBFSelectedPFJetsRef.clone()

VBFSelectedPFJetsAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFSelectedPFJetsAk5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsAk5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefAk5PFJets")

VBFSelectedPFJetsRefAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFSelectedPFJetsRefAk5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsRefAk5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefAk5PFJets")



# --- THE JET ID SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPFJetIdSelector_cfi import *
VBFIdSelectedPFJetsAk5PFJets = VBFIdSelectedPFJets.clone()
VBFIdSelectedPFJetsRefAk5PFJets = VBFIdSelectedPFJetsRef.clone()

VBFIdSelectedPFJetsAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFIdSelectedPFJetsAk5PFJets.doRefCheck = cms.bool(True)
VBFIdSelectedPFJetsAk5PFJets.srcJetsRef = cms.InputTag("VBFSelectedPFJetsRefAk5PFJets")

VBFIdSelectedPFJetsRefAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFIdSelectedPFJetsRefAk5PFJets.doRefCheck = cms.bool(True)
VBFIdSelectedPFJetsRefAk5PFJets.srcJetsRef = cms.InputTag("VBFSelectedPFJetsRefAk5PFJets")



# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinPFJetCountFilterAk5PFJets = VBFEtMinPFJetCountFilter.clone()

VBFEtMinPFJetCountFilterAk5PFJets.srcJets = cms.InputTag("VBFIdSelectedPFJetsAk5PFJets")












# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
VBFNtupleAk5PFJets = SimpleNtple.clone()

VBFNtupleAk5PFJets.HLTTag = cms.InputTag("TriggerResults::REDIGI")

VBFNtupleAk5PFJets.EleTag = cms.InputTag("gsfElectrons")
VBFNtupleAk5PFJets.doEleRefCheck = cms.untracked.bool(True)
VBFNtupleAk5PFJets.EleRefTag = cms.InputTag("VBFSpikeRemovalSelectedElectronsRefAll")

VBFNtupleAk5PFJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
VBFNtupleAk5PFJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
VBFNtupleAk5PFJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

VBFNtupleAk5PFJets.MuTag = cms.InputTag("muons")
VBFNtupleAk5PFJets.doMuRefCheck = cms.untracked.bool(True)
VBFNtupleAk5PFJets.MuRefTag = cms.InputTag("VBFIdSelectedMuonsRefAll")

VBFNtupleAk5PFJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
VBFNtupleAk5PFJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
VBFNtupleAk5PFJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

VBFNtupleAk5PFJets.JetTag = cms.InputTag("ak5PFJets")
VBFNtupleAk5PFJets.doJetRefCheck = cms.untracked.bool(True)
VBFNtupleAk5PFJets.JetRefTag = cms.InputTag("VBFIdSelectedPFJetsRefAk5PFJets")

VBFNtupleAk5PFJets.JetIDTag = cms.InputTag("VBFAk5JetID")

VBFNtupleAk5PFJets.Type1MetTag = cms.InputTag("VBFType1Ak5CaloMet")

VBFNtupleAk5PFJets.eventType       = cms.untracked.int32(1)
VBFNtupleAk5PFJets.saveMu          = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveTrack       = cms.untracked.bool(False)
VBFNtupleAk5PFJets.saveEle         = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveJet         = cms.untracked.bool(False)
VBFNtupleAk5PFJets.savePFJet       = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveJetBTagging = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveMet         = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveGenJet      = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveGenMet      = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveMC          = cms.untracked.bool(False)
VBFNtupleAk5PFJets.saveMCEle       = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveMCMu        = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveMCHiggs     = cms.untracked.bool(False)
VBFNtupleAk5PFJets.saveMCHiggsDecay= cms.untracked.bool(False)



VBFPreselectionSequenceAk5PFJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    VBFType1Ak5CaloMetSequence +

    VBFCleanedPFJetsAk5PFJets +
    VBFCleanedPFJetsRefAk5PFJets +
    
    VBFSelectedPFJetsAk5PFJets +
    VBFSelectedPFJetsRefAk5PFJets +

    VBFIdSelectedPFJetsAk5PFJets +
    VBFIdSelectedPFJetsRefAk5PFJets +

    VBFEtMinPFJetCountFilterAk5PFJets +
    
    VBFNtupleAk5PFJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
