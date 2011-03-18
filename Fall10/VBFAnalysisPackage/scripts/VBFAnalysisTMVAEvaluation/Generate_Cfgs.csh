#!/bin/csh

#Create similar files from a template

set Masses  = ( 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600 ) 

setenv INPATH /gwpool/users/ldimatt/scratch0/PHD_MiBi/VBF/VBFAnalysisPackage/cfg

cd $INPATH

setenv i 1
setenv imax 17


while ( $i <= $imax ) #cycle on different tangents

  if ( -e selections_VBFAnalysisPFJets_H${Masses[$i]}_MVAEvaluation_EGMu.cfg ) then  
    rm selections_VBFAnalysisPFJets_H${Masses[$i]}_MVAEvaluation_EGMu.cfg
  endif
 
  echo "selections_VBFAnalysisPFJets_H'${Masses[$i]}'_MVAEvaluation_EGMu.cfg"
  
  cat > selections_VBFAnalysisPFJets_H${Masses[$i]}_MVAEvaluation_EGMu.cfg <<EOF
[Input]
baseDir = /gwpool/users/ldimatt/scratch0/PHD_MiBi/VBF/VBFAnalysisPackage/data/VBFAnalysis_AK5PF_H170_EGMu_TrainMVA/
jetAlgorithm = L2L3Ak5PFJets
jetType = PF
inputFileName = VBFAnalysis_AK5PF
inputSigDirs = 2_VBF_HToWWToLNuQQ_M-${Masses[$i]}_7TeV-powheg-pythia6_Fall10-START38_V12-v1,2_GluGluToHToWWToLNuQQ_M-${Masses[$i]}_7TeV-powheg-pythia6_Fall10-START38_V12-v1
inputBkgDirs = 2_DYJetsToLL_TuneZ2_M-50_7TeV-madgraph-tauola_Fall10-START38_V12-v3,2_GJets_TuneD6T_HT-100To200_7TeV-madgraph_Fall10-START38_V12-v1,2_GJets_TuneD6T_HT-200_7TeV-madgraph_Fall10-START38_V12-v1,2_GJets_TuneD6T_HT-40To100_7TeV-madgraph_Fall10-START38_V12-v1,2_QCD_Pt-20_MuEnrichedPt-15_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1,2_QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1,2_QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1,2_QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1,2_QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1,2_QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1,2_TTJets_TuneZ2_7TeV-madgraph-tauola_Fall10-START38_V12-v3,2_TToBLNu_TuneZ2_s-channel_7TeV-madgraph_Fall10-START38_V12-v1,2_TToBLNu_TuneZ2_t-channel_7TeV-madgraph_Fall10-START38_V12-v2,2_TToBLNu_TuneZ2_tW-channel_7TeV-madgraph_Fall10-START38_V12-v2,2_VQQJetsToLL_TuneD6T_7TeV-madgraph-tauola_Fall10-START38_V12-v2,2_WJetsToLNu_TuneZ2_7TeV-madgraph-tauola_Fall10-START38_V12-v1,2_WWtoAnything_TuneZ2_7TeV-pythia6-tauola_Fall10-START38_V12-v1,2_WZtoAnything_TuneZ2_7TeV-pythia6-tauola_Fall10-START38_V12-v1,2_ZZtoAnything_TuneZ2_7TeV-pythia6-tauola_Fall10-START38_V12-v1,3_QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6_Fall10-START38_V12-v1
higgsMass = ${Masses[$i]}
inputweightfolder = /gwpool/users/ldimatt/scratch0/PHD_MiBi/VBF/VBFAnalysisPackage/weights


[Output]
outputRootFilePath = /gwpool/users/ldimatt/scratch0/PHD_MiBi/VBF/VBFAnalysisPackage/data/VBFAnalysis_AK5PF_EGMu_EvaluateMVA/
outputRootFileName = TMVAEvaluation

[Options]
entryFIRST = 0
entryMAX = -1
entryMODULO = 10000
step = 20
lumiInt = 1000

EOF

 setenv i `expr $i + 1`
   
end    
