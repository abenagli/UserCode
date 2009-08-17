#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/MCUtils.h"



// -----------------------------------------------------------------
// Read the ParticleDataTable to get mc particle names
// -----------------------------------------------------------------

std::string GetParticleName(const int& pdgId, edm::ESHandle<ParticleDataTable>& hPdt)
{
  const ParticleData* pd = hPdt -> particle(pdgId);
  
  if(!pd)
  {
    std::ostringstream ss;
    ss << "P" << pdgId;
    return ss.str();
  }
  
  else
    return pd -> name();
}






// -----------------------------------------------------------------
// Print the mc daughter particles
// -----------------------------------------------------------------

void PrintDaughterParticles(const reco::Candidate* pCurrent,
                            edm::ESHandle<ParticleDataTable>& hPdt,
                            const bool& verbosity,
                            const int& level)
{
  if(verbosity)
  {
    for(int j = 0; j < level; ++j) std::cout << "------";
    std::cout << "> ";
    PrintParticleInfo(pCurrent, hPdt);  
  }
  
  
  for(unsigned int i = 0; i < pCurrent -> numberOfDaughters(); ++i)
  {
    if(verbosity)
    {
      for(int j = 0; j < level+1; ++j) std::cout << "------";
      std::cout << "> ";
      PrintParticleInfo(pCurrent -> daughter(i), hPdt);
    }
  }
  
}






// -----------------------------------------------------------------
// Print the mc whole decay tree
// -----------------------------------------------------------------

void PrintDecayTree(const reco::Candidate* pCurrent,
                    edm::ESHandle<ParticleDataTable>& hPdt,
                    const bool& verbosity,
                    const int& level)
{
  if(verbosity)
  {
    for(int j = 0; j < level; ++j) std::cout << "------";
    std::cout << "> ";
    PrintParticleInfo(pCurrent, hPdt);  
  }
  
  
  for(unsigned int i = 0; i < pCurrent -> numberOfDaughters(); ++i)
  {
    if(pCurrent -> daughter(i) -> status() == 1)
    {
      if(verbosity)
      {
        for(int j = 0; j < level+1; ++j) std::cout << "------";
        std::cout << "> ";
        PrintParticleInfo(pCurrent -> daughter(i), hPdt);
      }
    }
     
    else PrintDecayTree(pCurrent -> daughter(i), hPdt, verbosity, level+1);
  }
  
}






// -----------------------------------------------------------------
// Get the vector of mc decay particles
// -----------------------------------------------------------------

void FindDecayParticles(std::vector<const reco::Candidate*>& pFinal,
                        const reco::Candidate* pCurrent,
                        edm::ESHandle<ParticleDataTable>& hPdt,
                        const bool& verbosity,
                        const int& level)
{
  if(verbosity)
  {
    for(int j = 0; j < level; ++j) std::cout << "------";
    std::cout << "> ";
    PrintParticleInfo(pCurrent, hPdt);  
  }
  
  
  for(unsigned int i = 0; i < pCurrent -> numberOfDaughters(); ++i)
  {
    if(pCurrent -> daughter(i) -> status() == 1)
    {
      pFinal.push_back(pCurrent -> daughter(i));
      
      if(verbosity)
      {
        for(int j = 0; j < level+1; ++j) std::cout << "------";
        std::cout << "> ";
        PrintParticleInfo(pCurrent -> daughter(i), hPdt);
      }
    }
     
    else FindDecayParticles(pFinal, pCurrent -> daughter(i), hPdt, verbosity, level+1);
  }
  
}






// -----------------------------------------------------------------
// Get the vector of mc particles from Higgs decay
// -----------------------------------------------------------------

void FindHDecayParticles(std::vector<const reco::Candidate*>& pFinal,
                         const reco::Candidate** pCurrent,
                         edm::ESHandle<ParticleDataTable>& hPdt,
                         const bool& verbosity,
                         const int& level)
{
  if(verbosity)
  {
    for(int j = 0; j < level; ++j) std::cout << "------";
    std::cout << "> ";
    PrintParticleInfo(*pCurrent, hPdt);  
  }
  
  
  // Variable for decayed Higgs
  const reco::Candidate* mcH = 0;


  // loop over Higgs daughters
  for(unsigned int i = 0; i < (*pCurrent) -> numberOfDaughters(); ++i)
  {
    if(verbosity)
    {
      for(int j = 0; j < level+1; ++j) std::cout << "------";
      std::cout << "> ";
      PrintParticleInfo((*pCurrent) -> daughter(i), hPdt);
    }
    
    
    // daughter is a W or a Z 
    if( (abs((*pCurrent) -> daughter(i) -> pdgId()) == 24) || 
	    ((*pCurrent) -> daughter(i) -> pdgId()  == 23) )
    {
      pFinal.push_back((*pCurrent) -> daughter(i));
    }


    // daughter is a Higgs (in status == 2)
    if( ((*pCurrent) -> daughter(i) -> pdgId()  == 25) &&
        ((*pCurrent) -> daughter(i) -> status()  == 2) )
    {
      mcH = (*pCurrent) -> daughter(i);
    }
     
  } // loop over Higgs daughters 
  
  
  // change Higgs particle to decayed Higgs
  if(mcH)
     (*pCurrent) =  mcH;
  
}





// -----------------------------------------------------------------
// Get the vector of mc particles from vector boson decay
// -----------------------------------------------------------------

void FindVDecayParticles(std::vector<const reco::Candidate*>& pFinal,
                         const reco::Candidate** pCurrent,
                         edm::ESHandle<ParticleDataTable>& hPdt,
                         const bool& verbosity,
                         const int& level)
{
  if(verbosity)
  {
    for(int j = 0; j < level; ++j) std::cout << "------";
    std::cout << "> ";
    PrintParticleInfo(*pCurrent, hPdt);  
  }
  
  
  // Variable for decayed vector boson
  const reco::Candidate* mcV = 0;


  // loop over vector boson daughters
  for(unsigned int i = 0; i < (*pCurrent) -> numberOfDaughters(); ++i)
  {
    // not a gluon
    //if((*pCurrent) -> daughter(i) -> pdgId() == 21) continue;
 
    if(verbosity)
    {
      for(int j = 0; j < level+1; ++j) std::cout << "------";
      std::cout << "> ";
      PrintParticleInfo((*pCurrent) -> daughter(i), hPdt);
    }
    
    
    // daughter is not Z or a W or a gluon
    if( (abs((*pCurrent) -> daughter(i) -> pdgId()) != 24) &&
        (    (*pCurrent) -> daughter(i) -> pdgId()  != 23) &&
        (    (*pCurrent) -> daughter(i) -> pdgId()  != 21) )
    {
      pFinal.push_back((*pCurrent) -> daughter(i));
    }


    // daughter is a vector boson (in status == 2)
    if( ( (abs((*pCurrent) -> daughter(i) -> pdgId())  == 24) || 
          (    (*pCurrent) -> daughter(i) -> pdgId()   == 23) ) &&
        ((*pCurrent) -> daughter(i) -> status()  == 2) )
    {
      mcV = (*pCurrent) -> daughter(i);
    }

  } // loop over vector boson daughters 
  
  
  // change vector boson particle to decayed vector boson
  if(mcV)
    (*pCurrent) =  mcV;
  
}






// -----------------------------------------------------------------
// Get the jet from a generic fermion decay
// -----------------------------------------------------------------

void FindFDecayJets(std::vector<const reco::Candidate*>& jFinal,
                    std::vector<const reco::Candidate*>& invJFinal,
                    const reco::Candidate** pCurrent,
                    edm::ESHandle<ParticleDataTable>& hPdt,
                    const bool& verbosity,
                    const int& level)
{
  if(verbosity)
  {
    for(int j = 0; j < level; ++j) std::cout << "------";
    std::cout << "> ";
    PrintParticleInfo(*pCurrent, hPdt);  
  }
  
  
  
  // particle is a quark
  if( (abs((*pCurrent) -> pdgId()) == 1) ||
      (abs((*pCurrent) -> pdgId()) == 2) ||
      (abs((*pCurrent) -> pdgId()) == 3) ||
      (abs((*pCurrent) -> pdgId()) == 4) ||
      (abs((*pCurrent) -> pdgId()) == 5) ||
      (abs((*pCurrent) -> pdgId()) == 6) )
    FindQDecayParticles(jFinal, invJFinal, *pCurrent, hPdt, false);
  
  
  // particle is not a quark
  else
  {
    // Variable for decayed fermion
    const reco::Candidate* mcF = 0;
    
    
    // loop over fermion daughters
    for(unsigned int i = 0; i < (*pCurrent) -> numberOfDaughters(); ++i)
    {
      if(verbosity)
      {
        for(int j = 0; j < level+1; ++j) std::cout << "------";
        std::cout << "> ";
        PrintParticleInfo((*pCurrent) -> daughter(i), hPdt);
      }
      
      // daughter is not Z or a W 
      //if( (abs((*pCurrent) -> daughter(i) -> pdgId()) != 24) &&
      //    (    (*pCurrent) -> daughter(i) -> pdgId()  != 23) )
      //{
      //  pFinal.push_back((*pCurrent) -> daughter(i));
      //}
      
      
      // daughter is the same particle as the mother (e,mu or nu )
      if( ((*pCurrent) -> daughter(i) -> pdgId() == (*pCurrent) -> pdgId() ) &&
          ( (abs((*pCurrent) -> pdgId()) == 11) ||
            (abs((*pCurrent) -> pdgId()) == 13) ||
            (abs((*pCurrent) -> pdgId()) == 12) ||
            (abs((*pCurrent) -> pdgId()) == 14) ||
            (abs((*pCurrent) -> pdgId()) == 16) ) &&
          ((*pCurrent) -> daughter(i) -> status()  == 1) )
      {
        mcF = (*pCurrent) -> daughter(i);
      }
      
      
      // daughter is the same particle as the mother (tau)
      if( ((*pCurrent) -> daughter(i) -> pdgId() == (*pCurrent) -> pdgId() ) &&
          (abs((*pCurrent) -> pdgId()) == 15) &&
          ((*pCurrent) -> daughter(i) -> status()  == 2) )
      {
        mcF = (*pCurrent) -> daughter(i);
        
        FindTauDecayParticles(jFinal, invJFinal, (*pCurrent) -> daughter(i), hPdt, false);
      }

      
      
    } // loop over vector boson daughters 

    // change vector boson particle to decayed vector boson
    if(mcF)
      (*pCurrent) =  mcF;
  }
  
  
}






// -----------------------------------------------------------------
// Get the neutrinos and visible tau-jet from tau decay
// -----------------------------------------------------------------

void FindTauDecayParticles(std::vector<const reco::Candidate*>& tauJ,
                           std::vector<const reco::Candidate*>& nu,
                           const reco::Candidate* pCurrent,
                           edm::ESHandle<ParticleDataTable>& hPdt,
                           const bool& verbosity)
{
  if( abs(pCurrent ->pdgId()) != 15 )
  {
    std::cerr << ">>>>>> HZZ2l2tauUtils::FindTauDecayParticles::Warning: Particle given is not a tau" << std::endl;
    return;
  }
  
  
  // clear vectors for final state particles from tau decay
  tauJ.clear();
  nu.clear();
  
  
  // decay parameters
  int nProng = 0;
  bool isLeptonic = false;

  std::vector<const reco::Candidate*> pFinal;
  FindDecayParticles(pFinal, pCurrent, hPdt, verbosity);
  
  
  // get neutrinos and visible tauJ 
  for(unsigned int i = 0; i < pFinal.size(); ++i)
  {
    int pdgId =  pFinal.at(i) -> pdgId();
    //int status = pFinal.at(i) -> status();
    int charge = pFinal.at(i) -> charge();
    
    if( (abs(pdgId) == 12) || (abs(pdgId) == 14) || (abs(pdgId) == 16) )
      nu.push_back(pFinal.at(i));
    
    if( (abs(pdgId) == 12) || (abs(pdgId) == 14) )
      isLeptonic = true;
    
    if( (abs(pdgId) != 12) && (abs(pdgId) != 14) && (abs(pdgId) != 16) )
      tauJ.push_back(pFinal.at(i));
  
    if(charge != 0)
      ++nProng;
  }
  
}






// -----------------------------------------------------------------
// Get the neutrinos and visible jet from quark fragmentation
// -----------------------------------------------------------------

void FindQDecayParticles(std::vector<const reco::Candidate*>& j,
                         std::vector<const reco::Candidate*>& nu,
                         const reco::Candidate* pCurrent,
                         edm::ESHandle<ParticleDataTable>& hPdt,
                         const bool& verbosity)
{
  if( (abs(pCurrent ->pdgId()) != 1) &&
      (abs(pCurrent ->pdgId()) != 2) &&      
      (abs(pCurrent ->pdgId()) != 3) &&
      (abs(pCurrent ->pdgId()) != 4) &&
      (abs(pCurrent ->pdgId()) != 5) &&
      (abs(pCurrent ->pdgId()) != 6) )
  {
    std::cerr << ">>>>>> HZZ2l2tauUtils::FindQuarkDecayParticles::Warning: Particle given is not a quark" << std::endl;
    return;
  }
  
  
  // clear vectors for final state particles from tau decay
  j.clear();
  nu.clear();
  

  
  std::vector<const reco::Candidate*> pFinal;
  FindDecayParticles(pFinal, pCurrent, hPdt, verbosity);
  
  
  // get neutrinos and visible jet 
  for(unsigned int i = 0; i < pFinal.size(); ++i)
  {
    int pdgId =  pFinal.at(i) -> pdgId();
    // int status = pFinal.at(i) -> status();
    // int charge = pFinal.at(i) -> charge();
    
    if( (abs(pdgId) == 12) || (abs(pdgId) == 14) || (abs(pdgId) == 16) )
      nu.push_back(pFinal.at(i));
    
    if( (abs(pdgId) != 12) && (abs(pdgId) != 14) && (abs(pdgId) != 16) )
      j.push_back(pFinal.at(i));
  }
  
}






// -----------------------------------------------------------------
// Get the leptons from Z boson decay
// -----------------------------------------------------------------

void FindZDecayParticles(std::vector<const reco::Candidate*>& lepton,
                         const reco::Candidate* pCurrent,
                         edm::ESHandle<ParticleDataTable>& hPdt,
                         const bool& verbosity)
{
  if(pCurrent ->pdgId() != 23 )
  {
    std::cerr << ">>>>>> HZZ2l2tauUtils::FindZDecayParticles::Warning: Particle given is not a Z" << std::endl;
    return;
  }
  
  
  // clear vectors for final state particles from Z decay
  lepton.clear();
  
  
  
  std::vector<const reco::Candidate*> pFinal;
  FindDecayParticles(pFinal, pCurrent, hPdt, verbosity);
}






// -----------------------------------------------------------------
// Find the higgs deacy mode
// -----------------------------------------------------------------

int FindHDecayMode(std::string& HDecayMode,
                    const reco::Candidate* mcV1,
                    const reco::Candidate* mcV2)
{
  if( (mcV1 -> pdgId() == 23) && (mcV2 -> pdgId() == 23) )
  {
    HDecayMode = "ZZ";
    return 225;
  }

  if( ( (mcV1 -> pdgId() ==  24) && (mcV2 -> pdgId() == -24) ) ||
      ( (mcV1 -> pdgId() == -24) && (mcV2 -> pdgId() ==  24) ) )
  {
    HDecayMode = "WW";
    return 226;
  }


  return -1;
}





int FindVDecayMode(std::string& VDecayMode,
                    const reco::Candidate* mcF_fromV,
                    const reco::Candidate* mcFbar_fromV)
{
  // ZZ modes
  if( (mcF_fromV -> pdgId() == 1) && (mcFbar_fromV -> pdgId() == -1) )
  {
    VDecayMode = "d_dbar";
    return 174;
  }
  if( (mcF_fromV -> pdgId() == 2) && (mcFbar_fromV -> pdgId() == -2) )
  {
    VDecayMode = "u_ubar";
    return 175;
  }
  if( (mcF_fromV -> pdgId() == 3) && (mcFbar_fromV -> pdgId() == -3) )
  {
    VDecayMode = "s_sbar";
    return 176;
  }
  if( (mcF_fromV -> pdgId() == 4) && (mcFbar_fromV -> pdgId() == -4) )
  {
  VDecayMode = "c_cbar";
  return 177;
  }
  if( (mcF_fromV -> pdgId() == 5) && (mcFbar_fromV -> pdgId() == -5) )
  {
    VDecayMode = "b_bbar";
    return 178;
  }
  if( (mcF_fromV -> pdgId() == 6) && (mcFbar_fromV -> pdgId() == -6) )
  {
    VDecayMode = "t_tbar";
    return 179;
  }
  if( (mcF_fromV -> pdgId() == 11) && (mcFbar_fromV -> pdgId() == -11) )
  {
    VDecayMode = "e_ebar";
    return 182;
  }
  if( (mcF_fromV -> pdgId() == 12) && (mcFbar_fromV -> pdgId() == -12) )
  {
    VDecayMode = "nue_nuebar";
    return 183;
  }
  if( (mcF_fromV -> pdgId() == 13) && (mcFbar_fromV -> pdgId() == -13) )
  {
    VDecayMode = "mu_mubar";
    return 184;
  }
  if( (mcF_fromV -> pdgId() == 14) && (mcFbar_fromV -> pdgId() == -14) )
  {
    VDecayMode = "numu_numubar";
    return 185;
  }
  if( (mcF_fromV -> pdgId() == 15) && (mcFbar_fromV -> pdgId() == -15) )
  {
    VDecayMode = "tau_taubar";
    return 186;
  }
  if( (mcF_fromV -> pdgId() == 16) && (mcFbar_fromV -> pdgId() == -16) )
  {
    VDecayMode = "nutau_nutaubar";
    return 187;
  }


  // WW modes
  if( (mcF_fromV -> pdgId() == 1) && (mcFbar_fromV -> pdgId() == -2) )
    VDecayMode = "d_ubar";
  if( (mcF_fromV -> pdgId() == 2) && (mcFbar_fromV -> pdgId() == -1) )
    VDecayMode = "u_dbar";

  if( (mcF_fromV -> pdgId() == 3) && (mcFbar_fromV -> pdgId() == -4) )
    VDecayMode = "s_cbar";
  if( (mcF_fromV -> pdgId() == 4) && (mcFbar_fromV -> pdgId() == -3) )
    VDecayMode = "c_sbar";

  if( (mcF_fromV -> pdgId() == 5) && (mcFbar_fromV -> pdgId() == -6) )
    VDecayMode = "b_tbar";
  if( (mcF_fromV -> pdgId() == 6) && (mcFbar_fromV -> pdgId() == -5) )
    VDecayMode = "t_bbar";



  if( (mcF_fromV -> pdgId() == 11) && (mcFbar_fromV -> pdgId() == -12) )
    VDecayMode = "e_nuebar";
  if( (mcF_fromV -> pdgId() == 12) && (mcFbar_fromV -> pdgId() == -11) )
    VDecayMode = "nue_ebar";
  if( (mcF_fromV -> pdgId() == 13) && (mcFbar_fromV -> pdgId() == -14) )
    VDecayMode = "mu_numubar";
  if( (mcF_fromV -> pdgId() == 14) && (mcFbar_fromV -> pdgId() == -13) )
    VDecayMode = "numu_mubar";
  if( (mcF_fromV -> pdgId() == 15) && (mcFbar_fromV -> pdgId() == -16) )
    VDecayMode = "tau_nutaubar";
  if( (mcF_fromV -> pdgId() == 16) && (mcFbar_fromV -> pdgId() == -15) )
    VDecayMode = "nutau_taubar";

  return -1;
}
