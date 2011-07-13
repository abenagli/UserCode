#include "VBFAnalysisVariables.h"



void SetVBFPreselectionTreeBranches(VBFAnalysisVariables& vars, TTree* reducedTree)
{
  //-------------
  // Reduced tree
  //-------------
  
  reducedTree -> SetBranchAddress("mH",           &vars.mH);
  reducedTree -> SetBranchAddress("totEvents",    &vars.totEvents);
  reducedTree -> SetBranchAddress("crossSection", &vars.crossSection);
  reducedTree -> SetBranchAddress("dataFlag",     &vars.dataFlag);
  reducedTree -> SetBranchAddress("runId",        &vars.runId);
  reducedTree -> SetBranchAddress("lumiId",       &vars.lumiId);
  reducedTree -> SetBranchAddress("eventId",      &vars.eventId);
  reducedTree -> SetBranchAddress("eventNaiveId", &vars.eventNaiveId);
  
  
  // MVA variables
  reducedTree -> SetBranchAddress("mva", &vars.mva);  
  
  
  // PU variables
  reducedTree -> SetBranchAddress("PUit_n", &vars.PUit_n);
  reducedTree -> SetBranchAddress("rhoForIsolation", &vars.rhoForIsolation);
  reducedTree -> SetBranchAddress("rhoForJets", &vars.rhoForJets);
  
  
  // PV variables
  reducedTree -> SetBranchAddress("PV_n",              &vars.PV_n);
  reducedTree -> SetBranchAddress("PV_d0",             &vars.PV_d0);
  reducedTree -> SetBranchAddress("PV_nTracks",        &vars.PV_nTracks);
  reducedTree -> SetBranchAddress("PV_ndof",           &vars.PV_ndof);
  reducedTree -> SetBranchAddress("PV_normalizedChi2", &vars.PV_normalizedChi2);
  reducedTree -> SetBranchAddress("PV_z",              &vars.PV_z);
  
  
  // lepton variables
  vars.p_lep = new ROOT::Math::XYZTVector;
  vars.p_lep_KF = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("lep",         &vars.p_lep);
  reducedTree -> SetBranchAddress("lep_KF",      &vars.p_lep_KF);
  reducedTree -> SetBranchAddress("lep_charge",  &vars.lep_charge);
  reducedTree -> SetBranchAddress("lep_flavour", &vars.lep_flavour);
  reducedTree -> SetBranchAddress("lep_pt",      &vars.lep_pt);
  reducedTree -> SetBranchAddress("lep_eta",     &vars.lep_eta);
  reducedTree -> SetBranchAddress("lep_zepp",    &vars.lep_zepp);
  reducedTree -> SetBranchAddress("lep_z",       &vars.lep_z);
  reducedTree -> SetBranchAddress("lep_dxy_BS",  &vars.lep_dxy_BS);
  reducedTree -> SetBranchAddress("lep_dz_BS",   &vars.lep_dz_BS);
  reducedTree -> SetBranchAddress("lep_dxy_PV",  &vars.lep_dxy_PV);
  reducedTree -> SetBranchAddress("lep_edxy_PV", &vars.lep_edxy_PV);
  reducedTree -> SetBranchAddress("lep_dz_PV",   &vars.lep_dz_PV);
  reducedTree -> SetBranchAddress("lep_tkIso",   &vars.lep_tkIso);
  reducedTree -> SetBranchAddress("lep_emIso",   &vars.lep_emIso);
  reducedTree -> SetBranchAddress("lep_hadIso",  &vars.lep_hadIso);
  reducedTree -> SetBranchAddress("lep_isEB",              &vars.lep_isEB);
  reducedTree -> SetBranchAddress("lep_etaSC",             &vars.lep_etaSC);
  reducedTree -> SetBranchAddress("lep_sigmaIetaIeta",     &vars.lep_sigmaIetaIeta);
  reducedTree -> SetBranchAddress("lep_DphiIn",            &vars.lep_DphiIn);
  reducedTree -> SetBranchAddress("lep_DetaIn",            &vars.lep_DetaIn);
  reducedTree -> SetBranchAddress("lep_HOverE",            &vars.lep_HOverE);
  reducedTree -> SetBranchAddress("lep_mishits",           &vars.lep_mishits);
  reducedTree -> SetBranchAddress("lep_dist",              &vars.lep_dist);
  reducedTree -> SetBranchAddress("lep_dcot",              &vars.lep_dcot);
  reducedTree -> SetBranchAddress("lep_tracker",                    &vars.lep_tracker);
  reducedTree -> SetBranchAddress("lep_standalone",                 &vars.lep_standalone);
  reducedTree -> SetBranchAddress("lep_global",                     &vars.lep_global);
  reducedTree -> SetBranchAddress("lep_normalizedChi2",             &vars.lep_normalizedChi2);
  reducedTree -> SetBranchAddress("lep_numberOfMatches",            &vars.lep_numberOfMatches);
  reducedTree -> SetBranchAddress("lep_numberOfValidTrackerHits",   &vars.lep_numberOfValidTrackerHits);
  reducedTree -> SetBranchAddress("lep_numberOfValidMuonHits",      &vars.lep_numberOfValidMuonHits);
  reducedTree -> SetBranchAddress("lep_pixelLayersWithMeasurement", &vars.lep_pixelLayersWithMeasurement);
  
  
  // met variables
  vars.p_met = new ROOT::Math::XYZTVector;
  vars.p_nu = new ROOT::Math::XYZTVector;
  vars.p_nu_KF = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("met",   &vars.p_met);
  reducedTree -> SetBranchAddress("nu",    &vars.p_nu);
  reducedTree -> SetBranchAddress("nu_KF", &vars.p_nu_KF);
  reducedTree -> SetBranchAddress("met_et",      &vars.met_et);
  reducedTree -> SetBranchAddress("lepMet_mt",   &vars.lepMet_mt);
  reducedTree -> SetBranchAddress("lepMet_Dphi", &vars.lepMet_Dphi);
  
  
  // jet variables
  reducedTree -> SetBranchAddress("nJets",      &vars.nJets);
  reducedTree -> SetBranchAddress("nJets_cnt",  &vars.nJets_cnt);
  reducedTree -> SetBranchAddress("nJets_fwd",  &vars.nJets_fwd);
  
  reducedTree -> SetBranchAddress("nJets_et20",     &vars.nJets_et20);
  reducedTree -> SetBranchAddress("nJets_cnt_et20", &vars.nJets_cnt_et20);
  reducedTree -> SetBranchAddress("nJets_fwd_et20", &vars.nJets_fwd_et20);
  reducedTree -> SetBranchAddress("nJets_btw_et20", &vars.nJets_btw_et20);
  reducedTree -> SetBranchAddress("nJets_et25",     &vars.nJets_et25);
  reducedTree -> SetBranchAddress("nJets_cnt_et25", &vars.nJets_cnt_et25);
  reducedTree -> SetBranchAddress("nJets_fwd_et25", &vars.nJets_fwd_et25);
  reducedTree -> SetBranchAddress("nJets_btw_et25", &vars.nJets_btw_et25);
  reducedTree -> SetBranchAddress("nJets_et30",     &vars.nJets_et30);
  reducedTree -> SetBranchAddress("nJets_cnt_et30", &vars.nJets_cnt_et30);
  reducedTree -> SetBranchAddress("nJets_fwd_et30", &vars.nJets_fwd_et30);
  reducedTree -> SetBranchAddress("nJets_btw_et30", &vars.nJets_btw_et30);
  
  reducedTree -> SetBranchAddress("jets_bTag1", &vars.jets_bTag1);
  reducedTree -> SetBranchAddress("jets_bTag2", &vars.jets_bTag2);
  
  
  // leading jet variables
  vars.p_leadingJ = new ROOT::Math::XYZTVector; 
  reducedTree -> SetBranchAddress("leadingJ",                     &vars.p_leadingJ);
  reducedTree -> SetBranchAddress("leadingJ_charge",              &vars.leadingJ_charge);
  reducedTree -> SetBranchAddress("leadingJ_bTag",                &vars.leadingJ_bTag);
  reducedTree -> SetBranchAddress("leadingJ_dzAvg",               &vars.leadingJ_dzAvg);
  reducedTree -> SetBranchAddress("leadingJ_dzAvgCut",            &vars.leadingJ_dzAvgCut);
  reducedTree -> SetBranchAddress("leadingJ_chargedMultiplicity", &vars.leadingJ_chargedMultiplicity);
  
  
  // W-jet variables
  vars.p_WJ1    = new ROOT::Math::XYZTVector;
  vars.p_WJ2    = new ROOT::Math::XYZTVector;
  vars.p_WJ1_KF = new ROOT::Math::XYZTVector;
  vars.p_WJ2_KF = new ROOT::Math::XYZTVector;
  vars.p_WJJ    = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("WJ1",                     &vars.p_WJ1);
  reducedTree -> SetBranchAddress("WJ2",                     &vars.p_WJ2);
  reducedTree -> SetBranchAddress("WJ1_KF",                  &vars.p_WJ1_KF);
  reducedTree -> SetBranchAddress("WJ2_KF",                  &vars.p_WJ2_KF);
  reducedTree -> SetBranchAddress("WJJ",                     &vars.p_WJJ);
  reducedTree -> SetBranchAddress("WJ1_charge",              &vars.WJ1_charge);
  reducedTree -> SetBranchAddress("WJ2_charge",              &vars.WJ2_charge);
  reducedTree -> SetBranchAddress("WJ1_lep_Dphi",            &vars.WJ1_lep_Dphi);
  reducedTree -> SetBranchAddress("WJ1_met_Dphi",            &vars.WJ1_met_Dphi);
  reducedTree -> SetBranchAddress("WJ2_lep_Dphi",            &vars.WJ2_lep_Dphi);
  reducedTree -> SetBranchAddress("WJ2_met_Dphi",            &vars.WJ2_met_Dphi);
  reducedTree -> SetBranchAddress("WJ1_zepp",                &vars.WJ1_zepp);
  reducedTree -> SetBranchAddress("WJ2_zepp",                &vars.WJ2_zepp);
  reducedTree -> SetBranchAddress("WJ1_bTag",                &vars.WJ1_bTag);
  reducedTree -> SetBranchAddress("WJ2_bTag",                &vars.WJ2_bTag);
  reducedTree -> SetBranchAddress("WJ1_dzAvg",               &vars.WJ1_dzAvg);
  reducedTree -> SetBranchAddress("WJ2_dzAvg",               &vars.WJ2_dzAvg);
  reducedTree -> SetBranchAddress("WJ1_dzAvgCut",            &vars.WJ1_dzAvgCut);
  reducedTree -> SetBranchAddress("WJ2_dzAvgCut",            &vars.WJ2_dzAvgCut);
  reducedTree -> SetBranchAddress("WJ1_chargedMultiplicity", &vars.WJ1_chargedMultiplicity);
  reducedTree -> SetBranchAddress("WJ2_chargedMultiplicity", &vars.WJ2_chargedMultiplicity);
  
  reducedTree -> SetBranchAddress("WJJ_Deta", &vars.WJJ_Deta);
  reducedTree -> SetBranchAddress("WJJ_Dphi", &vars.WJJ_Dphi);
  reducedTree -> SetBranchAddress("WJJ_DR",   &vars.WJJ_DR);
  reducedTree -> SetBranchAddress("WJJ_et",   &vars.WJJ_et);
  reducedTree -> SetBranchAddress("WJJ_m",    &vars.WJJ_m);
  reducedTree -> SetBranchAddress("WJJ_zepp", &vars.WJJ_zepp);
  
  reducedTree -> SetBranchAddress("lepWJJ_pt1", &vars.lepWJJ_pt1);
  reducedTree -> SetBranchAddress("lepWJJ_pt2", &vars.lepWJJ_pt2);
  reducedTree -> SetBranchAddress("lepWJJ_pt3", &vars.lepWJJ_pt3);
  
  
  // Higgs variables
  reducedTree -> SetBranchAddress("lepMetW_pt",   &vars.lepMetW_pt);
  reducedTree -> SetBranchAddress("lepMetW_mt",   &vars.lepMetW_mt);
  reducedTree -> SetBranchAddress("lepMetW_Dphi", &vars.lepMetW_Dphi);
  reducedTree -> SetBranchAddress("lepNuW_m",     &vars.lepNuW_m);
  reducedTree -> SetBranchAddress("lepNuW_zepp",  &vars.lepNuW_zepp);
  
  reducedTree -> SetBranchAddress("lepNuW_m_KF", &vars.lepNuW_m_KF);
  reducedTree -> SetBranchAddress("chi2_KF",     &vars.chi2_KF);
  reducedTree -> SetBranchAddress("ndf_KF",      &vars.ndf_KF);
  
  
  // tag-jet variables
  vars.p_tagJ1 = new ROOT::Math::XYZTVector;
  vars.p_tagJ2 = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("tagJ1",                     &vars.p_tagJ1);
  reducedTree -> SetBranchAddress("tagJ2",                     &vars.p_tagJ2);
  reducedTree -> SetBranchAddress("tagJ1_charge",              &vars.tagJ1_charge);
  reducedTree -> SetBranchAddress("tagJ2_charge",              &vars.tagJ2_charge);
  reducedTree -> SetBranchAddress("tagJ1_bTag",                &vars.tagJ1_bTag);
  reducedTree -> SetBranchAddress("tagJ2_bTag",                &vars.tagJ2_bTag);
  reducedTree -> SetBranchAddress("tagJ1_dzAvg",               &vars.tagJ1_dzAvg);
  reducedTree -> SetBranchAddress("tagJ2_dzAvg",               &vars.tagJ2_dzAvg);
  reducedTree -> SetBranchAddress("tagJ1_dzAvgCut",            &vars.tagJ1_dzAvgCut);
  reducedTree -> SetBranchAddress("tagJ2_dzAvgCut",            &vars.tagJ2_dzAvgCut);
  reducedTree -> SetBranchAddress("tagJ1_chargedMultiplicity", &vars.tagJ1_chargedMultiplicity);
  reducedTree -> SetBranchAddress("tagJ2_chargedMultiplicity", &vars.tagJ2_chargedMultiplicity);
  
  reducedTree -> SetBranchAddress("tagJJ_Deta", &vars.tagJJ_Deta);
  reducedTree -> SetBranchAddress("tagJJ_Dphi", &vars.tagJJ_Dphi);
  reducedTree -> SetBranchAddress("tagJJ_DR",   &vars.tagJJ_DR);
  reducedTree -> SetBranchAddress("tagJJ_m",    &vars.tagJJ_m);
  
  // third jet variables
  vars.p_thirdJ = new ROOT::Math::XYZTVector; 
  reducedTree -> SetBranchAddress("thirdJ", &vars.p_thirdJ);
  reducedTree -> SetBranchAddress("thirdJ_charge",    &vars.thirdJ_charge);
  reducedTree -> SetBranchAddress("thirdJ_bTag",      &vars.thirdJ_bTag);
  reducedTree -> SetBranchAddress("thirdJ_dzAvg",     &vars.thirdJ_dzAvg);
  reducedTree -> SetBranchAddress("thirdJ_dzAvgCut",  &vars.thirdJ_dzAvgCut);
  reducedTree -> SetBranchAddress("thirdJ_WJJDeta",   &vars.thirdJ_WJJDeta);
  reducedTree -> SetBranchAddress("thirdJ_WJJDphi",   &vars.thirdJ_WJJDphi);
  reducedTree -> SetBranchAddress("thirdJ_WJJDR",     &vars.thirdJ_WJJDR);
  reducedTree -> SetBranchAddress("thirdJ_tagJJDeta", &vars.thirdJ_tagJJDeta);
  reducedTree -> SetBranchAddress("thirdJ_tagJJDphi", &vars.thirdJ_tagJJDphi);
  reducedTree -> SetBranchAddress("thirdJ_tagJJDR",   &vars.thirdJ_tagJJDR);
}



void AddVBFAnalysisTreeBranches(VBFAnalysisVariables& vars, TTree* reducedTree)
{
  reducedTree -> Branch("lepNuW_cphi",  &vars.lepNuW_cphi,   "lepNuW_cphi/F");
  reducedTree -> Branch("lepNuZ_cphi",  &vars.lepNuZ_cphi,   "lepNuZ_cphi/F");
  reducedTree -> Branch("lep_ctheta",   &vars.lep_ctheta,     "lep_ctheta/F");
  reducedTree -> Branch("WJ1_ctheta",   &vars.WJ1_ctheta,     "WJ1_ctheta/F");
  reducedTree -> Branch("lepNu_ctheta", &vars.lepNu_ctheta, "lepNu_ctheta/F");
}
