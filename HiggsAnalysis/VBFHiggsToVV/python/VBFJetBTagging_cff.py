import FWCore.ParameterSet.Config as cms



# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# B-TAGGING
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

# b-tagging general configuration
from RecoJets.JetAssociationProducers.ic5JetTracksAssociatorAtVertex_cfi import *
from RecoBTag.Configuration.RecoBTag_cff import *






# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# CALO JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

# create a new jets and tracks association
VBFL2L3Ak5CaloJetsJetTracksAssociatorAtVertex = ic5JetTracksAssociatorAtVertex.clone()
VBFL2L3Ak5CaloJetsJetTracksAssociatorAtVertex.jets = "VBFL2L3Ak5CaloJets"
VBFL2L3Ak5CaloJetsJetTracksAssociatorAtVertex.tracks = "generalTracks"

VBFL2L3Ak5CaloJetsImpactParameterTagInfos = impactParameterTagInfos.clone()
VBFL2L3Ak5CaloJetsImpactParameterTagInfos.jetTracks = "VBFL2L3Ak5CaloJetsJetTracksAssociatorAtVertex"
VBFL2L3Ak5CaloJetsTrackCountingHighEffBJetTags = trackCountingHighEffBJetTags.clone()
VBFL2L3Ak5CaloJetsTrackCountingHighEffBJetTags.tagInfos = cms.VInputTag( cms.InputTag("VBFL2L3Ak5CaloJetsImpactParameterTagInfos") )
VBFL2L3Ak5CaloJetsTrackCountingHighPurBJetTags = trackCountingHighPurBJetTags.clone()
VBFL2L3Ak5CaloJetsTrackCountingHighPurBJetTags.tagInfos = cms.VInputTag( cms.InputTag("VBFL2L3Ak5CaloJetsImpactParameterTagInfos") )
VBFL2L3Ak5CaloJetsJetProbabilityBJetTags = jetProbabilityBJetTags.clone()
VBFL2L3Ak5CaloJetsJetProbabilityBJetTags.tagInfos = cms.VInputTag( cms.InputTag("VBFL2L3Ak5CaloJetsImpactParameterTagInfos") )
VBFL2L3Ak5CaloJetsJetBProbabilityBJetTags = jetBProbabilityBJetTags.clone()
VBFL2L3Ak5CaloJetsJetBProbabilityBJetTags.tagInfos = cms.VInputTag( cms.InputTag("VBFL2L3Ak5CaloJetsImpactParameterTagInfos") )


# prepare a path running the new modules
VBFL2L3Ak5CaloJetsJetTracksAssociator = cms.Sequence(
    VBFL2L3Ak5CaloJetsJetTracksAssociatorAtVertex
    )

VBFL2L3Ak5CaloJetsJetBtaggingIP = cms.Sequence(
    VBFL2L3Ak5CaloJetsImpactParameterTagInfos * (
        VBFL2L3Ak5CaloJetsTrackCountingHighEffBJetTags +
        VBFL2L3Ak5CaloJetsTrackCountingHighPurBJetTags +
        VBFL2L3Ak5CaloJetsJetProbabilityBJetTags +
        VBFL2L3Ak5CaloJetsJetBProbabilityBJetTags
        )
    )






# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
# PF JETS
# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

# create a new jets and tracks association
VBFL2L3Ak5PFJetsJetTracksAssociatorAtVertex = ic5JetTracksAssociatorAtVertex.clone()
VBFL2L3Ak5PFJetsJetTracksAssociatorAtVertex.jets = "VBFL2L3Ak5PFJets"
VBFL2L3Ak5PFJetsJetTracksAssociatorAtVertex.tracks = "generalTracks"

VBFL2L3Ak5PFJetsImpactParameterTagInfos = impactParameterTagInfos.clone()
VBFL2L3Ak5PFJetsImpactParameterTagInfos.jetTracks = "VBFL2L3Ak5PFJetsJetTracksAssociatorAtVertex"
VBFL2L3Ak5PFJetsTrackCountingHighEffBJetTags = trackCountingHighEffBJetTags.clone()
VBFL2L3Ak5PFJetsTrackCountingHighEffBJetTags.tagInfos = cms.VInputTag( cms.InputTag("VBFL2L3Ak5PFJetsImpactParameterTagInfos") )
VBFL2L3Ak5PFJetsTrackCountingHighPurBJetTags = trackCountingHighPurBJetTags.clone()
VBFL2L3Ak5PFJetsTrackCountingHighPurBJetTags.tagInfos = cms.VInputTag( cms.InputTag("VBFL2L3Ak5PFJetsImpactParameterTagInfos") )
VBFL2L3Ak5PFJetsJetProbabilityBJetTags = jetProbabilityBJetTags.clone()
VBFL2L3Ak5PFJetsJetProbabilityBJetTags.tagInfos = cms.VInputTag( cms.InputTag("VBFL2L3Ak5PFJetsImpactParameterTagInfos") )
VBFL2L3Ak5PFJetsJetBProbabilityBJetTags = jetBProbabilityBJetTags.clone()
VBFL2L3Ak5PFJetsJetBProbabilityBJetTags.tagInfos = cms.VInputTag( cms.InputTag("VBFL2L3Ak5PFJetsImpactParameterTagInfos") )


# prepare a path running the new modules
VBFL2L3Ak5PFJetsJetTracksAssociator = cms.Sequence(
    VBFL2L3Ak5PFJetsJetTracksAssociatorAtVertex
    )

VBFL2L3Ak5PFJetsJetBtaggingIP = cms.Sequence(
    VBFL2L3Ak5PFJetsImpactParameterTagInfos * (
        VBFL2L3Ak5PFJetsTrackCountingHighEffBJetTags +
        VBFL2L3Ak5PFJetsTrackCountingHighPurBJetTags +
        VBFL2L3Ak5PFJetsJetProbabilityBJetTags +
        VBFL2L3Ak5PFJetsJetBProbabilityBJetTags
        )
    )
