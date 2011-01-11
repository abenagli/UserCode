
#include "WplusWminusRatioVariables.h"



void InitializeWplusWminusRatioVariables(WplusWminusRatioVariables& vars, const std::string& name, const std::string& outputTreeFileName, const int& nStep)
{
  //-----------
  // histograms
  //-----------
  
  vars.histograms = new hFactory(name);
  vars.histograms2 = new h2Factory(name);
  
  vars.histograms -> add_h1("PV_d0",             "", 1000,   0.,   1., nStep+1);
  vars.histograms -> add_h1("PV_nTracks",        "",  100,   0., 100., nStep+1);
  vars.histograms -> add_h1("PV_ndof",           "",  200,   0., 200., nStep+1);
  vars.histograms -> add_h1("PV_normalizedChi2", "", 1000,   0.,  10., nStep+1);
  vars.histograms -> add_h1("PV_z",              "", 3000, -30.,  30., nStep+1);
  
  vars.histograms -> add_h1("mva", "", 2000, -10., 10., nStep+1);
  
  vars.histograms -> add_h1("ele_tkIso_EB",         "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_emIso_EB",         "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_hadIso_EB",        "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_fbrem_EB",         "", 3000, -2.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_HoverE_EB",        "", 3000,  0.000,  3.000, nStep+1);
  vars.histograms -> add_h1("ele_eOverP_EB",        "", 1000,  0.000, 10.000, nStep+1);
  vars.histograms -> add_h1("ele_DphiIn_EB",        "", 1000, -0.500,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_DetaIn_EB",        "", 1000, -0.100,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_sigmaIetaIeta_EB", "", 1000,  0.000,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_dxy_EB",           "", 1000, -0.500,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_dz_EB",            "", 1000, -0.500,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_tkIso_EE",         "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_emIso_EE",         "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_hadIso_EE",        "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_fbrem_EE",         "", 3000, -2.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_HoverE_EE",        "", 3000,  0.000,  3.000, nStep+1);
  vars.histograms -> add_h1("ele_eOverP_EE",        "", 1000,  0.000, 10.000, nStep+1);
  vars.histograms -> add_h1("ele_DphiIn_EE",        "", 1000, -0.500,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_DetaIn_EE",        "", 1000, -0.100,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_sigmaIetaIeta_EE", "", 1000,  0.000,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_dxy_EE",           "", 1000, -0.500,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_dz_EE",            "", 1000, -0.500,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_3DipSig_EB",       "", 2000, -100.0,  100.0,  nStep+1);
  vars.histograms -> add_h1("ele_3DipSig_EE",       "", 2000, -100.0,  100.0,  nStep+1);
  
  vars.histograms -> add_h1("mu_tkIso",   "", 2000,    0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("mu_emIso",   "", 2000,    0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("mu_hadIso",  "", 2000,    0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("mu_dxy",     "", 1000,   -0.5,   0.5, nStep+1);
  vars.histograms -> add_h1("mu_dz",      "", 1000,   -0.5,   0.5, nStep+1);
  vars.histograms -> add_h1("mu_3DipSig", "", 2000, -100.0, 100.0, nStep+1);
  vars.histograms -> add_h1("mu_tracker",    "", 2, 0., 2., nStep+1);
  vars.histograms -> add_h1("mu_standalone", "", 2, 0., 2., nStep+1);
  vars.histograms -> add_h1("mu_global",     "", 2, 0., 2., nStep+1);
  vars.histograms -> add_h1("mu_normalizedChi2",           "", 1000, 0.,  10., nStep+1);
  vars.histograms -> add_h1("mu_numberOfValidTrackerHits", "",  100, 0., 100., nStep+1);
  vars.histograms -> add_h1("mu_numberOfValidMuonHits",    "",  100, 0., 100., nStep+1);
  
  vars.histograms -> add_h1("caloMetOverGenMet",  "", 1000, 0., 10., nStep+1);
  vars.histograms -> add_h1("type1MetOverGenMet", "", 1000, 0., 10., nStep+1);
  vars.histograms -> add_h1("pfMetOverGenMet",    "", 1000, 0., 10., nStep+1);  
  
  vars.histograms -> add_h1("lepMet_mt",    "", 3000, 0., 3000., nStep+1);
  vars.histograms -> add_h1("lepMet_mt_p",  "", 3000, 0., 3000., nStep+1);
  vars.histograms -> add_h1("lepMet_mt_m",  "", 3000, 0., 3000., nStep+1);
  
  vars.histograms -> add_h1("jets_emEnergyFraction", "",  3000,  -1.0,   2.0, nStep+1);
  vars.histograms -> add_h1("jets_etaEtaMoment",     "",  1000,   0.0,   0.1, nStep+1);
  vars.histograms -> add_h1("jets_phiPhiMoment",     "",  1000,   0.0,   0.1, nStep+1);
  vars.histograms -> add_h1("jets_etaPhiMoment",     "",  1000,  -0.1,   0.1, nStep+1);
  vars.histograms -> add_h1("jets_fHPD",             "",  1000,   0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("jets_fRBX",             "",  1000,   0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("jets_n90Hits",          "",   200,   0.0, 200.0, nStep+1);
  vars.histograms -> add_h1("jets_cnt_emEnergyFraction", "",  3000,  -1.0,   2.0, nStep+1);
  vars.histograms -> add_h1("jets_cnt_etaEtaMoment",     "",  1000,   0.0,   0.1, nStep+1);
  vars.histograms -> add_h1("jets_cnt_phiPhiMoment",     "",  1000,   0.0,   0.1, nStep+1);
  vars.histograms -> add_h1("jets_cnt_etaPhiMoment",     "",  1000,  -0.1,   0.1, nStep+1);
  vars.histograms -> add_h1("jets_cnt_fHPD",             "",  1000,   0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("jets_cnt_fRBX",             "",  1000,   0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("jets_cnt_n90Hits",          "",   200,   0.0, 200.0, nStep+1);
  vars.histograms -> add_h1("jets_fwd_emEnergyFraction", "",  3000,  -1.0,   2.0, nStep+1);
  vars.histograms -> add_h1("jets_fwd_etaEtaMoment",     "",  1000,   0.0,   0.1, nStep+1);
  vars.histograms -> add_h1("jets_fwd_phiPhiMoment",     "",  1000,   0.0,   0.1, nStep+1);
  vars.histograms -> add_h1("jets_fwd_etaPhiMoment",     "",  1000,  -0.1,   0.1, nStep+1);
  vars.histograms -> add_h1("jets_fwd_fHPD",             "",  1000,   0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("jets_fwd_fRBX",             "",  1000,   0.0,   1.0, nStep+1);
  vars.histograms -> add_h1("jets_fwd_n90Hits",          "",   200,   0.0, 200.0, nStep+1);
  
  vars.histograms -> add_h1("jets_chargedHadronEnergyFraction", "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_neutralHadronEnergyFraction", "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_chargedEmEnergyFraction",     "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_neutralEmEnergyFraction",     "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_chargedMuEnergyFraction",     "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_chargedMultiplicity",         "",  100,  0., 100., nStep+1);
  vars.histograms -> add_h1("jets_neutralMultiplicity",         "",  100,  0., 100., nStep+1);
  vars.histograms -> add_h1("jets_muonMultiplicity",            "",  100,  0., 100., nStep+1);
  vars.histograms -> add_h1("jets_cnt_chargedHadronEnergyFraction", "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_cnt_neutralHadronEnergyFraction", "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_cnt_chargedEmEnergyFraction",     "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_cnt_neutralEmEnergyFraction",     "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_cnt_chargedMuEnergyFraction",     "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_cnt_chargedMultiplicity",         "",  100,  0., 100., nStep+1);
  vars.histograms -> add_h1("jets_cnt_neutralMultiplicity",         "",  100,  0., 100., nStep+1);
  vars.histograms -> add_h1("jets_cnt_muonMultiplicity",            "",  100,  0., 100., nStep+1);
  vars.histograms -> add_h1("jets_fwd_chargedHadronEnergyFraction", "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_fwd_neutralHadronEnergyFraction", "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_fwd_chargedEmEnergyFraction",     "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_fwd_neutralEmEnergyFraction",     "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_fwd_chargedMuEnergyFraction",     "", 3000, -1.,   2., nStep+1);
  vars.histograms -> add_h1("jets_fwd_chargedMultiplicity",         "",  100,  0., 100., nStep+1);
  vars.histograms -> add_h1("jets_fwd_neutralMultiplicity",         "",  100,  0., 100., nStep+1);
  vars.histograms -> add_h1("jets_fwd_muonMultiplicity",            "",  100,  0., 100., nStep+1);

  vars.histograms -> add_h1("jetLep_DR", "", 5000,     0.,   10., nStep+1);  
  vars.histograms -> add_h1("jets_bTag",      "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("jets_cnt_bTag",  "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("jets_fwd_bTag",  "", 2000,  -100.,  100., nStep+1);
  
  vars.histograms -> add_h1("jets_bTag1", "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("jets_bTag2", "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("jets_bTag3", "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("jets_bTag4", "", 2000,  -100.,  100., nStep+1);

  vars.histograms -> add_h1("leadingJetLep_DR", "", 5000, 0., 10., nStep+1);  
  
  vars.histograms2 -> add_h2("jetsjets_Deta_vs_m", "", 1000, 0., 1000., 200,  -10.,  10., nStep+1);
  vars.histograms2 -> add_h2("jetsjets_Dphi_vs_m", "", 1000, 0., 1000., 200,  -10.,  10., nStep+1);
  vars.histograms2 -> add_h2("jetsjets_DR_vs_m",   "", 1000, 0., 1000., 200,  -10.,  10., nStep+1);

  vars.histograms -> add_h1("WJJ_chargeSum",  "", 2000, -10., 10., nStep+1);
  vars.histograms -> add_h1("WJJ_etRatio",    "", 1000,   0.,  1., nStep+1);
  vars.histograms -> add_h1("WJJ_et1OverEt2", "", 1000,   0.,  5., nStep+1);
  
  vars.histograms -> add_h1("lepWJJ_pt1", "", 3000, 0., 3000., nStep+1);
  vars.histograms -> add_h1("lepWJJ_pt2", "", 3000, 0., 3000., nStep+1);
  vars.histograms -> add_h1("lepWJJ_pt3", "", 3000, 0., 3000., nStep+1);
  
  vars.histograms -> add_h1("lepMetW_mt", "", 3000, 0., 3000., nStep+1);  
  
  vars.histograms -> add_h1("tagJLep_Deta",  "", 200, -10., 10., nStep+1);
  vars.histograms -> add_h1("tagJWJJ_Deta",  "", 200, -10., 10., nStep+1);
  
  vars.histograms -> add_h1("matchWJJ_m",    "", 3000,   0., 3000., nStep+1);  
  vars.histograms -> add_h1("matchWJJ_Deta", "",  200, -10.,   10., nStep+1);  
  vars.histograms -> add_h1("matchWJJ_myDisc", "", 1000, 0.,   50., nStep+1);  
  vars.histograms -> add_h1("matchTagJJ_m",    "", 3000,   0., 3000., nStep+1);  
  vars.histograms -> add_h1("matchTagJJ_Deta", "",  200, -10.,   10., nStep+1);  
  vars.histograms -> add_h1("matchTagJJ_myDisc", "", 1000, 0.,   50., nStep+1);  
  vars.histograms -> add_h1("matchOtherJJ_m",    "", 3000,   0., 3000., nStep+1);  
  vars.histograms -> add_h1("matchOtherJJ_Deta", "",  200, -10.,   10., nStep+1);  
  vars.histograms -> add_h1("matchOtherJJ_myDisc", "", 1000, 0.,   50., nStep+1);  
  
  
  
  //--------------
  // stdHistograms
  //--------------
  
  vars.stdHistograms = new stdHisto(nStep+1, name);
  
  vars.stdHistograms -> Add1("electrons", nStep+1, false);
  vars.stdHistograms -> Add1("muons",     nStep+1, false);
  vars.stdHistograms -> Add1("lep",       nStep+1, false);
  vars.stdHistograms -> Add1("lep_p",     nStep+1, false);
  vars.stdHistograms -> Add1("lep_m",     nStep+1, false);
  vars.stdHistograms -> Add1("met",       nStep+1, false);
  vars.stdHistograms -> Add1("met_p",     nStep+1, false);
  vars.stdHistograms -> Add1("met_m",     nStep+1, false);
  vars.stdHistograms -> Add2("lepMet",    nStep+1, false);
  vars.stdHistograms -> Add2("lepMet_p",  nStep+1, false);
  vars.stdHistograms -> Add2("lepMet_m",  nStep+1, false);
  vars.stdHistograms -> Add1("jets",      nStep+1, false);
  vars.stdHistograms -> Add1("jets_cnt",  nStep+1, false);
  vars.stdHistograms -> Add1("jets_fwd",  nStep+1, false);
  vars.stdHistograms -> Add1("jets_btw",  nStep+1, false);
  vars.stdHistograms -> Add1("leadingJet",nStep+1, false);
  vars.stdHistograms -> Add2("jetsjets",  nStep+1, false);
  vars.stdHistograms -> Add2("WJJ",       nStep+1, false);
  vars.stdHistograms -> Add2("tagJJ",     nStep+1, false);
  vars.stdHistograms -> Add2("lepMetW",   nStep+1, false);
  vars.stdHistograms -> Add1("lepNuW",    nStep+1, false);
  
  
  
  //-------------
  // Profiles
  //--------------
  
  vars.efficiency_tag = new TProfile("efficiency_tag", "efficiency_tag", nStep+1, 0., nStep+1.);
  vars.efficiency_purity_2_tag = new TProfile("efficiency_purity_2tag", "efficiency_purity_2tag", nStep+1, 0., nStep+1.);
  vars.purity_0_tag = new TProfile("purity_0_tag", "purity_0_tag", nStep+1, 0., nStep+1.);
  vars.purity_1_tag = new TProfile("purity_1_tag", "purity_1_tag", nStep+1, 0., nStep+1.);
  vars.purity_2_tag = new TProfile("purity_2_tag", "purity_2_tag", nStep+1, 0., nStep+1.);
  
  vars.efficiency_W = new TProfile("efficiency_W", "efficiency_W", nStep+1, 0., nStep+1.);
  vars.efficiency_purity_2_W = new TProfile("efficiency_purity_2_W", "efficiency_purity_2_W", nStep+1, 0., nStep+1.);
  vars.purity_0_W = new TProfile("purity_0_W", "purity_0_W", nStep+1, 0., nStep+1.);
  vars.purity_1_W = new TProfile("purity_1_W", "purity_1_W", nStep+1, 0., nStep+1.);
  vars.purity_2_W = new TProfile("purity_2_W", "purity_2_W", nStep+1, 0., nStep+1.);
  
  vars.efficiency = new TProfile("efficiency", "efficiency", nStep+1, 0., nStep+1.);
  vars.efficiency_purity_4 = new TProfile("efficiency_purity_4", "efficiency_purity_4", nStep+1, 0., nStep+1.);
  vars.purity_0 = new TProfile("purity_0", "purity_0", nStep+1, 0., nStep+1.);
  vars.purity_1 = new TProfile("purity_1", "purity_1", nStep+1, 0., nStep+1.);
  vars.purity_2 = new TProfile("purity_2", "purity_2", nStep+1, 0., nStep+1.);
  vars.purity_3 = new TProfile("purity_3", "purity_3", nStep+1, 0., nStep+1.);
  vars.purity_4 = new TProfile("purity_4", "purity_4", nStep+1, 0., nStep+1.);

  
  
  
  //--------------
  // Tree
  //--------------

  vars.m_outputTreeFile = new TFile(outputTreeFileName.c_str(), "RECREATE");  
  
  for(int i = 1; i <= nStep; ++i)
  {
      char treeName[50];
      sprintf(treeName, "tree_%d", i);
      vars.m_tree[i] = new TTree(treeName, treeName);
      vars.m_tree[i] -> SetDirectory(vars.m_outputTreeFile);
      
      vars.m_tree[i] -> Branch("mH",           &vars.mH,                     "mH/F");
      vars.m_tree[i] -> Branch("totEvents",    &vars.totEvents,       "totEvents/I");
      vars.m_tree[i] -> Branch("crossSection", &vars.crossSection, "crossSection/F");
      vars.m_tree[i] -> Branch("dataFlag",     &vars.dataFlag,         "dataFlag/I");
      
      vars.m_tree[i] -> Branch("PV_nTracks", &vars.PV_nTracks, "PV_nTracks/I");
      
      vars.m_tree[i] -> Branch("lep_pt",     &vars.lep_pt,         "lep_pt/F");
      vars.m_tree[i] -> Branch("lep_eta",    &vars.lep_eta,       "lep_eta/F");
      vars.m_tree[i] -> Branch("lep_phi",    &vars.lep_phi,       "lep_phi/F");
      vars.m_tree[i] -> Branch("lep_tkIso",  &vars.lep_tkIso,   "lep_tkIso/F");
      vars.m_tree[i] -> Branch("lep_emIso",  &vars.lep_emIso,   "lep_emIso/F");
      vars.m_tree[i] -> Branch("lep_hadIso", &vars.lep_hadIso, "lep_hadIso/F");
      
      vars.m_tree[i] -> Branch("met_et",       &vars.met_et,             "met_et/F");
      vars.m_tree[i] -> Branch("lepMet_mt",    &vars.lepMet_mt,       "lepMet_mt/F");
      vars.m_tree[i] -> Branch("lepMet_Dphi",  &vars.lepMet_Dphi,   "lepMet_Dphi/F");
      vars.m_tree[i] -> Branch("lepMetW_mt",   &vars.lepMetW_mt,     "lepMetW_mt/F");
      vars.m_tree[i] -> Branch("lepMetW_Dphi", &vars.lepMetW_Dphi, "lepMetW_Dphi/F");
      vars.m_tree[i] -> Branch("lepNuW_m",     &vars.lepNuW_m,         "lepNuW_m/F");
      
      vars.m_tree[i] -> Branch("nJets",     &vars.nJets,         "nJets/I");
      vars.m_tree[i] -> Branch("nJets_cnt", &vars.nJets_cnt, "nJets_cnt/I");
      vars.m_tree[i] -> Branch("nJets_fwd", &vars.nJets_fwd, "nJets_fwd/I");
      vars.m_tree[i] -> Branch("nJets_btw", &vars.nJets_btw, "nJets_btw/I");
      
      vars.m_tree[i] -> Branch("WJJ_Deta",    &vars.WJJ_Deta,       "WJJ_Deta/F");
      vars.m_tree[i] -> Branch("WJJ_Dphi",    &vars.WJJ_Dphi,       "WJJ_Dphi/F");
      vars.m_tree[i] -> Branch("WJJ_DR",      &vars.WJJ_DR,           "WJJ_DR/F");
      vars.m_tree[i] -> Branch("WJJ_etaProd", &vars.WJJ_etaProd, "WJJ_etaProd/F");
      vars.m_tree[i] -> Branch("WJJ_m",       &vars.WJJ_m,             "WJJ_m/F");
      vars.m_tree[i] -> Branch("WJJ_et",      &vars.WJJ_et,           "WJJ_et/F");
      vars.m_tree[i] -> Branch("WJ1_e",       &vars.WJ1_e,             "WJ1_e/F");
      vars.m_tree[i] -> Branch("WJ2_e",       &vars.WJ2_e,             "WJ2_e/F");
      vars.m_tree[i] -> Branch("WJ1_et",      &vars.WJ1_et,           "WJ1_et/F");
      vars.m_tree[i] -> Branch("WJ2_et",      &vars.WJ2_et,           "WJ2_et/F");
      vars.m_tree[i] -> Branch("WJ1_eta",     &vars.WJ1_eta,         "WJ1_eta/F");
      vars.m_tree[i] -> Branch("WJ2_eta",     &vars.WJ2_eta,         "WJ2_eta/F");
      vars.m_tree[i] -> Branch("WJ1_phi",     &vars.WJ1_phi,         "WJ1_phi/F");
      vars.m_tree[i] -> Branch("WJ2_phi",     &vars.WJ2_phi,         "WJ2_phi/F");
      vars.m_tree[i] -> Branch("WJ1_bTag",    &vars.WJ1_bTag,       "WJ1_bTag/F");
      vars.m_tree[i] -> Branch("WJ2_bTag",    &vars.WJ2_bTag,       "WJ2_bTag/F");
      
      vars.m_tree[i] -> Branch("tagJJ_Deta",    &vars.tagJJ_Deta,       "tagJJ_Deta/F");
      vars.m_tree[i] -> Branch("tagJJ_Dphi",    &vars.tagJJ_Dphi,       "tagJJ_Dphi/F");
      vars.m_tree[i] -> Branch("tagJJ_DR",      &vars.tagJJ_DR,           "tagJJ_DR/F");
      vars.m_tree[i] -> Branch("tagJJ_etaProd", &vars.tagJJ_etaProd, "tagJJ_etaProd/F");
      vars.m_tree[i] -> Branch("tagJJ_m",       &vars.tagJJ_m,             "tagJJ_m/F");
      vars.m_tree[i] -> Branch("tagJJ_et",      &vars.tagJJ_et,           "tagJJ_et/F");
      vars.m_tree[i] -> Branch("tagJ1_e",       &vars.tagJ1_e,             "tagJ1_e/F");
      vars.m_tree[i] -> Branch("tagJ2_e",       &vars.tagJ2_e,             "tagJ2_e/F");
      vars.m_tree[i] -> Branch("tagJ1_et",      &vars.tagJ1_et,           "tagJ1_et/F");
      vars.m_tree[i] -> Branch("tagJ2_et",      &vars.tagJ2_et,           "tagJ2_et/F");
      vars.m_tree[i] -> Branch("tagJ1_eta",     &vars.tagJ1_eta,         "tagJ1_eta/F");
      vars.m_tree[i] -> Branch("tagJ2_eta",     &vars.tagJ2_eta,         "tagJ2_eta/F");
      vars.m_tree[i] -> Branch("tagJ1_phi",     &vars.tagJ1_phi,         "tagJ1_phi/F");
      vars.m_tree[i] -> Branch("tagJ2_phi",     &vars.tagJ2_phi,         "tagJ2_phi/F");
      vars.m_tree[i] -> Branch("tagJ1_bTag",    &vars.tagJ1_bTag,       "tagJ1_bTag/F");
      vars.m_tree[i] -> Branch("tagJ2_bTag",    &vars.tagJ2_bTag,       "tagJ2_bTag/F");
      
      vars.m_tree[i] -> Branch("lepWJJ_pt1", &vars.lepWJJ_pt1, "lepWJJ_pt1/F");
      vars.m_tree[i] -> Branch("lepWJJ_pt2", &vars.lepWJJ_pt2, "lepWJJ_pt2/F");
      vars.m_tree[i] -> Branch("lepWJJ_pt3", &vars.lepWJJ_pt3, "lepWJJ_pt3/F");
  }
  
}






void ClearWplusWminusRatioVariables(WplusWminusRatioVariables& vars)
{
  vars.lep_pt = -9999.;
  vars.lep_eta = -9999.;
  vars.lep_phi = -9999.;
  vars.lep_charge = -9999.;
  vars.lep_tkIso = -9999.;
  vars.lep_emIso = -9999.;
  vars.lep_hadIso = -9999.;
  
  vars.met_et = -9999.;
  vars.lepMet_mt = -9999.;
  vars.lepMet_Dphi = -9999.;
  vars.lepMetW_mt = -9999.;
  vars.lepMetW_Dphi = -9999.;
  
  vars.WJJ_Deta = -9999.;
  vars.WJJ_Dphi = -9999.;
  vars.WJJ_DR = -9999.;
  vars.WJJ_etaProd = -9999.;
  vars.WJJ_m = -9999.;
  vars.WJJ_et = -9999.;
  vars.WJ1_e = -9999.;
  vars.WJ2_e = -9999.;
  vars.WJ1_et = -9999.;
  vars.WJ2_et = -9999.;
  vars.WJ1_eta = -9999.;
  vars.WJ2_eta = -9999.;
  vars.WJ1_phi = -9999.;
  vars.WJ2_phi = -9999.;
  vars.WJ1_charge = -9999.;
  vars.WJ2_charge = -9999.;
  vars.WJ1_bTag = -9999.;
  vars.WJ2_bTag = -9999.;
  
  vars.tagJJ_Deta = -9999.;
  vars.tagJJ_Dphi = -9999.;
  vars.tagJJ_DR = -9999.;
  vars.tagJJ_etaProd = -9999.;
  vars.tagJJ_m = -9999.;
  vars.tagJJ_et = -9999.;
  vars.tagJ1_e = -9999.;
  vars.tagJ2_e = -9999.;
  vars.tagJ1_et = -9999.;
  vars.tagJ2_et = -9999.;
  vars.tagJ1_eta = -9999.;
  vars.tagJ2_eta = -9999.;
  vars.tagJ1_phi = -9999.;
  vars.tagJ2_phi = -9999.;
  vars.tagJ1_charge = -9999.;
  vars.tagJ2_charge = -9999.;
  vars.tagJ1_bTag = -9999.;
  vars.tagJ2_bTag = -9999.;
  
  vars.lepWJJ_pt1 = -9999.;
  vars.lepWJJ_pt2 = -9999.;
  vars.lepWJJ_pt3 = -9999.;
  
  
  
  
  
  vars.PV_d0 = -1.;
  vars.PV_nTracks = -1;
  vars.PV_ndof = -1;
  vars.PV_normalizedChi2 = -1.;
  vars.PV_z = 0.;  
  
  vars.mva = -9999.;  
  
  vars.selectIt_ele = -1;
  vars.selectIt_mu = -1;
  vars.selectIt_lep = -1;
  vars.lepton = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.lepMet_mt = -1.;
  vars.electrons.clear();
  vars.muons.clear();
  vars.leptons.clear();
  vars.leptonCharges.clear();
  vars.leptonFlavours.clear();    
  vars.leptons_tkIso.clear();
  vars.leptons_emIso.clear();
  vars.leptons_hadIso.clear();
  vars.leptons_dxy.clear();
  vars.leptons_dz.clear();
  vars.leptons_lipSig.clear();
  vars.leptons_tipSig.clear();
  vars.leptons_3DipSig.clear();
  vars.electrons_isEB.clear();
  vars.electrons_fbrem.clear();
  vars.electrons_HoverE.clear();
  vars.electrons_eOverP.clear();
  vars.electrons_DphiIn.clear();
  vars.electrons_DetaIn.clear();
  vars.electrons_sigmaIetaIeta.clear();
  vars.muons_tracker.clear();
  vars.muons_standalone.clear();
  vars.muons_global.clear();
  vars.muons_normalizedChi2.clear();
  vars.muons_numberOfValidTrackerHits.clear();
  vars.muons_numberOfValidMuonHits.clear();
  
  vars.met = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.caloMet = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.type1Met = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.pfMet = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.genMet = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.lepMet = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  
  vars.neutrino = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  
  vars.jets.clear();
  vars.jets_cnt.clear();
  vars.jets_fwd.clear();
  vars.jets_btw.clear();
  vars.nJets = 0;
  vars.nJets_cnt = 0;
  vars.nJets_fwd = 0;
  vars.nJets_btw = 0;
  
  vars.selectIt_leadingJet = -1;
  vars.selectIt_W.clear();
  vars.selectIt_tag.clear();
  vars.matchIt.clear();
  vars.matchIt_W.clear();
  vars.matchIt_tag.clear();
  
  vars.jets_bTag.clear();
  
  vars.jets_charge.clear();
  vars.jets_etaEtaMoment.clear();
  vars.jets_phiPhiMoment.clear();
  vars.jets_etaPhiMoment.clear();
  
  vars.jets_emEnergyFraction.clear();
  vars.jets_fHPD.clear();
  vars.jets_fRBX.clear();
  vars.jets_n90Hits.clear();
  vars.jets_etOrdered.clear();
  vars.jets_bTagOrdered.clear();
  
  vars.jets_chargedHadronEnergy.clear();
  vars.jets_chargedHadronEnergyFraction.clear();
  vars.jets_neutralHadronEnergy.clear();
  vars.jets_neutralHadronEnergyFraction.clear();
  vars.jets_chargedEmEnergy.clear();
  vars.jets_chargedEmEnergyFraction.clear();
  vars.jets_neutralEmEnergy.clear();
  vars.jets_neutralEmEnergyFraction.clear();
  vars.jets_chargedMuEnergy.clear();
  vars.jets_chargedMuEnergyFraction.clear();
  vars.jets_chargedMultiplicity.clear();
  vars.jets_neutralMultiplicity.clear();
  vars.jets_muonMultiplicity.clear();
  
  vars.jet1_W  = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.jet2_W  = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.jet12_W = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.lepW  = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.lepNuW  = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.lepWJJ_ptOrdered.clear();  

  vars.jet1_tag  = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.jet2_tag  = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.jet12_tag = ROOT::Math::XYZTVector(0., 0., 0., 0.);
}






void DeleteWplusWminusRatioVariables(WplusWminusRatioVariables& vars, const int& nStep)
{
  // save tree
  vars.m_outputTreeFile -> cd();
  
  for(int step = 1; step <= nStep; ++step)
    vars.m_tree[step] -> Write();
  
  vars.m_outputTreeFile -> Close();
  delete vars.m_outputTreeFile;
  
  delete vars.histograms;
  delete vars.histograms2;
  delete vars.stdHistograms;
}



void FillTree(WplusWminusRatioVariables& vars, const int& step)
{
  vars.m_tree[step] -> Fill();
}



void FillPVHistograms(WplusWminusRatioVariables& vars, const int& step)
{
  vars.histograms -> Fill("PV_d0", step, vars.PV_d0);
  vars.histograms -> Fill("PV_nTracks", step, vars.PV_nTracks);
  vars.histograms -> Fill("PV_ndof", step, vars.PV_ndof);
  vars.histograms -> Fill("PV_normalizedChi2", step, vars.PV_normalizedChi2);
  vars.histograms -> Fill("PV_z", step, vars.PV_z);
}






void FillLeptonHistograms(WplusWminusRatioVariables& vars, const int& step)
{
  //----------
  // electrons
  if( vars.leptonFlavours.at(vars.selectIt_lep) == "electron" )
  {
    if( vars.electrons_isEB.at(vars.selectIt_ele) == 1 )
    {
      vars.histograms -> Fill("ele_tkIso_EB",  step, vars.leptons_tkIso.at(vars.selectIt_lep)/vars.lepton.pt());        
      vars.histograms -> Fill("ele_emIso_EB",  step, vars.leptons_emIso.at(vars.selectIt_lep)/vars.lepton.pt());
      vars.histograms -> Fill("ele_hadIso_EB", step, vars.leptons_hadIso.at(vars.selectIt_lep)/vars.lepton.pt());
      vars.histograms -> Fill("ele_fbrem_EB",  step, vars.electrons_fbrem.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_HoverE_EB", step, vars.electrons_HoverE.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_eOverP_EB", step, vars.electrons_eOverP.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_DphiIn_EB", step, vars.electrons_DphiIn.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_DetaIn_EB", step, vars.electrons_DetaIn.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_sigmaIetaIeta_EB", step, vars.electrons_sigmaIetaIeta.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_dxy_EB",           step, vars.leptons_dxy.at(vars.selectIt_lep));
      vars.histograms -> Fill("ele_dz_EB",            step, vars.leptons_dz.at(vars.selectIt_lep));
      vars.histograms -> Fill("ele_3DipSig_EB",       step, vars.leptons_3DipSig.at(vars.selectIt_lep));
    }
    
    else
    {
      vars.histograms -> Fill("ele_tkIso_EE",  step, vars.leptons_tkIso.at(vars.selectIt_lep)/vars.lepton.pt());        
      vars.histograms -> Fill("ele_emIso_EE",  step, vars.leptons_emIso.at(vars.selectIt_lep)/vars.lepton.pt());
      vars.histograms -> Fill("ele_hadIso_EE", step, vars.leptons_hadIso.at(vars.selectIt_lep)/vars.lepton.pt());
      vars.histograms -> Fill("ele_fbrem_EE",  step, vars.electrons_fbrem.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_HoverE_EE", step, vars.electrons_HoverE.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_eOverP_EE", step, vars.electrons_eOverP.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_DphiIn_EE", step, vars.electrons_DphiIn.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_DetaIn_EE", step, vars.electrons_DetaIn.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_sigmaIetaIeta_EE", step, vars.electrons_sigmaIetaIeta.at(vars.selectIt_ele));
      vars.histograms -> Fill("ele_dxy_EE",           step, vars.leptons_dxy.at(vars.selectIt_lep));
      vars.histograms -> Fill("ele_dz_EE",            step, vars.leptons_dz.at(vars.selectIt_lep));
      vars.histograms -> Fill("ele_3DipSig_EE",       step, vars.leptons_3DipSig.at(vars.selectIt_lep));
    }
  }
  
  
  
  //----------
  // muons
  
  if( vars.leptonFlavours.at(vars.selectIt_lep) == "muon" )
  {
    vars.histograms -> Fill("mu_tkIso",   step, vars.leptons_tkIso.at(vars.selectIt_lep)/vars.lepton.pt());        
    vars.histograms -> Fill("mu_emIso",   step, vars.leptons_emIso.at(vars.selectIt_lep)/vars.lepton.pt());
    vars.histograms -> Fill("mu_hadIso",  step, vars.leptons_hadIso.at(vars.selectIt_lep)/vars.lepton.pt());
    vars.histograms -> Fill("mu_dxy",     step, vars.leptons_dxy.at(vars.selectIt_lep));
    vars.histograms -> Fill("mu_dz",      step, vars.leptons_dz.at(vars.selectIt_lep));
    vars.histograms -> Fill("mu_3DipSig", step, vars.leptons_3DipSig.at(vars.selectIt_lep));
    vars.histograms -> Fill("mu_tracker",    step, vars.muons_tracker.at(vars.selectIt_mu));    
    vars.histograms -> Fill("mu_standalone", step, vars.muons_standalone.at(vars.selectIt_mu));
    vars.histograms -> Fill("mu_global",     step, vars.muons_global.at(vars.selectIt_mu));    
    vars.histograms -> Fill("mu_normalizedChi2", step, vars.muons_normalizedChi2.at(vars.selectIt_mu));
    vars.histograms -> Fill("mu_numberOfValidTrackerHits", step, vars.muons_numberOfValidTrackerHits.at(vars.selectIt_mu));
    vars.histograms -> Fill("mu_numberOfValidMuonHits",    step, vars.muons_numberOfValidMuonHits.at(vars.selectIt_mu));
  }
  
  vars.stdHistograms -> Fill1(vars.electrons, "electrons", step);
  vars.stdHistograms -> Fill1(vars.muons, "muons", step);
  vars.stdHistograms -> Fill1(vars.lepton, "lep", step);
 
  if( vars.leptonCharges.at(vars.selectIt_lep) == 1 )
    vars.stdHistograms -> Fill1(vars.lepton, "lep_p", step);
  if( vars.leptonCharges.at(vars.selectIt_lep) == -1 )
    vars.stdHistograms -> Fill1(vars.lepton, "lep_m", step);
}



void FillMetHistograms(WplusWminusRatioVariables& vars, const int& step)
{
  vars.histograms -> Fill("lepMet_mt", step, vars.lepMet_mt);
  
  vars.histograms -> Fill("caloMetOverGenMet",  step, vars.caloMet.Et()  / vars.genMet.Et());
  vars.histograms -> Fill("type1MetOverGenMet", step, vars.type1Met.Et() / vars.genMet.Et());
  vars.histograms -> Fill("pfMetOverGenMet",    step, vars.pfMet.Et()    / vars.genMet.Et());
  
  vars.stdHistograms -> Fill1(vars.met, "met", step);
  vars.stdHistograms -> Fill2(vars.lepton, vars.met, "lepMet", step);
  
  if( vars.leptonCharges.at(vars.selectIt_lep) == 1 )
  {
    vars.histograms -> Fill("lepMet_mt_p", step, vars.lepMet_mt);
    vars.stdHistograms -> Fill1(vars.met, "met_p", step);
    vars.stdHistograms -> Fill2(vars.lepton, vars.met, "lepMet_p", step);
  }

  if( vars.leptonCharges.at(vars.selectIt_lep) == -1 )
  {
    vars.histograms -> Fill("lepMet_mt_m", step, vars.lepMet_mt);
    vars.stdHistograms -> Fill1(vars.met, "met_m", step);
    vars.stdHistograms -> Fill2(vars.lepton, vars.met, "lepMet_m", step);
  }
}



void FillJetHistograms(WplusWminusRatioVariables& vars, const int& step, const std::string& jetType, const float& jetEtaCNT, const float& jetEtaFWD)
{
  for(unsigned int jetIt = 0; jetIt < vars.jets.size(); ++jetIt)
  {
    if(jetType == "Calo")
    {
      vars.histograms -> Fill("jets_emEnergyFraction", step, vars.jets_emEnergyFraction.at(jetIt));
      vars.histograms -> Fill("jets_etaEtaMoment",     step, vars.jets_etaEtaMoment.at(jetIt));
      vars.histograms -> Fill("jets_phiPhiMoment",     step, vars.jets_phiPhiMoment.at(jetIt));
      vars.histograms -> Fill("jets_etaPhiMoment",     step, vars.jets_etaPhiMoment.at(jetIt));
      vars.histograms -> Fill("jets_fHPD",             step, vars.jets_fHPD.at(jetIt));
      vars.histograms -> Fill("jets_fRBX",             step, vars.jets_fRBX.at(jetIt));
      vars.histograms -> Fill("jets_n90Hits",          step, vars.jets_n90Hits.at(jetIt));
      
      vars.histograms -> Fill("jets_bTag", step, vars.jets_bTag.at(jetIt));
      
      if( fabs(vars.jets.at(jetIt).eta()) < jetEtaCNT )
      {
        vars.histograms -> Fill("jets_cnt_emEnergyFraction", step, vars.jets_emEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_cnt_etaEtaMoment",     step, vars.jets_etaEtaMoment.at(jetIt));
        vars.histograms -> Fill("jets_cnt_phiPhiMoment",     step, vars.jets_phiPhiMoment.at(jetIt));
        vars.histograms -> Fill("jets_cnt_etaPhiMoment",     step, vars.jets_etaPhiMoment.at(jetIt));
        vars.histograms -> Fill("jets_cnt_fHPD",             step, vars.jets_fHPD.at(jetIt));
        vars.histograms -> Fill("jets_cnt_fRBX",             step, vars.jets_fRBX.at(jetIt));
        vars.histograms -> Fill("jets_cnt_n90Hits",          step, vars.jets_n90Hits.at(jetIt));
        
        vars.histograms -> Fill("jets_cnt_bTag", step, vars.jets_bTag.at(jetIt));
      }
      
      if( fabs(vars.jets.at(jetIt).eta()) >= jetEtaFWD )
      {
        vars.histograms -> Fill("jets_fwd_emEnergyFraction", step, vars.jets_emEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_fwd_etaEtaMoment",     step, vars.jets_etaEtaMoment.at(jetIt));
        vars.histograms -> Fill("jets_fwd_phiPhiMoment",     step, vars.jets_phiPhiMoment.at(jetIt));
        vars.histograms -> Fill("jets_fwd_etaPhiMoment",     step, vars.jets_etaPhiMoment.at(jetIt));
        vars.histograms -> Fill("jets_fwd_fHPD",             step, vars.jets_fHPD.at(jetIt));
        vars.histograms -> Fill("jets_fwd_fRBX",             step, vars.jets_fRBX.at(jetIt));
        vars.histograms -> Fill("jets_fwd_n90Hits",          step, vars.jets_n90Hits.at(jetIt));
        
        vars.histograms -> Fill("jets_fwd_bTag", step, vars.jets_bTag.at(jetIt));
      }
    }
    
    if(jetType == "PF")
    {
      vars.histograms -> Fill("jets_etaEtaMoment",     step, vars.jets_etaEtaMoment.at(jetIt));
      vars.histograms -> Fill("jets_phiPhiMoment",     step, vars.jets_phiPhiMoment.at(jetIt));
      vars.histograms -> Fill("jets_etaPhiMoment",     step, vars.jets_etaPhiMoment.at(jetIt));
      vars.histograms -> Fill("jets_chargedHadronEnergyFraction", step, vars.jets_chargedHadronEnergyFraction.at(jetIt));
      vars.histograms -> Fill("jets_neutralHadronEnergyFraction", step, vars.jets_neutralHadronEnergyFraction.at(jetIt));
      vars.histograms -> Fill("jets_chargedEmEnergyFraction",     step, vars.jets_chargedEmEnergyFraction.at(jetIt));
      vars.histograms -> Fill("jets_neutralEmEnergyFraction",     step, vars.jets_neutralEmEnergyFraction.at(jetIt));
      vars.histograms -> Fill("jets_chargedMuEnergyFraction",     step, vars.jets_chargedMuEnergyFraction.at(jetIt));
      vars.histograms -> Fill("jets_chargedMultiplicity",         step, vars.jets_chargedMultiplicity.at(jetIt));
      vars.histograms -> Fill("jets_neutralMultiplicity",         step, vars.jets_neutralMultiplicity.at(jetIt));
      vars.histograms -> Fill("jets_muonMultiplicity",            step, vars.jets_muonMultiplicity.at(jetIt));
     
      vars.histograms -> Fill("jets_bTag", step, vars.jets_bTag.at(jetIt));
      
      if( fabs(vars.jets.at(jetIt).eta()) < jetEtaCNT )
      {
        vars.histograms -> Fill("jets_cnt_etaEtaMoment",     step, vars.jets_etaEtaMoment.at(jetIt));
        vars.histograms -> Fill("jets_cnt_phiPhiMoment",     step, vars.jets_phiPhiMoment.at(jetIt));
        vars.histograms -> Fill("jets_cnt_etaPhiMoment",     step, vars.jets_etaPhiMoment.at(jetIt));
        vars.histograms -> Fill("jets_cnt_chargedHadronEnergyFraction", step, vars.jets_chargedHadronEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_cnt_neutralHadronEnergyFraction", step, vars.jets_neutralHadronEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_cnt_chargedEmEnergyFraction",     step, vars.jets_chargedEmEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_cnt_neutralEmEnergyFraction",     step, vars.jets_neutralEmEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_cnt_chargedMuEnergyFraction",     step, vars.jets_chargedMuEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_cnt_chargedMultiplicity",         step, vars.jets_chargedMultiplicity.at(jetIt));
        vars.histograms -> Fill("jets_cnt_neutralMultiplicity",         step, vars.jets_neutralMultiplicity.at(jetIt));
        vars.histograms -> Fill("jets_cnt_muonMultiplicity",            step, vars.jets_muonMultiplicity.at(jetIt));
        
        vars.histograms -> Fill("jets_cnt_bTag", step, vars.jets_bTag.at(jetIt));
      }
      
      if( fabs(vars.jets.at(jetIt).eta()) >= jetEtaFWD )
      {
        vars.histograms -> Fill("jets_fwd_etaEtaMoment",     step, vars.jets_etaEtaMoment.at(jetIt));
        vars.histograms -> Fill("jets_fwd_phiPhiMoment",     step, vars.jets_phiPhiMoment.at(jetIt));
        vars.histograms -> Fill("jets_fwd_etaPhiMoment",     step, vars.jets_etaPhiMoment.at(jetIt));
        vars.histograms -> Fill("jets_fwd_chargedHadronEnergyFraction", step, vars.jets_chargedHadronEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_fwd_neutralHadronEnergyFraction", step, vars.jets_neutralHadronEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_fwd_chargedEmEnergyFraction",     step, vars.jets_chargedEmEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_fwd_neutralEmEnergyFraction",     step, vars.jets_neutralEmEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_fwd_chargedMuEnergyFraction",     step, vars.jets_chargedMuEnergyFraction.at(jetIt));
        vars.histograms -> Fill("jets_fwd_chargedMultiplicity",         step, vars.jets_chargedMultiplicity.at(jetIt));
        vars.histograms -> Fill("jets_fwd_neutralMultiplicity",         step, vars.jets_neutralMultiplicity.at(jetIt));
        vars.histograms -> Fill("jets_fwd_muonMultiplicity",            step, vars.jets_muonMultiplicity.at(jetIt));
        
        vars.histograms -> Fill("jets_fwd_bTag", step, vars.jets_bTag.at(jetIt));
      }
    }
    
    
    
    float tempDR = deltaR(vars.lepton.eta(), vars.lepton.phi(), vars.jets.at(jetIt).eta(), vars.jets.at(jetIt).phi());
    vars.histograms -> Fill("jetLep_DR", step, tempDR);
  }
  
  
  
  for(unsigned int jetIt1 = 0; jetIt1 < vars.jets_cnt.size(); ++jetIt1)
    for(unsigned int jetIt2 = jetIt1+1; jetIt2 < vars.jets_cnt.size(); ++jetIt2)
    {
      ROOT::Math::XYZTVector jet1 = vars.jets_cnt.at(jetIt1);   
      ROOT::Math::XYZTVector jet2 = vars.jets_cnt.at(jetIt2);   
      vars.stdHistograms -> Fill2(jet1, jet2, "jetsjets", step);      
      
      vars.histograms2 -> Fill("jetsjets_Deta_vs_m", step, (jet1+jet2).mass(), deltaEta(jet1.eta(), jet2.eta()));
      vars.histograms2 -> Fill("jetsjets_Dphi_vs_m", step, (jet1+jet2).mass(), deltaPhi(jet1.phi(), jet2.phi()));
      vars.histograms2 -> Fill("jetsjets_DR_vs_m",   step, (jet1+jet2).mass(), deltaR(jet1.eta(), jet1.phi(), jet2.eta(), jet2.phi()));
    }
  
  
  
  if( vars.jets_bTagOrdered.size() > 0)
    vars.histograms -> Fill("jets_bTag1", step, vars.jets_bTagOrdered.at(0));
  if( vars.jets_bTagOrdered.size() > 1)
    vars.histograms -> Fill("jets_bTag2", step, vars.jets_bTagOrdered.at(1));
  if( vars.jets_bTagOrdered.size() > 2)
    vars.histograms -> Fill("jets_bTag3", step, vars.jets_bTagOrdered.at(2));  
  if( vars.jets_bTagOrdered.size() > 3)
    vars.histograms -> Fill("jets_bTag4", step, vars.jets_bTagOrdered.at(3));
    
    
  
  vars.stdHistograms -> Fill1(vars.jets,     "jets",     step);
  vars.stdHistograms -> Fill1(vars.jets_cnt, "jets_cnt", step);
  vars.stdHistograms -> Fill1(vars.jets_fwd, "jets_fwd", step);
  
  
  
  if( vars.selectIt_leadingJet != -1 )
  {
    vars.stdHistograms -> Fill1(vars.jets.at(vars.selectIt_leadingJet), "leadingJet", step);
    
    float tempDR = deltaR(vars.lepton.eta(), vars.lepton.phi(), vars.jets.at(vars.selectIt_leadingJet).eta(), vars.jets.at(vars.selectIt_leadingJet).phi());
    vars.histograms -> Fill("leadingJetLep_DR", step, tempDR);
  }
  
}



void FillWJJHistograms(WplusWminusRatioVariables& vars, const int& step, const std::string& jetType)
{
  
  if( (vars.selectIt_W.at(0) == -1) || (vars.selectIt_W.at(1) == -1) ) return;  
  
  vars.stdHistograms -> Fill2(vars.jet1_W, vars.jet2_W, "WJJ", step);
  vars.stdHistograms -> Fill2(vars.lepMet, vars.jet12_W, "lepMetW", step);
  vars.stdHistograms -> Fill1(vars.lepNuW, "lepNuW", step);
  
  vars.histograms -> Fill("WJJ_chargeSum", step, -1.*vars.lep_charge*(vars.jets_charge.at(vars.selectIt_W.at(0))+vars.jets_charge.at(vars.selectIt_W.at(1))) );
  
  float etSum = vars.jet1_W.Et() + vars.jet2_W.Et();
  vars.histograms -> Fill("WJJ_etRatio",    step, vars.jet1_W.Et() / etSum );
  vars.histograms -> Fill("WJJ_etRatio",    step, vars.jet2_W.Et() / etSum );
  vars.histograms -> Fill("WJJ_et1OverEt2", step, vars.jet1_W.Et() / vars.jet2_W.Et() );
  
  vars.histograms -> Fill("lepWJJ_pt1", step, vars.lepWJJ_ptOrdered.at(0));
  vars.histograms -> Fill("lepWJJ_pt2", step, vars.lepWJJ_ptOrdered.at(1));
  vars.histograms -> Fill("lepWJJ_pt3", step, vars.lepWJJ_ptOrdered.at(2));
  
  vars.histograms -> Fill("lepMetW_mt", step, vars.lepMetW_mt);
}



void FillTagJJHistograms(WplusWminusRatioVariables& vars, const int& step, const std::string& jetType)
{
  if( (vars.selectIt_tag.at(0) != -1) && (vars.selectIt_tag.at(1) != -1) )
  {
    vars.stdHistograms -> Fill2(vars.jet1_tag, vars.jet2_tag, "tagJJ", step);
    vars.stdHistograms -> Fill1(vars.jets_btw, "jets_btw", step);
    
    float etaMax = vars.tagJ1_eta;
    
    vars.histograms -> Fill("tagJLep_Deta", step, deltaEta(etaMax, vars.lep_eta) );
    vars.histograms -> Fill("tagJWJJ_Deta", step, deltaEta(etaMax, vars.jet12_W.eta()) );
  }
  
  
  
  if( (vars.selectIt_tag.at(0) != -1) && (vars.selectIt_tag.at(1) == -1) )
  {
    float etaMax = vars.tagJ1_eta;
    
    vars.histograms -> Fill("tagJLep_Deta", step, deltaEta(etaMax, vars.lep_eta) );
    vars.histograms -> Fill("tagJWJJ_Deta", step, deltaEta(etaMax, vars.jet12_W.eta()) );
  }
}



void FillMVAHistograms(WplusWminusRatioVariables& vars, const int& step)
{
  vars.histograms -> Fill("mva", step, vars.mva);
}






void SetPVVariables(WplusWminusRatioVariables& vars, treeReader& reader)
{
  vars.PV_d0 = reader.GetFloat("PV_d0")->at(0);
  vars.PV_nTracks = reader.GetInt("PV_nTracks")->at(0);
  vars.PV_ndof = reader.GetInt("PV_ndof")->at(0);
  vars.PV_normalizedChi2 = reader.GetFloat("PV_normalizedChi2")->at(0);
  vars.PV_z = reader.GetFloat("PV_z")->at(0);
}



void SetElectronVariables(WplusWminusRatioVariables& vars, treeReader& reader, const int& eleIt)
{
  vars.electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
  vars.leptons.push_back( reader.Get4V("electrons")->at(eleIt) );
  vars.leptonCharges.push_back( reader.GetFloat("electrons_charge")->at(eleIt) );
  vars.leptonFlavours.push_back("electron");
  
  vars.leptons_tkIso.push_back(reader.GetFloat("electrons_tkIsoR03")->at(eleIt));
  vars.leptons_emIso.push_back(reader.GetFloat("electrons_emIsoR03")->at(eleIt));
  vars.leptons_hadIso.push_back(reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt)+
			        reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt));
  vars.leptons_dxy.push_back(reader.GetFloat("electrons_dxy")->at(eleIt));
  vars.leptons_dz.push_back(reader.GetFloat("electrons_dz")->at(eleIt));
  vars.leptons_3DipSig.push_back(reader.GetFloat("electrons_3DipSignificance")->at(eleIt));
  
  vars.electrons_isEB.push_back(reader.GetInt("electrons_isEB")->at(eleIt));
  vars.electrons_fbrem.push_back(reader.GetFloat("electrons_fbrem")->at(eleIt));
  vars.electrons_HoverE.push_back(reader.GetFloat("electrons_hOverE")->at(eleIt));
  vars.electrons_eOverP.push_back(reader.GetFloat("electrons_eOverP")->at(eleIt));
  vars.electrons_DphiIn.push_back(reader.GetFloat("electrons_deltaPhiIn")->at(eleIt));
  vars.electrons_DetaIn.push_back(reader.GetFloat("electrons_deltaEtaIn")->at(eleIt));
  vars.electrons_sigmaIetaIeta.push_back(reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt));
}



void SetMuonVariables(WplusWminusRatioVariables& vars, treeReader& reader, const int& muIt)
{
  vars.muons.push_back( reader.Get4V("muons")->at(muIt) );
  vars.leptons.push_back( reader.Get4V("muons")->at(muIt) );
  vars.leptonCharges.push_back( reader.GetFloat("muons_charge")->at(muIt) );
  vars.leptonFlavours.push_back("muon");
  vars.leptons_tkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
  vars.leptons_emIso.push_back(reader.GetFloat("muons_emIsoR03")->at(muIt));
  vars.leptons_hadIso.push_back(reader.GetFloat("muons_hadIsoR03")->at(muIt));
  vars.leptons_dxy.push_back(reader.GetFloat("muons_dxy")->at(muIt));
  vars.leptons_dz.push_back(reader.GetFloat("muons_dz")->at(muIt));
  vars.leptons_3DipSig.push_back(reader.GetFloat("muons_3DipSignificance")->at(muIt));
  
  vars.muons_tracker.push_back(reader.GetInt("muons_tracker")->at(muIt));
  vars.muons_standalone.push_back(reader.GetInt("muons_standalone")->at(muIt));
  vars.muons_global.push_back(reader.GetInt("muons_global")->at(muIt));
  vars.muons_normalizedChi2.push_back(reader.GetFloat("muons_normalizedChi2")->at(muIt));
  vars.muons_numberOfValidTrackerHits.push_back(reader.GetInt("muons_numberOfValidTrackerHits")->at(muIt));
  vars.muons_numberOfValidMuonHits.push_back(reader.GetInt("muons_numberOfValidMuonHits")->at(muIt));
}



void SetLeptonVariables(WplusWminusRatioVariables& vars, treeReader& reader)
{
  vars.lepton = vars.leptons.at(vars.selectIt_lep);
  vars.lep_pt = vars.lepton.pt();
  vars.lep_eta = vars.lepton.eta();
  vars.lep_phi = vars.lepton.phi();
  vars.lep_charge = vars.leptonCharges.at(vars.selectIt_lep);
  vars.lep_tkIso = vars.leptons_tkIso.at(vars.selectIt_lep);
  vars.lep_emIso = vars.leptons_emIso.at(vars.selectIt_lep);
  vars.lep_hadIso = vars.leptons_hadIso.at(vars.selectIt_lep);
  
  
  vars.selectIt_ele = -1;
  if(vars.leptonFlavours.at(vars.selectIt_lep) == "electron")
    for(unsigned int eleIt = 0; eleIt < vars.electrons.size(); ++eleIt)
    {
      ROOT::Math::XYZTVector ele = vars.electrons.at(eleIt);
      if( deltaR(ele.eta(), ele.phi(), vars.lepton.eta(), vars.lepton.phi()) < 0.0001 )
        vars.selectIt_ele = eleIt;
    }
  
  if(vars.leptonFlavours.at(vars.selectIt_lep) == "muon")
    for(unsigned int muIt = 0; muIt < vars.muons.size(); ++muIt)
    {
      ROOT::Math::XYZTVector mu = vars.muons.at(muIt);
        if(deltaR(mu.eta(), mu.phi(), vars.lepton.eta(), vars.lepton.phi()) < 0.0001)
          vars.selectIt_mu = muIt;
    }
}



void SetMetVariables(WplusWminusRatioVariables& vars, treeReader& reader, const std::string& jetType)
{
  // met
  vars.caloMet = reader.Get4V("met")->at(0);
  vars.type1Met = reader.Get4V("type1Met")->at(0);
  vars.pfMet = reader.Get4V("PFMet")->at(0);
  if(jetType == "Calo")
    vars.met = reader.Get4V("type1Met")->at(0);
  if(jetType == "PF")
    vars.met = reader.Get4V("PFMet")->at(0);
  
  vars.lepMet = vars.lepton + vars.met;
  
  vars.met_et = vars.met.Et();
  vars.lepMet_Dphi = deltaPhi(vars.lepton.phi(), vars.met.phi());
  vars.lepMet_mt = sqrt( vars.lepton.mass() * vars.lepton.mass() +
			 2. * vars.lepton.pt() * vars.met.pt() * ( 1 - cos(deltaPhi(vars.lepton.phi(), vars.met.phi()) ) ) );
}



void SetJetVariables(WplusWminusRatioVariables& vars, treeReader& reader, const int& jetIt, const std::string& jetType, const float& jetEtaCNT, const float& jetEtaFWD)
{
  ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);  

  vars.jets.push_back( reader.Get4V("jets")->at(jetIt) );
  if( fabs(jet.eta()) < jetEtaCNT )  vars.jets_cnt.push_back( reader.Get4V("jets")->at(jetIt) );
  if( fabs(jet.eta()) >= jetEtaFWD ) vars.jets_fwd.push_back( reader.Get4V("jets")->at(jetIt) );

  vars.jets_charge.push_back( reader.GetFloat("jets_charge")->at(jetIt) );
  vars.jets_etaEtaMoment.push_back( reader.GetFloat("jets_etaetaMoment")->at(jetIt) );
  vars.jets_phiPhiMoment.push_back( reader.GetFloat("jets_phiphiMoment")->at(jetIt) );
  vars.jets_etaPhiMoment.push_back( reader.GetFloat("jets_etaphiMoment")->at(jetIt) );

  vars.jets_bTag.push_back( reader.GetFloat("jets_trackCountingHighEffBJetTags")->at(jetIt) );
  vars.jets_etOrdered.push_back( reader.Get4V("jets")->at(jetIt).Et() );
  vars.jets_bTagOrdered.push_back( reader.GetFloat("jets_trackCountingHighEffBJetTags")->at(jetIt) );

  if(jetType == "Calo")
    {
      vars.jets_emEnergyFraction.push_back( reader.GetFloat("jets_emEnergyFraction")->at(jetIt) );
      vars.jets_fHPD.push_back( reader.GetFloat("jets_fHPD")->at(jetIt) );
      vars.jets_fRBX.push_back( reader.GetFloat("jets_fRBX")->at(jetIt) );
      vars.jets_n90Hits.push_back( reader.GetFloat("jets_n90Hits")->at(jetIt) );
    }
  
  if(jetType == "PF")
    {
      vars.jets_chargedHadronEnergy.push_back( reader.GetFloat("jets_chargedHadronEnergy")->at(jetIt) );
      vars.jets_chargedHadronEnergyFraction.push_back( reader.GetFloat("jets_chargedHadronEnergyFraction")->at(jetIt) );
      vars.jets_neutralHadronEnergy.push_back( reader.GetFloat("jets_neutralHadronEnergy")->at(jetIt) );
      vars.jets_neutralHadronEnergyFraction.push_back( reader.GetFloat("jets_neutralHadronEnergyFraction")->at(jetIt) );
      vars.jets_chargedEmEnergy.push_back( reader.GetFloat("jets_chargedEmEnergy")->at(jetIt) );
      vars.jets_chargedEmEnergyFraction.push_back( reader.GetFloat("jets_chargedEmEnergyFraction")->at(jetIt) );
      vars.jets_neutralEmEnergy.push_back( reader.GetFloat("jets_neutralEmEnergy")->at(jetIt) );
      vars.jets_neutralEmEnergyFraction.push_back( reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) );
      vars.jets_chargedMuEnergy.push_back( reader.GetFloat("jets_chargedMuEnergy")->at(jetIt) );
      vars.jets_chargedMuEnergyFraction.push_back( reader.GetFloat("jets_chargedMuEnergyFraction")->at(jetIt) );
      vars.jets_chargedMultiplicity.push_back( reader.GetInt("jets_chargedMultiplicity")->at(jetIt) );
      vars.jets_neutralMultiplicity.push_back( reader.GetInt("jets_neutralMultiplicity")->at(jetIt) );
      vars.jets_muonMultiplicity.push_back( reader.GetInt("jets_muonMultiplicity")->at(jetIt) );
    }

}



void SetWJJVariables(WplusWminusRatioVariables& vars, treeReader& reader)
{
  if( (vars.selectIt_W.at(0) == -1) || (vars.selectIt_W.at(1) == -1) ) return;
  
  vars.jet1_W = vars.jets.at(vars.selectIt_W.at(0));
  vars.jet2_W = vars.jets.at(vars.selectIt_W.at(1));
  vars.jet12_W = vars.jet1_W + vars.jet2_W;
  vars.lepW = vars.lepton + vars.jet12_W;
  
  vars.WJJ_Deta = deltaEta(vars.jet1_W.eta(), vars.jet2_W.eta());
  vars.WJJ_Dphi = deltaPhi(vars.jet1_W.phi(), vars.jet2_W.phi());
  vars.WJJ_DR = deltaR(vars.jet1_W.eta(), vars.jet1_W.phi(), vars.jet2_W.eta(), vars.jet2_W.phi());
  vars.WJJ_etaProd = vars.jet1_W.eta()*vars.jet2_W.eta();
  vars.WJJ_m = vars.jet12_W.mass();
  vars.WJJ_et = vars.jet12_W.Et();
  vars.WJ1_e = std::max(vars.jet1_W.energy(), vars.jet2_W.energy());
  vars.WJ2_e = std::min(vars.jet1_W.energy(), vars.jet2_W.energy());
  vars.WJ1_et = std::max(vars.jet1_W.Et(), vars.jet2_W.Et());
  vars.WJ2_et = std::min(vars.jet1_W.Et(), vars.jet2_W.Et());
  if( fabs(vars.jet1_W.eta()) > fabs(vars.jet2_W.eta()) )
    {
      vars.WJ1_eta = vars.jet1_W.eta();
      vars.WJ2_eta = vars.jet2_W.eta();
    }
  else
    {
      vars.WJ1_eta = vars.jet2_W.eta();
      vars.WJ2_eta = vars.jet1_W.eta();
    }
  vars.WJ1_phi = vars.jet1_W.phi();
  vars.WJ2_phi = vars.jet2_W.phi();
  vars.WJ1_charge = vars.jets_charge.at(vars.selectIt_W.at(0));
  vars.WJ2_charge = vars.jets_charge.at(vars.selectIt_W.at(1));
  vars.WJ1_bTag = std::max(vars.jets_bTag.at(vars.selectIt_W.at(0)), vars.jets_bTag.at(vars.selectIt_W.at(1)));
  vars.WJ2_bTag = std::min(vars.jets_bTag.at(vars.selectIt_W.at(0)), vars.jets_bTag.at(vars.selectIt_W.at(1)));

  vars.lepWJJ_ptOrdered.push_back( vars.lepton.pt() );
  vars.lepWJJ_ptOrdered.push_back( vars.WJ1_et );
  vars.lepWJJ_ptOrdered.push_back( vars.WJ2_et );
  std::sort(vars.lepWJJ_ptOrdered.begin(), vars.lepWJJ_ptOrdered.end(), maggiore);
  vars.lepWJJ_pt1 = vars.lepWJJ_ptOrdered.at(0);
  vars.lepWJJ_pt2 = vars.lepWJJ_ptOrdered.at(1);
  vars.lepWJJ_pt3 = vars.lepWJJ_ptOrdered.at(2);
}



void SetTagJJVariables(WplusWminusRatioVariables& vars, treeReader& reader)
{
  if( (vars.selectIt_tag.at(0) != -1) && (vars.selectIt_tag.at(1) != -1) )
  {
    vars.jet1_tag = vars.jets.at(vars.selectIt_tag.at(0));
    vars.jet2_tag = vars.jets.at(vars.selectIt_tag.at(1));
    vars.jet12_tag = vars.jet1_tag + vars.jet2_tag;
    
    vars.tagJJ_Deta = deltaEta(vars.jet1_tag.eta(), vars.jet2_tag.eta());
    vars.tagJJ_Dphi = deltaPhi(vars.jet1_tag.phi(), vars.jet2_tag.phi());
    vars.tagJJ_DR = deltaR(vars.jet1_tag.eta(), vars.jet1_tag.phi(), vars.jet2_tag.eta(), vars.jet2_tag.phi());
    vars.tagJJ_etaProd = vars.jet1_tag.eta()*vars.jet2_tag.eta();
    vars.tagJJ_m = vars.jet12_tag.mass();
    vars.tagJJ_et = vars.jet12_tag.Et();
    vars.tagJ1_e = std::max(vars.jet1_tag.energy(), vars.jet1_tag.energy());
    vars.tagJ2_e = std::min(vars.jet1_tag.energy(), vars.jet2_tag.energy());
    vars.tagJ1_et = std::max(vars.jet1_tag.Et(), vars.jet2_tag.Et());
    vars.tagJ2_et = std::min(vars.jet1_tag.Et(), vars.jet2_tag.Et());
    if( fabs(vars.jet1_tag.eta()) > fabs(vars.jet2_tag.eta()) )
    {
      vars.tagJ1_eta = vars.jet1_tag.eta();
      vars.tagJ2_eta = vars.jet2_tag.eta();
    }
    else
    {
      vars.tagJ1_eta = vars.jet2_tag.eta();
      vars.tagJ2_eta = vars.jet1_tag.eta();
    }
    vars.tagJ1_phi = vars.jet1_tag.phi();
    vars.tagJ2_phi = vars.jet2_tag.phi();
    vars.tagJ1_charge = vars.jets_charge.at(vars.selectIt_tag.at(0));
    vars.tagJ2_charge = vars.jets_charge.at(vars.selectIt_tag.at(1));
    vars.tagJ1_bTag = std::max(vars.jets_bTag.at(vars.selectIt_tag.at(0)), vars.jets_bTag.at(vars.selectIt_tag.at(1)));
    vars.tagJ2_bTag = std::min(vars.jets_bTag.at(vars.selectIt_tag.at(0)), vars.jets_bTag.at(vars.selectIt_tag.at(1)));
    
    
    
    for(int jetIt = 0; jetIt < vars.nJets; ++jetIt)
    {
      
      if( jetIt == vars.selectIt_tag.at(0) ) continue;
      if( jetIt == vars.selectIt_tag.at(1) ) continue;
      if( jetIt == vars.selectIt_W.at(0) ) continue;
      if( jetIt == vars.selectIt_W.at(1) ) continue;
      
      ROOT::Math::XYZTVector jet = vars.jets.at(jetIt);
      float etaMin = std::min(vars.tagJ1_eta, vars.tagJ2_eta);
      float etaMax = std::max(vars.tagJ1_eta, vars.tagJ2_eta);
      if( jet.eta() < etaMin ) continue;
      if( jet.eta() > etaMax ) continue;
      
      vars.jets_btw.push_back(jet);
    }
    vars.nJets_btw = vars.jets_btw.size();
    
  }
  
  
  
  if( (vars.selectIt_tag.at(0) != -1)  && (vars.selectIt_tag.at(1) == -1) )
  {
    vars.jet1_tag = vars.jets.at(vars.selectIt_tag.at(0));
    
    vars.tagJ1_e = vars.jet1_tag.energy();
    vars.tagJ1_et = vars.jet1_tag.Et();
    vars.tagJ1_eta = vars.jet1_tag.eta();
    vars.tagJ1_phi = vars.jet1_tag.phi();
    vars.tagJ1_charge = vars.jets_charge.at(vars.selectIt_tag.at(0));
    vars.tagJ1_bTag = vars.jets_bTag.at(vars.selectIt_tag.at(0));
  }  
  
}


void SetHVariables(WplusWminusRatioVariables& vars, treeReader& reader)
{
  // neutrino
  float alpha = vars.lepton.px()*vars.met.px() + vars.lepton.py()*vars.met.py();
  
  float delta = (alpha + 0.5*80.399*80.399)*(alpha + 0.5*80.399*80.399) -
    vars.lepton.pt()*vars.lepton.pt()*vars.met.pt()*vars.met.pt();
  
  if( alpha - vars.lepton.pt()*vars.met.pt() + 0.5*80.399*80.399 < 0 )
    delta = 0.;
  
  float pz1 = ( vars.lepton.pz()*(alpha + 0.5*80.399*80.399) + vars.lepton.energy()*sqrt(delta) ) / vars.lepton.pt() / vars.lepton.pt();
  float pz2 = ( vars.lepton.pz()*(alpha + 0.5*80.399*80.399) - vars.lepton.energy()*sqrt(delta) ) / vars.lepton.pt() / vars.lepton.pt();
  
  ROOT::Math::XYZTVector nu1(vars.met.px(), vars.met.py(), pz1, sqrt(vars.met.px()*vars.met.px() + vars.met.py()*vars.met.py() + pz1*pz1));
  ROOT::Math::XYZTVector nu2(vars.met.px(), vars.met.py(), pz2, sqrt(vars.met.px()*vars.met.px() + vars.met.py()*vars.met.py() + pz2*pz2));
  
  //std::cout<< "delta = " << delta << "   mass = " << (nu1+vars.lepton).mass() << std::endl;
  
  
  
  if( fabs(vars.mH - (vars.lepW + nu1).mass()) < fabs(vars.mH - (vars.lepW + nu2).mass()) )
    vars.neutrino = nu1;
  else
    vars.neutrino = nu2;

  vars.lepNuW = vars.lepW + vars.neutrino;
  vars.lepNuW_m = vars.lepNuW.mass();
  
  vars.lepMetW_mt = sqrt( vars.lepW.mass()*vars.lepW.mass() + 2. * vars.lepW.pt() * vars.met.pt() * ( 1 - cos(deltaPhi(vars.lepW.phi(), vars.met.phi()) ) ) );
  vars.lepMetW_Dphi = deltaPhi(vars.lepMet.phi(), vars.jet12_W.phi());
  
  //std::cout << "Higgs mt = " << vars.lepMetW_mt << "   Higgs m = " << vars.lepNuW_m << std::endl;
}





void FillPurityProfiles(WplusWminusRatioVariables& vars, const int& step)
{
  int nFound = 0;
  int nFound_W = 0;
  int nFound_tag = 0;
  
  // check tag existance
  if( vars.selectIt_tag.at(0) != -1 )
  {
    ++nFound;
    ++nFound_tag;
  }
  
  if( vars.selectIt_tag.at(1) != -1 )
  {
    ++nFound;
    ++nFound_tag;
  }
  
  // check W existance
  if( vars.selectIt_W.at(0) != -1 )
  {
    ++nFound;
    ++nFound_W;
  }
  
  if( vars.selectIt_W.at(1) != -1 )
  {
    ++nFound;
    ++nFound_W;
  }
  
  
  
  
  // 4 existing
  if( nFound == 4 )
  {
    vars.efficiency -> Fill(step, 1.);
  }
  
  else
  {
    vars.efficiency -> Fill(step, 0.);
  }
  
  // 2 tag existing
  if( nFound_tag == 2 )
  {
    vars.efficiency_tag -> Fill(step, 1.);
  }
  
  else
  {
    vars.efficiency_tag -> Fill(step, 0.);
  }
  
  // 2 W existing
  if( nFound_W == 2 )
  {
    vars.efficiency_W -> Fill(step, 1.);
  }
  
  else
  {
    vars.efficiency_W -> Fill(step, 0.);
  }
  
  
  
  
  
  
  int nMatching = 0;
  int nMatching_W = 0;
  int nMatching_tag = 0;
  
  // check tag jets matching
  if( vars.matchIt.at(0) == vars.selectIt_tag.at(0) )
  {
    ++nMatching;
    ++nMatching_tag;
  }
  
  if( vars.matchIt.at(1) == vars.selectIt_tag.at(1) )
  {
    ++nMatching;
    ++nMatching_tag;
  }
  
  
  // check W jets matching  
  if( vars.matchIt.at(2) == vars.selectIt_W.at(0) )
  {
    ++nMatching;
    ++nMatching_W;
  }
  
  if( vars.matchIt.at(3) == vars.selectIt_W.at(1) )
  {
    ++nMatching;
    ++nMatching_W;
  }
  
  
  
  
  // 4 matching
  if( nMatching == 4 && nFound == 4 )
  {
    vars.purity_0 -> Fill(step, 0.);
    vars.purity_1 -> Fill(step, 0.);
    vars.purity_2 -> Fill(step, 0.);
    vars.purity_3 -> Fill(step, 0.);
    vars.purity_4 -> Fill(step, 1.);
  }
  
  if( nMatching == 3 && nFound == 4 )
  {
    vars.purity_0 -> Fill(step, 0.);
    vars.purity_1 -> Fill(step, 0.);
    vars.purity_2 -> Fill(step, 0.);
    vars.purity_3 -> Fill(step, 1.);
    vars.purity_4 -> Fill(step, 0.);
  }
  
  if( nMatching == 2 && nFound == 4 )
  {
    vars.purity_0 -> Fill(step, 0.);
    vars.purity_1 -> Fill(step, 0.);
    vars.purity_2 -> Fill(step, 1.);
    vars.purity_3 -> Fill(step, 0.);
    vars.purity_4 -> Fill(step, 0.);
  }
  
  if( nMatching == 1 && nFound == 4 )
  {
    vars.purity_0 -> Fill(step, 0.);
    vars.purity_1 -> Fill(step, 1.);
    vars.purity_2 -> Fill(step, 0.);
    vars.purity_3 -> Fill(step, 0.);
    vars.purity_4 -> Fill(step, 0.);
  }
  
  if( nMatching == 0 && nFound == 4 )
  {
    vars.purity_0 -> Fill(step, 1.);
    vars.purity_1 -> Fill(step, 0.);
    vars.purity_2 -> Fill(step, 0.);
    vars.purity_3 -> Fill(step, 0.);
    vars.purity_4 -> Fill(step, 0.);
  }
  
  
  
  // 2 tag matching                                                                                                                                                          
  if( nMatching_tag == 2 && nFound_tag == 2 )
  {
    vars.purity_0_tag -> Fill(step, 0.);
    vars.purity_1_tag -> Fill(step, 0.);
    vars.purity_2_tag -> Fill(step, 1.);
  }
 
  if( nMatching_tag == 1 && nFound_tag == 2 )
  {
    vars.purity_0_tag -> Fill(step, 0.);
    vars.purity_1_tag -> Fill(step, 1.);
    vars.purity_2_tag -> Fill(step, 0.);
  }
  
  if( nMatching_tag == 0 && nFound_tag == 2 )
  {
    vars.purity_0_tag -> Fill(step, 1.);
    vars.purity_1_tag -> Fill(step, 0.);
    vars.purity_2_tag -> Fill(step, 0.);
  }
  
  
  
  // 2 W matching                                                                                                                                                            
  if( nMatching_W == 2 && nFound_W == 2 )
  {
    vars.purity_0_W -> Fill(step, 0.);
    vars.purity_1_W -> Fill(step, 0.);
    vars.purity_2_W -> Fill(step, 1.);
  }
  
  if( nMatching_W == 1 && nFound_W == 2 )
  {
    vars.purity_0_W -> Fill(step, 0.);
    vars.purity_1_W -> Fill(step, 1.);
    vars.purity_2_W -> Fill(step, 0.);
  }
  
  if( nMatching_W == 0 && nFound_W == 2 )
  {
    vars.purity_0_W -> Fill(step, 1.);
    vars.purity_1_W -> Fill(step, 0.);
    vars.purity_2_W -> Fill(step, 0.);
  }
  
  
  
  // fill histograms
  if( nMatching == 4 )
  {
    ROOT::Math::XYZTVector matchTagJ1 = vars.jets.at(vars.matchIt.at(0));
    ROOT::Math::XYZTVector matchTagJ2 = vars.jets.at(vars.matchIt.at(1));
    ROOT::Math::XYZTVector matchWJ1 = vars.jets.at(vars.matchIt.at(2));
    ROOT::Math::XYZTVector matchWJ2 = vars.jets.at(vars.matchIt.at(3));
    
    float myDisc_W = fabs((matchWJ1+matchWJ2).mass() - 80.399)/80.399 + deltaEta(matchWJ1.eta(), matchWJ2.eta()) / 5.;
    float myDisc_tag = fabs((matchTagJ1+matchTagJ2).mass() - 80.399)/80.399 + deltaEta(matchTagJ1.eta(), matchTagJ2.eta()) / 5.;
    
    vars.histograms -> Fill("matchWJJ_m", step, (matchWJ1+matchWJ2).mass());
    vars.histograms -> Fill("matchWJJ_Deta", step, deltaEta(matchWJ1.eta(), matchWJ2.eta()));
    vars.histograms -> Fill("matchWJJ_myDisc", step, myDisc_W);
    vars.histograms -> Fill("matchTagJJ_m", step, (matchTagJ1+matchTagJ2).mass());
    vars.histograms -> Fill("matchTagJJ_Deta", step, deltaEta(matchTagJ1.eta(), matchTagJ2.eta()));
    vars.histograms -> Fill("matchTagJJ_myDisc", step, myDisc_tag);
    
    for(int jetIt1 = 0; jetIt1 < vars.nJets; ++jetIt1)
      for(int jetIt2 = jetIt1+1; jetIt2 < vars.nJets; ++jetIt2)
      {
        if( (jetIt1 == vars.matchIt.at(0)) &&
            (jetIt2 == vars.matchIt.at(1)) )
          continue;
        if( (jetIt1 == vars.matchIt.at(2)) &&
            (jetIt2 == vars.matchIt.at(3)) )
          continue;
        
	ROOT::Math::XYZTVector jet1 = vars.jets.at(jetIt1);
	ROOT::Math::XYZTVector jet2 = vars.jets.at(jetIt2);
        float myDisc_other = fabs((jet1+jet2).mass() - 80.399)/80.399 + deltaEta(jet1.eta(), jet2.eta()) / 5.;        

        vars.histograms -> Fill("matchOtherJJ_m", step, (jet1+jet2).mass());
        vars.histograms -> Fill("matchOtherJJ_Deta", step, deltaEta(jet1.eta(), jet2.eta()));
        vars.histograms -> Fill("matchOtherJJ_myDisc", step, myDisc_other);
      }
  }
}
