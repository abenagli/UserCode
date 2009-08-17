#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFNtupleContent.h"



void SetBranchAddresses(TTree* tree, VBFNtupleContent& treeVars)
{
  tree -> SetBranchAddress("eventType", &treeVars.eventType);
  tree -> SetBranchAddress("eventId",   &treeVars.eventId);
  
  
  
  // mc variables
  tree -> SetBranchAddress("mcH_energy", &treeVars.mcH_energy);
  tree -> SetBranchAddress("mcH_px",     &treeVars.mcH_px);
  tree -> SetBranchAddress("mcH_py",     &treeVars.mcH_py);
  tree -> SetBranchAddress("mcH_pz",     &treeVars.mcH_pz);
  tree -> SetBranchAddress("mcH_p",      &treeVars.mcH_p);
  tree -> SetBranchAddress("mcH_pt",     &treeVars.mcH_pt);
  tree -> SetBranchAddress("mcH_mass",   &treeVars.mcH_mass);
  tree -> SetBranchAddress("mcH_phi",    &treeVars.mcH_phi);
  tree -> SetBranchAddress("mcH_eta",    &treeVars.mcH_eta);
  tree -> SetBranchAddress("mcH_charge", &treeVars.mcH_charge);
  
  tree -> SetBranchAddress("mcW_energy", treeVars.mcW_energy);
  tree -> SetBranchAddress("mcW_px",     treeVars.mcW_px);
  tree -> SetBranchAddress("mcW_py",     treeVars.mcW_py);
  tree -> SetBranchAddress("mcW_pz",     treeVars.mcW_pz);
  tree -> SetBranchAddress("mcW_p",      treeVars.mcW_p);
  tree -> SetBranchAddress("mcW_pt",     treeVars.mcW_pt);
  tree -> SetBranchAddress("mcW_mass",   treeVars.mcW_mass);
  tree -> SetBranchAddress("mcW_phi",    treeVars.mcW_phi);
  tree -> SetBranchAddress("mcW_eta",    treeVars.mcW_eta);
  tree -> SetBranchAddress("mcW_charge", treeVars.mcW_charge);
  
  tree -> SetBranchAddress("mcF_fromW1_energy", treeVars.mcF_fromW1_energy);
  tree -> SetBranchAddress("mcF_fromW1_px",     treeVars.mcF_fromW1_px);
  tree -> SetBranchAddress("mcF_fromW1_py",     treeVars.mcF_fromW1_py);
  tree -> SetBranchAddress("mcF_fromW1_pz",     treeVars.mcF_fromW1_pz);
  tree -> SetBranchAddress("mcF_fromW1_p",      treeVars.mcF_fromW1_p);
  tree -> SetBranchAddress("mcF_fromW1_pt",     treeVars.mcF_fromW1_pt);
  tree -> SetBranchAddress("mcF_fromW1_mass",   treeVars.mcF_fromW1_mass);
  tree -> SetBranchAddress("mcF_fromW1_phi",    treeVars.mcF_fromW1_phi);
  tree -> SetBranchAddress("mcF_fromW1_eta",    treeVars.mcF_fromW1_eta);
  tree -> SetBranchAddress("mcF_fromW1_charge", treeVars.mcF_fromW1_charge);
  tree -> SetBranchAddress("mcF_fromW1_pdgId",  treeVars.mcF_fromW1_pdgId);
  
  tree -> SetBranchAddress("mcQ_fromW2_energy", treeVars.mcQ_fromW2_energy);
  tree -> SetBranchAddress("mcQ_fromW2_px",     treeVars.mcQ_fromW2_px);
  tree -> SetBranchAddress("mcQ_fromW2_py",     treeVars.mcQ_fromW2_py);
  tree -> SetBranchAddress("mcQ_fromW2_pz",     treeVars.mcQ_fromW2_pz);
  tree -> SetBranchAddress("mcQ_fromW2_p",      treeVars.mcQ_fromW2_p);
  tree -> SetBranchAddress("mcQ_fromW2_pt",     treeVars.mcQ_fromW2_pt);
  tree -> SetBranchAddress("mcQ_fromW2_mass",   treeVars.mcQ_fromW2_mass);
  tree -> SetBranchAddress("mcQ_fromW2_phi",    treeVars.mcQ_fromW2_phi);
  tree -> SetBranchAddress("mcQ_fromW2_eta",    treeVars.mcQ_fromW2_eta);
  tree -> SetBranchAddress("mcQ_fromW2_charge", treeVars.mcQ_fromW2_charge);
  tree -> SetBranchAddress("mcQ_fromW2_pdgId",  treeVars.mcQ_fromW2_pdgId);
  
  tree -> SetBranchAddress("mcJet_fromW2_energy",     treeVars.mcJet_fromW2_energy);
  tree -> SetBranchAddress("mcJet_fromW2_px",         treeVars.mcJet_fromW2_px);
  tree -> SetBranchAddress("mcJet_fromW2_py",         treeVars.mcJet_fromW2_py);
  tree -> SetBranchAddress("mcJet_fromW2_pz",         treeVars.mcJet_fromW2_pz);
  tree -> SetBranchAddress("mcJet_fromW2_p",          treeVars.mcJet_fromW2_p);
  tree -> SetBranchAddress("mcJet_fromW2_pt",         treeVars.mcJet_fromW2_pt);
  tree -> SetBranchAddress("mcJet_fromW2_mass",       treeVars.mcJet_fromW2_mass);
  tree -> SetBranchAddress("mcJet_fromW2_phi",        treeVars.mcJet_fromW2_phi);
  tree -> SetBranchAddress("mcJet_fromW2_eta",        treeVars.mcJet_fromW2_eta);
  tree -> SetBranchAddress("mcJet_fromW2_charge",     treeVars.mcJet_fromW2_charge);
  tree -> SetBranchAddress("mcJet_fromW2_isMatching", treeVars.mcJet_fromW2_isMatching);
  
  tree -> SetBranchAddress("mcTagQ_energy", treeVars.mcTagQ_energy);
  tree -> SetBranchAddress("mcTagQ_px",     treeVars.mcTagQ_px);
  tree -> SetBranchAddress("mcTagQ_py",     treeVars.mcTagQ_py);
  tree -> SetBranchAddress("mcTagQ_pz",     treeVars.mcTagQ_pz);
  tree -> SetBranchAddress("mcTagQ_p",      treeVars.mcTagQ_p);
  tree -> SetBranchAddress("mcTagQ_pt",     treeVars.mcTagQ_pt);
  tree -> SetBranchAddress("mcTagQ_mass",   treeVars.mcTagQ_mass);
  tree -> SetBranchAddress("mcTagQ_phi",    treeVars.mcTagQ_phi);
  tree -> SetBranchAddress("mcTagQ_eta",    treeVars.mcTagQ_eta);
  tree -> SetBranchAddress("mcTagQ_charge", treeVars.mcTagQ_charge);
  tree -> SetBranchAddress("mcTagQ_pdgId",  treeVars.mcTagQ_pdgId);
  
  tree -> SetBranchAddress("mcTagJet_energy",     treeVars.mcTagJet_energy);
  tree -> SetBranchAddress("mcTagJet_px",         treeVars.mcTagJet_px);
  tree -> SetBranchAddress("mcTagJet_py",         treeVars.mcTagJet_py);
  tree -> SetBranchAddress("mcTagJet_pz",         treeVars.mcTagJet_pz);
  tree -> SetBranchAddress("mcTagJet_p",          treeVars.mcTagJet_p);
  tree -> SetBranchAddress("mcTagJet_pt",         treeVars.mcTagJet_pt);
  tree -> SetBranchAddress("mcTagJet_mass",       treeVars.mcTagJet_mass);
  tree -> SetBranchAddress("mcTagJet_phi",        treeVars.mcTagJet_phi);
  tree -> SetBranchAddress("mcTagJet_eta",        treeVars.mcTagJet_eta);
  tree -> SetBranchAddress("mcTagJet_charge",     treeVars.mcTagJet_charge);
  tree -> SetBranchAddress("mcTagJet_isMatching", treeVars.mcTagJet_isMatching);
  
  
  
  
  
  
  // mc electrons
  tree -> SetBranchAddress("nMcE",      &treeVars.nMcE);
  tree -> SetBranchAddress("mcE_energy", treeVars.mcE_energy);
  tree -> SetBranchAddress("mcE_px",     treeVars.mcE_px);
  tree -> SetBranchAddress("mcE_py",     treeVars.mcE_py);
  tree -> SetBranchAddress("mcE_pz",     treeVars.mcE_pz);
  tree -> SetBranchAddress("mcE_p",      treeVars.mcE_p);
  tree -> SetBranchAddress("mcE_pt",     treeVars.mcE_pt);
  tree -> SetBranchAddress("mcE_mass",   treeVars.mcE_mass);
  tree -> SetBranchAddress("mcE_phi",    treeVars.mcE_phi);
  tree -> SetBranchAddress("mcE_eta",    treeVars.mcE_eta);
  tree -> SetBranchAddress("mcE_charge", treeVars.mcE_charge);

  // reco electrons
  tree -> SetBranchAddress("nRecoE",      &treeVars.nRecoE);
  tree -> SetBranchAddress("recoE_energy", treeVars.recoE_energy);
  tree -> SetBranchAddress("recoE_px",     treeVars.recoE_px);
  tree -> SetBranchAddress("recoE_py",     treeVars.recoE_py);
  tree -> SetBranchAddress("recoE_pz",     treeVars.recoE_pz);
  tree -> SetBranchAddress("recoE_p",      treeVars.recoE_p);
  tree -> SetBranchAddress("recoE_pt",     treeVars.recoE_pt);
  tree -> SetBranchAddress("recoE_mass",   treeVars.recoE_mass);
  tree -> SetBranchAddress("recoE_phi",    treeVars.recoE_phi);
  tree -> SetBranchAddress("recoE_eta",    treeVars.recoE_eta);
  tree -> SetBranchAddress("recoE_charge", treeVars.recoE_charge);
  
  tree -> SetBranchAddress("recoE_SuperClusterEnergy", treeVars.recoE_SuperClusterEnergy);
  tree -> SetBranchAddress("recoE_HOverEm",            treeVars.recoE_HOverEm);
  
  tree -> SetBranchAddress("recoE_tkIso",      treeVars.recoE_tkIso);
  //tree -> SetBranchAddress("recoE_tkIsoVeto",  treeVars.recoE_tkIsoVeto);
  //tree -> SetBranchAddress("recoE_nTkIso",     treeVars.recoE_nTkIso);
  tree -> SetBranchAddress("recoE_emIso",      treeVars.recoE_emIso);
  //tree -> SetBranchAddress("recoE_emIsoVeto",  treeVars.recoE_emIsoVeto);
  tree -> SetBranchAddress("recoE_hadIso",     treeVars.recoE_hadIso);
  //tree -> SetBranchAddress("recoE_hadIsoVeto", treeVars.recoE_hadIsoVeto);
  
  tree -> SetBranchAddress("recoE_eleIdLoose",       treeVars.recoE_eleIdLoose);
  tree -> SetBranchAddress("recoE_eleIdTight",       treeVars.recoE_eleIdTight);
  tree -> SetBranchAddress("recoE_eleIdRobustLoose", treeVars.recoE_eleIdRobustLoose);
  tree -> SetBranchAddress("recoE_eleIdRobustTight", treeVars.recoE_eleIdRobustTight);
  
  
  
  
  
  
  // mc muons
  tree -> SetBranchAddress("nMcMu",      &treeVars.nMcMu);
  tree -> SetBranchAddress("mcMu_energy", treeVars.mcMu_energy);
  tree -> SetBranchAddress("mcMu_px",     treeVars.mcMu_px);
  tree -> SetBranchAddress("mcMu_py",     treeVars.mcMu_py);
  tree -> SetBranchAddress("mcMu_pz",     treeVars.mcMu_pz);
  tree -> SetBranchAddress("mcMu_p",      treeVars.mcMu_p);
  tree -> SetBranchAddress("mcMu_pt",     treeVars.mcMu_pt);
  tree -> SetBranchAddress("mcMu_mass",   treeVars.mcMu_mass);
  tree -> SetBranchAddress("mcMu_phi",    treeVars.mcMu_phi);
  tree -> SetBranchAddress("mcMu_eta",    treeVars.mcMu_eta);
  tree -> SetBranchAddress("mcMu_charge", treeVars.mcMu_charge);  

  // reco muons
  tree -> SetBranchAddress("nRecoMu",      &treeVars.nRecoMu);
  tree -> SetBranchAddress("recoMu_energy", treeVars.recoMu_energy);
  tree -> SetBranchAddress("recoMu_px",     treeVars.recoMu_px);
  tree -> SetBranchAddress("recoMu_py",     treeVars.recoMu_py);
  tree -> SetBranchAddress("recoMu_pz",     treeVars.recoMu_pz);
  tree -> SetBranchAddress("recoMu_p",      treeVars.recoMu_p);
  tree -> SetBranchAddress("recoMu_pt",     treeVars.recoMu_pt);
  tree -> SetBranchAddress("recoMu_mass",   treeVars.recoMu_mass);
  tree -> SetBranchAddress("recoMu_phi",    treeVars.recoMu_phi);
  tree -> SetBranchAddress("recoMu_eta",    treeVars.recoMu_eta);
  tree -> SetBranchAddress("recoMu_charge", treeVars.recoMu_charge);
  
  tree -> SetBranchAddress("recoMu_tkIsoR03",      treeVars.recoMu_tkIsoR03);
  //tree -> SetBranchAddress("recoMu_tkIsoR03Veto",  treeVars.recoMu_tkIsoR03Veto);
  tree -> SetBranchAddress("recoMu_nTkIsoR03",     treeVars.recoMu_nTkIsoR03);
  tree -> SetBranchAddress("recoMu_emIsoR03",      treeVars.recoMu_emIsoR03);
  //tree -> SetBranchAddress("recoMu_emIsoR03Veto",  treeVars.recoMu_emIsoR03Veto);
  tree -> SetBranchAddress("recoMu_hadIsoR03",     treeVars.recoMu_hadIsoR03);
  //tree -> SetBranchAddress("recoMu_hadIsoR03Veto", treeVars.recoMu_hadIsoR03Veto);
  
  tree -> SetBranchAddress("recoMu_tkIsoR05",      treeVars.recoMu_tkIsoR05);
  //tree -> SetBranchAddress("recoMu_tkIsoR05Veto",  treeVars.recoMu_tkIsoR05Veto);
  tree -> SetBranchAddress("recoMu_nTkIsoR05",     treeVars.recoMu_nTkIsoR05);
  tree -> SetBranchAddress("recoMu_emIsoR05",      treeVars.recoMu_emIsoR05);
  //tree -> SetBranchAddress("recoMu_emIsoR05Veto",  treeVars.recoMu_emIsoR05Veto);
  tree -> SetBranchAddress("recoMu_hadIsoR05",     treeVars.recoMu_hadIsoR05);
  //tree -> SetBranchAddress("recoMu_hadIsoR05Veto", treeVars.recoMu_hadIsoR05Veto);
  
  
  
  
  
  
  // mc jets
  tree -> SetBranchAddress("nMcJet",      &treeVars.nMcJet);
  tree -> SetBranchAddress("mcJet_energy", treeVars.mcJet_energy);
  tree -> SetBranchAddress("mcJet_px",     treeVars.mcJet_px);
  tree -> SetBranchAddress("mcJet_py",     treeVars.mcJet_py);
  tree -> SetBranchAddress("mcJet_pz",     treeVars.mcJet_pz);
  tree -> SetBranchAddress("mcJet_p",      treeVars.mcJet_p);
  tree -> SetBranchAddress("mcJet_pt",     treeVars.mcJet_pt);
  tree -> SetBranchAddress("mcJet_mass",   treeVars.mcJet_mass);
  tree -> SetBranchAddress("mcJet_phi",    treeVars.mcJet_phi);
  tree -> SetBranchAddress("mcJet_eta",    treeVars.mcJet_eta);
  tree -> SetBranchAddress("mcJet_charge", treeVars.mcJet_charge);
  
  
  
  // reco jets
  tree -> SetBranchAddress("nRecoJet",      &treeVars.nRecoJet);
  tree -> SetBranchAddress("recoJet_energy", treeVars.recoJet_energy);
  tree -> SetBranchAddress("recoJet_px",     treeVars.recoJet_px);
  tree -> SetBranchAddress("recoJet_py",     treeVars.recoJet_py);
  tree -> SetBranchAddress("recoJet_pz",     treeVars.recoJet_pz);
  tree -> SetBranchAddress("recoJet_p",      treeVars.recoJet_p);
  tree -> SetBranchAddress("recoJet_pt",     treeVars.recoJet_pt);
  tree -> SetBranchAddress("recoJet_mass",   treeVars.recoJet_mass);
  tree -> SetBranchAddress("recoJet_phi",    treeVars.recoJet_phi);
  tree -> SetBranchAddress("recoJet_eta",    treeVars.recoJet_eta);
  tree -> SetBranchAddress("recoJet_charge", treeVars.recoJet_charge);

}


 



void SetBranches(TTree* tree, VBFNtupleContent& treeVars)
{
  tree -> Branch("eventType", &treeVars.eventType, "eventType/i");
  tree -> Branch("eventId",   &treeVars.eventId,     "eventId/i");
  
  
  
  // mc variables
  tree -> Branch("mcH_energy", &treeVars.mcH_energy, "mcH_energy/F");
  tree -> Branch("mcH_px",     &treeVars.mcH_px,         "mcH_px/F");
  tree -> Branch("mcH_py",     &treeVars.mcH_py,         "mcH_py/F");
  tree -> Branch("mcH_pz",     &treeVars.mcH_pz,         "mcH_pz/F");
  tree -> Branch("mcH_p",      &treeVars.mcH_p,           "mcH_p/F");
  tree -> Branch("mcH_pt",     &treeVars.mcH_pt,         "mcH_pt/F");
  tree -> Branch("mcH_mass",   &treeVars.mcH_mass,     "mcH_mass/F");
  tree -> Branch("mcH_phi",    &treeVars.mcH_phi,       "mcH_phi/F");
  tree -> Branch("mcH_eta",    &treeVars.mcH_eta,       "mcH_eta/F");
  tree -> Branch("mcH_charge", &treeVars.mcH_charge, "mcH_charge/I");
  
  tree -> Branch("mcW_energy", treeVars.mcW_energy, "mcW_energy[2]/F");
  tree -> Branch("mcW_px",     treeVars.mcW_px,         "mcW_px[2]/F");
  tree -> Branch("mcW_py",     treeVars.mcW_py,         "mcW_py[2]/F");
  tree -> Branch("mcW_pz",     treeVars.mcW_pz,         "mcW_pz[2]/F");
  tree -> Branch("mcW_p",      treeVars.mcW_p,           "mcW_p[2]/F");
  tree -> Branch("mcW_pt",     treeVars.mcW_pt,         "mcW_pt[2]/F");
  tree -> Branch("mcW_mass",   treeVars.mcW_mass,     "mcW_mass[2]/F");
  tree -> Branch("mcW_phi",    treeVars.mcW_phi,       "mcW_phi[2]/F");
  tree -> Branch("mcW_eta",    treeVars.mcW_eta,       "mcW_eta[2]/F");
  tree -> Branch("mcW_charge", treeVars.mcW_charge, "mcW_charge[2]/I");
  
  tree -> Branch("mcF_fromW1_energy", treeVars.mcF_fromW1_energy, "mcF_fromW1_energy[2]/F");
  tree -> Branch("mcF_fromW1_px",     treeVars.mcF_fromW1_px,         "mcF_fromW1_px[2]/F");
  tree -> Branch("mcF_fromW1_py",     treeVars.mcF_fromW1_py,         "mcF_fromW1_py[2]/F");
  tree -> Branch("mcF_fromW1_pz",     treeVars.mcF_fromW1_pz,         "mcF_fromW1_pz[2]/F");
  tree -> Branch("mcF_fromW1_p",      treeVars.mcF_fromW1_p,           "mcF_fromW1_p[2]/F");
  tree -> Branch("mcF_fromW1_pt",     treeVars.mcF_fromW1_pt,         "mcF_fromW1_pt[2]/F");
  tree -> Branch("mcF_fromW1_mass",   treeVars.mcF_fromW1_mass,     "mcF_fromW1_mass[2]/F");
  tree -> Branch("mcF_fromW1_phi",    treeVars.mcF_fromW1_phi,       "mcF_fromW1_phi[2]/F");
  tree -> Branch("mcF_fromW1_eta",    treeVars.mcF_fromW1_eta,       "mcF_fromW1_eta[2]/F");
  tree -> Branch("mcF_fromW1_charge", treeVars.mcF_fromW1_charge, "mcF_fromW1_charge[2]/I");
  tree -> Branch("mcF_fromW1_pdgId",  treeVars.mcF_fromW1_pdgId,   "mcF_fromW1_pdgId[2]/I");
  
  tree -> Branch("mcQ_fromW2_energy", treeVars.mcQ_fromW2_energy, "mcQ_fromW2_energy[2]/F");
  tree -> Branch("mcQ_fromW2_px",     treeVars.mcQ_fromW2_px,         "mcQ_fromW2_px[2]/F");
  tree -> Branch("mcQ_fromW2_py",     treeVars.mcQ_fromW2_py,         "mcQ_fromW2_py[2]/F");
  tree -> Branch("mcQ_fromW2_pz",     treeVars.mcQ_fromW2_pz,         "mcQ_fromW2_pz[2]/F");
  tree -> Branch("mcQ_fromW2_p",      treeVars.mcQ_fromW2_p,           "mcQ_fromW2_p[2]/F");
  tree -> Branch("mcQ_fromW2_pt",     treeVars.mcQ_fromW2_pt,         "mcQ_fromW2_pt[2]/F");
  tree -> Branch("mcQ_fromW2_mass",   treeVars.mcQ_fromW2_mass,     "mcQ_fromW2_mass[2]/F");
  tree -> Branch("mcQ_fromW2_phi",    treeVars.mcQ_fromW2_phi,       "mcQ_fromW2_phi[2]/F");
  tree -> Branch("mcQ_fromW2_eta",    treeVars.mcQ_fromW2_eta,       "mcQ_fromW2_eta[2]/F");
  tree -> Branch("mcQ_fromW2_charge", treeVars.mcQ_fromW2_charge, "mcQ_fromW2_charge[2]/I");
  tree -> Branch("mcQ_fromW2_pdgId",  treeVars.mcQ_fromW2_pdgId,   "mcQ_fromW2_pdgId[2]/I");
  
  tree -> Branch("mcJet_fromW2_energy", treeVars.mcJet_fromW2_energy, "mcJet_fromW2_energy[2]/F");
  tree -> Branch("mcJet_fromW2_px",     treeVars.mcJet_fromW2_px,         "mcJet_fromW2_px[2]/F");
  tree -> Branch("mcJet_fromW2_py",     treeVars.mcJet_fromW2_py,         "mcJet_fromW2_py[2]/F");
  tree -> Branch("mcJet_fromW2_pz",     treeVars.mcJet_fromW2_pz,         "mcJet_fromW2_pz[2]/F");
  tree -> Branch("mcJet_fromW2_p",      treeVars.mcJet_fromW2_p,           "mcJet_fromW2_p[2]/F");
  tree -> Branch("mcJet_fromW2_pt",     treeVars.mcJet_fromW2_pt,         "mcJet_fromW2_pt[2]/F");
  tree -> Branch("mcJet_fromW2_mass",   treeVars.mcJet_fromW2_mass,     "mcJet_fromW2_mass[2]/F");
  tree -> Branch("mcJet_fromW2_phi",    treeVars.mcJet_fromW2_phi,       "mcJet_fromW2_phi[2]/F");
  tree -> Branch("mcJet_fromW2_eta",    treeVars.mcJet_fromW2_eta,       "mcJet_fromW2_eta[2]/F");
  tree -> Branch("mcJet_fromW2_charge", treeVars.mcJet_fromW2_charge, "mcJet_fromW2_charge[2]/I");
  tree -> Branch("mcJet_fromW2_isMatching",  treeVars.mcJet_fromW2_isMatching,   "mcJet_fromW2_isMatching[2]/I");
  
  tree -> Branch("mcTagQ_energy", treeVars.mcTagQ_energy, "mcTagQ_energy[2]/F");
  tree -> Branch("mcTagQ_px",     treeVars.mcTagQ_px,         "mcTagQ_px[2]/F");
  tree -> Branch("mcTagQ_py",     treeVars.mcTagQ_py,         "mcTagQ_py[2]/F");
  tree -> Branch("mcTagQ_pz",     treeVars.mcTagQ_pz,         "mcTagQ_pz[2]/F");
  tree -> Branch("mcTagQ_p",      treeVars.mcTagQ_p,           "mcTagQ_p[2]/F");
  tree -> Branch("mcTagQ_pt",     treeVars.mcTagQ_pt,         "mcTagQ_pt[2]/F");
  tree -> Branch("mcTagQ_mass",   treeVars.mcTagQ_mass,     "mcTagQ_mass[2]/F");
  tree -> Branch("mcTagQ_phi",    treeVars.mcTagQ_phi,       "mcTagQ_phi[2]/F");
  tree -> Branch("mcTagQ_eta",    treeVars.mcTagQ_eta,       "mcTagQ_eta[2]/F");
  tree -> Branch("mcTagQ_charge", treeVars.mcTagQ_charge, "mcTagQ_charge[2]/I");
  tree -> Branch("mcTagQ_pdgId",  treeVars.mcTagQ_pdgId,   "mcTagQ_pdgId[2]/I");
  
  tree -> Branch("mcTagJet_energy", treeVars.mcTagJet_energy, "mcTagJet_energy[2]/F");
  tree -> Branch("mcTagJet_px",     treeVars.mcTagJet_px,         "mcTagJet_px[2]/F");
  tree -> Branch("mcTagJet_py",     treeVars.mcTagJet_py,         "mcTagJet_py[2]/F");
  tree -> Branch("mcTagJet_pz",     treeVars.mcTagJet_pz,         "mcTagJet_pz[2]/F");
  tree -> Branch("mcTagJet_p",      treeVars.mcTagJet_p,           "mcTagJet_p[2]/F");
  tree -> Branch("mcTagJet_pt",     treeVars.mcTagJet_pt,         "mcTagJet_pt[2]/F");
  tree -> Branch("mcTagJet_mass",   treeVars.mcTagJet_mass,     "mcTagJet_mass[2]/F");
  tree -> Branch("mcTagJet_phi",    treeVars.mcTagJet_phi,       "mcTagJet_phi[2]/F");
  tree -> Branch("mcTagJet_eta",    treeVars.mcTagJet_eta,       "mcTagJet_eta[2]/F");
  tree -> Branch("mcTagJet_charge", treeVars.mcTagJet_charge, "mcTagJet_charge[2]/I");
  tree -> Branch("mcTagJet_isMatching",  treeVars.mcTagJet_isMatching,   "mcTagJet_isMatching[2]/I");
  
  
  
  
  
  
  // mc electrons
  tree -> Branch("nMcE",      &treeVars.nMcE,                 "nMcE/i");
  tree -> Branch("mcE_energy", treeVars.mcE_energy, "mcE_energy[25]/F");
  tree -> Branch("mcE_px",     treeVars.mcE_px,         "mcE_px[25]/F");
  tree -> Branch("mcE_py",     treeVars.mcE_py,         "mcE_py[25]/F");
  tree -> Branch("mcE_pz",     treeVars.mcE_pz,         "mcE_pz[25]/F");
  tree -> Branch("mcE_p",      treeVars.mcE_p,           "mcE_p[25]/F");
  tree -> Branch("mcE_pt",     treeVars.mcE_pt,         "mcE_pt[25]/F");
  tree -> Branch("mcE_mass",   treeVars.mcE_mass,     "mcE_mass[25]/F");
  tree -> Branch("mcE_phi",    treeVars.mcE_phi,       "mcE_phi[25]/F");
  tree -> Branch("mcE_eta",    treeVars.mcE_eta,       "mcE_eta[25]/F");
  tree -> Branch("mcE_charge", treeVars.mcE_charge, "mcE_charge[25]/I");  
  
  // reco electrons
  tree -> Branch("nRecoE",      &treeVars.nRecoE,                 "nRecoE/i");
  tree -> Branch("recoE_energy", treeVars.recoE_energy, "recoE_energy[25]/F");
  tree -> Branch("recoE_px",     treeVars.recoE_px,         "recoE_px[25]/F");
  tree -> Branch("recoE_py",     treeVars.recoE_py,         "recoE_py[25]/F");
  tree -> Branch("recoE_pz",     treeVars.recoE_pz,         "recoE_pz[25]/F");
  tree -> Branch("recoE_p",      treeVars.recoE_p,           "recoE_p[25]/F");
  tree -> Branch("recoE_pt",     treeVars.recoE_pt,         "recoE_pt[25]/F");
  tree -> Branch("recoE_mass",   treeVars.recoE_mass,     "recoE_mass[25]/F");
  tree -> Branch("recoE_phi",    treeVars.recoE_phi,       "recoE_phi[25]/F");
  tree -> Branch("recoE_eta",    treeVars.recoE_eta,       "recoE_eta[25]/F");
  tree -> Branch("recoE_charge", treeVars.recoE_charge, "recoE_charge[25]/I");
  
  tree -> Branch("recoE_SuperClusterEnergy", treeVars.recoE_SuperClusterEnergy, "recoE_SuperClusterEnergy[25]/F");
  tree -> Branch("recoE_HOverEm",            treeVars.recoE_HOverEm,                       "recoE_HOverEm[25]/F");
  
  tree -> Branch("recoE_tkIso",      treeVars.recoE_tkIso,           "recoE_tkIso[25]/F");
  //tree -> Branch("recoE_tkIsoVeto",  treeVars.recoE_tkIsoVeto,   "recoE_tkIsoVeto[25]/F");
  //tree -> Branch("recoE_nTkIso",     treeVars.recoE_nTkIso,         "recoE_nTkIso[25]/I");
  tree -> Branch("recoE_emIso",      treeVars.recoE_emIso,           "recoE_emIso[25]/F");
  //tree -> Branch("recoE_emIsoVeto",  treeVars.recoE_emIsoVeto,   "recoE_emIsoVeto[25]/F");
  tree -> Branch("recoE_hadIso",     treeVars.recoE_hadIso,         "recoE_hadIso[25]/F");
  //tree -> Branch("recoE_hadIsoVeto", treeVars.recoE_hadIsoVeto, "recoE_hadIsoVeto[25]/F");
  
  tree -> Branch("recoE_eleIdLoose",       treeVars.recoE_eleIdLoose,       "recoE_eleIdLoose[25]/F");
  tree -> Branch("recoE_eleIdTight",       treeVars.recoE_eleIdTight,       "recoE_eleIdTight[25]/F");
  tree -> Branch("recoE_eleIdRobustLoose", treeVars.recoE_eleIdRobustLoose, "recoE_eleIdRobustLoose[25]/F");
  tree -> Branch("recoE_eleIdRobustTight", treeVars.recoE_eleIdRobustTight, "recoE_eleIdRobustTight[25]/F");
  
  
  
  
  
  
  // mc muons
  tree -> Branch("nMcMu",      &treeVars.nMcMu,                 "nMcMu/i");
  tree -> Branch("mcMu_energy", treeVars.mcMu_energy, "mcMu_energy[25]/F");
  tree -> Branch("mcMu_px",     treeVars.mcMu_px,         "mcMu_px[25]/F");
  tree -> Branch("mcMu_py",     treeVars.mcMu_py,         "mcMu_py[25]/F");
  tree -> Branch("mcMu_pz",     treeVars.mcMu_pz,         "mcMu_pz[25]/F");
  tree -> Branch("mcMu_p",      treeVars.mcMu_p,           "mcMu_p[25]/F");
  tree -> Branch("mcMu_pt",     treeVars.mcMu_pt,         "mcMu_pt[25]/F");
  tree -> Branch("mcMu_mass",   treeVars.mcMu_mass,     "mcMu_mass[25]/F");
  tree -> Branch("mcMu_phi",    treeVars.mcMu_phi,       "mcMu_phi[25]/F");
  tree -> Branch("mcMu_eta",    treeVars.mcMu_eta,       "mcMu_eta[25]/F");
  tree -> Branch("mcMu_charge", treeVars.mcMu_charge, "mcMu_charge[25]/I");  
  
  // reco muons
  tree -> Branch("nRecoMu",      &treeVars.nRecoMu,                 "nRecoMu/i");
  tree -> Branch("recoMu_energy", treeVars.recoMu_energy, "recoMu_energy[25]/F");
  tree -> Branch("recoMu_px",     treeVars.recoMu_px,         "recoMu_px[25]/F");
  tree -> Branch("recoMu_py",     treeVars.recoMu_py,         "recoMu_py[25]/F");
  tree -> Branch("recoMu_pz",     treeVars.recoMu_pz,         "recoMu_pz[25]/F");
  tree -> Branch("recoMu_p",      treeVars.recoMu_p,           "recoMu_p[25]/F");
  tree -> Branch("recoMu_pt",     treeVars.recoMu_pt,         "recoMu_pt[25]/F");
  tree -> Branch("recoMu_mass",   treeVars.recoMu_mass,     "recoMu_mass[25]/F");
  tree -> Branch("recoMu_phi",    treeVars.recoMu_phi,       "recoMu_phi[25]/F");
  tree -> Branch("recoMu_eta",    treeVars.recoMu_eta,       "recoMu_eta[25]/F");
  tree -> Branch("recoMu_charge", treeVars.recoMu_charge, "recoMu_charge[25]/I");
  
  tree -> Branch("recoMu_tkIsoR03",      treeVars.recoMu_tkIsoR03,           "recoMu_tkIsoR03[25]/F");
  //tree -> Branch("recoMu_tkIsoR03Veto",  treeVars.recoMu_tkIsoR03Veto,   "recoMu_tkIsoR03Veto[25]/F");
  tree -> Branch("recoMu_nTkIsoR03",     treeVars.recoMu_nTkIsoR03,         "recoMu_nTkIsoR03[25]/I");
  tree -> Branch("recoMu_emIsoR03",      treeVars.recoMu_emIsoR03,           "recoMu_emIsoR03[25]/F");
  //tree -> Branch("recoMu_emIsoR03Veto",  treeVars.recoMu_emIsoR03Veto,   "recoMu_emIsoR03Veto[25]/F");
  tree -> Branch("recoMu_hadIsoR03",     treeVars.recoMu_hadIsoR03,         "recoMu_hadIsoR03[25]/F");
  //tree -> Branch("recoMu_hadIsoR03Veto", treeVars.recoMu_hadIsoR03Veto, "recoMu_hadIsoR03Veto[25]/F");
  
  tree -> Branch("recoMu_tkIsoR05",      treeVars.recoMu_tkIsoR05,           "recoMu_tkIsoR05[25]/F");
  //tree -> Branch("recoMu_tkIsoR05Veto",  treeVars.recoMu_tkIsoR05Veto,   "recoMu_tkIsoR05Veto[25]/F");
  tree -> Branch("recoMu_nTkIsoR05",     treeVars.recoMu_nTkIsoR05,         "recoMu_nTkIsoR05[25]/I");
  tree -> Branch("recoMu_emIsoR05",      treeVars.recoMu_emIsoR05,           "recoMu_emIsoR05[25]/F");
  //tree -> Branch("recoMu_emIsoR05Veto",  treeVars.recoMu_emIsoR05Veto,   "recoMu_emIsoR05Veto[25]/F");
  tree -> Branch("recoMu_hadIsoR05",     treeVars.recoMu_hadIsoR05,         "recoMu_hadIsoR05[25]/F");
  //tree -> Branch("recoMu_hadIsoR05Veto", treeVars.recoMu_hadIsoR05Veto, "recoMu_hadIsoR05Veto[25]/F");
  
  
  
  // mc jets
  tree -> Branch("nMcJet",      &treeVars.nMcJet,                  "nMcJet/i");
  tree -> Branch("mcJet_energy", treeVars.mcJet_energy, "mcJet_energy[75]/F");
  tree -> Branch("mcJet_px",     treeVars.mcJet_px,         "mcJet_px[75]/F");
  tree -> Branch("mcJet_py",     treeVars.mcJet_py,         "mcJet_py[75]/F");
  tree -> Branch("mcJet_pz",     treeVars.mcJet_pz,         "mcJet_pz[75]/F");
  tree -> Branch("mcJet_p",      treeVars.mcJet_p,           "mcJet_p[75]/F");
  tree -> Branch("mcJet_pt",     treeVars.mcJet_pt,         "mcJet_pt[75]/F");
  tree -> Branch("mcJet_mass",   treeVars.mcJet_mass,     "mcJet_mass[75]/F");
  tree -> Branch("mcJet_phi",    treeVars.mcJet_phi,       "mcJet_phi[75]/F");
  tree -> Branch("mcJet_eta",    treeVars.mcJet_eta,       "mcJet_eta[75]/F");
  tree -> Branch("mcJet_charge", treeVars.mcJet_charge, "mcJet_charge[75]/I");
  
  
  // reco jets
  tree -> Branch("nRecoJet",      &treeVars.nRecoJet,                  "nRecoJet/i");
  tree -> Branch("recoJet_energy", treeVars.recoJet_energy, "recoJet_energy[75]/F");
  tree -> Branch("recoJet_px",     treeVars.recoJet_px,         "recoJet_px[75]/F");
  tree -> Branch("recoJet_py",     treeVars.recoJet_py,         "recoJet_py[75]/F");
  tree -> Branch("recoJet_pz",     treeVars.recoJet_pz,         "recoJet_pz[75]/F");
  tree -> Branch("recoJet_p",      treeVars.recoJet_p,           "recoJet_p[75]/F");
  tree -> Branch("recoJet_pt",     treeVars.recoJet_pt,         "recoJet_pt[75]/F");
  tree -> Branch("recoJet_mass",   treeVars.recoJet_mass,     "recoJet_mass[75]/F");
  tree -> Branch("recoJet_phi",    treeVars.recoJet_phi,       "recoJet_phi[75]/F");
  tree -> Branch("recoJet_eta",    treeVars.recoJet_eta,       "recoJet_eta[75]/F");
  tree -> Branch("recoJet_charge", treeVars.recoJet_charge, "recoJet_charge[75]/I");
}






void InitializeBranches(TTree* tree, VBFNtupleContent& treeVars)
{
  treeVars.eventType = 0;
  treeVars.eventId   = 0; 
  
  
  
  treeVars.mcJet_fromW2_isMatching[0] = 0;
  treeVars.mcJet_fromW2_isMatching[1] = 0;
  treeVars.mcTagJet_isMatching[0] = 0;
  treeVars.mcTagJet_isMatching[1] = 0;
  
  
  
  // mc electrons
  treeVars.nMcE = 0;
  for(unsigned int i = 0; i < 25; ++i)
  {
    treeVars.mcE_energy[i] = -1.;
    treeVars.mcE_px[i] = 0.;
    treeVars.mcE_py[i] = 0.;
    treeVars.mcE_pz[i] = 0.;
    treeVars.mcE_p[i] = -1.;
    treeVars.mcE_pt[i] = -1.;
    treeVars.mcE_mass[i] = -1.;
    treeVars.mcE_phi[i] = 0.;
    treeVars.mcE_eta[i] = 0.;
    treeVars.mcE_charge[i] = 0;
  }
  
  // reco electrons
  treeVars.nRecoE = 0;
  for(unsigned int i = 0; i < 25; ++i)
  {
    treeVars.recoE_energy[i] = -1.;
    treeVars.recoE_px[i] = 0.;
    treeVars.recoE_py[i] = 0.;
    treeVars.recoE_pz[i] = 0.;
    treeVars.recoE_p[i] = -1.;
    treeVars.recoE_pt[i] = -1.;
    treeVars.recoE_mass[i] = -1.;
    treeVars.recoE_phi[i] = 0.;
    treeVars.recoE_eta[i] = 0.;
    treeVars.recoE_charge[i] = 0;
    
    treeVars.recoE_SuperClusterEnergy[i] = -1.;
    treeVars.recoE_HOverEm[i] = -1.;
    
    treeVars.recoE_tkIso[i] = -1.;
    //treeVars.recoE_tkIsoVeto[i] = -1.;
    //treeVars.recoE_nTkIso[i] = -1;
    treeVars.recoE_emIso[i] = -1.;
    //treeVars.recoE_emIsoVeto[i] = -1.;
    treeVars.recoE_hadIso[i] = -1.;
    //treeVars.recoE_hadIsoVeto[i] = -1.;
    
    treeVars.recoE_eleIdLoose[i] = -1.;
    treeVars.recoE_eleIdTight[i] = -1.;
    treeVars.recoE_eleIdRobustLoose[i] = -1.;
    treeVars.recoE_eleIdRobustTight[i] = -1.;
  }
  
  
  
  
  
  
  // mc muons
  treeVars.nMcMu = 0;
  for(unsigned int i = 0; i < 25; ++i)
  {
    treeVars.mcMu_energy[i] = -1.;
    treeVars.mcMu_px[i] = 0.;
    treeVars.mcMu_py[i] = 0.;
    treeVars.mcMu_pz[i] = 0.;
    treeVars.mcMu_p[i] = -1.;
    treeVars.mcMu_pt[i] = -1.;
    treeVars.mcMu_mass[i] = -1.;
    treeVars.mcMu_phi[i] = 0.;
    treeVars.mcMu_eta[i] = 0.;
    treeVars.mcMu_charge[i] = 0;
  }  
  
  // reco muons
  treeVars.nRecoMu = 0;
  for(unsigned int i = 0; i < 25; ++i)
  {
    treeVars.recoMu_energy[i] = -1.;
    treeVars.recoMu_px[i] = 0.;
    treeVars.recoMu_py[i] = 0.;
    treeVars.recoMu_pz[i] = 0.;
    treeVars.recoMu_p[i] = -1.;
    treeVars.recoMu_pt[i] = -1.;
    treeVars.recoMu_mass[i] = -1.;
    treeVars.recoMu_phi[i] = 0.;
    treeVars.recoMu_eta[i] = 0.;
    treeVars.recoMu_charge[i] = 0;

    treeVars.recoMu_tkIsoR03[i] = -1.;
    //treeVars.recoMu_tkIsoR03Veto[i] = -1.;
    treeVars.recoMu_nTkIsoR03[i] = -1;
    treeVars.recoMu_emIsoR03[i] = -1.;
    //treeVars.recoMu_emIsoR03Veto[i] = -1.;
    treeVars.recoMu_hadIsoR03[i] = -1.;
    //treeVars.recoMu_hadIsoR03Veto[i] = -1.;
    
    treeVars.recoMu_tkIsoR05[i] = -1.;
    //treeVars.recoMu_tkIsoR05Veto[i] = -1.;
    treeVars.recoMu_nTkIsoR05[i] = -1;
    treeVars.recoMu_emIsoR05[i] = -1.;
    //treeVars.recoMu_emIsoR05Veto[i] = -1.;
    treeVars.recoMu_hadIsoR05[i] = -1.;
    //treeVars.recoMu_hadIsoR05Veto[i] = -1.;
  }
  
  
  
  // mc jets
  treeVars.nMcJet = 0;
  for(unsigned int i = 0; i < 75; ++i)
  {
    treeVars.mcJet_energy[i] = -1.;
    treeVars.mcJet_px[i] = 0.;
    treeVars.mcJet_py[i] = 0.;
    treeVars.mcJet_pz[i] = 0.;
    treeVars.mcJet_p[i] = -1.;
    treeVars.mcJet_pt[i] = -1.;
    treeVars.mcJet_mass[i] = -1.;
    treeVars.mcJet_phi[i] = 0.;
    treeVars.mcJet_eta[i] = 0.;
    treeVars.mcJet_charge[i] = 0;
  }
  
  
  // reco jets
  treeVars.nRecoJet = 0;
  for(unsigned int i = 0; i < 75; ++i)
  {
    treeVars.recoJet_energy[i] = -1.;
    treeVars.recoJet_px[i] = 0.;
    treeVars.recoJet_py[i] = 0.;
    treeVars.recoJet_pz[i] = 0.;
    treeVars.recoJet_p[i] = -1.;
    treeVars.recoJet_pt[i] = -1.;
    treeVars.recoJet_mass[i] = -1.;
    treeVars.recoJet_phi[i] = 0.;
    treeVars.recoJet_eta[i] = 0.;
    treeVars.recoJet_charge[i] = 0;
  }
}
