#ifndef VBFAnalysisVariables_h
#define VBFAnalysisVariables_h

#include "treeReader.h"
#include "ntpleUtils.h"

#include "TH1F.h"
#include "TProfile.h"
#include "TObject.h"
#include "TTree.h"

#include "TMVA/Reader.h"



struct VBFAnalysisVariables
{
  // trees
  std::map<int, TTree*> m_reducedTrees;
  
  
  
  // outfile definition
  TFile* m_outputRootFile;
  
  
  
  // input parameters
  float mH;
  int totEvents;
  float crossSection;
  int dataFlag; 
  int runId;
  int lumiId;
  int eventId;
  int eventNaiveId;
  
  float eventWeight;
  
  
  
  // mva value
  float mva;
  
  
  
  // PV variables
  int PUit_n;
  int PUoot_n;
  float rhoForIsolation;
  float rhoForJets;
  
  
  
  // PV variables
  int PV_n;
  float PV_d0;
  int PV_nTracks;
  int PV_ndof;
  float PV_normalizedChi2;
  float PV_z;
  
  
  
  // lepton variables
  ROOT::Math::XYZTVector* p_lep;
  ROOT::Math::XYZTVector lep;
  ROOT::Math::XYZTVector* p_lep_KF;
  ROOT::Math::XYZTVector lep_KF;
  
  ROOT::Math::XYZTVector* p_lepNu;
  ROOT::Math::XYZTVector lepNu;
  
  float lep_charge;
  int lep_flavour;
  float lep_pt;
  float lep_eta;
  float lep_zepp;
  float lep_z;
  float lep_dxy_BS;
  float lep_dz_BS;
  float lep_dxy_PV;
  float lep_edxy_PV;
  float lep_dz_PV;
  float lep_tkIso;
  float lep_emIso;
  float lep_hadIso;
  int lep_isEB;
  float lep_etaSC;
  float lep_sigmaIetaIeta;
  float lep_DphiIn;
  float lep_DetaIn;
  float lep_HOverE;
  int lep_mishits;
  float lep_dist;
  float lep_dcot;
  int lep_tracker;
  int lep_standalone;
  int lep_global;
  float lep_normalizedChi2;
  int lep_numberOfMatches;
  int lep_numberOfValidTrackerHits;
  int lep_numberOfValidMuonHits;
  int lep_pixelLayersWithMeasurement;
  
  
  
  // met variables
  ROOT::Math::XYZTVector* p_met;
  ROOT::Math::XYZTVector met;
  ROOT::Math::XYZTVector* p_nu;
  ROOT::Math::XYZTVector nu;
  ROOT::Math::XYZTVector* p_nu_KF;
  ROOT::Math::XYZTVector nu_KF;
  float met_et;
  
  float lepMet_pt;
  float lepMet_mt;
  float lepMet_Dphi;
  
  
  
  // b-tag variables
  int nBTag_TCHEL_pt20;
  int nBTag_TCHEM_pt20;
  int nBTag_TCHPM_pt20;
  int nBTag_TCHPT_pt20;
  int nBTag_SSVHEM_pt20;
  int nBTag_SSVHPT_pt20;

  int nBTag_TCHEL_pt25;
  int nBTag_TCHEM_pt25;
  int nBTag_TCHPM_pt25;
  int nBTag_TCHPT_pt25;
  int nBTag_SSVHEM_pt25;
  int nBTag_SSVHPT_pt25;

  int nBTag_TCHEL_pt30;
  int nBTag_TCHEM_pt30;
  int nBTag_TCHPM_pt30;
  int nBTag_TCHPT_pt30;
  int nBTag_SSVHEM_pt30;
  int nBTag_SSVHPT_pt30;
  
  
  
  // jet variables
  int nJets;
  int nJets_cnt;
  int nJets_fwd;
  int nJets_pt20;
  int nJets_cnt_pt20;
  int nJets_fwd_pt20;
  int nJets_btw_pt20;
  int nJets_pt25;
  int nJets_cnt_pt25;
  int nJets_fwd_pt25;
  int nJets_btw_pt25;
  int nJets_pt30;
  int nJets_cnt_pt30;
  int nJets_fwd_pt30;
  int nJets_btw_pt30;

  float jets_bTag1;
  float jets_bTag2;
  
  ROOT::Math::XYZTVector* p_leadingJ;
  ROOT::Math::XYZTVector leadingJ;
    
  float leadingJ_bTag;
  float leadingJ_ptD;
  int leadingJ_chargedMultiplicity;
  int leadingJ_neutralMultiplicity;
  
  
  
  // W-jet variables
  ROOT::Math::XYZTVector* p_WJ1;
  ROOT::Math::XYZTVector WJ1;
  ROOT::Math::XYZTVector* p_WJ2;
  ROOT::Math::XYZTVector WJ2;

  ROOT::Math::XYZTVector* p_WJ1_KF;
  ROOT::Math::XYZTVector WJ1_KF;
  ROOT::Math::XYZTVector* p_WJ2_KF;
  ROOT::Math::XYZTVector WJ2_KF;
  
  ROOT::Math::XYZTVector* p_WJJ;
  ROOT::Math::XYZTVector WJJ;
  
  float WJ1_lep_Dphi;
  float WJ1_met_Dphi;
  float WJ2_lep_Dphi;
  float WJ2_met_Dphi;
  float WJ1_zepp;
  float WJ2_zepp;
  float WJ1_bTag;
  float WJ2_bTag;
  float WJ1_ptD;
  float WJ2_ptD;
  int WJ1_chargedMultiplicity;
  int WJ2_chargedMultiplicity;
  int WJ1_neutralMultiplicity;
  int WJ2_neutralMultiplicity;
  float WJ1_QGLikelihood;
  float WJ2_QGLikelihood;
  
  float WJJ_Deta;
  float WJJ_Dphi;
  float WJJ_DR;
  float WJJ_pt;
  float WJJ_m;
  float WJJ_zepp;
  
  float lepWJJ_pt1;  
  float lepWJJ_pt2;  
  float lepWJJ_pt3;  
  
  
  
  // Higgs variables
  float lepW_pt;
  
  float lepMetW_pt;
  float lepMetW_mt;
  float lepMetW_Dphi;
  float lepNu_m;
  int lepNu_nSolutions;
  float lepNuW_m;
  float lepNuW_zepp;
  
  float lepNuW_m_KF;
  float chi2_KF;
  int ndf_KF;
  int status_KF;
  
  float lepNuW_cphi;
  float lepNuZ_cphi;
  float lep_ctheta;
  float WJ1_ctheta;
  float lepNu_ctheta;
  
  
  
  // tag-jet variables
  ROOT::Math::XYZTVector* p_tagJ1;
  ROOT::Math::XYZTVector tagJ1;
  ROOT::Math::XYZTVector* p_tagJ2;
  ROOT::Math::XYZTVector tagJ2;
  
  float tagJ1_bTag;
  float tagJ2_bTag;
  float tagJ1_ptD;
  float tagJ2_ptD;
  int tagJ1_chargedMultiplicity;
  int tagJ2_chargedMultiplicity;
  int tagJ1_neutralMultiplicity;
  int tagJ2_neutralMultiplicity;
  
  float tagJJ_Deta;
  float tagJJ_Dphi;
  float tagJJ_DR;
  float tagJJ_m;
  
  
  
  // third jet variables
  ROOT::Math::XYZTVector* p_thirdJ;
  ROOT::Math::XYZTVector thirdJ;
  
  float thirdJ_bTag;
  float thirdJ_WJJDeta;
  float thirdJ_WJJDphi;
  float thirdJ_WJJDR;
  float thirdJ_tagJJDeta;
  float thirdJ_tagJJDphi;
  float thirdJ_tagJJDR;
};



void SetVBFPreselectionTreeBranches(VBFAnalysisVariables&, TTree*);
void AddVBFAnalysisTreeBranches(VBFAnalysisVariables& vars, TTree*);
void InitialiseVBFAnalysisTree(VBFAnalysisVariables& vars, TFile* outputRootFile, const int& nStep, const int& firstStep);
void FillVBFAnalysisTree(VBFAnalysisVariables& vars, const int&);

#endif
