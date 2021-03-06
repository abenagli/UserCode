#ifndef VBFAnalysisVariables_h
#define VBFAnalysisVariables_h

#include "treeReader.h"
#include "ntpleUtils.h"

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
  int dataFlag;
  int MCFlag; 
  int totEvents;
  float crossSection;
  int runId;
  int lumiId;
  Long64_t eventId;
  int eventNaiveId;
  
  float eventWeight;
  float PUWeight;
  
  float leptonWeight;
  float metWeight;
  float jetWeight;
  
  // mva value
  float mva;
  
  
  
  // PDF variables
  std::vector<float> PDF_weights_CT10 ;
  std::vector<float>* p_PDF_weights_CT10 ;
  
  std::vector<float> PDF_weights_MSTW2008nlo68cl ;
  std::vector<float>* p_PDF_weights_MSTW2008nlo68cl ;
  
  std::vector<float> PDF_weights_NNPDF20 ;
  std::vector<float>* p_PDF_weights_NNPDF20 ;
  
  
  
  // PU variables
  int PUtrue_n;
  int PUit_n;
  int PUoot_early_n;
  int PUoot_late_n;
  float rhoForIsolationPFlow;
  
  
  
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
  ROOT::Math::XYZTVector* p_lep_KF1;
  ROOT::Math::XYZTVector lep_KF1;
  ROOT::Math::XYZTVector* p_lep_KF2;
  ROOT::Math::XYZTVector lep_KF2;
  
  ROOT::Math::XYZTVector* p_lepNu;
  ROOT::Math::XYZTVector lepNu;
  
  float lep_charge;
  int lep_flavour;
  float lep_pt;
  float lep_eta;
  float lep_phi;
  float lep_zepp;
  float lep_z;
  float lep_dxy_BS;
  float lep_dz_BS;
  float lep_dB;
  float lep_edB;
  float lep_dz_PV;
  float lep_tkIso;
  float lep_emIso;
  float lep_hadIso;
  float lep_pfChIso;
  float lep_pfNhIso;
  float lep_pfPhIso;
  float lep_combIso;
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
  
  int lep_mcMatched;
  float lep_mcMatchDR;
  
  
  
  // met variables
  ROOT::Math::XYZTVector* p_met;
  ROOT::Math::XYZTVector met;
  ROOT::Math::XYZTVector* p_nu;
  ROOT::Math::XYZTVector nu;
  ROOT::Math::XYZTVector* p_nu1;
  ROOT::Math::XYZTVector nu1;
  ROOT::Math::XYZTVector* p_nu2;
  ROOT::Math::XYZTVector nu2;
  ROOT::Math::XYZTVector* p_nu_KF;
  ROOT::Math::XYZTVector nu_KF;
  ROOT::Math::XYZTVector* p_nu_KF1;
  ROOT::Math::XYZTVector nu_KF1;
  ROOT::Math::XYZTVector* p_nu_KF2;
  ROOT::Math::XYZTVector nu_KF2;
  
  float met_et;
  float met_phi;
  
  int met_mcMatched;
  float met_mcMatchDR;
  
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
  
  ROOT::Math::XYZTVector jet1;
  ROOT::Math::XYZTVector* p_jet1;
  ROOT::Math::XYZTVector jet2;
  ROOT::Math::XYZTVector* p_jet2;
  ROOT::Math::XYZTVector jet3;
  ROOT::Math::XYZTVector* p_jet3;
  ROOT::Math::XYZTVector jet4;
  ROOT::Math::XYZTVector* p_jet4;
  ROOT::Math::XYZTVector jet5;
  ROOT::Math::XYZTVector* p_jet5;
  ROOT::Math::XYZTVector jet6;
  ROOT::Math::XYZTVector* p_jet6;
  
  float jet1_bTag;
  float jet2_bTag;
  float jet3_bTag;
  float jet4_bTag;
  float jet5_bTag;
  float jet6_bTag;
  
  
  
  // W-jet variables
  ROOT::Math::XYZTVector* p_WJ1;
  ROOT::Math::XYZTVector WJ1;
  ROOT::Math::XYZTVector* p_WJ2;
  ROOT::Math::XYZTVector WJ2;

  ROOT::Math::XYZTVector* p_WJ1_KF;
  ROOT::Math::XYZTVector WJ1_KF;
  ROOT::Math::XYZTVector* p_WJ1_KF1;
  ROOT::Math::XYZTVector WJ1_KF1;
  ROOT::Math::XYZTVector* p_WJ1_KF2;
  ROOT::Math::XYZTVector WJ1_KF2;
  ROOT::Math::XYZTVector* p_WJ2_KF;
  ROOT::Math::XYZTVector WJ2_KF;
  ROOT::Math::XYZTVector* p_WJ2_KF1;
  ROOT::Math::XYZTVector WJ2_KF1;
  ROOT::Math::XYZTVector* p_WJ2_KF2;
  ROOT::Math::XYZTVector WJ2_KF2;
  
  ROOT::Math::XYZTVector* p_WJJ;
  ROOT::Math::XYZTVector WJJ;
  
  float WJ1_pt;
  float WJ2_pt;
  float WJ1_eta;
  float WJ2_eta;
  float WJ1_phi;
  float WJ2_phi;
  float WJ1_zepp;
  float WJ2_zepp;
  float WJ1_lep_Dphi;
  float WJ1_met_Dphi;
  float WJ2_lep_Dphi;
  float WJ2_met_Dphi;
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
  
  int WJ1_mcMatched;
  int WJ2_mcMatched;
  float WJ1_mcMatchDR;
  float WJ2_mcMatchDR;
  
  float WJJ_Deta;
  float WJJ_Dphi;
  float WJJ_DR;
  float WJJ_pt;
  float WJJ_eta;
  float WJJ_phi;
  float WJJ_m;
  float WJJ_zepp;
  
  float lepWJJ_pt1;  
  float lepWJJ_pt2;  
  float lepWJJ_pt3;  
  
  
  
  // Higgs variables
  float lepW_pt;
  
  float lepMetW_pt;
  float lepMetW_phi;
  float lepMetW_mt;
  float lepMetW_Dphi;
  float lepNu_m;
  float lepNu_m_KF;
  float lepNu_m_KF1;
  float lepNu_m_KF2;
  int lepNu_nSolutions;
  int lepNu_nSolutions_KF;
  float lepNuW_eta;
  float lepNuW_m;
  float lepNuW_m1;
  float lepNuW_m2;
  float lepNuW_zepp;
  
  float lepNuW_m_KF;
  float lepNuW_m_KF1;
  float lepNuW_m_KF2;
  float chi2_KF;
  float chi2_KF1;
  float chi2_KF2;
  int ndf_KF;
  int ndf_KF1;
  int ndf_KF2;
  int status_KF;
  int status_KF1;
  int status_KF2;
  
  float lepNuW_cphi;
  float lepNuZ_cphi;
  float lep_ctheta;
  float WJ1_ctheta;
  float lepNu_ctheta;
  
  float helicityLikelihood;
  
  
  
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
  
  
  
  // MC variables
  ROOT::Math::XYZTVector mc_lep;
  ROOT::Math::XYZTVector* p_mc_lep;
  
  float mc_lep_charge;
  int mc_lep_flavour;
  float mc_lep_pt;
  float mc_lep_eta;
  float mc_lep_phi;
  
  
  
  ROOT::Math::XYZTVector mc_nu;
  ROOT::Math::XYZTVector* p_mc_nu;
  
  float mc_nu_charge;
  int mc_nu_flavour;
  float mc_nu_pt;
  float mc_nu_eta;
  float mc_nu_phi;
  
  
  
  float mc_lepNu_pt;
  float mc_lepNu_mt;
  float mc_lepNu_Dphi;
  
  
  
  ROOT::Math::XYZTVector mc_WJ1;
  ROOT::Math::XYZTVector* p_mc_WJ1;
  ROOT::Math::XYZTVector mc_WJ2;
  ROOT::Math::XYZTVector* p_mc_WJ2;
  
  float mc_WJ1_pt;
  float mc_WJ2_pt;
  float mc_WJ1_eta;
  float mc_WJ2_eta;
  float mc_WJ1_phi;
  float mc_WJ2_phi;
  
  float mc_WJJ_Deta;
  float mc_WJJ_Dphi;
  float mc_WJJ_DR;
  float mc_WJJ_pt;
  float mc_WJJ_m;
  
  
  
  ROOT::Math::XYZTVector mc_W1;
  ROOT::Math::XYZTVector* p_mc_W1;
  ROOT::Math::XYZTVector mc_W2;
  ROOT::Math::XYZTVector* p_mc_W2;
  
  float mc_W1_pt;
  float mc_W2_pt;
  float mc_W1_eta;
  float mc_W2_eta;
  float mc_W1_phi;
  float mc_W2_phi;
  float mc_W1_m;
  float mc_W2_m;
  
  
  
  ROOT::Math::XYZTVector mc_H;
  ROOT::Math::XYZTVector* p_mc_H;
  
  float mc_H_pt;
  float mc_H_eta;
  float mc_H_phi;
  float mc_H_m;
  
};



void SetVBFPreselectionTreeBranches(VBFAnalysisVariables&, TTree*);
void AddVBFAnalysisTreeBranches(VBFAnalysisVariables& vars, TTree*);
void InitializeVBFAnalysisTree(VBFAnalysisVariables& vars);

#endif
