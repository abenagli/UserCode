#!/bin/csh

# clean
rm $CMSSW_BASE/src/FastSimulation/Configuration/python/HLT_cff.py
rm $CMSSW_BASE/src/FastSimulation/Configuration/test/IntegrationTestWithHLT.log
rm $CMSSW_BASE/src/FastSimulation/Configuration/python/blockHLT_cff.py
rm AODIntegrationTestWithHLT.root

# create HLT.cff :  For 2_0_4, use /dev/CMSSW_2_0_0/HLT/V35
# For the adventurous user, you may specify the L1 Menu and a select number of HLT paths
# Usage: ./getFastSimHLTcff.py <Version from ConfDB> <Name of cff file> <optional L1 Menu> <optional subset of paths>
# The default L1 Menu for 2_0_4 is L1Menu2008_2E30
# Any path subset must be given as a comma-separated list: a,b,c
# NOTE: If you choose to run a subset of the HLT, you MUST specify the L1 Menu used
# If you choose another L1 Menu, you must also change the corresponding include in 
# FastSimulation/Configuration/test/ExampleWithHLT.cfg
set HLTVersion=`head -1 $CMSSW_BASE/src/FastSimulation/Configuration/test/HLTVersion`
$CMSSW_BASE/src/FastSimulation/Configuration/test/getFastSimHLTcff.py $HLTVersion "$CMSSW_BASE/src/FastSimulation/Configuration/python/HLT_cff.py" "$CMSSW_BASE/src/FastSimulation/Configuration/python/blockHLT_cff.py" "L1Menu2008_2E30" "All" 

# Set environement
cmsenv

# Compile the HLT_cff.py file
cd $CMSSW_BASE/src/FastSimulation/
scramv1 b python
cd -

# cmsRun 
cmsRun $CMSSW_BASE/src/FastSimulation/Configuration/test/IntegrationTestWithHLT_cfg.py |& tee $CMSSW_BASE/src/FastSimulation/Configuration/test/IntegrationTestWithHLT_py.log
