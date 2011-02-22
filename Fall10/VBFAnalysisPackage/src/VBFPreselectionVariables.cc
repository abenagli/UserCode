#include "VBFPreselectionVariables.h"



void InitializeVBFPreselectionTree(VBFPreselectionVariables& vars, const std::string& outputRootFileName)
{
  //-------------
  // Reduced tree
  //-------------
  
  vars.m_outputRootFile = new TFile(outputRootFileName.c_str(), "RECREATE");  
  
  vars.m_reducedTree = new TTree("ntu", "ntu");
  vars.m_reducedTree -> SetDirectory(vars.m_outputRootFile);
  
  vars.m_reducedTree -> Branch("mH",           &vars.mH,                     "mH/F");
  vars.m_reducedTree -> Branch("dataFlag",     &vars.dataFlag,         "dataFlag/I");
  vars.m_reducedTree -> Branch("crossSection", &vars.crossSection, "crossSection/F");
  vars.m_reducedTree -> Branch("TMVA4Jet",     &vars.TMVA4Jet,         "TMVA4Jet/I");
  vars.m_reducedTree -> Branch("runId",        &vars.runId,               "runId/I");
  vars.m_reducedTree -> Branch("lumiId",       &vars.lumiId,             "lumiId/I");
  vars.m_reducedTree -> Branch("eventId",      &vars.eventId,           "eventId/I");
  
  
  // PV variables
  vars.m_reducedTree -> Branch("PV_d0",             &vars.PV_d0,                         "PV_d0/F");
  vars.m_reducedTree -> Branch("PV_z",              &vars.PV_z,                           "PV_z/F");
  vars.m_reducedTree -> Branch("PV_nTracks",        &vars.PV_nTracks,               "PV_nTracks/I");
  vars.m_reducedTree -> Branch("PV_ndof",           &vars.PV_ndof,                     "PV_ndof/I");
  vars.m_reducedTree -> Branch("PV_normalizedChi2", &vars.PV_normalizedChi2, "PV_normalizedChi2/F");
  
  
  // lepton variables
  vars.m_reducedTree -> Branch("lep", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_lep);
  vars.m_reducedTree -> Branch("lep_charge",  &vars.lep_charge,   "lep_charge/F");
  vars.m_reducedTree -> Branch("lep_flavour", &vars.lep_flavour, "lep_flavour/I");
  vars.m_reducedTree -> Branch("lep_zepp",    &vars.lep_zepp,       "lep_zepp/F");
  vars.m_reducedTree -> Branch("lep_dxy",     &vars.lep_dxy,         "lep_dxy/F");
  vars.m_reducedTree -> Branch("lep_dz",      &vars.lep_dz,           "lep_dz/F");
  vars.m_reducedTree -> Branch("lep_dB",      &vars.lep_dB,           "lep_dB/F");
  vars.m_reducedTree -> Branch("lep_edB",     &vars.lep_edB,         "lep_edB/F");
  vars.m_reducedTree -> Branch("lep_tkIso",   &vars.lep_tkIso,     "lep_tkIso/F");
  vars.m_reducedTree -> Branch("lep_emIso",   &vars.lep_emIso,     "lep_emIso/F");
  vars.m_reducedTree -> Branch("lep_hadIso",  &vars.lep_hadIso,   "lep_hadIso/F");
  vars.m_reducedTree -> Branch("lep_isEB",          &vars.lep_isEB,                   "lep_isEB/I");
  vars.m_reducedTree -> Branch("lep_sigmaIetaIeta", &vars.lep_sigmaIetaIeta, "lep_sigmaIetaIeta/F");
  vars.m_reducedTree -> Branch("lep_DphiIn",        &vars.lep_DphiIn,               "lep_DphiIn/F");
  vars.m_reducedTree -> Branch("lep_DetaIn",        &vars.lep_DetaIn,               "lep_DetaIn/F");
  vars.m_reducedTree -> Branch("lep_HOverE",        &vars.lep_HOverE,               "lep_HOverE/F");
  vars.m_reducedTree -> Branch("lep_fbrem",         &vars.lep_fbrem,                 "lep_fbrem/F");
  vars.m_reducedTree -> Branch("lep_EOverP",        &vars.lep_EOverP,               "lep_EOverP/F");
  vars.m_reducedTree -> Branch("lep_tracker",                  &vars.lep_tracker,                                   "lep_tracker/I");
  vars.m_reducedTree -> Branch("lep_standalone",               &vars.lep_standalone,                             "lep_standalone/I");
  vars.m_reducedTree -> Branch("lep_global",                   &vars.lep_global,                                     "lep_global/I");
  vars.m_reducedTree -> Branch("lep_normalizedChi2",           &vars.lep_normalizedChi2,                     "lep_normalizedChi2/F");
  vars.m_reducedTree -> Branch("lep_numberOfValidTrackerHits", &vars.lep_numberOfValidTrackerHits, "lep_numberOfValidTrackerHits/I");
  vars.m_reducedTree -> Branch("lep_numberOfValidMuonHits",    &vars.lep_numberOfValidMuonHits,       "lep_numberOfValidMuonHits/I");
  
  
  // met variables
  vars.m_reducedTree -> Branch("met", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_met);
  vars.m_reducedTree -> Branch("lepMet_mt",    &vars.lepMet_mt,       "lepMet_mt/F");
  vars.m_reducedTree -> Branch("lepMet_Dphi",  &vars.lepMet_Dphi,   "lepMet_Dphi/F");
  
  
  // jet variables
  vars.m_reducedTree -> Branch("nJets",     &vars.nJets,         "nJets/I");
  vars.m_reducedTree -> Branch("nJets_cnt", &vars.nJets_cnt, "nJets_cnt/I");
  vars.m_reducedTree -> Branch("nJets_fwd", &vars.nJets_fwd, "nJets_fwd/I");
  
  vars.m_reducedTree -> Branch("nJets_et20",     &vars.nJets_et20,         "nJets_et20/I");
  vars.m_reducedTree -> Branch("nJets_cnt_et20", &vars.nJets_cnt_et20, "nJets_cnt_et20/I");
  vars.m_reducedTree -> Branch("nJets_fwd_et20", &vars.nJets_fwd_et20, "nJets_fwd_et20/I");
  vars.m_reducedTree -> Branch("nJets_out_et20", &vars.nJets_out_et20, "nJets_out_et20/I");
  vars.m_reducedTree -> Branch("nJets_mid_et20", &vars.nJets_mid_et20, "nJets_mid_et20/I");
  vars.m_reducedTree -> Branch("nJets_btw_et20", &vars.nJets_btw_et20, "nJets_btw_et20/I");
  vars.m_reducedTree -> Branch("nJets_et25",     &vars.nJets_et25,         "nJets_et25/I");
  vars.m_reducedTree -> Branch("nJets_cnt_et25", &vars.nJets_cnt_et25, "nJets_cnt_et25/I");
  vars.m_reducedTree -> Branch("nJets_fwd_et25", &vars.nJets_fwd_et25, "nJets_fwd_et25/I");
  vars.m_reducedTree -> Branch("nJets_out_et25", &vars.nJets_out_et25, "nJets_out_et25/I");
  vars.m_reducedTree -> Branch("nJets_mid_et25", &vars.nJets_mid_et25, "nJets_mid_et25/I");
  vars.m_reducedTree -> Branch("nJets_btw_et25", &vars.nJets_btw_et25, "nJets_btw_et25/I");
  vars.m_reducedTree -> Branch("nJets_et30",     &vars.nJets_et30,         "nJets_et30/I");
  vars.m_reducedTree -> Branch("nJets_cnt_et30", &vars.nJets_cnt_et30, "nJets_cnt_et30/I");
  vars.m_reducedTree -> Branch("nJets_fwd_et30", &vars.nJets_fwd_et30, "nJets_fwd_et30/I");
  vars.m_reducedTree -> Branch("nJets_out_et30", &vars.nJets_out_et30, "nJets_out_et30/I");
  vars.m_reducedTree -> Branch("nJets_mid_et30", &vars.nJets_mid_et30, "nJets_mid_et30/I");
  vars.m_reducedTree -> Branch("nJets_btw_et30", &vars.nJets_btw_et30, "nJets_btw_et30/I");
  
  vars.m_reducedTree -> Branch("jets_bTag1", &vars.jets_bTag1, "jets_bTag1/F"); 
  vars.m_reducedTree -> Branch("jets_bTag2", &vars.jets_bTag2, "jets_bTag2/F"); 
  
  
  // leading jet variables
  vars.m_reducedTree -> Branch("leadingJ", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_leadingJ);
  vars.m_reducedTree -> Branch("leadingJ_charge",              &vars.leadingJ_charge,                           "leadingJ_charge/F");
  vars.m_reducedTree -> Branch("leadingJ_bTag",                &vars.leadingJ_bTag,                               "leadingJ_bTag/F");
  vars.m_reducedTree -> Branch("leadingJ_chargedMultiplicity", &vars.leadingJ_chargedMultiplicity, "leadingJ_chargedMultiplicity/I");
  
  
  // W-jet variables
  vars.m_reducedTree -> Branch("WJ1",  "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ1);
  vars.m_reducedTree -> Branch("WJ2",  "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ2);
  vars.m_reducedTree -> Branch("WJ12", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_WJ12);
  vars.m_reducedTree -> Branch("WJ1_charge",              &vars.WJ1_charge,                           "WJ1_charge/F");
  vars.m_reducedTree -> Branch("WJ2_charge",              &vars.WJ2_charge,                           "WJ2_charge/F");
  vars.m_reducedTree -> Branch("WJ1_zepp",                &vars.WJ1_zepp,                               "WJ1_zepp/F");
  vars.m_reducedTree -> Branch("WJ2_zepp",                &vars.WJ2_zepp,                               "WJ2_zepp/F");
  vars.m_reducedTree -> Branch("WJ1_bTag",                &vars.WJ1_bTag,                               "WJ1_bTag/F");
  vars.m_reducedTree -> Branch("WJ2_bTag",                &vars.WJ2_bTag,                               "WJ2_bTag/F");
  vars.m_reducedTree -> Branch("WJ1_chargedMultiplicity", &vars.WJ1_chargedMultiplicity, "WJ1_chargedMultiplicity/I");
  vars.m_reducedTree -> Branch("WJ2_chargedMultiplicity", &vars.WJ2_chargedMultiplicity, "WJ2_chargedMultiplicity/I");
  vars.m_reducedTree -> Branch("WJJ_Deta",   &vars.WJJ_Deta,     "WJJ_Deta/F");
  vars.m_reducedTree -> Branch("WJJ_Dphi",   &vars.WJJ_Dphi,     "WJJ_Dphi/F");
  vars.m_reducedTree -> Branch("WJJ_DR",     &vars.WJJ_DR,         "WJJ_DR/F");
  vars.m_reducedTree -> Branch("WJJ_m",      &vars.WJJ_m,           "WJJ_m/F");
  vars.m_reducedTree -> Branch("WJJ_zepp",   &vars.WJJ_zepp,     "WJJ_zepp/F");

  vars.m_reducedTree -> Branch("lepWJJ_pt1", &vars.lepWJJ_pt1, "lepWJJ_pt1/F");
  vars.m_reducedTree -> Branch("lepWJJ_pt2", &vars.lepWJJ_pt2, "lepWJJ_pt2/F");
  vars.m_reducedTree -> Branch("lepWJJ_pt3", &vars.lepWJJ_pt3, "lepWJJ_pt3/F");
  
  
  // Higgs variables
  vars.m_reducedTree -> Branch("lepMetW_mt",   &vars.lepMetW_mt,     "lepMetW_mt/F");
  vars.m_reducedTree -> Branch("lepMetW_Dphi", &vars.lepMetW_Dphi, "lepMetW_Dphi/F");
  vars.m_reducedTree -> Branch("lepNuW_m",     &vars.lepNuW_m,         "lepNuW_m/F");
  vars.m_reducedTree -> Branch("lepNuW_zepp",  &vars.lepNuW_zepp,   "lepNuW_zepp/F");
  
  
  // tag-jet variables
  vars.m_reducedTree -> Branch("tagJ1", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_tagJ1);
  vars.m_reducedTree -> Branch("tagJ2", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_tagJ2);
  vars.m_reducedTree -> Branch("tagJ1_charge", &vars.tagJ1_charge, "tagJ1_charge/F");
  vars.m_reducedTree -> Branch("tagJ2_charge", &vars.tagJ2_charge, "tagJ2_charge/F");
  vars.m_reducedTree -> Branch("tagJ1_bTag",   &vars.tagJ1_bTag,     "tagJ1_bTag/F");
  vars.m_reducedTree -> Branch("tagJ2_bTag",   &vars.tagJ2_bTag,     "tagJ2_bTag/F");
  vars.m_reducedTree -> Branch("tagJJ_Deta",   &vars.tagJJ_Deta,     "tagJJ_Deta/F");
  vars.m_reducedTree -> Branch("tagJJ_Dphi",   &vars.tagJJ_Dphi,     "tagJJ_Dphi/F");
  vars.m_reducedTree -> Branch("tagJJ_DR",     &vars.tagJJ_DR,         "tagJJ_DR/F");
  vars.m_reducedTree -> Branch("tagJJ_m",      &vars.tagJJ_m,           "tagJJ_m/F");
  
  
  // third jet variables
  vars.m_reducedTree -> Branch("thirdJ", "ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >", &vars.p_thirdJ);
  vars.m_reducedTree -> Branch("thirdJ_charge", &vars.thirdJ_charge, "thirdJ_charge/F");
  vars.m_reducedTree -> Branch("thirdJ_bTag",   &vars.thirdJ_bTag,     "thirdJ_bTag/F");
}



void FillVBFPreselectionTree(VBFPreselectionVariables& vars)
{
  vars.m_reducedTree -> Fill();
}






void ClearVBFPreselectionVariables(VBFPreselectionVariables& vars)
{
  // PV variables
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
  vars.leptons_dxy.clear();
  vars.leptons_dz.clear();
  vars.leptons_dB.clear();
  vars.leptons_edB.clear();
  vars.leptons_tkIso.clear();
  vars.leptons_emIso.clear();
  vars.leptons_hadIso.clear();
  vars.electrons_isEB.clear();
  vars.electrons_sigmaIetaIeta.clear();
  vars.electrons_DphiIn.clear();
  vars.electrons_DetaIn.clear();
  vars.electrons_HOverE.clear();
  vars.electrons_fbrem.clear();
  vars.electrons_EOverP.clear();
  vars.muons_tracker.clear();
  vars.muons_standalone.clear();
  vars.muons_global.clear();
  vars.muons_normalizedChi2.clear();
  vars.muons_numberOfValidTrackerHits.clear();
  vars.muons_numberOfValidMuonHits.clear();
  
  vars.selectIt_lep = -1;
  vars.selectIt_ele = -1;
  vars.selectIt_mu = -1;

  vars.lep = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_lep = NULL;
  
  vars.lep_charge = -1.;
  vars.lep_flavour = -1;
  vars.lep_zepp = -99.;
  vars.lep_dxy = -1.;
  vars.lep_dz = -99.;
  vars.lep_dB = -1.;
  vars.lep_edB = -1.;
  vars.lep_tkIso = -1.;
  vars.lep_emIso = -1.;
  vars.lep_hadIso = -1.;
  vars.lep_isEB = -1;
  vars.lep_sigmaIetaIeta = -1.;
  vars.lep_DphiIn = -99.;
  vars.lep_DetaIn = -99.;
  vars.lep_HOverE = -1.;
  vars.lep_fbrem = -1.;
  vars.lep_EOverP = -1.;
  vars.lep_tracker = -1;
  vars.lep_standalone = -1;
  vars.lep_global = -1;
  vars.lep_normalizedChi2 = -1.;
  vars.lep_numberOfValidTrackerHits = -1;
  vars.lep_numberOfValidMuonHits = -1;
  
  
  
  // met variables 
  vars.met = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_met = NULL;
  
  vars.lepMet = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.neutrino = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  
  vars.lepMet_mt = -1.;
  vars.lepMet_Dphi = -1.;
  
  
  
  // jet variables
  vars.jets.clear();
  vars.jets_charge.clear();
  vars.jets_etaEtaMoment.clear();
  vars.jets_phiPhiMoment.clear();
  vars.jets_etaPhiMoment.clear();
  vars.jets_emEnergyFraction.clear();
  vars.jets_fHPD.clear();
  vars.jets_fRBX.clear();
  vars.jets_n90Hits.clear();
  vars.jets_chargedHadronEnergyFraction.clear();
  vars.jets_neutralHadronEnergyFraction.clear();
  vars.jets_chargedEmEnergyFraction.clear();
  vars.jets_neutralEmEnergyFraction.clear();
  vars.jets_muonEnergyFraction.clear();
  vars.jets_chargedMultiplicity.clear();
  vars.jets_neutralMultiplicity.clear();
  vars.jets_muonMultiplicity.clear();  
  vars.jets_etOrdered.clear();
  vars.jets_bTag.clear();
  vars.jets_bTagOrdered.clear();
  
  vars.nJets = 0;
  vars.nJets_cnt = 0;
  vars.nJets_fwd = 0;
  
  vars.nJets_et20 = 0;
  vars.nJets_cnt_et20 = 0;
  vars.nJets_fwd_et20 = 0;
  vars.nJets_out_et20 = 0;
  vars.nJets_mid_et20 = 0;
  vars.nJets_btw_et20 = 0;
  vars.nJets_et25 = 0;
  vars.nJets_cnt_et25 = 0;
  vars.nJets_fwd_et25 = 0;
  vars.nJets_out_et25 = 0;
  vars.nJets_mid_et25 = 0;
  vars.nJets_btw_et25 = 0;
  vars.nJets_et30 = 0;
  vars.nJets_cnt_et30 = 0;
  vars.nJets_fwd_et30 = 0;
  vars.nJets_out_et30 = 0;
  vars.nJets_mid_et30 = 0;
  vars.nJets_btw_et30 = 0;
  
  vars.jets_bTag1 = -99.;
  vars.jets_bTag2 = -99.;
  
  vars.selectIt_leadingJet = -1;
  
  vars.leadingJ = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_leadingJ = NULL;
  
  vars.leadingJ_charge = -99.;
  vars.leadingJ_bTag = -99.;
  vars.leadingJ_chargedMultiplicity = -1;
  
  
  
  // W-jet variables
  vars.selectIt_W.clear();
  
  vars.WJ1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_WJ1 = NULL;
  vars.WJ2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_WJ2 = NULL;
  
  vars.WJ12 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_WJ12 = NULL;
  
  vars.WJ1_charge = -99.;
  vars.WJ2_charge = -99.;
  vars.WJ1_zepp = -99.;
  vars.WJ2_zepp = -99.;
  vars.WJ1_bTag = -99.;
  vars.WJ2_bTag = -99.;
  vars.WJ1_chargedMultiplicity = -1;
  vars.WJ2_chargedMultiplicity = -1;
  
  vars.WJJ_Deta = -99.;
  vars.WJJ_Dphi = -99.;
  vars.WJJ_DR = -99.;
  vars.WJJ_m = -99.;
  vars.WJJ_zepp = -99.;
  
  vars.lepWJJ_ptOrdered.clear();  
  vars.lepWJJ_pt1 = -1.;
  vars.lepWJJ_pt2 = -1.;
  vars.lepWJJ_pt3 = -1.;
  
  
  
  // Higgs variables
  vars.lepW = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.lepNuW = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  
  vars.lepMetW_mt = -1.;
  vars.lepMetW_Dphi = -1.;
  vars.lepNuW_m = -1.;
  vars.lepNuW_zepp = -99.;
  
  
  
  // tag-jet variables
  vars.selectIt_tag.clear();
  
  vars.tagJ1 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_tagJ1 = NULL;
  vars.tagJ2 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_tagJ2 = NULL;
  
  vars.tagJ12 = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  
  vars.tagJ1_charge = -99.;
  vars.tagJ2_charge = -99.;
  vars.tagJ1_bTag = -99.;
  vars.tagJ2_bTag = -99.;
  
  vars.tagJJ_Deta = -99.;
  vars.tagJJ_Dphi = -99.;
  vars.tagJJ_DR = -99.;
  vars.tagJJ_m = -99.;
  
  
  // third jet variables
  vars.selectIt_thirdJet = -1;
  
  vars.thirdJ = ROOT::Math::XYZTVector(0., 0., 0., 0.);
  vars.p_thirdJ = NULL;
  
  vars.thirdJ_charge = -99.;
  vars.thirdJ_bTag = -99.;
}



void DeleteVBFPreselectionVariables(VBFPreselectionVariables& vars)
{
  // save tree
  vars.m_outputRootFile -> cd();
  vars.m_reducedTree -> Write();
  vars.m_outputRootFile -> Close();
}






void SetPVVariables(VBFPreselectionVariables& vars, treeReader& reader)
{
  vars.PV_d0 = reader.GetFloat("PV_d0")->at(0);
  vars.PV_nTracks = reader.GetInt("PV_nTracks")->at(0);
  vars.PV_ndof = reader.GetInt("PV_ndof")->at(0);
  vars.PV_normalizedChi2 = reader.GetFloat("PV_normalizedChi2")->at(0);
  vars.PV_z = reader.GetFloat("PV_z")->at(0);
}



void SetLeptonVariables(VBFPreselectionVariables& vars, treeReader& reader)
{
  vars.lep = vars.leptons.at(vars.selectIt_lep);
  vars.p_lep = &vars.lep;
  
  vars.lep_charge  = vars.leptonCharges.at(vars.selectIt_lep);
  vars.lep_dxy     = vars.leptons_dxy.at(vars.selectIt_lep);
  vars.lep_dz      = vars.leptons_dz.at(vars.selectIt_lep);
  vars.lep_dB      = vars.leptons_dB.at(vars.selectIt_lep);
  vars.lep_edB     = vars.leptons_edB.at(vars.selectIt_lep);
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
    vars.lep_sigmaIetaIeta = vars.electrons_sigmaIetaIeta.at(vars.selectIt_ele);
    vars.lep_DphiIn = vars.electrons_DphiIn.at(vars.selectIt_ele);
    vars.lep_DetaIn = vars.electrons_DetaIn.at(vars.selectIt_ele);
    vars.lep_HOverE = vars.electrons_HOverE.at(vars.selectIt_ele);
    vars.lep_fbrem  = vars.electrons_fbrem.at(vars.selectIt_ele);
    vars.lep_EOverP = vars.electrons_EOverP.at(vars.selectIt_ele);
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
    vars.lep_numberOfValidTrackerHits = vars.muons_numberOfValidTrackerHits.at(vars.selectIt_mu);
    vars.lep_numberOfValidMuonHits = vars.muons_numberOfValidMuonHits.at(vars.selectIt_mu);
  }
  
}



void SetElectronVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& eleIt)
{
  vars.electrons.push_back( reader.Get4V("electrons")->at(eleIt) );
  vars.leptons.push_back( reader.Get4V("electrons")->at(eleIt) );
  vars.leptonCharges.push_back( reader.GetFloat("electrons_charge")->at(eleIt) );
  vars.leptonFlavours.push_back("electron");
  
  vars.leptons_tkIso.push_back(reader.GetFloat("electrons_tkIsoR03")->at(eleIt));
  vars.leptons_emIso.push_back(reader.GetFloat("electrons_emIsoR03")->at(eleIt));
  vars.leptons_hadIso.push_back(reader.GetFloat("electrons_hadIsoR03_depth1")->at(eleIt)+
			        reader.GetFloat("electrons_hadIsoR03_depth2")->at(eleIt));
  vars.leptons_dxy.push_back(reader.GetFloat("electrons_dxy_PV")->at(eleIt));
  vars.leptons_dz.push_back(reader.GetFloat("electrons_dz_PV")->at(eleIt));
  vars.leptons_dB.push_back(reader.GetFloat("electrons_dB")->at(eleIt));
  vars.leptons_edB.push_back(reader.GetFloat("electrons_edB")->at(eleIt));
  
  vars.electrons_isEB.push_back(reader.GetInt("electrons_isEB")->at(eleIt));
  vars.electrons_sigmaIetaIeta.push_back(reader.GetFloat("electrons_sigmaIetaIeta")->at(eleIt));
  vars.electrons_DphiIn.push_back(reader.GetFloat("electrons_deltaPhiIn")->at(eleIt));
  vars.electrons_DetaIn.push_back(reader.GetFloat("electrons_deltaEtaIn")->at(eleIt));
  vars.electrons_HOverE.push_back(reader.GetFloat("electrons_hOverE")->at(eleIt));
  vars.electrons_fbrem.push_back(reader.GetFloat("electrons_fbrem")->at(eleIt));
  vars.electrons_EOverP.push_back(reader.GetFloat("electrons_eSCOverP")->at(eleIt));
}



void SetMuonVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& muIt)
{
  vars.muons.push_back( reader.Get4V("muons")->at(muIt) );
  vars.leptons.push_back( reader.Get4V("muons")->at(muIt) );
  vars.leptonCharges.push_back( reader.GetFloat("muons_charge")->at(muIt) );
  vars.leptonFlavours.push_back("muon");
  vars.leptons_tkIso.push_back(reader.GetFloat("muons_tkIsoR03")->at(muIt));
  vars.leptons_emIso.push_back(reader.GetFloat("muons_emIsoR03")->at(muIt));
  vars.leptons_hadIso.push_back(reader.GetFloat("muons_hadIsoR03")->at(muIt));
  vars.leptons_dxy.push_back(reader.GetFloat("muons_dxy_PV")->at(muIt));
  vars.leptons_dz.push_back(reader.GetFloat("muons_dz_PV")->at(muIt));
  vars.leptons_dB.push_back(reader.GetFloat("muons_dB")->at(muIt));
  vars.leptons_edB.push_back(reader.GetFloat("muons_edB")->at(muIt));
  
  vars.muons_tracker.push_back(reader.GetInt("muons_tracker")->at(muIt));
  vars.muons_standalone.push_back(reader.GetInt("muons_standalone")->at(muIt));
  vars.muons_global.push_back(reader.GetInt("muons_global")->at(muIt));
  vars.muons_normalizedChi2.push_back(reader.GetFloat("muons_normalizedChi2")->at(muIt));
  vars.muons_numberOfValidTrackerHits.push_back(reader.GetInt("muons_numberOfValidTrackerHits")->at(muIt));
  vars.muons_numberOfValidMuonHits.push_back(reader.GetInt("muons_numberOfValidMuonHits")->at(muIt));
}



void SetMetVariables(VBFPreselectionVariables& vars, treeReader& reader, const std::string& jetType)
{
  // met
  if(jetType == "Calo")
    vars.met = reader.Get4V("type1Met")->at(0);
  if(jetType == "PF")
    vars.met = reader.Get4V("PFMet")->at(0);
  vars.p_met = &vars.met;
  
  vars.lepMet = vars.lep + vars.met;
  
  vars.lepMet_Dphi = deltaPhi(vars.lep.phi(), vars.met.phi());
  vars.lepMet_mt = sqrt( vars.lep.mass() * vars.lep.mass() +
			 2. * vars.lep.pt() * vars.met.pt() * ( 1 - cos(deltaPhi(vars.lep.phi(), vars.met.phi()) ) ) );
}



void SetJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& jetIt, const std::string& jetType, const float& jetEtaCNT, const float& jetEtaFWD)
{
  ROOT::Math::XYZTVector jet = reader.Get4V("jets")->at(jetIt);  
  
  vars.jets.push_back( reader.Get4V("jets")->at(jetIt) );
  
  ++vars.nJets;  
  if( fabs(jet.eta()) < jetEtaCNT )  ++vars.nJets_cnt;  
  if( fabs(jet.eta()) >= jetEtaFWD ) ++vars.nJets_fwd;  
  
  if( jet.Et() > 20.)
  {
    ++vars.nJets_et20;
    if( fabs(jet.eta()) < jetEtaCNT )  ++vars.nJets_cnt_et20;
    if( fabs(jet.eta()) >= jetEtaFWD ) ++vars.nJets_fwd_et20;
  }
  if( jet.Et() > 25.)
  {
    ++vars.nJets_et25;
    if( fabs(jet.eta()) < jetEtaCNT )  ++vars.nJets_cnt_et25;
    if( fabs(jet.eta()) >= jetEtaFWD ) ++vars.nJets_fwd_et25;
  }
  if( jet.Et() > 30.)
  {
    ++vars.nJets_et30;
    if( fabs(jet.eta()) < jetEtaCNT )  ++vars.nJets_cnt_et30;
    if( fabs(jet.eta()) >= jetEtaFWD ) ++vars.nJets_fwd_et30;
  }

  vars.jets_charge.push_back( reader.GetFloat("jets_charge")->at(jetIt) );
  vars.jets_etaEtaMoment.push_back( reader.GetFloat("jets_etaetaMoment")->at(jetIt) );
  vars.jets_phiPhiMoment.push_back( reader.GetFloat("jets_phiphiMoment")->at(jetIt) );
  vars.jets_etaPhiMoment.push_back( reader.GetFloat("jets_etaphiMoment")->at(jetIt) );

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
  
  vars.jets_etOrdered.push_back( reader.Get4V("jets")->at(jetIt).Et() );
  vars.jets_bTag.push_back( reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) );
  vars.jets_bTagOrdered.push_back( reader.GetFloat("trackCountingHighEffBJetTags")->at(jetIt) );
  
}



void SetLeadingJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const float& jetEtaCNT)
{
  float jetEtMAX = -9999.;
  for(unsigned int jetIt = 0; jetIt < vars.jets.size(); ++jetIt)
  {
    if( ( (vars.jets.at(jetIt)).Et() > jetEtMAX ) &&
        ( fabs((vars.jets.at(jetIt)).eta()) < jetEtaCNT ) )
    {
      vars.selectIt_leadingJet = jetIt;
      jetEtMAX = (vars.jets.at(jetIt)).Et();
    }
  }
  
  vars.leadingJ = vars.jets.at(vars.selectIt_leadingJet);
  vars.p_leadingJ = &vars.leadingJ;
  
  vars.leadingJ_charge = vars.jets_charge.at(vars.selectIt_leadingJet);
  vars.leadingJ_bTag = vars.jets_bTag.at(vars.selectIt_leadingJet);
  vars.leadingJ_chargedMultiplicity = vars.jets_chargedMultiplicity.at(vars.selectIt_leadingJet);
}



void SetWJJVariables(VBFPreselectionVariables& vars, treeReader& reader)
{
  if( (vars.selectIt_W.at(0) == -1) || (vars.selectIt_W.at(1) == -1) ) return;
  
  vars.WJ1 = vars.jets.at(vars.selectIt_W.at(0));
  vars.p_WJ1 = &vars.WJ1;
  vars.WJ2 = vars.jets.at(vars.selectIt_W.at(1));
  vars.p_WJ2 = &vars.WJ2;

  vars.WJ12 = vars.WJ1 + vars.WJ2;
  vars.p_WJ12 = &vars.WJ12;
  
  vars.lepW = vars.lep + vars.WJ12;
  
  vars.WJ1_charge = vars.jets_charge.at(vars.selectIt_W.at(0));
  vars.WJ2_charge = vars.jets_charge.at(vars.selectIt_W.at(1));
  vars.WJ1_bTag = vars.jets_bTag.at(vars.selectIt_W.at(0));
  vars.WJ2_bTag = vars.jets_bTag.at(vars.selectIt_W.at(1));
  vars.WJ1_chargedMultiplicity = vars.jets_chargedMultiplicity.at(vars.selectIt_W.at(0));
  vars.WJ2_chargedMultiplicity = vars.jets_chargedMultiplicity.at(vars.selectIt_W.at(1));
  
  vars.WJJ_Deta = deltaEta(vars.WJ1.eta(),vars.WJ2.eta());
  vars.WJJ_Dphi = deltaPhi(vars.WJ1.phi(),vars.WJ2.phi());
  vars.WJJ_DR = deltaR(vars.WJ1.eta(),vars.WJ1.phi(),vars.WJ2.eta(),vars.WJ2.phi());
  vars.WJJ_m = (vars.WJ1+vars.WJ2).mass();
  
  vars.lepWJJ_ptOrdered.push_back( vars.lep.pt() );
  vars.lepWJJ_ptOrdered.push_back( vars.WJ1.Et() );
  vars.lepWJJ_ptOrdered.push_back( vars.WJ2.Et() );
  std::sort(vars.lepWJJ_ptOrdered.begin(), vars.lepWJJ_ptOrdered.end(), maggiore);
  vars.lepWJJ_pt1 = vars.lepWJJ_ptOrdered.at(0);
  vars.lepWJJ_pt2 = vars.lepWJJ_ptOrdered.at(1);
  vars.lepWJJ_pt3 = vars.lepWJJ_ptOrdered.at(2);
}






void SetHVariables(VBFPreselectionVariables& vars, treeReader& reader)
{
  vars.lepMetW_mt = sqrt( vars.lepW.mass()*vars.lepW.mass() + 2. * vars.lepW.pt() * vars.met.pt() * ( 1 - cos(deltaPhi(vars.lepW.phi(), vars.met.phi()) ) ) );
  vars.lepMetW_Dphi = deltaPhi(vars.lepMet.phi(), vars.WJ12.phi());
  
  
  // neutrino
  float alpha = vars.lep.px()*vars.met.px() + vars.lep.py()*vars.met.py();
  
  float delta = (alpha + 0.5*80.399*80.399)*(alpha + 0.5*80.399*80.399) -
    vars.lep.pt()*vars.lep.pt()*vars.met.pt()*vars.met.pt();
  
  if( alpha - vars.lep.pt()*vars.met.pt() + 0.5*80.399*80.399 < 0 )
    delta = 0.;
  
  float pz1 = ( vars.lep.pz()*(alpha + 0.5*80.399*80.399) + vars.lep.energy()*sqrt(delta) ) / vars.lep.pt() / vars.lep.pt();
  float pz2 = ( vars.lep.pz()*(alpha + 0.5*80.399*80.399) - vars.lep.energy()*sqrt(delta) ) / vars.lep.pt() / vars.lep.pt();
  
  ROOT::Math::XYZTVector nu1(vars.met.px(), vars.met.py(), pz1, sqrt(vars.met.px()*vars.met.px() + vars.met.py()*vars.met.py() + pz1*pz1));
  ROOT::Math::XYZTVector nu2(vars.met.px(), vars.met.py(), pz2, sqrt(vars.met.px()*vars.met.px() + vars.met.py()*vars.met.py() + pz2*pz2));
  
  //std::cout<< "delta = " << delta << "   mass = " << (nu1+vars.lepton).mass() << std::endl;
  
  
  
  if( fabs(vars.mH - (vars.lepW + nu1).mass()) < fabs(vars.mH - (vars.lepW + nu2).mass()) )
    vars.neutrino = nu1;
  else
    vars.neutrino = nu2;

  vars.lepNuW = vars.lepW + vars.neutrino;
  vars.lepNuW_m = vars.lepNuW.mass();
  
  //std::cout << "Higgs mt = " << vars.lepMetW_mt << "   Higgs m = " << vars.lepNuW_m << std::endl;
}



void SetTagJJVariables(VBFPreselectionVariables& vars, treeReader& reader)
{
  if( (vars.selectIt_tag.at(0) != -1) && (vars.selectIt_tag.at(1) != -1) )
  {
    vars.tagJ1 = vars.jets.at(vars.selectIt_tag.at(0));
    vars.p_tagJ1 = &vars.tagJ1;
    vars.tagJ2 = vars.jets.at(vars.selectIt_tag.at(1));
    vars.p_tagJ2 = &vars.tagJ2;    

    vars.tagJ12 = vars.tagJ1 + vars.tagJ2;
    
    vars.tagJ1_charge = vars.jets_charge.at(vars.selectIt_tag.at(0));
    vars.tagJ2_charge = vars.jets_charge.at(vars.selectIt_tag.at(1));
    vars.tagJ1_bTag = vars.jets_bTag.at(vars.selectIt_tag.at(0));
    vars.tagJ2_bTag = vars.jets_bTag.at(vars.selectIt_tag.at(1));
    
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
      float WEtaMin = std::min(vars.WJ1.eta(), vars.WJ2.eta());
      float WEtaMax = std::max(vars.WJ1.eta(), vars.WJ2.eta());
      float tagEtaMin = std::min(vars.tagJ1.eta(), vars.tagJ2.eta());
      float tagEtaMax = std::max(vars.tagJ1.eta(), vars.tagJ2.eta());
      
      if( jet.Et() > 20.)
      {
        if( (jet.eta() < tagEtaMin) || (jet.eta() > tagEtaMax) ) ++vars.nJets_out_et20;
        if( (jet.eta() > tagEtaMin) && (jet.eta() < WEtaMin) )   ++vars.nJets_mid_et20;
        if( (jet.eta() < tagEtaMax) && (jet.eta() > WEtaMax) )   ++vars.nJets_mid_et20;
        if( (jet.eta() > WEtaMin)   && (jet.eta() < WEtaMax) )   ++vars.nJets_btw_et20;
      }
      if( jet.Et() > 25.)
      {
        if( (jet.eta() < tagEtaMin) || (jet.eta() > tagEtaMax) ) ++vars.nJets_out_et25;
        if( (jet.eta() > tagEtaMin) && (jet.eta() < WEtaMin) )   ++vars.nJets_mid_et25;
        if( (jet.eta() < tagEtaMax) && (jet.eta() > WEtaMax) )   ++vars.nJets_mid_et25;
        if( (jet.eta() > WEtaMin)   && (jet.eta() < WEtaMax) )   ++vars.nJets_btw_et25;
      }
      if( jet.Et() > 30.)
      {
        if( (jet.eta() < tagEtaMin) || (jet.eta() > tagEtaMax) ) ++vars.nJets_out_et30;
        if( (jet.eta() > tagEtaMin) && (jet.eta() < WEtaMin) )   ++vars.nJets_mid_et30;
        if( (jet.eta() < tagEtaMax) && (jet.eta() > WEtaMax) )   ++vars.nJets_mid_et30;
        if( (jet.eta() > WEtaMin)   && (jet.eta() < WEtaMax) )   ++vars.nJets_btw_et30;
      }
    }
    
    
    
    float avgEta = 0.5 * (vars.tagJ1.eta()+vars.tagJ2.eta());
    float Deta = deltaEta(vars.tagJ1.eta(),vars.tagJ2.eta());
    
    vars.lep_zepp = (vars.lep.eta() - avgEta) / Deta;
    
    vars.WJ1_zepp = (vars.WJ1.eta()  - avgEta) / Deta;
    vars.WJ2_zepp = (vars.WJ2.eta()  - avgEta) / Deta;
    vars.WJJ_zepp = (vars.WJ12.eta() - avgEta) / Deta;
    
    vars.lepNuW_zepp = (vars.lepNuW.eta() - avgEta) / Deta;
  }
  
  
  
  if( (vars.selectIt_tag.at(0) != -1)  && (vars.selectIt_tag.at(1) == -1) )
  {
    vars.tagJ1 = vars.jets.at(vars.selectIt_tag.at(0));
    vars.p_tagJ1 = &vars.tagJ1;
    
    vars.tagJ1_charge = vars.jets_charge.at(vars.selectIt_tag.at(0));
    vars.tagJ1_bTag = vars.jets_bTag.at(vars.selectIt_tag.at(0));
  }  
  
  if( (vars.selectIt_tag.at(0) == -1)  && (vars.selectIt_tag.at(1) != -1) )
  {
    vars.tagJ1 = vars.jets.at(vars.selectIt_tag.at(1));
    vars.p_tagJ1 = &vars.tagJ1;
    
    vars.tagJ1_charge = vars.jets_charge.at(vars.selectIt_tag.at(1));
    vars.tagJ1_bTag = vars.jets_bTag.at(vars.selectIt_tag.at(1));
  }  
  
}



void SetThirdJetVariables(VBFPreselectionVariables& vars, treeReader& reader)
{
  float jetEtMAX = -9999.;
  for(int jetIt = 0; jetIt < (int)vars.jets.size(); ++jetIt)
  {
    if( ( (vars.jets.at(jetIt)).Et() > jetEtMAX ) &&
        ( jetIt != vars.selectIt_W.at(0) ) &&
        ( jetIt != vars.selectIt_W.at(1) ) &&
        ( jetIt != vars.selectIt_tag.at(0) ) &&
        ( jetIt != vars.selectIt_tag.at(1) ) )
    {
      vars.selectIt_thirdJet = jetIt;
      jetEtMAX = (vars.jets.at(jetIt)).Et();
    }
  }
  
  
  if( vars.selectIt_thirdJet == -1 ) return;
  
  vars.thirdJ = vars.jets.at(vars.selectIt_thirdJet);
  vars.p_thirdJ = &vars.thirdJ;
  
  vars.thirdJ_charge = vars.jets_charge.at(vars.selectIt_thirdJet);
  vars.thirdJ_bTag = vars.jets_bTag.at(vars.selectIt_thirdJet);
}
