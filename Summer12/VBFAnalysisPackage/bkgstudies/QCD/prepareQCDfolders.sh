#Example
#./prepareQCDfolders.sh /gwteray/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_EGMu_PFlow_QCDSelection_noEleHLTcorr_NoIso_NoId_NoMET /gwteray/users/ldimatt/NTUPLES/Fall11_v3/EGMu/VBFAnalysis_PFlow_allH_PT30_maxSumPt_maxDeta_EGMu_PFlow_noEleHLTcorr_NoMET/
#

#!/bin/bash          
if [ -z "$1" ] || [ -z "$2" ]
then 
  echo "usage: $0 QCDselectionFolder AnalysisFolder"
  exit
fi

rm -r $2/QCD_data*

ls $1 | grep data | awk '{print "mkdir '$2'/QCD_"$1"" }' | /bin/sh      

