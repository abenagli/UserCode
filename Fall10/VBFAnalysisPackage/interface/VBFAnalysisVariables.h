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
  
  
  
  // mva value
  double mva;
  
  
  
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
  
  float lep_charge;
  int lep_flavour;
  float lep_eta;
  float lep_zepp;
  float lep_dxy;
  float lep_dz;
  float lep_dB;
  float lep_edB;
  float lep_tkIso;
  float lep_emIso;
  float lep_hadIso;
  int lep_isEB;
  float lep_simpleEleId80cIso;
  float lep_sigmaIetaIeta;
  float lep_DphiIn;
  float lep_DetaIn;
  float lep_HOverE;
  int lep_tracker;
  int lep_standalone;
  int lep_global;
  float lep_normalizedChi2;
  int lep_numberOfValidTrackerHits;
  int lep_numberOfValidMuonHits;
  
  
  
  // met variables
  ROOT::Math::XYZTVector* p_met;
  ROOT::Math::XYZTVector met;
  float met_et;
  
  float lepMet_mt;
  float lepMet_Dphi;
  
  
  
  // jet variables
  int nJets;
  int nJets_cnt;
  int nJets_fwd;
  int nJets_et20;
  int nJets_cnt_et20;
  int nJets_fwd_et20;
  int nJets_btw_et20;
  int nJets_et25;
  int nJets_cnt_et25;
  int nJets_fwd_et25;
  int nJets_btw_et25;
  int nJets_et30;
  int nJets_cnt_et30;
  int nJets_fwd_et30;
  int nJets_btw_et30;

  float jets_bTag1;
  float jets_bTag2;
  
  ROOT::Math::XYZTVector* p_leadingJ;
  ROOT::Math::XYZTVector leadingJ;
    
  float leadingJ_charge;
  float leadingJ_bTag;
  float leadingJ_dzAvg;
  float leadingJ_dzAvgCut;
  int leadingJ_chargedMultiplicity;
  
  
  
  // W-jet variables
  ROOT::Math::XYZTVector* p_WJ1;
  ROOT::Math::XYZTVector WJ1;
  
  ROOT::Math::XYZTVector* p_WJ2;
  ROOT::Math::XYZTVector WJ2;
  
  ROOT::Math::XYZTVector* p_WJJ;
  ROOT::Math::XYZTVector WJJ;
  
  float WJ1_charge;
  float WJ2_charge;
  float WJ1_zepp;
  float WJ2_zepp;
  float WJ1_bTag;
  float WJ2_bTag;
  float WJ1_dzAvg;
  float WJ2_dzAvg;
  float WJ1_dzAvgCut;
  float WJ2_dzAvgCut;
  int WJ1_chargedMultiplicity;
  int WJ2_chargedMultiplicity;
  
  float WJJ_Deta;
  float WJJ_Dphi;
  float WJJ_DR;
  float WJJ_m;
  float WJJ_zepp;
  
  float lepWJJ_pt1;  
  float lepWJJ_pt2;  
  float lepWJJ_pt3;  
  
  
  
  // Higgs variables
  float lepMetW_mt;
  float lepMetW_Dphi;
  float lepNuW_m;
  float lepNuW_zepp;
  
  
  
  // tag-jet variables
  ROOT::Math::XYZTVector* p_tagJ1;
  ROOT::Math::XYZTVector tagJ1;
  
  ROOT::Math::XYZTVector* p_tagJ2;
  ROOT::Math::XYZTVector tagJ2;
  
  float tagJ1_charge;
  float tagJ2_charge;
  float tagJ1_bTag;
  float tagJ2_bTag;
  float tagJ1_dzAvg;
  float tagJ2_dzAvg;
  float tagJ1_dzAvgCut;
  float tagJ2_dzAvgCut;
  int tagJ1_chargedMultiplicity;
  int tagJ2_chargedMultiplicity;
  
  float tagJJ_Deta;
  float tagJJ_Dphi;
  float tagJJ_DR;
  float tagJJ_m;
  
  
  
  // third jet variables
  ROOT::Math::XYZTVector thirdJ;
  ROOT::Math::XYZTVector* p_thirdJ;
  
  float thirdJ_charge;
  float thirdJ_bTag;
  float thirdJ_dzAvg;
  float thirdJ_dzAvgCut;
  float thirdJ_WJJDeta;
  float thirdJ_WJJDphi;
  float thirdJ_WJJDR;
  float thirdJ_tagJJDeta;
  float thirdJ_tagJJDphi;
  float thirdJ_tagJJDR;
};



void SetVBFPreselectionTreeBranches(VBFAnalysisVariables&, TTree*);
void InitialiseVBFAnalysisTree(VBFAnalysisVariables& vars, TFile* outputRootFile, const int& nStep, const int& firstStep);
void FillVBFAnalysisTree(VBFAnalysisVariables& vars, const int& step);
