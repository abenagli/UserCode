#include "readReducedTree.h"






void initializeCuts(cuts& myCuts)
{
  myCuts.tagJJ_DetaMIN = 4.;
  myCuts.tagJJ_mMIN = 700.;
  myCuts.tagJJ_max_etMIN = 40.;
  myCuts.tagJJ_max_etMAX = 500.;
  myCuts.tagJJ_min_etMIN = 20.;
  myCuts.tagJJ_min_etMAX = 500.;
  
  myCuts.WJJ_mMIN = 60.;
  myCuts.WJJ_mMAX = 110.;
  myCuts.WJJ_max_etMIN = 30.;
  myCuts.WJJ_max_etMAX = 150.;
  myCuts.WJJ_min_etMIN = 15.;
  myCuts.WJJ_min_etMAX = 100.;
  myCuts.WJJ_zeppMAX = 0.45;
  myCuts.WJJ_DRMAX = 5.;
  
  myCuts.nCentralJets_et25MAX = 0;
  
  myCuts.lep_nMAX = 1;
  myCuts.lep_ptMIN = 15.;
  myCuts.lep_ptMAX = 500.;
  myCuts.lep_etaMAX = 2.3;
  myCuts.lep_zeppMAX = 0.45;
  myCuts.lepTkIsoOverPtMAX = 0.1;
  myCuts.lep_eleIdMIN = 1.;
  myCuts.lep_tipSigMAX = 5.;
  myCuts.lepMet_mtMIN = 20.;
  myCuts.lepMet_mtMAX = 140.;
  
  myCuts.tagJ_bTagMAX = 5.;
  myCuts.tagJ_bTagMAX = 5.;
  myCuts.WJ_bTagMAX = 5.;
  myCuts.WJ_bTagMAX = 5.;
  
  myCuts.met_etMIN = 15.;
  myCuts.met_etMAX = 120.;
  
  myCuts.lepMetW_DphiMAX = 1.;
  
  myCuts.lepWJJ_pt1MIN = 40.;
  
  myCuts.lepMetW_mtMIN = 50.;
  myCuts.lepMetW_mtMAX = 400.;
  
  
  
  myCuts.mvaValueMIN = 0.;
}






void readReducedTree(const std::string& inFileName,
                     const bool& useTMVA,
                     std::vector<float>* nEventsScaled_sig,
                     std::vector<float>* nEventsScaled_bkg,
                     std::vector<cuts*>* myCuts)
{
  
  //***********************
  // DEFINE TREE
  //***********************
  
  TFile* f = new TFile(inFileName.c_str(), "READ");
  
  TTree* t;
  f -> GetObject("tree_3", t);
  
  float mH;
  int totEvents;
  float crossSection;
  int type;
  float tagJJ_Deta;
  float tagJJ_etaProd;
  float tagJJ_m;
  float tagJJ_max_e;
  float tagJJ_min_e;
  float tagJJ_max_et;
  float tagJJ_min_et;
  float tagJJ_max_eta;
  float tagJJ_min_eta;
  float tagJ1_bTag;
  float tagJ2_bTag;
  float WJJ_pt;
  float WJJ_eta;
  float WJJ_zepp;
  float WJJ_Deta;
  float WJJ_Dphi;
  float WJJ_DR;
  float WJJ_etaProd;
  float WJJ_m;
  float WJJ_max_et;
  float WJJ_min_et;
  float WJJ_max_eta;
  float WJJ_min_eta;
  float WJJ_max_zepp;
  float WJJ_min_zepp;
  float WJ1_bTag;
  float WJ2_bTag;
  int lep_n;
  float lep_pt;
  float lep_eta;
  float lep_zepp;
  float lep_eleId;
  float lep_tkIso;
  float lep_emIso;
  float lep_hadIso;
  float lep_tipSig;
  float lep_lipSig;
  float lep_3DipSig;
  float lepWJJ_pt1;
  float lepWJJ_pt2;
  float lepWJJ_pt3;
  float met_et;
  float lepMet_mt;
  float lepMetW_mt;
  float lepMetW_Dphi;
  int nOtherJets_et15;
  int nOtherJets_et20;
  int nOtherJets_et25;
  int nOtherJets_et30;
  int nCentralJets_et15;
  int nCentralJets_et20;
  int nCentralJets_et25;
  int nCentralJets_et30;
  
  t -> SetBranchAddress("mH",                &mH               );
  t -> SetBranchAddress("totEvents",         &totEvents        );
  t -> SetBranchAddress("crossSection",      &crossSection     );
  t -> SetBranchAddress("type",              &type             );
  t -> SetBranchAddress("tagJJ_Deta",        &tagJJ_Deta       );
  t -> SetBranchAddress("tagJJ_etaProd",     &tagJJ_etaProd    );
  t -> SetBranchAddress("tagJJ_m",           &tagJJ_m          );  
  t -> SetBranchAddress("tagJJ_max_e",       &tagJJ_max_e      );   
  t -> SetBranchAddress("tagJJ_min_e",       &tagJJ_min_e      );
  t -> SetBranchAddress("tagJJ_max_et",      &tagJJ_max_et     );   
  t -> SetBranchAddress("tagJJ_min_et",      &tagJJ_min_et     ); 
  t -> SetBranchAddress("tagJJ_max_eta",     &tagJJ_max_eta    );   
  t -> SetBranchAddress("tagJJ_min_eta",     &tagJJ_min_eta    );
  t -> SetBranchAddress("tagJ1_bTag",        &tagJ1_bTag       );
  t -> SetBranchAddress("tagJ2_bTag",        &tagJ2_bTag       );
  t -> SetBranchAddress("WJJ_pt",            &WJJ_pt           );
  t -> SetBranchAddress("WJJ_eta",           &WJJ_eta          );
  t -> SetBranchAddress("WJJ_zepp",          &WJJ_zepp         );
  t -> SetBranchAddress("WJJ_Deta",          &WJJ_Deta         );
  t -> SetBranchAddress("WJJ_Dphi",          &WJJ_Dphi         );
  t -> SetBranchAddress("WJJ_DR",            &WJJ_DR           );
  t -> SetBranchAddress("WJJ_etaProd",       &WJJ_etaProd      );
  t -> SetBranchAddress("WJJ_m",             &WJJ_m            );  
  t -> SetBranchAddress("WJJ_max_et",        &WJJ_max_et       );   
  t -> SetBranchAddress("WJJ_min_et",        &WJJ_min_et       );
  t -> SetBranchAddress("WJJ_max_eta",       &WJJ_max_eta      );   
  t -> SetBranchAddress("WJJ_min_eta",       &WJJ_min_eta      );
  t -> SetBranchAddress("WJJ_max_zepp",      &WJJ_max_zepp     );   
  t -> SetBranchAddress("WJJ_min_zepp",      &WJJ_min_zepp     );
  t -> SetBranchAddress("WJ1_bTag",          &WJ1_bTag         );
  t -> SetBranchAddress("WJ2_bTag",          &WJ2_bTag         );
  t -> SetBranchAddress("lep_n",             &lep_n            );
  t -> SetBranchAddress("lep_pt",            &lep_pt           );
  t -> SetBranchAddress("lep_eta",           &lep_eta          );
  t -> SetBranchAddress("lep_zepp",          &lep_zepp         );
  t -> SetBranchAddress("lep_eleId",         &lep_eleId        );
  t -> SetBranchAddress("lep_tkIso",         &lep_tkIso        );
  t -> SetBranchAddress("lep_emIso",         &lep_emIso        );
  t -> SetBranchAddress("lep_hadIso",        &lep_hadIso       );
  t -> SetBranchAddress("lep_tipSig",        &lep_tipSig       );
  t -> SetBranchAddress("lep_lipSig",        &lep_lipSig       );
  t -> SetBranchAddress("lep_3DipSig",       &lep_3DipSig      );
  t -> SetBranchAddress("lepWJJ_pt1",        &lepWJJ_pt1       );
  t -> SetBranchAddress("lepWJJ_pt2",        &lepWJJ_pt2       );
  t -> SetBranchAddress("lepWJJ_pt3",        &lepWJJ_pt3       );
  t -> SetBranchAddress("met_et",            &met_et           );
  t -> SetBranchAddress("lepMet_mt",         &lepMet_mt        );
  t -> SetBranchAddress("lepMetW_mt",        &lepMetW_mt       );
  t -> SetBranchAddress("lepMetW_Dphi",      &lepMetW_Dphi     );
  t -> SetBranchAddress("nOtherJets_et15",   &nOtherJets_et15  );
  t -> SetBranchAddress("nOtherJets_et20",   &nOtherJets_et20  );
  t -> SetBranchAddress("nOtherJets_et25",   &nOtherJets_et25  );
  t -> SetBranchAddress("nOtherJets_et30",   &nOtherJets_et30  );        
  t -> SetBranchAddress("nCentralJets_et15", &nCentralJets_et15);
  t -> SetBranchAddress("nCentralJets_et20", &nCentralJets_et20);
  t -> SetBranchAddress("nCentralJets_et25", &nCentralJets_et25);
  t -> SetBranchAddress("nCentralJets_et30", &nCentralJets_et30);  
  
  
  
  
  
  
  //***********************
  // DEFINE CUTS
  //***********************
  
  if(myCuts == NULL)
  {
    myCuts = new std::vector<cuts*>;
    myCuts -> push_back(new cuts);
    //initializeCuts(myCuts->at(0));
  }
  
  
  
  
  
  
  //***********************
  // LOOP OVER EVENTS
  //***********************
  
  float lumi = 1000.;
  
  if(nEventsScaled_sig == NULL)
  {
    nEventsScaled_sig = new std::vector<float>;
    
   for(unsigned int cutIt = 0; cutIt < myCuts->size(); ++cutIt)
      nEventsScaled_sig->push_back(0.);
  }
  
  if(nEventsScaled_bkg == NULL)
  {
    nEventsScaled_bkg = new std::vector<float>;
    
    for(unsigned int cutIt = 0; cutIt < myCuts->size(); ++cutIt)
      nEventsScaled_bkg->push_back(0.);
  }
  
  
  
  TMVA::Reader* TMVAReader = new TMVA::Reader();
  if(useTMVA)
  {  
    TMVAReader -> AddVariable("tagJJ_Deta", &tagJJ_Deta);
    TMVAReader -> AddVariable("tagJJ_m", &tagJJ_m);
    TMVAReader -> AddVariable("tagJJ_max_e", &tagJJ_max_e);
    TMVAReader -> AddVariable("tagJJ_min_e", &tagJJ_min_e);
    TMVAReader -> AddVariable("WJJ_m", &WJJ_m);
    TMVAReader -> AddVariable("WJJ_DR", &WJJ_DR);
    TMVAReader -> AddVariable("WJJ_max_et", &WJJ_max_et);
    TMVAReader -> AddVariable("WJJ_min_et", &WJJ_min_et);
    TMVAReader -> AddVariable("lepMetW_Dphi", &lepMetW_Dphi);
    TMVAReader -> AddVariable("nCentralJets_et25", &nCentralJets_et25);
    TMVAReader -> AddVariable("lep_pt", &lep_pt);
    TMVAReader -> AddVariable("lep_zepp", &lep_zepp);
    
    TMVAReader -> BookMVA("kBDT", "/Users/andrea/VBF/new/VBFAnalysisPackage/data/Spring10ReReco_7TeV/ReferenceAnalysis_L2L3Antikt5CaloJets_H180/TMVA/weights/example_kBDT.weights.xml");
  }
  
  
  
  
  
  
  std::cout << ">>>>> readReducedTree::Read " << t -> GetEntries() << " entries" << std::endl;  
  for(int entry = 0 ; entry < t -> GetEntries() ; ++entry)
  {
    t -> GetEntry(entry);
    double mvaValue = 0.;
    if(useTMVA) mvaValue = TMVAReader -> EvaluateMVA("kBDT");
    if((entry%100000) == 0) std::cout << ">>>>> readReducedTree::GetEntry " << entry << std::endl;
    
    
    
    float weight = 1. / totEvents * crossSection * lumi;

    
    
    for(unsigned int cutIt = 0; cutIt < myCuts->size(); ++cutIt)
    {
      if(useTMVA)
        if(mvaValue < (myCuts->at(cutIt))->mvaValueMIN) continue;
      
      
      if(!useTMVA)
      {
        if(tagJJ_Deta < (myCuts->at(cutIt))->tagJJ_DetaMIN) continue;
        if(tagJJ_m < (myCuts->at(cutIt))->tagJJ_mMIN) continue;
        if(tagJJ_max_et < (myCuts->at(cutIt))->tagJJ_max_etMIN) continue;
        if(tagJJ_max_et > (myCuts->at(cutIt))->tagJJ_max_etMAX) continue;
        if(tagJJ_min_et < (myCuts->at(cutIt))->tagJJ_min_etMIN) continue;
        if(tagJJ_min_et > (myCuts->at(cutIt))->tagJJ_min_etMAX) continue;
        
        if(WJJ_m < (myCuts->at(cutIt))->WJJ_mMIN) continue;
        if(WJJ_m > (myCuts->at(cutIt))->WJJ_mMAX) continue;
        if(WJJ_max_et < (myCuts->at(cutIt))->WJJ_max_etMIN) continue;
        if(WJJ_max_et > (myCuts->at(cutIt))->WJJ_max_etMAX) continue;
        if(WJJ_min_et < (myCuts->at(cutIt))->WJJ_min_etMIN) continue;
        if(WJJ_min_et > (myCuts->at(cutIt))->WJJ_min_etMAX) continue;
        if(fabs(WJJ_zepp) > (myCuts->at(cutIt))->WJJ_zeppMAX) continue;
        if(WJJ_DR > (myCuts->at(cutIt))->WJJ_DRMAX) continue;
        
        if(nCentralJets_et25 > (myCuts->at(cutIt))->nCentralJets_et25MAX) continue;
        
        if(lep_n > (myCuts->at(cutIt))->lep_nMAX) continue;
        if(lep_pt < (myCuts->at(cutIt))->lep_ptMIN) continue;
        if(lep_pt > (myCuts->at(cutIt))->lep_ptMAX) continue;
        if(fabs(lep_eta) > (myCuts->at(cutIt))->lep_etaMAX) continue;
        if(fabs(lep_zepp) > (myCuts->at(cutIt))->lep_zeppMAX) continue;
        if(lep_tkIso/lep_pt > (myCuts->at(cutIt))->lepTkIsoOverPtMAX) continue;
        if(lep_eleId < (myCuts->at(cutIt))->lep_eleIdMIN) continue;
        if(lep_tipSig > (myCuts->at(cutIt))->lep_tipSigMAX) continue;
        if(lepMet_mt < (myCuts->at(cutIt))->lepMet_mtMIN) continue;
        if(lepMet_mt > (myCuts->at(cutIt))->lepMet_mtMAX) continue;
        
        if(tagJ1_bTag > (myCuts->at(cutIt))->tagJ_bTagMAX) continue;
        if(tagJ2_bTag > (myCuts->at(cutIt))->tagJ_bTagMAX) continue;
        if(WJ1_bTag > (myCuts->at(cutIt))->WJ_bTagMAX) continue;
        if(WJ2_bTag > (myCuts->at(cutIt))->WJ_bTagMAX) continue;
        
        if(met_et < (myCuts->at(cutIt))->met_etMIN) continue;
        if(met_et > (myCuts->at(cutIt))->met_etMAX) continue;
        
        if(lepMetW_Dphi > (myCuts->at(cutIt))->lepMetW_DphiMAX) continue;
        
        if(lepWJJ_pt1 < (myCuts->at(cutIt))->lepWJJ_pt1MIN) continue;
        
        if(lepMetW_mt < (myCuts->at(cutIt))->lepMetW_mtMIN) continue;
        if(lepMetW_mt > (myCuts->at(cutIt))->lepMetW_mtMAX) continue;
      }
      
      
      
      // compute scaled number of events
      if(mH > 0) nEventsScaled_sig->at(cutIt) += weight;
      if(mH < 0) nEventsScaled_bkg->at(cutIt) += weight;
    }
  
  }
  
  
  
  //std::cout << "nEventsScaled_sig = " << *nEventsScaled_sig << std::endl;
  //std::cout << "nEventsScaled_bkg = " << *nEventsScaled_bkg << std::endl;
  //std::cout << "S/sqrt(B) = " << *nEventsScaled_sig/sqrt(*nEventsScaled_bkg) << std::endl;
  
  //return(*nEventsScaled_sig/sqrt(*nEventsScaled_bkg));
}
