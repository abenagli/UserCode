#include "HiggsAnalysis/VBFHiggsToVV/plugins/MCDecayModeFilter.h"






//! ctor
MCDecayModeFilter::MCDecayModeFilter(const edm::ParameterSet& iConfig): 
 m_srcGenParticles(iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 m_eventType(iConfig.getUntrackedParameter<int>("eventType", 0)),
 m_H_W_W(iConfig.getParameter<bool>("H_W_W")),
 m_H_Z_Z(iConfig.getParameter<bool>("H_Z_Z")),
 m_V1_e_nue    (iConfig.getParameter<bool>("V1_e_nue")),
 m_V1_mu_numu  (iConfig.getParameter<bool>("V1_mu_numu")),
 m_V1_tau_nutau(iConfig.getParameter<bool>("V1_tau_nutau")),
 m_V1_d_u(iConfig.getParameter<bool>("V1_d_u")),
 m_V1_d_c(iConfig.getParameter<bool>("V1_d_c")),
 m_V1_s_u(iConfig.getParameter<bool>("V1_s_u")),
 m_V1_s_c(iConfig.getParameter<bool>("V1_s_c")),
 m_V1_b_u(iConfig.getParameter<bool>("V1_b_u")),
 m_V1_b_c(iConfig.getParameter<bool>("V1_b_c")),
 m_V1_e_e        (iConfig.getParameter<bool>("V1_e_e")),
 m_V1_nue_nue    (iConfig.getParameter<bool>("V1_nue_nue")),
 m_V1_mu_mu      (iConfig.getParameter<bool>("V1_mu_mu")),
 m_V1_numu_numu  (iConfig.getParameter<bool>("V1_numu_numu")),
 m_V1_tau_tau    (iConfig.getParameter<bool>("V1_tau_tau")),
 m_V1_nutau_nutau(iConfig.getParameter<bool>("V1_nutau_nutau")),
 m_V1_d_d(iConfig.getParameter<bool>("V1_d_d")),
 m_V1_u_u(iConfig.getParameter<bool>("V1_u_u")),
 m_V1_s_s(iConfig.getParameter<bool>("V1_s_s")),
 m_V1_c_c(iConfig.getParameter<bool>("V1_c_c")),
 m_V1_b_b(iConfig.getParameter<bool>("V1_b_b")),
 m_V2_e_nue    (iConfig.getParameter<bool>("V2_e_nue")),
 m_V2_mu_numu  (iConfig.getParameter<bool>("V2_mu_numu")),
 m_V2_tau_nutau(iConfig.getParameter<bool>("V2_tau_nutau")),
 m_V2_d_u(iConfig.getParameter<bool>("V2_d_u")),
 m_V2_d_c(iConfig.getParameter<bool>("V2_d_c")),
 m_V2_s_u(iConfig.getParameter<bool>("V2_s_u")),
 m_V2_s_c(iConfig.getParameter<bool>("V2_s_c")),
 m_V2_b_u(iConfig.getParameter<bool>("V2_b_u")),
 m_V2_b_c(iConfig.getParameter<bool>("V2_b_c")),
 m_V2_e_e        (iConfig.getParameter<bool>("V2_e_e")),
 m_V2_nue_nue    (iConfig.getParameter<bool>("V2_nue_nue")),
 m_V2_mu_mu      (iConfig.getParameter<bool>("V2_mu_mu")),
 m_V2_numu_numu  (iConfig.getParameter<bool>("V2_numu_numu")),
 m_V2_tau_tau    (iConfig.getParameter<bool>("V2_tau_tau")),
 m_V2_nutau_nutau(iConfig.getParameter<bool>("V2_nutau_nutau")),
 m_V2_d_d(iConfig.getParameter<bool>("V2_d_d")),
 m_V2_u_u(iConfig.getParameter<bool>("V2_u_u")),
 m_V2_s_s(iConfig.getParameter<bool>("V2_s_s")),
 m_V2_c_c(iConfig.getParameter<bool>("V2_c_c")),
 m_V2_b_b(iConfig.getParameter<bool>("V2_b_b"))
{
  edm::Service<TFileService> fs;
  
  m_totalEvents = fs -> make<TH1F>("totalEvents", "totalEvents", 1,  0., 1.);
  m_passedEvents = fs -> make<TH1F>("passedEvents", "passedEvents", 1,  0., 1.);
  m_filterEfficiency = fs -> make<TH1F>("filterEfficiency", "filterEfficiency", 1,  0., 1.);
}

// ----------------------------------------------------------------






//! dtor
MCDecayModeFilter::~MCDecayModeFilter()
{}

// ----------------------------------------------------------------






//! loop over the reco particles and count leptons
bool MCDecayModeFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  
  
  
  // -----------------------------------------------------------------
  // Get the collections
  // -----------------------------------------------------------------  

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(m_srcGenParticles, genParticles);
  if( !(genParticles.isValid ()) )
  {
    std::cerr << ">>>>>> MCDecayModeFilter::Warning: " << m_srcGenParticles << " not available" << std::endl;
    return false;
  }
  
  
  
  
  
  
  // MCDumperVBF  
  double keepEvent = true;

  // if is signal
  if(m_eventType == 0)
  {
    MCDumperVBF mcAnalysis(genParticles, 0, false);
    bool isValid = mcAnalysis.isValid();
    if(!isValid) keepEvent = false;
    
    
    
    
    
    
    // -------------------------
    // Higgs decay
    // -------------------------
    
    if( (mcAnalysis.mcHDecayMode() == "W_W") && (m_H_W_W == false) ) keepEvent = false;
    if( (mcAnalysis.mcHDecayMode() == "Z_Z") && (m_H_Z_Z == false) ) keepEvent = false;
    
    
    
    // -------------------------
    // V1 decay - W modes
    // -------------------------
    
    if( (mcAnalysis.mcV1DecayMode() == "e_nue")     && (m_V1_e_nue == false) )     keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "mu_numu")   && (m_V1_mu_numu == false) )   keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "tau_nutau") && (m_V1_tau_nutau == false) ) keepEvent = false;
    
    if( (mcAnalysis.mcV1DecayMode() == "d_u") && (m_V1_d_u == false) ) keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "d_c") && (m_V1_d_c == false) ) keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "s_u") && (m_V1_s_u == false) ) keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "s_c") && (m_V1_s_c == false) ) keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "b_u") && (m_V1_b_u == false) ) keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "b_c") && (m_V1_b_c == false) ) keepEvent = false;
    
    // -------------------------
    // V1 decay - Z modes
    // -------------------------
    
    if( (mcAnalysis.mcV1DecayMode() == "e_e")         && (m_V1_e_e == false) )         keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "nue_nue")     && (m_V1_nue_nue == false) )     keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "mu_mu")       && (m_V1_mu_mu == false) )       keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "numu_numu")   && (m_V1_numu_numu == false) )   keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "tau_tau")     && (m_V1_tau_tau == false) )     keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "nutau_nutau") && (m_V1_nutau_nutau == false) ) keepEvent = false;
    
    if( (mcAnalysis.mcV1DecayMode() == "d_d") && (m_V1_d_d == false) ) keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "u_u") && (m_V1_u_u == false) ) keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "s_s") && (m_V1_s_s == false) ) keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "c_c") && (m_V1_c_c == false) ) keepEvent = false;
    if( (mcAnalysis.mcV1DecayMode() == "b_b") && (m_V1_b_b == false) ) keepEvent = false;
    
    
    
    // -------------------------
    // V2 decay - W modes
    // -------------------------
    
    if( (mcAnalysis.mcV2DecayMode() == "e_nue")     && (m_V2_e_nue == false) )     keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "mu_numu")   && (m_V2_mu_numu == false) )   keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "tau_nutau") && (m_V2_tau_nutau == false) ) keepEvent = false;
    
    if( (mcAnalysis.mcV2DecayMode() == "d_u") && (m_V2_d_u == false) ) keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "d_c") && (m_V2_d_c == false) ) keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "s_u") && (m_V2_s_u == false) ) keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "s_c") && (m_V2_s_c == false) ) keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "b_u") && (m_V2_b_u == false) ) keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "b_c") && (m_V2_b_c == false) ) keepEvent = false;
    
    // -------------------------
    // V2 decay - Z modes
    // -------------------------
    
    if( (mcAnalysis.mcV2DecayMode() == "e_e")         && (m_V2_e_e == false) )         keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "nue_nue")     && (m_V2_nue_nue == false) )     keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "mu_mu")       && (m_V2_mu_mu == false) )       keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "numu_numu")   && (m_V2_numu_numu == false) )   keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "tau_tau")     && (m_V2_tau_tau == false) )     keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "nutau_nutau") && (m_V2_nutau_nutau == false) ) keepEvent = false;
    
    if( (mcAnalysis.mcV2DecayMode() == "d_d") && (m_V2_d_d == false) ) keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "u_u") && (m_V2_u_u == false) ) keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "s_s") && (m_V2_s_s == false) ) keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "c_c") && (m_V2_c_c == false) ) keepEvent = false;
    if( (mcAnalysis.mcV2DecayMode() == "b_b") && (m_V2_b_b == false) ) keepEvent = false;
  } // if is signal
    
    
  
  
  
  
  int nTotalEvents = static_cast<int>(m_totalEvents -> GetBinContent(1));
  int nPassedEvents = static_cast<int>(m_passedEvents -> GetBinContent(1));

  if(keepEvent == true)
  {
    m_totalEvents -> Fill(0.5);
    m_passedEvents -> Fill(0.5);
    m_filterEfficiency -> SetBinContent(1, 1.*(nPassedEvents+1)/(nTotalEvents+1));
    
    return true;
  }

  else
  {
    m_totalEvents -> Fill(0.5);
    m_filterEfficiency -> SetBinContent(1, 1.*(nPassedEvents)/(nTotalEvents+1));
    
    return false;
  }

}
