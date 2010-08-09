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

from HiggsAnalysis.VBFHiggsToVV.VBFMetCorrector_cff import *



# --- THE JET CLEANER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetCleaner_cfi import *
VBFCleanedPFJetsAk5PFJets = VBFCleanedPFJets.clone()
VBFCleanedPFJetsRefAk5PFJets = VBFCleanedPFJetsRef.clone()

VBFCleanedPFJetsAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFCleanedPFJetsAk5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsAk5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")

VBFCleanedPFJetsRefAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFCleanedPFJetsRefAk5PFJets.doElectronRefCheck = cms.bool(True)
VBFCleanedPFJetsRefAk5PFJets.srcElectronsRef = cms.InputTag("VBFIdSelectedElectronsRefAll")


# --- THE JET SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFJetSelector_cfi import *
VBFSelectedPFJetsAk5PFJets = VBFSelectedPFJets.clone()
VBFSelectedPFJetsRefAk5PFJets = VBFSelectedPFJetsRef.clone()

VBFSelectedPFJetsAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFSelectedPFJetsAk5PFJets.etMin = cms.double(15.)
VBFSelectedPFJetsAk5PFJets.etaMin = cms.double(-5.0)
VBFSelectedPFJetsAk5PFJets.etaMax = cms.double(+5.0)
VBFSelectedPFJetsAk5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsAk5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefAk5PFJets")

VBFSelectedPFJetsRefAk5PFJets.src = cms.InputTag("ak5PFJets")
VBFSelectedPFJetsRefAk5PFJets.etMin = cms.double(15.)
VBFSelectedPFJetsRefAk5PFJets.etaMin = cms.double(-5.0)
VBFSelectedPFJetsAk5PFJets.etaMax = cms.double(+5.0)
VBFSelectedPFJetsRefAk5PFJets.doRefCheck = cms.bool(True)
VBFSelectedPFJetsRefAk5PFJets.srcJetsRef = cms.InputTag("VBFCleanedPFJetsRefAk5PFJets")






# --- THE JET FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFEtMinJetCountFilter_cfi import *
VBFEtMinPFJetCountFilterAk5PFJets = VBFEtMinPFJetCountFilter.clone()

VBFEtMinPFJetCountFilterAk5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsAk5PFJets")
VBFEtMinPFJetCountFilterAk5PFJets.etMin = cms.double(15.)
VBFEtMinPFJetCountFilterAk5PFJets.etaMin = cms.double(-5.0)
VBFEtMinPFJetCountFilterAk5PFJets.etaMax = cms.double(+5.0)
VBFEtMinPFJetCountFilterAk5PFJets.minNumber = cms.int32(2)


# --- THE FWD JET FILTER --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFFwdJetCountFilter_cfi import *
#VBFFwdPFJetCountFilterAk5PFJets = VBFFwdPFJetCountFilter.clone()

#VBFFwdPFJetCountFilterAk5PFJets.srcJets = cms.InputTag("VBFSelectedPFJetsAk5PFJets")






# --- THE NTUPLE --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.SimpleNtple_cfi import *
VBFNtupleAk5PFJets = SimpleNtple.clone()

VBFNtupleAk5PFJets.EleTag = cms.InputTag("gsfElectrons")
VBFNtupleAk5PFJets.doEleRefCheck = cms.untracked.bool(True)
VBFNtupleAk5PFJets.EleRefTag = cms.InputTag("VBFIdSelectedElectronsRefAll")
VBFNtupleAk5PFJets.Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Ele3DipSignificanceMap")
VBFNtupleAk5PFJets.EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleTipSignificanceMap")
VBFNtupleAk5PFJets.EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:EleLipSignificanceMap")

VBFNtupleAk5PFJets.MuTag = cms.InputTag("muons")
VBFNtupleAk5PFJets.doMuRefCheck = cms.untracked.bool(True)
VBFNtupleAk5PFJets.MuRefTag = cms.InputTag("VBFIsolatedMuonsRefAll")
VBFNtupleAk5PFJets.Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducerAll:Mu3DipSignificanceMap")
VBFNtupleAk5PFJets.MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuTipSignificanceMap")
VBFNtupleAk5PFJets.MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducerAll:MuLipSignificanceMap")

VBFNtupleAk5PFJets.JetTag = cms.InputTag("ak5PFJets")
VBFNtupleAk5PFJets.doJetRefCheck = cms.untracked.bool(True)
VBFNtupleAk5PFJets.JetRefTag = cms.InputTag("VBFSelectedPFJetsRefAk5PFJets")
VBFNtupleAk5PFJets.genJetTag = cms.InputTag("ak5GenJets")

VBFNtupleAk5PFJets.Type1MetTag = cms.InputTag("VBFType1Ak5CaloMet")

VBFNtupleAk5PFJets.saveMu          = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveTrack       = cms.untracked.bool(False)
VBFNtupleAk5PFJets.saveEle         = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveJet         = cms.untracked.bool(False)
VBFNtupleAk5PFJets.savePFJet       = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveJetBTagging = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveMet         = cms.untracked.bool(True)
VBFNtupleAk5PFJets.saveGenJet      = cms.untracked.bool(False)
VBFNtupleAk5PFJets.saveGenMet      = cms.untracked.bool(False)
VBFNtupleAk5PFJets.saveMC          = cms.untracked.bool(False)






VBFPreselectionSequenceAk5PFJets = cms.Sequence(
    
    VBFPreselectionSequenceAll +
    
    
    
    VBFType1Ak5CaloMetSequence +
    

    
    VBFCleanedPFJetsAk5PFJets +
    VBFCleanedPFJetsRefAk5PFJets +
    
    VBFSelectedPFJetsAk5PFJets +
    VBFSelectedPFJetsRefAk5PFJets +
    
    
    
    VBFEtMinPFJetCountFilterAk5PFJets +
    #VBFFwdPFJetCountFilterAk5PFJets +
    
    
    
    VBFNtupleAk5PFJets
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
