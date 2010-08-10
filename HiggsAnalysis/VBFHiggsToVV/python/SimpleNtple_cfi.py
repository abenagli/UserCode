import FWCore.ParameterSet.Config as cms

SimpleNtple = cms.EDAnalyzer(
    "SimpleNtple",
    
     HLTTag           = cms.InputTag("TriggerResults::HLT"),

     PVTag           = cms.InputTag("offlinePrimaryVertices"),
     
     MuTag           = cms.InputTag("muons"),
     Mu3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducer:Mu3DipSignificanceMap"),
     #MuTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:MuTipSignificanceMap"),
     #MuLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:MuLipSignificanceMap"),
     
     EleTag          = cms.InputTag("gsfElectrons"),
     Ele3DipSignificanceTag = cms.InputTag("VBFLepton3DipProducer:Ele3DipSignificanceMap"),
     #EleTipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:EleTipSignificanceMap"),
     #EleLipSignificanceTag = cms.InputTag("VBFLeptonTipLipProducer:EleLipSignificanceMap"),
     
     TracksTag       = cms.InputTag("generalTracks"),

     JetTag          = cms.InputTag("ak5CaloJets"),
     JetTag_forID    = cms.InputTag("ak5CaloJets"),
     JetIDTag        = cms.InputTag("ak5JetID"),
     genJetTag       = cms.InputTag("ak5GenJets"),

     TrackCountingHighEffBJetTagsTag = cms.InputTag("trackCountingHighEffBJetTags"),
     TrackCountingHighPurBJetTagsTag = cms.InputTag("trackCountingHighPurBJetTags"),
     JetProbabilityBJetTagsTag       = cms.InputTag("jetProbabilityBJetTags"),
     JetBProbabilityBJetTagsTag      = cms.InputTag("jetBProbabilityBJetTags"),

     JetChargeTag = cms.InputTag("VBFCaloJetsCharge"),

     MetTag          = cms.InputTag("corMetGlobalMuons"),
     Type1MetTag     = cms.InputTag("VBFType1Ak5CaloMet"),
     TcMetTag        = cms.InputTag("tcMet"),         
     PFMetTag        = cms.InputTag("pfMet"),         
     genMetTag       = cms.InputTag("genMetTrue"),

     MCtruthTag      = cms.InputTag("genParticles"),

     doMuRefCheck = cms.untracked.bool (False),
     MuRefTag = cms.InputTag("MuRef"),
     doEleRefCheck = cms.untracked.bool (False),
     EleRefTag = cms.InputTag("EleRef"),
     doJetRefCheck = cms.untracked.bool (False),
     JetRefTag = cms.InputTag("JetRef"),

     saveHLT          = cms.untracked.bool (True),
     savePV           = cms.untracked.bool (True),     
     saveTrack        = cms.untracked.bool (True), 
     saveMu           = cms.untracked.bool (True),
     saveEle          = cms.untracked.bool (True),
     saveJet          = cms.untracked.bool (True),
     savePFJet        = cms.untracked.bool (False),
     saveJetBTagging  = cms.untracked.bool (True),
     saveMet          = cms.untracked.bool (True),
     saveGenJet       = cms.untracked.bool (True),
     saveGenMet       = cms.untracked.bool (True),                        
     saveMCPtHat      = cms.untracked.bool (True),
     saveMCHiggs      = cms.untracked.bool (True),    
     saveMCHiggsDecay = cms.untracked.bool (True),    
     saveMCEle        = cms.untracked.bool (True),
     saveMCMu         = cms.untracked.bool (True),    
     
        
     verbosity = cms.untracked.bool(False),
     eventType = cms.untracked.int32(0) 
)
