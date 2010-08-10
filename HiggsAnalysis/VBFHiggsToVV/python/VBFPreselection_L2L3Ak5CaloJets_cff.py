import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# ak5CaloJets
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE COMMON MODULES --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPreselection_all_cff import *






# --- THE JET/MET CORRECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCorrector_cff import *
from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *



# --- THE JET ID --- --- --- --- --- --- --- --- --- --- ---

from RecoJets.JetProducers.JetIDParams_cfi import *

VBFL2L3Ak5CaloJetID = cms.EDProducer(
    'JetIDProducer',
    JetIDParams,
    src = cms.InputTag('ak5CaloJets')
    )



# --- THE B-TAGGING --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetBTagging_cff import *



# --- THE JET CHARGE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCharge_cfi import *

VBFL2L3Ak5CaloJetsCharge = VBFCaloJetsCharge.clone()
VBFL2L3Ak5CaloJetsCharge.src = cms.InputTag("VBFL2L3Ak5CaloJetsJetTracksAssociatorAtVertex")



# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---
#
#from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
#VBFCleanedCaloJetsL2L3Ak5CaloJets = VBFCleanedCaloJets.clone()
#VBFCleanedCaloJetsRefL2L3Ak5CaloJets = VBFCleanedCaloJetsRef.clone()
#
#VBFCleanedCaloJetsL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")
#VBFCleanedCaloJetsL2L3Ak5CaloJets.doElectronRefCheck = cms.bool(True)
#VBFCleanedCaloJetsL2L3Ak5CaloJets.srcElectronsRef = cms.InputTag("VBFSpikeRemovalSelectedElectronsRefAll")
#VBFCleanedCaloJetsL2L3Ak5CaloJets.srcMuonsRef = cms.InputTag("VBFIdSelectedMuonsRefAll")
#
#VBFCleanedCaloJetsRefL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")
#VBFCleanedCaloJetsRefL2L3Ak5CaloJets.doElectronRefCheck = cms.bool(True)
#VBFCleanedCaloJetsRefL2L3Ak5CaloJets.srcElectronsRef = cms.InputTag("VBFSpikeRemovalSelectedElectronsRefAll")
#VBFCleanedCaloJetsRefL2L3Ak5CaloJets.srcMuonsRef = cms.InputTag("VBFIdSelectedMuonsRefAll")



# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedCaloJetsL2L3Ak5CaloJets = VBFSelectedCaloJets.clone()
VBFSelectedCaloJetsRefL2L3Ak5CaloJets = VBFSelectedCaloJetsRef.clone()

VBFSelectedCaloJetsL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")

VBFSelectedCaloJetsRefL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")



## --- THE JET ID SELECTOR --- --- --- --- --- --- --- --- --- --- ---
#
#from HiggsAnalysis.VBFHiggsToVV.VBFCaloJetIdSelector_cfi import *
#VBFIdSelectedCaloJetsL2L3Ak5CaloJets = VBFIdSelectedCaloJets.clone()
#VBFIdSelectedCaloJetsRefL2L3Ak5CaloJets = VBFIdSelectedCaloJetsRef.clone()
#
#VBFIdSelectedCaloJetsL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")
#VBFIdSelectedCaloJetsL2L3Ak5CaloJets.srcJetID = cms.InputTag("VBFL2L3Ak5JetID")
#VBFIdSelectedCaloJetsL2L3Ak5CaloJets.doRefCheck = cms.bool(True)
#VBFIdSelectedCaloJetsL2L3Ak5CaloJets.srcJetsRef = cms.InputTag("VBFSelectedCaloJetsRefL2L3Ak5CaloJets")
#
#VBFIdSelectedCaloJetsRefL2L3Ak5CaloJets.src = cms.InputTag("VBFL2L3Ak5CaloJets")
#VBFIdSelectedCaloJetsRefL2L3Ak5CaloJets.srcJetID = cms.InputTag("VBFL2L3Ak5JetID")
#VBFIdSelectedCaloJetsRefL2L3Ak5CaloJets.doRefCheck = cms.bool(True)
#VBFIdSelectedCaloJetsRefL2L3Ak5CaloJets.srcJetsRef = cms.InputTag("VBFSelectedCaloJetsRefL2L3Ak5CaloJets")



## --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---
#
#from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
#VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets = VBFEtMinCaloJetCountFilter.clone()
#
#VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets.srcJets = cms.InputTag("VBFIdSelectedCaloJetsL2L3Ak5CaloJets")












# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
VBFNtupleL2L3Ak5CaloJets = SimpleNtple.clone()

VBFNtupleL2L3Ak5CaloJets.EleTag = cms.InputTag("gsfElectrons")
VBFNtupleL2L3Ak5CaloJets.doEleRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.EleRefTag = cms.InputTag("VBFConversionRemovalSelectedElectronsRefAll")

VBFNtupleL2L3Ak5CaloJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
#VBFNtupleL2L3Ak5CaloJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
#VBFNtupleL2L3Ak5CaloJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

VBFNtupleL2L3Ak5CaloJets.MuTag = cms.InputTag("muons")
VBFNtupleL2L3Ak5CaloJets.doMuRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.MuRefTag = cms.InputTag("VBFSelectedMuonsRefAll")

VBFNtupleL2L3Ak5CaloJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
#VBFNtupleL2L3Ak5CaloJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
#VBFNtupleL2L3Ak5CaloJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

VBFNtupleL2L3Ak5CaloJets.JetTag = cms.InputTag("VBFL2L3Ak5CaloJets")
VBFNtupleL2L3Ak5CaloJets.doJetRefCheck = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.JetRefTag = cms.InputTag("VBFSelectedCaloJetsRefL2L3Ak5CaloJets")

VBFNtupleL2L3Ak5CaloJets.JetIDTag = cms.InputTag("VBFL2L3Ak5CaloJetID")

VBFNtupleL2L3Ak5CaloJets.TrackCountingHighEffBJetTagsTag = cms.InputTag("VBFL2L3Ak5CaloJetsTrackCountingHighEffBJetTags")
VBFNtupleL2L3Ak5CaloJets.TrackCountingHighPurBJetTagsTag = cms.InputTag("VBFL2L3Ak5CaloJetsTrackCountingHighPurBJetTags")
VBFNtupleL2L3Ak5CaloJets.JetProbabilityBJetTagsTag       = cms.InputTag("VBFL2L3Ak5CaloJetsJetProbabilityBJetTags")
VBFNtupleL2L3Ak5CaloJets.JetBProbabilityBJetTagsTag      = cms.InputTag("VBFL2L3Ak5CaloJetsJetBProbabilityBJetTags")

VBFNtupleL2L3Ak5CaloJets.JetChargeTag = cms.InputTag("VBFL2L3Ak5CaloJetsCharge")

VBFNtupleL2L3Ak5CaloJets.Type1MetTag = cms.InputTag("VBFType1Ak5CaloMet")

VBFNtupleL2L3Ak5CaloJets.eventType       = cms.untracked.int32(1)
VBFNtupleL2L3Ak5CaloJets.saveTrack       = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveMu          = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveEle         = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveJet         = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.savePFJet       = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveJetBTagging = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveMet         = cms.untracked.bool(True)
VBFNtupleL2L3Ak5CaloJets.saveGenJet      = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveGenMet      = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveMC          = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveMCEle       = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveMCMu        = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveMCPtHat     = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveMCHiggs     = cms.untracked.bool(False)
VBFNtupleL2L3Ak5CaloJets.saveMCHiggsDecay= cms.untracked.bool(False)



VBFPreselectionSequenceL2L3Ak5CaloJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    VBFL2L3Ak5CaloJetsSequence +
    VBFType1Ak5CaloMetSequence +

    VBFL2L3Ak5CaloJetID +

    VBFL2L3Ak5CaloJetsJetTracksAssociator +
    VBFL2L3Ak5CaloJetsJetBtaggingIP +

    VBFL2L3Ak5CaloJetsCharge + 

    #VBFCleanedCaloJetsL2L3Ak5CaloJets +
    #VBFCleanedCaloJetsRefL2L3Ak5CaloJets +
    
    VBFSelectedCaloJetsL2L3Ak5CaloJets +
    VBFSelectedCaloJetsRefL2L3Ak5CaloJets +

    #VBFIdSelectedCaloJetsL2L3Ak5CaloJets +
    #VBFIdSelectedCaloJetsRefL2L3Ak5CaloJets +

    #VBFEtMinCaloJetCountFilterL2L3Ak5CaloJets +
    
    VBFNtupleL2L3Ak5CaloJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
