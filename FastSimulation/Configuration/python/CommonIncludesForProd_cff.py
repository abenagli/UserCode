import FWCore.ParameterSet.Config as cms

# Common includes for the Spring 08 production
#   Initialize the random generators
from FastSimulation.Configuration.RandomServiceInitialization_cff import *
# Famos sequences (With HLT)
from FastSimulation.Configuration.CommonInputsFake_cff import *
from FastSimulation.Configuration.FamosSequences_cff import *
# L1 Emulator and HLT Setup
from FastSimulation.HighLevelTrigger.HLTSetup_cff import *
# prescale factors at L1 : useful for testing all L1 paths
from Configuration.StandardSequences.L1TriggerDefaultMenu_cff import *


