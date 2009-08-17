#include "HiggsAnalysis/VBFHiggsToWWTolnujj/interface/VBFUtils.h"






int GetMatching_recoJets_genJets(VBFNtupleContent& treeVars,
                                 const unsigned int& nRecoJetMax,
                                 std::vector<int>* matchRecoJetIt)
{
  unsigned int iMax = nRecoJetMax;
  if(nRecoJetMax > treeVars.nRecoJet) iMax = treeVars.nRecoJet;
  
  
  std::vector<const reco::Particle*> recoJets;
  for(unsigned int i = 0; i < iMax; ++i)
    recoJets.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.recoJet_px[i],
                                                                     treeVars.recoJet_py[i],
                                                                     treeVars.recoJet_pz[i],
                                                                     treeVars.recoJet_energy[i])));
  
  
  std::vector<const reco::Particle*> genJets;
  genJets.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcTagJet_px[0],
                                                                  treeVars.mcTagJet_py[0],
                                                                  treeVars.mcTagJet_pz[0],
                                                                  treeVars.mcTagJet_energy[0])));
  genJets.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcTagJet_px[1],
                                                                  treeVars.mcTagJet_py[1],
                                                                  treeVars.mcTagJet_pz[1],
                                                                  treeVars.mcTagJet_energy[1])));
  genJets.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcJet_fromW2_px[0],
                                                                  treeVars.mcJet_fromW2_py[0],
                                                                  treeVars.mcJet_fromW2_pz[0],
                                                                  treeVars.mcJet_fromW2_energy[0])));
  genJets.push_back(new reco::Particle(0, math::XYZTLorentzVector(treeVars.mcJet_fromW2_px[1],
                                                                  treeVars.mcJet_fromW2_py[1],
                                                                  treeVars.mcJet_fromW2_pz[1],
                                                                  treeVars.mcJet_fromW2_energy[1])));
  
  int nRecoMatching = 0;  
  
  if(matchRecoJetIt != 0)
    nRecoMatching = GetMatching(recoJets, genJets, 0.3, false, false, matchRecoJetIt);
  else
    nRecoMatching = GetMatching(recoJets, genJets, 0.3, false, false);
  
  
  for_each(recoJets.begin(), recoJets.end(), deallocateVector<const reco::Particle*>());
  for_each(genJets.begin(), genJets.end(), deallocateVector<const reco::Particle*>());
  
  
  return nRecoMatching;
}

// --------------------------------------------------------------------------------------






int GetMatching_recoE_mcE(VBFNtupleContent& treeVars,
                          const unsigned int& nRecoEMax,
                          std::vector<int>* matchRecoEIt)
{
  unsigned int iMax = nRecoEMax;
  if(nRecoEMax > treeVars.nRecoE) iMax = treeVars.nRecoE;
  
  
  std::vector<const reco::Particle*> recoElectrons;
  for(unsigned int i = 0; i < iMax; ++i)
    recoElectrons.push_back(new reco::Particle(treeVars.recoE_charge[i],
                                               math::XYZTLorentzVector(treeVars.recoE_px[i],
                                                                       treeVars.recoE_py[i],
                                                                       treeVars.recoE_pz[i],
                                                                       treeVars.recoE_energy[i])));
  
  
  std::vector<const reco::Particle*> mcElectrons;
  for(unsigned int i = 0; i < iMax; ++i)
    mcElectrons.push_back(new reco::Particle(treeVars.mcE_charge[i],
                                             math::XYZTLorentzVector(treeVars.mcE_px[i],
                                                                     treeVars.mcE_py[i],
                                                                     treeVars.mcE_pz[i],
                                                                     treeVars.mcE_energy[i])));
  
  
  
  
  int nRecoMatching = 0;  
  
  if(matchRecoEIt != 0)
    nRecoMatching = GetMatching(recoElectrons, mcElectrons, 0.05, true, false, matchRecoEIt);
  else
    nRecoMatching = GetMatching(recoElectrons, mcElectrons, 0.05, true, false);
  
  
  for_each(recoElectrons.begin(), recoElectrons.end(), deallocateVector<const reco::Particle*>());
  for_each(mcElectrons.begin(), mcElectrons.end(), deallocateVector<const reco::Particle*>());
  
  
  return nRecoMatching;
}

// --------------------------------------------------------------------------------------






int GetMatching_recoMu_mcMu(VBFNtupleContent& treeVars,
                            const unsigned int& nRecoMuMax,
                            std::vector<int>* matchRecoMuIt)
{
  unsigned int iMax = nRecoMuMax;
  if(nRecoMuMax > treeVars.nRecoMu) iMax = treeVars.nRecoMu;
  
  
  std::vector<const reco::Particle*> recoMuons;
  for(unsigned int i = 0; i < iMax; ++i)
    recoMuons.push_back(new reco::Particle(treeVars.recoMu_charge[i],
                                           math::XYZTLorentzVector(treeVars.recoMu_px[i],
                                                                   treeVars.recoMu_py[i],
                                                                   treeVars.recoMu_pz[i],
                                                                   treeVars.recoMu_energy[i])));
  
  
  std::vector<const reco::Particle*> mcMuons;
  for(unsigned int i = 0; i < iMax; ++i)
    mcMuons.push_back(new reco::Particle(treeVars.mcMu_charge[i],
                                           math::XYZTLorentzVector(treeVars.mcMu_px[i],
                                                                   treeVars.mcMu_py[i],
                                                                   treeVars.mcMu_pz[i],
                                                                   treeVars.mcMu_energy[i])));
  
  
  
  int nRecoMatching = 0;  
  
  if(matchRecoMuIt != 0)
    nRecoMatching = GetMatching(recoMuons, mcMuons, 0.05, true, false, matchRecoMuIt);
  else
    nRecoMatching = GetMatching(recoMuons, mcMuons, 0.05, true, false);
  
  
  for_each(recoMuons.begin(), recoMuons.end(), deallocateVector<const reco::Particle*>());
  for_each(mcMuons.begin(), mcMuons.end(), deallocateVector<const reco::Particle*>());
  
  
  return nRecoMatching;
}

// --------------------------------------------------------------------------------------
