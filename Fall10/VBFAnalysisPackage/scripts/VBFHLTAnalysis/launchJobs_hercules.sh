#!/bin/sh

#Usage mode
if [ $# -ne 1 ]
then
    echo "Error in $0 - Invalid Argument Count"
    echo "Syntax: $0 input_template_cfg"
    exit
fi

#Set up all folders and inputs

QUEUE="longcms"

TEMPLATE=$1

EXE="measurePFMetTurnOn.exe"

HOME="/gwpool/users/ldimatt/scratch0/PHD_MiBi/Fall11_ForDataVsMC_NewNT/VBFAnalysisPackage/"
THISFOLDER="${HOME}scripts/VBFHLTAnalysis/"

INPUTS=(scripts/VBFHLTAnalysis/listaFile_MC.txt
scripts/VBFHLTAnalysis/listaFile_MC.txt
scripts/VBFHLTAnalysis/listaFile_11A.txt
scripts/VBFHLTAnalysis/listaFile_11B.txt
scripts/VBFHLTAnalysis/listaFile_11B.txt
scripts/VBFHLTAnalysis/listaFile_11B.txt)

OUTFOLDER="data/PFMETto"

PERIODS=(-2
-1
1
2
3
4)

ERA=(WJets_PFMHT25
WJets_PFMHT20
Run11A
Run11B_Normal
Run11B_Backup
Run11B_PF)

RUNMIN=(-1
-1
165088
175832
176461
178420)

RUNMAX=(9999999
9999999
173692
176309
178380
180252)

INTREE="ntu_15"

for i in {0..5}
do
  # First step: Make the various cfg files
  filecfg="${THISFOLDER}${ERA[i]}.cfg"
  #echo "$filecfg"
  # Check if filecfg exists, remove it if so
  if [ -e "$filecfg" ]       
  then
    rm $filecfg
  fi
  
  # Now take the template and plug in the correct variables
  cp $TEMPLATE $filecfg
  cat $filecfg | sed -e s\%INPUTFILELIST\%${HOME}${INPUTS[i]}\%g  > pippo.txt ; mv pippo.txt $filecfg
  cat $filecfg | sed -e s\%INPUTTREE\%$INTREE\%g  > pippo.txt ; mv pippo.txt $filecfg
  cat $filecfg | sed -e s\%DATAERA\%${ERA[i]}\%g  > pippo.txt ; mv pippo.txt $filecfg
  cat $filecfg | sed -e s\%OUTPUTFOLDER\%${HOME}${OUTFOLDER}\%g  > pippo.txt ; mv pippo.txt $filecfg
  cat $filecfg | sed -e s\%RUNMIN\%${RUNMIN[i]}\%g  > pippo.txt ; mv pippo.txt $filecfg
  cat $filecfg | sed -e s\%RUNMAX\%${RUNMAX[i]}\%g  > pippo.txt ; mv pippo.txt $filecfg
  cat $filecfg | sed -e s\%HLTPERIOD\%${PERIODS[i]}\%g  > pippo.txt ; mv pippo.txt $filecfg

  # Second step: Make the various sh files and prepare the source.sh
  filesh="${THISFOLDER}${ERA[i]}.sh"
  # Check if filesh exists, remove it if so
  if [ -e "$filesh" ]       
  then
    rm $filesh
  fi

  printf "cd $HOME\n" > $filesh
  printf "source ./scripts/setup.sh\n" >> $filesh
  printf "cd $THISFOLDER\n" >> $filesh
  printf "unbuffer $EXE $filecfg > out_${ERA[i]}.txt\n" >> $filesh
  
  lanciash="${THISFOLDER}lancia.sh"
  if [ $i = "0" ]
  then
    rm $lanciash
    printf "echo \"qsub -V -q $QUEUE -d ./ ./$filesh\"\n" > $lanciash
    printf "qsub -V -q $QUEUE -d ./ $filesh \n" >> $lanciash
  else
    printf "echo \"qsub -V -q $QUEUE -d ./ ./$filesh\"\n" >> $lanciash
    printf "qsub -V -q $QUEUE -d ./ $filesh \n" >> $lanciash
  fi
  
done  

