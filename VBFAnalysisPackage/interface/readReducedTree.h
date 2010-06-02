#include "TTree.h"
#include "TFile.h"
#include "TMVA/Reader.h"

#include <iostream>
#include <vector>
#include <cmath>






struct cuts
{
  float tagJJ_DetaMIN;
  float tagJJ_mMIN;
  float tagJJ_max_etMIN;
  float tagJJ_max_etMAX;
  float tagJJ_min_etMIN;
  float tagJJ_min_etMAX;
   
  float WJJ_mMIN;
  float WJJ_mMAX;
  float WJJ_max_etMIN;
  float WJJ_max_etMAX;
  float WJJ_min_etMIN;
  float WJJ_min_etMAX;
  float WJJ_zeppMAX;
  float WJJ_DRMAX;
   
  int nCentralJets_et25MAX;
   
  int lep_nMAX;
  float lep_ptMIN;
  float lep_ptMAX;
  float lep_etaMAX;
  float lep_zeppMAX;
  float lepTkIsoOverPtMAX;
  float lep_eleIdMIN;
  float lep_tipSigMAX;
  float lepMet_mtMIN;
  float lepMet_mtMAX;
   
  float tagJ_bTagMAX;
  float WJ_bTagMAX;
  
  float met_etMIN;
  float met_etMAX;
   
  float lepMetW_DphiMAX;
   
  float lepWJJ_pt1MIN;
   
  float lepMetW_mtMIN;
  float lepMetW_mtMAX;
  
  
  
  float mvaValueMIN;
};



void initializeCuts(cuts& myCuts);



void readReducedTree(const std::string& inFileName,
                     const bool& useTMVA,
                     std::vector<float>* nEventsScaled_sig = NULL,
                     std::vector<float>* nEventsScaled_bkg = NULL,
                     std::vector<cuts*>* myCuts = NULL);
