import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JET CORRECTOR
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

#from JetMETCorrections.Configuration.L2L3Corrections_Summer09_cff import *
#from JetMETCorrections.Configuration.L2L3Corrections_Summer09_7TeV_ReReco332_cff import *
from JetMETCorrections.Configuration.DefaultJEC_cff import *
#from JetMETCorrections.Configuration.DefaultJEC_cff import *



VBFL2L3Ak5CaloJets = ak5CaloJetsL2L3.clone()

VBFL2L3Ak5CaloJetsSequence = cms.Sequence(
        VBFL2L3Ak5CaloJets
        )






VBFL2L3Ak5PFJets = ak5PFJetsL2L3.clone()

VBFL2L3Ak5PFJetsSequence = cms.Sequence(
        VBFL2L3Ak5PFJets
        )
