import FWCore.ParameterSet.Config as cms



MCDecayModeFilter = cms.EDFilter(
    "MCDecayModeFilter",
    srcGenParticles = cms.InputTag("genParticles"),
    eventType = cms.untracked.int32(0),
    
    # Higgs decay modes
    H_W_W = cms.bool(False),
    H_Z_Z = cms.bool(False),


    
    # V1 decay modes - W modes
    V1_e_nue     = cms.bool(False),
    V1_mu_numu   = cms.bool(False),
    V1_tau_nutau = cms.bool(False),

    V1_d_u = cms.bool(False),
    V1_d_c = cms.bool(False),
    V1_s_u = cms.bool(False),
    V1_s_c = cms.bool(False),
    V1_b_u = cms.bool(False),
    V1_b_c = cms.bool(False),
    
    # V1 decay modes - Z modes
    V1_e_e     = cms.bool(False),
    V1_nue_nue = cms.bool(False),
    V1_mu_mu     = cms.bool(False),
    V1_numu_numu = cms.bool(False),
    V1_tau_tau     = cms.bool(False),
    V1_nutau_nutau = cms.bool(False),    

    V1_d_d = cms.bool(False),
    V1_u_u = cms.bool(False),
    V1_s_s = cms.bool(False),
    V1_c_c = cms.bool(False),
    V1_b_b = cms.bool(False),
    
    

    # V2 decay modes - W modes
    V2_e_nue     = cms.bool(False),
    V2_mu_numu   = cms.bool(False),
    V2_tau_nutau = cms.bool(False),

    V2_d_u = cms.bool(False),
    V2_d_c = cms.bool(False),
    V2_s_u = cms.bool(False),
    V2_s_c = cms.bool(False),
    V2_b_u = cms.bool(False),
    V2_b_c = cms.bool(False),
    
    # V2 decay modes - Z modes
    V2_e_e     = cms.bool(False),
    V2_nue_nue = cms.bool(False),
    V2_mu_mu     = cms.bool(False),
    V2_numu_numu = cms.bool(False),
    V2_tau_tau     = cms.bool(False),
    V2_nutau_nutau = cms.bool(False),    

    V2_d_d = cms.bool(False),
    V2_u_u = cms.bool(False),
    V2_s_s = cms.bool(False),
    V2_c_c = cms.bool(False),
    V2_b_b = cms.bool(False),
    )

# ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
