#include "VBFPreselectionVariables.h"



TFile* GetOutputRootFile(VBFPreselectionVariables& vars)
{
  return vars.m_outputRootFile;
}



void InitializeVBFPreselectionTree(VBFPreselectionVariables& vars, const std::string& outputRootFileName, const int& doTnP)
{
  //-------------
  // Reduced tree
  //-------------
  
  vars.m_outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");  
  
  vars.m_reducedTree = new TTree("ntu", "ntu");
  vars.m_reducedTree -> SetDirectory(vars.m_outputRootFile);
  
  vars.m_reducedTree -> Branch("mH",           &vars.mH,                     "mH/F");
  vars.m_reducedTree -> Branch("dataFlag",     &vars.dataFlag,         "dataFlag/I");
  vars.m_reducedTree -> Branch("MCFlag",       &vars.MCFlag,             "MCFlag/I");
  vars.m_reducedTree -> Branch("totEvents",    &vars.totEvents,       "totEvents/I");
  vars.m_reducedTree -> Branch("crossSection", &vars.crossSection, "crossSection/F");
  vars.m_reducedTree -> Branch("TMVA4Jet",     &vars.TMVA4Jet,         "TMVA4Jet/I");
  vars.m_reducedTree -> Branch("runId",        &vars.runId,               "runId/I");
  vars.m_reducedTree -> Branch("lumiId",       &vars.lumiId,             "lumiId/I");
  vars.m_reducedTree -> Branch("eventId",      &vars.eventId,           "eventId/L");
  vars.m_reducedTree -> Branch("eventNaiveId", &vars.eventNaiveId, "eventNaiveId/I");
  
  vars.m_reducedTree -> Branch("eventWeight", &vars.eventWeight, "eventWeight/F");
  
  
  // MVA variables
  vars.m_reducedTree -> Branch("mva", &vars.mva, "mva/F");
  
  
  // Gen variables
  vars.m_reducedTree -> Branch("genPtHat", &vars.genPtHat,  "genPtHat/F");
  vars.m_reducedTree -> Branch("genWeight",&vars.genWeight,"genWeight/F");
  vars.m_reducedTree -> Branch("genNUP",   &vars.genNUP,      "genNUP/I");
  
  
  // PU variables
  vars.m_reducedTree -> Branch("PUtrue_n",        &vars.PUtrue_n,               "PUtrue_n/I");
  vars.m_reducedTree -> Branch("PUit_n",          &vars.PUit_n,                   "PUit_n/I");
  vars.m_reducedTree -> Branch("PUoot_early_n",   &vars.PUoot_early_n,     "PUoot_early_n/I");
  vars.m_reducedTree -> Branch("PUoot_late_n",    &vars.PUoot_late_n,       "PUoot_late_n/I");
  vars.m_reducedTree -> Branch("rhoForIsolation", &vars.rhoForIsolation, "rhoForIsolation/F");
  vars.m_reducedTree -> Branch("rhoForJets",      &vars.rhoForJets,           "rhoForJets/F");
  vars.m_reducedTree -> Branch("rhoForJetsPFlow", &vars.rhoForJetsPFlow, "rhoForJetsPFlow/F");
  
  
  // HLT variables
  vars.m_reducedTree -> Branch("HLT_Names",  "std::vector<std::string>", &vars.p_HLT_Names);
  vars.m_reducedTree -> Branch("HLT_Accept", "std::vector<float>",       &vars.p_HLT_Accept);
  
  
  // PDF variables
//  vars.m_reducedTree -> Branch("PDF_weights_CT10",            "std::vector<float>",            &vars.p_PDF_weights_CT10);
//  vars.m_reducedTree -> Branch("PDF_weights_MSTW2008nlo68cl", "std::vector<float>", &vars.p_PDF_weights_MSTW2008nlo68cl);
//  vars.m_reducedTree -> Branch("PDF_weights_NNPDF20",         "std::vector<float>",         &vars.p_PDF_weights_NNPDF20);
  
  
  // PV variables
  vars.m_reducedTree -> Branch("PV_n",              &vars.PV_n,                           "PV_n/I");
  vars.m_reducedTree -> Branch("PV_d0",             &vars.PV_d0,                         "PV_d0/F");
  vars.m_reducedTree -> Branch("PV_z",              &vars.PV_z,                           "PV_z/F");
  vars.m_reducedTree -> Branch("PV_nTracks",        &vars.PV_nTracks,               "PV_nTracks/I");
  vars.m_reducedTree -> Branch("PV_ndof",           &vars.PV_ndof,                     "PV_ndof/I");
  vars.m_reducedTree -> Branch("PV_normalizedChi2", &vars.PV_normalizedChi2, "PV_normalizedChi2/F");
  
  
  // lepton variables
  vars.m_reducedTree -> Branch("lep",    "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_lep);
  vars.m_reducedTree -> Branch("lep_charge",  &vars.lep_charge,   "lep_charge/F");
  vars.m_reducedTree -> Branch("lep_flavour", &vars.lep_flavour, "lep_flavour/I");
  vars.m_reducedTree -> Branch("lep_pt",      &vars.lep_pt,           "lep_pt/F");
  vars.m_reducedTree -> Branch("lep_eta",     &vars.lep_eta,         "lep_eta/F");
  vars.m_reducedTree -> Branch("lep_phi",     &vars.lep_phi,         "lep_phi/F");
  vars.m_reducedTree -> Branch("lep_zepp",    &vars.lep_zepp,       "lep_zepp/F");
  vars.m_reducedTree -> Branch("lep_z",       &vars.lep_z,             "lep_z/F");
  vars.m_reducedTree -> Branch("lep_dxy_BS",  &vars.lep_dxy_BS,   "lep_dxy_BS/F");
  vars.m_reducedTree -> Branch("lep_dz_BS",   &vars.lep_dz_BS,     "lep_dz_BS/F");
  vars.m_reducedTree -> Branch("lep_dB",      &vars.lep_dB,           "lep_dB/F");
  vars.m_reducedTree -> Branch("lep_edB",     &vars.lep_edB,         "lep_edB/F");
  vars.m_reducedTree -> Branch("lep_dz_PV",   &vars.lep_dz_PV,     "lep_dz_PV/F");
  vars.m_reducedTree -> Branch("lep_tkIso",   &vars.lep_tkIso,     "lep_tkIso/F");
  vars.m_reducedTree -> Branch("lep_emIso",   &vars.lep_emIso,     "lep_emIso/F");
  vars.m_reducedTree -> Branch("lep_hadIso",  &vars.lep_hadIso,   "lep_hadIso/F");
  vars.m_reducedTree -> Branch("lep_isEB",              &vars.lep_isEB,                           "lep_isEB/I");
  vars.m_reducedTree -> Branch("lep_etaSC",             &vars.lep_etaSC,                         "lep_etaSC/F");
  vars.m_reducedTree -> Branch("lep_sigmaIetaIeta",     &vars.lep_sigmaIetaIeta,         "lep_sigmaIetaIeta/F");
  vars.m_reducedTree -> Branch("lep_DphiIn",            &vars.lep_DphiIn,                       "lep_DphiIn/F");
  vars.m_reducedTree -> Branch("lep_DetaIn",            &vars.lep_DetaIn,                       "lep_DetaIn/F");
  vars.m_reducedTree -> Branch("lep_HOverE",            &vars.lep_HOverE,                       "lep_HOverE/F");
  vars.m_reducedTree -> Branch("lep_fbrem",             &vars.lep_fbrem,                         "lep_fbrem/F");
  vars.m_reducedTree -> Branch("lep_EOverP",            &vars.lep_EOverP,                       "lep_EOverP/F");
  vars.m_reducedTree -> Branch("lep_mishits",           &vars.lep_mishits,                     "lep_mishits/I");
  vars.m_reducedTree -> Branch("lep_dist",              &vars.lep_dist,                           "lep_dist/F");
  vars.m_reducedTree -> Branch("lep_dcot",              &vars.lep_dcot,                           "lep_dcot/F");
  vars.m_reducedTree -> Branch("lep_tracker",                    &vars.lep_tracker,                                       "lep_tracker/I");
  vars.m_reducedTree -> Branch("lep_standalone",                 &vars.lep_standalone,                                 "lep_standalone/I");
  vars.m_reducedTree -> Branch("lep_global",                     &vars.lep_global,                                         "lep_global/I");
  vars.m_reducedTree -> Branch("lep_normalizedChi2",             &vars.lep_normalizedChi2,                         "lep_normalizedChi2/F");
  vars.m_reducedTree -> Branch("lep_numberOfMatches",            &vars.lep_numberOfMatches,                       "lep_numberOfMatches/I");
  vars.m_reducedTree -> Branch("lep_numberOfValidTrackerHits",   &vars.lep_numberOfValidTrackerHits,     "lep_numberOfValidTrackerHits/I");
  vars.m_reducedTree -> Branch("lep_numberOfValidMuonHits",      &vars.lep_numberOfValidMuonHits,           "lep_numberOfValidMuonHits/I");
  vars.m_reducedTree -> Branch("lep_pixelLayersWithMeasurement", &vars.lep_pixelLayersWithMeasurement, "lep_pixelLayersWithMeasurement/I");
  vars.m_reducedTree -> Branch("lep_mcMatched", &vars.lep_mcMatched, "lep_mcMatched/I");
  vars.m_reducedTree -> Branch("lep_mcMatchDR", &vars.lep_mcMatchDR, "lep_mcMatchDR/F");
  vars.m_reducedTree -> Branch("lep_hltMatched", &vars.lep_hltMatched, "lep_hltMatched/I");
  vars.m_reducedTree -> Branch("lep_hltMatchPt", &vars.lep_hltMatchPt, "lep_hltMatchPt/F");
  vars.m_reducedTree -> Branch("lep_hltPass",    &vars.lep_hltPass,  "lep_hltMatchPass/I");
  
  if( doTnP == 1 )
  {
    vars.m_reducedTree -> Branch("lep2",    "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_lep2);
    vars.m_reducedTree -> Branch("lep2_charge",  &vars.lep2_charge,   "lep2_charge/F");
    vars.m_reducedTree -> Branch("lep2_flavour", &vars.lep2_flavour, "lep2_flavour/I");
    vars.m_reducedTree -> Branch("lep2_pt",      &vars.lep2_pt,           "lep2_pt/F");
    vars.m_reducedTree -> Branch("lep2_eta",     &vars.lep2_eta,         "lep2_eta/F");
    vars.m_reducedTree -> Branch("lep2_phi",     &vars.lep2_phi,         "lep2_phi/F");
    vars.m_reducedTree -> Branch("lep2_zepp",    &vars.lep2_zepp,       "lep2_zepp/F");
    vars.m_reducedTree -> Branch("lep2_z",       &vars.lep2_z,             "lep2_z/F");
    vars.m_reducedTree -> Branch("lep2_dxy_BS",  &vars.lep2_dxy_BS,   "lep2_dxy_BS/F");
    vars.m_reducedTree -> Branch("lep2_dz_BS",   &vars.lep2_dz_BS,     "lep2_dz_BS/F");
    vars.m_reducedTree -> Branch("lep2_dB",      &vars.lep2_dB,           "lep2_dB/F");
    vars.m_reducedTree -> Branch("lep2_edB",     &vars.lep2_edB,         "lep2_edB/F");
    vars.m_reducedTree -> Branch("lep2_dz_PV",   &vars.lep2_dz_PV,     "lep2_dz_PV/F");
    vars.m_reducedTree -> Branch("lep2_tkIso",   &vars.lep2_tkIso,     "lep2_tkIso/F");
    vars.m_reducedTree -> Branch("lep2_emIso",   &vars.lep2_emIso,     "lep2_emIso/F");
    vars.m_reducedTree -> Branch("lep2_hadIso",  &vars.lep2_hadIso,   "lep2_hadIso/F");
    vars.m_reducedTree -> Branch("lep2_isEB",              &vars.lep2_isEB,                           "lep2_isEB/I");
    vars.m_reducedTree -> Branch("lep2_etaSC",             &vars.lep2_etaSC,                         "lep2_etaSC/F");
    vars.m_reducedTree -> Branch("lep2_sigmaIetaIeta",     &vars.lep2_sigmaIetaIeta,         "lep2_sigmaIetaIeta/F");
    vars.m_reducedTree -> Branch("lep2_DphiIn",            &vars.lep2_DphiIn,                       "lep2_DphiIn/F");
    vars.m_reducedTree -> Branch("lep2_DetaIn",            &vars.lep2_DetaIn,                       "lep2_DetaIn/F");
    vars.m_reducedTree -> Branch("lep2_HOverE",            &vars.lep2_HOverE,                       "lep2_HOverE/F");
    vars.m_reducedTree -> Branch("lep2_fbrem",             &vars.lep2_fbrem,                         "lep2_fbrem/F");
    vars.m_reducedTree -> Branch("lep2_EOverP",            &vars.lep2_EOverP,                       "lep2_EOverP/F");
    vars.m_reducedTree -> Branch("lep2_mishits",           &vars.lep2_mishits,                     "lep2_mishits/I");
    vars.m_reducedTree -> Branch("lep2_dist",              &vars.lep2_dist,                           "lep2_dist/F");
    vars.m_reducedTree -> Branch("lep2_dcot",              &vars.lep2_dcot,                           "lep2_dcot/F");
    vars.m_reducedTree -> Branch("lep2_tracker",                    &vars.lep2_tracker,                                       "lep2_tracker/I");
    vars.m_reducedTree -> Branch("lep2_standalone",                 &vars.lep2_standalone,                                 "lep2_standalone/I");
    vars.m_reducedTree -> Branch("lep2_global",                     &vars.lep2_global,                                         "lep2_global/I");
    vars.m_reducedTree -> Branch("lep2_normalizedChi2",             &vars.lep2_normalizedChi2,                         "lep2_normalizedChi2/F");
    vars.m_reducedTree -> Branch("lep2_numberOfMatches",            &vars.lep2_numberOfMatches,                       "lep2_numberOfMatches/I");
    vars.m_reducedTree -> Branch("lep2_numberOfValidTrackerHits",   &vars.lep2_numberOfValidTrackerHits,     "lep2_numberOfValidTrackerHits/I");
    vars.m_reducedTree -> Branch("lep2_numberOfValidMuonHits",      &vars.lep2_numberOfValidMuonHits,           "lep2_numberOfValidMuonHits/I");
    vars.m_reducedTree -> Branch("lep2_pixelLayersWithMeasurement", &vars.lep2_pixelLayersWithMeasurement, "lep2_pixelLayersWithMeasurement/I");
    vars.m_reducedTree -> Branch("lep2_mcMatched", &vars.lep2_mcMatched, "lep2_mcMatched/I");
    vars.m_reducedTree -> Branch("lep2_mcMatchDR", &vars.lep2_mcMatchDR, "lep2_mcMatchDR/F");
    vars.m_reducedTree -> Branch("lep2_hltMatched", &vars.lep2_hltMatched, "lep2_hltMatched/I");
    vars.m_reducedTree -> Branch("lep2_hltMatchPt", &vars.lep2_hltMatchPt, "lep2_hltMatchPt/F");
    vars.m_reducedTree -> Branch("lep2_hltPass",    &vars.lep2_hltPass,       "lep2_hltPass/I");
  }
  
  
  // met variables
  vars.m_reducedTree -> Branch("met",   "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_met);
  vars.m_reducedTree -> Branch("nu",    "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_nu);
  vars.m_reducedTree -> Branch("nu1",   "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_nu1);
  vars.m_reducedTree -> Branch("nu2",   "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_nu2);
  vars.m_reducedTree -> Branch("met_et",       &vars.met_et,             "met_et/F");
  vars.m_reducedTree -> Branch("met_phi",      &vars.met_phi,           "met_phi/F");
  vars.m_reducedTree -> Branch("lepMet_pt",    &vars.lepMet_pt,       "lepMet_pt/F");
  vars.m_reducedTree -> Branch("lepMet_mt",    &vars.lepMet_mt,       "lepMet_mt/F");
  vars.m_reducedTree -> Branch("lepMet_Dphi",  &vars.lepMet_Dphi,   "lepMet_Dphi/F");
  vars.m_reducedTree -> Branch("met_mcMatched", &vars.met_mcMatched, "met_mcMatched/I");
  vars.m_reducedTree -> Branch("met_mcMatchDR", &vars.met_mcMatchDR, "met_mcMatchDR/F");  
  
  // btag variables
  vars.m_reducedTree -> Branch("nBTag_TCHEL_pt20",  &vars.nBTag_TCHEL_pt20,   "nBTag_TCHEL_pt20/I");
  vars.m_reducedTree -> Branch("nBTag_TCHEM_pt20",  &vars.nBTag_TCHEM_pt20,   "nBTag_TCHEM_pt20/I");
  vars.m_reducedTree -> Branch("nBTag_TCHPM_pt20",  &vars.nBTag_TCHPM_pt20,   "nBTag_TCHPM_pt20/I");
  vars.m_reducedTree -> Branch("nBTag_TCHPT_pt20",  &vars.nBTag_TCHPT_pt20,   "nBTag_TCHPT_pt20/I");
  vars.m_reducedTree -> Branch("nBTag_SSVHEM_pt20", &vars.nBTag_SSVHEM_pt20, "nBTag_SSVHEM_pt20/I");
  
  vars.m_reducedTree -> Branch("nBTag_TCHEL_pt25",  &vars.nBTag_TCHEL_pt25,   "nBTag_TCHEL_pt25/I");
  vars.m_reducedTree -> Branch("nBTag_TCHEM_pt25",  &vars.nBTag_TCHEM_pt25,   "nBTag_TCHEM_pt25/I");
  vars.m_reducedTree -> Branch("nBTag_TCHPM_pt25",  &vars.nBTag_TCHPM_pt25,   "nBTag_TCHPM_pt25/I");
  vars.m_reducedTree -> Branch("nBTag_TCHPT_pt25",  &vars.nBTag_TCHPT_pt25,   "nBTag_TCHPT_pt25/I");
  vars.m_reducedTree -> Branch("nBTag_SSVHEM_pt25", &vars.nBTag_SSVHEM_pt25, "nBTag_SSVHEM_pt25/I");
  
  vars.m_reducedTree -> Branch("nBTag_TCHEL_pt30",  &vars.nBTag_TCHEL_pt30,   "nBTag_TCHEL_pt30/I");
  vars.m_reducedTree -> Branch("nBTag_TCHEM_pt30",  &vars.nBTag_TCHEM_pt30,   "nBTag_TCHEM_pt30/I");
  vars.m_reducedTree -> Branch("nBTag_TCHPM_pt30",  &vars.nBTag_TCHPM_pt30,   "nBTag_TCHPM_pt30/I");
  vars.m_reducedTree -> Branch("nBTag_TCHPT_pt30",  &vars.nBTag_TCHPT_pt30,   "nBTag_TCHPT_pt30/I");
  vars.m_reducedTree -> Branch("nBTag_SSVHEM_pt30", &vars.nBTag_SSVHEM_pt30, "nBTag_SSVHEM_pt30/I");
  
  
  // jet variables
  vars.m_reducedTree -> Branch("nJets",     &vars.nJets,         "nJets/I");
  vars.m_reducedTree -> Branch("nJets_cnt", &vars.nJets_cnt, "nJets_cnt/I");
  vars.m_reducedTree -> Branch("nJets_fwd", &vars.nJets_fwd, "nJets_fwd/I");
  
  vars.m_reducedTree -> Branch("nJets_pt20",     &vars.nJets_pt20,         "nJets_pt20/I");
  vars.m_reducedTree -> Branch("nJets_cnt_pt20", &vars.nJets_cnt_pt20, "nJets_cnt_pt20/I");
  vars.m_reducedTree -> Branch("nJets_fwd_pt20", &vars.nJets_fwd_pt20, "nJets_fwd_pt20/I");
  vars.m_reducedTree -> Branch("nJets_btw_pt20", &vars.nJets_btw_pt20, "nJets_btw_pt20/I");
  vars.m_reducedTree -> Branch("nJets_pt25",     &vars.nJets_pt25,         "nJets_pt25/I");
  vars.m_reducedTree -> Branch("nJets_cnt_pt25", &vars.nJets_cnt_pt25, "nJets_cnt_pt25/I");
  vars.m_reducedTree -> Branch("nJets_fwd_pt25", &vars.nJets_fwd_pt25, "nJets_fwd_pt25/I");
  vars.m_reducedTree -> Branch("nJets_btw_pt25", &vars.nJets_btw_pt25, "nJets_btw_pt25/I");
  vars.m_reducedTree -> Branch("nJets_pt30",     &vars.nJets_pt30,         "nJets_pt30/I");
  vars.m_reducedTree -> Branch("nJets_cnt_pt30", &vars.nJets_cnt_pt30, "nJets_cnt_pt30/I");
  vars.m_reducedTree -> Branch("nJets_fwd_pt30", &vars.nJets_fwd_pt30, "nJets_fwd_pt30/I");
  vars.m_reducedTree -> Branch("nJets_btw_pt30", &vars.nJets_btw_pt30, "nJets_btw_pt30/I");
  
  vars.m_reducedTree -> Branch("jets_bTag1", &vars.jets_bTag1, "jets_bTag1/F"); 
  vars.m_reducedTree -> Branch("jets_bTag2", &vars.jets_bTag2, "jets_bTag2/F"); 
  
  
  // leading jet variables
  vars.m_reducedTree -> Branch("leadingJ", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_leadingJ);
  vars.m_reducedTree -> Branch("leadingJ_bTag",                &vars.leadingJ_bTag,                               "leadingJ_bTag/F");
  vars.m_reducedTree -> Branch("leadingJ_ptD",                 &vars.leadingJ_ptD,                                 "leadingJ_ptD/F");
  vars.m_reducedTree -> Branch("leadingJ_chargedMultiplicity", &vars.leadingJ_chargedMultiplicity, "leadingJ_chargedMultiplicity/I");
  vars.m_reducedTree -> Branch("leadingJ_neutralMultiplicity", &vars.leadingJ_neutralMultiplicity, "leadingJ_neutralMultiplicity/I");
  vars.m_reducedTree -> Branch("jet1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_jet1);
  vars.m_reducedTree -> Branch("jet2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_jet2);
  vars.m_reducedTree -> Branch("jet3", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_jet3);
  vars.m_reducedTree -> Branch("jet4", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_jet4);
  vars.m_reducedTree -> Branch("jet5", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_jet5);
  vars.m_reducedTree -> Branch("jet6", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_jet6);
  vars.m_reducedTree -> Branch("jet1_bTag", &vars.jet1_bTag, "jet1_bTag/F");
  vars.m_reducedTree -> Branch("jet2_bTag", &vars.jet2_bTag, "jet2_bTag/F");
  vars.m_reducedTree -> Branch("jet3_bTag", &vars.jet3_bTag, "jet3_bTag/F");
  vars.m_reducedTree -> Branch("jet4_bTag", &vars.jet4_bTag, "jet4_bTag/F");
  vars.m_reducedTree -> Branch("jet5_bTag", &vars.jet5_bTag, "jet5_bTag/F");
  vars.m_reducedTree -> Branch("jet6_bTag", &vars.jet6_bTag, "jet6_bTag/F");
  vars.m_reducedTree -> Branch("jet1_hltMatched", &vars.jet1_hltMatched, "jet1_hltMatched/I");
  vars.m_reducedTree -> Branch("jet2_hltMatched", &vars.jet2_hltMatched, "jet2_hltMatched/I");
  vars.m_reducedTree -> Branch("jet3_hltMatched", &vars.jet3_hltMatched, "jet3_hltMatched/I");
  vars.m_reducedTree -> Branch("jet4_hltMatched", &vars.jet4_hltMatched, "jet4_hltMatched/I");
  vars.m_reducedTree -> Branch("jet5_hltMatched", &vars.jet5_hltMatched, "jet5_hltMatched/I");
  vars.m_reducedTree -> Branch("jet6_hltMatched", &vars.jet6_hltMatched, "jet6_hltMatched/I");
  vars.m_reducedTree -> Branch("jet1_hltMatchPt", &vars.jet1_hltMatchPt, "jet1_hltMatchPt/F");
  vars.m_reducedTree -> Branch("jet2_hltMatchPt", &vars.jet2_hltMatchPt, "jet2_hltMatchPt/F");
  vars.m_reducedTree -> Branch("jet3_hltMatchPt", &vars.jet3_hltMatchPt, "jet3_hltMatchPt/F");
  vars.m_reducedTree -> Branch("jet4_hltMatchPt", &vars.jet4_hltMatchPt, "jet4_hltMatchPt/F");
  vars.m_reducedTree -> Branch("jet5_hltMatchPt", &vars.jet5_hltMatchPt, "jet5_hltMatchPt/F");
  vars.m_reducedTree -> Branch("jet6_hltMatchPt", &vars.jet6_hltMatchPt, "jet6_hltMatchPt/F");
    
  // W-jet variables
  vars.m_reducedTree -> Branch("WJ1",    "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ1);
  vars.m_reducedTree -> Branch("WJ2",    "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ2);
  vars.m_reducedTree -> Branch("WJJ", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJJ);
  vars.m_reducedTree -> Branch("WJ1_pt",                  &vars.WJ1_pt,                                   "WJ1_pt/F");
  vars.m_reducedTree -> Branch("WJ2_pt",                  &vars.WJ2_pt,                                   "WJ2_pt/F");
  vars.m_reducedTree -> Branch("WJ1_eta",                 &vars.WJ1_eta,                                 "WJ1_eta/F");
  vars.m_reducedTree -> Branch("WJ2_eta",                 &vars.WJ2_eta,                                 "WJ2_eta/F");
  vars.m_reducedTree -> Branch("WJ1_phi",                 &vars.WJ1_phi,                                 "WJ1_phi/F");
  vars.m_reducedTree -> Branch("WJ2_phi",                 &vars.WJ2_phi,                                 "WJ2_phi/F");
  vars.m_reducedTree -> Branch("WJ1_zepp",                &vars.WJ1_zepp,                               "WJ1_zepp/F");
  vars.m_reducedTree -> Branch("WJ2_zepp",                &vars.WJ2_zepp,                               "WJ2_zepp/F");
  vars.m_reducedTree -> Branch("WJ1_lep_Dphi",            &vars.WJ1_lep_Dphi,                       "WJ1_lep_Dphi/F");
  vars.m_reducedTree -> Branch("WJ2_lep_Dphi",            &vars.WJ2_lep_Dphi,                       "WJ2_lep_Dphi/F");
  vars.m_reducedTree -> Branch("WJ1_met_Dphi",            &vars.WJ1_met_Dphi,                       "WJ1_met_Dphi/F");
  vars.m_reducedTree -> Branch("WJ2_met_Dphi",            &vars.WJ2_met_Dphi,                       "WJ2_met_Dphi/F");
  vars.m_reducedTree -> Branch("WJ1_bTag",                &vars.WJ1_bTag,                               "WJ1_bTag/F");
  vars.m_reducedTree -> Branch("WJ2_bTag",                &vars.WJ2_bTag,                               "WJ2_bTag/F");
  vars.m_reducedTree -> Branch("WJ1_ptD",                 &vars.WJ1_ptD,                                 "WJ1_ptD/F");
  vars.m_reducedTree -> Branch("WJ2_ptD",                 &vars.WJ2_ptD,                                 "WJ2_ptD/F");
  vars.m_reducedTree -> Branch("WJ1_chargedMultiplicity", &vars.WJ1_chargedMultiplicity, "WJ1_chargedMultiplicity/I");
  vars.m_reducedTree -> Branch("WJ2_chargedMultiplicity", &vars.WJ2_chargedMultiplicity, "WJ2_chargedMultiplicity/I");
  vars.m_reducedTree -> Branch("WJ1_neutralMultiplicity", &vars.WJ1_neutralMultiplicity, "WJ1_neutralMultiplicity/I");
  vars.m_reducedTree -> Branch("WJ2_neutralMultiplicity", &vars.WJ2_neutralMultiplicity, "WJ2_neutralMultiplicity/I");
  vars.m_reducedTree -> Branch("WJ1_mcMatched", &vars.WJ1_mcMatched, "WJ1_mcMatched/I");
  vars.m_reducedTree -> Branch("WJ2_mcMatched", &vars.WJ2_mcMatched, "WJ2_mcMatched/I");
  vars.m_reducedTree -> Branch("WJ1_mcMatchDR", &vars.WJ1_mcMatchDR, "WJ1_mcMatchDR/F");
  vars.m_reducedTree -> Branch("WJ2_mcMatchDR", &vars.WJ2_mcMatchDR, "WJ2_mcMatchDR/F");
  
  vars.m_reducedTree -> Branch("WJJ_Deta",   &vars.WJJ_Deta,     "WJJ_Deta/F");
  vars.m_reducedTree -> Branch("WJJ_Dphi",   &vars.WJJ_Dphi,     "WJJ_Dphi/F");
  vars.m_reducedTree -> Branch("WJJ_DR",     &vars.WJJ_DR,         "WJJ_DR/F");
  vars.m_reducedTree -> Branch("WJJ_pt",     &vars.WJJ_pt,         "WJJ_pt/F");
  vars.m_reducedTree -> Branch("WJJ_eta",    &vars.WJJ_eta,       "WJJ_eta/F");
  vars.m_reducedTree -> Branch("WJJ_phi",    &vars.WJJ_phi,       "WJJ_phi/F");
  vars.m_reducedTree -> Branch("WJJ_m",      &vars.WJJ_m,           "WJJ_m/F");
  vars.m_reducedTree -> Branch("WJJ_zepp",   &vars.WJJ_zepp,     "WJJ_zepp/F");

  vars.m_reducedTree -> Branch("lepWJJ_pt1", &vars.lepWJJ_pt1, "lepWJJ_pt1/F");
  vars.m_reducedTree -> Branch("lepWJJ_pt2", &vars.lepWJJ_pt2, "lepWJJ_pt2/F");
  vars.m_reducedTree -> Branch("lepWJJ_pt3", &vars.lepWJJ_pt3, "lepWJJ_pt3/F");
  
  
  // Higgs variables
  vars.m_reducedTree -> Branch("lepMetW_pt",       &vars.lepMetW_pt,             "lepMetW_pt/F");
  vars.m_reducedTree -> Branch("lepMetW_phi",      &vars.lepMetW_phi,           "lepMetW_phi/F");
  vars.m_reducedTree -> Branch("lepMetW_mt",       &vars.lepMetW_mt,             "lepMetW_mt/F");
  vars.m_reducedTree -> Branch("lepMetW_Dphi",     &vars.lepMetW_Dphi,         "lepMetW_Dphi/F");
  vars.m_reducedTree -> Branch("lepNu_m",          &vars.lepNu_m,                   "lepNu_m/F");
  vars.m_reducedTree -> Branch("lepNu_nSolutions", &vars.lepNu_nSolutions, "lepNu_nSolutions/I");
  vars.m_reducedTree -> Branch("lepW_pt",          &vars.lepW_pt,                   "lepW_pt/F");
  vars.m_reducedTree -> Branch("lepNuW_eta",       &vars.lepNuW_eta,             "lepNuW_eta/F");
  vars.m_reducedTree -> Branch("lepNuW_m",         &vars.lepNuW_m,                 "lepNuW_m/F");
  vars.m_reducedTree -> Branch("lepNuW_m1",        &vars.lepNuW_m1,               "lepNuW_m1/F");
  vars.m_reducedTree -> Branch("lepNuW_m2",        &vars.lepNuW_m2,               "lepNuW_m2/F");
  vars.m_reducedTree -> Branch("lepNuW_zepp",      &vars.lepNuW_zepp,           "lepNuW_zepp/F");
  
  
  // tag-jet variables
  vars.m_reducedTree -> Branch("tagJ1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_tagJ1);
  vars.m_reducedTree -> Branch("tagJ2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_tagJ2);
  vars.m_reducedTree -> Branch("tagJ1_bTag",                &vars.tagJ1_bTag,                               "tagJ1_bTag/F");
  vars.m_reducedTree -> Branch("tagJ2_bTag",                &vars.tagJ2_bTag,                               "tagJ2_bTag/F");
  vars.m_reducedTree -> Branch("tagJ1_ptD",                 &vars.tagJ1_ptD,                                 "tagJ1_ptD/F");
  vars.m_reducedTree -> Branch("tagJ2_ptD",                 &vars.tagJ2_ptD,                                 "tagJ2_ptD/F");
  vars.m_reducedTree -> Branch("tagJ1_chargedMultiplicity", &vars.tagJ1_chargedMultiplicity, "tagJ1_chargedMultiplicity/I");
  vars.m_reducedTree -> Branch("tagJ2_chargedMultiplicity", &vars.tagJ2_chargedMultiplicity, "tagJ2_chargedMultiplicity/I");
  vars.m_reducedTree -> Branch("tagJ1_neutralMultiplicity", &vars.tagJ1_neutralMultiplicity, "tagJ1_neutralMultiplicity/I");
  vars.m_reducedTree -> Branch("tagJ2_neutralMultiplicity", &vars.tagJ2_neutralMultiplicity, "tagJ2_neutralMultiplicity/I");
  vars.m_reducedTree -> Branch("tagJJ_Deta",   &vars.tagJJ_Deta,     "tagJJ_Deta/F");
  vars.m_reducedTree -> Branch("tagJJ_Dphi",   &vars.tagJJ_Dphi,     "tagJJ_Dphi/F");
  vars.m_reducedTree -> Branch("tagJJ_DR",     &vars.tagJJ_DR,         "tagJJ_DR/F");
  vars.m_reducedTree -> Branch("tagJJ_m",      &vars.tagJJ_m,           "tagJJ_m/F");
  
  
  // third jet variables
  vars.m_reducedTree -> Branch("thirdJ", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_thirdJ);
  vars.m_reducedTree -> Branch("thirdJ_bTag",      &vars.thirdJ_bTag,           "thirdJ_bTag/F");
  vars.m_reducedTree -> Branch("thirdJ_WJJDeta",   &vars.thirdJ_WJJDeta,     "thirdJ_WJJDeta/F");
  vars.m_reducedTree -> Branch("thirdJ_WJJDphi",   &vars.thirdJ_WJJDphi,     "thirdJ_WJJDphi/F");
  vars.m_reducedTree -> Branch("thirdJ_WJJDR",     &vars.thirdJ_WJJDR,         "thirdJ_WJJDR/F");
  vars.m_reducedTree -> Branch("thirdJ_tagJJDeta", &vars.thirdJ_tagJJDeta, "thirdJ_tagJJDeta/F");
  vars.m_reducedTree -> Branch("thirdJ_tagJJDphi", &vars.thirdJ_tagJJDphi, "thirdJ_tagJJDphi/F");
  vars.m_reducedTree -> Branch("thirdJ_tagJJDR",   &vars.thirdJ_tagJJDR,     "thirdJ_tagJJDR/F");
  
  
  // MC variables
  vars.m_reducedTree -> Branch("mc_lep", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_mc_lep);
  vars.m_reducedTree -> Branch("mc_lep_charge",  &vars.mc_lep_charge,   "mc_lep_charge/F");
  vars.m_reducedTree -> Branch("mc_lep_flavour", &vars.mc_lep_flavour, "mc_lep_flavour/I");
  vars.m_reducedTree -> Branch("mc_lep_pt",      &vars.mc_lep_pt,           "mc_lep_pt/F");
  vars.m_reducedTree -> Branch("mc_lep_eta",     &vars.mc_lep_eta,         "mc_lep_eta/F");
  vars.m_reducedTree -> Branch("mc_lep_phi",     &vars.mc_lep_phi,         "mc_lep_phi/F");
  
  vars.m_reducedTree -> Branch("mc_nu", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_mc_nu);
  vars.m_reducedTree -> Branch("mc_nu_charge",  &vars.mc_nu_charge,   "mc_nu_charge/F");
  vars.m_reducedTree -> Branch("mc_nu_flavour", &vars.mc_nu_flavour, "mc_nu_flavour/I");
  vars.m_reducedTree -> Branch("mc_nu_pt",      &vars.mc_nu_pt,           "mc_nu_pt/F");
  vars.m_reducedTree -> Branch("mc_nu_eta",     &vars.mc_nu_eta,         "mc_nu_eta/F");
  vars.m_reducedTree -> Branch("mc_nu_phi",     &vars.mc_nu_phi,         "mc_nu_phi/F");
  
  vars.m_reducedTree -> Branch("mc_lepNu_pt",   &vars.mc_lepNu_pt,     "mc_lepNu_pt/F");
  vars.m_reducedTree -> Branch("mc_lepNu_mt",   &vars.mc_lepNu_mt,     "mc_lepNu_mt/F");
  vars.m_reducedTree -> Branch("mc_lepNu_Dphi", &vars.mc_lepNu_Dphi, "mc_lepNu_Dphi/F");
  
  vars.m_reducedTree -> Branch("mc_WJ1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_mc_WJ1);  
  vars.m_reducedTree -> Branch("mc_WJ2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_mc_WJ2);  
  vars.m_reducedTree -> Branch("mc_WJ1_pt",  &vars.mc_WJ1_pt,   "mc_WJ1_pt/F");
  vars.m_reducedTree -> Branch("mc_WJ2_pt",  &vars.mc_WJ2_pt,   "mc_WJ2_pt/F");
  vars.m_reducedTree -> Branch("mc_WJ1_eta", &vars.mc_WJ1_eta, "mc_WJ1_eta/F");
  vars.m_reducedTree -> Branch("mc_WJ2_eta", &vars.mc_WJ2_eta, "mc_WJ2_eta/F");
  vars.m_reducedTree -> Branch("mc_WJ1_phi", &vars.mc_WJ1_phi, "mc_WJ1_phi/F");
  vars.m_reducedTree -> Branch("mc_WJ2_phi", &vars.mc_WJ2_phi, "mc_WJ2_phi/F");
  
  vars.m_reducedTree -> Branch("mc_WJJ_Deta", &vars.mc_WJJ_Deta, "mc_WJJ_Deta/F");
  vars.m_reducedTree -> Branch("mc_WJJ_Dphi", &vars.mc_WJJ_Dphi, "mc_WJJ_Dphi/F");
  vars.m_reducedTree -> Branch("mc_WJJ_DR",   &vars.mc_WJJ_DR,     "mc_WJJ_DR/F");
  vars.m_reducedTree -> Branch("mc_WJJ_pt",   &vars.mc_WJJ_pt,     "mc_WJJ_pt/F");
  vars.m_reducedTree -> Branch("mc_WJJ_m",    &vars.mc_WJJ_m,       "mc_WJJ_m/F");
  
  vars.m_reducedTree -> Branch("mc_W1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_mc_W1);  
  vars.m_reducedTree -> Branch("mc_W2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_mc_W2);  
  vars.m_reducedTree -> Branch("mc_W1_pt",  &vars.mc_W1_pt,   "mc_W1_pt/F");
  vars.m_reducedTree -> Branch("mc_W2_pt",  &vars.mc_W2_pt,   "mc_W2_pt/F");
  vars.m_reducedTree -> Branch("mc_W1_eta", &vars.mc_W1_eta, "mc_W1_eta/F");
  vars.m_reducedTree -> Branch("mc_W2_eta", &vars.mc_W2_eta, "mc_W2_eta/F");
  vars.m_reducedTree -> Branch("mc_W1_phi", &vars.mc_W1_phi, "mc_W1_phi/F");
  vars.m_reducedTree -> Branch("mc_W2_phi", &vars.mc_W2_phi, "mc_W2_phi/F");
  vars.m_reducedTree -> Branch("mc_W1_m",   &vars.mc_W1_m,     "mc_W1_m/F");
  vars.m_reducedTree -> Branch("mc_W2_m",   &vars.mc_W2_m,     "mc_WJ_m/F");
  
  vars.m_reducedTree -> Branch("mc_H", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_mc_H);  
  vars.m_reducedTree -> Branch("mc_H_pt",  &vars.mc_H_pt,   "mc_H_pt/F");
  vars.m_reducedTree -> Branch("mc_H_eta", &vars.mc_H_eta, "mc_H_eta/F");
  vars.m_reducedTree -> Branch("mc_H_phi", &vars.mc_H_phi, "mc_H_phi/F");
  vars.m_reducedTree -> Branch("mc_H_m",   &vars.mc_H_m,     "mc_H_m/F");
  
}



void FillVBFPreselectionTree(VBFPreselectionVariables& vars)
{
  vars.m_reducedTree -> Fill();
}






void ClearVBFPreselectionVariables(VBFPreselectionVariables& vars)
{
  // mva variables
  vars.mva = -99.; 
  
  
  // GEN variables
  vars.genPtHat = -1.;
  vars.genWeight = -1.;
  vars.genNUP = -1;
  
  
  // PU variables
  vars.PUtrue_n = -1;
  vars.PUit_n = -1;
  vars.PUoot_early_n = -1;
  vars.PUoot_late_n = -1;
  vars.rhoForIsolation = -99.;
  vars.rhoForJets = -99.;
  vars.rhoForJetsPFlow = -99.;
  
  
  // PV variables
  vars.PV_n = -1;
  vars.PV_d0 = -1.;
  vars.PV_z = -99.;  
  vars.PV_nTracks = -1;
  vars.PV_ndof = -1;
  vars.PV_normalizedChi2 = -1.;
  
  
  
  // lepton variables
  vars.leptons.clear();
  vars.electrons.clear();
  vars.electrons_loose.clear();
  vars.muons.clear();
  vars.muons_loose.clear();
  vars.leptonCharges.clear();
  vars.leptonFlavours.clear();    
  vars.leptons_z.clear();
  vars.leptons_dxy_BS.clear();
  vars.leptons_dz_BS.clear();
  vars.leptons_dB.clear();
  vars.leptons_edB.clear();
  vars.leptons_dz_PV.clear();
  vars.leptons_tkIso.clear();
  vars.leptons_emIso.clear();
  vars.leptons_hadIso.clear();
  vars.electrons_isEB.clear();
  vars.electrons_etaSC.clear();
  vars.electrons_sigmaIetaIeta.clear();
  vars.electrons_DphiIn.clear();
  vars.electrons_DetaIn.clear();
  vars.electrons_HOverE.clear();
  vars.electrons_fbrem.clear();
  vars.electrons_EOverP.clear();
  vars.electrons_mishits.clear();
  vars.electrons_dist.clear();
  vars.electrons_dcot.clear();
  vars.muons_tracker.clear();
  vars.muons_standalone.clear();
  vars.muons_global.clear();
  vars.muons_normalizedChi2.clear();
  vars.muons_numberOfMatches.clear();
  vars.muons_numberOfValidTrackerHits.clear();
  vars.muons_numberOfValidMuonHits.clear();
  vars.muons_pixelLayersWithMeasurement.clear();
  
  vars.selectIt_lep = -1;
  vars.selectIt_ele = -1;
  vars.selectIt_mu = -1;

  vars.selectIt_lep2 = -1;
  vars.selectIt_ele2 = -1;
  vars.selectIt_mu2 = -1;

  vars.lep = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_lep = NULL;
  
  vars.lep_charge = -1.;
  vars.lep_flavour = -1;
  vars.lep_pt = -1.;
  vars.lep_eta = -99.;
  vars.lep_phi = -99.;
  vars.lep_zepp = -99.;
  vars.lep_z = -99.;
  vars.lep_dxy_BS = -99.;
  vars.lep_dz_BS = -99.;
  vars.lep_dB = -99.;
  vars.lep_edB = -99.;
  vars.lep_dz_PV = -99.;
  vars.lep_tkIso = -1.;
  vars.lep_emIso = -1.;
  vars.lep_hadIso = -1.;
  vars.lep_isEB = -1;
  vars.lep_etaSC = -1.;
  vars.lep_sigmaIetaIeta = -1.;
  vars.lep_DphiIn = -99.;
  vars.lep_DetaIn = -99.;
  vars.lep_HOverE = -1.;
  vars.lep_fbrem = -1.;
  vars.lep_mishits = -1;
  vars.lep_dist = -99.;
  vars.lep_dcot = -99.;
  vars.lep_EOverP = -1.;
  vars.lep_tracker = -1;
  vars.lep_standalone = -1;
  vars.lep_global = -1;
  vars.lep_normalizedChi2 = -1.;
  vars.lep_numberOfMatches = -1;
  vars.lep_numberOfValidTrackerHits = -1;
  vars.lep_numberOfValidMuonHits = -1;
  vars.lep_pixelLayersWithMeasurement = -1;
  
  vars.lep_mcMatched = -1;
  vars.lep_mcMatchDR = -99.;
  vars.lep_hltMatched = -1;
  vars.lep_hltMatchPt = -99.;
  vars.lep_hltPass = -99;
  
  
  vars.lep2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_lep2 = NULL;
  
  vars.lep2_charge = -1.;
  vars.lep2_flavour = -1;
  vars.lep2_pt = -1.;
  vars.lep2_eta = -99.;
  vars.lep2_phi = -99.;
  vars.lep2_zepp = -99.;
  vars.lep2_z = -99.;
  vars.lep2_dxy_BS = -99.;
  vars.lep2_dz_BS = -99.;
  vars.lep2_dB = -99.;
  vars.lep2_edB = -99.;
  vars.lep2_dz_PV = -99.;
  vars.lep2_tkIso = -1.;
  vars.lep2_emIso = -1.;
  vars.lep2_hadIso = -1.;
  vars.lep2_isEB = -1;
  vars.lep2_etaSC = -1.;
  vars.lep2_sigmaIetaIeta = -1.;
  vars.lep2_DphiIn = -99.;
  vars.lep2_DetaIn = -99.;
  vars.lep2_HOverE = -1.;
  vars.lep2_fbrem = -1.;
  vars.lep2_mishits = -1;
  vars.lep2_dist = -99.;
  vars.lep2_dcot = -99.;
  vars.lep2_EOverP = -1.;
  vars.lep2_tracker = -1;
  vars.lep2_standalone = -1;
  vars.lep2_global = -1;
  vars.lep2_normalizedChi2 = -1.;
  vars.lep2_numberOfMatches = -1;
  vars.lep2_numberOfValidTrackerHits = -1;
  vars.lep2_numberOfValidMuonHits = -1;
  vars.lep2_pixelLayersWithMeasurement = -1;
  
  vars.lep2_mcMatched = -1;
  vars.lep2_mcMatchDR = -99.;
  vars.lep2_hltMatched = -1;
  vars.lep2_hltMatchPt = -99.;
  vars.lep2_hltPass = -99;
  
  
  // met variables 
  vars.met = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_met = NULL;
  vars.nu = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_nu = NULL;
  vars.nu1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_nu1 = NULL;
  vars.nu2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_nu2 = NULL;
  vars.met_et = -1.;
  vars.met_phi = -1.;
  
  vars.met_mcMatched = -1;
  vars.met_mcMatchDR = -99.;
  
  vars.lepMet = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  
  vars.lepMet_pt = -1.;
  vars.lepMet_mt = -1.;
  vars.lepMet_Dphi = -1.;
  
  
  
  // b-tag variables
  vars.nBTag_TCHEL_pt20 = 0;
  vars.nBTag_TCHEM_pt20 = 0;
  vars.nBTag_TCHPM_pt20 = 0;
  vars.nBTag_TCHPT_pt20 = 0;
  vars.nBTag_SSVHEM_pt20 = 0;
  vars.nBTag_SSVHPT_pt20 = 0;
  
  vars.nBTag_TCHEL_pt25 = 0;
  vars.nBTag_TCHEM_pt25 = 0;
  vars.nBTag_TCHPM_pt25 = 0;
  vars.nBTag_TCHPT_pt25 = 0;
  vars.nBTag_SSVHEM_pt25 = 0;
  vars.nBTag_SSVHPT_pt25 = 0;
  
  vars.nBTag_TCHEL_pt30 = 0;
  vars.nBTag_TCHEM_pt30 = 0;
  vars.nBTag_TCHPM_pt30 = 0;
  vars.nBTag_TCHPT_pt30 = 0;
  vars.nBTag_SSVHEM_pt30 = 0;
  vars.nBTag_SSVHPT_pt30 = 0;
  
  
  
  // jet variables
  vars.jets.clear();
  vars.jets_etaEtaMoment.clear();
  vars.jets_phiPhiMoment.clear();
  vars.jets_etaPhiMoment.clear();
  vars.jets_emEnergyFraction.clear();
  vars.jets_fHPD.clear();
  vars.jets_fRBX.clear();
  vars.jets_n90Hits.clear();
  vars.jets_ptD.clear();
  vars.jets_chargedHadronEnergyFraction.clear();
  vars.jets_neutralHadronEnergyFraction.clear();
  vars.jets_chargedEmEnergyFraction.clear();
  vars.jets_neutralEmEnergyFraction.clear();
  vars.jets_muonEnergyFraction.clear();
  vars.jets_chargedMultiplicity.clear();
  vars.jets_neutralMultiplicity.clear();
  vars.jets_muonMultiplicity.clear();  
  vars.jets_ptOrdered.clear();
  vars.jets_bTag.clear();
  vars.jets_bTagOrdered.clear();
  
  vars.nJets = 0;
  vars.nJets_cnt = 0;
  vars.nJets_fwd = 0;
  
  vars.nJets_pt20 = 0;
  vars.nJets_cnt_pt20 = 0;
  vars.nJets_fwd_pt20 = 0;
  vars.nJets_btw_pt20 = 0;
  vars.nJets_pt25 = 0;
  vars.nJets_cnt_pt25 = 0;
  vars.nJets_fwd_pt25 = 0;
  vars.nJets_btw_pt25 = 0;
  vars.nJets_pt30 = 0;
  vars.nJets_cnt_pt30 = 0;
  vars.nJets_fwd_pt30 = 0;
  vars.nJets_btw_pt30 = 0;
  
  vars.jets_bTag1 = -99.;
  vars.jets_bTag2 = -99.;
  
  vars.selectIt_leadingJet = -1;
  
  
  vars.leadingJ = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_leadingJ = NULL;
  
  vars.leadingJ_bTag = -99.;
  vars.leadingJ_ptD = -99.;
  vars.leadingJ_chargedMultiplicity = -1;
  vars.leadingJ_neutralMultiplicity = -1;
  
  
  vars.jet1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_jet1 = NULL;
  vars.jet2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_jet2 = NULL;
  vars.jet3 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_jet3 = NULL;
  vars.jet4 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_jet4 = NULL;
  vars.jet5 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_jet5 = NULL;
  vars.jet6 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_jet6 = NULL;
  
  vars.jet1_bTag = -99.;
  vars.jet2_bTag = -99.;
  vars.jet3_bTag = -99.;
  vars.jet4_bTag = -99.;
  vars.jet5_bTag = -99.;
  vars.jet6_bTag = -99.;
  
  vars.jet1_hltMatched = 0;
  vars.jet2_hltMatched = 0;
  vars.jet3_hltMatched = 0;
  vars.jet4_hltMatched = 0;
  vars.jet5_hltMatched = 0;
  vars.jet6_hltMatched = 0;
  vars.jet1_hltMatchPt = -99;
  vars.jet2_hltMatchPt = -99;
  vars.jet3_hltMatchPt = -99;
  vars.jet4_hltMatchPt = -99;
  vars.jet5_hltMatchPt = -99;
  vars.jet6_hltMatchPt = -99;
  
  
  // W-jet variables
  vars.selectIt_W.clear();
  
  vars.WJ1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_WJ1 = NULL;
  vars.WJ2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_WJ2 = NULL;
  
  vars.WJJ = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_WJJ = NULL;
  
  vars.WJ1_pt = -99.;
  vars.WJ2_pt = -99.;
  vars.WJ1_eta = -99.;
  vars.WJ2_eta = -99.;
  vars.WJ1_phi = -99.;
  vars.WJ2_phi = -99.;
  vars.WJ1_zepp = -99.;
  vars.WJ2_zepp = -99.;
  vars.WJ1_lep_Dphi = -99.;
  vars.WJ2_lep_Dphi = -99.;
  vars.WJ1_met_Dphi = -99.;
  vars.WJ2_met_Dphi = -99.;
  vars.WJ1_bTag = -99.;
  vars.WJ2_bTag = -99.;
  vars.WJ1_ptD = -99.;
  vars.WJ2_ptD = -99.;
  vars.WJ1_chargedMultiplicity = -1;
  vars.WJ2_chargedMultiplicity = -1;
  vars.WJ1_neutralMultiplicity = -1;
  vars.WJ2_neutralMultiplicity = -1;
  
  vars.WJ1_mcMatched = -1;
  vars.WJ2_mcMatched = -1;
  vars.WJ1_mcMatchDR = -99.;
  vars.WJ2_mcMatchDR = -99.;
  
  vars.WJJ_Deta = -99.;
  vars.WJJ_Dphi = -99.;
  vars.WJJ_DR = -99.;
  vars.WJJ_pt = -99.;
  vars.WJJ_eta = -99.;
  vars.WJJ_phi = -99.;
  vars.WJJ_m = -99.;
  vars.WJJ_zepp = -99.;
  
  vars.lepWJJ_ptOrdered.clear();  
  vars.lepWJJ_pt1 = -1.;
  vars.lepWJJ_pt2 = -1.;
  vars.lepWJJ_pt3 = -1.;
  
  
  
  // Higgs variables
  vars.lepW = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.lepNuW = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.lepNuW1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.lepNuW2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  
  vars.lepMetW_pt = -1.;
  vars.lepMetW_phi = -1.;
  vars.lepMetW_mt = -1.;
  vars.lepMetW_Dphi = -1.;
  vars.lepNu_m = -1.;
  vars.lepNu_nSolutions = -1;
  vars.lepW_pt = -1.;
  vars.lepNuW_eta = -1.;
  vars.lepNuW_m = -1.;
  vars.lepNuW_m1 = -1.;
  vars.lepNuW_m2 = -1.;
  vars.lepNuW_zepp = -99.;
  
  
  
  // tag-jet variables
  vars.selectIt_tag.clear();
  
  vars.tagJ1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_tagJ1 = NULL;
  vars.tagJ2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_tagJ2 = NULL;
  
  vars.tagJJ = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  
  vars.tagJ1_bTag = -99.;
  vars.tagJ2_bTag = -99.;
  vars.tagJ1_ptD = -99.;
  vars.tagJ2_ptD = -99.;
  vars.tagJ1_chargedMultiplicity = -1;
  vars.tagJ2_chargedMultiplicity = -1;
  vars.tagJ1_neutralMultiplicity = -1;
  vars.tagJ2_neutralMultiplicity = -1;
  
  vars.tagJJ_Deta = -99.;
  vars.tagJJ_Dphi = -99.;
  vars.tagJJ_DR = -99.;
  vars.tagJJ_m = -99.;
  
  
  // third jet variables
  vars.selectIt_thirdJet = -1;
  
  vars.thirdJ = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_thirdJ = NULL;
  
  vars.thirdJ_bTag = -99.;
  vars.thirdJ_WJJDeta = -99.;
  vars.thirdJ_WJJDphi = -99.;
  vars.thirdJ_WJJDR = -99.;
  vars.thirdJ_tagJJDeta = -99.;
  vars.thirdJ_tagJJDphi = -99.;
  vars.thirdJ_tagJJDR = -99.;
  
  
  // MC variables
  vars.mc_lep = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_mc_lep = NULL;
  
  vars.mc_lep_charge = -1.;
  vars.mc_lep_flavour = -1;
  vars.mc_lep_pt = -99.;
  vars.mc_lep_eta = -99.;
  vars.mc_lep_phi = -99.;
  
  
  vars.mc_nu = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_mc_nu = NULL;
  
  vars.mc_nu_charge = -1.;
  vars.mc_nu_flavour = -1;
  vars.mc_nu_pt = -99.;
  vars.mc_nu_eta = -99.;
  vars.mc_nu_phi = -99.;
  
  
  vars.mc_lepNu_pt = -99.;
  vars.mc_lepNu_mt = -99.;
  vars.mc_lepNu_Dphi = -99.;
  
  
  vars.mc_WJ1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_mc_WJ1 = NULL;
  vars.mc_WJ2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_mc_WJ2 = NULL;
  
  vars.mc_WJ1_pt = -99.;
  vars.mc_WJ2_pt = -99.;
  vars.mc_WJ1_eta = -99.;
  vars.mc_WJ2_eta = -99.;
  vars.mc_WJ1_phi = -99.;
  vars.mc_WJ2_phi = -99.;
  
  vars.mc_WJJ_Deta = -99.;
  vars.mc_WJJ_Dphi = -99.;
  vars.mc_WJJ_DR = -99.;
  vars.mc_WJJ_pt = -99.;
  vars.mc_WJJ_m = -99.;
  
  
  vars.mc_W1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_mc_W1 = NULL;
  vars.mc_W2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_mc_W2 = NULL;
  
  vars.mc_W1_pt = -99.;
  vars.mc_W2_pt = -99.;
  vars.mc_W1_eta = -99.;
  vars.mc_W2_eta = -99.;
  vars.mc_W1_phi = -99.;
  vars.mc_W2_phi = -99.;
  vars.mc_W1_m = -99.;
  vars.mc_W2_m = -99.;
  
  
  vars.mc_H = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_mc_H = NULL;
  
  vars.mc_H_pt = -99.;
  vars.mc_H_eta = -99.;
  vars.mc_H_phi = -99.;
  vars.mc_H_m = -99.;
  
}



void DeleteVBFPreselectionVariables(VBFPreselectionVariables& vars)
{
  // save tree
  vars.m_outputRootFile -> cd();
  vars.m_reducedTree -> Write();
  vars.m_outputRootFile -> Close();
}






void SetGenVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& dataFlag, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetGenVariables" << std::endl;
  
  if( dataFlag == 0 )
  {
    vars.genPtHat  = reader.GetFloat("mc_ptHat")->at(0);
    vars.genWeight = reader.GetFloat("mc_weight")->at(0);
    vars.genNUP    = (int)(reader.GetFloat("mc_NUP")->at(0));
  }
}



void SetPUVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& dataFlag, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetPUVariables" << std::endl;
  
  if( dataFlag == 0 )
  {
    vars.PUtrue_n      = (int)(reader.GetFloat("mc_PUit_TrueNumInteractions")->at(0));
    vars.PUit_n        = (int)(reader.GetInt("mc_PUit_NumInteractions")->at(0));
    vars.PUoot_early_n = (int)(reader.GetInt("mc_PUoot_early_NumInteractions")->at(0));
    vars.PUoot_late_n  = (int)(reader.GetInt("mc_PUoot_late_NumInteractions")->at(0));
  }
  
  vars.rhoForIsolation = reader.GetFloat("rho_isolation")->at(0);
  vars.rhoForJets      = reader.GetFloat("rho_jets")->at(0);
  vars.rhoForJetsPFlow = reader.GetFloat("rho_jetsPFlow")->at(0);
}



void SetHLTVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetHLTVariables" << std::endl;
  
  vars.HLT_Names =  *(reader.GetString("HLT_Names"));
  vars.p_HLT_Names = &(vars.HLT_Names);
  vars.HLT_Accept =  *(reader.GetFloat("HLT_Accept"));
  vars.p_HLT_Accept = &(vars.HLT_Accept);
}



void SetPDFVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetPDFVariables" << std::endl;
  
  vars.PDF_weights_CT10 = *(reader.GetFloat("CT10pdfWeights"));
  vars.p_PDF_weights_CT10 = &(vars.PDF_weights_CT10);

  vars.PDF_weights_MSTW2008nlo68cl = *(reader.GetFloat("MSTW2008nlo68clpdfWeights"));
  vars.p_PDF_weights_MSTW2008nlo68cl = &(vars.PDF_weights_MSTW2008nlo68cl);

  vars.PDF_weights_NNPDF20 = *(reader.GetFloat("NNPDF20pdfWeights"));
  vars.p_PDF_weights_NNPDF20 = &(vars.PDF_weights_NNPDF20);
}



void SetPVVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetPVVariables" << std::endl;
  
  vars.PV_n = (int)(reader.GetFloat("PV_d0")->size());
  vars.PV_d0 = reader.GetFloat("PV_d0")->at(0);
  vars.PV_nTracks = reader.GetInt("PV_nTracks")->at(0);
  vars.PV_ndof = reader.GetInt("PV_ndof")->at(0);
  vars.PV_normalizedChi2 = reader.GetFloat("PV_normalizedChi2")->at(0);
  vars.PV_z = reader.GetFloat("PV_z")->at(0);
}



void SetLeptonVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetLeptonVariables" << std::endl;
  
  vars.lep = vars.leptons.at(vars.selectIt_lep);
  vars.p_lep = &vars.lep;
  
  vars.lep_pt      = vars.p_lep->pt();
  vars.lep_eta     = vars.p_lep->eta();
  vars.lep_phi     = vars.p_lep->phi();
  vars.lep_charge  = vars.leptonCharges.at(vars.selectIt_lep);
  vars.lep_z       = vars.leptons_z.at(vars.selectIt_lep);
  vars.lep_dxy_BS  = vars.leptons_dxy_BS.at(vars.selectIt_lep);
  vars.lep_dz_BS   = vars.leptons_dz_BS.at(vars.selectIt_lep);
  vars.lep_dB      = vars.leptons_dB.at(vars.selectIt_lep);
  vars.lep_edB     = vars.leptons_edB.at(vars.selectIt_lep);
  vars.lep_dz_PV   = vars.leptons_dz_PV.at(vars.selectIt_lep);
  vars.lep_tkIso   = vars.leptons_tkIso.at(vars.selectIt_lep);
  vars.lep_emIso   = vars.leptons_emIso.at(vars.selectIt_lep);
  vars.lep_hadIso  = vars.leptons_hadIso.at(vars.selectIt_lep);
  

  // if electron
  vars.selectIt_ele = -1;
  if(vars.leptonFlavours.at(vars.selectIt_lep) == "electron")
  {
    for(unsigned int eleIt = 0; eleIt < vars.electrons.size(); ++eleIt)
    {
      ROOT::Math::XYZTVector ele = vars.electrons.at(eleIt);
      if( deltaR(ele.eta(), ele.phi(), vars.lep.eta(), vars.lep.phi()) < 0.0001 )
        vars.selectIt_ele = eleIt;
    }
    
    vars.lep_flavour = 11;
    vars.lep_isEB = vars.electrons_isEB.at(vars.selectIt_ele);
    vars.lep_etaSC = vars.electrons_etaSC.at(vars.selectIt_ele);
    vars.lep_sigmaIetaIeta = vars.electrons_sigmaIetaIeta.at(vars.selectIt_ele);
    vars.lep_DphiIn = vars.electrons_DphiIn.at(vars.selectIt_ele);
    vars.lep_DetaIn = vars.electrons_DetaIn.at(vars.selectIt_ele);
    vars.lep_HOverE = vars.electrons_HOverE.at(vars.selectIt_ele);
    vars.lep_fbrem  = vars.electrons_fbrem.at(vars.selectIt_ele);
    vars.lep_EOverP = vars.electrons_EOverP.at(vars.selectIt_ele);
    vars.lep_mishits = vars.electrons_mishits.at(vars.selectIt_ele);
    vars.lep_dist = vars.electrons_dist.at(vars.selectIt_ele);
    vars.lep_dcot = vars.electrons_dcot.at(vars.selectIt_ele);
    
    // hlt matching
    vars.lep_hltMatched = 0;
    float thisDR_DPT = 1000.;
    for(unsigned int hltEleIt = 0; hltEleIt < reader.Get4V("goodHLT_electrons")->size(); ++hltEleIt)
    {
      ROOT::Math::XYZTVector ele_hlt = reader.Get4V("goodHLT_electrons")->at(hltEleIt);
      float thisDR = deltaR(ele_hlt.eta(), ele_hlt.phi(), vars.lep.eta(), vars.lep.phi());
      // discard hlt ele more distant than dr = 0.1 from the offline ele
      if( thisDR > 0.1 ) continue;
      vars.lep_hltMatched = 1;
      if( thisDR * fabs(vars.lep.pt() - ele_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(vars.lep.pt() - ele_hlt.pt()); 
        vars.lep_hltMatchPt = ele_hlt.pt();
        vars.lep_hltPass = reader.GetInt("goodHLT_electrons_pass")->at(hltEleIt);
      }
    }
    
  }
  
  // if muon
  if(vars.leptonFlavours.at(vars.selectIt_lep) == "muon")
  { 
    for(unsigned int muIt = 0; muIt < vars.muons.size(); ++muIt)
    {
      ROOT::Math::XYZTVector mu = vars.muons.at(muIt);
        if(deltaR(mu.eta(), mu.phi(), vars.lep.eta(), vars.lep.phi()) < 0.0001)
          vars.selectIt_mu = muIt;
    }

    vars.lep_flavour = 13;    
    vars.lep_tracker = vars.muons_tracker.at(vars.selectIt_mu);
    vars.lep_standalone = vars.muons_standalone.at(vars.selectIt_mu);
    vars.lep_global = vars.muons_global.at(vars.selectIt_mu);
    vars.lep_normalizedChi2 = vars.muons_normalizedChi2.at(vars.selectIt_mu);
    vars.lep_numberOfMatches = vars.muons_numberOfMatches.at(vars.selectIt_mu);
    vars.lep_numberOfValidTrackerHits = vars.muons_numberOfValidTrackerHits.at(vars.selectIt_mu);
    vars.lep_numberOfValidMuonHits = vars.muons_numberOfValidMuonHits.at(vars.selectIt_mu);
    vars.lep_pixelLayersWithMeasurement = vars.muons_pixelLayersWithMeasurement.at(vars.selectIt_mu);
    
    // hlt matching
    float thisDR_DPT = 1000.;
    vars.lep_hltMatched = 0;
    for(unsigned int hltMuIt = 0; hltMuIt < reader.Get4V("goodHLT_muons")->size(); ++hltMuIt)
    {
      ROOT::Math::XYZTVector mu_hlt = reader.Get4V("goodHLT_muons")->at(hltMuIt);
      float thisDR = deltaR(mu_hlt.eta(), mu_hlt.phi(), vars.lep.eta(), vars.lep.phi());
      // discard hlt mu more distant than dr = 0.1 from the offline mu
      if( thisDR > 0.1 ) continue;
      vars.lep_hltMatched = 1;
      if( thisDR * fabs(vars.lep.pt() - mu_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(vars.lep.pt() - mu_hlt.pt()); 
        vars.lep_hltMatchPt = mu_hlt.pt();
        vars.lep_hltPass = reader.GetInt("goodHLT_muons_pass")->at(hltMuIt);
      }
    }
    
  }
  
}



void SetLepton2Variables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetLepton2Variables" << std::endl;
  
  vars.lep2 = vars.leptons.at(vars.selectIt_lep2);
  vars.p_lep2 = &vars.lep2;
  
  vars.lep2_pt      = vars.p_lep2->pt();
  vars.lep2_eta     = vars.p_lep2->eta();
  vars.lep2_phi     = vars.p_lep2->phi();
  vars.lep2_charge  = vars.leptonCharges.at(vars.selectIt_lep2);
  vars.lep2_z       = vars.leptons_z.at(vars.selectIt_lep2);
  vars.lep2_dxy_BS  = vars.leptons_dxy_BS.at(vars.selectIt_lep2);
  vars.lep2_dz_BS   = vars.leptons_dz_BS.at(vars.selectIt_lep2);
  vars.lep2_dB      = vars.leptons_dB.at(vars.selectIt_lep2);
  vars.lep2_edB     = vars.leptons_edB.at(vars.selectIt_lep2);
  vars.lep2_dz_PV   = vars.leptons_dz_PV.at(vars.selectIt_lep2);
  vars.lep2_tkIso   = vars.leptons_tkIso.at(vars.selectIt_lep2);
  vars.lep2_emIso   = vars.leptons_emIso.at(vars.selectIt_lep2);
  vars.lep2_hadIso  = vars.leptons_hadIso.at(vars.selectIt_lep2);
  

  // if electron
  vars.selectIt_ele2 = -1;
  if(vars.leptonFlavours.at(vars.selectIt_lep2) == "electron")
  {
    for(unsigned int eleIt = 0; eleIt < vars.electrons.size(); ++eleIt)
    {
      ROOT::Math::XYZTVector ele = vars.electrons.at(eleIt);
      if( deltaR(ele.eta(), ele.phi(), vars.lep2.eta(), vars.lep2.phi()) < 0.0001 )
        vars.selectIt_ele2 = eleIt;
    }
    
    vars.lep2_flavour = 11;
    vars.lep2_isEB = vars.electrons_isEB.at(vars.selectIt_ele2);
    vars.lep2_etaSC = vars.electrons_etaSC.at(vars.selectIt_ele2);
    vars.lep2_sigmaIetaIeta = vars.electrons_sigmaIetaIeta.at(vars.selectIt_ele2);
    vars.lep2_DphiIn = vars.electrons_DphiIn.at(vars.selectIt_ele2);
    vars.lep2_DetaIn = vars.electrons_DetaIn.at(vars.selectIt_ele2);
    vars.lep2_HOverE = vars.electrons_HOverE.at(vars.selectIt_ele2);
    vars.lep2_fbrem  = vars.electrons_fbrem.at(vars.selectIt_ele2);
    vars.lep2_EOverP = vars.electrons_EOverP.at(vars.selectIt_ele2);
    vars.lep2_mishits = vars.electrons_mishits.at(vars.selectIt_ele2);
    vars.lep2_dist = vars.electrons_dist.at(vars.selectIt_ele2);
    vars.lep2_dcot = vars.electrons_dcot.at(vars.selectIt_ele2);
    
    // hlt matching
    float thisDR_DPT = 1000.;
    vars.lep2_hltMatched = 0;
    for(unsigned int hltEleIt = 0; hltEleIt < reader.Get4V("goodHLT_electrons")->size(); ++hltEleIt)
    {
      ROOT::Math::XYZTVector ele_hlt = reader.Get4V("goodHLT_electrons")->at(hltEleIt);
      float thisDR = deltaR(ele_hlt.eta(), ele_hlt.phi(), vars.lep2.eta(), vars.lep2.phi());
      // discard hlt ele more distant than dr = 0.1 from the offline ele
      if( thisDR > 0.1 ) continue;
      vars.lep2_hltMatched = 1;
      if( thisDR * fabs(vars.lep2.pt() - ele_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(vars.lep2.pt() - ele_hlt.pt()); 
        vars.lep2_hltMatchPt = ele_hlt.pt();
        vars.lep2_hltPass = reader.GetInt("goodHLT_electrons_pass")->at(hltEleIt);
      }
    }
    
  }
  
  // if muon
  if(vars.leptonFlavours.at(vars.selectIt_lep2) == "muon")
  { 
    for(unsigned int muIt = 0; muIt < vars.muons.size(); ++muIt)
    {
      ROOT::Math::XYZTVector mu = vars.muons.at(muIt);
        if(deltaR(mu.eta(), mu.phi(), vars.lep2.eta(), vars.lep2.phi()) < 0.0001)
          vars.selectIt_mu2 = muIt;
    }

    vars.lep2_flavour = 13;    
    vars.lep2_tracker = vars.muons_tracker.at(vars.selectIt_mu2);
    vars.lep2_standalone = vars.muons_standalone.at(vars.selectIt_mu2);
    vars.lep2_global = vars.muons_global.at(vars.selectIt_mu2);
    vars.lep2_normalizedChi2 = vars.muons_normalizedChi2.at(vars.selectIt_mu2);
    vars.lep2_numberOfMatches = vars.muons_numberOfMatches.at(vars.selectIt_mu2);
    vars.lep2_numberOfValidTrackerHits = vars.muons_numberOfValidTrackerHits.at(vars.selectIt_mu2);
    vars.lep2_numberOfValidMuonHits = vars.muons_numberOfValidMuonHits.at(vars.selectIt_mu2);
    vars.lep2_pixelLayersWithMeasurement = vars.muons_pixelLayersWithMeasurement.at(vars.selectIt_mu2);
    
    // hlt matching
    float thisDR_DPT = 1000.;
    vars.lep2_hltMatched = 0;
    for(unsigned int hltMuIt = 0; hltMuIt < reader.Get4V("goodHLT_muons")->size(); ++hltMuIt)
    {
      ROOT::Math::XYZTVector mu_hlt = reader.Get4V("goodHLT_muons")->at(hltMuIt);
      float thisDR = deltaR(mu_hlt.eta(), mu_hlt.phi(), vars.lep2.eta(), vars.lep2.phi());
      // discard hlt mu more distant than dr = 0.1 from the offline mu
      if( thisDR > 0.1 ) continue;
      vars.lep2_hltMatched = 1;
      if( thisDR * fabs(vars.lep2.pt() - mu_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(vars.lep2.pt() - mu_hlt.pt()); 
        vars.lep2_hltMatchPt = mu_hlt.pt();
        vars.lep2_hltPass = reader.GetInt("goodHLT_muons_pass")->at(hltMuIt);
      }
    }
    
  }
  
}



void SetElectronVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& eleIt, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetElectronVariables" << std::endl;
  
  vars.electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
  vars.leptons.push_back( reader.Get4V("electrons")->at(eleIt) );
  vars.leptonCharges.push_back( reader.GetFloat("electrons_charge")->at(eleIt) );
  vars.leptonFlavours.push_back("electron");
  
  vars.leptons_tkIso.push_back(reader.GetFloat("electrons_tkIsoR03")->at(eleIt));
  vars.leptons_emIso.push_back(reader.GetFloat("electrons_emIsoR03")->at(eleIt));
  vars.leptons_hadIso.push_back(reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt)+
			        reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt));
  vars.leptons_z.push_back(reader.GetFloat("electrons_z")->at(eleIt));
  vars.leptons_dxy_BS.push_back(reader.GetFloat("electrons_dxy_BS")->at(eleIt));
  vars.leptons_dz_BS.push_back(reader.GetFloat("electrons_dz_BS")->at(eleIt));
  vars.leptons_dB.push_back(reader.GetFloat("electrons_dB")->at(eleIt));
  vars.leptons_edB.push_back(reader.GetFloat("electrons_edB")->at(eleIt));
  vars.leptons_dz_PV.push_back(reader.GetFloat("electrons_dz_PV")->at(eleIt));
  
  vars.electrons_isEB.push_back(reader.GetInt("electrons_isEB")->at(eleIt));
  vars.electrons_etaSC.push_back(reader.Get3PV("electrons_positionSC")->at(eleIt).eta());
  vars.electrons_sigmaIetaIeta.push_back(reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt));
  vars.electrons_DphiIn.push_back(reader.GetFloat("electrons_deltaPhiIn")->at(eleIt));
  vars.electrons_DetaIn.push_back(reader.GetFloat("electrons_deltaEtaIn")->at(eleIt));
  vars.electrons_HOverE.push_back(reader.GetFloat("electrons_hOverE")->at(eleIt));
  vars.electrons_fbrem.push_back(reader.GetFloat("electrons_fbrem")->at(eleIt));
  vars.electrons_EOverP.push_back(reader.GetFloat("electrons_eSCOverP")->at(eleIt));
  vars.electrons_mishits.push_back(reader.GetInt("electrons_mishits")->at(eleIt));
  vars.electrons_dist.push_back(reader.GetFloat("electrons_dist")->at(eleIt));
  vars.electrons_dcot.push_back(reader.GetFloat("electrons_dcot")->at(eleIt));
}



void SetMuonVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& muIt, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetMuonVariables" << std::endl;
  
  vars.muons.push_back( reader.Get4V("muons")->at(muIt) );
  vars.leptons.push_back( reader.Get4V("muons")->at(muIt) );
  vars.leptonCharges.push_back( reader.GetFloat("muons_charge")->at(muIt) );
  vars.leptonFlavours.push_back("muon");
  vars.leptons_tkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
  vars.leptons_emIso.push_back(reader.GetFloat("muons_emIsoR03")->at(muIt));
  vars.leptons_hadIso.push_back(reader.GetFloat("muons_hadIsoR03")->at(muIt));
  vars.leptons_z.push_back(reader.GetFloat("muons_z")->at(muIt));
  vars.leptons_dxy_BS.push_back(reader.GetFloat("muons_dxy_BS")->at(muIt));
  vars.leptons_dz_BS.push_back(reader.GetFloat("muons_dz_BS")->at(muIt));
  vars.leptons_dB.push_back(reader.GetFloat("muons_dB")->at(muIt));
  vars.leptons_edB.push_back(reader.GetFloat("muons_edB")->at(muIt));
  vars.leptons_dz_PV.push_back(reader.GetFloat("muons_dz_PV")->at(muIt));
  
  vars.muons_tracker.push_back(reader.GetInt("muons_tracker")->at(muIt));
  vars.muons_standalone.push_back(reader.GetInt("muons_standalone")->at(muIt));
  vars.muons_global.push_back(reader.GetInt("muons_global")->at(muIt));
  vars.muons_normalizedChi2.push_back(reader.GetFloat("muons_normalizedChi2")->at(muIt));
  vars.muons_numberOfMatches.push_back(reader.GetInt("muons_numberOfMatches")->at(muIt));
  vars.muons_numberOfValidTrackerHits.push_back(reader.GetInt("muons_numberOfValidTrackerHits")->at(muIt));
  vars.muons_numberOfValidMuonHits.push_back(reader.GetInt("muons_numberOfValidMuonHits")->at(muIt));
  vars.muons_pixelLayersWithMeasurement.push_back(reader.GetInt("muons_pixelLayersWithMeasurement")->at(muIt));
}



void SetMetVariables(VBFPreselectionVariables& vars, treeReader& reader, const std::string& jetType, 
                     const float& JESScaleVariation, TH2F* JECUncertainty, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetMetVariables" << std::endl;
  
  // met
  if(jetType == "Calo")
    vars.met = reader.Get4V("type1Met")->at(0);
  if(jetType == "PF")
    vars.met = reader.Get4V("PFMet")->at(0);
  
  
  if( JESScaleVariation != 0. )
  {
    ROOT::Math::XYZVector myMet = -1. * (vars.lep.Vect() );
    ROOT::Math::XYZVector myScaledMet = -1. * (vars.lep.Vect() );
    
    
    float JESScale = 1.;
    
    for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
    {
      ROOT::Math::XYZVector jet = (reader.Get4V("jets")->at(jetIt)).Vect();
      
      float eta = jet.eta();
      if( eta < -5. ) eta = -5.;
      if( eta > 5. ) eta = 5.;
      
      float pt = sqrt( jet.perp2() );
      if( pt < 0. ) pt = 0.;
      if( pt > 1000. ) pt = 1000.;
      int bin = JECUncertainty -> FindFixBin(eta,pt);
      
      JESScale = 1. + JESScaleVariation * JECUncertainty->GetBinContent(bin);
      
      myMet -= jet;
      myScaledMet -= JESScale * jet;
    }
    
    ROOT::Math::XYZVector myUnclusterizedMet = (vars.met).Vect() - myMet;
    JESScale = 1. + JESScaleVariation * 0.05;
    myScaledMet += JESScale * myUnclusterizedMet;
    
    //std::cout << "met: " << vars.met.Et() << "   myMet: " << sqrt(myMet.perp2()) << std::endl;
    vars.met.SetPxPyPzE(myScaledMet.X(),myScaledMet.Y(),0.,sqrt(myScaledMet.perp2()));
  }
  
  
  vars.p_met = &vars.met;
  vars.met_et = vars.p_met->Et();
  vars.met_phi = vars.p_met->phi();
  
  vars.lepMet = vars.lep + vars.met;
  
  vars.lepMet_Dphi = deltaPhi(vars.lep.phi(), vars.met.phi());
  vars.lepMet_pt = vars.lepMet.pt();
  vars.lepMet_mt = sqrt( vars.lep.mass() * vars.lep.mass() +
			 2. * vars.lep.pt() * vars.met.pt() * ( 1 - cos(deltaPhi(vars.lep.phi(), vars.met.phi()) ) ) );
}



void SetBTagVariables(VBFPreselectionVariables& vars, treeReader& reader, const std::string& jetType, const float& jetEtaCNT, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetBTagVariables" << std::endl;
  
  for(unsigned int jetIt = 0; jetIt < (reader.Get4V("jets")->size()); ++jetIt)
  {
    ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);
    
    // jet eta max
    if( fabs(jet.eta()) > jetEtaCNT ) continue;
    
    // clean jets from selected lepton
    float DR = deltaR(jet.eta(), jet.phi(), vars.lep.eta(), vars.lep.phi());
    if( DR < 0.5 ) continue;
    
    // jetID
    if(jetType == "Calo")
    {
      if( (fabs(jet.eta()) < 2.6) && (reader.GetFloat("jets_emEnergyFraction")->at(jetIt) < 0.01) ) continue;
      if( reader.GetFloat("jets_emEnergyFraction")->at(jetIt) >= 0.98 ) continue;
      if( reader.GetFloat("jets_n90Hits")->at(jetIt) < 2 ) continue;
      if( reader.GetFloat("jets_fHPD")->at(jetIt) >= 0.98 ) continue;
    }
    
    if(jetType == "PF")
    {
      if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_neutralHadronEnergyFraction")->at(jetIt) >= 0.99) ) continue;
      if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
      if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_chargedHadronEnergyFraction")->at(jetIt) <= 0.) ) continue;
      if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_chargedEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
      if( (fabs(jet.eta()) < 2.4) && (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) <= 0) ) continue;
      //if( (fabs(jet.eta()) < 2.4) && (reader.GetFloat("jets_muonEnergyFraction")->at(jetIt) >= 0.99) ) continue;
      if( (fabs(jet.eta()) >= 2.4) && (reader.GetFloat("jets_neutralHadronEnergyFraction")->at(jetIt) >= 0.99) ) continue;
      if( (fabs(jet.eta()) >= 2.4) && (reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) >= 0.99) ) continue;
      if( (fabs(jet.eta()) >= 2.4) && (reader.GetInt("jets_chargedMultiplicity")->at(jetIt) + reader.GetInt("jets_neutralMultiplicity")->at(jetIt) <= 1) ) continue;
    }
    
    if( (reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) > 1.70) && (jet.pt() > 20.) ) ++vars.nBTag_TCHEL_pt20;
    if( (reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) > 3.30) && (jet.pt() > 20.) ) ++vars.nBTag_TCHEM_pt20;
    if( (reader.GetFloat("trackCountingHighPurBJetTags")->at(jetIt) > 1.93) && (jet.pt() > 20.) ) ++vars.nBTag_TCHPM_pt20;
    if( (reader.GetFloat("trackCountingHighPurBJetTags")->at(jetIt) > 4.41) && (jet.pt() > 20.) ) ++vars.nBTag_TCHPT_pt20;
    if( (reader.GetFloat("simpleSecondaryVertexHighEffBJetTags")->at(jetIt) > 1.74) && (jet.pt() > 20.) ) ++vars.nBTag_SSVHEM_pt20;
    if( (reader.GetFloat("simpleSecondaryVertexHighPurBJetTags")->at(jetIt) > 2.00) && (jet.pt() > 20.) ) ++vars.nBTag_SSVHPT_pt20;
    
    if( (reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) > 1.70) && (jet.pt() > 25.) ) ++vars.nBTag_TCHEL_pt25;
    if( (reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) > 3.30) && (jet.pt() > 25.) ) ++vars.nBTag_TCHEM_pt25;
    if( (reader.GetFloat("trackCountingHighPurBJetTags")->at(jetIt) > 1.93) && (jet.pt() > 25.) ) ++vars.nBTag_TCHPM_pt25;
    if( (reader.GetFloat("trackCountingHighPurBJetTags")->at(jetIt) > 4.41) && (jet.pt() > 25.) ) ++vars.nBTag_TCHPT_pt25;
    if( (reader.GetFloat("simpleSecondaryVertexHighEffBJetTags")->at(jetIt) > 1.74) && (jet.pt() > 25.) ) ++vars.nBTag_SSVHEM_pt25;
    if( (reader.GetFloat("simpleSecondaryVertexHighPurBJetTags")->at(jetIt) > 2.00) && (jet.pt() > 25.) ) ++vars.nBTag_SSVHPT_pt25;
    
    if( (reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) > 1.70) && (jet.pt() > 30.) ) ++vars.nBTag_TCHEL_pt30;
    if( (reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) > 3.30) && (jet.pt() > 30.) ) ++vars.nBTag_TCHEM_pt30;
    if( (reader.GetFloat("trackCountingHighPurBJetTags")->at(jetIt) > 1.93) && (jet.pt() > 30.) ) ++vars.nBTag_TCHPM_pt30;
    if( (reader.GetFloat("trackCountingHighPurBJetTags")->at(jetIt) > 4.41) && (jet.pt() > 30.) ) ++vars.nBTag_TCHPT_pt30;
    if( (reader.GetFloat("simpleSecondaryVertexHighEffBJetTags")->at(jetIt) > 1.74) && (jet.pt() > 30.) ) ++vars.nBTag_SSVHEM_pt30;
    if( (reader.GetFloat("simpleSecondaryVertexHighPurBJetTags")->at(jetIt) > 2.00) && (jet.pt() > 30.) ) ++vars.nBTag_SSVHPT_pt30;
  }
  
}



void SetJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& jetIt, const std::string& jetType, const float& jetEtaCNT, const float& jetEtaFWD, const float& JESScaleVariation, TH2F* JECUncertainty, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetJetVariables" << std::endl;
  
  ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);  
  
  
  if( JESScaleVariation != 0. )
  {
    float eta = jet.eta();
    if( eta < -5. ) eta = -5.;
    if( eta > 5. ) eta = 5.;
    
    float pt = jet.pt();
    if( pt < 0. ) pt = 0.;
    if( pt > 1000. ) pt = 1000.;
    int bin = JECUncertainty -> FindFixBin(eta,pt);
    
    float JESScale = 1. + JESScaleVariation * JECUncertainty->GetBinContent(bin);
    //std::cout << "eta: " << eta << "   pt: " << pt << "   JESScale: " << JESScale << std::endl;                                                                              
    jet.SetPxPyPzE(JESScale*jet.Px(),JESScale*jet.Py(),JESScale*jet.Pz(),fabs(JESScale)*jet.E());
  }
  
  
  vars.jets.push_back( jet );
  
  ++vars.nJets;  
  if( fabs(jet.eta()) < jetEtaCNT )  ++vars.nJets_cnt;  
  if( fabs(jet.eta()) >= jetEtaFWD ) ++vars.nJets_fwd;  
  
  if( jet.pt() > 20.)
  {
    ++vars.nJets_pt20;
    if( fabs(jet.eta()) < jetEtaCNT )  ++vars.nJets_cnt_pt20;
    if( fabs(jet.eta()) >= jetEtaFWD ) ++vars.nJets_fwd_pt20;
  }
  if( jet.pt() > 25.)
  {
    ++vars.nJets_pt25;
    if( fabs(jet.eta()) < jetEtaCNT )  ++vars.nJets_cnt_pt25;
    if( fabs(jet.eta()) >= jetEtaFWD ) ++vars.nJets_fwd_pt25;
  }
  if( jet.pt() > 30.)
  {
    ++vars.nJets_pt30;
    if( fabs(jet.eta()) < jetEtaCNT )  ++vars.nJets_cnt_pt30;
    if( fabs(jet.eta()) >= jetEtaFWD ) ++vars.nJets_fwd_pt30;
  }

  vars.jets_etaEtaMoment.push_back( reader.GetFloat("jets_etaetaMoment")->at(jetIt) );
  vars.jets_phiPhiMoment.push_back( reader.GetFloat("jets_phiphiMoment")->at(jetIt) );
  vars.jets_etaPhiMoment.push_back( reader.GetFloat("jets_etaphiMoment")->at(jetIt) );
  vars.jets_ptD.push_back( reader.GetFloat("jets_ptD")->at(jetIt) );
  
  if(jetType == "Calo")
  {
    vars.jets_emEnergyFraction.push_back( reader.GetFloat("jets_emEnergyFraction")->at(jetIt) );
    vars.jets_fHPD.push_back( reader.GetFloat("jets_fHPD")->at(jetIt) );
    vars.jets_fRBX.push_back( reader.GetFloat("jets_fRBX")->at(jetIt) );
    vars.jets_n90Hits.push_back( reader.GetFloat("jets_n90Hits")->at(jetIt) );
    
    vars.jets_chargedHadronEnergyFraction.push_back( -99. );
    vars.jets_neutralHadronEnergyFraction.push_back( -99. );
    vars.jets_chargedEmEnergyFraction.push_back( -99. );
    vars.jets_neutralEmEnergyFraction.push_back( -99. );
    vars.jets_muonEnergyFraction.push_back( -99. );
    vars.jets_chargedMultiplicity.push_back( -1 );
    vars.jets_neutralMultiplicity.push_back( -1 );
    vars.jets_muonMultiplicity.push_back( -1 );
  }
  
  if(jetType == "PF")
  {
    vars.jets_emEnergyFraction.push_back( -99. );
    vars.jets_fHPD.push_back( -99. );
    vars.jets_fRBX.push_back( -99. );
    vars.jets_n90Hits.push_back( -99 );
    
    vars.jets_chargedHadronEnergyFraction.push_back( reader.GetFloat("jets_chargedHadronEnergyFraction")->at(jetIt) );
    vars.jets_neutralHadronEnergyFraction.push_back( reader.GetFloat("jets_neutralHadronEnergyFraction")->at(jetIt) );
    vars.jets_chargedEmEnergyFraction.push_back( reader.GetFloat("jets_chargedEmEnergyFraction")->at(jetIt) );
    vars.jets_neutralEmEnergyFraction.push_back( reader.GetFloat("jets_neutralEmEnergyFraction")->at(jetIt) );
    vars.jets_muonEnergyFraction.push_back( reader.GetFloat("jets_muonEnergyFraction")->at(jetIt) );
    vars.jets_chargedMultiplicity.push_back( reader.GetInt("jets_chargedMultiplicity")->at(jetIt) );
    vars.jets_neutralMultiplicity.push_back( reader.GetInt("jets_neutralMultiplicity")->at(jetIt) );
    vars.jets_muonMultiplicity.push_back( reader.GetInt("jets_muonMultiplicity")->at(jetIt) );
  }
  
  vars.jets_ptOrdered.push_back( reader.Get4V("jets")->at(jetIt).pt() );
  vars.jets_bTag.push_back( reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) );
  vars.jets_bTagOrdered.push_back( reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) );
  
  
  
  if( vars.jets.size() == 1 )
  {
    vars.jet1 = jet;
    vars.p_jet1 = &vars.jet1;
    vars.jet1_bTag = vars.jets_bTag.at(vars.jets.size()-1);
    // hlt matching
    float thisDR_DPT = 1000.;
    for ( unsigned int hltJetIt = 0; hltJetIt < reader.Get4V("goodHLT_jets")->size(); ++hltJetIt )
    {
      ROOT::Math::XYZTVector jet_hlt = reader.Get4V("goodHLT_jets")->at(hltJetIt);
      float thisDR = deltaR(jet_hlt.eta(), jet_hlt.phi(), jet.eta(), jet.phi());
      // discard hlt Jet more distant than dr = 0.5 from the offline Jet
      if ( thisDR > 0.5 ) continue;
      vars.jet1_hltMatched = 1;
      if ( thisDR * fabs(jet.pt() - jet_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(jet.pt() - jet_hlt.pt()); 
        vars.jet1_hltMatchPt = jet_hlt.pt();
      }
    }
  }
  if( vars.jets.size() == 2 )
  {
    vars.jet2 = jet;
    vars.p_jet2 = &vars.jet2;
    vars.jet2_bTag = vars.jets_bTag.at(vars.jets.size()-1);
    // hlt matching
    float thisDR_DPT = 1000.;
    for ( unsigned int hltJetIt = 0; hltJetIt < reader.Get4V("goodHLT_jets")->size(); ++hltJetIt )
    {
      ROOT::Math::XYZTVector jet_hlt = reader.Get4V("goodHLT_jets")->at(hltJetIt);
      float thisDR = deltaR(jet_hlt.eta(), jet_hlt.phi(), jet.eta(), jet.phi());
      // discard hlt Jet more distant than dr = 0.5 from the offline Jet
      if ( thisDR > 0.5 ) continue;
      vars.jet2_hltMatched = 1;
      if ( thisDR * fabs(jet.pt() - jet_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(jet.pt() - jet_hlt.pt()); 
        vars.jet2_hltMatchPt = jet_hlt.pt();
      }
    }
  }
  if( vars.jets.size() == 3 )
  {
    vars.jet3 = jet;
    vars.p_jet3 = &vars.jet3;
    vars.jet3_bTag = vars.jets_bTag.at(vars.jets.size()-1);
    // hlt matching
    float thisDR_DPT = 1000.;
    for ( unsigned int hltJetIt = 0; hltJetIt < reader.Get4V("goodHLT_jets")->size(); ++hltJetIt )
    {
      ROOT::Math::XYZTVector jet_hlt = reader.Get4V("goodHLT_jets")->at(hltJetIt);
      float thisDR = deltaR(jet_hlt.eta(), jet_hlt.phi(), jet.eta(), jet.phi());
      // discard hlt Jet more distant than dr = 0.5 from the offline Jet
      if ( thisDR > 0.5 ) continue;
      vars.jet3_hltMatched = 1;
      if ( thisDR * fabs(jet.pt() - jet_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(jet.pt() - jet_hlt.pt()); 
        vars.jet3_hltMatchPt = jet_hlt.pt();
      }
    }
  }
  if( vars.jets.size() == 4 )
  {
    vars.jet4 = jet;
    vars.p_jet4 = &vars.jet4;
    vars.jet4_bTag = vars.jets_bTag.at(vars.jets.size()-1);
    // hlt matching
    float thisDR_DPT = 1000.;
    for ( unsigned int hltJetIt = 0; hltJetIt < reader.Get4V("goodHLT_jets")->size(); ++hltJetIt )
    {
      ROOT::Math::XYZTVector jet_hlt = reader.Get4V("goodHLT_jets")->at(hltJetIt);
      float thisDR = deltaR(jet_hlt.eta(), jet_hlt.phi(), jet.eta(), jet.phi());
      // discard hlt Jet more distant than dr = 0.5 from the offline Jet
      if ( thisDR > 0.5 ) continue;
      vars.jet4_hltMatched = 1;
      if ( thisDR * fabs(jet.pt() - jet_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(jet.pt() - jet_hlt.pt()); 
        vars.jet4_hltMatchPt = jet_hlt.pt();
      }
    }
  }
  if( vars.jets.size() == 5 )
  {
    vars.jet5 = jet;
    vars.p_jet5 = &vars.jet5;
    vars.jet5_bTag = vars.jets_bTag.at(vars.jets.size()-1);
    // hlt matching
    float thisDR_DPT = 1000.;
    for ( unsigned int hltJetIt = 0; hltJetIt < reader.Get4V("goodHLT_jets")->size(); ++hltJetIt )
    {
      ROOT::Math::XYZTVector jet_hlt = reader.Get4V("goodHLT_jets")->at(hltJetIt);
      float thisDR = deltaR(jet_hlt.eta(), jet_hlt.phi(), jet.eta(), jet.phi());
      // discard hlt Jet more distant than dr = 0.5 from the offline Jet
      if ( thisDR > 0.5 ) continue;
      vars.jet5_hltMatched = 1;
      if ( thisDR * fabs(jet.pt() - jet_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(jet.pt() - jet_hlt.pt()); 
        vars.jet5_hltMatchPt = jet_hlt.pt();
      }
    }
  }
  if( vars.jets.size() == 6 )
  {
    vars.jet6 = jet;
    vars.p_jet6 = &vars.jet6;
    vars.jet6_bTag = vars.jets_bTag.at(vars.jets.size()-1);
    // hlt matching
    float thisDR_DPT = 1000.;
    for ( unsigned int hltJetIt = 0; hltJetIt < reader.Get4V("goodHLT_jets")->size(); ++hltJetIt )
    {
      ROOT::Math::XYZTVector jet_hlt = reader.Get4V("goodHLT_jets")->at(hltJetIt);
      float thisDR = deltaR(jet_hlt.eta(), jet_hlt.phi(), jet.eta(), jet.phi());
      // discard hlt Jet more distant than dr = 0.5 from the offline Jet
      if ( thisDR > 0.5 ) continue;
      vars.jet6_hltMatched = 1;
      if ( thisDR * fabs(jet.pt() - jet_hlt.pt()) < thisDR_DPT ) 
      {
        thisDR_DPT = thisDR * fabs(jet.pt() - jet_hlt.pt()); 
        vars.jet6_hltMatchPt = jet_hlt.pt();
      }
    }
  }
  
}



void SetLeadingJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const float& jetEtaCNT, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetLeadingJetVariables" << std::endl;
  
  float jetPtMAX = -9999.;
  for(unsigned int jetIt = 0; jetIt < vars.jets.size(); ++jetIt)
  {
    if( ( (vars.jets.at(jetIt)).pt() > jetPtMAX ) &&
        ( fabs((vars.jets.at(jetIt)).eta()) < jetEtaCNT ) )
    {
      vars.selectIt_leadingJet = jetIt;
      jetPtMAX = (vars.jets.at(jetIt)).pt();
    }
  }
  
  vars.leadingJ = vars.jets.at(vars.selectIt_leadingJet);
  vars.p_leadingJ = &vars.leadingJ;
  
  vars.leadingJ_bTag = vars.jets_bTag.at(vars.selectIt_leadingJet);
  vars.leadingJ_ptD = vars.jets_ptD.at(vars.selectIt_leadingJet);
  vars.leadingJ_chargedMultiplicity = vars.jets_chargedMultiplicity.at(vars.selectIt_leadingJet);
  vars.leadingJ_neutralMultiplicity = vars.jets_neutralMultiplicity.at(vars.selectIt_leadingJet);
}



void SetWJJVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetWJJVariables" << std::endl;
  
  if( (vars.selectIt_W.at(0) == -1) || (vars.selectIt_W.at(1) == -1) ) return;
  
  vars.WJ1 = vars.jets.at(vars.selectIt_W.at(0));
  vars.p_WJ1 = &vars.WJ1;
  vars.WJ2 = vars.jets.at(vars.selectIt_W.at(1));
  vars.p_WJ2 = &vars.WJ2;

  vars.WJJ = vars.WJ1 + vars.WJ2;
  vars.p_WJJ = &vars.WJJ;
  
  vars.WJ1_pt = vars.WJ1.pt();
  vars.WJ2_pt = vars.WJ2.pt();
  vars.WJ1_eta = vars.WJ1.eta();
  vars.WJ2_eta = vars.WJ2.eta();
  vars.WJ1_phi = vars.WJ1.phi();
  vars.WJ2_phi = vars.WJ2.phi();
  vars.WJ1_lep_Dphi = deltaPhi(vars.WJ1.phi(),vars.lep.phi());
  vars.WJ2_lep_Dphi = deltaPhi(vars.WJ2.phi(),vars.lep.phi());
  vars.WJ1_met_Dphi = deltaPhi(vars.WJ1.phi(),vars.met.phi());
  vars.WJ2_met_Dphi = deltaPhi(vars.WJ2.phi(),vars.met.phi());
  vars.WJ1_bTag = vars.jets_bTag.at(vars.selectIt_W.at(0));
  vars.WJ2_bTag = vars.jets_bTag.at(vars.selectIt_W.at(1));
  vars.WJ1_ptD = vars.jets_ptD.at(vars.selectIt_W.at(0));
  vars.WJ2_ptD = vars.jets_ptD.at(vars.selectIt_W.at(1));
  vars.WJ1_chargedMultiplicity = vars.jets_chargedMultiplicity.at(vars.selectIt_W.at(0));
  vars.WJ2_chargedMultiplicity = vars.jets_chargedMultiplicity.at(vars.selectIt_W.at(1));
  vars.WJ1_neutralMultiplicity = vars.jets_neutralMultiplicity.at(vars.selectIt_W.at(0));
  vars.WJ2_neutralMultiplicity = vars.jets_neutralMultiplicity.at(vars.selectIt_W.at(1));
  
  vars.WJJ_Deta = deltaEta(vars.WJ1.eta(),vars.WJ2.eta());
  vars.WJJ_Dphi = deltaPhi(vars.WJ1.phi(),vars.WJ2.phi());
  vars.WJJ_DR = deltaR(vars.WJ1.eta(),vars.WJ1.phi(),vars.WJ2.eta(),vars.WJ2.phi());
  vars.WJJ_pt = (vars.WJ1+vars.WJ2).pt();
  vars.WJJ_eta = (vars.WJ1+vars.WJ2).eta();
  vars.WJJ_phi = (vars.WJ1+vars.WJ2).phi();
  vars.WJJ_m = (vars.WJ1+vars.WJ2).mass();
  
  vars.lepWJJ_ptOrdered.push_back( vars.lep.pt() );
  vars.lepWJJ_ptOrdered.push_back( vars.WJ1.pt() );
  vars.lepWJJ_ptOrdered.push_back( vars.WJ2.pt() );
  std::sort(vars.lepWJJ_ptOrdered.begin(), vars.lepWJJ_ptOrdered.end(), maggiore);
  vars.lepWJJ_pt1 = vars.lepWJJ_ptOrdered.at(0);
  vars.lepWJJ_pt2 = vars.lepWJJ_ptOrdered.at(1);
  vars.lepWJJ_pt3 = vars.lepWJJ_ptOrdered.at(2);
}






void SetHVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetHVariables" << std::endl;
  
  vars.lepNu_nSolutions = GetNeutrino(vars.nu1,vars.nu2,vars.lep,vars.met);
  vars.p_nu1 = &(vars.nu1);
  vars.p_nu2 = &(vars.nu2);
  
  if( fabs(vars.nu1.Pz()) < fabs(vars.nu2.Pz()) )
    vars.nu = vars.nu1;
  else
    vars.nu = vars.nu2;
  
  vars.p_nu = &(vars.nu);
  vars.lepNu_m = (vars.lep+vars.nu).mass();
  vars.lepW = vars.lep + vars.WJJ;
  vars.lepW_pt = vars.lepW.pt();
  vars.lepNuW = vars.lepW + vars.nu;
  vars.lepNuW1 = vars.lepW + vars.nu1;
  vars.lepNuW2 = vars.lepW + vars.nu2;
  vars.lepNuW_eta = vars.lepNuW.eta();
  vars.lepNuW_m = vars.lepNuW.mass();
  vars.lepNuW_m1 = vars.lepNuW1.mass();
  vars.lepNuW_m2 = vars.lepNuW2.mass();
  
  vars.lepMetW_pt = (vars.lep + vars.met + vars.WJJ).pt();
  vars.lepMetW_phi = (vars.lep + vars.met + vars.WJJ).phi();
  vars.lepMetW_mt = sqrt( vars.lepW.mass()*vars.lepW.mass() + 2. * vars.lepW.pt() * vars.met.pt() * ( 1 - cos(deltaPhi(vars.lepW.phi(), vars.met.phi()) ) ) );
  vars.lepMetW_Dphi = deltaPhi(vars.lepMet.phi(), vars.WJJ.phi());
  
  //std::cout << "Higgs mt = " << vars.lepMetW_mt << "   Higgs m = " << vars.lepNuW_m << std::endl;
}



void SetTagJJVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetTagJJVariables" << std::endl;
  
  if( (vars.selectIt_tag.at(0) != -1) && (vars.selectIt_tag.at(1) != -1) )
  {
    vars.tagJ1 = vars.jets.at(vars.selectIt_tag.at(0));
    vars.p_tagJ1 = &vars.tagJ1;
    vars.tagJ2 = vars.jets.at(vars.selectIt_tag.at(1));
    vars.p_tagJ2 = &vars.tagJ2;    

    vars.tagJJ = vars.tagJ1 + vars.tagJ2;
    
    vars.tagJ1_bTag = vars.jets_bTag.at(vars.selectIt_tag.at(0));
    vars.tagJ2_bTag = vars.jets_bTag.at(vars.selectIt_tag.at(1));
    vars.tagJ1_ptD = vars.jets_ptD.at(vars.selectIt_tag.at(0));
    vars.tagJ2_ptD = vars.jets_ptD.at(vars.selectIt_tag.at(1));
    vars.tagJ1_chargedMultiplicity = vars.jets_chargedMultiplicity.at(vars.selectIt_tag.at(0));
    vars.tagJ2_chargedMultiplicity = vars.jets_chargedMultiplicity.at(vars.selectIt_tag.at(1));
    vars.tagJ1_neutralMultiplicity = vars.jets_neutralMultiplicity.at(vars.selectIt_tag.at(0));
    vars.tagJ2_neutralMultiplicity = vars.jets_neutralMultiplicity.at(vars.selectIt_tag.at(1));
    
    vars.tagJJ_Deta = deltaEta(vars.tagJ1.eta(),vars.tagJ2.eta());
    vars.tagJJ_Dphi = deltaPhi(vars.tagJ1.phi(),vars.tagJ2.phi());
    vars.tagJJ_DR = deltaR(vars.tagJ1.eta(),vars.tagJ1.phi(),vars.tagJ2.eta(),vars.tagJ2.phi());
    vars.tagJJ_m = (vars.tagJ1+vars.tagJ2).mass();
    
    
    
    for(int jetIt = 0; jetIt < vars.nJets; ++jetIt)
    {
      if( jetIt == vars.selectIt_tag.at(0) ) continue;
      if( jetIt == vars.selectIt_tag.at(1) ) continue;
      if( jetIt == vars.selectIt_W.at(0) ) continue;
      if( jetIt == vars.selectIt_W.at(1) ) continue;
      
      ROOT::Math::XYZTVector jet = vars.jets.at(jetIt);
      float tagEtaMin = std::min(vars.tagJ1.eta(), vars.tagJ2.eta());
      float tagEtaMax = std::max(vars.tagJ1.eta(), vars.tagJ2.eta());
      
      if( jet.pt() > 20.)
      {
        if( (jet.eta() > tagEtaMin) && (jet.eta() < tagEtaMax) ) ++vars.nJets_btw_pt20;
      }
      if( jet.pt() > 25.)
      {
        if( (jet.eta() > tagEtaMin) && (jet.eta() < tagEtaMax) ) ++vars.nJets_btw_pt25;
      }
      if( jet.pt() > 30.)
      {
        if( (jet.eta() > tagEtaMin) && (jet.eta() < tagEtaMax) ) ++vars.nJets_btw_pt30;
      }
    }
    
    
    
    float avgEta = 0.5 * (vars.tagJ1.eta()+vars.tagJ2.eta());
    float Deta = deltaEta(vars.tagJ1.eta(),vars.tagJ2.eta());
    
    vars.lep_zepp = (vars.lep.eta() - avgEta) / Deta;
    
    vars.WJ1_zepp = (vars.WJ1.eta() - avgEta) / Deta;
    vars.WJ2_zepp = (vars.WJ2.eta() - avgEta) / Deta;
    vars.WJJ_zepp = (vars.WJJ.eta() - avgEta) / Deta;
    
    vars.lepNuW_zepp = (vars.lepNuW.eta() - avgEta) / Deta;
  }
  
  
  
  if( (vars.selectIt_tag.at(0) != -1)  && (vars.selectIt_tag.at(1) == -1) )
  {
    vars.tagJ1 = vars.jets.at(vars.selectIt_tag.at(0));
    vars.p_tagJ1 = &vars.tagJ1;
    
    vars.tagJ1_bTag = vars.jets_bTag.at(vars.selectIt_tag.at(0));
  }  
  
  if( (vars.selectIt_tag.at(0) == -1)  && (vars.selectIt_tag.at(1) != -1) )
  {
    vars.tagJ1 = vars.jets.at(vars.selectIt_tag.at(1));
    vars.p_tagJ1 = &vars.tagJ1;
    
    vars.tagJ1_bTag = vars.jets_bTag.at(vars.selectIt_tag.at(1));
  }  
  
}



void SetThirdJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetThirdJetVariables" << std::endl;
  
  float jetPtMAX = -9999.;
  for(int jetIt = 0; jetIt < (int)vars.jets.size(); ++jetIt)
  {
    if( ( (vars.jets.at(jetIt)).pt() > jetPtMAX ) &&
        ( jetIt != vars.selectIt_W.at(0) ) &&
        ( jetIt != vars.selectIt_W.at(1) ) &&
        ( jetIt != vars.selectIt_tag.at(0) ) &&
        ( jetIt != vars.selectIt_tag.at(1) ) )
    {
      vars.selectIt_thirdJet = jetIt;
      jetPtMAX = (vars.jets.at(jetIt)).pt();
    }
  }
  
  
  if( vars.selectIt_thirdJet == -1 ) return;
  
  vars.thirdJ = vars.jets.at(vars.selectIt_thirdJet);
  vars.p_thirdJ = &vars.thirdJ;
  
  vars.thirdJ_bTag = vars.jets_bTag.at(vars.selectIt_thirdJet);
  
  if( (vars.selectIt_W.at(0) != -1) && (vars.selectIt_W.at(1) != -1) )
  {
    float WJ1Deta = deltaEta(vars.thirdJ.eta(),vars.WJ1.eta());
    float WJ2Deta = deltaEta(vars.thirdJ.eta(),vars.WJ2.eta());
    float WJ1Dphi = deltaPhi(vars.thirdJ.phi(),vars.WJ1.phi());
    float WJ2Dphi = deltaPhi(vars.thirdJ.phi(),vars.WJ2.phi());
    float WJ1DR = deltaR(vars.thirdJ.eta(),vars.thirdJ.phi(),vars.WJ1.eta(),vars.WJ1.phi());
    float WJ2DR = deltaR(vars.thirdJ.eta(),vars.thirdJ.phi(),vars.WJ2.eta(),vars.WJ2.phi());
    vars.thirdJ_WJJDeta = std::min(WJ1Deta,WJ2Deta);
    vars.thirdJ_WJJDphi = std::min(WJ1Dphi,WJ2Dphi);
    vars.thirdJ_WJJDR = std::min(WJ1DR,WJ2DR);
  }
  
  if( (vars.selectIt_tag.at(0) != -1) && (vars.selectIt_tag.at(1) != -1) )
  {
    float tagJ1Deta = deltaEta(vars.thirdJ.eta(),vars.tagJ1.eta());
    float tagJ2Deta = deltaEta(vars.thirdJ.eta(),vars.tagJ2.eta());
    float tagJ1Dphi = deltaPhi(vars.thirdJ.phi(),vars.tagJ1.phi());
    float tagJ2Dphi = deltaPhi(vars.thirdJ.phi(),vars.tagJ2.phi());
    float tagJ1DR = deltaR(vars.thirdJ.eta(),vars.thirdJ.phi(),vars.tagJ1.eta(),vars.tagJ1.phi());
    float tagJ2DR = deltaR(vars.thirdJ.eta(),vars.thirdJ.phi(),vars.tagJ2.eta(),vars.tagJ2.phi());
    vars.thirdJ_tagJJDeta = std::min(tagJ1Deta,tagJ2Deta);
    vars.thirdJ_tagJJDphi = std::min(tagJ1Dphi,tagJ2Dphi);
    vars.thirdJ_tagJJDR = std::min(tagJ1DR,tagJ2DR);
  }
  
}



void SetMCVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity)
{
  if( verbosity == 1 ) std::cout << ">>>>>>>>> VBFPreselectionVariables::SetMCVariables" << std::endl;
  
  // lepton
  vars.mc_lep = reader.Get4V("mcF1_fromV1")->at(0);
  vars.p_mc_lep = &vars.mc_lep;
  
  vars.mc_lep_charge = reader.GetFloat("mcF1_fromV1_charge")->at(0);
  vars.mc_lep_flavour = int(reader.GetFloat("mcF1_fromV1_pdgId")->at(0));
  vars.mc_lep_pt  = vars.mc_lep.pt();
  vars.mc_lep_eta = vars.mc_lep.eta();
  vars.mc_lep_phi = vars.mc_lep.phi();
  
  // neutrino
  vars.mc_nu = reader.Get4V("mcF2_fromV1")->at(0);
  vars.p_mc_nu = &vars.mc_nu;
  
  vars.mc_nu_charge = reader.GetFloat("mcF2_fromV1_charge")->at(0);
  vars.mc_nu_flavour = int(reader.GetFloat("mcF2_fromV1_pdgId")->at(0));
  vars.mc_nu_pt  = vars.mc_nu.pt();
  vars.mc_nu_eta = vars.mc_nu.eta();
  vars.mc_nu_phi = vars.mc_nu.phi();
  
  if( abs(vars.mc_lep_flavour%2 == 0) )
  {
    vars.mc_lep = reader.Get4V("mcF2_fromV1")->at(0);
    vars.p_mc_lep = &vars.mc_lep;
    
    vars.mc_lep_charge = reader.GetFloat("mcF2_fromV1_charge")->at(0);
    vars.mc_lep_flavour = int(reader.GetFloat("mcF2_fromV1_pdgId")->at(0));
    vars.mc_lep_pt  = vars.mc_lep.pt();
    vars.mc_lep_eta = vars.mc_lep.eta();
    vars.mc_lep_phi = vars.mc_lep.phi();
    
    
    vars.mc_nu = reader.Get4V("mcF1_fromV1")->at(0);
    vars.p_mc_nu = &vars.mc_nu;
    
    vars.mc_nu_charge = reader.GetFloat("mcF1_fromV1_charge")->at(0);
    vars.mc_nu_flavour = int(reader.GetFloat("mcF1_fromV1_pdgId")->at(0));
    vars.mc_nu_pt  = vars.mc_nu.pt();
    vars.mc_nu_eta = vars.mc_nu.eta();
    vars.mc_nu_phi = vars.mc_nu.phi();  
  }
  
  vars.mc_lepNu_pt = (vars.mc_lep+vars.mc_nu).pt();
  vars.mc_lepNu_mt = sqrt( vars.mc_lep.mass() * vars.mc_lep.mass() + 2. * vars.mc_lep.pt() * vars.mc_nu.pt() * ( 1 - cos(deltaPhi(vars.mc_lep.phi(), vars.mc_nu.phi()) ) ) );
  vars.mc_lepNu_Dphi = deltaPhi(vars.mc_lep_phi,vars.mc_nu_phi);
  
  // WJets
  vars.mc_WJ1 = reader.Get4V("mcF1_fromV2")->at(0);
  vars.p_mc_WJ1 = &vars.mc_WJ1;
  vars.mc_WJ2 = reader.Get4V("mcF2_fromV2")->at(0);
  vars.p_mc_WJ2 = &vars.mc_WJ2;
  
  vars.mc_WJ1_pt  = vars.mc_WJ1.pt();
  vars.mc_WJ2_pt  = vars.mc_WJ2.pt();
  vars.mc_WJ1_eta = vars.mc_WJ1.eta();
  vars.mc_WJ2_eta = vars.mc_WJ2.eta();
  vars.mc_WJ1_phi = vars.mc_WJ1.phi();
  vars.mc_WJ2_phi = vars.mc_WJ2.phi();
  
  
  float DRWJ11 = deltaR(vars.WJ1_eta,vars.WJ1_phi,vars.mc_WJ1_eta,vars.mc_WJ1_phi);
  float DRWJ22 = deltaR(vars.WJ2_eta,vars.WJ2_phi,vars.mc_WJ2_eta,vars.mc_WJ2_phi);
  float DRWJ12 = deltaR(vars.WJ1_eta,vars.WJ1_phi,vars.mc_WJ2_eta,vars.mc_WJ2_phi);
  float DRWJ21 = deltaR(vars.WJ2_eta,vars.WJ2_phi,vars.mc_WJ1_eta,vars.mc_WJ1_phi);
  
  if( (DRWJ11+DRWJ22) > (DRWJ12+DRWJ21) )
  {
    vars.mc_WJ1 = reader.Get4V("mcF2_fromV2")->at(0);
    vars.p_mc_WJ1 = &vars.mc_WJ1;
    vars.mc_WJ2 = reader.Get4V("mcF1_fromV2")->at(0);
    vars.p_mc_WJ2 = &vars.mc_WJ2;
    
    vars.mc_WJ1_pt  = vars.mc_WJ1.pt();
    vars.mc_WJ2_pt  = vars.mc_WJ2.pt();
    vars.mc_WJ1_eta = vars.mc_WJ1.eta();
    vars.mc_WJ2_eta = vars.mc_WJ2.eta();
    vars.mc_WJ1_phi = vars.mc_WJ1.phi();
    vars.mc_WJ2_phi = vars.mc_WJ2.phi();  
  }
  
  
  vars.mc_WJJ_Deta = deltaEta(vars.mc_WJ1_eta,vars.mc_WJ2_eta);
  vars.mc_WJJ_Dphi = deltaPhi(vars.mc_WJ1_phi,vars.mc_WJ2_phi);
  vars.mc_WJJ_DR = deltaR(vars.mc_WJ1_eta,vars.mc_WJ1_phi,vars.mc_WJ2_eta,vars.mc_WJ2_phi);
  vars.mc_WJJ_pt = (vars.mc_WJ1+vars.mc_WJ2).pt();
  vars.mc_WJJ_m = (vars.mc_WJ1+vars.mc_WJ2).mass();
  
  // W
  vars.mc_W1 = reader.Get4V("mcV1")->at(0);
  vars.p_mc_W1 = &vars.mc_W1;
  vars.mc_W2 = reader.Get4V("mcV2")->at(0);
  vars.p_mc_W2 = &vars.mc_W2;
  
  vars.mc_W1_pt  = vars.mc_W1.pt();
  vars.mc_W2_pt  = vars.mc_W2.pt();
  vars.mc_W1_eta = vars.mc_W1.eta();
  vars.mc_W2_eta = vars.mc_W2.eta();
  vars.mc_W1_phi = vars.mc_W1.phi();
  vars.mc_W2_phi = vars.mc_W2.phi();
  vars.mc_W1_m = vars.mc_W1.mass();
  vars.mc_W2_m = vars.mc_W2.mass();
  
  // Higgs
  vars.mc_H = reader.Get4V("mc_H")->at(0);
  vars.p_mc_H = &vars.mc_H;
  
  vars.mc_H_pt  = vars.mc_H.pt();
  vars.mc_H_eta = vars.mc_H.eta();
  vars.mc_H_phi = vars.mc_H.phi();
  vars.mc_H_m = vars.mc_H.mass();
  
  
  // get matchings
  vars.lep_mcMatchDR = deltaR(vars.lep_eta,vars.lep_phi,vars.mc_lep_eta,vars.mc_lep_phi);
  vars.met_mcMatchDR = deltaPhi(vars.met_phi,vars.mc_nu_phi);
  vars.WJ1_mcMatchDR = deltaR(vars.WJ1_eta,vars.WJ1_phi,vars.mc_WJ1_eta,vars.mc_WJ1_phi);
  vars.WJ2_mcMatchDR = deltaR(vars.WJ2_eta,vars.WJ2_phi,vars.mc_WJ2_eta,vars.mc_WJ2_phi);
  
  if( vars.lep_mcMatchDR < 0.1 ) vars.lep_mcMatched = 1; else vars.lep_mcMatched = 0;
  if( vars.met_mcMatchDR < 0.5 ) vars.met_mcMatched = 1; else vars.met_mcMatched = 0;
  if( vars.WJ1_mcMatchDR < 0.5 ) vars.WJ1_mcMatched = 1; else vars.WJ1_mcMatched = 0;
  if( vars.WJ2_mcMatchDR < 0.5 ) vars.WJ2_mcMatched = 1; else vars.WJ2_mcMatched = 0;
  
}
