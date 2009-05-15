#!/usr/bin/env python

# Usage: ./getFastSimHLTcff.py <Version from ConfDB> <Name of cff file> <optional L1 Menu> <optional subset of paths>

import sys
import os
import commands
import getopt
import fileinput

def usage():
    print "Usage: ./getFastSimHLTcff.py <Version from ConfDB> <Name of cff> <optional L1 Menu> <optional subset of paths>"
    print "       Default L1 Menu: L1Menu2008_2E30"
    print "       Define subset of paths as comma-separated list: a,b,c (Default is to run all paths)" 
    sys.exit(1)

argc = len(sys.argv)
blockName = "None"
#cfgName = "None"
usePaths = "All"
L1Menu   = "L1Menu2008_2E30"

if argc == 3:
    dbName  = sys.argv[1]
    cffName = sys.argv[2]
elif argc == 4:
    dbName  = sys.argv[1]
    cffName = sys.argv[2]
    blockName = sys.argv[3]
#elif argc == 5:
#    dbName  = sys.argv[1]
#    cffName = sys.argv[2]
#    blockName = sys.argv[3]
#    cfgName = sys.argv[4]
elif argc == 5:
    dbName  = sys.argv[1]
    cffName = sys.argv[2]
    blockName = sys.argv[3]
#    cfgName = sys.argv[4]
    L1Menu  = sys.argv[4]
elif argc == 6:
    dbName  = sys.argv[1]
    cffName = sys.argv[2]
    blockName = sys.argv[3]
#    cfgName = sys.argv[4]
    L1Menu   = sys.argv[4]
    usePaths = sys.argv[5]
else:
    usage()

if os.path.exists(cffName):
    print cffName, "already exists.  WARNING: No new file created!"
else:
    essources = "--essources "
    #--- Recall that data is not unpacked by FastSim...can remove the cabling
    essources += "-DTCabling,"
    essources += "-cscPackingCabling,"
    essources += "-cscUnpackingCabling,"
    essources += "-RPCCabling,"
    essources += "-siPixelCabling,"
    essources += "-siStripFedCabling,"
    essources += "-siPixelGainCalibration,"
    essources += "-siPixelLorentzAngle,"
    essources += "-siStripApvGain,"
    essources += "-siStripLorentzAngle,"
    essources += "-siStripNoise,"
    # Start of list removing all ESSources
    essources += "-BeamSpotEarlyCollision,"
    essources += "-GlobalPosition,"
    essources += "-cscConditions,"
    essources += "-ecalConditions,"
    essources += "-es_pool,"
    essources += "-maps_frontier,"
    essources += "-muonAlignment,"
    essources += "-trackProbabilityFrontierCond,"
    essources += "-trackerAlignment,"
    # essources += "-,"
    # essources += "-,"
    # essources += "-,"
    # essources += "-,"
    # essources += "-,"
    # End of list removing all ESSources
    essources += "-es_hardcode,"
    essources += "-tpparams11,"
    essources += "-tpparams10,"
    essources += "-tpparams9,"
    essources += "-tpparams8,"
    essources += "-tpparams7,"
    essources += "-tpparams6,"
    essources += "-tpparams5,"
    essources += "-tpparams4,"
    essources += "-tpparams3,"
    essources += "-tpparams2,"
    essources += "-tpparams,"
    essources += "-magfield,"
    essources += "-l1GctParamsRecords,"
    essources += "-l1GctJcPosParsRecords,"
    essources += "-l1GctJcNegParsRecords,"
    essources += "-l1GctConfigRecords,"
    essources += "-l1CaloGeomRecordSource,"
    essources += "-jetrcdsrc,"
    essources += "-emrcdsrc,"
    essources += "-eegeom,"
    essources += "-XMLIdealGeometryESSource,"
    essources += "-L1MuTriggerScalesRcdSource,"
    essources += "-L1MuGMTScalesRcdSource,"
    essources += "-L1MuTriggerPtScaleRcdSource,"
    essources += "-L1GtTriggerMenuRcdSource,"
    essources += "-L1GtTriggerMaskVetoTechTrigRcdSource,"
    essources += "-L1GtTriggerMaskVetoAlgoTrigRcdSource,"
    essources += "-L1GtTriggerMaskTechTrigRcdSource,"
    essources += "-L1GtTriggerMaskRcdSource,"
    essources += "-L1GtTriggerMaskAlgoTrigRcdSource,"
    essources += "-L1GtStableParametersRcdSource,"
    essources += "-L1GtPrescaleFactorsTechTrigRcdSource,"
    essources += "-L1GtPrescaleFactorsRcdSource,"
    essources += "-L1GtPrescaleFactorsAlgoTrigRcdSource,"
    essources += "-L1GtParametersRcdSource,"
    essources += "-L1GtBoardMapsRcdSource,"
    essources += "-HepPDTESSource,"
    essources += "-BTauMVAJetTagComputerRecord,"
    essources += "-BTagRecord,"
    essources += "-GlobalTag"

    esmodules = "--esmodules "
    esmodules += "-ttrhbwr,"
    esmodules += "-ttrhbwor,"
    esmodules += "-myTTRHBuilderWithoutAngle,"
    esmodules += "-myTTRHBuilderWithoutAngle4PixelPairs,"
    esmodules += "-myTTRHBuilderWithoutAngle4PixelTriplets,"
    esmodules += "-TTRHBuilderPixelOnly,"
    esmodules += "-WithTrackAngle,"
    esmodules += "-trajectoryCleanerBySharedHits,"
    esmodules += "-trackCounting3D3rd,"
    esmodules += "-trackCounting3D2nd,"
    esmodules += "-navigationSchoolESProducer,"
    esmodules += "-muonCkfTrajectoryFilter,"
    esmodules += "-l1GtTriggerMenuXml,"
    esmodules += "-l1GtTriggerMaskVetoTechTrig,"
    esmodules += "-l1GtTriggerMaskVetoAlgoTrig,"
    esmodules += "-l1GtTriggerMaskTechTrig,"
    esmodules += "-l1GtTriggerMaskAlgoTrig,"
    esmodules += "-l1GtStableParameters,"
    esmodules += "-l1GtPrescaleFactorsTechTrig,"
    esmodules += "-l1GtPrescaleFactorsAlgoTrig,"
    esmodules += "-l1GtParameters,"
    esmodules += "-l1GtFactors,"
    esmodules += "-l1GtBoardMaps,"
    esmodules += "-l1CaloScales,"
    esmodules += "-l1CaloGeometry,"
    esmodules += "-L1MuTriggerPtScale,"
    esmodules += "-jetProbability,"
    esmodules += "-jetBProbability,"
    esmodules += "-impactParameterMVAComputer,"
    esmodules += "-hcal_db_producer,"
    # esmodules += "-egammaHLTChi2MeasurementEstimatorESProducer,"
    esmodules += "-cosmicsNavigationSchoolESProducer,"
    esmodules += "-compositeTrajectoryFilterESProducer,"
    esmodules += "-ckfBaseTrajectoryFilter,"
    esmodules += "-beamHaloNavigationSchoolESProducer,"
    esmodules += "-ZdcHardcodeGeometryEP,"
    esmodules += "-VolumeBasedMagneticFieldESProducer,"
    esmodules += "-TransientTrackBuilderESProducer,"
    # esmodules += "-TrajectoryBuilderForPixelMatchElectronsL1NonIsoLargeWindow,"
    # esmodules += "-TrajectoryBuilderForPixelMatchElectronsL1NonIso,"
    # esmodules += "-TrajectoryBuilderForPixelMatchElectronsL1IsoLargeWindow,"
    # esmodules += "-TrajectoryBuilderForPixelMatchElectronsL1Iso,"
    esmodules += "-TrackerRecoGeometryESProducer,"
    esmodules += "-TrackerGeometricDetESModule,"
    esmodules += "-TrackerDigiGeometryESModule,"
    esmodules += "-StripCPEfromTrackAngleESProducer,"
    esmodules += "-StripCPEESProducer,"
    esmodules += "-SteppingHelixPropagatorOpposite,"
    esmodules += "-SteppingHelixPropagatorAny,"
    esmodules += "-SteppingHelixPropagatorAlong,"
    esmodules += "-SmootherRK,"
    esmodules += "-SmartPropagatorRK,"
    esmodules += "-SmartPropagatorOpposite,"
    esmodules += "-SmartPropagatorAnyRK,"
    esmodules += "-SmartPropagatorAnyOpposite,"
    esmodules += "-SmartPropagatorAny,"
    esmodules += "-SmartPropagator,"
    esmodules += "-SiStripRecHitMatcherESProducer,"
    esmodules += "-SiStripGainESProducer,"
    esmodules += "-RungeKuttaTrackerPropagator,"
    esmodules += "-RPCGeometryESModule,"
    esmodules += "-RKTrackerPropagator,"
    esmodules += "-PixelCPEParmErrorESProducer,"
    esmodules += "-OppositeMaterialPropagator,"
    esmodules += "-MuonTransientTrackingRecHitBuilderESProducer,"
    esmodules += "-MuonNumberingInitialization,"
    esmodules += "-MuonDetLayerGeometryESProducer,"
    esmodules += "-MuonCkfTrajectoryBuilder,"
    esmodules += "-MeasurementTracker,"
    esmodules += "-MaterialPropagator,"
    esmodules += "-L3MuKFFitter,"
    esmodules += "-L1MuTriggerScales,"
    esmodules += "-L1MuGMTScales,"
    esmodules += "-L1GctConfigProducers,"
    esmodules += "-KFUpdatorESProducer,"
    esmodules += "-KFTrajectorySmoother,"
    esmodules += "-KFTrajectoryFitter,"
    esmodules += "-KFSmootherForRefitOutsideIn,"
    esmodules += "-KFSmootherForRefitInsideOut,"
    esmodules += "-KFSmootherForMuonTrackLoader,"
    esmodules += "-KFFittingSmoother,"
    esmodules += "-KFFitterForRefitOutsideIn,"
    esmodules += "-KFFitterForRefitInsideOut,"
    esmodules += "-HcalTopologyIdealEP,"
    esmodules += "-HcalHardcodeGeometryEP,"
    esmodules += "-GroupedCkfTrajectoryBuilder,"
    esmodules += "-GlobalTrackingGeometryESProducer,"
    esmodules += "-FittingSmootherRK,"
    esmodules += "-FitterRK,"
    esmodules += "-EcalTrigTowerConstituentsMapBuilder,"
    esmodules += "-EcalTrigPrimESProducer,"
    esmodules += "-EcalPreshowerGeometryEP,"
    esmodules += "-EcalLaserCorrectionService,"
    esmodules += "-EcalEndcapGeometryEP,"
    esmodules += "-EcalElectronicsMappingBuilder,"
    esmodules += "-EcalBarrelGeometryEP,"
    esmodules += "-DTGeometryESModule,"
    esmodules += "-CkfTrajectoryBuilder,"
    esmodules += "-Chi2MeasurementEstimator,"
    esmodules += "-Chi2EstimatorForRefit,"
    esmodules += "-Chi2EstimatorForMuonTrackLoader,"
    esmodules += "-Chi2EstimatorForL3Refit,"
    esmodules += "-CaloTowerHardcodeGeometryEP,"
    esmodules += "-CaloTowerConstituentsMapBuilder,"
    esmodules += "-CaloTopologyBuilder,"
    esmodules += "-CaloGeometryBuilder,"
    esmodules += "-CSCGeometryESModule"

    #--- Define blocks for ElectronPixelSeeds ---#
    #--- Please see FastSimulation/EgammaElectronAlgos/data/pixelMatchElectronL1[Non]Iso[LargeWindow]SequenceForHLT.cff ---#
    blocks = "--blocks "
    blocks += "hltL1NonIsoLargeWindowElectronPixelSeeds::SeedConfiguration,"
    blocks += "hltL1IsoLargeWindowElectronPixelSeeds::SeedConfiguration,"
    blocks += "hltL1NonIsoStartUpElectronPixelSeeds::SeedConfiguration,"
    blocks += "hltL1NonIsoElectronPixelSeeds::SeedConfiguration,"
    blocks += "hltL1IsoStartUpElectronPixelSeeds::SeedConfiguration,"
    blocks += "hltL1IsoElectronPixelSeeds::SeedConfiguration,"
    blocks += "hltL3TrajectorySeed::MuonTrackingRegionBuilder"


    #--- Some notes about removed/redefined modules ---#
    #--- hltGtDigis --> FastSim uses gtDigis from L1Trigger/Configuration/data/L1Emulator.cff
    #--- hltMuon[CSC/DT/RPC]Digis defined (as muon[CSC/DT/RPC]Digis) through FastSimulation/Configuration/data/FamosSequences.cff
    #--- hltL1[Non]IsoElectronPixelSeeds -> defined in FastSimulation/EgammaElectronAlgos/data/pixelMatchElectronL1[Non]IsoSequenceForHLT.cff
    #--- hltCtfL1[Non]IsoWithMaterialTracks, hltCkfL1[Non]IsoTrackCandidates -> defined in FastSimulation/EgammaElectronAlgos/data/pixelMatchElectronL1[Non]IsoSequenceForHLT.cff
    #--- hltL1[Non]IsoLargeWindowElectronPixelSeeds -> defined in FastSimulation/EgammaElectronAlgos/data/pixelMatchElectronL1[Non]IsoLargeWindowSequenceForHLT.cff
    #--- hltCtfL1[Non]IsoLargeWindowWithMaterialTracks, hltCkfL1[Non]IsoLargeWindowTrackCandidates -> defined in FastSimulation/EgammaElectronAlgos/data/pixelMatchElectronL1[Non]IsoLargeWindowSequenceForHLT.cff
    #--- hltEcalPreshowerDigis,hltEcalRegionalJetsFEDs,hltEcalRegionalJetsDigis,hltEcalRegionalJetsWeightUncalibRecHit,hltEcalRegionalJetsRecHitTmp redefined as dummyModules in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- hltEcalRegional[Muons/Rest]FEDs,hltEcalRegional[Muons/Rest]Digis,hltEcalRegional[Muons/Rest]WeightUncalibRecHit,hltEcalRegional[Muons/Rest]RecHitTmp redefined as dummyModules in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- hltL3Muons defined in FastSimulation/HighLevelTrigger/data/Muon/HLTFastRecoForMuon.cff
    #--- hltL3TrajectorySeed redefined as dummyModule in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- hltL3TrackCandidateFromL2 redefined as a sequence in FastSimulation/HighLevelTrigger/data/Muon/HLTFastRecoForMuon.cff
    #--- hltHcalDigis redefined as dummyModule in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- hlt[Hbhe/Ho/Hf]reco --> defined in FastSimulation/HighLevelTrigger/data/common/RecoLocalCalo.cff
    #--- hltEcal[Preshower]RecHit --> defined in FastSimulation/HighLevelTrigger/data/common/RecoLocalCalo.cff
    #--- hltEcalRegional[Jets/Egamma/Muons/Taus]RecHit --> defined in FastSimulation/HighLevelTrigger/data/common/EcalRegionalReco.cff
    #--- hltEcalRecHitAll --> defined in FastSimulation/HighLevelTrigger/data/common/EcalRegionalReco.cff
    #--- hltPixelVertices --> kept when HLTRecopixelvertexingSequence removed from HLT.cff
    #--- hltCtfWithMaterialTracksMumu,hltMuTracks defined in FastSimulation/HighLevelTrigger/data/btau/L3ForDisplacedMumuTrigger.cff
    #--- hltEcalDigis, hltEcalWeightUncalibRecHit redefined as dummyModules in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- hltL3SingleTau[MET]PixelSeeds[Relaxed] redefined as dummyModules in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- hltCkfTrackCandidatesL3SingleTau[MET][Relaxed] replaced by sequences in FastSimulation/HighLevelTrigger/data/btau/HLTFastRecoForTau.cff
    #--- hltCtfWithMaterialTracksL3SingleTau[MET][Relaxed] now defined in FastSimulation/HighLevelTrigger/data/btau/HLTFastRecoForTau.cff
    #--- hltBLifetimeRegionalPixelSeedGenerator[Relaxed] redefined as dummyModules in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- hltBLifetimeRegionalCkfTrackCandidates[Relaxed] replaced by sequences in FastSimulation/HighLevelTrigger/data/btau/lifetimeRegionalTracking.cff
    #--- hltBLifetimeRegionalCtfWithMaterialTracks[Relaxed] redefined in FastSimulation/HighLevelTrigger/data/btau/lifetimeRegionalTracking.cff
    #--- hltMumuPixelSeedFromL2Candidate redefined as dummyModule in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- hltCkfTrackCandidatesMumu replaced by sequence in FastSimulation/HighLevelTrigger/data/btau/L3ForDisplacedMumuTrigger.cff
    #--- hltCtfWithMaterialTracksMumu redefined in FastSimulation/HighLevelTrigger/data/btau/L3ForDisplacedMumuTrigger.cff
    #--- hltMumukPixelSeedFromL2Candidate redefined as dummyModule in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- hltCkfTrackCandidatesMumuk replaced by sequence in FastSimulation/HighLevelTrigger/data/btau/L3ForMuMuk.cff
    #--- hltCtfWithMaterialTracksMumuk,hltMumukAllConeTracks defined in FastSimulation/HighLevelTrigger/data/btau/L3ForMuMuk.cff
    #--- hltL3MuonIsolations -> Kept when HLTL3muonisorecoSequence removed from HLT.cff
    #--- hltPixelTracksForMinBias -> defined in FastSimulation/HighLevelTrigger/data/special/HLTFastRecoForSpecial.cff
    #--- hltFilterTriggerType defined in FastSimulation/HighLevelTrigger/data/special/HLTFastRecoForSpecial.cff
    #--- hltL1[Non]IsoStartUpElectronPixelSeeds -> defined in FastSimulation/EgammaElectronAlgosFastSimulation/data/pixelMatchElectronL1[Non]IsoSequenceForHLT.cff
    #--- hltCkfL1[Non]IsoStartUpTrackCandidates, hltCtfL1[Non]IsoStartUpWithMaterialTracks -> defined in FastSimulation/EgammaElectronAlgos/data/pixelMatchElectronL1[Non]IsoSequenceForHLT.cff
    modules = "--modules "
    modules += "hltL3MuonIsolations,"
    modules += "hltPixelVertices,"
    modules += "-hltL1IsoElectronPixelSeeds,"
    modules += "-hltL1IsoStartUpElectronPixelSeeds,"
    modules += "-hltCkfL1IsoTrackCandidates,"
    modules += "-hltCkfL1IsoStartUpTrackCandidates,"
    modules += "-hltCtfL1IsoWithMaterialTracks,"
    modules += "-hltCtfL1IsoStartUpWithMaterialTracks,"
    modules += "-hltL1NonIsoElectronPixelSeeds,"
    modules += "-hltL1NonIsoStartUpElectronPixelSeeds,"
    modules += "-hltCkfL1NonIsoTrackCandidates,"
    modules += "-hltCkfL1NonIsoStartUpTrackCandidates,"
    modules += "-hltCtfL1NonIsoWithMaterialTracks,"
    modules += "-hltCtfL1NonIsoStartUpWithMaterialTracks,"
    modules += "-hltL1IsoLargeWindowElectronPixelSeeds,"
    modules += "hltPixelMatchElectronsL1IsoLargeWindow,"
    modules += "-hltL1NonIsoLargeWindowElectronPixelSeeds,"
    modules += "hltPixelMatchElectronsL1NonIsoLargeWindow,"
    modules += "-hltEcalPreshowerDigis,"
    modules += "-hltEcalRegionalJetsFEDs,"
    modules += "-hltEcalRegionalJetsDigis,"
    modules += "-hltEcalRegionalJetsWeightUncalibRecHit,"
    modules += "-hltEcalRegionalJetsRecHitTmp,"
    modules += "-hltEcalRegionalMuonsFEDs,"
    modules += "-hltEcalRegionalMuonsDigis,"
    modules += "-hltEcalRegionalMuonsWeightUncalibRecHit,"
    modules += "-hltEcalRegionalMuonsRecHitTmp,"
    modules += "-hltEcalRegionalEgammaFEDs,"
    modules += "-hltEcalRegionalEgammaDigis,"
    modules += "-hltEcalRegionalEgammaWeightUncalibRecHit,"
    modules += "-hltEcalRegionalEgammaRecHitTmp,"
    modules += "-hltEcalRegionalTausFEDs,"
    modules += "-hltEcalRegionalTausDigis,"
    modules += "-hltEcalRegionalTausWeightUncalibRecHit,"
    modules += "-hltEcalRegionalTausRecHitTmp,"
    modules += "-hltL3Muons,"
    modules += "-hltL3TrajectorySeed,"
    modules += "-hltL3TrackCandidateFromL2,"
    modules += "-hltL3TkMuons,"
    modules += "-hltHcalDigis,"
    modules += "-hltHoreco,"
    modules += "-hltHfreco,"
    modules += "-hltHbhereco,"
    modules += "-hltEcalPreshowerRecHit,"
    modules += "-hltEcalRecHit,"
    modules += "-hltEcalRegionalRestFEDs,"
    modules += "-hltEcalRegionalRestDigis,"
    modules += "-hltEcalRegionalRestWeightUncalibRecHit,"
    modules += "-hltEcalRegionalRestRecHitTmp,"
    modules += "-hltEcalRegionalJetsRecHit,"
    modules += "-hltEcalRegionalTausRecHit,"
    modules += "-hltEcalRegionalMuonsRecHit,"
    modules += "-hltEcalRegionalEgammaRecHit,"
    modules += "-hltEcalRecHitAll,"
    modules += "-hltCtfWithMaterialTracksMumu,"
    modules += "-hltMuTracks,"
    modules += "-hltEcalDigis,"
    modules += "-hltEcalWeightUncalibRecHit,"
    modules += "-hltL3SingleTauPixelSeeds,"
    modules += "-hltL3SingleTauPixelSeedsRelaxed,"
    modules += "-hltL3SingleTauMETPixelSeeds,"
    modules += "-hltL3SingleTauMETPixelSeedsRelaxed,"
    modules += "-hltCkfTrackCandidatesL3SingleTau,"
    modules += "-hltCkfTrackCandidatesL3SingleTauRelaxed,"
    modules += "-hltCkfTrackCandidatesL3SingleTauMET,"
    modules += "-hltCkfTrackCandidatesL3SingleTauMETRelaxed,"
    modules += "-hltCtfWithMaterialTracksL3SingleTau,"
    modules += "-hltCtfWithMaterialTracksL3SingleTauRelaxed,"
    modules += "-hltCtfWithMaterialTracksL3SingleTauMET,"
    modules += "-hltCtfWithMaterialTracksL3SingleTauMETRelaxed,"
    modules += "-hltBLifetimeRegionalPixelSeedGenerator,"
    modules += "-hltBLifetimeRegionalCkfTrackCandidates,"
    modules += "-hltBLifetimeRegionalCtfWithMaterialTracks,"
    modules += "-hltBLifetimeRegionalPixelSeedGeneratorRelaxed,"
    modules += "-hltBLifetimeRegionalCkfTrackCandidatesRelaxed,"
    modules += "-hltBLifetimeRegionalCtfWithMaterialTracksRelaxed,"
    modules += "-hltMumuPixelSeedFromL2Candidate,"
    modules += "-hltCkfTrackCandidatesMumu,"
    modules += "-hltCtfWithMaterialTracksMumu,"
    modules += "-hltMumukPixelSeedFromL2Candidate,"
    modules += "-hltCkfTrackCandidatesMumuk,"
    modules += "-hltCtfWithMaterialTracksMumuk,"
    modules += "-hltMumukAllConeTracks,"
    modules += "-hltPixelTracksForMinBias,"
    modules += "-hltMuonCSCDigis,"
    modules += "-hltMuonDTDigis,"
    modules += "-hltMuonRPCDigis,"
    modules += "-hltFilterTriggerType,"
    modules += "-hltGtDigis,"
    modules += "-hltSiStripRegFED,"
    modules += "-hltEcalRegFED,"
    modules += "-hltSubdetFED,"
    modules += "-hcalFED,"    
    modules += "-hltL1gtTrigReport,"
#--- The following modules must always be present to allow for individual paths to be run
    modules += "hltCsc2DRecHits,"
    modules += "hltDt1DRecHits,"
    modules += "hltRpcRecHits"

    #--- Some notes about removed sequences ---#
    #--- HLTL1[Non]IsoEgammaRegionalRecoTrackerSequence defined in FastSimulation/EgammaElectronAlgos/data/l1[Non]IsoEgammaRegionalRecoTracker.cff
    #--- HLTL1[Non]Iso[Startup]ElectronsRegionalRecoTrackerSequence defined in FastSimulation/EgammaElectronAlgos/data/l1[Non]IsoElectronsRegionalRecoTracker.cff
    #--- HLTL1[Non]IsoLargeWindowElectronsRegionalRecoTrackerSequence defined in FastSimulation/EgammaElectronAlgos/data/l1[Non]IsoLargeWindowElectronsRegionalRecoTracker.cff
    #--- HLTPixelMatchElectronL1[Non]IsoSequence defined in FastSimulation/EgammaElectronAlgos/data/pixelMatchElectronL1[Non]IsoSequenceForHLT.cff
    #--- HLTPixelMatchElectronL1[Non]IsoLargeWindow[Tracking]Sequence defined in FastSimulation/EgammaElectronAlgos/data/pixelMatchElectronL1[Non]IsoLargeWindowSequenceForHLT.cff
    #--- HLTDoLocal[Pixel/Strip]Sequence defined in FastSimulation/HighLevelTrigger/data/common/HLTSetup.cff
    #--- HLTrecopixelvertexingSequence defined in FastSimulation/Tracking/data/PixelVerticesProducer.cff
    #--- HLTL3displacedMumurecoSequence defined in FastSimulation/HighLevelTrigger/data/btau/L3ForDisplacedMumuTrigger.cff
    #--- HLTPixelTrackingForMinBiasSequence defined in FastSimulation/HighLevelTrigger/data/special/HLTFastRecoForSpecial.cff
    #--- HLTEndSequence defined in FastSimulation/Configuration/test/ExampleWithHLT.cff
    sequences = "--sequences "
    sequences += "-HLTL1IsoEgammaRegionalRecoTrackerSequence,"
    sequences += "-HLTL1NonIsoEgammaRegionalRecoTrackerSequence,"
    sequences += "-HLTL1IsoLargeWindowElectronsRegionalRecoTrackerSequence,"
    sequences += "-HLTL1NonIsoLargeWindowElectronsRegionalRecoTrackerSequence,"
    sequences += "-HLTL1IsoElectronsRegionalRecoTrackerSequence,"
    sequences += "-HLTL1IsoStartUpElectronsRegionalRecoTrackerSequence,"
    sequences += "-HLTL1NonIsoElectronsRegionalRecoTrackerSequence,"
    sequences += "-HLTL1NonIsoStartUpElectronsRegionalRecoTrackerSequence,"
    sequences += "-HLTPixelMatchElectronL1IsoLargeWindowTrackingSequence,"
    sequences += "-HLTPixelMatchElectronL1NonIsoLargeWindowTrackingSequence,"
    sequences += "-HLTPixelTrackingForMinBiasSequence,"
    sequences += "-HLTDoLocalStripSequence,"
    sequences += "-HLTDoLocalPixelSequence,"
    sequences += "-HLTRecopixelvertexingSequence,"
    sequences += "-HLTEndSequence,"
    sequences += "-HLTBeginSequence"
    
    #--- Some notes about removed paths: ---#
    #--- CandHLTCSCBeamHalo removed because of L1_SingleMuBeamHalo (not found in L1Menu2007)
    #--- HLT1MuonL1Open removed because of L1_SingleMuOpen (not found in L1Menu2007)
    #--- HLTMinBiasHcal removed because no relevant L1 bits found in L1Menu2007
    #--- HLTMinBiasEcal removed because no relevant L1 bits found in L1Menu2007
    #--- HLT4jet30 removed because of L1_QuadJet15 (not found in L1Menu2007)
    #--- HLTXElectron3Jet30 removed because of L1_EG5_TripleJet15 (not found in L1Menu2007)
    #--- CandHLTCSCBeamHaloOverlapRing1 removed because of L1_SingleMuBeamHalo (not found in L1Menu2007)
    #--- CandHLTCSCBeamHaloOverlapRing2 removed because of L1_SingleMuBeamHalo (not found in L1Menu2007)
    #--- CandHLTCSCBeamHaloRing2or3 removed because of L1_SingleMuBeamHalo (not found in L1Menu2007)
    paths = "--paths "
    
    if L1Menu == "L1Menu2007":
        paths += "-HLT_L1MuOpen,"
        paths += "-HLT_MinBiasHcal,"
        paths += "-HLT_MinBiasEcal,"
        paths += "-HLT_CSCBeamHalo,"
        paths += "-HLT_CSCBeamHaloOverlapRing1,"
        paths += "-HLT_CSCBeamHaloOverlapRing2,"
        paths += "-HLT_CSCBeamHaloRing2or3,"
        paths += "-HLT_QuadJet30,"
        paths += "-HLT_IsoEle5_TripleJet30,"

    if L1Menu == "L1Menu2008_2E30":
        paths += "-AlCa_HcalPhiSym,"

    paths += "-DummyPath"


    #--- Special case: Running a user-specified set of paths ---#
    if usePaths != "All":
        paths = "--paths " + usePaths

    services = "--services "
    #--- Begin services removed in 21X ---#
    services += "-UpdaterService,"
    #--- End services removed in 21X ---#
    services += "-PrescaleService,"
    services += "-MessageLogger"

    psets = "--psets "
    psets += "-options,"
    psets += "-maxEvents"

    #--- Adapt for python ---#
    #subStart = cffName.find(".")
    #subEnd = len(cffName)
    #cffType = cffName[subStart:subEnd]
    if cffName.endswith(".py"):
        cffType = ".py"
    else:
        cffType = ".cff"
    baseCommand = "edmConfigFromDB --cff --configName " + dbName
    if cffType == ".py":
        baseCommand += " --format Python"

    # myGetCff = "edmConfigFromDB --cff --configName " + dbName + " " + essources + " " + esmodules + " " + services + " " + psets + " > " + cffName
    if ( blockName == "None" ):
        myGetCff = baseCommand + " " + essources + " " + esmodules + " " + blocks + " " + sequences + " " + modules + " " + paths + " " + services + " " + psets + " > " + cffName
    else:
        blockpaths = "--nopaths"
        blockessources = "--noessources"
        blockesmodules = "--noesmodules"
        blockservices = "--noservices"
        blockpsets = "--nopsets"
        myGetCff = baseCommand + " " + essources + " " + esmodules + " " + sequences + " " + modules + " " + paths + " " + services + " " + psets + " > " + cffName
        myGetBlocks = baseCommand + " " + blockessources + " " + blockesmodules + " " + blockservices + " " + blockpaths + " " + blockpsets + " " + blocks + " > " + blockName 
        
    # Write blocks for electrons and muons, in py configuration
    if ( blockName != "None" ) :
        os.system(myGetBlocks)
        bName = "None"
        for line in fileinput.input(blockName,inplace=1):

            if line.find("hltOfflineBeamSpot") > 0:
                line = line.replace('hltOfflineBeamSpot','offlineBeamSpot') 

            if line.find("block_hlt") == 0:
                subStart = line.find("_")
                subEnd = line.find(" ",subStart)
                bName = line[subStart:subEnd]

            if line.find("SeedConfiguration") == 0:
                if bName == "None":
                    print line[:-1]
            elif line.find("MuonTrackingRegionBuilder") == 0:
                if bName == "None":
                    print line[:-1]
            elif line.find(")") == 0:
                if bName != "None":
                    bName = "None"
                else:
                    print line[:-1]
            else:
                print line[:-1]

    # Write all HLT
    os.system(myGetCff)

    # myReplaceTrigResults = "replace TriggerResults::HLT " + process + " -- " + cffName
    # os.system(myReplaceTrigResults)

    # Make replace statements at the beginning of the cff
    mName = "None"
    mType = "None"
    bName = "None"
    #schedule = "process.schedule = cms.Schedule(process."
    for line in fileinput.input(cffName,inplace=1):
        if cffType == ".py":
            if line.find("cms.EDProducer") >= 0:
                subStart = line.find("hlt")
                subEnd = line.find(" ",subStart)
                mName = line[subStart:subEnd]
                subStart = line.find("=") + 19
                subEnd = line.find(" ",subStart)
                mType = line[subStart:subEnd-2]
            #if line.find("cms.Path") > 0:
                #subEnd = line.find("cms.Path")
                #myPath = line[0:subEnd-3]
                #schedule += myPath
                #if myPath != "HLTriggerFinalPath":
                    #schedule += ",process."
                #else:
                    #schedule += ")"                    
        else:
            if line.find("module hlt") >= 0:
                subStart = line.find("hlt")
                subEnd = line.find(" ",subStart)
                mName = line[subStart:subEnd]
                subStart = line.find("=") + 2
                subEnd = line.find(" ",subStart)
                mType = line[subStart:subEnd]

        if line.find("block block_hlt") == 0:
            subStart = line.find("_")
            subEnd = line.find(" ",subStart)
            bName = line[subStart:subEnd]


        if line.find("HLTL3PixelIsolFilterSequence = ") == 0:
            line = line.replace('hltPixelTracks','hltPixelTracking')
            print line[:-1]
        elif line.find("GMTReadoutCollection") > 0:
            if mName == "hltL2MuonSeeds":
                line = line.replace('hltGtDigis','l1ParamMuons') 
            print line[:-1]
        elif line.find("InputObjects") > 0:
            if mName == "hltL2MuonSeeds":
                line = line.replace('hltL1extraParticles','l1ParamMuons') 
            print line[:-1]
        elif line.find("L1MuonCollectionTag") > 0:
            line = line.replace('hltL1extraParticles','l1ParamMuons')
            print line[:-1] 
        elif line.find("L1CollectionsTag") > 0:
            line = line.replace('hltL1extraParticles','l1extraParticles')
            print line[:-1]
        elif line.find("L1GtObjectMapTag") > 0:
            line = line.replace('hltL1GtObjectMap','gtDigis') 
            print line[:-1]
        elif line.find("L1GtReadoutRecordTag") > 0:
            line = line.replace('hltGtDigis','gtDigis') 
            print line[:-1] 
        elif line.find("PSet SeedConfiguration") > 0:
            if bName == "None":
                print line[:-1]
        elif line.find("PSet MuonTrackingRegionBuilder") > 0:
            if bName == "None":
                print line[:-1]
        elif line.find("CandTag") > 0:
            line = line.replace('hltL1extraParticles','l1ParamMuons')
            print line[:-1]
        elif line.find("}") > 0:
            if bName != "None":
                bName = "None"
            else:
                print line[:-1]
        else:
            print line[:-1]
            # The first line should be where the comments go 
            if line.find("//") == 0 or line.find("#") == 0:
                print "# Begin replace statements specific to the FastSim HLT"
                print "# For all HLTLevel1GTSeed objects, make the following replacements:"
                print "#   - L1GtReadoutRecordTag changed from hltGtDigis to gtDigis"
                print "#   - L1CollectionsTag changed from hltL1extraParticles to l1extraParticles"
                print "#   - L1MuonCollectionTag changed from hltL1extraParticles to l1ParamMuons"
                print "# For hltL2MuonSeeds: InputObjects and GMTReadoutCollection set to l1ParamMuons"
                print "# All other occurances of hltL1extraParticles recast as l1extraParticles"
                print "# L1GtObjectMapTag: hltL1GtObjectMap recast as gtDigis"
                print "# L1GtReadoutRecordTag: hltGtDigis recast as gtDigis"
                print "# hltMuon[CSC/DT/RPC]Digis changed to muon[CSC/DT/RPC]Digis"
                print "# Replace hltOfflineBeamSpot with offlineBeamSpot"
                print "# AlCaIsoTrack needs HLTpixelTracking instead of pixelTracks"
                print "# Some HLT modules were recast as FastSim sequences: "
                print "#   - hltL3TrackCandidateFromL2, see FastSimulation/HighLevelTrigger/data/Muon/HLTFastRecoForMuon.cff"
                print "#   - hltCkfTrackCandidatesL3SingleTau[MET][Relaxed], see FastSimulation/HighLevelTrigger/data/btau/HLTFastRecoForTau.cff"
                print "#   - hltCkfTrackCandidatesMumu, see FastSimulation/HighLevelTrigger/data/btau/L3ForDisplacedMumuTrigger.cff"
                print "#   - hltCkfTrackCandidatesMumuk, see FastSimulation/HighLevelTrigger/data/btau/L3ForMuMuk.cff"
                print "#   - hltBLifetimeRegionalCkfTrackCandidates[Relaxed], see FastSimulation/HighLevelTrigger/data/btau/lifetimeRegionalTracking.cff"
                print "# See FastSimulation/Configuration/test/getFastSimHLTcff.py for other documentation"
                print "# (L1Menu2007 only) Replace L1_QuadJet30 with L1_QuadJet40"
                print "# (Temporary) Remove PSet begin and end from block" 
                print "# End replace statements specific to the FastSim HLT"
            if cffType == ".py":    
                if line.find("#") == 0:
                    print "# Additional import to make this file self contained"
                    print "from FastSimulation.HighLevelTrigger.HLTSetup_cff import *"
#                if line.find("cms.EndPath") > 0:
#                    print schedule

    # Now try to make the replacements
    for line in fileinput.input(cffName,inplace=1):
        if line.find("//") < 0:
            if line.find("hltOfflineBeamSpot") > 0:
                line = line.replace('hltOfflineBeamSpot','offlineBeamSpot') 
            if line.find("hltMuonCSCDigis") > 0:
                if cffType == ".py":
                    if line.find("hltMuonCSCDigis +") > 0:
                        line = line.replace('hltMuonCSCDigis','cms.SequencePlaceholder("simMuonCSCDigis")')
                    else:
                        line = line.replace('hltMuonCSCDigis','simMuonCSCDigis')
                else:
                    line = line.replace('hltMuonCSCDigis','simMuonCSCDigis')
            if line.find("hltMuonDTDigis") > 0:
                if cffType == ".py":
                    if line.find("hltMuonDTDigis +") > 0:
                        line = line.replace('hltMuonDTDigis','cms.SequencePlaceholder("simMuonDTDigis")')
                    else:
                        line = line.replace('hltMuonDTDigis','simMuonDTDigis')
                else:
                    line = line.replace('hltMuonDTDigis','simMuonDTDigis') 
            if line.find("hltMuonRPCDigis") > 0:
                if cffType == ".py":
                    if line.find("hltMuonRPCDigis +") > 0:
                        line = line.replace('hltMuonRPCDigis','cms.SequencePlaceholder("simMuonRPCDigis")')
                    else:
                        line = line.replace('hltMuonRPCDigis','simMuonRPCDigis')
                else:
                    line = line.replace('hltMuonRPCDigis','simMuonRPCDigis') 
            if line.find("HLTEndSequence") > 0:
                if cffType == ".py":
                    line = line.replace('HLTEndSequence','cms.SequencePlaceholder("HLTEndSequence")')
            if line.find("hltL1extraParticles") > 0:
                line = line.replace('hltL1extraParticles','l1extraParticles') 
            if line.find("QuadJet30") > 0:
                if L1Menu == "L1Menu2007":
                    line = line.replace('QuadJet30','QuadJet40') 

        print line[:-1]

    #if ( cfgName != "None" ):
        #for line in fileinput.input(cfgName,inplace=1):
            #if line.find("process.schedule =") >= 0 :
                #print schedule
            #else:
                #print line[:-1]
