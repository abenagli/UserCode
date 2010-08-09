#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFElectronDistributions.h"

#define PI 3.141592654






VBFElectronDistributions::VBFElectronDistributions(const edm::ParameterSet& iConfig):
 m_srcGenParticles               (iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 m_srcElectrons                  (iConfig.getParameter<edm::InputTag>("srcElectrons")),
 m_srcElectronTkIsoValues        (iConfig.getParameter<edm::InputTag>("srcElectronTkIsoValues")),
 m_srcElectronEmIsoValues        (iConfig.getParameter<edm::InputTag>("srcElectronEmIsoValues")),
 m_srcElectronHadIsoValues       (iConfig.getParameter<edm::InputTag>("srcElectronHadIsoValues")),
 m_srcElectronIdLooseValues      (iConfig.getParameter<edm::InputTag>("srcElectronIdLooseValues")),
 m_srcElectronIdTightValues      (iConfig.getParameter<edm::InputTag>("srcElectronIdTightValues")),
 m_srcElectronIdRobustLooseValues(iConfig.getParameter<edm::InputTag>("srcElectronIdRobustLooseValues")),
 m_srcElectronIdRobustTightValues(iConfig.getParameter<edm::InputTag>("srcElectronIdRobustTightValues")),
 m_tkIsoCoeff     (iConfig.getParameter<double>("tkIsoCoeff")), 
 m_emIsoCoeff     (iConfig.getParameter<double>("tkIsoCoeff")), 
 m_hadIsoCoeff    (iConfig.getParameter<double>("tkIsoCoeff")), 
 m_doRefCheck     (iConfig.getParameter<bool>("doRefCheck")),
 m_srcElectronsRef(iConfig.getParameter<edm::InputTag>("srcElectronsRef")),
 m_fileName (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFElectronDistributions"))),
 m_verbosity(iConfig.getUntrackedParameter<bool>("verbosity", true)),
 m_eventType(iConfig.getUntrackedParameter<int>("eventType", 0)),
 m_eventId  (0)
{}






VBFElectronDistributions::~VBFElectronDistributions()
{}







void VBFElectronDistributions::beginJob(const edm::EventSetup& iSetup)
{
  if(m_verbosity)
    std::cerr << "*** VBFElectronDistributions::beginJob ***" << std::endl;
  
  
  
  // electron histograms
  recoENDistr = new TH1F("recoENDistr", "recoENDistr", 25, 0., 25.);
  
  recoEPtDistr = new TH1F("recoEPtDistr", "recoEPtDistr", 1000, 0., 500.);
  recoEEtaDistr = new TH1F("recoEEtaDistr", "recoEEtaDistr", 1000, -5., 5.);
  
  recoETkIsoDistr       = new TH1F("recoETkIsoDistr",       "recoETkIsoDistr",       1000, 0., 100.);
  recoEEmIsoDistr       = new TH1F("recoEEmIsoDistr",       "recoEEmIsoDistr",       1000, 0., 100.);
  recoEHadIsoDistr      = new TH1F("recoEHadIsoDistr",      "recoEHadIsoDistr",      1000, 0., 100.);
  recoECombinedIsoDistr = new TH1F("recoECombinedIsoDistr", "recoECombinedIsoDistr", 3000, 0., 300.);
  recoETkIsoOverPtDistr       = new TH1F("recoETkIsoOverPtDistr",       "recoETkIsoOverPtDistr",       1000, 0., 10.);
  recoEEmIsoOverPtDistr       = new TH1F("recoEEmIsoOverPtDistr",       "recoEEmIsoOverPtDistr",       1000, 0., 10.);
  recoEHadIsoOverPtDistr      = new TH1F("recoEHadIsoOverPtDistr",      "recoEHadIsoOverPtDistr",      1000, 0., 10.);
  recoECombinedIsoOverPtDistr = new TH1F("recoECombinedIsoOverPtDistr", "recoECombinedIsoOverPtDistr", 3000, 0., 30.);
  
  recoEEleIdLooseDistr       = new TH1F("recoEEleIdLoose",       "recoEEleIdLooseDistr",       2, 0., 2.);
  recoEEleIdTightDistr       = new TH1F("recoEEleIdTight",       "recoEEleIdTightDistr",       2, 0., 2.);
  recoEEleIdRobustLooseDistr = new TH1F("recoEEleIdRobustLoose", "recoEEleIdRobustLooseDistr", 2, 0., 2.);
  recoEEleIdRobustTightDistr = new TH1F("recoEEleIdRobustTight", "recoEEleIdRobustTightDistr", 2, 0., 2.);
  
  
  
  matchingRecoENDistr = new TH1F("matchingRecoENDistr", "matchingRecoENDistr", 25, 0., 25.);
  
  matchingRecoEPtDistr = new TH1F("matchingRecoEPtDistr", "matchingRecoEPtDistr", 1000, 0., 500.);
  matchingRecoEEtaDistr = new TH1F("matchingRecoEEtaDistr", "matchingRecoEEtaDistr", 1000, -5., 5.);
  
  matchingRecoETkIsoDistr       = new TH1F("matchingRecoETkIsoDistr",       "matchingRecoETkIsoDistr",       1000, 0., 100.);
  matchingRecoEEmIsoDistr       = new TH1F("matchingRecoEEmIsoDistr",       "matchingRecoEEmIsoDistr",       1000, 0., 100.);
  matchingRecoEHadIsoDistr      = new TH1F("matchingRecoEHadIsoDistr",      "matchingRecoEHadIsoDistr",      1000, 0., 100.);
  matchingRecoECombinedIsoDistr = new TH1F("matchingRecoECombinedIsoDistr", "matchingRecoECombinedIsoDistr", 3000, 0., 300.);
  matchingRecoETkIsoOverPtDistr       = new TH1F("matchingRecoETkIsoOverPtDistr",       "matchingRecoETkIsoOverPtDistr",       1000, 0., 10.);
  matchingRecoEEmIsoOverPtDistr       = new TH1F("matchingRecoEEmIsoOverPtDistr",       "matchingRecoEEmIsoOverPtDistr",       1000, 0., 10.);
  matchingRecoEHadIsoOverPtDistr      = new TH1F("matchingRecoEHadIsoOverPtDistr",      "matchingRecoEHadIsoOverPtDistr",      1000, 0., 10.);
  matchingRecoECombinedIsoOverPtDistr = new TH1F("matchingRecoECombinedIsoOverPtDistr", "matchingRecoECombinedIsoOverPtDistr", 3000, 0., 30.);
  
  matchingRecoEEleIdLooseDistr       = new TH1F("matchingRecoEEleIdLoose",       "matchingRecoEEleIdLooseDistr",       2, 0., 2.);
  matchingRecoEEleIdTightDistr       = new TH1F("matchingRecoEEleIdTight",       "matchingRecoEEleIdTightDistr",       2, 0., 2.);
  matchingRecoEEleIdRobustLooseDistr = new TH1F("matchingRecoEEleIdRobustLoose", "matchingRecoEEleIdRobustLooseDistr", 2, 0., 2.);
  matchingRecoEEleIdRobustTightDistr = new TH1F("matchingRecoEEleIdRobustTight", "matchingRecoEEleIdRobustTightDistr", 2, 0., 2.);
}






void VBFElectronDistributions::endJob()
{
  if(m_verbosity)
    std::cerr << "*** VBFElectronDistributions::endJob ***" << std::endl;
  
  
  
  // Create File
  m_outFile = new TFile(m_fileName.c_str() , "RECREATE");
  m_outFile -> cd();
  
  
  
  m_outFile -> mkdir("recoElectronDistr");  
  m_outFile -> cd("recoElectronDistr");  
  
  WriteNormalized(recoENDistr);
  
  WriteNormalized(recoEPtDistr); 
  WriteNormalized(recoEEtaDistr); 
  
  WriteNormalized(recoETkIsoDistr); 
  WriteNormalized(recoEEmIsoDistr); 
  WriteNormalized(recoEHadIsoDistr); 
  WriteNormalized(recoECombinedIsoDistr); 
  WriteNormalized(recoETkIsoOverPtDistr); 
  WriteNormalized(recoEEmIsoOverPtDistr); 
  WriteNormalized(recoEHadIsoOverPtDistr); 
  WriteNormalized(recoECombinedIsoOverPtDistr); 
  
  WriteNormalized(recoEEleIdLooseDistr); 
  WriteNormalized(recoEEleIdTightDistr); 
  WriteNormalized(recoEEleIdRobustLooseDistr); 
  WriteNormalized(recoEEleIdRobustTightDistr); 
  
  m_outFile -> cd();    
  
  
  
  m_outFile -> mkdir("matchingRecoElectronDistr");  
  m_outFile -> cd("matchingRecoElectronDistr");  
  
  WriteNormalized(matchingRecoENDistr);
  
  WriteNormalized(matchingRecoEPtDistr); 
  WriteNormalized(matchingRecoEEtaDistr); 
  
  WriteNormalized(matchingRecoETkIsoDistr); 
  WriteNormalized(matchingRecoEEmIsoDistr); 
  WriteNormalized(matchingRecoEHadIsoDistr); 
  WriteNormalized(matchingRecoECombinedIsoDistr); 
  WriteNormalized(matchingRecoETkIsoOverPtDistr); 
  WriteNormalized(matchingRecoEEmIsoOverPtDistr); 
  WriteNormalized(matchingRecoEHadIsoOverPtDistr); 
  WriteNormalized(matchingRecoECombinedIsoOverPtDistr); 
  
  WriteNormalized(matchingRecoEEleIdLooseDistr); 
  WriteNormalized(matchingRecoEEleIdTightDistr); 
  WriteNormalized(matchingRecoEEleIdRobustLooseDistr); 
  WriteNormalized(matchingRecoEEleIdRobustTightDistr); 
  
  m_outFile -> cd();    
  
  
  
  m_outFile -> Close();
}






void VBFElectronDistributions::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // set event id
  ++m_eventId;
  
  if( (m_eventId%100 == 0) && (m_verbosity == true) )
    std::cout << ">>>>>> VBFElectronDistributions::Event number -----> " << std::fixed << std::setw(10) << m_eventId << std::endl;
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Get the collections
  // -----------------------------------------------------------------  

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(m_srcGenParticles, genParticles);
  if( !(genParticles.isValid ()) )
  {
    std::cerr << ">>>>>> VBFElectronDistributions::Warning: " << m_srcGenParticles << " not available" << std::endl;
    return;
  }
  
  
  
  edm::Handle<reco::GsfElectronCollection> electrons;
  iEvent.getByLabel(m_srcElectrons, electrons);
  if( !(electrons.isValid()) )
  {
    std::cerr << ">>>>>> VBFElectronDistributions::Warning: " << m_srcElectrons << " not available" << std::endl;
    return;
  }
  
  edm::Handle< edm::RefVector<reco::GsfElectronCollection> > electronsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcElectronsRef, electronsRef);
  
  
  
  edm::Handle<edm::ValueMap<double> > electronTkIsoValues;
  iEvent.getByLabel(m_srcElectronTkIsoValues, electronTkIsoValues);
  if( !(electronTkIsoValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFElectronDistributions::Warning: " << m_srcElectronTkIsoValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > electronEmIsoValues;
  iEvent.getByLabel(m_srcElectronEmIsoValues, electronEmIsoValues);
 if( !(electronEmIsoValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFElectronDistributions::Warning: " << m_srcElectronEmIsoValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<double> > electronHadIsoValues;
  iEvent.getByLabel(m_srcElectronHadIsoValues, electronHadIsoValues);
  if( !(electronHadIsoValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFElectronDistributions::Warning: " << m_srcElectronHadIsoValues << " not available" << std::endl;
    return;
  }
  
  
  
  edm::Handle<edm::ValueMap<float> > electronIdLooseValues;
  iEvent.getByLabel(m_srcElectronIdLooseValues, electronIdLooseValues);
  if( !(electronIdLooseValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFElectronDistributions::Warning: " << m_srcElectronIdLooseValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > electronIdTightValues;
  iEvent.getByLabel(m_srcElectronIdTightValues, electronIdTightValues);
  if( !(electronIdTightValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFElectronDistributions::Warning: " << m_srcElectronIdTightValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > electronIdRobustLooseValues;
  iEvent.getByLabel(m_srcElectronIdRobustLooseValues, electronIdRobustLooseValues);
  if( !(electronIdRobustLooseValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFElectronDistributions::Warning: " << m_srcElectronIdRobustLooseValues << " not available" << std::endl;
    return;
  }
  
  edm::Handle<edm::ValueMap<float> > electronIdRobustTightValues;
  iEvent.getByLabel(m_srcElectronIdRobustTightValues, electronIdRobustTightValues);
  if( !(electronIdRobustTightValues.isValid()) )
  {
    std::cerr << ">>>>>> VBFElectronDistributions::Warning: " << m_srcElectronIdRobustTightValues << " not available" << std::endl;
    return;
  }
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Fill the histograms
  // -----------------------------------------------------------------  
  
  
  
  int recoEN = 0;  
  std::vector<GsfElectronRef> recoElectrons;
  for(unsigned int ii = 0; ii < electrons -> size(); ++ii)
  {
    // do the reference check
    bool isRefCheckOk = true;
    GsfElectronRef electronRef(electrons, ii);
    if(m_doRefCheck)
      if(find(electronsRef -> begin(), electronsRef -> end(), electronRef) == electronsRef -> end())
        isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    ++recoEN;    
    recoElectrons.push_back(electronRef);
    
    
    
    // pt distribution
    recoEPtDistr  -> Fill( electronRef -> pt() );
    recoEEtaDistr -> Fill( electronRef -> eta() );
    
      
    // isolation distributions
    recoETkIsoDistr  -> Fill( (*electronTkIsoValues) [electronRef] );
    recoEEmIsoDistr  -> Fill( (*electronEmIsoValues) [electronRef] );
    recoEHadIsoDistr -> Fill( (*electronHadIsoValues)[electronRef] );
    recoECombinedIsoDistr -> Fill( ( m_tkIsoCoeff  * (*electronTkIsoValues) [electronRef] + 
                                     m_emIsoCoeff  * (*electronEmIsoValues) [electronRef] + 
                                     m_hadIsoCoeff * (*electronHadIsoValues)[electronRef] ) );
    recoETkIsoOverPtDistr  -> Fill( (*electronTkIsoValues) [electronRef] / electronRef -> pt() );
    recoEEmIsoOverPtDistr  -> Fill( (*electronEmIsoValues) [electronRef] / electronRef -> pt() );
    recoEHadIsoOverPtDistr -> Fill( (*electronHadIsoValues)[electronRef] / electronRef -> pt() );
    recoECombinedIsoOverPtDistr -> Fill( ( m_tkIsoCoeff  * (*electronTkIsoValues) [electronRef] + 
                                           m_emIsoCoeff  * (*electronEmIsoValues) [electronRef] + 
                                           m_hadIsoCoeff * (*electronHadIsoValues)[electronRef] ) / electronRef -> pt() );
    
    
    // electronId distributions
    recoEEleIdLooseDistr  -> Fill( (*electronIdLooseValues)[electronRef] );
    recoEEleIdTightDistr  -> Fill( (*electronIdTightValues)[electronRef] );
    recoEEleIdRobustLooseDistr  -> Fill( (*electronIdRobustLooseValues)[electronRef] );
    recoEEleIdRobustTightDistr  -> Fill( (*electronIdRobustTightValues)[electronRef] );
  }
  
  recoENDistr  -> Fill( recoEN );
    
  
  
  
  
  
  // MCDumperVBF
  if(m_eventType != 0) return;
  MCDumperVBF mcAnalysis(genParticles, 0, false);
  bool isValid = mcAnalysis.isValid();
  if(!isValid) return;
  
  
  
  // Get electrons from V decay (if any)
  std::vector<const reco::Candidate*> mcElectrons;
  
  const reco::Candidate* mcL1_fromV1 = mcAnalysis.mcF1_fromV1();
  if( abs(mcL1_fromV1 -> pdgId()) == 11 )
    mcElectrons.push_back(mcL1_fromV1);
  
  const reco::Candidate* mcL2_fromV1 = mcAnalysis.mcF2_fromV1();
  if( abs(mcL2_fromV1 -> pdgId()) == 11 )
    mcElectrons.push_back(mcL2_fromV1);
  
  const reco::Candidate* mcL1_fromV2 = mcAnalysis.mcF1_fromV2();
  if( abs(mcL1_fromV2 -> pdgId()) == 11 )
    mcElectrons.push_back(mcL1_fromV2);
  
  const reco::Candidate* mcL2_fromV2 = mcAnalysis.mcF2_fromV2();
  if( abs(mcL2_fromV2 -> pdgId()) == 11 )
    mcElectrons.push_back(mcL2_fromV2);
  
  

  // Matching between recoElectrons and electrons from V decay  
  std::vector<int> matchRecoEIt;
  GetMatching(recoElectrons, mcElectrons, 0.05, true, 0.1, 10., &matchRecoEIt);
  
  
  for(unsigned int ii = 0; ii < matchRecoEIt.size(); ++ii)
  {
    int jj = matchRecoEIt.at(ii);
    if(jj == -1) continue;
    GsfElectronRef matchingElectronRef = recoElectrons.at(jj);    
    
    
    

    // pt distribution
    matchingRecoEPtDistr  -> Fill( matchingElectronRef -> pt() );
    matchingRecoEEtaDistr -> Fill( matchingElectronRef -> eta() );
    
      
    // isolation distributions
    matchingRecoETkIsoDistr  -> Fill( (*electronTkIsoValues) [matchingElectronRef] );
    matchingRecoEEmIsoDistr  -> Fill( (*electronEmIsoValues) [matchingElectronRef] );
    matchingRecoEHadIsoDistr -> Fill( (*electronHadIsoValues)[matchingElectronRef] );
    matchingRecoECombinedIsoDistr -> Fill( ( m_tkIsoCoeff  * (*electronTkIsoValues) [matchingElectronRef] + 
                                     m_emIsoCoeff  * (*electronEmIsoValues) [matchingElectronRef] + 
                                     m_hadIsoCoeff * (*electronHadIsoValues)[matchingElectronRef] ) );
    matchingRecoETkIsoOverPtDistr  -> Fill( (*electronTkIsoValues) [matchingElectronRef] / matchingElectronRef -> pt() );
    matchingRecoEEmIsoOverPtDistr  -> Fill( (*electronEmIsoValues) [matchingElectronRef] / matchingElectronRef -> pt() );
    matchingRecoEHadIsoOverPtDistr -> Fill( (*electronHadIsoValues)[matchingElectronRef] / matchingElectronRef -> pt() );
    matchingRecoECombinedIsoOverPtDistr -> Fill( ( m_tkIsoCoeff  * (*electronTkIsoValues) [matchingElectronRef] + 
                                           m_emIsoCoeff  * (*electronEmIsoValues) [matchingElectronRef] + 
                                           m_hadIsoCoeff * (*electronHadIsoValues)[matchingElectronRef] ) / matchingElectronRef -> pt() );
    
    
    // electronId distributions
    matchingRecoEEleIdLooseDistr  -> Fill( (*electronIdLooseValues)[matchingElectronRef] );
    matchingRecoEEleIdTightDistr  -> Fill( (*electronIdTightValues)[matchingElectronRef] );
    matchingRecoEEleIdRobustLooseDistr  -> Fill( (*electronIdRobustLooseValues)[matchingElectronRef] );
    matchingRecoEEleIdRobustTightDistr  -> Fill( (*electronIdRobustTightValues)[matchingElectronRef] );
  }
  
  matchingRecoENDistr -> Fill(matchRecoEIt.size());
  
  
}
