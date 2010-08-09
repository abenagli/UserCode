#ifndef MCDecayModeFilter_h
#define MCDecayModeFilter_h

#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumperVBF.h"

#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
// #include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h" //---- CMSSW_2_X_Y
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h" //---- CMSSW_3_X_Y
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1F.h"

#include <string>






class MCDecayModeFilter : public edm::EDFilter
{
  
 public:
  
  //! ctor
  explicit MCDecayModeFilter(const edm::ParameterSet&);
  
  //! dtor 
  ~MCDecayModeFilter();
  
  
  
 private:
  
  //! the actual filter method 
  bool filter(edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcGenParticles;
  int m_eventType;
  
  
  bool m_H_W_W;
  bool m_H_Z_Z;
  
  
  
  bool m_V1_e_nue;
  bool m_V1_mu_numu;
  bool m_V1_tau_nutau;
  
  bool m_V1_d_u;
  bool m_V1_d_c;
  bool m_V1_s_u;
  bool m_V1_s_c;
  bool m_V1_b_u;
  bool m_V1_b_c;
  
  bool m_V1_e_e;
  bool m_V1_nue_nue;
  bool m_V1_mu_mu;
  bool m_V1_numu_numu;
  bool m_V1_tau_tau;
  bool m_V1_nutau_nutau;
  
  bool m_V1_d_d;
  bool m_V1_u_u;
  bool m_V1_s_s;
  bool m_V1_c_c;
  bool m_V1_b_b;
  
  
  
  bool m_V2_e_nue;
  bool m_V2_mu_numu;
  bool m_V2_tau_nutau;
  
  bool m_V2_d_u;
  bool m_V2_d_c;
  bool m_V2_s_u;
  bool m_V2_s_c;
  bool m_V2_b_u;
  bool m_V2_b_c;
  
  bool m_V2_e_e;
  bool m_V2_nue_nue;
  bool m_V2_mu_mu;
  bool m_V2_numu_numu;
  bool m_V2_tau_tau;
  bool m_V2_nutau_nutau;
  
  bool m_V2_d_d;
  bool m_V2_u_u;
  bool m_V2_s_s;
  bool m_V2_c_c;
  bool m_V2_b_b;
  
  
  
  TH1F* m_totalEvents;
  TH1F* m_passedEvents;
  TH1F* m_filterEfficiency;
  
 };

#endif
