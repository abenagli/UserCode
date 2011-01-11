#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include "TH1F.h"
#include "TProfile.h"
#include "TObject.h"
#include "TTree.h"



struct VBFPreselectionVariables
{
  // tree definition
  TFile* m_outputRootFile;
  TTree* m_reducedTree;
  
  
  // input parameters
  float mH;
  float crossSection;
  int dataFlag; 
  int runId; 
  int lumiId; 
  int eventId; 
  
  
  // PV variables
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
  std::vector<float> leptons_dxy;
  std::vector<float> leptons_dz;
  std::vector<float> leptons_3DipSig;
  std::vector<float> leptons_tkIso;
  std::vector<float> leptons_emIso;
  std::vector<float> leptons_hadIso;
  std::vector<int> electrons_isEB;
  std::vector<float> electrons_sigmaIetaIeta;
  std::vector<float> electrons_DphiIn;
  std::vector<float> electrons_DetaIn;
  std::vector<float> electrons_HOverE;
  std::vector<float> electrons_fbrem;
  std::vector<float> electrons_EOverP;
  std::vector<int> muons_tracker;
  std::vector<int> muons_standalone;
  std::vector<int> muons_global;
  std::vector<float> muons_normalizedChi2;
  std::vector<int> muons_numberOfValidTrackerHits;
  std::vector<int> muons_numberOfValidMuonHits;
  
  int selectIt_lep;
  int selectIt_ele;
  int selectIt_mu;
  
  ROOT::Math::XYZTVector lep;
  ROOT::Math::XYZTVector* p_lep;
  
  float lep_charge;
  int lep_flavour;
  float lep_dxy;
  float lep_dz;
  float lep_3DipSig;
  float lep_tkIso;
  float lep_emIso;
  float lep_hadIso;
  int lep_isEB;
  float lep_sigmaIetaIeta;
  float lep_DphiIn;
  float lep_DetaIn;
  float lep_HOverE;
  float lep_fbrem;
  float lep_EOverP;
  int lep_tracker;
  int lep_standalone;
  int lep_global;
  float lep_normalizedChi2;
  int lep_numberOfValidTrackerHits;
  int lep_numberOfValidMuonHits;
  
  
  
  // met variables
  ROOT::Math::XYZTVector met;
  ROOT::Math::XYZTVector* p_met;
  
  ROOT::Math::XYZTVector lepMet;
  ROOT::Math::XYZTVector neutrino;
  
  float lepMet_mt;
  float lepMet_Dphi;
  
  
  
  // jet variables
  std::vector<ROOT::Math::XYZTVector> jets;
  std::vector<ROOT::Math::XYZTVector> jets_cnt;
  std::vector<ROOT::Math::XYZTVector> jets_fwd;
  std::vector<ROOT::Math::XYZTVector> jets_btw;
  
  std::vector<float> jets_charge;
  std::vector<float> jets_etaEtaMoment;
  std::vector<float> jets_phiPhiMoment;
  std::vector<float> jets_etaPhiMoment;
  std::vector<float> jets_emEnergyFraction;
  std::vector<float> jets_fHPD;
  std::vector<float> jets_fRBX;
  std::vector<float> jets_n90Hits;
  std::vector<float> jets_chargedHadronEnergyFraction;
  std::vector<float> jets_neutralHadronEnergyFraction;
  std::vector<float> jets_chargedEmEnergyFraction;
  std::vector<float> jets_neutralEmEnergyFraction;
  std::vector<float> jets_chargedMuEnergyFraction;
  std::vector<int> jets_chargedMultiplicity;
  std::vector<int> jets_neutralMultiplicity;
  std::vector<int> jets_muonMultiplicity;
  std::vector<float> jets_etOrdered;
  std::vector<float> jets_bTag;
  std::vector<float> jets_bTagOrdered;  
  
  int nJets;
  int nJets_cnt;
  int nJets_fwd;
  int nJets_btw;
  float jets_bTag1;
  float jets_bTag2;
  float HT;
  float HT_cnt;
  float HT_fwd;
  
  int selectIt_leadingJet;
  
  ROOT::Math::XYZTVector leadingJ;
  ROOT::Math::XYZTVector* p_leadingJ;
  
  float leadingJ_charge;
  float leadingJ_bTag;
  
  
  
  // W-jet variables
  std::vector<int> selectIt_W;
  
  ROOT::Math::XYZTVector WJ1;
  ROOT::Math::XYZTVector* p_WJ1;
  ROOT::Math::XYZTVector WJ2;
  ROOT::Math::XYZTVector* p_WJ2;
  
  ROOT::Math::XYZTVector WJ12;
  ROOT::Math::XYZTVector* p_WJ12;
  
  float WJ1_charge;
  float WJ2_charge;
  float WJ1_bTag;
  float WJ2_bTag;  
  
  std::vector<float> lepWJJ_ptOrdered;
  float lepWJJ_pt1;  
  float lepWJJ_pt2;  
  float lepWJJ_pt3;  
  
  
  
  // Higgs variables
  ROOT::Math::XYZTVector lepW;
  ROOT::Math::XYZTVector lepNuW;
  
  float lepMetW_mt;
  float lepMetW_Dphi;
  float lepNuW_m;
  
  
  
  // tag-jet variables
  std::vector<int> selectIt_tag;
  
  ROOT::Math::XYZTVector tagJ1;
  ROOT::Math::XYZTVector* p_tagJ1;
  ROOT::Math::XYZTVector tagJ2;
  ROOT::Math::XYZTVector* p_tagJ2;
  
  ROOT::Math::XYZTVector tagJ12;
  
  float tagJ1_charge;
  float tagJ2_charge;
  float tagJ1_bTag;
  float tagJ2_bTag;  
  
  
  
  // third jet variables
  int selectIt_thirdJet;
  
  ROOT::Math::XYZTVector thirdJ;
  ROOT::Math::XYZTVector* p_thirdJ;
  
  float thirdJ_charge;
  float thirdJ_bTag;
};



void InitializeVBFPreselectionTree(VBFPreselectionVariables&, const std::string&);
void FillVBFPreselectionTree(VBFPreselectionVariables& vars);

void ClearVBFPreselectionVariables(VBFPreselectionVariables&);
void DeleteVBFPreselectionVariables(VBFPreselectionVariables&);

void SetPVVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetElectronVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& eleIt);
void SetMuonVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& eleIt);
void SetLeptonVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetMetVariables(VBFPreselectionVariables& vars, treeReader& reader, const std::string& jetType);
void SetJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const int& jetIt, const std::string& jetType, const float& jetEtaCNT, const float& jetEtaFWD);
void SetLeadingJetVariables(VBFPreselectionVariables& vars, treeReader& reader, const float& jetEtaCNT);
void SetWJJVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetTagJJVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetThirdJetVariables(VBFPreselectionVariables& vars, treeReader& reader);
void SetHVariables(VBFPreselectionVariables& vars, treeReader& reader);
