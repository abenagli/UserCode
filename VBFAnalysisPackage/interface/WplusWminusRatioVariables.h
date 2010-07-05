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


struct WplusWminusRatioVariables
{
  hFactory* histograms;
  h2Factory* histograms2;
  stdHisto* stdHistograms;
  
  int selectIt_ele;
  int selectIt_mu;
  int selectIt_lep;
  ROOT::Math::XYZTVector lepton;
  float lepMet_mt;
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



void InitializeWplusWminusRatioVariables(WplusWminusRatioVariables&, const std::string&, const int&);
void ClearWplusWminusRatioVariables(WplusWminusRatioVariables&);
void DeleteWplusWminusRatioVariables(WplusWminusRatioVariables&);

void FillLeptonHistograms(WplusWminusRatioVariables& vars, const int& step);
void FillMetHistograms(WplusWminusRatioVariables& vars, const int& step);
void FillJetHistograms(WplusWminusRatioVariables& vars, const int& step);

