#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFLeptonDistributions.h"

#define PI 3.141592654






VBFLeptonDistributions::VBFLeptonDistributions(const edm::ParameterSet& iConfig):
 srcGenParticles_p             (iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 srcRecoElectrons_p            (iConfig.getParameter<edm::InputTag>("srcRecoElectrons")),
 srcRecoElectronTkIsolations_p (iConfig.getParameter<edm::InputTag>("srcRecoElectronTkIsolations")),
 srcRecoElectronEmIsolations_p (iConfig.getParameter<edm::InputTag>("srcRecoElectronEmIsolations")),
 srcRecoElectronHadIsolations_p(iConfig.getParameter<edm::InputTag>("srcRecoElectronHadIsolations")),
 srcRecoElectronIdLoose_p      (iConfig.getParameter<edm::InputTag>("srcRecoElectronIdLoose")),
 srcRecoElectronIdTight_p      (iConfig.getParameter<edm::InputTag>("srcRecoElectronIdTight")),
 srcRecoElectronIdRobustLoose_p(iConfig.getParameter<edm::InputTag>("srcRecoElectronIdRobustLoose")),
 srcRecoElectronIdRobustTight_p(iConfig.getParameter<edm::InputTag>("srcRecoElectronIdRobustTight")),
 srcRecoMuons_p                (iConfig.getParameter<edm::InputTag>("srcRecoMuons")),
 fileName_p                    (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFLeptonDistributions"))),
 verbosity_p                   (iConfig.getUntrackedParameter<bool>("verbosity", true)),
 eventsToPrint_p               (iConfig.getUntrackedParameter<int>("eventsToPrint", 0)),
 eventType_p                   (iConfig.getUntrackedParameter<int>("eventType", 0)),
 eventId_p(0)
{}






VBFLeptonDistributions::~VBFLeptonDistributions()
{}







void VBFLeptonDistributions::beginJob(const edm::EventSetup& iSetup)
{
  // electron histograms
  matchedRecoEPtDistr = new TH1F("matchedRecoEPtDistr", "matchedRecoEPtDistr", 1000, 0., 500.);
    
  matchedRecoETkIsoDistr   = new TH1F("matchedRecoETkIsoDistr", "matchedRecoETkIsoDistr",  1000, 0., 1.);
  matchedRecoEEmIsoDistr  = new TH1F("matchedRecoEEmIsoDistr",  "matchedRecoEEmIsoDistr",  1000, 0., 1.);
  matchedRecoEHadIsoDistr = new TH1F("matchedRecoEHadIsoDistr", "matchedRecoEHadIsoDistr", 1000, 0., 1.);
  
  matchedRecoEEleIdLooseDistr       = new TH1F("matchedRecoEEleIdLoose",       "matchedRecoEEleIdLooseDistr",       2, 0., 2.);
  matchedRecoEEleIdTightDistr       = new TH1F("matchedRecoEEleIdTight",       "matchedRecoEEleIdTightDistr",       2, 0., 2.);
  matchedRecoEEleIdRobustLooseDistr = new TH1F("matchedRecoEEleIdRobustLoose", "matchedRecoEEleIdRobustLooseDistr", 2, 0., 2.);
  matchedRecoEEleIdRobustTightDistr = new TH1F("matchedRecoEEleIdRobustTight", "matchedRecoEEleIdRobustTightDistr", 2, 0., 2.);
  
  
  
  // muon histograms
  matchedRecoMuPtDistr = new TH1F("matchedRecoMuPtDistr", "matchedRecoMuPtDistr", 1000, 0., 500.);
  
  matchedRecoMuTkIsoDistr  = new TH1F("matchedRecoMuTkIsoDistr",  "matchedRecoMuTkIsoDistr",  1000, 0., 1.);
  matchedRecoMuEmIsoDistr  = new TH1F("matchedRecoMuEmIsoDistr",  "matchedRecoMuEmIsoDistr",  1000, 0., 1.);
  matchedRecoMuHadIsoDistr = new TH1F("matchedRecoMuHadIsoDistr", "matchedRecoMuHadIsoDistr", 1000, 0., 1.);
}






void VBFLeptonDistributions::endJob()
{
  if(verbosity_p)
    std::cerr << "*** VBFLeptonDistributions::endJob ***" << std::endl;
  
  
  
  // Create File
  outFile_p = new TFile(fileName_p.c_str() , "RECREATE");
  outFile_p -> cd();
  
  
  
  // electron histograms
  matchedRecoEPtDistr -> Write(); 
  
  matchedRecoETkIsoDistr  -> Write(); 
  matchedRecoEEmIsoDistr  -> Write(); 
  matchedRecoEHadIsoDistr -> Write(); 
  
  matchedRecoEEleIdLooseDistr       -> Write(); 
  matchedRecoEEleIdTightDistr       -> Write(); 
  matchedRecoEEleIdRobustLooseDistr -> Write(); 
  matchedRecoEEleIdRobustTightDistr -> Write(); 
  
  
  
  // muon histograms
  matchedRecoMuPtDistr -> Write(); 
  
  matchedRecoMuTkIsoDistr  -> Write(); 
  matchedRecoMuEmIsoDistr  -> Write(); 
  matchedRecoMuHadIsoDistr -> Write(); 
  
  
  
  outFile_p -> Close();
}






void VBFLeptonDistributions::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // set event id
  ++eventId_p;
  
  if( (eventId_p%100 == 0) && (verbosity_p == true) )
    std::cout << ">>>>>> VBFLeptonDistributions::Event number -----> " << std::fixed << std::setw(10) << eventId_p << std::endl;
  
  
  
  
  
  // -----------------------------------------------------------------
  // Get the collections
  // -----------------------------------------------------------------  

  edm::Handle<reco::GenParticleCollection> hGenParticles;
  iEvent.getByLabel(srcGenParticles_p, hGenParticles);
  if( !(hGenParticles.isValid ()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcGenParticles_p << " not available" << std::endl;
    return;
  }
  
  edm::ESHandle<ParticleDataTable> hPdt;
  iSetup.getData(hPdt);
  
  edm::Handle<reco::PixelMatchGsfElectronCollection> hRecoElectrons;
  iEvent.getByLabel(srcRecoElectrons_p, hRecoElectrons);
  if( !(hRecoElectrons.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcRecoElectrons_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > hRecoElectronTkIsolations;
  iEvent.getByLabel(srcRecoElectronTkIsolations_p, hRecoElectronTkIsolations);
  if( !(hRecoElectronTkIsolations.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcRecoElectronTkIsolations_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > hRecoElectronEmIsolations;
  iEvent.getByLabel(srcRecoElectronEmIsolations_p, hRecoElectronEmIsolations);
 if( !(hRecoElectronEmIsolations.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcRecoElectronEmIsolations_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > hRecoElectronHadIsolations;
  iEvent.getByLabel(srcRecoElectronHadIsolations_p, hRecoElectronHadIsolations);
  if( !(hRecoElectronHadIsolations.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcRecoElectronHadIsolations_p << " not available" << std::endl;
    return;
  }

  edm::Handle<edm::ValueMap<float> > hRecoElectronIdLoose;
  iEvent.getByLabel(srcRecoElectronIdLoose_p, hRecoElectronIdLoose);
  if( !(hRecoElectronIdLoose.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcRecoElectronIdLoose_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > hRecoElectronIdTight;
  iEvent.getByLabel(srcRecoElectronIdTight_p, hRecoElectronIdTight);
  if( !(hRecoElectronIdTight.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcRecoElectronIdTight_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > hRecoElectronIdRobustLoose;
  iEvent.getByLabel(srcRecoElectronIdRobustLoose_p, hRecoElectronIdRobustLoose);
  if( !(hRecoElectronIdRobustLoose.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcRecoElectronIdRobustLoose_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > hRecoElectronIdRobustTight;
  iEvent.getByLabel(srcRecoElectronIdRobustTight_p, hRecoElectronIdRobustTight);
  if( !(hRecoElectronIdRobustTight.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcRecoElectronIdRobustTight_p << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::MuonCollection> hRecoMuons;
  iEvent.getByLabel (srcRecoMuons_p, hRecoMuons);
  if( !(hRecoMuons.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << srcRecoMuons_p << " not available" << std::endl;
    return;
  } 
  
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Fill the tree
  // -----------------------------------------------------------------  
  
  
  
  // mc variables
  
  MCAnalysis mcAnalysis(hGenParticles, hPdt, false);
  bool isMCAnalysisValid = mcAnalysis.isValid();
  if(!isMCAnalysisValid) return;
  
  
  
  // Get leptons from V1 (if any)
  const reco::Candidate* mcL_fromV1 = mcAnalysis.mcF_fromV1();
  const reco::Candidate* mcNu_fromV1 = mcAnalysis.mcFbar_fromV1();
  if( ( (abs(mcL_fromV1 -> pdgId()) == 12) ||
        (abs(mcL_fromV1 -> pdgId()) == 14) ||
        (abs(mcL_fromV1 -> pdgId()) == 16) ) && 
      ( (abs(mcNu_fromV1 -> pdgId()) == 11) ||
        (abs(mcNu_fromV1 -> pdgId()) == 13) ||
        (abs(mcNu_fromV1 -> pdgId()) == 15) ) )
  {
    mcL_fromV1 = mcAnalysis.mcFbar_fromV1();
    mcNu_fromV1 = mcAnalysis.mcF_fromV1();
  }
  
  
  
  // Get leptons from V2 (if any)
  const reco::Candidate* mcL_fromV2 = mcAnalysis.mcF_fromV2();
  const reco::Candidate* mcNu_fromV2 = mcAnalysis.mcFbar_fromV2();
  if( ( (abs(mcL_fromV2 -> pdgId()) == 12) ||
        (abs(mcL_fromV2 -> pdgId()) == 14) ||
        (abs(mcL_fromV2 -> pdgId()) == 16) ) && 
      ( (abs(mcNu_fromV2 -> pdgId()) == 11) ||
        (abs(mcNu_fromV2 -> pdgId()) == 13) ||
        (abs(mcNu_fromV2 -> pdgId()) == 15) ) )
  {
    mcL_fromV2 = mcAnalysis.mcFbar_fromV2();
    mcNu_fromV2 = mcAnalysis.mcF_fromV2();
  }
  
  
  
  
    
  
  // Matching between recoElectrons and mcElectrons
  std::vector<reco::GsfElectronCollection::const_iterator> recoElectrons;
  for(reco::GsfElectronCollection::const_iterator eleIt = hRecoElectrons -> begin();
      eleIt != hRecoElectrons -> end(); ++eleIt)
    recoElectrons.push_back(eleIt);
  
  std::vector<const reco::Candidate*> mcElectrons;
  if(abs(mcL_fromV1 -> pdgId()) == 11)
    mcElectrons.push_back(mcL_fromV1);
  if(abs(mcL_fromV2 -> pdgId()) == 11)
    mcElectrons.push_back(mcL_fromV2);
  
  
  std::vector<int> matchRecoEIt;
  GetMatching(recoElectrons, mcElectrons, 0.05, true, 0., 100., &matchRecoEIt);
  
  
  for(unsigned int i = 0; i < matchRecoEIt.size(); ++i)
  {
    if(matchRecoEIt.at(i) == -1) continue;
    
    const GsfElectronRef eleRef(hRecoElectrons, matchRecoEIt.at(i));    
    
    
    // pt distribution
    matchedRecoEPtDistr -> Fill( (recoElectrons.at(matchRecoEIt.at(i))) -> pt() );
    
    
    // isolation distributions
    matchedRecoETkIsoDistr  -> Fill( (*hRecoElectronTkIsolations) [eleRef] / (recoElectrons.at(matchRecoEIt.at(i))) -> pt() );
    matchedRecoEEmIsoDistr  -> Fill( (*hRecoElectronEmIsolations) [eleRef] / (recoElectrons.at(matchRecoEIt.at(i))) -> pt() );
    matchedRecoEHadIsoDistr -> Fill( (*hRecoElectronHadIsolations)[eleRef] / (recoElectrons.at(matchRecoEIt.at(i))) -> pt() );
    
    
    // electronId distributions
    matchedRecoEEleIdLooseDistr  -> Fill( (*hRecoElectronIdLoose) [eleRef] );
    matchedRecoEEleIdTightDistr  -> Fill( (*hRecoElectronIdTight) [eleRef] );
    matchedRecoEEleIdRobustLooseDistr  -> Fill( (*hRecoElectronIdRobustLoose) [eleRef] );
    matchedRecoEEleIdRobustTightDistr  -> Fill( (*hRecoElectronIdRobustTight) [eleRef] );
  }
  
  
  
  
  
  
  // Matching between recoMuons and mcMuons
  std::vector<reco::MuonCollection::const_iterator> recoMuons;
  for(reco::MuonCollection::const_iterator muIt = hRecoMuons -> begin();
      muIt != hRecoMuons -> end(); ++muIt)
    recoMuons.push_back(muIt);
  
  std::vector<const reco::Candidate*> mcMuons;
  if(abs(mcL_fromV1 -> pdgId()) == 13)
    mcMuons.push_back(mcL_fromV1);
  if(abs(mcL_fromV2 -> pdgId()) == 13)
    mcMuons.push_back(mcL_fromV2);
  
  
  std::vector<int> matchRecoMuIt;
  GetMatching(recoMuons, mcMuons, 0.05, true, 0., 100., &matchRecoMuIt);
  
  
  for(unsigned int i = 0; i < matchRecoMuIt.size(); ++i)
  {
    if(matchRecoMuIt.at(i) == -1) continue;
    
    
    // pt distribution
    matchedRecoMuPtDistr -> Fill( (recoMuons.at(matchRecoMuIt.at(i))) -> pt() );
    
    
    // isolation distributions
    matchedRecoMuTkIsoDistr  -> Fill( (recoMuons.at(matchRecoMuIt.at(i)) -> isolationR03()).sumPt / (recoMuons.at(matchRecoMuIt.at(i))) -> pt());
    matchedRecoMuEmIsoDistr  -> Fill( (recoMuons.at(matchRecoMuIt.at(i)) -> isolationR03()).emEt  / (recoMuons.at(matchRecoMuIt.at(i))) -> pt());
    matchedRecoMuHadIsoDistr -> Fill( (recoMuons.at(matchRecoMuIt.at(i)) -> isolationR03()).hadEt / (recoMuons.at(matchRecoMuIt.at(i))) -> pt());
  }
  
  

}
