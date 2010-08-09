import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# all
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE MC FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.MCDecayModeFilter_cfi import *
MCDecayModeFilterAll = MCDecayModeFilter.clone()

MCDecayModeFilterAll.eventType = cms.untracked.int32(0)
MCDecayModeFilterAll.H_W_W = cms.bool(True)

MCDecayModeFilterAll.V1_e_nue = cms.bool(True)
MCDecayModeFilterAll.V1_mu_numu = cms.bool(True)
MCDecayModeFilterAll.V1_tau_nutau = cms.bool(True)

MCDecayModeFilterAll.V2_e_nue = cms.bool(True)
MCDecayModeFilterAll.V2_mu_numu = cms.bool(True)
MCDecayModeFilterAll.V2_tau_nutau = cms.bool(True)
MCDecayModeFilterAll.V2_d_u = cms.bool(True)
MCDecayModeFilterAll.V2_d_c = cms.bool(True)
MCDecayModeFilterAll.V2_s_u = cms.bool(True)
MCDecayModeFilterAll.V2_s_c = cms.bool(True)
MCDecayModeFilterAll.V2_b_u = cms.bool(True)
MCDecayModeFilterAll.V2_b_c = cms.bool(True)






# --- THE ELECTRON SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFElectronSelector_cfi import *
VBFSelectedElectronsAll = VBFSelectedElectrons.clone()
VBFSelectedElectronsRefAll = VBFSelectedElectronsRef.clone()

VBFSelectedElectronsAll.ptMin  = cms.double(5.)
VBFSelectedElectronsAll.etaMin = cms.double(-2.5)
VBFSelectedElectronsAll.etaMax = cms.double(+2.5)
VBFSelectedElectronsAll.doRefCheck = cms.bool(False)

VBFSelectedElectronsRefAll.ptMin  = cms.double(5.)
VBFSelectedElectronsRefAll.etaMin = cms.double(-2.5)
VBFSelectedElectronsRefAll.etaMax = cms.double(+2.5)
VBFSelectedElectronsRefAll.doRefCheck = cms.bool(False)


# --- THE ELECTRON ISOLATOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFElectronIsolator_cfi import *
VBFIsolatedElectronsAll = VBFIsolatedElectrons.clone()
VBFIsolatedElectronsRefAll = VBFIsolatedElectronsRef.clone()

VBFIsolatedElectronsAll.tkIsoCut = cms.double(0.5)
VBFIsolatedElectronsAll.doRefCheck = cms.bool(True)
VBFIsolatedElectronsAll.srcElectronsRef = cms.InputTag("VBFSelectedElectronsRefAll")

VBFIsolatedElectronsRefAll.tkIsoCut = cms.double(0.5)
VBFIsolatedElectronsRefAll.doRefCheck = cms.bool(True)
VBFIsolatedElectronsRefAll.srcElectronsRef = cms.InputTag("VBFSelectedElectronsRefAll")


# --- THE ELECTRON ID SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFElectronIdSelector_cfi import *
VBFIdSelectedElectronsAll = VBFIdSelectedElectrons.clone()
VBFIdSelectedElectronsRefAll = VBFIdSelectedElectronsRef.clone()

VBFIdSelectedElectronsAll.doRefCheck = cms.bool(True)
VBFIdSelectedElectronsAll.srcElectronIdValues = cms.InputTag("eidRobustLoose")
VBFIdSelectedElectronsAll.srcElectronsRef = cms.InputTag("VBFIsolatedElectronsRefAll")

VBFIdSelectedElectronsRefAll.doRefCheck = cms.bool(True)
VBFIdSelectedElectronsRefAll.srcElectronIdValues = cms.InputTag("eidRobustLoose")
VBFIdSelectedElectronsRefAll.srcElectronsRef = cms.InputTag("VBFIsolatedElectronsRefAll")






# --- THE MUON SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFMuonSelector_cfi import *
VBFSelectedMuonsAll = VBFSelectedMuons.clone()
VBFSelectedMuonsRefAll = VBFSelectedMuonsRef.clone()

VBFSelectedMuonsAll.ptMin  = cms.double(5.)
VBFSelectedMuonsAll.etaMin = cms.double(-2.7)
VBFSelectedMuonsAll.etaMax = cms.double(+2.7)

VBFSelectedMuonsRefAll.ptMin  = cms.double(5.)
VBFSelectedMuonsRefAll.etaMin = cms.double(-2.7)
VBFSelectedMuonsRefAll.etaMax = cms.double(+2.7)
            

# --- THE MUON ISOLATOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFMuonIsolator_cfi import *
VBFIsolatedMuonsAll = VBFIsolatedMuons.clone()
VBFIsolatedMuonsRefAll = VBFIsolatedMuonsRef.clone()

VBFIsolatedMuonsAll.tkIsoCut = cms.double(0.5)
VBFIsolatedMuonsAll.doRefCheck = cms.bool(True)
VBFIsolatedMuonsAll.srcMuonsRef = cms.InputTag("VBFSelectedMuonsRefAll")

VBFIsolatedMuonsRefAll.tkIsoCut = cms.double(0.5)
VBFIsolatedMuonsRefAll.doRefCheck = cms.bool(True)
VBFIsolatedMuonsRefAll.srcMuonsRef = cms.InputTag("VBFSelectedMuonsRefAll")






# --- THE LEPTON FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPtMinLeptonCountFilter_cfi import *
VBFPtMinLeptonCountFilterAll = VBFPtMinLeptonCountFilter.clone()

VBFPtMinLeptonCountFilterAll.srcElectrons = cms.InputTag("VBFIdSelectedElectronsAll")
VBFPtMinLeptonCountFilterAll.srcMuons = cms.InputTag("VBFIsolatedMuonsAll")
VBFPtMinLeptonCountFilterAll.ptMin  = cms.double(10.)
VBFPtMinLeptonCountFilterAll.etaMin = cms.double(-2.7)
VBFPtMinLeptonCountFilterAll.etaMax = cms.double(+2.7)
VBFPtMinLeptonCountFilterAll.minNumber = cms.int32(1)






# --- Tip/Lip/3Dip producer --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFLeptonTipLipProducer_cfi import *
from HiggsAnalysis.VBFHiggsToVV.VBFLepton3DipProducer_cfi import *

VBFLeptonTipLipProducerAll = VBFLeptonTipLipProducer.clone()
VBFLepton3DipProducerAll = VBFLepton3DipProducer.clone()






VBFPreselectionSequenceAll = cms.Sequence(
    
    MCDecayModeFilterAll +
    
    

    # electrons
    VBFSelectedElectronsAll +
    VBFSelectedElectronsRefAll +
    
    VBFIsolatedElectronsAll +
    VBFIsolatedElectronsRefAll +
    
    VBFIdSelectedElectronsAll +
    VBFIdSelectedElectronsRefAll +
    
    

    # muons
    VBFSelectedMuonsAll +
    VBFSelectedMuonsRefAll +
    
    VBFIsolatedMuonsAll +
    VBFIsolatedMuonsRefAll +



    # lepton filter
    VBFPtMinLeptonCountFilterAll + 


    
    # impact parameter producers
    VBFLeptonTipLipProducerAll +
    VBFLepton3DipProducerAll 
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
