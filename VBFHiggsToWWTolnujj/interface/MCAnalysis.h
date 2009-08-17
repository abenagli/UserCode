#ifndef MCAnalysis_h
#define MCAnalysis_h

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/MCUtils.h"
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFUtils.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "SimGeneral/HepPDTRecord/interface/ParticleDataTable.h"

#include <vector>



class MCAnalysis
{
 public:
 
  MCAnalysis(edm::Handle<reco::GenParticleCollection>& hGenParticles,
                      edm::ESHandle<ParticleDataTable>& hPdt,
                      const bool& verbosity);
  ~MCAnalysis();


  void PrintHDecayTree(edm::ESHandle<ParticleDataTable>& hPdt);
  void PrintHInfo     (edm::ESHandle<ParticleDataTable>& hPdt);
   
  std::pair<bool, bool> GetMatchTagJets(reco::GenJetCollection::const_iterator& jet1,
                                        reco::GenJetCollection::const_iterator& jet2,
                                        float& DRmax_mcTagQ1,
                                        float& DRmax_mcTagQ2,
                                        edm::Handle<reco::GenJetCollection>& hMcJets);

  std::pair<bool, bool> GetMatchJets_fromV2(reco::GenJetCollection::const_iterator& jet1,
                                            reco::GenJetCollection::const_iterator& jet2,
                                            float& DRmax_mcF,
                                            float& DRmax_mcFbar,
                                            edm::Handle<reco::GenJetCollection>& hMcJets);
   
  
  bool isValid() { return isValid_p; };
  
  std::string mcHDecayMode()  { return mcHDecayMode_p; };
  int         mcHDecayId()    { return mcHDecayId_p; };

  std::string mcV1DecayMode() { return mcV1DecayMode_p; };
  int         mcV1DecayId()   { return mcV1DecayId_p; };
  std::string mcV2DecayMode() { return mcV2DecayMode_p; };
  int         mcV2DecayId()   { return mcV2DecayId_p; };
  
  
  std::vector<const reco::Candidate*> GetMcEle() { return mcEle_p; };
  std::vector<const reco::Candidate*> GetMcMu()  { return mcMu_p ; };
  
  
  const reco::Candidate* mcH()    { return mcH_p; };

  const reco::Candidate* mcTagQ1() { return mcTagQ1_p; };
  const reco::Candidate* mcTagQ2() { return mcTagQ2_p; };

  const reco::Candidate* mcV1()    { return mcV1_p; };
  const reco::Candidate* mcV2()    { return mcV2_p; };

  const reco::Candidate* mcF_fromV1()    { return mcF_fromV1_p; };
  const reco::Candidate* mcFbar_fromV1() { return mcFbar_fromV1_p; };
  const reco::Candidate* mcF_fromV2()    { return mcF_fromV2_p; };
  const reco::Candidate* mcFbar_fromV2() { return mcFbar_fromV2_p; };

  std::vector<const reco::Candidate*> mcJ_fromV1()       { return mcJ_fromV1_p; };
  std::vector<const reco::Candidate*> mcInvJ_fromV1()    { return mcInvJ_fromV1_p; };
  std::vector<const reco::Candidate*> mcJbar_fromV1()    { return mcJbar_fromV1_p; };
  std::vector<const reco::Candidate*> mcInvJbar_fromV1() { return mcInvJbar_fromV1_p; };
  std::vector<const reco::Candidate*> mcJ_fromV2()       { return mcJ_fromV2_p; };
  std::vector<const reco::Candidate*> mcInvJ_fromV2()    { return mcInvJ_fromV2_p; };
  std::vector<const reco::Candidate*> mcJbar_fromV2()    { return mcJbar_fromV2_p; };
  std::vector<const reco::Candidate*> mcInvJbar_fromV2() { return mcInvJbar_fromV2_p; };


  
  
  
 private:
  
  
  void Analyze(edm::Handle<reco::GenParticleCollection>& hGenParticles,
               edm::ESHandle<ParticleDataTable>& hPdt);


  bool verbosity_p;

  bool isValid_p;
  
  std::string mcHDecayMode_p;
  int         mcHDecayId_p;

  std::string mcV1DecayMode_p;
  int         mcV1DecayId_p;
  std::string mcV2DecayMode_p;
  int         mcV2DecayId_p;
  
  std::vector<const reco::Candidate*> mcCharged_p;
  std::vector<const reco::Candidate*> mcEle_p;
  std::vector<const reco::Candidate*> mcMu_p;
  std::vector<const reco::Candidate*> mcTau_p;
  std::vector<std::vector<const reco::Candidate*> > mcTauJ_p;
  
  const reco::Candidate* mcH_p;

  const reco::Candidate* mcTagQ1_p;
  const reco::Candidate* mcTagQ2_p;

  const reco::Candidate* mcV1_p;
  const reco::Candidate* mcV2_p;

  const reco::Candidate* mcF_fromV1_p;
  const reco::Candidate* mcFbar_fromV1_p;
  const reco::Candidate* mcF_fromV2_p;
  const reco::Candidate* mcFbar_fromV2_p;

  std::vector<const reco::Candidate*> mcJ_fromV1_p;
  std::vector<const reco::Candidate*> mcInvJ_fromV1_p;
  std::vector<const reco::Candidate*> mcJbar_fromV1_p;
  std::vector<const reco::Candidate*> mcInvJbar_fromV1_p;
  std::vector<const reco::Candidate*> mcJ_fromV2_p;
  std::vector<const reco::Candidate*> mcInvJ_fromV2_p;
  std::vector<const reco::Candidate*> mcJbar_fromV2_p;
  std::vector<const reco::Candidate*> mcInvJbar_fromV2_p;


//edm::Handle<reco::GenParticleCollection> hGenParticles_;


  //const reco::Candidate * mcLZTau1_;
  //const reco::Candidate * mcLZTau2_;


  //std::vector<int> mcTauJets_is1Prong_;
  //std::vector<int> mcTauJets_isLeptonic_;
 
  //std::vector<const reco::Candidate *> da_mcTau1_;
  //std::vector<const reco::Candidate *> da_mcTau2_;

  //LorentzVector da_mcTau1_momentum_;
  //LorentzVector da_mcTau2_momentum_;

  //LorentzVector nu_mcTau1_momentum_;
  //LorentzVector nu_mcTau2_momentum_;

  //int da_Tau1id_;
  //int da_Tau2id_;

  


};

#endif

