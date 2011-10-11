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
  int totEvents;
  float crossSection;
  int TMVA4Jet;
  int runId; 
  int lumiId; 
  int eventId; 
  int eventNaiveId; 
  
  float eventWeight;
  
  
  
  // mva variables  
  float mva;
  
  
  
  // PU variables
  int PUit_n;
  int PUoot_n;
  float rhoForIsolation;
  float rhoForJets;
  
  
  
  // HLT variables
  std::vector<std::string> HLT_Names;
  std::vector<std::string>* p_HLT_Names;
  std::vector<float> HLT_Accept;
  std::vector<float>* p_HLT_Accept;
  
  
  
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
  std::vector<float> leptons_dxy_PV;
  std::vector<float> leptons_edxy_PV;
  std::vector<float> leptons_dz_PV;
  std::vector<float> leptons_tkIso;
  std::vector<float> leptons_emIso;
  std::vector<float> leptons_hadIso;
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
  
  ROOT::Math::XYZTVector lep;
  ROOT::Math::XYZTVector* p_lep;
  
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
  
  
  
  // met variables
  ROOT::Math::XYZTVector met;
  ROOT::Math::XYZTVector* p_met;
  ROOT::Math::XYZTVector nu;
  ROOT::Math::XYZTVector* p_nu;
  
  float met_et;  
  
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
  
  
  
  // W-jet variables
  std::vector<int> selectIt_W;
  
  ROOT::Math::XYZTVector WJ1;
  ROOT::Math::XYZTVector* p_WJ1;
  ROOT::Math::XYZTVector WJ2;
  ROOT::Math::XYZTVector* p_WJ2;
  
  ROOT::Math::XYZTVector WJJ;
  ROOT::Math::XYZTVector* p_WJJ;
  
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
  
  float WJJ_Deta;
  float WJJ_Dphi;
  float WJJ_DR;
  float WJJ_pt;
  float WJJ_m;
  float WJJ_zepp;
  
  std::vector<float> lepWJJ_ptOrdered;
  float lepWJJ_pt1;  
  float lepWJJ_pt2;  
  float lepWJJ_pt3;  
  
  
  
  // Higgs variables
  ROOT::Math::XYZTVector lepW;
  ROOT::Math::XYZTVector lepNuW;
  
  float lepMetW_pt;
  float lepMetW_mt;
  float lepMetW_Dphi;
  float lepNu_m;
  int lepNu_nSolutions;
  float lepW_pt;
  float lepNuW_m;
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
};



TFile* GetOutputRootFile(VBFPreselectionVariables& vars);
void InitializeVBFPreselectionTree(VBFPreselectionVariables&, const std::string&);
void FillVBFPreselectionTree(VBFPreselectionVariables& vars);

void ClearVBFPreselectionVariables(VBFPreselectionVariables&);
void DeleteVBFPreselectionVariables(VBFPreselectionVariables&);

void SetPUVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& dataFlag);
void SetHLTVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetPVVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetElectronVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& eleIt);
void SetMuonVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& eleIt);
void SetLeptonVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetMetVariables(VBFPreselectionVariables& vars, treeReader& reader, const std::string& jetType,
		     const float& JESScaleVariation, TH2F* JECUncertainty);
void SetBTagVariables(VBFPreselectionVariables& vars, treeReader& reader, const std::string& jetType, const float& jetEtaCNT);
void SetJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& jetIt, const std::string& jetType, const float& jetEtaCNT, const float& jetEtaFWD,
                     const float& JESScaleVariation, TH2F* JECUncertainty);
void SetLeadingJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const float& jetEtaCNT);
void SetWJJVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetTagJJVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetThirdJetVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetHVariables(VBFPreselectionVariables& vars, treeReader& reader);

#endif
