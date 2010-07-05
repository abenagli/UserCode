#include "treeReader.h"
#include "ConfigParser.h"
#include "ntpleUtils.h"
#include "hFactory.h"
#include "h2Factory.h"
#include "stdHisto.h"

#include <iomanip>

#include "TH1F.h"
#include "TProfile.h"
#include "TObject.h"

#include "TMVA/Reader.h"


struct TTBarFullyLeptonicVariables
{
  hFactory* histograms;
  h2Factory* histograms2;
  stdHisto* stdHistograms;
  
  std::vector<int> selectIt_ele;
  std::vector<int> selectIt_mu;
  std::vector<int> selectIt_lep;
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
  std::vector<float> leptons_3DipSig;
  std::vector<int> electrons_isEB;
  std::vector<float> electrons_fbrem;
  std::vector<float> electrons_HoverE;
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
  
  std::vector<int> selectIt_tag;
  std::vector<int> selectIt_3rd;
  std::vector<ROOT::Math::XYZTVector> jets;
  int nJets;
  std::vector<float> jets_bTag;
  std::vector<float> jets_emEnergyFraction;
  std::vector<float> jets_etaEtaMoment;
  std::vector<float> jets_phiPhiMoment;
  std::vector<float> jets_etaPhiMoment;
  std::vector<float> jets_fHPD;
  std::vector<float> jets_fRBX;
  std::vector<float> jets_n90Hits;
  std::vector<float> jets_etOrdered;
  std::vector<float> jets_bTagOrdered;  
};



void InitializeTTBarFullyLeptonicVariables(TTBarFullyLeptonicVariables&, const std::string&, const int&);
void ClearTTBarFullyLeptonicVariables(TTBarFullyLeptonicVariables&);
void DeleteTTBarFullyLeptonicVariables(TTBarFullyLeptonicVariables&);

void FillLeptonHistograms(TTBarFullyLeptonicVariables& vars, const int& step);
void FillMetHistograms(TTBarFullyLeptonicVariables& vars, const int& step);
void FillJetHistograms(TTBarFullyLeptonicVariables& vars, const int& step);
void Fill3rdJetHistograms(TTBarFullyLeptonicVariables& vars, const int& step);

