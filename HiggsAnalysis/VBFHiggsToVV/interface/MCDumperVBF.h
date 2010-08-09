#ifndef MCDumperVBF_h
#define MCDumperVBF_h

#include "PhysicsTools/NtupleUtils/interface/MCUtils.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <vector>



class MCDumperVBF
{
 public:
  
  //!ctor
  MCDumperVBF(edm::Handle<reco::GenParticleCollection>& genParticles,
           const int& eventType,
           const bool& verbosity);
  
  //!dtor
  ~MCDumperVBF();
  
  
  void PrintEventInfo();
  void GetHDecayMode();
  void GetVDecayMode(const int& Vindex);
  
     
  bool isValid() { return isValid_p; };
  
  std::string mcHDecayMode()  { return mcHDecayMode_p; };
  std::string mcV1DecayMode() { return mcV1DecayMode_p; };
  std::string mcV2DecayMode() { return mcV2DecayMode_p; };
  
  
  std::vector<const reco::Candidate*> GetMcE()  { return mcE_p;   };
  std::vector<const reco::Candidate*> GetMcMu() { return mcMu_p ; };
  
  
  const reco::Candidate* mcH() { return mcH_p; };

  const reco::Candidate* mcQ1_tag() { return mcQ1_tag_p; };
  const reco::Candidate* mcQ2_tag() { return mcQ2_tag_p; };

  const reco::Candidate* mcV1() { return mcV1_p; };
  const reco::Candidate* mcV2() { return mcV2_p; };

  const reco::Candidate* mcF1_fromV1() { return mcF1_fromV1_p; };
  const reco::Candidate* mcF2_fromV1() { return mcF2_fromV1_p; };
  const reco::Candidate* mcF1_fromV2() { return mcF1_fromV2_p; };
  const reco::Candidate* mcF2_fromV2() { return mcF2_fromV2_p; };
  
  
  
 private:
  
  void Analyze(edm::Handle<reco::GenParticleCollection>& hGenParticles);
  
  
  bool verbosity_p;
  int eventType_p;
  
  bool isValid_p;
  
  std::string mcHDecayMode_p;
  int         mcHDecayId_p;

  std::string mcV1DecayMode_p;
  int         mcV1DecayId_p;
  std::string mcV2DecayMode_p;
  int         mcV2DecayId_p;
  
  std::vector<const reco::Candidate*> mcCharged_p;
  std::vector<const reco::Candidate*> mcE_p;
  std::vector<const reco::Candidate*> mcMu_p;
  std::vector<const reco::Candidate*> mcTau_p;
  std::vector<std::vector<const reco::Candidate*> > mcTauJ_p;
  
  const reco::Candidate* mcH_p;

  const reco::Candidate* mcQ1_tag_p;
  const reco::Candidate* mcQ2_tag_p;

  const reco::Candidate* mcV1_p;
  const reco::Candidate* mcV2_p;

  const reco::Candidate* mcF1_fromV1_p;
  const reco::Candidate* mcF2_fromV1_p;
  const reco::Candidate* mcF1_fromV2_p;
  const reco::Candidate* mcF2_fromV2_p;
};

#endif
