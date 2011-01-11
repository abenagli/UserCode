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






/*void InitialiseVBFAnalysisTree(VBFAnalysisVariables& vars, TFile* outputRootFile, const int& nStep, const int& firstStep)
{
  std::cout << ">>>>>>>>>>>> VBFAnalysisVariables::InitializeVBFAnalysisTree" << std::endl;
  
  
  vars.m_outputRootFile = outputRootFile;
  
  for(int i = firstStep; i <= nStep; ++i)
  {
    char treeName[50];
    sprintf(treeName, "ntu_%d", i);
    
    vars.m_reducedTrees[i] = new TTree(treeName, treeName);
    vars.m_reducedTrees[i] -> SetDirectory(vars.m_outputRootFile);
    
    vars.m_reducedTrees[i] -> Branch("mH",           &vars.mH,                     "mH/F");
    vars.m_reducedTrees[i] -> Branch("crossSection", &vars.crossSection, "crossSection/F");
    vars.m_reducedTrees[i] -> Branch("totEvents",    &vars.totEvents,       "totEvents/I");
    vars.m_reducedTrees[i] -> Branch("dataFlag",     &vars.dataFlag,         "dataFlag/I");
    vars.m_reducedTrees[i] -> Branch("runId",        &vars.runId,               "runId/I");
    vars.m_reducedTrees[i] -> Branch("lumiId",       &vars.lumiId,             "lumiId/I");
    vars.m_reducedTrees[i] -> Branch("eventId",      &vars.eventId,           "eventId/I");
    
    
    // mva variables
    vars.m_reducedTrees[i] -> Branch("mva", &vars.mva, "mva/F");
    
    
    // PV variables
    vars.m_reducedTrees[i] -> Branch("PV_nTracks", &vars.PV_nTracks, "PV_nTracks/I");
    
    
    // lepton variables
    vars.m_reducedTrees[i] -> Branch("lep", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_lep);
    vars.m_reducedTrees[i] -> Branch("lep_charge",  &vars.lep_charge,   "lep_charge/F");
    vars.m_reducedTrees[i] -> Branch("lep_flavour", &vars.lep_flavour, "lep_flavour/I");
    vars.m_reducedTrees[i] -> Branch("lep_dxy",     &vars.lep_dxy,         "lep_dxy/F");
    vars.m_reducedTrees[i] -> Branch("lep_dz",      &vars.lep_dz,           "lep_dz/F");
    vars.m_reducedTrees[i] -> Branch("lep_3DipSig", &vars.lep_3DipSig, "lep_3DipSig/F");
    vars.m_reducedTrees[i] -> Branch("lep_tkIso",   &vars.lep_tkIso,     "lep_tkIso/F");
    vars.m_reducedTrees[i] -> Branch("lep_emIso",   &vars.lep_emIso,     "lep_emIso/F");
    vars.m_reducedTrees[i] -> Branch("lep_hadIso",  &vars.lep_hadIso,   "lep_hadIso/F");
    vars.m_reducedTrees[i] -> Branch("lep_isEB",          &vars.lep_isEB,                   "lep_isEB/I");
    vars.m_reducedTrees[i] -> Branch("lep_sigmaIetaIeta", &vars.lep_sigmaIetaIeta, "lep_sigmaIetaIeta/F");
    vars.m_reducedTrees[i] -> Branch("lep_DphiIn",        &vars.lep_DphiIn,               "lep_DphiIn/F");
    vars.m_reducedTrees[i] -> Branch("lep_DetaIn",        &vars.lep_DetaIn,               "lep_DetaIn/F");
    vars.m_reducedTrees[i] -> Branch("lep_HOverE",        &vars.lep_HOverE,               "lep_HOverE/F");
    vars.m_reducedTrees[i] -> Branch("lep_tracker",                  &vars.lep_tracker,                                   "lep_tracker/I");
    vars.m_reducedTrees[i] -> Branch("lep_standalone",               &vars.lep_standalone,                             "lep_standalone/I");
    vars.m_reducedTrees[i] -> Branch("lep_global",                   &vars.lep_global,                                     "lep_global/I");
    vars.m_reducedTrees[i] -> Branch("lep_normalizedChi2",           &vars.lep_normalizedChi2,                     "lep_normalizedChi2/F");
    vars.m_reducedTrees[i] -> Branch("lep_numberOfValidTrackerHits", &vars.lep_numberOfValidTrackerHits, "lep_numberOfValidTrackerHits/I");
    vars.m_reducedTrees[i] -> Branch("lep_numberOfValidMuonHits",    &vars.lep_numberOfValidMuonHits,       "lep_numberOfValidMuonHits/I");
    
    
    // met variables
    vars.m_reducedTrees[i] -> Branch("met", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_met);
    vars.m_reducedTrees[i] -> Branch("lepMet_mt",    &vars.lepMet_mt,       "lepMet_mt/F");
    vars.m_reducedTrees[i] -> Branch("lepMet_Dphi",  &vars.lepMet_Dphi,   "lepMet_Dphi/F");
    
    
    // jet variables
    vars.m_reducedTrees[i] -> Branch("nJets",      &vars.nJets,           "nJets/I");
    vars.m_reducedTrees[i] -> Branch("nJets_cnt",  &vars.nJets_cnt,   "nJets_cnt/I");
    vars.m_reducedTrees[i] -> Branch("nJets_fwd",  &vars.nJets_fwd,   "nJets_fwd/I");
    vars.m_reducedTrees[i] -> Branch("nJets_btw",  &vars.nJets_btw,   "nJets_btw/I"); 
    vars.m_reducedTrees[i] -> Branch("jets_bTag1", &vars.jets_bTag1, "jets_bTag1/F"); 
    vars.m_reducedTrees[i] -> Branch("jets_bTag2", &vars.jets_bTag1, "jets_bTag2/F"); 
    
    
    // leading jet variables
    vars.m_reducedTrees[i] -> Branch("leadingJ", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_leadingJ);
    vars.m_reducedTrees[i] -> Branch("leadingJ_charge", &vars.leadingJ_charge, "leadingJ_charge/F");
    vars.m_reducedTrees[i] -> Branch("leadingJ_bTag",   &vars.leadingJ_bTag,     "leadingJ_bTag/F");
    
    
    // W-jet variables
    vars.m_reducedTrees[i] -> Branch("WJ1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ1);
    vars.m_reducedTrees[i] -> Branch("WJ2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ2);
    vars.m_reducedTrees[i] -> Branch("WJ1_charge", &vars.WJ1_charge, "WJ1_charge/F");
    vars.m_reducedTrees[i] -> Branch("WJ2_charge", &vars.WJ2_charge, "WJ2_charge/F");
    vars.m_reducedTrees[i] -> Branch("WJ1_bTag",   &vars.WJ1_bTag,     "WJ1_bTag/F");
    vars.m_reducedTrees[i] -> Branch("WJ2_bTag",   &vars.WJ2_bTag,     "WJ2_bTag/F");
    
    vars.m_reducedTrees[i] -> Branch("lepWJJ_pt1", &vars.lepWJJ_pt1, "lepWJJ_pt1/F");
    vars.m_reducedTrees[i] -> Branch("lepWJJ_pt2", &vars.lepWJJ_pt2, "lepWJJ_pt2/F");
    vars.m_reducedTrees[i] -> Branch("lepWJJ_pt3", &vars.lepWJJ_pt3, "lepWJJ_pt3/F");
    
    
    // Higgs variables
    vars.m_reducedTrees[i] -> Branch("lepMetW_mt",   &vars.lepMetW_mt,     "lepMetW_mt/F");
    vars.m_reducedTrees[i] -> Branch("lepMetW_Dphi", &vars.lepMetW_Dphi, "lepMetW_Dphi/F");
    vars.m_reducedTrees[i] -> Branch("lepNuW_m",     &vars.lepNuW_m,         "lepNuW_m/F");
    
    
    // tag-jet variables
    vars.m_reducedTrees[i] -> Branch("tagJ1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_tagJ1);
    vars.m_reducedTrees[i] -> Branch("tagJ2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_tagJ2);
    vars.m_reducedTrees[i] -> Branch("tagJ1_charge", &vars.tagJ1_charge, "tagJ1_charge/F");
    vars.m_reducedTrees[i] -> Branch("tagJ2_charge", &vars.tagJ2_charge, "tagJ2_charge/F");
    vars.m_reducedTrees[i] -> Branch("tagJ1_bTag",   &vars.tagJ1_bTag,     "tagJ1_bTag/F");
    vars.m_reducedTrees[i] -> Branch("tagJ2_bTag",   &vars.tagJ2_bTag,     "tagJ2_bTag/F");
  }
}



void FillVBFAnalysisTree(VBFAnalysisVariables& vars, const int& step)
{
  vars.m_reducedTrees[step] -> Fill();
}
*/


/*
void SetVBFAnalysisHistograms(VBFAnalysisVariables& vars, const std::string& outputRootFileName, const int& nStep)
{
  vars.histograms = new hFactory(outputRootFileName);
  vars.histograms2 = new h2Factory(outputRootFileName);
  vars.stdHistograms = new stdHisto(nStep+1, outputRootFileName);
  
  
    
  vars.histograms -> add_h1("PV_d0",             "", 1000,   0.,   1., nStep+1);
  vars.histograms -> add_h1("PV_nTracks",        "",  100,   0., 100., nStep+1);
  vars.histograms -> add_h1("PV_ndof",           "",  200,   0., 200., nStep+1);
  vars.histograms -> add_h1("PV_normalizedChi2", "", 1000,   0.,  10., nStep+1);
  vars.histograms -> add_h1("PV_z",              "", 3000, -30.,  30., nStep+1);
  
  vars.histograms -> add_h1("mva", "", 2000, -10., 10., nStep+1);
  
  vars.histograms -> add_h1("ele_tkIso_EB",         "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_emIso_EB",         "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_hadIso_EB",        "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_HOverE_EB",        "", 3000,  0.000,  3.000, nStep+1);
  vars.histograms -> add_h1("ele_DphiIn_EB",        "", 1000, -0.500,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_DetaIn_EB",        "", 1000, -0.100,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_sigmaIetaIeta_EB", "", 1000,  0.000,  0.100, nStep+1);
  vars.histograms -> add_h1("ele_dxy_EB",           "", 1000, -0.500,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_dz_EB",            "", 1000, -0.500,  0.500, nStep+1);
  vars.histograms -> add_h1("ele_tkIso_EE",         "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_emIso_EE",         "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_hadIso_EE",        "", 2000,  0.000,  1.000, nStep+1);
  vars.histograms -> add_h1("ele_HOverE_EE",        "", 3000,  0.000,  3.000, nStep+1);
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
  
  vars.histograms -> add_h1("lepMet_mt",    "", 3000, 0., 3000., nStep+1);
  vars.histograms -> add_h1("lepMet_mt_p",  "", 3000, 0., 3000., nStep+1);
  vars.histograms -> add_h1("lepMet_mt_m",  "", 3000, 0., 3000., nStep+1);

  vars.histograms -> add_h1("jets_n",     "", 20,  0.,  20., nStep+1);
  vars.histograms -> add_h1("jets_cnt_n", "", 20,  0.,  20., nStep+1);
  vars.histograms -> add_h1("jets_fwd_n", "", 20,  0.,  20., nStep+1);
  vars.histograms -> add_h1("jets_btw_n", "", 20,  0.,  20., nStep+1);
  vars.histograms -> add_h1("jets_bTag1", "", 2000,  -100.,  100., nStep+1);
  vars.histograms -> add_h1("jets_bTag2", "", 2000,  -100.,  100., nStep+1);
  
  vars.histograms -> add_h1("leadingJ_charge", "", 200,  -10,  10., nStep+1);
  vars.histograms -> add_h1("leadingJ_btag",   "", 200, -100, 100., nStep+1);
  vars.histograms -> add_h1("leadingJLep_DR", "", 5000, 0., 10., nStep+1);
  
  vars.histograms -> add_h1("WJJ_chargeSum",  "", 2000, -10., 10., nStep+1);
  vars.histograms -> add_h1("WJJ_etRatio",    "", 1000,   0.,  1., nStep+1);
  vars.histograms -> add_h1("WJJ_et1OverEt2", "", 1000,   0.,  5., nStep+1);

  vars.histograms -> add_h1("lepWJJ_pt1", "", 3000, 0., 3000., nStep+1);
  vars.histograms -> add_h1("lepWJJ_pt2", "", 3000, 0., 3000., nStep+1);
  vars.histograms -> add_h1("lepWJJ_pt3", "", 3000, 0., 3000., nStep+1);
  
  vars.histograms -> add_h1("lepMetW_mt", "", 3000, 0., 3000., nStep+1);
  vars.histograms -> add_h1("lepNuW_mt",   "", 3000, 0., 3000., nStep+1);
  
  vars.histograms -> add_h1("tagJLep_Deta",  "", 200, -10., 10., nStep+1);
  vars.histograms -> add_h1("tagJWJJ_Deta",  "", 200, -10., 10., nStep+1);
  
  
  
  vars.stdHistograms -> Add1("lep",      nStep+1, false);
  vars.stdHistograms -> Add1("lep_p",    nStep+1, false);
  vars.stdHistograms -> Add1("lep_m",    nStep+1, false);
  vars.stdHistograms -> Add1("met",      nStep+1, false);
  vars.stdHistograms -> Add1("met_p",    nStep+1, false);
  vars.stdHistograms -> Add1("met_m",    nStep+1, false);
  vars.stdHistograms -> Add2("lepMet",   nStep+1, false);
  vars.stdHistograms -> Add2("lepMet_p", nStep+1, false);
  vars.stdHistograms -> Add2("lepMet_m", nStep+1, false);
  vars.stdHistograms -> Add1("leadingJ", nStep+1, false);
  vars.stdHistograms -> Add2("WJJ",      nStep+1, false);
  vars.stdHistograms -> Add2("lepMetW",  nStep+1, false);
  vars.stdHistograms -> Add2("tagJJ",    nStep+1, false);
}
*/





void DeleteVBFAnalysisVariables(VBFAnalysisVariables& vars, const int& nStep, const int& firstStep)
{
  // save tree
  //vars.m_outputRootFile -> cd();
  
  //for(int step = firstStep; step <= nStep; ++step)
  //  vars.m_reducedTrees[step] -> Write();
  
  //vars.m_outputRootFile -> Close();
  
  
  //delete vars.histograms;
  //delete vars.histograms2;
  //delete vars.stdHistograms;
}





/*
void FillAllHistograms(VBFAnalysisVariables& vars, const int& step, const int& verbosity)
{
  if( verbosity )
    std::cout << ">>>>>>>>>>>> VBFAnalysisVariables::FillPVHistograms" << std::endl;
  FillPVHistograms(vars, step);
  
  if( verbosity )
    std::cout << ">>>>>>>>>>>> VBFAnalysisVariables::FillLeptonHistograms" << std::endl;
  FillLeptonHistograms(vars, step);
  
  if( verbosity )
    std::cout << ">>>>>>>>>>>> VBFAnalysisVariables::FillMetHistograms" << std::endl;
  FillMetHistograms(vars, step);
  
  if( verbosity )
    std::cout << ">>>>>>>>>>>> VBFAnalysisVariables::FillJetHistograms" << std::endl;
  FillJetHistograms(vars, step);
  
  if( verbosity )
    std::cout << ">>>>>>>>>>>> VBFAnalysisVariables::FillWJJHistograms" << std::endl;
  FillWJJHistograms(vars, step);
  
  if( verbosity )
    std::cout << ">>>>>>>>>>>> VBFAnalysisVariables::FillHiggsHistograms" << std::endl;
  FillHiggsHistograms(vars, step);
  
  if( verbosity )
    std::cout << ">>>>>>>>>>>> VBFAnalysisVariables::FillTagJJHistograms" << std::endl;
  FillTagJJHistograms(vars, step);
  
  if( verbosity )
    std::cout << ">>>>>>>>>>>> VBFAnalysisVariables::FillMVAHistograms" << std::endl;
  FillMVAHistograms(vars, step);
}



void FillPVHistograms(VBFAnalysisVariables& vars, const int& step)
{
  //vars.histograms -> Fill("PV_d0", step, vars.PV_d0);
  vars.histograms -> Fill("PV_nTracks", step, vars.PV_nTracks);
  //vars.histograms -> Fill("PV_ndof", step, vars.PV_ndof);
  //vars.histograms -> Fill("PV_normalizedChi2", step, vars.PV_normalizedChi2);
  //vars.histograms -> Fill("PV_z", step, vars.PV_z);
}



void FillLeptonHistograms(VBFAnalysisVariables& vars, const int& step)
{
  //----------
  // electrons
  if( vars.lep_flavour == 11 )
  {
    if( vars.lep_isEB == 1 )
    {
      vars.histograms -> Fill("ele_tkIso_EB",  step, vars.lep_tkIso/vars.lep.pt());
      vars.histograms -> Fill("ele_emIso_EB",  step, vars.lep_emIso/vars.lep.pt());
      vars.histograms -> Fill("ele_hadIso_EB", step, vars.lep_hadIso/vars.lep.pt());
      vars.histograms -> Fill("ele_DphiIn_EB", step, vars.lep_DphiIn);
      vars.histograms -> Fill("ele_DetaIn_EB", step, vars.lep_DetaIn);
      vars.histograms -> Fill("ele_sigmaIetaIeta_EB", step, vars.lep_sigmaIetaIeta);
      vars.histograms -> Fill("ele_dxy_EB",           step, vars.lep_dxy);
      vars.histograms -> Fill("ele_dz_EB",            step, vars.lep_dz);
      vars.histograms -> Fill("ele_3DipSig_EB",       step, vars.lep_3DipSig);
    }
    
    else
    {
      vars.histograms -> Fill("ele_tkIso_EE",  step, vars.lep_tkIso/vars.lep.pt());
      vars.histograms -> Fill("ele_emIso_EE",  step, vars.lep_emIso/vars.lep.pt());
      vars.histograms -> Fill("ele_hadIso_EE", step, vars.lep_hadIso/vars.lep.pt());
      vars.histograms -> Fill("ele_DphiIn_EE", step, vars.lep_DphiIn);
      vars.histograms -> Fill("ele_DetaIn_EE", step, vars.lep_DetaIn);
      vars.histograms -> Fill("ele_sigmaIetaIeta_EE", step, vars.lep_sigmaIetaIeta);
      vars.histograms -> Fill("ele_dxy_EE",           step, vars.lep_dxy);
      vars.histograms -> Fill("ele_dz_EE",            step, vars.lep_dz);
      vars.histograms -> Fill("ele_3DipSig_EE",       step, vars.lep_3DipSig);
    }
  } 
  
  
  //------
  // muons
  if( vars.lep_flavour == 13 )
  {
    vars.histograms -> Fill("mu_tkIso",   step, vars.lep_tkIso/vars.lep.pt());
    vars.histograms -> Fill("mu_emIso",   step, vars.lep_emIso/vars.lep.pt());
    vars.histograms -> Fill("mu_hadIso",  step, vars.lep_hadIso/vars.lep.pt());
    vars.histograms -> Fill("mu_dxy",     step, vars.lep_dxy);
    vars.histograms -> Fill("mu_dz",      step, vars.lep_dz);
    vars.histograms -> Fill("mu_3DipSig", step, vars.lep_3DipSig);
    vars.histograms -> Fill("mu_tracker",    step, vars.lep_tracker);
    vars.histograms -> Fill("mu_standalone", step, vars.lep_standalone);
    vars.histograms -> Fill("mu_global",     step, vars.lep_global);
    vars.histograms -> Fill("mu_normalizedChi2", step, vars.lep_normalizedChi2);
    vars.histograms -> Fill("mu_numberOfValidTrackerHits", step, vars.lep_numberOfValidTrackerHits);
    vars.histograms -> Fill("mu_numberOfValidMuonHits",    step, vars.lep_numberOfValidMuonHits);
  }
  
  
  
  vars.stdHistograms -> Fill1(vars.lep, "lep", step);
  if( vars.lep_charge > 0. )
    vars.stdHistograms -> Fill1(vars.lep, "lep_p", step);
  if( vars.lep_charge < 0. )
    vars.stdHistograms -> Fill1(vars.lep, "lep_m", step);
}



void FillMetHistograms(VBFAnalysisVariables& vars, const int& step)
{
  vars.histograms -> Fill("lepMet_mt", step, vars.lepMet_mt);
  
  vars.stdHistograms -> Fill1(vars.met, "met", step);
  vars.stdHistograms -> Fill2(vars.lep, vars.met, "lepMet", step);
  
  if( vars.lep_charge > 0. )
  {
    vars.histograms -> Fill("lepMet_mt_p", step, vars.lepMet_mt);
    vars.stdHistograms -> Fill1(vars.met, "met_p", step);
    vars.stdHistograms -> Fill2(vars.lep, vars.met, "lepMet_p", step);
  }
 
  if( vars.lep_charge < 0. )
  {
    vars.histograms -> Fill("lepMet_mt_m", step, vars.lepMet_mt);
    vars.stdHistograms -> Fill1(vars.met, "met_m", step);
    vars.stdHistograms -> Fill2(vars.lep, vars.met, "lepMet_m", step);
  }
}



void FillJetHistograms(VBFAnalysisVariables& vars, const int& step)
{
  vars.histograms -> Fill("jets_n", step, vars.nJets);
  vars.histograms -> Fill("jets_cnt_n", step, vars.nJets_cnt);
  vars.histograms -> Fill("jets_fwd_n", step, vars.nJets_fwd);
  vars.histograms -> Fill("jets_btw_n", step, vars.nJets_btw);
  
  vars.histograms -> Fill("jets_bTag1", step, vars.jets_bTag1);
  vars.histograms -> Fill("jets_bTag2", step, vars.jets_bTag2);
  
  
  
  if( vars.leadingJ.Et() <= 0. ) return;
  vars.stdHistograms -> Fill1(vars.leadingJ, "leadingJ", step);
  vars.histograms -> Fill("leadingJ_charge", step, vars.leadingJ_charge);  
  vars.histograms -> Fill("leadingJ_bTag", step, vars.leadingJ_bTag);  

  float tempDR = deltaR(vars.lep.eta(), vars.lep.phi(), vars.leadingJ.eta(), vars.leadingJ.phi());
  vars.histograms -> Fill("leadingJLep_DR", step, tempDR);
}



void FillWJJHistograms(VBFAnalysisVariables& vars, const int& step)
{
  if( (vars.WJ1.Et() <= 0.) || (vars.WJ2.Et() <= 0.) ) return;  
  
  vars.stdHistograms -> Fill2(vars.WJ1, vars.WJ2, "WJJ", step);
  
  float etSum = vars.WJ1.Et() + vars.WJ2.Et();
  vars.histograms -> Fill("WJJ_etRatio",    step, vars.WJ1.Et() / etSum );
  vars.histograms -> Fill("WJJ_etRatio",    step, vars.WJ2.Et() / etSum );
  vars.histograms -> Fill("WJJ_et1OverEt2", step, vars.WJ1.Et() / vars.WJ2.Et() );
  vars.histograms -> Fill("WJJ_chargeSum", step, -1. * vars.lep_charge * (vars.WJ1_charge+vars.WJ2_charge));
  
  vars.histograms -> Fill("lepWJJ_pt1", step, vars.lepWJJ_pt1);
  vars.histograms -> Fill("lepWJJ_pt2", step, vars.lepWJJ_pt2);
  vars.histograms -> Fill("lepWJJ_pt3", step, vars.lepWJJ_pt3);
}




void FillHiggsHistograms(VBFAnalysisVariables& vars, const int& step)
{
  if( (vars.WJ1.Et() <= 0.) || (vars.WJ2.Et() <= 0.) ) return;  
  
  vars.stdHistograms -> Fill2(vars.lep+vars.met, vars.WJ1+vars.WJ2, "lepMetW", step);
  vars.histograms -> Fill("lepMetW_mt", step, vars.lepMetW_mt);
  vars.histograms -> Fill("lepNuW_m", step, vars.lepNuW_m);
}



void FillTagJJHistograms(VBFAnalysisVariables& vars, const int& step)
{
  // 3 jet case
  if( (vars.tagJ1.Et() > 0.) && (vars.tagJ2.Et() <= 0.) )
  {
    vars.stdHistograms -> Fill2(vars.tagJ1, vars.tagJ2, "tagJJ", step);
    
    float etaMax = vars.tagJ1.eta();
    vars.histograms -> Fill("tagJLep_Deta", step, deltaEta(etaMax, vars.lep.eta()) );
    vars.histograms -> Fill("tagJWJJ_Deta", step, deltaEta(etaMax, (vars.WJ1+vars.WJ2).eta()) );
  }
  
  
  // 4 jet case
  if( (vars.tagJ1.Et() > 0.) && (vars.tagJ2.Et() > 0.) )
  {
    float etaMax = std::max(vars.tagJ1.eta(), vars.tagJ2.eta());
    vars.histograms -> Fill("tagJLep_Deta", step, deltaEta(etaMax, vars.lep.eta()) );
    vars.histograms -> Fill("tagJWJJ_Deta", step, deltaEta(etaMax, (vars.WJ1+vars.WJ2).eta()) );
  }
}



void FillMVAHistograms(VBFAnalysisVariables& vars, const int& step)
{
  vars.histograms -> Fill("mva", step, vars.mva);
}
*/
