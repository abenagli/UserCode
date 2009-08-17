#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFNtupleCaloJets.h"

#define PI 3.141592654






VBFNtupleCaloJets::VBFNtupleCaloJets(const edm::ParameterSet& iConfig):
 srcGenParticles_p             (iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 srcGenJets_p                  (iConfig.getParameter<edm::InputTag>("srcGenJets")),
 srcRecoJets_p                 (iConfig.getParameter<edm::InputTag>("srcRecoJets")),
 srcRecoElectrons_p            (iConfig.getParameter<edm::InputTag>("srcRecoElectrons")),
 srcRecoElectronTkIsolations_p (iConfig.getParameter<edm::InputTag>("srcRecoElectronTkIsolations")),
 srcRecoElectronEmIsolations_p (iConfig.getParameter<edm::InputTag>("srcRecoElectronEmIsolations")),
 srcRecoElectronHadIsolations_p(iConfig.getParameter<edm::InputTag>("srcRecoElectronHadIsolations")),
 srcRecoElectronIdLoose_p      (iConfig.getParameter<edm::InputTag>("srcRecoElectronIdLoose")),
 srcRecoElectronIdTight_p      (iConfig.getParameter<edm::InputTag>("srcRecoElectronIdTight")),
 srcRecoElectronIdRobustLoose_p(iConfig.getParameter<edm::InputTag>("srcRecoElectronIdRobustLoose")),
 srcRecoElectronIdRobustTight_p(iConfig.getParameter<edm::InputTag>("srcRecoElectronIdRobustTight")),
 srcRecoMuons_p                (iConfig.getParameter<edm::InputTag>("srcRecoMuons")),
 fileName_p                    (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFNtupleCaloJets"))),
 verbosity_p                   (iConfig.getUntrackedParameter<bool>("verbosity", true)),
 eventsToPrint_p               (iConfig.getUntrackedParameter<int>("eventsToPrint", 0)),
 eventType_p                   (iConfig.getUntrackedParameter<int>("eventType", 0)),
 eventId_p(0)
{
  // Create File
  outFile_p = new TFile(fileName_p.c_str() , "RECREATE");
  outFile_p -> cd();
  
  
  // Initialize Tree
  tree_p = new TTree("ntu", "ntu");
  SetBranches(tree_p, treeVars_p);
}






VBFNtupleCaloJets::~VBFNtupleCaloJets()
{}







void VBFNtupleCaloJets::beginJob(const edm::EventSetup& iSetup)
{
}






void VBFNtupleCaloJets::endJob()
{
  if(verbosity_p)
    std::cerr << "*** VBFNtupleCaloJets::endJob ***" << std::endl;
  
  outFile_p -> cd();
  tree_p -> Write();
  outFile_p -> Close();
}






void VBFNtupleCaloJets::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // set event id
  ++eventId_p;
  
  if( (eventId_p%100 == 0) && (verbosity_p == true) )
    std::cout << ">>>>>> VBFNtupleCaloJets::Event number -----> " << std::fixed << std::setw(10) << eventId_p << std::endl;
  
  
  
  
  
  // -----------------------------------------------------------------
  // Get the collections
  // -----------------------------------------------------------------  

  edm::Handle<reco::GenParticleCollection> hGenParticles;
  iEvent.getByLabel(srcGenParticles_p, hGenParticles);
  if( !(hGenParticles.isValid ()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcGenParticles_p << " not available" << std::endl;
    return;
  }

  edm::ESHandle<ParticleDataTable> hPdt;
  iSetup.getData(hPdt);

  edm::Handle<reco::GenJetCollection> hGenJets;
  iEvent.getByLabel(srcGenJets_p, hGenJets);
  if( !(hGenJets.isValid ()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcGenJets_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::PixelMatchGsfElectronCollection> hRecoElectrons;
  iEvent.getByLabel(srcRecoElectrons_p, hRecoElectrons);
  if( !(hRecoElectrons.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoElectrons_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > hRecoElectronTkIsolations;
  iEvent.getByLabel(srcRecoElectronTkIsolations_p, hRecoElectronTkIsolations);
  if( !(hRecoElectronTkIsolations.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoElectronTkIsolations_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > hRecoElectronEmIsolations;
  iEvent.getByLabel(srcRecoElectronEmIsolations_p, hRecoElectronEmIsolations);
 if( !(hRecoElectronEmIsolations.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoElectronEmIsolations_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > hRecoElectronHadIsolations;
  iEvent.getByLabel(srcRecoElectronHadIsolations_p, hRecoElectronHadIsolations);
  if( !(hRecoElectronHadIsolations.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoElectronHadIsolations_p << " not available" << std::endl;
    return;
  }

  edm::Handle<edm::ValueMap<float> > hRecoElectronIdLoose;
  iEvent.getByLabel(srcRecoElectronIdLoose_p, hRecoElectronIdLoose);
  if( !(hRecoElectronIdLoose.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoElectronIdLoose_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > hRecoElectronIdTight;
  iEvent.getByLabel(srcRecoElectronIdTight_p, hRecoElectronIdTight);
  if( !(hRecoElectronIdTight.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoElectronIdTight_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > hRecoElectronIdRobustLoose;
  iEvent.getByLabel(srcRecoElectronIdRobustLoose_p, hRecoElectronIdRobustLoose);
  if( !(hRecoElectronIdRobustLoose.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoElectronIdRobustLoose_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > hRecoElectronIdRobustTight;
  iEvent.getByLabel(srcRecoElectronIdRobustTight_p, hRecoElectronIdRobustTight);
  if( !(hRecoElectronIdRobustTight.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoElectronIdRobustTight_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::MuonCollection> hRecoMuons;
  iEvent.getByLabel (srcRecoMuons_p, hRecoMuons);
  if( !(hRecoMuons.isValid()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoMuons_p << " not available" << std::endl;
    return;
  } 
  
  edm::Handle<reco::CaloJetCollection> hRecoJets;
  iEvent.getByLabel(srcRecoJets_p, hRecoJets);
  if( !(hRecoJets.isValid ()) )
  {
    std::cerr << ">>>>>> VBFNtupleCaloJets::Warning: " << srcRecoJets_p << " not available" << std::endl;
    return;
  }  
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Fill the tree
  // -----------------------------------------------------------------  

  // Initialize tree branches
  InitializeBranches(tree_p, treeVars_p);
  
  
  
  treeVars_p.eventType = eventType_p;
  treeVars_p.eventId   = eventId_p;
  
  
  
  // mc variables

  MCAnalysis mcAnalysis(hGenParticles, hPdt, false);
  bool isMCAnalysisValid = mcAnalysis.isValid();
  if(!isMCAnalysisValid) return;
  
  
  const reco::Candidate* mcH = mcAnalysis.mcH();
  
  const reco::Candidate* mcW1 = mcAnalysis.mcV1();
  const reco::Candidate* mcW2 = mcAnalysis.mcV2();
  
  const reco::Candidate* mcL_fromW1 = mcAnalysis.mcF_fromV1();
  const reco::Candidate* mcNu_fromW1 = mcAnalysis.mcFbar_fromV1();
  if( (abs(mcNu_fromW1 -> pdgId()) != 12) && (abs(mcNu_fromW1 -> pdgId()) != 14) && (abs(mcNu_fromW1 -> pdgId()) != 16) )
  {
    mcL_fromW1 = mcAnalysis.mcFbar_fromV1();
    mcNu_fromW1 = mcAnalysis.mcF_fromV1();
  }
  if(abs(mcL_fromW1 -> pdgId()) == 15) return;
  
  const reco::Candidate* mcQ1_fromW2 = mcAnalysis.mcF_fromV2();
  const reco::Candidate* mcQ2_fromW2 = mcAnalysis.mcFbar_fromV2();
  
  const reco::Candidate* mcTagQ1 = mcAnalysis.mcTagQ1();
  const reco::Candidate* mcTagQ2 = mcAnalysis.mcTagQ2();
  
  
  
  treeVars_p.mcH_energy = mcH -> energy();
  treeVars_p.mcH_px     = mcH -> px();
  treeVars_p.mcH_py     = mcH -> py();
  treeVars_p.mcH_pz     = mcH -> pz();
  treeVars_p.mcH_p      = mcH -> p();
  treeVars_p.mcH_pt     = mcH -> pt();
  treeVars_p.mcH_mass   = mcH -> mass();
  treeVars_p.mcH_phi    = mcH -> phi();
  treeVars_p.mcH_eta    = mcH -> eta();
  treeVars_p.mcH_charge = mcH -> charge();
  
  
  
  treeVars_p.mcW_energy[0] = mcW1 -> energy();
  treeVars_p.mcW_px[0]     = mcW1 -> px();
  treeVars_p.mcW_py[0]     = mcW1 -> py();
  treeVars_p.mcW_pz[0]     = mcW1 -> pz();
  treeVars_p.mcW_p[0]      = mcW1 -> p();
  treeVars_p.mcW_pt[0]     = mcW1 -> pt();
  treeVars_p.mcW_mass[0]   = mcW1 -> mass();
  treeVars_p.mcW_phi[0]    = mcW1 -> phi();
  treeVars_p.mcW_eta[0]    = mcW1 -> eta();
  treeVars_p.mcW_charge[0] = mcW1 -> charge();

  treeVars_p.mcW_energy[1] = mcW2 -> energy();
  treeVars_p.mcW_px[1]     = mcW2 -> px();
  treeVars_p.mcW_py[1]     = mcW2 -> py();
  treeVars_p.mcW_pz[1]     = mcW2 -> pz();
  treeVars_p.mcW_p[1]      = mcW2 -> p();
  treeVars_p.mcW_pt[1]     = mcW2 -> pt();
  treeVars_p.mcW_mass[1]   = mcW2 -> mass();
  treeVars_p.mcW_phi[1]    = mcW2 -> phi();
  treeVars_p.mcW_eta[1]    = mcW2 -> eta();
  treeVars_p.mcW_charge[1] = mcW2 -> charge();
  
  
  
  treeVars_p.mcF_fromW1_energy[0] = mcL_fromW1 -> energy();
  treeVars_p.mcF_fromW1_px[0]     = mcL_fromW1 -> px();
  treeVars_p.mcF_fromW1_py[0]     = mcL_fromW1 -> py();
  treeVars_p.mcF_fromW1_pz[0]     = mcL_fromW1 -> pz();
  treeVars_p.mcF_fromW1_p[0]      = mcL_fromW1 -> p();
  treeVars_p.mcF_fromW1_pt[0]     = mcL_fromW1 -> pt();
  treeVars_p.mcF_fromW1_mass[0]   = mcL_fromW1 -> mass();
  treeVars_p.mcF_fromW1_phi[0]    = mcL_fromW1 -> phi();
  treeVars_p.mcF_fromW1_eta[0]    = mcL_fromW1 -> eta();
  treeVars_p.mcF_fromW1_charge[0] = mcL_fromW1 -> charge();
  treeVars_p.mcF_fromW1_pdgId[0]  = mcL_fromW1 -> pdgId();

  treeVars_p.mcF_fromW1_energy[1] = mcNu_fromW1 -> energy();
  treeVars_p.mcF_fromW1_px[1]     = mcNu_fromW1 -> px();
  treeVars_p.mcF_fromW1_py[1]     = mcNu_fromW1 -> py();
  treeVars_p.mcF_fromW1_pz[1]     = mcNu_fromW1 -> pz();
  treeVars_p.mcF_fromW1_p[1]      = mcNu_fromW1 -> p();
  treeVars_p.mcF_fromW1_pt[1]     = mcNu_fromW1 -> pt();
  treeVars_p.mcF_fromW1_mass[1]   = mcNu_fromW1 -> mass();
  treeVars_p.mcF_fromW1_phi[1]    = mcNu_fromW1 -> phi();
  treeVars_p.mcF_fromW1_eta[1]    = mcNu_fromW1 -> eta();
  treeVars_p.mcF_fromW1_charge[1] = mcNu_fromW1 -> charge();
  treeVars_p.mcF_fromW1_pdgId[1]  = mcNu_fromW1 -> pdgId();  
  
  
  
  treeVars_p.mcQ_fromW2_energy[0] = mcQ1_fromW2 -> energy();
  treeVars_p.mcQ_fromW2_px[0]     = mcQ1_fromW2 -> px();
  treeVars_p.mcQ_fromW2_py[0]     = mcQ1_fromW2 -> py();
  treeVars_p.mcQ_fromW2_pz[0]     = mcQ1_fromW2 -> pz();
  treeVars_p.mcQ_fromW2_p[0]      = mcQ1_fromW2 -> p();
  treeVars_p.mcQ_fromW2_pt[0]     = mcQ1_fromW2 -> pt();
  treeVars_p.mcQ_fromW2_mass[0]   = mcQ1_fromW2 -> mass();
  treeVars_p.mcQ_fromW2_phi[0]    = mcQ1_fromW2 -> phi();
  treeVars_p.mcQ_fromW2_eta[0]    = mcQ1_fromW2 -> eta();
  treeVars_p.mcQ_fromW2_charge[0] = mcQ1_fromW2 -> charge();
  treeVars_p.mcQ_fromW2_pdgId[0]  = mcQ1_fromW2 -> pdgId();

  treeVars_p.mcQ_fromW2_energy[1] = mcQ2_fromW2 -> energy();
  treeVars_p.mcQ_fromW2_px[1]     = mcQ2_fromW2 -> px();
  treeVars_p.mcQ_fromW2_py[1]     = mcQ2_fromW2 -> py();
  treeVars_p.mcQ_fromW2_pz[1]     = mcQ2_fromW2 -> pz();
  treeVars_p.mcQ_fromW2_p[1]      = mcQ2_fromW2 -> p();
  treeVars_p.mcQ_fromW2_pt[1]     = mcQ2_fromW2 -> pt();
  treeVars_p.mcQ_fromW2_mass[1]   = mcQ2_fromW2 -> mass();
  treeVars_p.mcQ_fromW2_phi[1]    = mcQ2_fromW2 -> phi();
  treeVars_p.mcQ_fromW2_eta[1]    = mcQ2_fromW2 -> eta();
  treeVars_p.mcQ_fromW2_charge[1] = mcQ2_fromW2 -> charge();
  treeVars_p.mcQ_fromW2_pdgId[1]  = mcQ2_fromW2 -> pdgId();  
  
  
  
  treeVars_p.mcTagQ_energy[0] = mcTagQ1 -> energy();
  treeVars_p.mcTagQ_px[0]     = mcTagQ1 -> px();
  treeVars_p.mcTagQ_py[0]     = mcTagQ1 -> py();
  treeVars_p.mcTagQ_pz[0]     = mcTagQ1 -> pz();
  treeVars_p.mcTagQ_p[0]      = mcTagQ1 -> p();
  treeVars_p.mcTagQ_pt[0]     = mcTagQ1 -> pt();
  treeVars_p.mcTagQ_mass[0]   = mcTagQ1 -> mass();
  treeVars_p.mcTagQ_phi[0]    = mcTagQ1 -> phi();
  treeVars_p.mcTagQ_eta[0]    = mcTagQ1 -> eta();
  treeVars_p.mcTagQ_charge[0] = mcTagQ1 -> charge();
  treeVars_p.mcTagQ_pdgId[0]  = mcTagQ1 -> pdgId();

  treeVars_p.mcTagQ_energy[1] = mcTagQ2 -> energy();
  treeVars_p.mcTagQ_px[1]     = mcTagQ2 -> px();
  treeVars_p.mcTagQ_py[1]     = mcTagQ2 -> py();
  treeVars_p.mcTagQ_pz[1]     = mcTagQ2 -> pz();
  treeVars_p.mcTagQ_p[1]      = mcTagQ2 -> p();
  treeVars_p.mcTagQ_pt[1]     = mcTagQ2 -> pt();
  treeVars_p.mcTagQ_mass[1]   = mcTagQ2 -> mass();
  treeVars_p.mcTagQ_phi[1]    = mcTagQ2 -> phi();
  treeVars_p.mcTagQ_eta[1]    = mcTagQ2 -> eta();
  treeVars_p.mcTagQ_charge[1] = mcTagQ2 -> charge(); 
  treeVars_p.mcTagQ_pdgId[1] = mcTagQ2 -> pdgId(); 
  
  
  
  
  
  
  // Matching between genJets and quarks
  std::vector<reco::GenJetCollection::const_iterator> genJets;
  for(reco::GenJetCollection::const_iterator jetIt = hGenJets -> begin();
      jetIt != hGenJets -> end(); ++jetIt)
    genJets.push_back(jetIt);   
  
  std::vector<const reco::Candidate*> mcQuarks;
  mcQuarks.push_back(mcTagQ1);
  mcQuarks.push_back(mcTagQ2);
  mcQuarks.push_back(mcQ1_fromW2);
  mcQuarks.push_back(mcQ2_fromW2);

  std::vector<int> matchGenJetIt;
  GetMatching(genJets, mcQuarks, 0.3, false, false, &matchGenJetIt);
  
  
  if(matchGenJetIt.at(0) != -1) 
  {
    //std::cout << "TagQ1:   jetIt = " << matchGenJetIt.at(0) << std::endl;
    
    treeVars_p.mcTagJet_energy[0] = (hGenJets -> at(matchGenJetIt.at(0))).energy();
    treeVars_p.mcTagJet_px[0]     = (hGenJets -> at(matchGenJetIt.at(0))).px();
    treeVars_p.mcTagJet_py[0]     = (hGenJets -> at(matchGenJetIt.at(0))).py();
    treeVars_p.mcTagJet_pz[0]     = (hGenJets -> at(matchGenJetIt.at(0))).pz();
    treeVars_p.mcTagJet_p[0]      = (hGenJets -> at(matchGenJetIt.at(0))).p();
    treeVars_p.mcTagJet_pt[0]     = (hGenJets -> at(matchGenJetIt.at(0))).pt();
    treeVars_p.mcTagJet_mass[0]   = (hGenJets -> at(matchGenJetIt.at(0))).mass();
    treeVars_p.mcTagJet_phi[0]    = (hGenJets -> at(matchGenJetIt.at(0))).phi();
    treeVars_p.mcTagJet_eta[0]    = (hGenJets -> at(matchGenJetIt.at(0))).eta();
    treeVars_p.mcTagJet_charge[0] = (hGenJets -> at(matchGenJetIt.at(0))).charge();
    treeVars_p.mcTagJet_isMatching[0] = 1;
  }
  
  if(matchGenJetIt.at(1) != -1) 
  {
    //std::cout << "TagQ2:   jetIt = " << matchGenJetIt.at(1) << std::endl;
    
    treeVars_p.mcTagJet_energy[1] = (hGenJets -> at(matchGenJetIt.at(1))).energy();
    treeVars_p.mcTagJet_px[1]     = (hGenJets -> at(matchGenJetIt.at(1))).px();
    treeVars_p.mcTagJet_py[1]     = (hGenJets -> at(matchGenJetIt.at(1))).py();
    treeVars_p.mcTagJet_pz[1]     = (hGenJets -> at(matchGenJetIt.at(1))).pz();
    treeVars_p.mcTagJet_p[1]      = (hGenJets -> at(matchGenJetIt.at(1))).p();
    treeVars_p.mcTagJet_pt[1]     = (hGenJets -> at(matchGenJetIt.at(1))).pt();
    treeVars_p.mcTagJet_mass[1]   = (hGenJets -> at(matchGenJetIt.at(1))).mass();
    treeVars_p.mcTagJet_phi[1]    = (hGenJets -> at(matchGenJetIt.at(1))).phi();
    treeVars_p.mcTagJet_eta[1]    = (hGenJets -> at(matchGenJetIt.at(1))).eta();
    treeVars_p.mcTagJet_charge[1] = (hGenJets -> at(matchGenJetIt.at(1))).charge();
    treeVars_p.mcTagJet_isMatching[1] = 1;
  }  
  
  if(matchGenJetIt.at(2) != -1) 
  {
    //std::cout << "Q1_fromW2:   jetIt = " << matchGenJetIt.at(2) << std::endl;
    
    treeVars_p.mcJet_fromW2_energy[0] = (hGenJets -> at(matchGenJetIt.at(2))).energy();
    treeVars_p.mcJet_fromW2_px[0]     = (hGenJets -> at(matchGenJetIt.at(2))).px();
    treeVars_p.mcJet_fromW2_py[0]     = (hGenJets -> at(matchGenJetIt.at(2))).py();
    treeVars_p.mcJet_fromW2_pz[0]     = (hGenJets -> at(matchGenJetIt.at(2))).pz();
    treeVars_p.mcJet_fromW2_p[0]      = (hGenJets -> at(matchGenJetIt.at(2))).p();
    treeVars_p.mcJet_fromW2_pt[0]     = (hGenJets -> at(matchGenJetIt.at(2))).pt();
    treeVars_p.mcJet_fromW2_mass[0]   = (hGenJets -> at(matchGenJetIt.at(2))).mass();
    treeVars_p.mcJet_fromW2_phi[0]    = (hGenJets -> at(matchGenJetIt.at(2))).phi();
    treeVars_p.mcJet_fromW2_eta[0]    = (hGenJets -> at(matchGenJetIt.at(2))).eta();
    treeVars_p.mcJet_fromW2_charge[0] = (hGenJets -> at(matchGenJetIt.at(2))).charge();
    treeVars_p.mcJet_fromW2_isMatching[0] = 1;
  }  
  
  if(matchGenJetIt.at(3) != -1) 
  {
    //std::cout << "Q2_fromW2:   jetIt = " << matchGenJetIt.at(3) << std::endl;
    
    treeVars_p.mcJet_fromW2_energy[1] = (hGenJets -> at(matchGenJetIt.at(3))).energy();
    treeVars_p.mcJet_fromW2_px[1]     = (hGenJets -> at(matchGenJetIt.at(3))).px();
    treeVars_p.mcJet_fromW2_py[1]     = (hGenJets -> at(matchGenJetIt.at(3))).py();
    treeVars_p.mcJet_fromW2_pz[1]     = (hGenJets -> at(matchGenJetIt.at(3))).pz();
    treeVars_p.mcJet_fromW2_p[1]      = (hGenJets -> at(matchGenJetIt.at(3))).p();
    treeVars_p.mcJet_fromW2_pt[1]     = (hGenJets -> at(matchGenJetIt.at(3))).pt();
    treeVars_p.mcJet_fromW2_mass[1]   = (hGenJets -> at(matchGenJetIt.at(3))).mass();
    treeVars_p.mcJet_fromW2_phi[1]    = (hGenJets -> at(matchGenJetIt.at(3))).phi();
    treeVars_p.mcJet_fromW2_eta[1]    = (hGenJets -> at(matchGenJetIt.at(3))).eta();
    treeVars_p.mcJet_fromW2_charge[1] = (hGenJets -> at(matchGenJetIt.at(3))).charge();
    treeVars_p.mcJet_fromW2_isMatching[1] = 1;
  }  
  
  
  
  
  
  
  // mc electrons
  treeVars_p.nMcE = 0;
  for(unsigned eleIt = 0; eleIt < (mcAnalysis.GetMcEle()).size(); ++eleIt)
  {
    if(treeVars_p.nMcE >= MCEMAX) break;
    
    treeVars_p.mcE_energy[treeVars_p.nMcE] = ((mcAnalysis.GetMcEle()).at(eleIt)) -> energy();
    treeVars_p.mcE_px[treeVars_p.nMcE]     = ((mcAnalysis.GetMcEle()).at(eleIt)) -> px();
    treeVars_p.mcE_py[treeVars_p.nMcE]     = ((mcAnalysis.GetMcEle()).at(eleIt)) -> py();
    treeVars_p.mcE_pz[treeVars_p.nMcE]     = ((mcAnalysis.GetMcEle()).at(eleIt)) -> pz();
    treeVars_p.mcE_p[treeVars_p.nMcE]      = ((mcAnalysis.GetMcEle()).at(eleIt)) -> p();
    treeVars_p.mcE_pt[treeVars_p.nMcE]     = ((mcAnalysis.GetMcEle()).at(eleIt)) -> pt();
    treeVars_p.mcE_mass[treeVars_p.nMcE]   = ((mcAnalysis.GetMcEle()).at(eleIt)) -> mass();
    treeVars_p.mcE_phi[treeVars_p.nMcE]    = ((mcAnalysis.GetMcEle()).at(eleIt)) -> phi();
    treeVars_p.mcE_eta[treeVars_p.nMcE]    = ((mcAnalysis.GetMcEle()).at(eleIt)) -> eta();
    treeVars_p.mcE_charge[treeVars_p.nMcE] = ((mcAnalysis.GetMcEle()).at(eleIt)) -> charge();
    
    ++treeVars_p.nMcE;
  }  
  
  // reco electrons
  treeVars_p.nRecoE = 0;
  for(reco::PixelMatchGsfElectronCollection::const_iterator eleIt = hRecoElectrons -> begin();
      eleIt != hRecoElectrons -> end(); ++eleIt)
  {
    if(treeVars_p.nRecoE >= RECOEMAX) break;
    
    treeVars_p.recoE_energy[treeVars_p.nRecoE] = eleIt -> energy();
    treeVars_p.recoE_px[treeVars_p.nRecoE]     = eleIt -> px();
    treeVars_p.recoE_py[treeVars_p.nRecoE]     = eleIt -> py();
    treeVars_p.recoE_pz[treeVars_p.nRecoE]     = eleIt -> pz();
    treeVars_p.recoE_p[treeVars_p.nRecoE]      = eleIt -> p();
    treeVars_p.recoE_pt[treeVars_p.nRecoE]     = eleIt -> pt();
    treeVars_p.recoE_mass[treeVars_p.nRecoE]   = eleIt -> mass();
    treeVars_p.recoE_phi[treeVars_p.nRecoE]    = eleIt -> phi();
    treeVars_p.recoE_eta[treeVars_p.nRecoE]    = eleIt -> eta();
    treeVars_p.recoE_charge[treeVars_p.nRecoE] = eleIt -> charge();
    
    treeVars_p.recoE_SuperClusterEnergy[treeVars_p.nRecoE] = eleIt -> caloEnergy();
    treeVars_p.recoE_HOverEm[treeVars_p.nRecoE]            = eleIt -> hadronicOverEm();
    
    const GsfElectronRef eleRef(hRecoElectrons, treeVars_p.nRecoE);
    
    treeVars_p.recoE_tkIso[treeVars_p.nRecoE]  =  (*hRecoElectronTkIsolations)[eleRef];
    treeVars_p.recoE_emIso[treeVars_p.nRecoE]  =  (*hRecoElectronEmIsolations)[eleRef];
    treeVars_p.recoE_hadIso[treeVars_p.nRecoE] = (*hRecoElectronHadIsolations)[eleRef];
    
    
    treeVars_p.recoE_eleIdLoose[treeVars_p.nRecoE]       =       (*hRecoElectronIdLoose)[eleRef];
    treeVars_p.recoE_eleIdTight[treeVars_p.nRecoE]       =       (*hRecoElectronIdTight)[eleRef];
    treeVars_p.recoE_eleIdRobustLoose[treeVars_p.nRecoE] = (*hRecoElectronIdRobustLoose)[eleRef];
    treeVars_p.recoE_eleIdRobustTight[treeVars_p.nRecoE] = (*hRecoElectronIdRobustTight)[eleRef];
    
    ++treeVars_p.nRecoE;
  }
  
  
  
  
  
  // mc muons
  treeVars_p.nMcMu = 0;
  for(unsigned muIt = 0; muIt < (mcAnalysis.GetMcMu()).size(); ++muIt)
  {
    if(treeVars_p.nMcMu >= MCMUMAX) break;
    
    treeVars_p.mcMu_energy[treeVars_p.nMcMu] = ((mcAnalysis.GetMcMu()).at(muIt)) -> energy();
    treeVars_p.mcMu_px[treeVars_p.nMcMu]     = ((mcAnalysis.GetMcMu()).at(muIt)) -> px();
    treeVars_p.mcMu_py[treeVars_p.nMcMu]     = ((mcAnalysis.GetMcMu()).at(muIt)) -> py();
    treeVars_p.mcMu_pz[treeVars_p.nMcMu]     = ((mcAnalysis.GetMcMu()).at(muIt)) -> pz();
    treeVars_p.mcMu_p[treeVars_p.nMcMu]      = ((mcAnalysis.GetMcMu()).at(muIt)) -> p();
    treeVars_p.mcMu_pt[treeVars_p.nMcMu]     = ((mcAnalysis.GetMcMu()).at(muIt)) -> pt();
    treeVars_p.mcMu_mass[treeVars_p.nMcMu]   = ((mcAnalysis.GetMcMu()).at(muIt)) -> mass();
    treeVars_p.mcMu_phi[treeVars_p.nMcMu]    = ((mcAnalysis.GetMcMu()).at(muIt)) -> phi();
    treeVars_p.mcMu_eta[treeVars_p.nMcMu]    = ((mcAnalysis.GetMcMu()).at(muIt)) -> eta();
    treeVars_p.mcMu_charge[treeVars_p.nMcMu] = ((mcAnalysis.GetMcMu()).at(muIt)) -> charge();
    
    ++treeVars_p.nMcMu;
  }  
  
  // reco muons
  treeVars_p.nRecoMu = 0;
  for(reco::MuonCollection::const_iterator muIt = hRecoMuons -> begin();
      muIt != hRecoMuons -> end(); ++muIt)
  {
    if(!muIt -> isGlobalMuon()) continue;
    if(treeVars_p.nRecoMu >= RECOMUMAX) break;
    
    treeVars_p.recoMu_energy[treeVars_p.nRecoMu] = muIt -> energy();
    treeVars_p.recoMu_px[treeVars_p.nRecoMu]     = muIt -> px();
    treeVars_p.recoMu_py[treeVars_p.nRecoMu]     = muIt -> py();
    treeVars_p.recoMu_pz[treeVars_p.nRecoMu]     = muIt -> pz();
    treeVars_p.recoMu_p[treeVars_p.nRecoMu]      = muIt -> p();
    treeVars_p.recoMu_pt[treeVars_p.nRecoMu]     = muIt -> pt();
    treeVars_p.recoMu_mass[treeVars_p.nRecoMu]   = muIt -> mass();
    treeVars_p.recoMu_phi[treeVars_p.nRecoMu]    = muIt -> phi();
    treeVars_p.recoMu_eta[treeVars_p.nRecoMu]    = muIt -> eta();
    treeVars_p.recoMu_charge[treeVars_p.nRecoMu] = muIt -> charge();
    
    treeVars_p.recoMu_tkIsoR03[treeVars_p.nRecoMu]      = (muIt -> isolationR03()).sumPt;
    //treeVars_p.recoMu_tkIsoR03Veto[treeVars_p.nRecoMu]  = (muIt -> isolationR03()).trackerVetoP;
    treeVars_p.recoMu_nTkIsoR03[treeVars_p.nRecoMu]     = (muIt -> isolationR03()).nTracks;    
    treeVars_p.recoMu_emIsoR03[treeVars_p.nRecoMu]      = (muIt -> isolationR03()).emEt;
    //treeVars_p.recoMu_emIsoR03Veto[treeVars_p.nRecoMu]  = (muIt -> isolationR03()).emVetoEt;
    treeVars_p.recoMu_hadIsoR03[treeVars_p.nRecoMu]     = (muIt -> isolationR03()).hadEt;
    //treeVars_p.recoMu_hadIsoR03Veto[treeVars_p.nRecoMu] = (muIt -> isolationR03()).hadVetoEt;
    
    treeVars_p.recoMu_tkIsoR05[treeVars_p.nRecoMu]      = (muIt -> isolationR05()).sumPt;
    //treeVars_p.recoMu_tkIsoR05Veto[treeVars_p.nRecoMu]  = (muIt -> isolationR05()).trackerVetoP;
    treeVars_p.recoMu_nTkIsoR05[treeVars_p.nRecoMu]     = (muIt -> isolationR05()).nTracks;    
    treeVars_p.recoMu_emIsoR05[treeVars_p.nRecoMu]      = (muIt -> isolationR05()).emEt;
    //treeVars_p.recoMu_emIsoR05Veto[treeVars_p.nRecoMu]  = (muIt -> isolationR05()).emVetoEt;
    treeVars_p.recoMu_hadIsoR05[treeVars_p.nRecoMu]     = (muIt -> isolationR05()).hadEt;
    //treeVars_p.recoMu_hadIsoR05Veto[treeVars_p.nRecoMu] = (muIt -> isolationR05()).hadVetoEt;

    ++treeVars_p.nRecoMu;
  }
  
  
  
  // mc jets
  treeVars_p.nMcJet = 0;
  for(reco::GenJetCollection::const_iterator jetIt = hGenJets -> begin();
      jetIt != hGenJets -> end(); ++jetIt)
  {
    if(treeVars_p.nMcJet >= MCJETMAX) break;
    
    treeVars_p.mcJet_energy[treeVars_p.nMcJet] = jetIt -> energy();
    treeVars_p.mcJet_px[treeVars_p.nMcJet]     = jetIt -> px();
    treeVars_p.mcJet_py[treeVars_p.nMcJet]     = jetIt -> py();
    treeVars_p.mcJet_pz[treeVars_p.nMcJet]     = jetIt -> pz();
    treeVars_p.mcJet_p[treeVars_p.nMcJet]      = jetIt -> p();
    treeVars_p.mcJet_pt[treeVars_p.nMcJet]     = jetIt -> pt();
    treeVars_p.mcJet_mass[treeVars_p.nMcJet]   = jetIt -> mass();
    treeVars_p.mcJet_phi[treeVars_p.nMcJet]    = jetIt -> phi();
    treeVars_p.mcJet_eta[treeVars_p.nMcJet]    = jetIt -> eta();
    treeVars_p.mcJet_charge[treeVars_p.nMcJet] = jetIt -> charge();
    
    ++treeVars_p.nMcJet;
  }
  
  
  // reco jets
  treeVars_p.nRecoJet = 0;
  for(reco::CaloJetCollection::const_iterator jetIt = hRecoJets -> begin();
      jetIt != hRecoJets -> end(); ++jetIt)
  {
    if(treeVars_p.nRecoJet >= RECOJETMAX) break;
    
    treeVars_p.recoJet_energy[treeVars_p.nRecoJet] = jetIt -> energy();
    treeVars_p.recoJet_px[treeVars_p.nRecoJet]     = jetIt -> px();
    treeVars_p.recoJet_py[treeVars_p.nRecoJet]     = jetIt -> py();
    treeVars_p.recoJet_pz[treeVars_p.nRecoJet]     = jetIt -> pz();
    treeVars_p.recoJet_p[treeVars_p.nRecoJet]      = jetIt -> p();
    treeVars_p.recoJet_pt[treeVars_p.nRecoJet]     = jetIt -> pt();
    treeVars_p.recoJet_mass[treeVars_p.nRecoJet]   = jetIt -> mass();
    treeVars_p.recoJet_phi[treeVars_p.nRecoJet]    = jetIt -> phi();
    treeVars_p.recoJet_eta[treeVars_p.nRecoJet]    = jetIt -> eta();
    treeVars_p.recoJet_charge[treeVars_p.nRecoJet] = jetIt -> charge();
    
    ++treeVars_p.nRecoJet;
  }
  
  
  
  tree_p -> Fill();
}
