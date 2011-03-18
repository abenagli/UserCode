#!/bin/csh

#Create similar files from a template

set Masses  = ( 120 130 140 150 160 170 180 190 200 250 300 350 400 450 500 550 600 ) 

setenv INPATH /gwpool/users/ldimatt/scratch0/PHD_MiBi/VBF/VBFAnalysisPackage/cfg
setenv OUTPATH /gwpool/users/ldimatt/scratch0/PHD_MiBi/VBF/VBFAnalysisPackage/data/VBFAnalysis_AK5PF_EGMu_EvaluateMVA/
setenv THISPATH /gwpool/users/ldimatt/scratch0/PHD_MiBi/VBF/VBFAnalysisPackage/scripts/VBFAnalysisTMVAEvaluation/

cd $INPATH

setenv i 1
setenv imax 17

if ( -e $OUTPATH/Jobs ) then
  rm -r $OUTPATH/Jobs
  mkdir $OUTPATH/Jobs
endif

if ( -e $THISPATH/lancia.sh ) then
  rm $THISPATH/lancia.sh
endif

rm $OUTPATH/*
rm $OUTPATH/Jobs/*
cd $OUTPATH/Jobs

while ( $i <= $imax ) #cycle on different masses

  if ( -e $OUTPATH/Jobs/job_${Masses[$i]}.sh ) then  
    rm $OUTPATH/Jobs/job_${Masses[$i]}.sh
  endif
   
  cat > $OUTPATH/Jobs/job_${Masses[$i]}.sh <<EOF
cd /gwpool/users/ldimatt/scratch0/PHD_MiBi/VBF/VBFAnalysisPackage/
source ./scripts/setup.sh
unbuffer VBFAnalysis_evaluateTMVA.exe /gwpool/users/ldimatt/scratch0/PHD_MiBi/VBF/VBFAnalysisPackage/cfg/selections_VBFAnalysisPFJets_H${Masses[$i]}_MVAEvaluation_EGMu.cfg >> $OUTPATH/out_${Masses[$i]}.txt

EOF
  
  cat >> $THISPATH/lancia.sh <<EOF
  sleep 0.5
  echo "qsub -V -d $OUTPATH -q production $OUTPATH/Jobs/job_${Masses[$i]}.sh"
  qsub -V -d $OUTPATH -q production $OUTPATH/Jobs/job_${Masses[$i]}.sh

EOF

  setenv i `expr $i + 1`
   
end    
