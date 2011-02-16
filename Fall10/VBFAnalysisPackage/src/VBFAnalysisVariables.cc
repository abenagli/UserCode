#include "VBFAnalysisVariables.h"



void SetVBFPreselectionTreeBranches(VBFAnalysisVariables& vars, TTree* reducedTree)
{
  //-------------
  // Reduced tree
  //-------------
  
  reducedTree -> SetBranchAddress("mH",           &vars.mH);
  reducedTree -> SetBranchAddress("crossSection", &vars.crossSection);
  reducedTree -> SetBranchAddress("dataFlag",     &vars.dataFlag);
  reducedTree -> SetBranchAddress("runId",        &vars.runId);
  reducedTree -> SetBranchAddress("lumiId",       &vars.lumiId);
  reducedTree -> SetBranchAddress("eventId",      &vars.eventId);
  
  
  // PV variables
  reducedTree -> SetBranchAddress("PV_nTracks", &vars.PV_nTracks);
  
  
  // lepton variables
  vars.p_lep = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("lep",         &vars.p_lep);
  reducedTree -> SetBranchAddress("lep_charge",  &vars.lep_charge);
  reducedTree -> SetBranchAddress("lep_flavour", &vars.lep_flavour);
  reducedTree -> SetBranchAddress("lep_dxy",     &vars.lep_dxy);
  reducedTree -> SetBranchAddress("lep_dz",      &vars.lep_dz);
  reducedTree -> SetBranchAddress("lep_3DipSig", &vars.lep_3DipSig);
  reducedTree -> SetBranchAddress("lep_tkIso",   &vars.lep_tkIso);
  reducedTree -> SetBranchAddress("lep_emIso",   &vars.lep_emIso);
  reducedTree -> SetBranchAddress("lep_hadIso",  &vars.lep_hadIso);
  reducedTree -> SetBranchAddress("lep_isEB",          &vars.lep_isEB);
  reducedTree -> SetBranchAddress("lep_sigmaIetaIeta", &vars.lep_sigmaIetaIeta);
  reducedTree -> SetBranchAddress("lep_DphiIn",        &vars.lep_DphiIn);
  reducedTree -> SetBranchAddress("lep_DetaIn",        &vars.lep_DetaIn);
  reducedTree -> SetBranchAddress("lep_HOverE",        &vars.lep_HOverE);
  reducedTree -> SetBranchAddress("lep_tracker",                  &vars.lep_tracker);
  reducedTree -> SetBranchAddress("lep_standalone",               &vars.lep_standalone);
  reducedTree -> SetBranchAddress("lep_global",                   &vars.lep_global);
  reducedTree -> SetBranchAddress("lep_normalizedChi2",           &vars.lep_normalizedChi2);
  reducedTree -> SetBranchAddress("lep_numberOfValidTrackerHits", &vars.lep_numberOfValidTrackerHits);
  reducedTree -> SetBranchAddress("lep_numberOfValidMuonHits",    &vars.lep_numberOfValidMuonHits);
  
  
  // met variables
  vars.p_met = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("met", &vars.p_met);
  reducedTree -> SetBranchAddress("lepMet_mt",    &vars.lepMet_mt);
  reducedTree -> SetBranchAddress("lepMet_Dphi",  &vars.lepMet_Dphi);
  
  
  // jet variables
  reducedTree -> SetBranchAddress("nJets",      &vars.nJets);
  reducedTree -> SetBranchAddress("nJets_cnt",  &vars.nJets_cnt);
  reducedTree -> SetBranchAddress("nJets_fwd",  &vars.nJets_fwd);
  reducedTree -> SetBranchAddress("nJets_btw",  &vars.nJets_btw);
  reducedTree -> SetBranchAddress("jets_bTag1", &vars.jets_bTag1);
  reducedTree -> SetBranchAddress("jets_bTag2", &vars.jets_bTag2);
  reducedTree -> SetBranchAddress("HT",         &vars.HT);
  reducedTree -> SetBranchAddress("HT_cnt",     &vars.HT_cnt);
  reducedTree -> SetBranchAddress("HT_fwd",     &vars.HT_fwd);
  
  
  // leading jet variables
  vars.p_leadingJ = new ROOT::Math::XYZTVector; 
  reducedTree -> SetBranchAddress("leadingJ", &vars.p_leadingJ);
  reducedTree -> SetBranchAddress("leadingJ_charge", &vars.leadingJ_charge);
  reducedTree -> SetBranchAddress("leadingJ_bTag",   &vars.leadingJ_bTag);
  
  
  // W-jet variables
  vars.p_WJ1 = new ROOT::Math::XYZTVector;
  vars.p_WJ2 = new ROOT::Math::XYZTVector;
  vars.p_WJ12 = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("WJ1", &vars.p_WJ1);
  reducedTree -> SetBranchAddress("WJ2", &vars.p_WJ2);
  reducedTree -> SetBranchAddress("WJ12", &vars.p_WJ12);
  reducedTree -> SetBranchAddress("WJ1_charge", &vars.WJ1_charge);
  reducedTree -> SetBranchAddress("WJ2_charge", &vars.WJ2_charge);
  reducedTree -> SetBranchAddress("WJ1_bTag",   &vars.WJ1_bTag);
  reducedTree -> SetBranchAddress("WJ2_bTag",   &vars.WJ2_bTag);
  
  reducedTree -> SetBranchAddress("lepWJJ_pt1", &vars.lepWJJ_pt1);
  reducedTree -> SetBranchAddress("lepWJJ_pt2", &vars.lepWJJ_pt2);
  reducedTree -> SetBranchAddress("lepWJJ_pt3", &vars.lepWJJ_pt3);
  
  
  // Higgs variables
  reducedTree -> SetBranchAddress("lepMetW_mt",   &vars.lepMetW_mt);
  reducedTree -> SetBranchAddress("lepMetW_Dphi", &vars.lepMetW_Dphi);
  reducedTree -> SetBranchAddress("lepNuW_m",     &vars.lepNuW_m);
  
  
  // tag-jet variables
  vars.p_tagJ1 = new ROOT::Math::XYZTVector;
  vars.p_tagJ2 = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("tagJ1", &vars.p_tagJ1);
  reducedTree -> SetBranchAddress("tagJ2", &vars.p_tagJ2);
  reducedTree -> SetBranchAddress("tagJ1_charge", &vars.tagJ1_charge);
  reducedTree -> SetBranchAddress("tagJ2_charge", &vars.tagJ2_charge);
  reducedTree -> SetBranchAddress("tagJ1_bTag",   &vars.tagJ1_bTag);
  reducedTree -> SetBranchAddress("tagJ2_bTag",   &vars.tagJ2_bTag);
  
  
  // third jet variables
  vars.p_thirdJ = new ROOT::Math::XYZTVector; 
  reducedTree -> SetBranchAddress("thirdJ", &vars.p_thirdJ);
  reducedTree -> SetBranchAddress("thirdJ_charge", &vars.thirdJ_charge);
  reducedTree -> SetBranchAddress("thirdJ_bTag",   &vars.thirdJ_bTag);
}
