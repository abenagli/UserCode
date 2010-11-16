

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
  
  
  
  // histograms
  hFactory* histograms;
  h2Factory* histograms2;
  stdHisto* stdHistograms;
  
  
  
  
  
  
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
  int lep_tracker;
  int lep_standalone;
  int lep_global;
  float lep_normalizedChi2;
  int lep_numberOfValidTrackerHits;
  int lep_numberOfValidMuonHits;
  
  
  
  // met variables
  ROOT::Math::XYZTVector* p_met;
  ROOT::Math::XYZTVector met;
  
  float lepMet_mt;
  float lepMet_Dphi;
  
  
  
  // jet variables
  int nJets;
  int nJets_cnt;
  int nJets_fwd;
  int nJets_btw;
  float jets_bTag1;
  float jets_bTag2;
  
  ROOT::Math::XYZTVector* p_leadingJ;
  ROOT::Math::XYZTVector leadingJ;
    
  float leadingJ_charge;
  float leadingJ_bTag;
  
  
  
  // W-jet variables
  ROOT::Math::XYZTVector* p_WJ1;
  ROOT::Math::XYZTVector WJ1;
  
  ROOT::Math::XYZTVector* p_WJ2;
  ROOT::Math::XYZTVector WJ2;
  
  float WJ1_charge;
  float WJ2_charge;
  float WJ1_bTag;
  float WJ2_bTag;  
  
  float lepWJJ_pt1;  
  float lepWJJ_pt2;  
  float lepWJJ_pt3;  
  
  
  
  // Higgs variables
  float lepMetW_mt;
  float lepMetW_Dphi;
  float lepNuW_m;
  
  
  
  // tag-jet variables
  ROOT::Math::XYZTVector* p_tagJ1;
  ROOT::Math::XYZTVector tagJ1;
  
  ROOT::Math::XYZTVector* p_tagJ2;
  ROOT::Math::XYZTVector tagJ2;
  
  float tagJ1_charge;
  float tagJ2_charge;
  float tagJ1_bTag;
  float tagJ2_bTag;  
};



void SetVBFPreselectionTreeBranches(VBFAnalysisVariables&, TTree*);
void SetVBFAnalysisHistograms(VBFAnalysisVariables&, const std::string&, const int&);

void InitialiseVBFAnalysisTree(VBFAnalysisVariables& vars, TFile* outputRootFile, const int& nStep, const int& firstStep);
void FillVBFAnalysisTree(VBFAnalysisVariables& vars, const int& step);

void DeleteVBFAnalysisVariables(VBFAnalysisVariables& vars, const int& nStep, const int& firstStep);

//void FillAllHistograms(VBFAnalysisVariables& vars, const int& step, const int& verbosity);
//void FillPVHistograms(VBFAnalysisVariables& vars, const int& step);
//void FillLeptonHistograms(VBFAnalysisVariables& vars, const int& step);
//void FillMetHistograms(VBFAnalysisVariables& vars, const int& step);
//void FillJetHistograms(VBFAnalysisVariables& vars, const int& step);
//void FillWJJHistograms(VBFAnalysisVariables& vars, const int& step);
//void FillHiggsHistograms(VBFAnalysisVariables& vars, const int& step);
//void FillTagJJHistograms(VBFAnalysisVariables& vars, const int& step);
//void FillMVAHistograms(VBFAnalysisVariables& vars, const int& step);
