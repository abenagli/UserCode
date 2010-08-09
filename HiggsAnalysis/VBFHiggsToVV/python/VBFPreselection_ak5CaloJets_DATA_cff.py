import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# ak5CaloJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_DATA_cff import *






# --- THE JET/MET CORRECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *



# --- THE JET ID --- --- --- --- --- --- --- --- --- --- ---

from RecoJets.JetProducers.JetIDParams_cfi import *

VBFAk5JetID = cms.EDProducer(
    'JetIDProducer',
    JetIDParams,
    src = cms.InputTag('ak5CaloJets')
    )



# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedCaloJetsAk5CaloJets = VBFCleanedCaloJets.clone()
VBFCleanedCaloJetsRefAk5CaloJets = VBFCleanedCaloJetsRef.clone()

VBFCleanedCaloJetsAk5CaloJets.src = cms.InputTag("ak5CaloJets")
VBFCleanedCaloJetsAk5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsAk5CaloJets.srcElectronsRef = cms.InputTag("VBFSpikeRemovalSelectedElectronsRefAll")
VBFCleanedCaloJetsAk5CaloJets.srcMuonsRef = cms.InputTag("VBFIdSelectedMuonsRefAll")

VBFCleanedCaloJetsRefAk5CaloJets.src = cms.InputTag("ak5CaloJets")
VBFCleanedCaloJetsRefAk5CaloJets.doElectronRefCheck = cms.bool(True)
VBFCleanedCaloJetsRefAk5CaloJets.srcElectronsRef = cms.InputTag("VBFSpikeRemovalSelectedElectronsRefAll")
VBFCleanedCaloJetsRefAk5CaloJets.srcMuonsRef = cms.InputTag("VBFIdSelectedMuonsRefAll")



# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedCaloJetsAk5CaloJets = VBFSelectedCaloJets.clone()
VBFSelectedCaloJetsRefAk5CaloJets = VBFSelectedCaloJetsRef.clone()

VBFSelectedCaloJetsAk5CaloJets.src = cms.InputTag("ak5CaloJets")
VBFSelectedCaloJetsAk5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsAk5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefAk5CaloJets")

VBFSelectedCaloJetsRefAk5CaloJets.src = cms.InputTag("ak5CaloJets")
VBFSelectedCaloJetsRefAk5CaloJets.doRefCheck = cms.bool(True)
VBFSelectedCaloJetsRefAk5CaloJets.srcJetsRef = cms.InputTag("VBFCleanedCaloJetsRefAk5CaloJets")



# --- THE JET ID SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFCaloJetIdSelector_cfi import *
VBFIdSelectedCaloJetsAk5CaloJets = VBFIdSelectedCaloJets.clone()
VBFIdSelectedCaloJetsRefAk5CaloJets = VBFIdSelectedCaloJetsRef.clone()

VBFIdSelectedCaloJetsAk5CaloJets.src = cms.InputTag("ak5CaloJets")
VBFIdSelectedCaloJetsAk5CaloJets.doRefCheck = cms.bool(True)
VBFIdSelectedCaloJetsAk5CaloJets.srcJetsRef = cms.InputTag("VBFSelectedCaloJetsRefAk5CaloJets")

VBFIdSelectedCaloJetsRefAk5CaloJets.src = cms.InputTag("ak5CaloJets")
VBFIdSelectedCaloJetsRefAk5CaloJets.doRefCheck = cms.bool(True)
VBFIdSelectedCaloJetsRefAk5CaloJets.srcJetsRef = cms.InputTag("VBFSelectedCaloJetsRefAk5CaloJets")



# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinCaloJetCountFilterAk5CaloJets = VBFEtMinCaloJetCountFilter.clone()

VBFEtMinCaloJetCountFilterAk5CaloJets.srcJets = cms.InputTag("VBFIdSelectedCaloJetsAk5CaloJets")












# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
VBFNtupleAk5CaloJets = SimpleNtple.clone()

VBFNtupleAk5CaloJets.EleTag = cms.InputTag("gsfElectrons")
VBFNtupleAk5CaloJets.doEleRefCheck = cms.untracked.bool(True)
VBFNtupleAk5CaloJets.EleRefTag = cms.InputTag("VBFSpikeRemovalSelectedElectronsRefAll")

VBFNtupleAk5CaloJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
VBFNtupleAk5CaloJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
VBFNtupleAk5CaloJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

VBFNtupleAk5CaloJets.MuTag = cms.InputTag("muons")
VBFNtupleAk5CaloJets.doMuRefCheck = cms.untracked.bool(True)
VBFNtupleAk5CaloJets.MuRefTag = cms.InputTag("VBFIdSelectedMuonsRefAll")

VBFNtupleAk5CaloJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
VBFNtupleAk5CaloJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
VBFNtupleAk5CaloJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

VBFNtupleAk5CaloJets.JetTag = cms.InputTag("ak5CaloJets")
VBFNtupleAk5CaloJets.doJetRefCheck = cms.untracked.bool(True)
VBFNtupleAk5CaloJets.JetRefTag = cms.InputTag("VBFIdSelectedCaloJetsRefAk5CaloJets")

VBFNtupleAk5CaloJets.JetIDTag = cms.InputTag("VBFAk5JetID")

VBFNtupleAk5CaloJets.Type1MetTag = cms.InputTag("VBFType1Ak5CaloMet")

VBFNtupleAk5CaloJets.eventType       = cms.untracked.int32(1)
VBFNtupleAk5CaloJets.saveMu          = cms.untracked.bool(True)
VBFNtupleAk5CaloJets.saveTrack       = cms.untracked.bool(False)
VBFNtupleAk5CaloJets.saveEle         = cms.untracked.bool(True)
VBFNtupleAk5CaloJets.saveJet         = cms.untracked.bool(True)
VBFNtupleAk5CaloJets.savePFJet       = cms.untracked.bool(False)
VBFNtupleAk5CaloJets.saveJetBTagging = cms.untracked.bool(True)
VBFNtupleAk5CaloJets.saveMet         = cms.untracked.bool(True)
VBFNtupleAk5CaloJets.saveGenJet      = cms.untracked.bool(False)
VBFNtupleAk5CaloJets.saveGenMet      = cms.untracked.bool(False)
VBFNtupleAk5CaloJets.saveMC          = cms.untracked.bool(False)
VBFNtupleAk5CaloJets.saveMCEle       = cms.untracked.bool(False)
VBFNtupleAk5CaloJets.saveMCMu        = cms.untracked.bool(False)
VBFNtupleAk5CaloJets.saveMCPtHat     = cms.untracked.bool(False)
VBFNtupleAk5CaloJets.saveMCHiggs     = cms.untracked.bool(False)
VBFNtupleAk5CaloJets.saveMCHiggsDecay= cms.untracked.bool(False)


VBFPreselectionSequenceAk5CaloJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    VBFType1Ak5CaloMetSequence +

    VBFAk5JetID +
    
    VBFCleanedCaloJetsAk5CaloJets +
    VBFCleanedCaloJetsRefAk5CaloJets +
    
    VBFSelectedCaloJetsAk5CaloJets +
    VBFSelectedCaloJetsRefAk5CaloJets +

    VBFIdSelectedCaloJetsAk5CaloJets +
    VBFIdSelectedCaloJetsRefAk5CaloJets +
    
    VBFEtMinCaloJetCountFilterAk5CaloJets +
    
    VBFNtupleAk5CaloJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
