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
  
  reducedTree -> SetBranchAddress("eventWeight", &vars.eventWeight);
  
  
  // MVA variables
  reducedTree -> SetBranchAddress("mva", &vars.mva);  
  
  
  // PU variables
  reducedTree -> SetBranchAddress("PUit_n", &vars.PUit_n);
  reducedTree -> SetBranchAddress("PUoot_n", &vars.PUoot_n);
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
  reducedTree -> SetBranchAddress("lep",         &vars.p_lep);
  reducedTree -> SetBranchAddress("lep_charge",  &vars.lep_charge);
  reducedTree -> SetBranchAddress("lep_flavour", &vars.lep_flavour);
  reducedTree -> SetBranchAddress("lep_pt",      &vars.lep_pt);
  reducedTree -> SetBranchAddress("lep_eta",     &vars.lep_eta);
  reducedTree -> SetBranchAddress("lep_phi",     &vars.lep_phi);
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
  
  
  // btag variables
  reducedTree -> SetBranchAddress("nBTag_TCHEL_pt20",  &vars.nBTag_TCHEL_pt20);
  reducedTree -> SetBranchAddress("nBTag_TCHEM_pt20",  &vars.nBTag_TCHEM_pt20);
  reducedTree -> SetBranchAddress("nBTag_TCHPM_pt20",  &vars.nBTag_TCHPM_pt20);
  reducedTree -> SetBranchAddress("nBTag_TCHPT_pt20",  &vars.nBTag_TCHPT_pt20);
  reducedTree -> SetBranchAddress("nBTag_SSVHEM_pt20", &vars.nBTag_SSVHEM_pt20);
  
  reducedTree -> SetBranchAddress("nBTag_TCHEL_pt25",  &vars.nBTag_TCHEL_pt25);
  reducedTree -> SetBranchAddress("nBTag_TCHEM_pt25",  &vars.nBTag_TCHEM_pt25);
  reducedTree -> SetBranchAddress("nBTag_TCHPM_pt25",  &vars.nBTag_TCHPM_pt25);
  reducedTree -> SetBranchAddress("nBTag_TCHPT_pt25",  &vars.nBTag_TCHPT_pt25);
  reducedTree -> SetBranchAddress("nBTag_SSVHEM_pt25", &vars.nBTag_SSVHEM_pt25);
  
  reducedTree -> SetBranchAddress("nBTag_TCHEL_pt30",  &vars.nBTag_TCHEL_pt30);
  reducedTree -> SetBranchAddress("nBTag_TCHEM_pt30",  &vars.nBTag_TCHEM_pt30);
  reducedTree -> SetBranchAddress("nBTag_TCHPM_pt30",  &vars.nBTag_TCHPM_pt30);
  reducedTree -> SetBranchAddress("nBTag_TCHPT_pt30",  &vars.nBTag_TCHPT_pt30);
  reducedTree -> SetBranchAddress("nBTag_SSVHEM_pt30", &vars.nBTag_SSVHEM_pt30);
  
  
  // met variables
  vars.p_met = new ROOT::Math::XYZTVector;
  vars.p_nu = new ROOT::Math::XYZTVector;
  vars.p_nu1 = new ROOT::Math::XYZTVector;
  vars.p_nu2 = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("met",   &vars.p_met);
  reducedTree -> SetBranchAddress("nu",    &vars.p_nu);
  reducedTree -> SetBranchAddress("nu1",   &vars.p_nu1);
  reducedTree -> SetBranchAddress("nu2",   &vars.p_nu2);
  reducedTree -> SetBranchAddress("met_et",      &vars.met_et);
  reducedTree -> SetBranchAddress("met_phi",     &vars.met_phi);
  reducedTree -> SetBranchAddress("lepMet_pt",   &vars.lepMet_pt);
  reducedTree -> SetBranchAddress("lepMet_mt",   &vars.lepMet_mt);
  reducedTree -> SetBranchAddress("lepMet_Dphi", &vars.lepMet_Dphi);
  
  
  // jet variables
  reducedTree -> SetBranchAddress("nJets",      &vars.nJets);
  reducedTree -> SetBranchAddress("nJets_cnt",  &vars.nJets_cnt);
  reducedTree -> SetBranchAddress("nJets_fwd",  &vars.nJets_fwd);
  
  reducedTree -> SetBranchAddress("nJets_pt20",     &vars.nJets_pt20);
  reducedTree -> SetBranchAddress("nJets_cnt_pt20", &vars.nJets_cnt_pt20);
  reducedTree -> SetBranchAddress("nJets_fwd_pt20", &vars.nJets_fwd_pt20);
  reducedTree -> SetBranchAddress("nJets_btw_pt20", &vars.nJets_btw_pt20);
  reducedTree -> SetBranchAddress("nJets_pt25",     &vars.nJets_pt25);
  reducedTree -> SetBranchAddress("nJets_cnt_pt25", &vars.nJets_cnt_pt25);
  reducedTree -> SetBranchAddress("nJets_fwd_pt25", &vars.nJets_fwd_pt25);
  reducedTree -> SetBranchAddress("nJets_btw_pt25", &vars.nJets_btw_pt25);
  reducedTree -> SetBranchAddress("nJets_pt30",     &vars.nJets_pt30);
  reducedTree -> SetBranchAddress("nJets_cnt_pt30", &vars.nJets_cnt_pt30);
  reducedTree -> SetBranchAddress("nJets_fwd_pt30", &vars.nJets_fwd_pt30);
  reducedTree -> SetBranchAddress("nJets_btw_pt30", &vars.nJets_btw_pt30);
  
  reducedTree -> SetBranchAddress("jets_bTag1", &vars.jets_bTag1);
  reducedTree -> SetBranchAddress("jets_bTag2", &vars.jets_bTag2);
  
  
  // leading jet variables
  vars.p_leadingJ = new ROOT::Math::XYZTVector; 
  reducedTree -> SetBranchAddress("leadingJ",                     &vars.p_leadingJ);
  reducedTree -> SetBranchAddress("leadingJ_bTag",                &vars.leadingJ_bTag);
  reducedTree -> SetBranchAddress("leadingJ_ptD",                 &vars.leadingJ_ptD);
  reducedTree -> SetBranchAddress("leadingJ_chargedMultiplicity", &vars.leadingJ_chargedMultiplicity);
  reducedTree -> SetBranchAddress("leadingJ_neutralMultiplicity", &vars.leadingJ_neutralMultiplicity);
  
  
  // W-jet variables
  vars.p_WJ1    = new ROOT::Math::XYZTVector;
  vars.p_WJ2    = new ROOT::Math::XYZTVector;
  vars.p_WJJ    = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("WJ1",                     &vars.p_WJ1);
  reducedTree -> SetBranchAddress("WJ2",                     &vars.p_WJ2);
  reducedTree -> SetBranchAddress("WJ1_pt",                  &vars.WJ1_pt);
  reducedTree -> SetBranchAddress("WJ2_pt",                  &vars.WJ2_pt);
  reducedTree -> SetBranchAddress("WJ1_eta",                  &vars.WJ1_eta);
  reducedTree -> SetBranchAddress("WJ2_eta",                  &vars.WJ2_eta);
  reducedTree -> SetBranchAddress("WJ1_phi",                  &vars.WJ1_phi);
  reducedTree -> SetBranchAddress("WJ2_phi",                  &vars.WJ2_phi);
  reducedTree -> SetBranchAddress("WJ1_lep_Dphi",            &vars.WJ1_lep_Dphi);
  reducedTree -> SetBranchAddress("WJ1_met_Dphi",            &vars.WJ1_met_Dphi);
  reducedTree -> SetBranchAddress("WJ2_lep_Dphi",            &vars.WJ2_lep_Dphi);
  reducedTree -> SetBranchAddress("WJ2_met_Dphi",            &vars.WJ2_met_Dphi);
  reducedTree -> SetBranchAddress("WJ1_zepp",                &vars.WJ1_zepp);
  reducedTree -> SetBranchAddress("WJ2_zepp",                &vars.WJ2_zepp);
  reducedTree -> SetBranchAddress("WJ1_bTag",                &vars.WJ1_bTag);
  reducedTree -> SetBranchAddress("WJ2_bTag",                &vars.WJ2_bTag);
  reducedTree -> SetBranchAddress("WJ1_ptD",                 &vars.WJ1_ptD);
  reducedTree -> SetBranchAddress("WJ2_ptD",                 &vars.WJ2_ptD);
  reducedTree -> SetBranchAddress("WJ1_chargedMultiplicity", &vars.WJ1_chargedMultiplicity);
  reducedTree -> SetBranchAddress("WJ2_chargedMultiplicity", &vars.WJ2_chargedMultiplicity);
  reducedTree -> SetBranchAddress("WJ1_neutralMultiplicity", &vars.WJ1_neutralMultiplicity);
  reducedTree -> SetBranchAddress("WJ2_neutralMultiplicity", &vars.WJ2_neutralMultiplicity);

  reducedTree -> SetBranchAddress("WJJ",      &vars.p_WJJ);
  reducedTree -> SetBranchAddress("WJJ_Deta", &vars.WJJ_Deta);
  reducedTree -> SetBranchAddress("WJJ_Dphi", &vars.WJJ_Dphi);
  reducedTree -> SetBranchAddress("WJJ_DR",   &vars.WJJ_DR);
  reducedTree -> SetBranchAddress("WJJ_pt",   &vars.WJJ_pt);
  reducedTree -> SetBranchAddress("WJJ_m",    &vars.WJJ_m);
  reducedTree -> SetBranchAddress("WJJ_zepp", &vars.WJJ_zepp);
  
  reducedTree -> SetBranchAddress("lepWJJ_pt1", &vars.lepWJJ_pt1);
  reducedTree -> SetBranchAddress("lepWJJ_pt2", &vars.lepWJJ_pt2);
  reducedTree -> SetBranchAddress("lepWJJ_pt3", &vars.lepWJJ_pt3);
  
  
  // Higgs variables
  reducedTree -> SetBranchAddress("lepMetW_pt",       &vars.lepMetW_pt);
  reducedTree -> SetBranchAddress("lepMetW_mt",       &vars.lepMetW_mt);
  reducedTree -> SetBranchAddress("lepMetW_Dphi",     &vars.lepMetW_Dphi);
  reducedTree -> SetBranchAddress("lepNu_m",          &vars.lepNu_m);
  reducedTree -> SetBranchAddress("lepNu_nSolutions", &vars.lepNu_nSolutions);
  reducedTree -> SetBranchAddress("lepW_pt",          &vars.lepW_pt);
  reducedTree -> SetBranchAddress("lepNuW_m",         &vars.lepNuW_m);
  reducedTree -> SetBranchAddress("lepNuW_m1",        &vars.lepNuW_m1);
  reducedTree -> SetBranchAddress("lepNuW_m2",        &vars.lepNuW_m2);
  reducedTree -> SetBranchAddress("lepNuW_zepp",      &vars.lepNuW_zepp);
  
  
  // tag-jet variables
  vars.p_tagJ1 = new ROOT::Math::XYZTVector;
  vars.p_tagJ2 = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("tagJ1",                     &vars.p_tagJ1);
  reducedTree -> SetBranchAddress("tagJ2",                     &vars.p_tagJ2);
  reducedTree -> SetBranchAddress("tagJ1_bTag",                &vars.tagJ1_bTag);
  reducedTree -> SetBranchAddress("tagJ2_bTag",                &vars.tagJ2_bTag);
  reducedTree -> SetBranchAddress("tagJ1_ptD",                 &vars.tagJ1_ptD);
  reducedTree -> SetBranchAddress("tagJ2_ptD",                 &vars.tagJ2_ptD);
  reducedTree -> SetBranchAddress("tagJ1_chargedMultiplicity", &vars.tagJ1_chargedMultiplicity);
  reducedTree -> SetBranchAddress("tagJ2_chargedMultiplicity", &vars.tagJ2_chargedMultiplicity);
  reducedTree -> SetBranchAddress("tagJ1_neutralMultiplicity", &vars.tagJ1_neutralMultiplicity);
  reducedTree -> SetBranchAddress("tagJ2_neutralMultiplicity", &vars.tagJ2_neutralMultiplicity);
  
  reducedTree -> SetBranchAddress("tagJJ_Deta", &vars.tagJJ_Deta);
  reducedTree -> SetBranchAddress("tagJJ_Dphi", &vars.tagJJ_Dphi);
  reducedTree -> SetBranchAddress("tagJJ_DR",   &vars.tagJJ_DR);
  reducedTree -> SetBranchAddress("tagJJ_m",    &vars.tagJJ_m);
  
  
  // third jet variables
  vars.p_thirdJ = new ROOT::Math::XYZTVector; 
  reducedTree -> SetBranchAddress("thirdJ", &vars.p_thirdJ);
  reducedTree -> SetBranchAddress("thirdJ_bTag",      &vars.thirdJ_bTag);
  reducedTree -> SetBranchAddress("thirdJ_WJJDeta",   &vars.thirdJ_WJJDeta);
  reducedTree -> SetBranchAddress("thirdJ_WJJDphi",   &vars.thirdJ_WJJDphi);
  reducedTree -> SetBranchAddress("thirdJ_WJJDR",     &vars.thirdJ_WJJDR);
  reducedTree -> SetBranchAddress("thirdJ_tagJJDeta", &vars.thirdJ_tagJJDeta);
  reducedTree -> SetBranchAddress("thirdJ_tagJJDphi", &vars.thirdJ_tagJJDphi);
  reducedTree -> SetBranchAddress("thirdJ_tagJJDR",   &vars.thirdJ_tagJJDR);
  
  
  // MC variables
  vars.p_mc_lep = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("mc_lep", &vars.p_mc_lep);
  reducedTree -> SetBranchAddress("mc_lep_charge",  &vars.mc_lep_charge);
  reducedTree -> SetBranchAddress("mc_lep_flavour", &vars.mc_lep_flavour);
  reducedTree -> SetBranchAddress("mc_lep_pt",      &vars.mc_lep_pt);
  reducedTree -> SetBranchAddress("mc_lep_eta",     &vars.mc_lep_eta);
  reducedTree -> SetBranchAddress("mc_lep_phi",     &vars.mc_lep_phi);
  
  vars.p_mc_nu = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("mc_nu", vars.p_mc_nu);
  reducedTree -> SetBranchAddress("mc_nu_charge",  &vars.mc_nu_charge);
  reducedTree -> SetBranchAddress("mc_nu_flavour", &vars.mc_nu_flavour);
  reducedTree -> SetBranchAddress("mc_nu_pt",      &vars.mc_nu_pt);
  reducedTree -> SetBranchAddress("mc_nu_eta",     &vars.mc_nu_eta);
  reducedTree -> SetBranchAddress("mc_nu_phi",     &vars.mc_nu_phi);
  
  reducedTree -> SetBranchAddress("mc_lepNu_pt",   &vars.mc_lepNu_pt);
  reducedTree -> SetBranchAddress("mc_lepNu_mt",   &vars.mc_lepNu_mt);
  reducedTree -> SetBranchAddress("mc_lepNu_Dphi", &vars.mc_lepNu_Dphi);
  
  vars.p_mc_WJ1 = new ROOT::Math::XYZTVector;
  vars.p_mc_WJ2 = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("mc_WJ1", &vars.p_mc_WJ1);  
  reducedTree -> SetBranchAddress("mc_WJ2", &vars.p_mc_WJ2);  
  reducedTree -> SetBranchAddress("mc_WJ1_pt",  &vars.mc_WJ1_pt);
  reducedTree -> SetBranchAddress("mc_WJ2_pt",  &vars.mc_WJ2_pt);
  reducedTree -> SetBranchAddress("mc_WJ1_eta", &vars.mc_WJ1_eta);
  reducedTree -> SetBranchAddress("mc_WJ2_eta", &vars.mc_WJ2_eta);
  reducedTree -> SetBranchAddress("mc_WJ1_phi", &vars.mc_WJ1_phi);
  reducedTree -> SetBranchAddress("mc_WJ2_phi", &vars.mc_WJ2_phi);
  
  reducedTree -> SetBranchAddress("mc_WJJ_Deta", &vars.mc_WJJ_Deta);
  reducedTree -> SetBranchAddress("mc_WJJ_Dphi", &vars.mc_WJJ_Dphi);
  reducedTree -> SetBranchAddress("mc_WJJ_DR",   &vars.mc_WJJ_DR);
  reducedTree -> SetBranchAddress("mc_WJJ_pt",   &vars.mc_WJJ_pt);
  reducedTree -> SetBranchAddress("mc_WJJ_m",    &vars.mc_WJJ_m);
  
  vars.p_mc_W1 = new ROOT::Math::XYZTVector;
  vars.p_mc_W2 = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("mc_W1", &vars.p_mc_W1);  
  reducedTree -> SetBranchAddress("mc_W2", &vars.p_mc_W2);  
  reducedTree -> SetBranchAddress("mc_W1_pt",  &vars.mc_W1_pt);
  reducedTree -> SetBranchAddress("mc_W2_pt",  &vars.mc_W2_pt);
  reducedTree -> SetBranchAddress("mc_W1_eta", &vars.mc_W1_eta);
  reducedTree -> SetBranchAddress("mc_W2_eta", &vars.mc_W2_eta);
  reducedTree -> SetBranchAddress("mc_W1_phi", &vars.mc_W1_phi);
  reducedTree -> SetBranchAddress("mc_W2_phi", &vars.mc_W2_phi);
  reducedTree -> SetBranchAddress("mc_W1_m",   &vars.mc_W1_m);
  reducedTree -> SetBranchAddress("mc_W2_m",   &vars.mc_W2_m);
  
  vars.p_mc_H = new ROOT::Math::XYZTVector;
  reducedTree -> SetBranchAddress("mc_H", &vars.p_mc_H);  
  reducedTree -> SetBranchAddress("mc_H_pt",  &vars.mc_H_pt);
  reducedTree -> SetBranchAddress("mc_H_eta", &vars.mc_H_eta);
  reducedTree -> SetBranchAddress("mc_H_phi", &vars.mc_H_phi);
  reducedTree -> SetBranchAddress("mc_H_m",   &vars.mc_H_m);
  
}



void AddVBFAnalysisTreeBranches(VBFAnalysisVariables& vars, TTree* reducedTree)
{
  reducedTree -> Branch("lepNuW_cphi",  &vars.lepNuW_cphi,   "lepNuW_cphi/F");
  reducedTree -> Branch("lepNuZ_cphi",  &vars.lepNuZ_cphi,   "lepNuZ_cphi/F");
  reducedTree -> Branch("lep_ctheta",   &vars.lep_ctheta,     "lep_ctheta/F");
  reducedTree -> Branch("WJ1_ctheta",   &vars.WJ1_ctheta,     "WJ1_ctheta/F");
  reducedTree -> Branch("lepNu_ctheta", &vars.lepNu_ctheta, "lepNu_ctheta/F");
  
  reducedTree -> Branch("WJ1_QGLikelihood", &vars.WJ1_QGLikelihood);
  reducedTree -> Branch("WJ2_QGLikelihood", &vars.WJ2_QGLikelihood);
  
  vars.p_lep_KF = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("lep_KF", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_lep_KF);
  vars.p_lep_KF1 = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("lep_KF1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_lep_KF1);
  vars.p_lep_KF2 = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("lep_KF2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_lep_KF2);
  
  vars.p_nu_KF = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("nu_KF", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_nu_KF);
  vars.p_nu_KF1 = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("nu_KF1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_nu_KF1);
  vars.p_nu_KF2 = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("nu_KF2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_nu_KF2);
  
  vars.p_WJ1_KF = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("WJ1_KF", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ1_KF);
  vars.p_WJ1_KF1 = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("WJ1_KF1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ1_KF1);
  vars.p_WJ1_KF2 = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("WJ1_KF2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ1_KF2);
  
  vars.p_WJ2_KF = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("WJ2_KF", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ2_KF);
  vars.p_WJ2_KF1 = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("WJ2_KF1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ2_KF1);
  vars.p_WJ2_KF2 = new ROOT::Math::XYZTVector;
  reducedTree -> Branch("WJ2_KF2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ2_KF2);
  
  reducedTree -> Branch("lepNuW_m_KF",  &vars.lepNuW_m_KF,   "lepNuW_m_KF/F");
  reducedTree -> Branch("lepNuW_m_KF1", &vars.lepNuW_m_KF1, "lepNuW_m_KF1/F");
  reducedTree -> Branch("lepNuW_m_KF2", &vars.lepNuW_m_KF2, "lepNuW_m_KF2/F");
  
  reducedTree -> Branch("chi2_KF", &vars.chi2_KF,  "chi2_KF/F");
  reducedTree -> Branch("chi2_KF1",&vars.chi2_KF1,"chi2_KF1/F");
  reducedTree -> Branch("chi2_KF2",&vars.chi2_KF2,"chi2_KF2/F");
  
  reducedTree -> Branch("ndf_KF", &vars.ndf_KF,  "ndf_KF/I");
  reducedTree -> Branch("ndf_KF1",&vars.ndf_KF1,"ndf_KF1/I");
  reducedTree -> Branch("ndf_KF2",&vars.ndf_KF2,"ndf_KF2/I");
  
  reducedTree -> Branch("status_KF", &vars.status_KF,  "status_KF/I");
  reducedTree -> Branch("status_KF1",&vars.status_KF1,"status_KF1/I");
  reducedTree -> Branch("status_KF2",&vars.status_KF2,"status_KF2/I");
}



void InitializeVBFAnalysisTree(VBFAnalysisVariables& vars)
{
  vars.lepNuW_cphi = -1.;
  vars.lepNuZ_cphi = -1.;
  vars.lep_ctheta = -1.;
  vars.WJ1_ctheta = -1.;
  vars.lepNu_ctheta = -1.;
  
  vars.WJ1_QGLikelihood = -1.;
  vars.WJ2_QGLikelihood = -1.;
  
  vars.p_lep_KF = NULL;
  vars.p_nu_KF = NULL;
  vars.p_WJ1_KF = NULL;
  vars.p_WJ2_KF = NULL;
  
  vars.lepNuW_m_KF = -1.;
  vars.chi2_KF = -1.;
  vars.ndf_KF = -1;
  vars.status_KF = -1;
}
