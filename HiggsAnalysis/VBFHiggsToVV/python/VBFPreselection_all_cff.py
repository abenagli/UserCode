import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# all
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----



# --- ==================== --- --- --- --- --- --- --- --- --- --- ---
# --- THE VBF PRESELECTION --- --- --- --- --- --- --- --- --- --- ---
# --- ==================== --- --- --- --- --- --- --- --- --- --- ---

# --- THE ELECTRON SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFElectronSelector_cfi import *
VBFSelectedElectronsAll = VBFSelectedElectrons.clone()
VBFSelectedElectronsRefAll = VBFSelectedElectronsRef.clone()



## --- THE ELECTRON ISOLATOR --- --- --- --- --- --- --- --- --- --- ---
#
#from HiggsAnalysis.VBFHiggsToVV.VBFElectronIsolator_cfi import *
#VBFIsolatedElectronsAll = VBFIsolatedElectrons.clone()
#VBFIsolatedElectronsRefAll = VBFIsolatedElectronsRef.clone()
#
#VBFIsolatedElectronsAll.doRefCheck = cms.bool(True)
#VBFIsolatedElectronsAll.srcElectronsRef = cms.InputTag("VBFSelectedElectronsRefAll")
#
#VBFIsolatedElectronsRefAll.doRefCheck = cms.bool(True)
#VBFIsolatedElectronsRefAll.srcElectronsRef = cms.InputTag("VBFSelectedElectronsRefAll")



## --- THE ELECTRON ID SELECTOR --- --- --- --- --- --- --- --- --- --- ---
#
#from HiggsAnalysis.VBFHiggsToVV.VBFElectronIdSelector_cfi import *
#VBFIdSelectedElectronsAll = VBFIdSelectedElectrons.clone()
#VBFIdSelectedElectronsRefAll = VBFIdSelectedElectronsRef.clone()
#
#VBFIdSelectedElectronsAll.doRefCheck = cms.bool(True)
#VBFIdSelectedElectronsAll.srcElectronsRef = cms.InputTag("VBFIsolatedElectronsRefAll")
#
#VBFIdSelectedElectronsRefAll.doRefCheck = cms.bool(True)
#VBFIdSelectedElectronsRefAll.srcElectronsRef = cms.InputTag("VBFIsolatedElectronsRefAll")



# --- THE ELECTRON CONVERSION REMOVAL SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFElectronConversionRemovalSelector_cfi import *
VBFConversionRemovalSelectedElectronsAll = VBFConversionRemovalSelectedElectrons.clone()
VBFConversionRemovalSelectedElectronsRefAll = VBFConversionRemovalSelectedElectronsRef.clone()

VBFConversionRemovalSelectedElectronsAll.doRefCheck = cms.bool(True)
VBFConversionRemovalSelectedElectronsAll.srcElectronsRef = cms.InputTag("VBFSelectedElectronsRefAll")

VBFConversionRemovalSelectedElectronsRefAll.doRefCheck = cms.bool(True)
VBFConversionRemovalSelectedElectronsRefAll.srcElectronsRef = cms.InputTag("VBFSelectedElectronsRefAll")



# --- THE ELECTRON SPIKE REMOVAL SELECTOR --- --- --- --- --- --- --- --- --- --- ---
#
#from HiggsAnalysis.VBFHiggsToVV.VBFElectronSpikeRemovalSelector_cfi import *
#VBFSpikeRemovalSelectedElectronsAll = VBFSpikeRemovalSelectedElectrons.clone()
#VBFSpikeRemovalSelectedElectronsRefAll = VBFSpikeRemovalSelectedElectronsRef.clone()
#
#VBFSpikeRemovalSelectedElectronsAll.doRefCheck = cms.bool(True)
#VBFSpikeRemovalSelectedElectronsAll.srcElectronsRef = cms.InputTag("VBFConversionRemovalSelectedElectronsRefAll")
#
#VBFSpikeRemovalSelectedElectronsRefAll.doRefCheck = cms.bool(True)
#VBFSpikeRemovalSelectedElectronsRefAll.srcElectronsRef = cms.InputTag("VBFConversionRemovalSelectedElectronsRefAll")












# --- THE MUON SELECTOR --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFMuonSelector_cfi import *
VBFSelectedMuonsAll = VBFSelectedMuons.clone()
VBFSelectedMuonsRefAll = VBFSelectedMuonsRef.clone()



## --- THE MUON ISOLATOR --- --- --- --- --- --- --- --- --- --- ---
#
#from HiggsAnalysis.VBFHiggsToVV.VBFMuonIsolator_cfi import *
#VBFIsolatedMuonsAll = VBFIsolatedMuons.clone()
#VBFIsolatedMuonsRefAll = VBFIsolatedMuonsRef.clone()
#
#VBFIsolatedMuonsAll.doRefCheck = cms.bool(True)
#VBFIsolatedMuonsAll.srcMuonsRef = cms.InputTag("VBFSelectedMuonsRefAll")
#
#VBFIsolatedMuonsRefAll.doRefCheck = cms.bool(True)
#VBFIsolatedMuonsRefAll.srcMuonsRef = cms.InputTag("VBFSelectedMuonsRefAll")



## --- THE MUON ID SELECTOR --- --- --- --- --- --- --- --- --- --- ---
#
#from HiggsAnalysis.VBFHiggsToVV.VBFMuonIdSelector_cfi import *
#VBFIdSelectedMuonsAll = VBFIdSelectedMuons.clone()
#VBFIdSelectedMuonsRefAll = VBFIdSelectedMuonsRef.clone()
#
#VBFIdSelectedMuonsAll.doRefCheck = cms.bool(True)
#VBFIdSelectedMuonsAll.srcMuonsRef = cms.InputTag("VBFIsolatedMuonsRefAll")
#
#VBFIdSelectedMuonsRefAll.doRefCheck = cms.bool(True)
#VBFIdSelectedMuonsRefAll.srcMuonsRef = cms.InputTag("VBFIsolatedMuonsRefAll")
                





# --- THE LEPTON FILTER --- --- --- --- --- --- --- --- --- --- ---

from HiggsAnalysis.VBFHiggsToVV.VBFPtMinLeptonCountFilter_cfi import *
VBFPtMinLeptonCountFilterAll = VBFPtMinLeptonCountFilter.clone()

VBFPtMinLeptonCountFilterAll.srcElectrons = cms.InputTag("VBFConversionRemovalSelectedElectronsAll")
VBFPtMinLeptonCountFilterAll.srcMuons = cms.InputTag("VBFSelectedMuonsAll")
VBFPtMinLeptonCountFilterAll.ptMin  = cms.double(20.)
VBFPtMinLeptonCountFilterAll.etaMin = cms.double(-2.5)
VBFPtMinLeptonCountFilterAll.etaMax = cms.double(+2.5)
VBFPtMinLeptonCountFilterAll.minNumber = cms.int32(1)











# --- Tip/Lip/3Dip producer --- --- --- --- --- --- --- --- --- --- ---

#from HiggsAnalysis.VBFHiggsToVV.VBFLeptonTipLipProducer_cfi import *
from HiggsAnalysis.VBFHiggsToVV.VBFLepton3DipProducer_cfi import *

#VBFLeptonTipLipProducerAll = VBFLeptonTipLipProducer.clone()
VBFLepton3DipProducerAll = VBFLepton3DipProducer.clone()












VBFPreselectionSequenceAll = cms.Sequence(
    
    # electrons
    VBFSelectedElectronsAll +
    VBFSelectedElectronsRefAll +

    #VBFIsolatedElectronsAll +
    #VBFIsolatedElectronsRefAll +
    
    #VBFIdSelectedElectronsAll +
    #VBFIdSelectedElectronsRefAll +
    
    VBFConversionRemovalSelectedElectronsAll +
    VBFConversionRemovalSelectedElectronsRefAll +

    #VBFSpikeRemovalSelectedElectronsAll +
    #VBFSpikeRemovalSelectedElectronsRefAll +
    
    
    
    # muons
    VBFSelectedMuonsAll +
    VBFSelectedMuonsRefAll +
    
    #VBFIsolatedMuonsAll +
    #VBFIsolatedMuonsRefAll +

    #VBFIdSelectedMuonsAll +
    #VBFIdSelectedMuonsRefAll +
    


    # lepton filter
    VBFPtMinLeptonCountFilterAll + 


    
    # impact parameter producers
    #VBFLeptonTipLipProducerAll +
    VBFLepton3DipProducerAll 
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
