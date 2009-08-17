#ifndef HZZ2l2tauUtils_h
#define HZZ2l2tauUtils_h

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/Particle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include <vector>



// -----------------------------------------------------------------
// Read the ParticleDataTable to get mc particle names
// -----------------------------------------------------------------
std::string GetParticleName(const int& pdgId,
                            edm::ESHandle<ParticleDataTable>& hPdt);


// -----------------------------------------------------------------
// Print mc particle infos
// -----------------------------------------------------------------
template <class T>
void PrintParticleInfo(T* pCurrent, 
                       edm::ESHandle<ParticleDataTable>& hPdt)
{
  std::cout << std::setprecision(3) << std::fixed
            << "name:"              << std::setw(10) << GetParticleName(pCurrent -> pdgId(), hPdt)
            << "   Charge:"         << std::setw(4) << pCurrent -> charge()
            << "   St:"             << std::setw(1) << pCurrent -> status()
            << "   P=("             << std::setw(8) << pCurrent -> energy()
            << ","                  << std::setw(8) << pCurrent -> px()
            << ","                  << std::setw(8) << pCurrent -> py()
            << ","                  << std::setw(8) << pCurrent -> pz()
            << ")   pT = "          << std::setw(7) << pCurrent -> pt()
            << "   m = "            << std::setw(7) << pCurrent -> mass()
            << std::endl;
}


// -----------------------------------------------------------------
// Print the daughter particles
// -----------------------------------------------------------------
void PrintDaughterParticles(const reco::Candidate* pCurrent,
                            edm::ESHandle<ParticleDataTable>& hPdt,
                            const bool& verbosity = true,
                            const int& level = 1);


// -----------------------------------------------------------------
// Print the whole decay tree
// -----------------------------------------------------------------
void PrintDecayTree(const reco::Candidate* pCurrent,
                    edm::ESHandle<ParticleDataTable>& hPdt,
                    const bool& verbosity = true,
                    const int& level = 1);


// -----------------------------------------------------------------
// Get the vector of mc decay particles
// -----------------------------------------------------------------
void FindDecayParticles(std::vector<const reco::Candidate*>& pFinal,
                        const reco::Candidate* pCurrent,
                        edm::ESHandle<ParticleDataTable>& hPdt,
                        const bool& verbosity = false,
                        const int& level = 1);


// -----------------------------------------------------------------
// Get the vector of vector bosons from vector boson decay
// -----------------------------------------------------------------
void FindVDecayParticles(std::vector<const reco::Candidate*>& pFinal,
                        const reco::Candidate** pCurrent,
                        edm::ESHandle<ParticleDataTable>& hPdt,
                        const bool& verbosity = false,
                        const int& level = 1);


// -----------------------------------------------------------------
// Get the vector of vector bosons from Higgs decay
// -----------------------------------------------------------------
void FindHDecayParticles(std::vector<const reco::Candidate*>& pFinal,
                        const reco::Candidate** pCurrent,
                        edm::ESHandle<ParticleDataTable>& hPdt,
                        const bool& verbosity = false,
                        const int& level = 1);


// -----------------------------------------------------------------
// Get the neutrinos and visible tau-jet from tau decay
// -----------------------------------------------------------------
void FindTauDecayParticles(std::vector<const reco::Candidate*>& tauJ,
                           std::vector<const reco::Candidate*>& nu,
                           const reco::Candidate* pCurrent,
                           edm::ESHandle<ParticleDataTable>& hPdt,
                           const bool& verbosity = false);


// -----------------------------------------------------------------
// Get the neutrinos and visible jet from quark fragmentation
// -----------------------------------------------------------------
void FindQDecayParticles(std::vector<const reco::Candidate*>& j,
                         std::vector<const reco::Candidate*>& nu,
                         const reco::Candidate* pCurrent,
                         edm::ESHandle<ParticleDataTable>& hPdt,
                         const bool& verbosity = false);


// -----------------------------------------------------------------
// Get the leptons from Z boson decay
// -----------------------------------------------------------------
void FindZDecayParticles(std::vector<const reco::Candidate*>& lepton,
                         const reco::Candidate* pCurrent,
                         edm::ESHandle<ParticleDataTable>& hPdt,
                         const bool& verbosity = false);


// -----------------------------------------------------------------
// Get the jets from a generic fermion decay
// -----------------------------------------------------------------
void FindFDecayJets(std::vector<const reco::Candidate*>& jet,
                    std::vector<const reco::Candidate*>& invJet,
                    const reco::Candidate** pCurrent,
                    edm::ESHandle<ParticleDataTable>& hPdt,
                    const bool& verbosity = false,
                    const int& level = 1);


// -----------------------------------------------------------------
// Find the higgs boson decay mode
// -----------------------------------------------------------------
int FindHDecayMode(std::string& HdecayMode,
                   const reco::Candidate* mcV1,
                   const reco::Candidate* mcV2);


// -----------------------------------------------------------------
// Find the vector boson decay mode
// -----------------------------------------------------------------
int FindVDecayMode(std::string& VdecayMode,
                   const reco::Candidate* mcF_fromV,
                   const reco::Candidate* mcFbar_fromV);

#endif
