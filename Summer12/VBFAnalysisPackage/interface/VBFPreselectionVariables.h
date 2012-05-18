#ifndef VBFPreselectionVariables_h
#define VBFPreselectionVariables_h

#include "treeReader.h"
#include "ntpleUtils.h"
#include "analysisUtils.h"

#include "TTree.h"
#include "TH2F.h"



struct VBFPreselectionVariables
{
  // tree definition
  TFile* m_outputRootFile;
  TTree* m_reducedTree;
  
  
  
  // input parameters
  float mH;
  int dataFlag;
  int MCFlag;
  int totEvents;
  float crossSection;
  int TMVA4Jet;
  int runId; 
  int lumiId; 
  Long64_t eventId; 
  int eventNaiveId; 
  
  float eventWeight;
  
  
  
  // mva variables  
  float mva;
  
  
  
  // MC variables
  float genPtHat;
  float genWeight;
  int genNUP;
  
  
  
  // PU variables
  int PUtrue_n;
  int PUit_n;
  int PUoot_early_n;
  int PUoot_late_n;
  float rhoForIsolation;
  float rhoForIsolationPFlow;
  float rhoForJets;
  float rhoForJetsPFlow;
  
  
  
  // HLT variables
  std::vector<std::string> HLT_Names;
  std::vector<std::string>* p_HLT_Names;
  std::vector<float> HLT_Accept;
  std::vector<float>* p_HLT_Accept;
  
  
  
  // PDF variables
  std::vector<float> PDF_weights_CT10 ;
  std::vector<float>* p_PDF_weights_CT10 ;

  std::vector<float> PDF_weights_MSTW2008nlo68cl ;
  std::vector<float>* p_PDF_weights_MSTW2008nlo68cl ;

  std::vector<float> PDF_weights_NNPDF20 ;
  std::vector<float>* p_PDF_weights_NNPDF20 ;
  
  
  
  // PV variables
  int PV_n;
  float PV_d0;
  float PV_z;
  int PV_nTracks;
  int PV_ndof;
  float PV_normalizedChi2;
  
  
  
  // lepton variables
  std::vector<ROOT::Math::XYZTVector> leptons;
  std::vector<ROOT::Math::XYZTVector> electrons;
  std::vector<ROOT::Math::XYZTVector> electrons_loose;
  std::vector<ROOT::Math::XYZTVector> muons;
  std::vector<ROOT::Math::XYZTVector> muons_loose;
  std::vector<float> leptonCharges;
  std::vector<std::string> leptonFlavours;    
  std::vector<float> leptons_z;
  std::vector<float> leptons_dxy_BS;
  std::vector<float> leptons_dz_BS;
  std::vector<float> leptons_dB;
  std::vector<float> leptons_edB;
  std::vector<float> leptons_dz_PV;
  std::vector<float> leptons_tkIso;
  std::vector<float> leptons_emIso;
  std::vector<float> leptons_hadIso;
  std::vector<float> leptons_pfChIso;
  std::vector<float> leptons_pfNhIso;
  std::vector<float> leptons_pfPhIso;
  std::vector<int> electrons_isEB;
  std::vector<float> electrons_etaSC;
  std::vector<float> electrons_sigmaIetaIeta;
  std::vector<float> electrons_DphiIn;
  std::vector<float> electrons_DetaIn;
  std::vector<float> electrons_HOverE;
  std::vector<float> electrons_fbrem;
  std::vector<float> electrons_EOverP;
  std::vector<int> electrons_mishits;
  std::vector<float> electrons_dist;
  std::vector<float> electrons_dcot;
  std::vector<int> muons_tracker;
  std::vector<int> muons_standalone;
  std::vector<int> muons_global;
  std::vector<float> muons_normalizedChi2;
  std::vector<int> muons_numberOfMatches;
  std::vector<int> muons_numberOfValidTrackerHits;
  std::vector<int> muons_numberOfValidMuonHits;
  std::vector<int> muons_pixelLayersWithMeasurement;
  
  int selectIt_lep;
  int selectIt_ele;
  int selectIt_mu;
  
  int selectIt_lep2;
  int selectIt_ele2;
  int selectIt_mu2;
  
  ROOT::Math::XYZTVector lep;
  ROOT::Math::XYZTVector* p_lep;
  
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
  int lep_isEB;
  float lep_etaSC;
  float lep_sigmaIetaIeta;
  float lep_DphiIn;
  float lep_DetaIn;
  float lep_HOverE;
  float lep_fbrem;
  float lep_EOverP;
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
  int lep_hltMatched;
  float lep_hltMatchPt;  
  int lep_hltPass;
  
  
  ROOT::Math::XYZTVector lep2;
  ROOT::Math::XYZTVector* p_lep2;
  
  float lep2_charge;
  int lep2_flavour;
  float lep2_pt;
  float lep2_eta;
  float lep2_phi;
  float lep2_zepp;
  float lep2_z;
  float lep2_dxy_BS;
  float lep2_dz_BS;
  float lep2_dB;
  float lep2_edB;
  float lep2_dz_PV;
  float lep2_tkIso;
  float lep2_emIso;
  float lep2_hadIso;
  float lep2_pfChIso;
  float lep2_pfNhIso;
  float lep2_pfPhIso;
  int lep2_isEB;
  float lep2_etaSC;
  float lep2_sigmaIetaIeta;
  float lep2_DphiIn;
  float lep2_DetaIn;
  float lep2_HOverE;
  float lep2_fbrem;
  float lep2_EOverP;
  int lep2_mishits;
  float lep2_dist;
  float lep2_dcot;
  int lep2_tracker;
  int lep2_standalone;
  int lep2_global;
  float lep2_normalizedChi2;
  int lep2_numberOfMatches;
  int lep2_numberOfValidTrackerHits;
  int lep2_numberOfValidMuonHits;
  int lep2_pixelLayersWithMeasurement;
  
  int lep2_mcMatched;
  float lep2_mcMatchDR;
  int lep2_hltMatched;
  float lep2_hltMatchPt;
  int lep2_hltPass;
  
  
  // met variables
  ROOT::Math::XYZTVector met;
  ROOT::Math::XYZTVector* p_met;
  ROOT::Math::XYZTVector nu;
  ROOT::Math::XYZTVector* p_nu;
  ROOT::Math::XYZTVector nu1;
  ROOT::Math::XYZTVector* p_nu1;
  ROOT::Math::XYZTVector nu2;
  ROOT::Math::XYZTVector* p_nu2;
  
  float met_et;
  float met_phi;
  float sumEt;
  
  int met_mcMatched;
  float met_mcMatchDR;
  
  ROOT::Math::XYZTVector lepMet;
  
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
  std::vector<ROOT::Math::XYZTVector> jets;
  
  std::vector<float> jets_charge;
  std::vector<float> jets_etaEtaMoment;
  std::vector<float> jets_phiPhiMoment;
  std::vector<float> jets_etaPhiMoment;
  std::vector<float> jets_emEnergyFraction;
  std::vector<float> jets_fHPD;
  std::vector<float> jets_fRBX;
  std::vector<float> jets_n90Hits;
  std::vector<float> jets_ptD;
  std::vector<float> jets_chargedHadronEnergyFraction;
  std::vector<float> jets_neutralHadronEnergyFraction;
  std::vector<float> jets_chargedEmEnergyFraction;
  std::vector<float> jets_neutralEmEnergyFraction;
  std::vector<float> jets_muonEnergyFraction;
  std::vector<int> jets_chargedMultiplicity;
  std::vector<int> jets_neutralMultiplicity;
  std::vector<int> jets_muonMultiplicity;
  std::vector<float> jets_ptOrdered;
  std::vector<float> jets_bTag;
  std::vector<float> jets_bTagOrdered;  
  
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
  
  
  int selectIt_leadingJet;
  
  ROOT::Math::XYZTVector leadingJ;
  ROOT::Math::XYZTVector* p_leadingJ;
  
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
  
  int jet1_hltMatched;
  int jet2_hltMatched;
  int jet3_hltMatched;
  int jet4_hltMatched;
  int jet5_hltMatched;
  int jet6_hltMatched;
  float jet1_hltMatchPt;
  float jet2_hltMatchPt;
  float jet3_hltMatchPt;
  float jet4_hltMatchPt;
  float jet5_hltMatchPt;
  float jet6_hltMatchPt;
  
  
  // W-jet variables
  std::vector<int> selectIt_W;
  
  ROOT::Math::XYZTVector WJ1;
  ROOT::Math::XYZTVector* p_WJ1;
  ROOT::Math::XYZTVector WJ2;
  ROOT::Math::XYZTVector* p_WJ2;
  
  ROOT::Math::XYZTVector WJJ;
  ROOT::Math::XYZTVector* p_WJJ;
  
  float WJ1_pt;
  float WJ2_pt;
  float WJ1_eta;
  float WJ2_eta;
  float WJ1_phi;
  float WJ2_phi;
  float WJ1_zepp;
  float WJ2_zepp;
  float WJ1_lep_Dphi;
  float WJ2_lep_Dphi;
  float WJ1_met_Dphi;
  float WJ2_met_Dphi;
  float WJ1_bTag;
  float WJ2_bTag;
  float WJ1_ptD;
  float WJ2_ptD;
  int WJ1_chargedMultiplicity;
  int WJ2_chargedMultiplicity;
  int WJ1_neutralMultiplicity;
  int WJ2_neutralMultiplicity;
  
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
  
  std::vector<float> lepWJJ_ptOrdered;
  float lepWJJ_pt1;  
  float lepWJJ_pt2;  
  float lepWJJ_pt3;  
  
  
  
  // Higgs variables
  ROOT::Math::XYZTVector lepW;
  ROOT::Math::XYZTVector lepNuW;
  ROOT::Math::XYZTVector lepNuW1;
  ROOT::Math::XYZTVector lepNuW2;
  
  float lepMetW_pt;
  float lepMetW_phi;
  float lepMetW_mt;
  float lepMetW_Dphi;
  float lepNu_m;
  int lepNu_nSolutions;
  float lepW_pt;
  float lepNuW_eta;
  float lepNuW_m;
  float lepNuW_m1;
  float lepNuW_m2;
  float lepNuW_zepp;
  
  
  
  // tag-jet variables
  std::vector<int> selectIt_tag;
  
  ROOT::Math::XYZTVector tagJ1;
  ROOT::Math::XYZTVector* p_tagJ1;
  ROOT::Math::XYZTVector tagJ2;
  ROOT::Math::XYZTVector* p_tagJ2;
  
  ROOT::Math::XYZTVector tagJJ;
  
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
  int selectIt_thirdJet;
  
  ROOT::Math::XYZTVector thirdJ;
  ROOT::Math::XYZTVector* p_thirdJ;
  
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



TFile* GetOutputRootFile(VBFPreselectionVariables& vars);
void InitializeVBFPreselectionTree(VBFPreselectionVariables&, const std::string&, const int& doTnP = false);
void FillVBFPreselectionTree(VBFPreselectionVariables& vars);

void ClearVBFPreselectionVariables(VBFPreselectionVariables&);
void DeleteVBFPreselectionVariables(VBFPreselectionVariables&);

void SetGenVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& dataFlag, const int& verbosity = 0);
void SetPUVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& dataFlag, const int& verbosity = 0);
void SetHLTVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);
void SetPDFVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);
void SetPVVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);
void SetElectronVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& eleIt, const int& verbosity = 0);
void SetMuonVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& eleIt, const int& verbosity = 0);
void SetLeptonVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);
void SetLepton2Variables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);
void SetMetVariables(VBFPreselectionVariables& vars, treeReader& reader, const std::string& jetType,
                     const int& correctMet, const std::string& run, const float& JESScaleVariation, TH2F* JECUncertainty, const int& verbosity = 0);
void SetBTagVariables(VBFPreselectionVariables& vars, treeReader& reader, const std::string& jetType, const float& jetEtaCNT, const int& verbosity = 0);
void SetJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& jetIt, const std::string& jetType, const float& jetEtaCNT, const float& jetEtaFWD,
                     const float& JESScaleVariation, TH2F* JECUncertainty, const int& verbosity = 0);
void SetLeadingJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const float& jetEtaCNT, const int& verbosity = 0);
void SetWJJVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);
void SetTagJJVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);
void SetThirdJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);
void SetHVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);
void SetMCVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& verbosity = 0);

#endif
