import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# MET CORRECTOR
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

from JetMETCorrections.Type1MET.MetType1Corrections_cff import *






VBFType1Ak5CaloMet = metJESCorAK5CaloJet.clone(
    inputUncorMetLabel = "corMetGlobalMuons"
    )

VBFType1Ak5CaloMetSequence = cms.Sequence(
    VBFType1Ak5CaloMet
    )
