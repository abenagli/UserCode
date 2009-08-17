#ifndef VBFNtupleContent_h
#define VBFNtupleContent_h

#include "TChain.h" 

#define MCEMAX 25
#define MCMUMAX 25
#define MCJETMAX 75

#define RECOEMAX 25
#define RECOMUMAX 25
#define RECOJETMAX 75






// ------------------------------------------------------------------------
//! Define variables

struct VBFNtupleContent
{
  unsigned int eventType;
  unsigned int eventId;
  
  
  
  // mc variables
  float mcH_energy;
  float mcH_px;
  float mcH_py;
  float mcH_pz;
  float mcH_p;
  float mcH_pt;
  float mcH_mass;
  float mcH_phi;
  float mcH_eta;
  int mcH_charge;  
  
  float mcW_energy[2];
  float mcW_px[2];
  float mcW_py[2];
  float mcW_pz[2];
  float mcW_p[2];
  float mcW_pt[2];
  float mcW_mass[2];
  float mcW_phi[2];
  float mcW_eta[2];
  int mcW_charge[2];
  
  float mcF_fromW1_energy[2];
  float mcF_fromW1_px[2];
  float mcF_fromW1_py[2];
  float mcF_fromW1_pz[2];
  float mcF_fromW1_p[2];
  float mcF_fromW1_pt[2];
  float mcF_fromW1_mass[2];
  float mcF_fromW1_phi[2];
  float mcF_fromW1_eta[2];
  int mcF_fromW1_charge[2];
  int mcF_fromW1_pdgId[2];
  
  float mcQ_fromW2_energy[2];
  float mcQ_fromW2_px[2];
  float mcQ_fromW2_py[2];
  float mcQ_fromW2_pz[2];
  float mcQ_fromW2_p[2];
  float mcQ_fromW2_pt[2];
  float mcQ_fromW2_mass[2];
  float mcQ_fromW2_phi[2];
  float mcQ_fromW2_eta[2];
  int mcQ_fromW2_charge[2];
  int mcQ_fromW2_pdgId[2];
  
  float mcJet_fromW2_energy[2];
  float mcJet_fromW2_px[2];
  float mcJet_fromW2_py[2];
  float mcJet_fromW2_pz[2];
  float mcJet_fromW2_p[2];
  float mcJet_fromW2_pt[2];
  float mcJet_fromW2_mass[2];
  float mcJet_fromW2_phi[2];
  float mcJet_fromW2_eta[2];
  int mcJet_fromW2_charge[2];
  int mcJet_fromW2_isMatching[2];
  
  float mcTagQ_energy[2];
  float mcTagQ_px[2];
  float mcTagQ_py[2];
  float mcTagQ_pz[2];
  float mcTagQ_p[2];
  float mcTagQ_pt[2];
  float mcTagQ_mass[2];
  float mcTagQ_phi[2];
  float mcTagQ_eta[2];
  int mcTagQ_charge[2];
  int mcTagQ_pdgId[2];
  
  float mcTagJet_energy[2];
  float mcTagJet_px[2];
  float mcTagJet_py[2];
  float mcTagJet_pz[2];
  float mcTagJet_p[2];
  float mcTagJet_pt[2];
  float mcTagJet_mass[2];
  float mcTagJet_phi[2];
  float mcTagJet_eta[2];
  int mcTagJet_charge[2];
  int mcTagJet_isMatching[2];
  
  
  
  
  
  
  // mc electrons
  unsigned int nMcE;
  float mcE_energy[MCEMAX];
  float mcE_px[MCEMAX];
  float mcE_py[MCEMAX];
  float mcE_pz[MCEMAX];
  float mcE_p[MCEMAX];
  float mcE_pt[MCEMAX];
  float mcE_mass[MCEMAX];
  float mcE_phi[MCEMAX];
  float mcE_eta[MCEMAX];
  int mcE_charge[MCEMAX];  

  // reco electrons
  unsigned int nRecoE;
  float recoE_energy[RECOEMAX];
  float recoE_px[RECOEMAX];
  float recoE_py[RECOEMAX];
  float recoE_pz[RECOEMAX];
  float recoE_p[RECOEMAX];
  float recoE_pt[RECOEMAX];
  float recoE_mass[RECOEMAX];
  float recoE_phi[RECOEMAX];
  float recoE_eta[RECOEMAX];
  int recoE_charge[RECOEMAX];
  
  float recoE_SuperClusterEnergy[RECOEMAX];
  float recoE_HOverEm[RECOEMAX];
  
  float recoE_tkIso[RECOMUMAX];
  //float recoE_tkIsoVeto[RECOMUMAX];
  //int recoE_nTkIso[RECOMUMAX];
  float recoE_emIso[RECOMUMAX];
  //float recoE_emIsoVeto[RECOMUMAX];
  float recoE_hadIso[RECOMUMAX];
  //float recoE_hadIsoVeto[RECOMUMAX];
    
  float recoE_eleIdLoose[RECOMUMAX];
  float recoE_eleIdTight[RECOMUMAX];
  float recoE_eleIdRobustLoose[RECOMUMAX];
  float recoE_eleIdRobustTight[RECOMUMAX];
  
  
  
  
  
  // mc muons
  unsigned int nMcMu;
  float mcMu_energy[MCMUMAX];
  float mcMu_px[MCMUMAX];
  float mcMu_py[MCMUMAX];
  float mcMu_pz[MCMUMAX];
  float mcMu_p[MCMUMAX];
  float mcMu_pt[MCMUMAX];
  float mcMu_mass[MCMUMAX];
  float mcMu_phi[MCMUMAX];
  float mcMu_eta[MCMUMAX];
  int mcMu_charge[MCMUMAX];
  
  // reco muons
  unsigned int nRecoMu;
  float recoMu_energy[RECOMUMAX];
  float recoMu_px[RECOMUMAX];
  float recoMu_py[RECOMUMAX];
  float recoMu_pz[RECOMUMAX];
  float recoMu_p[RECOMUMAX];
  float recoMu_pt[RECOMUMAX];
  float recoMu_mass[RECOMUMAX];
  float recoMu_phi[RECOMUMAX];
  float recoMu_eta[RECOMUMAX];
  int recoMu_charge[RECOMUMAX];
  
  float recoMu_tkIsoR03[RECOMUMAX];
  //float recoMu_tkIsoR03Veto[RECOMUMAX];
  int recoMu_nTkIsoR03[RECOMUMAX];
  float recoMu_emIsoR03[RECOMUMAX];
  //float recoMu_emIsoR03Veto[RECOMUMAX];
  float recoMu_hadIsoR03[RECOMUMAX];
  //float recoMu_hadIsoR03Veto[RECOMUMAX];
  
  float recoMu_tkIsoR05[RECOMUMAX];
  //float recoMu_tkIsoR05Veto[RECOMUMAX];
  int recoMu_nTkIsoR05[RECOMUMAX];
  float recoMu_emIsoR05[RECOMUMAX];
  //float recoMu_emIsoR05Veto[RECOMUMAX];
  float recoMu_hadIsoR05[RECOMUMAX];
  //float recoMu_hadIsoR05Veto[RECOMUMAX];
  
  
  
  // mc jets
  unsigned int nMcJet;
  float mcJet_energy[MCJETMAX];
  float mcJet_px[MCJETMAX];
  float mcJet_py[MCJETMAX];
  float mcJet_pz[MCJETMAX];
  float mcJet_p[MCJETMAX];
  float mcJet_pt[MCJETMAX];
  float mcJet_mass[MCJETMAX];
  float mcJet_phi[MCJETMAX];
  float mcJet_eta[MCJETMAX];
  int mcJet_charge[MCJETMAX];
  
  
    
  // reco jets
  unsigned int nRecoJet;
  float recoJet_energy[RECOJETMAX];
  float recoJet_px[RECOJETMAX];
  float recoJet_py[RECOJETMAX];
  float recoJet_pz[RECOJETMAX];
  float recoJet_p[RECOJETMAX];
  float recoJet_pt[RECOJETMAX];
  float recoJet_mass[RECOJETMAX];
  float recoJet_phi[RECOJETMAX];
  float recoJet_eta[RECOJETMAX];
  int recoJet_charge[RECOJETMAX];
};







// ------------------------------------------------------------------------
//! Branch addresses settings

void SetBranchAddresses(TTree* tree, VBFNtupleContent& treeVars);






// ------------------------------------------------------------------------
//! Create branches for the ntuple

void SetBranches(TTree* tree, VBFNtupleContent& treeVars);






// ------------------------------------------------------------------------
//! Initialize branches

void InitializeBranches(TTree* tree, VBFNtupleContent& treeVars);



#endif
