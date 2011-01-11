#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>
#include <map>

#include "TH1F.h"
#include "TProfile.h"
#include "TObject.h"
#include "TTree.h"

#include "TMVA/Reader.h"


struct WplusWminusRatioVariables
{
  //-----------------------
  // TREE VARIABLES
  //-----------------------
  
  TFile* m_outputTreeFile;
  std::map<int, TTree*> m_tree;
  std::map<int, TTree*> m_treeEvents;
  
  float mH;
  int totEvents;
  float crossSection;
  int dataFlag; 
  
  double mva;
    
  float lep_pt;
  float lep_eta;
  float lep_phi;
  float lep_charge;
  float lep_tkIso;
  float lep_emIso;
  float lep_hadIso;
  
  float met_et;
  float lepMet_mt;
  float lepMet_Dphi;
  float lepMetW_mt;
  float lepMetW_Dphi;
  float lepNuW_m;
  
  float WJJ_Deta;
  float WJJ_Dphi;
  float WJJ_DR;
  float WJJ_etaProd;
  float WJJ_m;
  float WJJ_et;
  float WJ1_e;
  float WJ2_e;
  float WJ1_et;
  float WJ2_et;
  float WJ1_eta;
  float WJ2_eta;
  float WJ1_phi;
  float WJ2_phi;
  float WJ1_charge;
  float WJ2_charge;
  float WJ1_bTag;
  float WJ2_bTag;
  
  float tagJJ_Deta;
  float tagJJ_Dphi;
  float tagJJ_DR;
  float tagJJ_etaProd;
  float tagJJ_m;
  float tagJJ_et;
  float tagJ1_e;
  float tagJ2_e;
  float tagJ1_et;
  float tagJ2_et;
  float tagJ1_eta;
  float tagJ2_eta;
  float tagJ1_phi;
  float tagJ2_phi;
  float tagJ1_charge;
  float tagJ2_charge;
  float tagJ1_bTag;
  float tagJ2_bTag;
  
  float lepWJJ_pt1;  
  float lepWJJ_pt2;  
  float lepWJJ_pt3;  
      
  
  
  
  
  //-----------------------
  // HISTOGRAMS
  //-----------------------
  
  hFactory* histograms;
  h2Factory* histograms2;
  stdHisto* stdHistograms;
  
  TProfile* efficiency_W;
  TProfile* efficiency_purity_2_W;
  TProfile* purity_0_W;
  TProfile* purity_1_W;
  TProfile* purity_2_W;
  
  TProfile* efficiency_tag;
  TProfile* efficiency_purity_2_tag;
  TProfile* purity_0_tag;
  TProfile* purity_1_tag;
  TProfile* purity_2_tag;

  TProfile* efficiency;
  TProfile* efficiency_purity_4;
  TProfile* purity_0;
  TProfile* purity_1;
  TProfile* purity_2;
  TProfile* purity_3;
  TProfile* purity_4;
  
  float PV_d0;
  int PV_nTracks;
  int PV_ndof;
  float PV_normalizedChi2;
  float PV_z;
  
  int selectIt_ele;
  int selectIt_mu;
  int selectIt_lep;
  ROOT::Math::XYZTVector lepton;
  std::vector<ROOT::Math::XYZTVector> electrons;
  std::vector<ROOT::Math::XYZTVector> muons;
  std::vector<ROOT::Math::XYZTVector> leptons;
  std::vector<float> leptonCharges;
  std::vector<std::string> leptonFlavours;    
  std::vector<float> leptons_tkIso;
  std::vector<float> leptons_emIso;
  std::vector<float> leptons_hadIso;
  std::vector<float> leptons_dxy;
  std::vector<float> leptons_dz;
  std::vector<float> leptons_lipSig;
  std::vector<float> leptons_tipSig;
  std::vector<float> leptons_3DipSig;
  std::vector<int> electrons_isEB;
  std::vector<float> electrons_fbrem;
  std::vector<float> electrons_HoverE;
  std::vector<float> electrons_eOverP;
  std::vector<float> electrons_DphiIn;
  std::vector<float> electrons_DetaIn;
  std::vector<float> electrons_sigmaIetaIeta;
  std::vector<int> muons_tracker;
  std::vector<int> muons_standalone;
  std::vector<int> muons_global;
  std::vector<float> muons_normalizedChi2;
  std::vector<int> muons_numberOfValidTrackerHits;
  std::vector<int> muons_numberOfValidMuonHits;
  
  ROOT::Math::XYZTVector met;
  ROOT::Math::XYZTVector caloMet;
  ROOT::Math::XYZTVector type1Met;
  ROOT::Math::XYZTVector pfMet;
  ROOT::Math::XYZTVector genMet;
  ROOT::Math::XYZTVector lepMet;
  
  ROOT::Math::XYZTVector neutrino;
  
  std::vector<ROOT::Math::XYZTVector> jets;
  std::vector<ROOT::Math::XYZTVector> jets_cnt;
  std::vector<ROOT::Math::XYZTVector> jets_fwd;
  std::vector<ROOT::Math::XYZTVector> jets_btw;
  int nJets;
  int nJets_cnt;
  int nJets_fwd;
  int nJets_btw;
  
  int selectIt_leadingJet;
  std::vector<int> selectIt_W;
  std::vector<int> selectIt_tag;
  std::vector<int> matchIt;
  std::vector<int> matchIt_W;
  std::vector<int> matchIt_tag;
  
  std::vector<float> jets_bTag;
  std::vector<float> jets_emEnergyFraction;
  
  std::vector<float> jets_charge;
  std::vector<float> jets_etaEtaMoment;
  std::vector<float> jets_phiPhiMoment;
  std::vector<float> jets_etaPhiMoment;
  
  std::vector<float> jets_fHPD;
  std::vector<float> jets_fRBX;
  std::vector<float> jets_n90Hits;
  
  std::vector<float> jets_chargedHadronEnergy;
  std::vector<float> jets_chargedHadronEnergyFraction;
  std::vector<float> jets_neutralHadronEnergy;
  std::vector<float> jets_neutralHadronEnergyFraction;
  std::vector<float> jets_chargedEmEnergy;
  std::vector<float> jets_chargedEmEnergyFraction;
  std::vector<float> jets_neutralEmEnergy;
  std::vector<float> jets_neutralEmEnergyFraction;
  std::vector<float> jets_chargedMuEnergy;
  std::vector<float> jets_chargedMuEnergyFraction;
  std::vector<int> jets_chargedMultiplicity;
  std::vector<int> jets_neutralMultiplicity;
  std::vector<int> jets_muonMultiplicity;

  std::vector<float> jets_etOrdered;
  std::vector<float> jets_bTagOrdered;  
  
  ROOT::Math::XYZTVector jet1_W;
  ROOT::Math::XYZTVector jet2_W;
  ROOT::Math::XYZTVector jet12_W;
  ROOT::Math::XYZTVector lepW;
  std::vector<float> lepWJJ_ptOrdered;
  ROOT::Math::XYZTVector lepNuW;
  
  ROOT::Math::XYZTVector jet1_tag;
  ROOT::Math::XYZTVector jet2_tag;
  ROOT::Math::XYZTVector jet12_tag;
};



void InitializeWplusWminusRatioVariables(WplusWminusRatioVariables&, const std::string&, const std::string&, const int&);
void ClearWplusWminusRatioVariables(WplusWminusRatioVariables&);
void DeleteWplusWminusRatioVariables(WplusWminusRatioVariables&, const int&);

void SetPVVariables(WplusWminusRatioVariables& vars, treeReader& reader);
void SetElectronVariables(WplusWminusRatioVariables& vars, treeReader& reader, const int& eleIt);
void SetMuonVariables(WplusWminusRatioVariables& vars, treeReader& reader, const int& eleIt);
void SetLeptonVariables(WplusWminusRatioVariables& vars, treeReader& reader);
void SetMetVariables(WplusWminusRatioVariables& vars, treeReader& reader, const std::string& jetType);
void SetJetVariables(WplusWminusRatioVariables& vars, treeReader& reader, const int& jetIt, const std::string& jetType, const float& jetEtaCNT, const float& jetEtaFWD);
void SetWJJVariables(WplusWminusRatioVariables& vars, treeReader& reader);
void SetTagJJVariables(WplusWminusRatioVariables& vars, treeReader& reader);
void SetHVariables(WplusWminusRatioVariables& vars, treeReader& reader);

void FillTree(WplusWminusRatioVariables& vars, const int& step);
void FillPVHistograms(WplusWminusRatioVariables& vars, const int& step);
void FillLeptonHistograms(WplusWminusRatioVariables& vars, const int& step);
void FillMetHistograms(WplusWminusRatioVariables& vars, const int& step);
void FillJetHistograms(WplusWminusRatioVariables& vars, const int& step, const std::string& jetType, const float& jetEtaCNT, const float& jetEtaFWD);
void FillWJJHistograms(WplusWminusRatioVariables& vars, const int& step, const std::string& jetType);
void FillTagJJHistograms(WplusWminusRatioVariables& vars, const int& step, const std::string& jetType);
void FillMVAHistograms(WplusWminusRatioVariables& vars, const int& step);

void FillPurityProfiles(WplusWminusRatioVariables& vars, const int& step);
