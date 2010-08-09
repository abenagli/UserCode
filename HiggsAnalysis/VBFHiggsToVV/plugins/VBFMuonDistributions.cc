#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonDistributions.h"

#define PI 3.141592654






VBFMuonDistributions::VBFMuonDistributions(const edm::ParameterSet& iConfig):
 m_srcGenParticles(iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 m_srcMuons       (iConfig.getParameter<edm::InputTag>("srcMuons")),
 m_tkIsoCoeff     (iConfig.getParameter<double>("tkIsoCoeff")), 
 m_emIsoCoeff     (iConfig.getParameter<double>("tkIsoCoeff")), 
 m_hadIsoCoeff    (iConfig.getParameter<double>("tkIsoCoeff")), 
 m_doRefCheck     (iConfig.getParameter<bool>("doRefCheck")),
 m_srcMuonsRef(iConfig.getParameter<edm::InputTag>("srcMuonsRef")),
 m_fileName (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFMuonDistributions"))),
 m_verbosity(iConfig.getUntrackedParameter<bool>("verbosity", true)),
 m_eventType(iConfig.getUntrackedParameter<int>("eventType", 0)),
 m_eventId  (0)
{}






VBFMuonDistributions::~VBFMuonDistributions()
{}







void VBFMuonDistributions::beginJob(const edm::EventSetup& iSetup)
{
  // muon histograms
  recoMuNDistr = new TH1F("recoMuNDistr", "recoMuNDistr", 25, 0., 25.);
  
  recoMuPtDistr = new TH1F("recoMuPtDistr", "recoMuPtDistr", 1000, 0., 500.);
  recoMuEtaDistr = new TH1F("recoMuEtaDistr", "recoMuEtaDistr", 1000, -5., 5.);
  
  recoMuTkIsoDistr       = new TH1F("recoMuTkIsoDistr",       "recoMuTkIsoDistr",       1000, 0., 100.);
  recoMuEmIsoDistr       = new TH1F("recoMuEmIsoDistr",       "recoMuEmIsoDistr",       1000, 0., 100.);
  recoMuHadIsoDistr      = new TH1F("recoMuHadIsoDistr",      "recoMuHadIsoDistr",      1000, 0., 100.);
  recoMuCombinedIsoDistr = new TH1F("recoMuCombinedIsoDistr", "recoMuCombinedIsoDistr", 3000, 0., 300.);
  recoMuTkIsoOverPtDistr       = new TH1F("recoMuTkIsoOverPtDistr",       "recoMuTkIsoOverPtDistr",       1000, 0., 10.);
  recoMuEmIsoOverPtDistr       = new TH1F("recoMuEmIsoOverPtDistr",       "recoMuEmIsoOverPtDistr",       1000, 0., 10.);
  recoMuHadIsoOverPtDistr      = new TH1F("recoMuHadIsoOverPtDistr",      "recoMuHadIsoOverPtDistr",      1000, 0., 10.);
  recoMuCombinedIsoOverPtDistr = new TH1F("recoMuCombinedIsoOverPtDistr", "recoMuCombinedIsoOverPtDistr", 3000, 0., 30.);
  
  
  
  matchingRecoMuNDistr = new TH1F("matchingRecoMuNDistr", "matchingRecoMuNDistr", 25, 0., 25.);
  
  matchingRecoMuPtDistr = new TH1F("matchingRecoMuPtDistr", "matchingRecoMuPtDistr", 1000, 0., 500.);
  matchingRecoMuEtaDistr = new TH1F("matchingRecoMuEtaDistr", "matchingRecoMuEtaDistr", 1000, -5., 5.);
  
  matchingRecoMuTkIsoDistr       = new TH1F("matchingRecoMuTkIsoDistr",       "matchingRecoMuTkIsoDistr",       1000, 0., 100.);
  matchingRecoMuEmIsoDistr       = new TH1F("matchingRecoMuEmIsoDistr",       "matchingRecoMuEmIsoDistr",       1000, 0., 100.);
  matchingRecoMuHadIsoDistr      = new TH1F("matchingRecoMuHadIsoDistr",      "matchingRecoMuHadIsoDistr",      1000, 0., 100.);
  matchingRecoMuCombinedIsoDistr = new TH1F("matchingRecoMuCombinedIsoDistr", "matchingRecoMuCombinedIsoDistr", 3000, 0., 300.);
  matchingRecoMuTkIsoOverPtDistr       = new TH1F("matchingRecoMuTkIsoOverPtDistr",       "matchingRecoMuTkIsoOverPtDistr",       1000, 0., 10.);
  matchingRecoMuEmIsoOverPtDistr       = new TH1F("matchingRecoMuEmIsoOverPtDistr",       "matchingRecoMuEmIsoOverPtDistr",       1000, 0., 10.);
  matchingRecoMuHadIsoOverPtDistr      = new TH1F("matchingRecoMuHadIsoOverPtDistr",      "matchingRecoMuHadIsoOverPtDistr",      1000, 0., 10.);
  matchingRecoMuCombinedIsoOverPtDistr = new TH1F("matchingRecoMuCombinedIsoOverPtDistr", "matchingRecoMuCombinedIsoOverPtDistr", 3000, 0., 30.);
}






void VBFMuonDistributions::endJob()
{
  if(m_verbosity)
    std::cerr << "*** VBFMuonDistributions::endJob ***" << std::endl;
  
  
  
  // Create File
  m_outFile = new TFile(m_fileName.c_str() , "RECREATE");
  m_outFile -> cd();
  
  
  
  m_outFile -> mkdir("recoMuonDistr");  
  m_outFile -> cd("recoMuonDistr");  
  
  WriteNormalized(recoMuNDistr);
  
  WriteNormalized(recoMuPtDistr); 
  WriteNormalized(recoMuEtaDistr); 
  
  WriteNormalized(recoMuTkIsoDistr); 
  WriteNormalized(recoMuEmIsoDistr); 
  WriteNormalized(recoMuHadIsoDistr); 
  WriteNormalized(recoMuCombinedIsoDistr); 
  WriteNormalized(recoMuTkIsoOverPtDistr); 
  WriteNormalized(recoMuEmIsoOverPtDistr); 
  WriteNormalized(recoMuHadIsoOverPtDistr); 
  WriteNormalized(recoMuCombinedIsoOverPtDistr); 
  
  m_outFile -> cd();    
  
  
  
  m_outFile -> mkdir("matchingRecoMuonDistr");  
  m_outFile -> cd("matchingRecoMuonDistr");  
  
  WriteNormalized(matchingRecoMuNDistr);
  
  WriteNormalized(matchingRecoMuPtDistr); 
  WriteNormalized(matchingRecoMuEtaDistr); 
  
  WriteNormalized(matchingRecoMuTkIsoDistr); 
  WriteNormalized(matchingRecoMuEmIsoDistr); 
  WriteNormalized(matchingRecoMuHadIsoDistr); 
  WriteNormalized(matchingRecoMuCombinedIsoDistr); 
  WriteNormalized(matchingRecoMuTkIsoOverPtDistr); 
  WriteNormalized(matchingRecoMuEmIsoOverPtDistr); 
  WriteNormalized(matchingRecoMuHadIsoOverPtDistr); 
  WriteNormalized(matchingRecoMuCombinedIsoOverPtDistr); 
  
  m_outFile -> cd();    
  
  
  
  m_outFile -> Close();
}






void VBFMuonDistributions::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // set event id
  ++m_eventId;
  
  if( (m_eventId%100 == 0) && (m_verbosity == true) )
    std::cout << ">>>>>> VBFMuonDistributions::Event number -----> " << std::fixed << std::setw(10) << m_eventId << std::endl;
  
  
  
  
  
  // -----------------------------------------------------------------
  // Get the collections
  // -----------------------------------------------------------------  

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(m_srcGenParticles, genParticles);
  if( !(genParticles.isValid ()) )
  {
    std::cerr << ">>>>>> VBFMuonDistributions::Warning: " << m_srcGenParticles << " not available" << std::endl;
    return;
  }
  
  
  
  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(m_srcMuons, muons);
  if( !(muons.isValid()) )
  {
    std::cerr << ">>>>>> VBFMuonDistributions::Warning: " << m_srcMuons << " not available" << std::endl;
    return;
  }
  
  edm::Handle< edm::RefVector<reco::MuonCollection> > muonsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcMuonsRef, muonsRef);
  
  
  
  
  
  
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Fill the histograms
  // -----------------------------------------------------------------  
  
  
  
  int recoMuN = 0;  
  std::vector<MuonRef> recoMuons;
  for(unsigned int ii = 0; ii < muons -> size(); ++ii)
  {
    // do the reference check
    bool isRefCheckOk = true;
    MuonRef muonRef(muons, ii);
    if(m_doRefCheck)
      if(find(muonsRef -> begin(), muonsRef -> end(), muonRef) == muonsRef -> end())
        isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    ++recoMuN;    
    recoMuons.push_back(muonRef);
    
    
    
    // pt distribution
    recoMuPtDistr  -> Fill( muonRef -> pt() );
    recoMuEtaDistr -> Fill( muonRef -> eta() );
    
      
    // isolation distributions
    double tkIsoVal  = (muonRef -> isolationR03()).sumPt;
    double emIsoVal  = (muonRef -> isolationR03()).emEt;
    double hadIsoVal = (muonRef -> isolationR03()).hadEt;
    double combinedIsoVal = tkIsoVal + emIsoVal + hadIsoVal;
    
    recoMuTkIsoDistr  -> Fill( tkIsoVal );
    recoMuEmIsoDistr  -> Fill( emIsoVal );
    recoMuHadIsoDistr -> Fill( hadIsoVal );
    recoMuCombinedIsoDistr -> Fill( combinedIsoVal );
    
    recoMuTkIsoOverPtDistr  -> Fill(  tkIsoVal / muonRef -> pt() );
    recoMuEmIsoOverPtDistr  -> Fill(  emIsoVal / muonRef -> pt() );
    recoMuHadIsoOverPtDistr -> Fill( hadIsoVal / muonRef -> pt() );
    recoMuCombinedIsoOverPtDistr -> Fill( combinedIsoVal / muonRef -> pt() );
  }
  
  recoMuNDistr  -> Fill( recoMuN );
    
  
  
  
  
  
  // MCDumperVBF
  if(m_eventType != 0) return;
  MCDumperVBF mcAnalysis(genParticles, 0, false);
  bool isValid = mcAnalysis.isValid();
  if(!isValid) return;
  
  
  
  // Get muons from V decay (if any)
  std::vector<const reco::Candidate*> mcMuons;
  
  const reco::Candidate* mcL1_fromV1 = mcAnalysis.mcF1_fromV1();
  if( abs(mcL1_fromV1 -> pdgId()) == 13 )
    mcMuons.push_back(mcL1_fromV1);
  
  const reco::Candidate* mcL2_fromV1 = mcAnalysis.mcF2_fromV1();
  if( abs(mcL2_fromV1 -> pdgId()) == 13 )
    mcMuons.push_back(mcL2_fromV1);
  
  const reco::Candidate* mcL1_fromV2 = mcAnalysis.mcF1_fromV2();
  if( abs(mcL1_fromV2 -> pdgId()) == 13 )
    mcMuons.push_back(mcL1_fromV2);
  
  const reco::Candidate* mcL2_fromV2 = mcAnalysis.mcF2_fromV2();
  if( abs(mcL2_fromV2 -> pdgId()) == 13 )
    mcMuons.push_back(mcL2_fromV2);
  
  

  // Matching between recoMuons and muons from V decay  
  std::vector<int> matchRecoMuIt;
  GetMatching(recoMuons, mcMuons, 0.05, true, 0.1, 10., &matchRecoMuIt);
  
  
  for(unsigned int ii = 0; ii < matchRecoMuIt.size(); ++ii)
  {
    int jj = matchRecoMuIt.at(ii);
    if(jj == -1) continue;
    MuonRef matchingMuonRef = recoMuons.at(jj);    
    
    
    

    // pt distribution
    matchingRecoMuPtDistr  -> Fill( matchingMuonRef -> pt() );
    matchingRecoMuEtaDistr -> Fill( matchingMuonRef -> eta() );
    
      
    // isolation distributions
    double tkIsoVal  = (matchingMuonRef -> isolationR03()).sumPt;
    double emIsoVal  = (matchingMuonRef -> isolationR03()).emEt;
    double hadIsoVal = (matchingMuonRef -> isolationR03()).hadEt;
    double combinedIsoVal = tkIsoVal + emIsoVal + hadIsoVal;
    
    matchingRecoMuTkIsoDistr  -> Fill( tkIsoVal );
    matchingRecoMuEmIsoDistr  -> Fill( emIsoVal );
    matchingRecoMuHadIsoDistr -> Fill( hadIsoVal );
    matchingRecoMuCombinedIsoDistr -> Fill( combinedIsoVal );
    
    matchingRecoMuTkIsoOverPtDistr  -> Fill(  tkIsoVal / matchingMuonRef -> pt() );
    matchingRecoMuEmIsoOverPtDistr  -> Fill(  emIsoVal / matchingMuonRef -> pt() );
    matchingRecoMuHadIsoOverPtDistr -> Fill( hadIsoVal / matchingMuonRef -> pt() );
    matchingRecoMuCombinedIsoOverPtDistr -> Fill( combinedIsoVal / matchingMuonRef -> pt() );
  }
  
  matchingRecoMuNDistr -> Fill(matchRecoMuIt.size());
  
  
}
