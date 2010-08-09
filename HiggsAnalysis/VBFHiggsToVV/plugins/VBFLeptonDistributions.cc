#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFLeptonDistributions.h"

#define PI 3.141592654






VBFLeptonDistributions::VBFLeptonDistributions(const edm::ParameterSet& iConfig):
 m_srcGenParticles               (iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 m_srcElectrons                  (iConfig.getParameter<edm::InputTag>("srcElectrons")),
 m_srcElectronTkIsoValues        (iConfig.getParameter<edm::InputTag>("srcElectronTkIsoValues")),
 m_srcElectronEmIsoValues        (iConfig.getParameter<edm::InputTag>("srcElectronEmIsoValues")),
 m_srcElectronHadIsoValues       (iConfig.getParameter<edm::InputTag>("srcElectronHadIsoValues")),
 m_srcElectronIdLooseValues      (iConfig.getParameter<edm::InputTag>("srcElectronIdLooseValues")),
 m_srcElectronIdTightValues      (iConfig.getParameter<edm::InputTag>("srcElectronIdTightValues")),
 m_srcElectronIdRobustLooseValues(iConfig.getParameter<edm::InputTag>("srcElectronIdRobustLooseValues")),
 m_srcElectronIdRobustTightValues(iConfig.getParameter<edm::InputTag>("srcElectronIdRobustTightValues")),
 m_srcMuons                      (iConfig.getParameter<edm::InputTag>("srcMuons")),
 m_tkIsoCoeff (iConfig.getParameter<double>("tkIsoCoeff")), 
 m_emIsoCoeff (iConfig.getParameter<double>("tkIsoCoeff")), 
 m_hadIsoCoeff(iConfig.getParameter<double>("tkIsoCoeff")), 
 m_doElectronRefCheck(iConfig.getParameter<bool>("doElectronRefCheck")),
 m_srcElectronsRef   (iConfig.getParameter<edm::InputTag>("srcElectronsRef")),
 m_doMuonRefCheck    (iConfig.getParameter<bool>("doMuonRefCheck")),
 m_srcMuonsRef       (iConfig.getParameter<edm::InputTag>("srcMuonsRef")),
 m_fileName (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFLeptonDistributions"))),
 m_verbosity(iConfig.getUntrackedParameter<bool>("verbosity", true)),
 m_eventType(iConfig.getUntrackedParameter<int>("eventType", 0)),
 m_eventId  (0)
{}






VBFLeptonDistributions::~VBFLeptonDistributions()
{}







void VBFLeptonDistributions::beginJob(const edm::EventSetup& iSetup)
{
  // electron histograms
  recoLNDistr = new TH1F("recoLNDistr", "recoLNDistr", 25, 0., 25.);
  
  recoLPtDistr = new TH1F("recoLPtDistr", "recoLPtDistr", 1000, 0., 500.);
  recoLEtaDistr = new TH1F("recoLEtaDistr", "recoLEtaDistr", 1000, -5., 5.);
  
  recoLTkIsoDistr       = new TH1F("recoLTkIsoDistr",       "recoLTkIsoDistr",       1000, 0., 100.);
  recoLEmIsoDistr       = new TH1F("recoLEmIsoDistr",       "recoLEmIsoDistr",       1000, 0., 100.);
  recoLHadIsoDistr      = new TH1F("recoLHadIsoDistr",      "recoLHadIsoDistr",      1000, 0., 100.);
  recoLCombinedIsoDistr = new TH1F("recoLCombinedIsoDistr", "recoLCombinedIsoDistr", 3000, 0., 300.);
  recoLTkIsoOverPtDistr       = new TH1F("recoLTkIsoOverPtDistr",       "recoLTkIsoOverPtDistr",       1000, 0., 10.);
  recoLEmIsoOverPtDistr       = new TH1F("recoLEmIsoOverPtDistr",       "recoLEmIsoOverPtDistr",       1000, 0., 10.);
  recoLHadIsoOverPtDistr      = new TH1F("recoLHadIsoOverPtDistr",      "recoLHadIsoOverPtDistr",      1000, 0., 10.);
  recoLCombinedIsoOverPtDistr = new TH1F("recoLCombinedIsoOverPtDistr", "recoLCombinedIsoOverPtDistr", 3000, 0., 30.);
  
  recoEEleIdLooseDistr       = new TH1F("recoEEleIdLoose",       "recoEEleIdLooseDistr",       2, 0., 2.);
  recoEEleIdTightDistr       = new TH1F("recoEEleIdTight",       "recoEEleIdTightDistr",       2, 0., 2.);
  recoEEleIdRobustLooseDistr = new TH1F("recoEEleIdRobustLoose", "recoEEleIdRobustLooseDistr", 2, 0., 2.);
  recoEEleIdRobustTightDistr = new TH1F("recoEEleIdRobustTight", "recoEEleIdRobustTightDistr", 2, 0., 2.);
  
  
  
  matchingRecoLNDistr = new TH1F("matchingRecoLNDistr", "matchingRecoLNDistr", 25, 0., 25.);
  
  matchingRecoLPtDistr = new TH1F("matchingRecoLPtDistr", "matchingRecoLPtDistr", 1000, 0., 500.);
  matchingRecoLEtaDistr = new TH1F("matchingRecoLEtaDistr", "matchingRecoLEtaDistr", 1000, -5., 5.);
  
  matchingRecoLTkIsoDistr       = new TH1F("matchingRecoLTkIsoDistr",       "matchingRecoLTkIsoDistr",       1000, 0., 100.);
  matchingRecoLEmIsoDistr       = new TH1F("matchingRecoLEmIsoDistr",       "matchingRecoLEmIsoDistr",       1000, 0., 100.);
  matchingRecoLHadIsoDistr      = new TH1F("matchingRecoLHadIsoDistr",      "matchingRecoLHadIsoDistr",      1000, 0., 100.);
  matchingRecoLCombinedIsoDistr = new TH1F("matchingRecoLCombinedIsoDistr", "matchingRecoLCombinedIsoDistr", 3000, 0., 300.);
  matchingRecoLTkIsoOverPtDistr       = new TH1F("matchingRecoLTkIsoOverPtDistr",       "matchingRecoLTkIsoOverPtDistr",       1000, 0., 10.);
  matchingRecoLEmIsoOverPtDistr       = new TH1F("matchingRecoLEmIsoOverPtDistr",       "matchingRecoLEmIsoOverPtDistr",       1000, 0., 10.);
  matchingRecoLHadIsoOverPtDistr      = new TH1F("matchingRecoLHadIsoOverPtDistr",      "matchingRecoLHadIsoOverPtDistr",      1000, 0., 10.);
  matchingRecoLCombinedIsoOverPtDistr = new TH1F("matchingRecoLCombinedIsoOverPtDistr", "matchingRecoLCombinedIsoOverPtDistr", 3000, 0., 30.);
  
  matchingRecoEEleIdLooseDistr       = new TH1F("matchingRecoEEleIdLoose",       "matchingRecoEEleIdLooseDistr",       2, 0., 2.);
  matchingRecoEEleIdTightDistr       = new TH1F("matchingRecoEEleIdTight",       "matchingRecoEEleIdTightDistr",       2, 0., 2.);
  matchingRecoEEleIdRobustLooseDistr = new TH1F("matchingRecoEEleIdRobustLoose", "matchingRecoEEleIdRobustLooseDistr", 2, 0., 2.);
  matchingRecoEEleIdRobustTightDistr = new TH1F("matchingRecoEEleIdRobustTight", "matchingRecoEEleIdRobustTightDistr", 2, 0., 2.);
}






void VBFLeptonDistributions::endJob()
{
  if(m_verbosity)
    std::cerr << "*** VBFLeptonDistributions::endJob ***" << std::endl;
  
  
  
  // Create File
  m_outFile = new TFile(m_fileName.c_str() , "RECREATE");
  m_outFile -> cd();
  
  
  
  m_outFile -> mkdir("recoLeptonDistr");  
  m_outFile -> cd("recoLeptonDistr");  
  
  WriteNormalized(recoLNDistr);
  
  WriteNormalized(recoLPtDistr); 
  WriteNormalized(recoLEtaDistr); 
  
  WriteNormalized(recoLTkIsoDistr); 
  WriteNormalized(recoLEmIsoDistr); 
  WriteNormalized(recoLHadIsoDistr); 
  WriteNormalized(recoLCombinedIsoDistr); 
  WriteNormalized(recoLTkIsoOverPtDistr); 
  WriteNormalized(recoLEmIsoOverPtDistr); 
  WriteNormalized(recoLHadIsoOverPtDistr); 
  WriteNormalized(recoLCombinedIsoOverPtDistr); 
  
  WriteNormalized(recoEEleIdLooseDistr); 
  WriteNormalized(recoEEleIdTightDistr); 
  WriteNormalized(recoEEleIdRobustLooseDistr); 
  WriteNormalized(recoEEleIdRobustTightDistr); 
  
  m_outFile -> cd();    
  
  
  
  m_outFile -> mkdir("matchingRecoLeptonDistr");  
  m_outFile -> cd("matchingRecoLeptonDistr");  
  
  WriteNormalized(matchingRecoLNDistr);
  
  WriteNormalized(matchingRecoLPtDistr); 
  WriteNormalized(matchingRecoLEtaDistr); 
  
  WriteNormalized(matchingRecoLTkIsoDistr); 
  WriteNormalized(matchingRecoLEmIsoDistr); 
  WriteNormalized(matchingRecoLHadIsoDistr); 
  WriteNormalized(matchingRecoLCombinedIsoDistr); 
  WriteNormalized(matchingRecoLTkIsoOverPtDistr); 
  WriteNormalized(matchingRecoLEmIsoOverPtDistr); 
  WriteNormalized(matchingRecoLHadIsoOverPtDistr); 
  WriteNormalized(matchingRecoLCombinedIsoOverPtDistr); 
  
  WriteNormalized(matchingRecoEEleIdLooseDistr); 
  WriteNormalized(matchingRecoEEleIdTightDistr); 
  WriteNormalized(matchingRecoEEleIdRobustLooseDistr); 
  WriteNormalized(matchingRecoEEleIdRobustTightDistr); 
  
  m_outFile -> cd();    
  
  
  
  m_outFile -> Close();
}






void VBFLeptonDistributions::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // set event id
  ++m_eventId;
  
  if( (m_eventId%100 == 0) && (m_verbosity == true) )
    std::cout << ">>>>>> VBFLeptonDistributions::Event number -----> " << std::fixed << std::setw(10) << m_eventId << std::endl;
  
  
  
  
  
  // -----------------------------------------------------------------
  // Get the collections
  // -----------------------------------------------------------------  

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(m_srcGenParticles, genParticles);
  if( !(genParticles.isValid ()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcGenParticles << " not available" << std::endl;
    return;
  }
  
  
  
  edm::Handle<reco::GsfElectronCollection> electrons;
  iEvent.getByLabel(m_srcElectrons, electrons);
  if( !(electrons.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcElectrons << " not available" << std::endl;
    return;
  }
  
  edm::Handle< edm::RefVector<reco::GsfElectronCollection> > electronsRef;
  if(m_doElectronRefCheck)
    iEvent.getByLabel(m_srcElectronsRef, electronsRef);
  
  
  
  edm::Handle<edm::ValueMap<double> > electronTkIsoValues;
  iEvent.getByLabel(m_srcElectronTkIsoValues, electronTkIsoValues);
  if( !(electronTkIsoValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcElectronTkIsoValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > electronEmIsoValues;
  iEvent.getByLabel(m_srcElectronEmIsoValues, electronEmIsoValues);
 if( !(electronEmIsoValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcElectronEmIsoValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > electronHadIsoValues;
  iEvent.getByLabel(m_srcElectronHadIsoValues, electronHadIsoValues);
  if( !(electronHadIsoValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcElectronHadIsoValues << " not available" << std::endl;
    return;
  }
  
  
  
  edm::Handle<edm::ValueMap<float> > electronIdLooseValues;
  iEvent.getByLabel(m_srcElectronIdLooseValues, electronIdLooseValues);
  if( !(electronIdLooseValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcElectronIdLooseValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > electronIdTightValues;
  iEvent.getByLabel(m_srcElectronIdTightValues, electronIdTightValues);
  if( !(electronIdTightValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcElectronIdTightValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > electronIdRobustLooseValues;
  iEvent.getByLabel(m_srcElectronIdRobustLooseValues, electronIdRobustLooseValues);
  if( !(electronIdRobustLooseValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcElectronIdRobustLooseValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > electronIdRobustTightValues;
  iEvent.getByLabel(m_srcElectronIdRobustTightValues, electronIdRobustTightValues);
  if( !(electronIdRobustTightValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcElectronIdRobustTightValues << " not available" << std::endl;
    return;
  }
  
  
  
  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(m_srcMuons, muons);
  if( !(muons.isValid()) )
  {
    std::cerr << ">>>>>> VBFLeptonDistributions::Warning: " << m_srcMuons << " not available" << std::endl;
    return;
  }
  
  edm::Handle< edm::RefVector<reco::MuonCollection> > muonsRef;
  if(m_doMuonRefCheck)
    iEvent.getByLabel(m_srcMuonsRef, muonsRef);
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Fill the histograms
  // -----------------------------------------------------------------  
  
  
  
  int recoLN = 0;
  
  // ----------
  // electrons
  // ----------
  
  std::vector<GsfElectronRef> recoElectrons;
  for(unsigned int ii = 0; ii < electrons -> size(); ++ii)
  {
    // do the reference check
    bool isRefCheckOk = true;
    GsfElectronRef electronRef(electrons, ii);
    if(m_doElectronRefCheck)
      if(find(electronsRef -> begin(), electronsRef -> end(), electronRef) == electronsRef -> end())
        isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    ++recoLN;    
    recoElectrons.push_back(electronRef);
    
    
    
    // pt distribution
    recoLPtDistr  -> Fill( electronRef -> pt() );
    recoLEtaDistr -> Fill( electronRef -> eta() );
    
      
    // isolation distributions
    recoLTkIsoDistr  -> Fill( (*electronTkIsoValues) [electronRef] );
    recoLEmIsoDistr  -> Fill( (*electronEmIsoValues) [electronRef] );
    recoLHadIsoDistr -> Fill( (*electronHadIsoValues)[electronRef] );
    recoLCombinedIsoDistr -> Fill( ( m_tkIsoCoeff  * (*electronTkIsoValues) [electronRef] + 
                                     m_emIsoCoeff  * (*electronEmIsoValues) [electronRef] + 
                                     m_hadIsoCoeff * (*electronHadIsoValues)[electronRef] ) );
    recoLTkIsoOverPtDistr  -> Fill( (*electronTkIsoValues) [electronRef] / electronRef -> pt() );
    recoLEmIsoOverPtDistr  -> Fill( (*electronEmIsoValues) [electronRef] / electronRef -> pt() );
    recoLHadIsoOverPtDistr -> Fill( (*electronHadIsoValues)[electronRef] / electronRef -> pt() );
    recoLCombinedIsoOverPtDistr -> Fill( ( m_tkIsoCoeff  * (*electronTkIsoValues) [electronRef] + 
                                           m_emIsoCoeff  * (*electronEmIsoValues) [electronRef] + 
                                           m_hadIsoCoeff * (*electronHadIsoValues)[electronRef] ) / electronRef -> pt() );
    
    
    // electronId distributions
    recoEEleIdLooseDistr  -> Fill( (*electronIdLooseValues)[electronRef] );
    recoEEleIdTightDistr  -> Fill( (*electronIdTightValues)[electronRef] );
    recoEEleIdRobustLooseDistr  -> Fill( (*electronIdRobustLooseValues)[electronRef] );
    recoEEleIdRobustTightDistr  -> Fill( (*electronIdRobustTightValues)[electronRef] );
  }
  
  
  
  // -----
  // muons
  // -----  
  
  std::vector<MuonRef> recoMuons;
  for(unsigned int ii = 0; ii < muons -> size(); ++ii)
  {
    // do the reference check
    bool isRefCheckOk = true;
    MuonRef muonRef(muons, ii);
    if(m_doMuonRefCheck)
      if(find(muonsRef -> begin(), muonsRef -> end(), muonRef) == muonsRef -> end())
        isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    ++recoLN;    
    recoMuons.push_back(muonRef);
    
    
    
    // pt distribution
    recoLPtDistr  -> Fill( muonRef -> pt() );
    recoLEtaDistr -> Fill( muonRef -> eta() );
    
      
    // isolation distributions
    double tkIsoVal  = (muonRef -> isolationR03()).sumPt;
    double emIsoVal  = (muonRef -> isolationR03()).emEt;
    double hadIsoVal = (muonRef -> isolationR03()).hadEt;
    double combinedIsoVal = tkIsoVal + emIsoVal + hadIsoVal;
    
    recoLTkIsoDistr  -> Fill( tkIsoVal );
    recoLEmIsoDistr  -> Fill( emIsoVal );
    recoLHadIsoDistr -> Fill( hadIsoVal );
    recoLCombinedIsoDistr -> Fill( combinedIsoVal );
    
    recoLTkIsoOverPtDistr  -> Fill(  tkIsoVal / muonRef -> pt() );
    recoLEmIsoOverPtDistr  -> Fill(  emIsoVal / muonRef -> pt() );
    recoLHadIsoOverPtDistr -> Fill( hadIsoVal / muonRef -> pt() );
    recoLCombinedIsoOverPtDistr -> Fill( combinedIsoVal / muonRef -> pt() );
  }
  
  recoLNDistr  -> Fill( recoLN );
  
  
  
  
  
  
  // MCDumperVBF
  if(m_eventType != 0) return;
  MCDumperVBF mcAnalysis(genParticles, 0, false);
  bool isValid = mcAnalysis.isValid();
  if(!isValid) return;
  
  const reco::Candidate* mcL1_fromV1 = mcAnalysis.mcF1_fromV1();
  const reco::Candidate* mcL2_fromV1 = mcAnalysis.mcF2_fromV1();
  const reco::Candidate* mcL1_fromV2 = mcAnalysis.mcF1_fromV2();
  const reco::Candidate* mcL2_fromV2 = mcAnalysis.mcF2_fromV2();
  
  bool isLeptonic = false;

  if( (abs(mcL1_fromV1 -> pdgId()) == 11) || (abs(mcL1_fromV1 -> pdgId()) == 13) )
    isLeptonic = true;
  if( (abs(mcL2_fromV1 -> pdgId()) == 11) || (abs(mcL2_fromV1 -> pdgId()) == 13) )
    isLeptonic = true;
  if( (abs(mcL1_fromV2 -> pdgId()) == 11) || (abs(mcL1_fromV2 -> pdgId()) == 13) )
    isLeptonic = true;
  if( (abs(mcL2_fromV2 -> pdgId()) == 11) || (abs(mcL2_fromV2 -> pdgId()) == 13) )
    isLeptonic = true;
  
  if(isLeptonic == false)
    return;
  
  
  
  
  
  
  // Get electrons from V decay (if any)
  std::vector<const reco::Candidate*> mcElectrons;
  
  if( abs(mcL1_fromV1 -> pdgId()) == 11 )
    mcElectrons.push_back(mcL1_fromV1);

  if( abs(mcL2_fromV1 -> pdgId()) == 11 )
    mcElectrons.push_back(mcL2_fromV1);
  
  if( abs(mcL1_fromV2 -> pdgId()) == 11 )
    mcElectrons.push_back(mcL1_fromV2);
  
  if( abs(mcL2_fromV2 -> pdgId()) == 11 )
    mcElectrons.push_back(mcL2_fromV2);
  
  
  
  // Get muons from V decay (if any)
  std::vector<const reco::Candidate*> mcMuons;
  
  if( abs(mcL1_fromV1 -> pdgId()) == 13 )
    mcMuons.push_back(mcL1_fromV1);
  
  if( abs(mcL2_fromV1 -> pdgId()) == 13 )
    mcMuons.push_back(mcL2_fromV1);
  
  if( abs(mcL1_fromV2 -> pdgId()) == 13 )
    mcMuons.push_back(mcL1_fromV2);
  
  if( abs(mcL2_fromV2 -> pdgId()) == 13 )
    mcMuons.push_back(mcL2_fromV2);
  
  
  
  
  
  
  // Matching between recoElectrons and electrons from V decay  
  std::vector<int> matchRecoEIt;
  GetMatching(recoElectrons, mcElectrons, 0.05, true, 0.1, 10., &matchRecoEIt);
  
  std::vector<int> matchRecoMuIt;
  GetMatching(recoMuons, mcMuons, 0.05, true, 0.1, 10., &matchRecoMuIt);
  
  
  
  int matchingRecoLN = 0;
  
  // ----------
  // electrons
  // ----------
  
  for(unsigned int ii = 0; ii < matchRecoEIt.size(); ++ii)
  {
    int jj = matchRecoEIt.at(ii);
    if(jj == -1) continue;
    ++matchingRecoLN;
    GsfElectronRef matchingElectronRef = recoElectrons.at(jj);    
    
    
    
    // pt distribution
    matchingRecoLPtDistr  -> Fill( matchingElectronRef -> pt() );
    matchingRecoLEtaDistr -> Fill( matchingElectronRef -> eta() );
    
      
    // isolation distributions
    matchingRecoLTkIsoDistr  -> Fill( (*electronTkIsoValues) [matchingElectronRef] );
    matchingRecoLEmIsoDistr  -> Fill( (*electronEmIsoValues) [matchingElectronRef] );
    matchingRecoLHadIsoDistr -> Fill( (*electronHadIsoValues)[matchingElectronRef] );
    matchingRecoLCombinedIsoDistr -> Fill( ( m_tkIsoCoeff  * (*electronTkIsoValues) [matchingElectronRef] + 
                                     m_emIsoCoeff  * (*electronEmIsoValues) [matchingElectronRef] + 
                                     m_hadIsoCoeff * (*electronHadIsoValues)[matchingElectronRef] ) );
    matchingRecoLTkIsoOverPtDistr  -> Fill( (*electronTkIsoValues) [matchingElectronRef] / matchingElectronRef -> pt() );
    matchingRecoLEmIsoOverPtDistr  -> Fill( (*electronEmIsoValues) [matchingElectronRef] / matchingElectronRef -> pt() );
    matchingRecoLHadIsoOverPtDistr -> Fill( (*electronHadIsoValues)[matchingElectronRef] / matchingElectronRef -> pt() );
    matchingRecoLCombinedIsoOverPtDistr -> Fill( ( m_tkIsoCoeff  * (*electronTkIsoValues) [matchingElectronRef] + 
                                           m_emIsoCoeff  * (*electronEmIsoValues) [matchingElectronRef] + 
                                           m_hadIsoCoeff * (*electronHadIsoValues)[matchingElectronRef] ) / matchingElectronRef -> pt() );
    
    
    // electronId distributions
    matchingRecoEEleIdLooseDistr  -> Fill( (*electronIdLooseValues)[matchingElectronRef] );
    matchingRecoEEleIdTightDistr  -> Fill( (*electronIdTightValues)[matchingElectronRef] );
    matchingRecoEEleIdRobustLooseDistr  -> Fill( (*electronIdRobustLooseValues)[matchingElectronRef] );
    matchingRecoEEleIdRobustTightDistr  -> Fill( (*electronIdRobustTightValues)[matchingElectronRef] );
  }
  
  
  
  // -----
  // muons
  // -----
  
  for(unsigned int ii = 0; ii < matchRecoMuIt.size(); ++ii)
  {
    int jj = matchRecoMuIt.at(ii);
    if(jj == -1) continue;
    ++matchingRecoLN;
    MuonRef matchingMuonRef = recoMuons.at(jj);    
    
    
    
    // pt distribution
    matchingRecoLPtDistr  -> Fill( matchingMuonRef -> pt() );
    matchingRecoLEtaDistr -> Fill( matchingMuonRef -> eta() );
    
      
    // isolation distributions
    double tkIsoVal  = (matchingMuonRef -> isolationR03()).sumPt;
    double emIsoVal  = (matchingMuonRef -> isolationR03()).emEt;
    double hadIsoVal = (matchingMuonRef -> isolationR03()).hadEt;
    double combinedIsoVal = tkIsoVal + emIsoVal + hadIsoVal;
    
    matchingRecoLTkIsoDistr  -> Fill( tkIsoVal );
    matchingRecoLEmIsoDistr  -> Fill( emIsoVal );
    matchingRecoLHadIsoDistr -> Fill( hadIsoVal );
    matchingRecoLCombinedIsoDistr -> Fill( combinedIsoVal );
    
    matchingRecoLTkIsoOverPtDistr  -> Fill(  tkIsoVal / matchingMuonRef -> pt() );
    matchingRecoLEmIsoOverPtDistr  -> Fill(  emIsoVal / matchingMuonRef -> pt() );
    matchingRecoLHadIsoOverPtDistr -> Fill( hadIsoVal / matchingMuonRef -> pt() );
    matchingRecoLCombinedIsoOverPtDistr -> Fill( combinedIsoVal / matchingMuonRef -> pt() );
  }
  
  matchingRecoLNDistr -> Fill(matchingRecoLN);
  
  
}
