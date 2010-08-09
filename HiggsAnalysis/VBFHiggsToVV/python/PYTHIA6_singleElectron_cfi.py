import FWCore.ParameterSet.Config as cms

from Configuration.Generator.PythiaUESettings_cfi import *

generator = cms.EDProducer(
    "Pythia6PtGun",
    PGunParameters = cms.PSet(
        ParticleID = cms.vint32(11),
        AddAntiParticle = cms.bool(False),
        MinPhi = cms.double(-3.14159265359),
        MaxPhi = cms.double(3.14159265359),
        MinPt = cms.double(5.0),
        MaxPt = cms.double(200.),
        MinEta = cms.double(-2.5),
        MaxEta = cms.double(2.5)
        ),
    PythiaParameters = cms.PSet(
        pythiaUESettingsBlock,
         ),
    parameterSets = cms.vstring(
        'pythiaUESettings',
         )
    )

configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    name = cms.untracked.string('$Source: /local/projects/CMSSW/rep/CMSSW/Configuration/Generator/PYTHIA6_singleElectron_cfi.py,v $'),
    annotation = cms.untracked.string('PYTHIA6 - single electron particle gun')
    )
