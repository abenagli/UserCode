#include "HiggsAnalysis/VBFHiggsToVV/plugins/SimpleNtple.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Common/interface/ValueMap.h"






SimpleNtple::SimpleNtple(const edm::ParameterSet& iConfig)
{
  //---- Out file ----
  mcAnalysis_ = NULL; 
  
  edm::Service<TFileService> fs;
  outTree_  = fs -> make <TTree>("SimpleNtple", "SimpleNtple"); 
  NtupleFactory_ = new NtupleFactory(outTree_);  
  
  
  
  //---- Input tags ----
  HLTTag_ = iConfig.getParameter<edm::InputTag>("HLTTag");
  PVTag_ = iConfig.getParameter<edm::InputTag>("PVTag");
  TracksTag_ = iConfig.getParameter<edm::InputTag>("TracksTag");
  
  EleTag_ = iConfig.getParameter<edm::InputTag>("EleTag");
  Ele3DipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("Ele3DipSignificanceTag");
  EleTipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("EleTipSignificanceTag");
  EleLipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("EleLipSignificanceTag");
  
  MuTag_ = iConfig.getParameter<edm::InputTag>("MuTag");
  Mu3DipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("Mu3DipSignificanceTag");
  MuTipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("MuTipSignificanceTag");
  MuLipSignificanceTag_ = iConfig.getParameter<edm::InputTag>("MuLipSignificanceTag");
 
  MetTag_ = iConfig.getParameter<edm::InputTag>("MetTag");
  Type1MetTag_ = iConfig.getParameter<edm::InputTag>("Type1MetTag");
  PFMetTag_ = iConfig.getParameter<edm::InputTag>("PFMetTag");
  TcMetTag_ = iConfig.getParameter<edm::InputTag>("TcMetTag");
  
  JetTag_ = iConfig.getParameter<edm::InputTag>("JetTag");
  JetTag_forID_ = iConfig.getParameter<edm::InputTag>("JetTag_forID");
  jetIDTag_ = iConfig.getParameter<edm::InputTag> ("JetIDTag");
  
  MCtruthTag_ = iConfig.getParameter<edm::InputTag>("MCtruthTag");
  genJetTag_ = iConfig.getParameter<edm::InputTag>("genJetTag");
  genMetTag_ = iConfig.getParameter<edm::InputTag>("genMetTag");
  
  
  
  //---- ref check ----
  doEleRefCheck_ = iConfig.getUntrackedParameter<bool>("doEleRefCheck", false);
  if(doEleRefCheck_) EleRefTag_  = iConfig.getParameter<edm::InputTag>("EleRefTag");
  
  doMuRefCheck_ = iConfig.getUntrackedParameter<bool>("doMuRefCheck", false);
  if(doMuRefCheck_) MuRefTag_ = iConfig.getParameter<edm::InputTag>("MuRefTag");
  
  doJetRefCheck_ = iConfig.getUntrackedParameter<bool>("doJetRefCheck", false);
  if(doJetRefCheck_) JetRefTag_ = iConfig.getParameter<edm::InputTag>("JetRefTag");
  
  
  
  //---- flags ----
  saveHLT_ =iConfig.getUntrackedParameter<bool> ("saveHLT", true);
  savePV_ =iConfig.getUntrackedParameter<bool> ("savePV", true);
  saveTrack_ = iConfig.getUntrackedParameter<bool> ("saveTrack", true);
  
  saveEle_ = iConfig.getUntrackedParameter<bool> ("saveEle", true);
  
  saveMu_ =iConfig.getUntrackedParameter<bool> ("saveMu", true);

  saveMet_ = iConfig.getUntrackedParameter<bool> ("saveMet", true);
  
  saveJet_ = iConfig.getUntrackedParameter<bool> ("saveJet", true);
  savePFJet_ = iConfig.getUntrackedParameter<bool> ("savePFJet", false);
  if(saveJet_ && savePFJet_) savePFJet_ = false;
  saveJetBTagging_ = iConfig.getUntrackedParameter<bool> ("saveJetBTagging", true);
  
  saveGenJet_ = iConfig.getUntrackedParameter<bool> ("saveGenJet", true);
  saveGenMet_ = iConfig.getUntrackedParameter<bool> ("saveGenMet", true);
  saveMCPtHat_ = iConfig.getUntrackedParameter<bool> ("saveMCPtHat", true);
  saveMCHiggs_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggs", true);
  saveMCHiggsDecay_ = iConfig.getUntrackedParameter<bool> ("saveMCHiggsDecay", true);
  saveMCEle_ = iConfig.getUntrackedParameter<bool> ("saveMCEle", true);
  saveMCMu_ = iConfig.getUntrackedParameter<bool> ("saveMCMu", true);
  
  verbosity_ = iConfig.getUntrackedParameter<bool>("verbosity",false);
  eventType_ = iConfig.getUntrackedParameter<int>("eventType",1);
  
  
  
  
  
  
  //---- Add branches to ntuple ----
  NtupleFactory_ -> AddInt("runId"); 
  NtupleFactory_ -> AddInt("lumiId"); 
  NtupleFactory_ -> AddInt("BXId"); 
  NtupleFactory_ -> AddInt("eventId"); 
  NtupleFactory_ -> AddInt("eventNaiveId"); 
  eventNaiveId_ = 0;
  
  
  
  if(saveHLT_)
  {
    NtupleFactory_ -> AddFloat("HLT_WasRun"); 
    NtupleFactory_ -> AddFloat("HLT_Accept"); 
    NtupleFactory_ -> AddFloat("HLT_Error"); 
    NtupleFactory_ -> AddString("HLT_Names"); 
  }
  
  if(savePV_)
  {
    NtupleFactory_ -> AddFloat("PV_normalizedChi2"); 
    NtupleFactory_ -> AddInt("PV_nTracks"); 
  }
  
  if(saveTrack_)
  {
    NtupleFactory_ -> Add3V("tracks_in");
    NtupleFactory_ -> Add3V("tracks_out");   
  }
  
  
  
  if(saveEle_)
  {
    NtupleFactory_ -> Add4V   ("electrons");
    NtupleFactory_ -> AddFloat("electrons_charge"); 
    NtupleFactory_ -> AddFloat("electrons_dxy");
    NtupleFactory_ -> AddFloat("electrons_dz");
    NtupleFactory_ -> AddFloat("electrons_3DipSignificance");    
    NtupleFactory_ -> AddFloat("electrons_tipSignificance");    
    NtupleFactory_ -> AddFloat("electrons_lipSignificance");
    
    NtupleFactory_ -> AddFloat("electrons_tkIsoR03"); 
    NtupleFactory_ -> AddFloat("electrons_tkIsoR04"); 
    NtupleFactory_ -> AddFloat("electrons_emIsoR03"); 
    NtupleFactory_ -> AddFloat("electrons_emIsoR04"); 
    NtupleFactory_ -> AddFloat("electrons_hadIsoR03_depth1"); 
    NtupleFactory_ -> AddFloat("electrons_hadIsoR03_depth2"); 
    NtupleFactory_ -> AddFloat("electrons_hadIsoR04_depth1"); 
    NtupleFactory_ -> AddFloat("electrons_hadIsoR04_depth2"); 
    
    NtupleFactory_ -> AddInt  ("electrons_isEB");
    NtupleFactory_ -> AddInt  ("electrons_ecalDrivenSeed");
    NtupleFactory_ -> AddInt  ("electrons_trackerDrivenSeed");
    NtupleFactory_ -> AddFloat("electrons_scE");
    NtupleFactory_ -> AddFloat("electrons_eSeed");
    NtupleFactory_ -> AddFloat("electrons_pin");
    NtupleFactory_ -> AddFloat("electrons_pout");
    NtupleFactory_ -> AddFloat("electrons_eOverP");
    NtupleFactory_ -> AddInt  ("electrons_classification");
    NtupleFactory_ -> AddFloat("electrons_fbrem");
    NtupleFactory_ -> AddFloat("electrons_hOverE");
    NtupleFactory_ -> AddFloat("electrons_deltaPhiIn");
    NtupleFactory_ -> AddFloat("electrons_deltaEtaIn");
    NtupleFactory_ -> AddFloat("electrons_sigmaIetaIeta");
    
    NtupleFactory_ -> AddString("electrons_seed_flag");
    NtupleFactory_ -> AddString("electrons_seed_severityLevel");
    NtupleFactory_ -> AddString("electrons_other_flag");
    NtupleFactory_ -> AddString("electrons_other_severityLevel");
  }
  
  
  
  if(saveMu_)
  {
    NtupleFactory_ -> Add4V("muons");
    NtupleFactory_ -> AddFloat("muons_charge"); 
    NtupleFactory_ -> AddFloat("muons_dxy");
    NtupleFactory_ -> AddFloat("muons_dz");
    NtupleFactory_ -> AddFloat("muons_3DipSignificance");    
    NtupleFactory_ -> AddFloat("muons_tipSignificance");    
    NtupleFactory_ -> AddFloat("muons_lipSignificance");    
    
    NtupleFactory_ -> AddFloat("muons_nTkIsoR03"); 
    NtupleFactory_ -> AddFloat("muons_nTkIsoR05"); 
    NtupleFactory_ -> AddFloat("muons_tkIsoR03"); 
    NtupleFactory_ -> AddFloat("muons_tkIsoR05"); 
    NtupleFactory_ -> AddFloat("muons_emIsoR03"); 
    NtupleFactory_ -> AddFloat("muons_emIsoR05"); 
    NtupleFactory_ -> AddFloat("muons_hadIsoR03"); 
    NtupleFactory_ -> AddFloat("muons_hadIsoR05"); 
    
    NtupleFactory_ -> AddInt  ("muons_tracker");
    NtupleFactory_ -> AddInt  ("muons_standalone");
    NtupleFactory_ -> AddInt  ("muons_global");
    NtupleFactory_ -> AddFloat("muons_normalizedChi2");
    NtupleFactory_ -> AddInt  ("muons_numberOfValidTrackerHits");
    NtupleFactory_ -> AddInt  ("muons_numberOfValidMuonHits");
  }
  
  
  
  if(saveMet_)
  {
    NtupleFactory_ -> Add4V("met");         
    NtupleFactory_ -> Add4V("type1Met");         
    NtupleFactory_ -> Add4V("PFMet");         
    NtupleFactory_ -> Add4V("TcMet");         
  }
  
  
  
  if(saveJet_ || savePFJet_)
  {
    NtupleFactory_->Add4V("jets");
    
    NtupleFactory_->AddFloat("jets_trackCountingHighEffBJetTags");   
    NtupleFactory_->AddFloat("jets_trackCountingHighPurBJetTags");   
    //NtupleFactory_->AddFloat("jets_simpleSecondaryVertexHighEffBJetTags");   
    //NtupleFactory_->AddFloat("jets_simpleSecondaryVertexHighPurBJetTags");   
    //NtupleFactory_->AddFloat("jets_combinedSecondaryVertexBJetTags");   
    //NtupleFactory_->AddFloat("jets_combinedSecondaryVertexMVABJetTags");   
    //NtupleFactory_->AddFloat("jets_jetProbabilityBJetTags");   
    //NtupleFactory_->AddFloat("jets_jetBProbabilityBJetTags");   
    NtupleFactory_->AddFloat("jets_emEnergyFraction");   

    if(saveJet_)
    {
      NtupleFactory_->AddFloat("jets_etaetaMoment");   
      NtupleFactory_->AddFloat("jets_phiphiMoment");   
      NtupleFactory_->AddFloat("jets_etaphiMoment");   
      NtupleFactory_->AddFloat("jets_jetArea");   
      NtupleFactory_->AddFloat("jets_fHPD");   
      NtupleFactory_->AddFloat("jets_fRBX");   
      NtupleFactory_->AddFloat("jets_n90Hits");   
      NtupleFactory_->AddFloat("jets_nHCALTowers");   
      NtupleFactory_->AddFloat("jets_nECALTowers");   
    }
    
    if(savePFJet_)
    {
      NtupleFactory_->AddFloat("jets_chargedHadronEnergy"); 
      NtupleFactory_->AddFloat("jets_chargedHadronEnergyFraction");
      NtupleFactory_->AddFloat("jets_neutralHadronEnergy"); 
      NtupleFactory_->AddFloat("jets_neutralHadronEnergyFraction"); 
      NtupleFactory_->AddFloat("jets_chargedEmEnergy"); 
      NtupleFactory_->AddFloat("jets_chargedEmEnergyFraction"); 
      NtupleFactory_->AddFloat("jets_chargedMuEnergy"); 
      NtupleFactory_->AddFloat("jets_chargedMuEnergyFraction"); 
      NtupleFactory_->AddFloat("jets_neutralEmEnergy"); 
      NtupleFactory_->AddFloat("jets_neutralEmEnergyFraction"); 
      NtupleFactory_->AddInt("jets_chargedMultiplicity"); 
      NtupleFactory_->AddInt("jets_neutralMultiplicity"); 
      NtupleFactory_->AddInt("jets_muonMultiplicity"); 
    }
  }
  
  
  
  if(saveGenJet_)
  {
    NtupleFactory_->Add4V("genJets");         
  }    
  
  if(saveGenMet_)
  {
   NtupleFactory_->Add4V("genMet");                
  }
  
  if(saveMCPtHat_)
  {
    NtupleFactory_->AddFloat("mc_ptHat");    
  }

  if(saveMCHiggs_)
  {
    NtupleFactory_->Add4V("mc_H");    
    NtupleFactory_->AddFloat("mc_H_charge");    
  }

  if(saveMCHiggsDecay_)
  {
    NtupleFactory_->Add4V("mcV1");         
    NtupleFactory_->AddFloat("mcV1_charge");    
    NtupleFactory_->AddFloat("mcV1_pdgId");    
    
    NtupleFactory_->Add4V("mcV2");         
    NtupleFactory_->AddFloat("mcV2_charge");    
    NtupleFactory_->AddFloat("mcV2_pdgId");  
    
    NtupleFactory_->Add4V("mcF1_fromV1");   
    NtupleFactory_->AddFloat("mcF1_fromV1_charge");    
    NtupleFactory_->AddFloat("mcF1_fromV1_pdgId");  
       
    NtupleFactory_->Add4V("mcF2_fromV1");         
    NtupleFactory_->AddFloat("mcF2_fromV1_charge");    
    NtupleFactory_->AddFloat("mcF2_fromV1_pdgId");  
 
    NtupleFactory_->Add4V("mcF1_fromV2");         
    NtupleFactory_->AddFloat("mcF1_fromV2_charge");    
    NtupleFactory_->AddFloat("mcF1_fromV2_pdgId");  
 
    NtupleFactory_->Add4V("mcF2_fromV2");         
    NtupleFactory_->AddFloat("mcF2_fromV2_charge");    
    NtupleFactory_->AddFloat("mcF2_fromV2_pdgId");  
 
    NtupleFactory_->Add4V("mcQ1_tag");    
    NtupleFactory_->AddFloat("mcQ1_tag_charge");    
    NtupleFactory_->AddFloat("mcQ1_tag_pdgId");  
      
    NtupleFactory_->Add4V("mcQ2_tag");         
    NtupleFactory_->AddFloat("mcQ2_tag_charge");    
    NtupleFactory_->AddFloat("mcQ2_tag_pdgId");  
  }
  
  if(saveMCEle_)
  {
    NtupleFactory_->Add4V("mc_ele");    
    NtupleFactory_->AddFloat("mc_ele_charge");    
  }

  if(saveMCMu_)
  {
    NtupleFactory_->Add4V("mc_mu");    
    NtupleFactory_->AddFloat("mc_mu_charge");    
  }
  
  
  
}

// --------------------------------------------------------------------






SimpleNtple::~SimpleNtple()
{
  NtupleFactory_ -> WriteNtuple();
  delete NtupleFactory_;
}

// --------------------------------------------------------------------












// ------------ method called to for each event  ------------
void SimpleNtple::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ++eventNaiveId_;
  
  NtupleFactory_ -> FillInt("runId", iEvent.id().run());
  NtupleFactory_ -> FillInt("lumiId", iEvent.luminosityBlock());
  NtupleFactory_ -> FillInt("BXId", iEvent.bunchCrossing());
  NtupleFactory_ -> FillInt("eventId", iEvent.id().event());
  NtupleFactory_ -> FillInt("eventNaiveId", eventNaiveId_);
  
  
  
  edm::Handle<reco::GenParticleCollection> genParticles;
  if(saveMCHiggs_ || saveMCHiggsDecay_ || saveMCEle_ || saveMCMu_)
  {
    iEvent.getByLabel(MCtruthTag_, genParticles);
    mcAnalysis_ = new MCDumperVBF(genParticles, eventType_, verbosity_);
  }
  
  
  
  if(saveHLT_) fillHLTInfo (iEvent, iSetup);
  if(savePV_) fillPVInfo (iEvent, iSetup);
  if(saveTrack_) fillTrackInfo (iEvent, iSetup);
  
  if(saveEle_) fillEleInfo (iEvent, iSetup);
  if(saveMu_) fillMuInfo (iEvent, iSetup);
  
  if(saveMet_) fillMetInfo (iEvent, iSetup);
  
  if(saveJet_) fillJetInfo (iEvent, iSetup);
  if(savePFJet_) fillPFJetInfo (iEvent, iSetup);
  
  if(saveGenJet_) fillGenJetInfo (iEvent, iSetup);
  if(saveGenMet_) fillGenMetInfo (iEvent, iSetup);
  if(saveMCPtHat_) fillMCPtHatInfo (iEvent, iSetup);
  if(saveMCHiggs_) fillMCHiggsInfo (iEvent, iSetup);
  if(saveMCHiggsDecay_) fillMCHiggsDecayInfo (iEvent, iSetup);
  if(saveMCEle_) fillMCEleInfo (iEvent, iSetup);
  if(saveMCMu_) fillMCMuInfo (iEvent, iSetup);
  
  NtupleFactory_->FillNtuple();
}











 
///---- HLT ----
void SimpleNtple::fillHLTInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillHLTInfo::begin" << std::endl;
  
  edm::Handle<edm::TriggerResults> triggerResultsHandle;
  iEvent.getByLabel(HLTTag_, triggerResultsHandle);
  
  const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResultsHandle);
  
  
  for(unsigned int iHLT = 0; iHLT < triggerResultsHandle->size(); ++iHLT)
  {
    //std::cout << "bit: " << std::fixed << setw(3)<< iHLT << "   name: " << triggerNames.triggerName(iHLT) << std::endl;
    
    if( triggerResultsHandle -> wasrun(iHLT) )
      NtupleFactory_ -> FillFloat("HLT_WasRun", 1);
    else
      NtupleFactory_ -> FillFloat("HLT_WasRun", 0);
    
    
    if( triggerResultsHandle -> accept(iHLT) )
      NtupleFactory_ -> FillFloat("HLT_Accept", 1);
    else
      NtupleFactory_ -> FillFloat("HLT_Accept", 0);
    
    
    if( triggerResultsHandle -> error(iHLT) )
      NtupleFactory_->FillFloat("HLT_Error", 1);
    else
      NtupleFactory_->FillFloat("HLT_Error", 0);
    
    
    NtupleFactory_ -> FillString("HLT_Names", triggerNames.triggerName(iHLT));
  }
  
  //std::cout << "SimpleNtple::fillHLTInfo::end" << std::endl;
}






///---- primary vertex ----
void SimpleNtple::fillPVInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillPVInfo::begin" << std::endl;
  
  edm::Handle<reco::VertexCollection> vertexes;
  iEvent.getByLabel(PVTag_, vertexes);
  
  
  // select the primary vertex                                                                                                                                                   
  reco::Vertex PV;
  bool PVfound = (vertexes -> size() != 0);
  
  if(PVfound)
  {
    // select the primary vertex as the one with higest sum of (pt)^2 of tracks                                                                                                  
    PrimaryVertexSorter PVSorter;
    std::vector<reco::Vertex> sortedVertices = PVSorter.sortedList( *(vertexes.product()) );
    PV = sortedVertices.front();
    
    NtupleFactory_->FillFloat("PV_normalizedChi2", PV.normalizedChi2());
    NtupleFactory_->FillInt("PV_nTracks", PV.tracksSize());
  }
  else
  {
    //creating a dummy PV                                                                                                                                                        
    reco::Vertex::Point p(0,0,0);
    reco::Vertex::Error e;
    e(0,0) = 0.0015*0.0015;
    e(1,1) = 0.0015*0.0015;
    e(2,2) = 15.*15.;
    PV = reco::Vertex(p, e, 1, 1, 1);
  }
  
  math::XYZPoint PVPoint(PV.position().x(), PV.position().y(), PV.position().z());
  PVPoint_ = PVPoint;
  
  //std::cout << "SimpleNtple::fillPVInfo::end" << std::endl;
}






///---- tracks ----
void SimpleNtple::fillTrackInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillTrackInfo::begin" << std::endl;
  
  edm::Handle<edm::View<reco::Track> > TracksHandle ;
  iEvent.getByLabel (TracksTag_, TracksHandle) ;
  for (edm::View<reco::Track>::const_iterator tkIt = TracksHandle->begin (); tkIt != TracksHandle->end (); ++tkIt ) 
  { 
    NtupleFactory_ -> Fill3V("tracks_in",(*tkIt).innerMomentum ());
    NtupleFactory_ -> Fill3V("tracks_out",(*tkIt).outerMomentum ());
  }
  
  //std::cout << "SimpleNtple::fillTrackInfo::end" << std::endl;
}












///---- electrons ----
void SimpleNtple::fillEleInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup)
{
  //std::cout << "SimpleNtple::fillEleInfo::begin" << std::endl;
  
  edm::Handle<reco::GsfElectronCollection> EleHandle ;
  iEvent.getByLabel (EleTag_,EleHandle);
  
  edm::Handle<eleMap> Ele3DipSignificanceHandle ;
  iEvent.getByLabel (Ele3DipSignificanceTag_,Ele3DipSignificanceHandle);
  edm::Handle<eleMap> EleTipSignificanceHandle ;
  iEvent.getByLabel (EleTipSignificanceTag_,EleTipSignificanceHandle);
  edm::Handle<eleMap> EleLipSignificanceHandle ;
  iEvent.getByLabel (EleLipSignificanceTag_,EleLipSignificanceHandle);
  
  edm::Handle<edm::RefVector<reco::GsfElectronCollection> > EleRefHandle;
  if(doEleRefCheck_)
    iEvent.getByLabel(EleRefTag_, EleRefHandle);
  
  
  
  // loop on electrons
  for(unsigned int i=0; i<EleHandle->size(); i++)
  {
    reco::GsfElectronRef eleRef(EleHandle, i);  
    
    // do the reference check 
    bool isEleRefCheckOk = true;
    if(doEleRefCheck_)
      if(find(EleRefHandle -> begin(), EleRefHandle -> end(), eleRef) == EleRefHandle -> end())
        isEleRefCheckOk = false;
    
    if(!isEleRefCheckOk) continue;
    
    
    
    NtupleFactory_ -> Fill4V   ("electrons",eleRef->p4());
    NtupleFactory_ -> FillFloat("electrons_charge",eleRef->charge());
    NtupleFactory_ -> FillFloat("electrons_dxy",(eleRef->gsfTrack())->dxy(PVPoint_));
    NtupleFactory_ -> FillFloat("electrons_dz",(eleRef->gsfTrack())->dz(PVPoint_));
    NtupleFactory_ -> FillFloat("electrons_3DipSignificance",((*Ele3DipSignificanceHandle)[eleRef]));
    NtupleFactory_ -> FillFloat("electrons_tipSignificance",((*EleTipSignificanceHandle)[eleRef]));
    NtupleFactory_ -> FillFloat("electrons_lipSignificance",((*EleLipSignificanceHandle)[eleRef]));
    
    NtupleFactory_ -> FillFloat("electrons_tkIsoR03",eleRef->dr03TkSumPt());
    NtupleFactory_ -> FillFloat("electrons_tkIsoR04",eleRef->dr04TkSumPt());
    NtupleFactory_ -> FillFloat("electrons_emIsoR03",eleRef->dr03EcalRecHitSumEt());
    NtupleFactory_ -> FillFloat("electrons_emIsoR04",eleRef->dr04EcalRecHitSumEt());
    NtupleFactory_ -> FillFloat("electrons_hadIsoR03_depth1",eleRef->dr03HcalDepth1TowerSumEt());
    NtupleFactory_ -> FillFloat("electrons_hadIsoR03_depth2",eleRef->dr03HcalDepth2TowerSumEt());
    NtupleFactory_ -> FillFloat("electrons_hadIsoR04_depth1",eleRef->dr04HcalDepth1TowerSumEt());
    NtupleFactory_ -> FillFloat("electrons_hadIsoR04_depth2",eleRef->dr04HcalDepth2TowerSumEt());
      
    if(eleRef->isEB()) NtupleFactory_ -> FillInt("electrons_isEB", 1);
    else               NtupleFactory_ -> FillInt("electrons_isEB", 0);
    if(eleRef->ecalDrivenSeed()) NtupleFactory_ -> FillInt("electrons_ecalDrivenSeed", 1);
    else                         NtupleFactory_ -> FillInt("electrons_ecalDrivenSeed", 0);
    if(eleRef->trackerDrivenSeed()) NtupleFactory_ -> FillInt("electrons_trackerDrivenSeed", 1);
    else                            NtupleFactory_ -> FillInt("electrons_trackerDrivenSeed", 0);
    NtupleFactory_ -> FillFloat("electrons_scE",(eleRef->superCluster())->energy());
    NtupleFactory_ -> FillFloat("electrons_eSeed",eleRef->superCluster()->seed()->energy());
    NtupleFactory_ -> FillFloat("electrons_pin",eleRef->trackMomentumAtVtx().R());
    NtupleFactory_ -> FillFloat("electrons_pout",eleRef->trackMomentumOut().R());
    NtupleFactory_ -> FillFloat("electrons_eOverP",eleRef->eSuperClusterOverP());
    NtupleFactory_ -> FillInt  ("electrons_classification",eleRef->classification());
    NtupleFactory_ -> FillFloat("electrons_fbrem",eleRef->fbrem());
    NtupleFactory_ -> FillFloat("electrons_hOverE",eleRef->hadronicOverEm());
    NtupleFactory_ -> FillFloat("electrons_deltaPhiIn",eleRef->deltaPhiSuperClusterTrackAtVtx());
    NtupleFactory_ -> FillFloat("electrons_deltaEtaIn",eleRef->deltaEtaSuperClusterTrackAtVtx());
    NtupleFactory_ -> FillFloat("electrons_sigmaIetaIeta",eleRef->sigmaIetaIeta());
    
    
    
    VBFGetElectronRecHitFlags theRecHitFlags(iEvent, iESetup, eleRef);
    NtupleFactory_ -> FillString("electrons_seed_flag", theRecHitFlags.GetSeedFlag());
    NtupleFactory_ -> FillString("electrons_seed_severityLevel", theRecHitFlags.GetSeedSeverityLevel());
    NtupleFactory_ -> FillString("electrons_other_flag", theRecHitFlags.GetOtherFlag());
    NtupleFactory_ -> FillString("electrons_other_severityLevel", theRecHitFlags.GetOtherSeverityLevel());
    
  } // loop on electrons
  
   //std::cout << "SimpleNtple::fillEleInfo::end" << std::endl;
}






///---- muons ----
void SimpleNtple::fillMuInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillMuInfo::begin" << std::endl;
  
  edm::Handle<reco::MuonCollection> MuHandle ;
  iEvent.getByLabel (MuTag_,MuHandle);
  
  edm::Handle<muMap> Mu3DipSignificanceHandle ;
  iEvent.getByLabel (Mu3DipSignificanceTag_,Mu3DipSignificanceHandle);
  edm::Handle<muMap> MuTipSignificanceHandle ;
  iEvent.getByLabel (MuTipSignificanceTag_,MuTipSignificanceHandle);
  edm::Handle<muMap> MuLipSignificanceHandle ;
  iEvent.getByLabel (MuLipSignificanceTag_,MuLipSignificanceHandle);
  
  edm::Handle<edm::RefVector<reco::MuonCollection> > MuRefHandle;
  if(doMuRefCheck_)
    iEvent.getByLabel(MuRefTag_, MuRefHandle);
  
  
  
  // loop on muons 
  for(unsigned int i=0; i<MuHandle->size(); i++)
  {
    reco::MuonRef muRef(MuHandle, i);
    
    // do the reference check
    bool isMuRefCheckOk = true;
    if(doMuRefCheck_)
      if(find(MuRefHandle -> begin(), MuRefHandle -> end(), muRef) == MuRefHandle -> end())
        isMuRefCheckOk = false;
    
    if(!isMuRefCheckOk) continue;
    
    
    
    NtupleFactory_ -> Fill4V   ("muons",muRef->p4());
    NtupleFactory_ -> FillFloat("muons_charge",muRef->charge());
    NtupleFactory_ -> FillFloat("muons_dxy",(muRef->globalTrack())->dxy(PVPoint_));
    NtupleFactory_ -> FillFloat("muons_dz",(muRef->globalTrack())->dz(PVPoint_));
    NtupleFactory_ -> FillFloat("muons_3DipSignificance",((*Mu3DipSignificanceHandle)[muRef]));
    NtupleFactory_ -> FillFloat("muons_tipSignificance",((*MuTipSignificanceHandle)[muRef]));
    NtupleFactory_ -> FillFloat("muons_lipSignificance",((*MuLipSignificanceHandle)[muRef]));
    
    NtupleFactory_ -> FillFloat("muons_nTkIsoR03",(muRef->isolationR03()).nTracks);    
    NtupleFactory_ -> FillFloat("muons_nTkIsoR05",(muRef->isolationR05()).nTracks);    
    NtupleFactory_ -> FillFloat("muons_tkIsoR03",(muRef->isolationR03()).sumPt);
    NtupleFactory_ -> FillFloat("muons_tkIsoR05",(muRef->isolationR05()).sumPt);
    NtupleFactory_ -> FillFloat("muons_emIsoR03",(muRef->isolationR03()).emEt);
    NtupleFactory_ -> FillFloat("muons_emIsoR05",(muRef->isolationR05()).emEt);
    NtupleFactory_ -> FillFloat("muons_hadIsoR03",(muRef->isolationR03()).hadEt);
    NtupleFactory_ -> FillFloat("muons_hadIsoR05",(muRef->isolationR05()).hadEt);
    
    NtupleFactory_ -> FillInt  ("muons_tracker",muRef->isTrackerMuon());
    NtupleFactory_ -> FillInt  ("muons_standalone",muRef->isStandAloneMuon());
    NtupleFactory_ -> FillInt  ("muons_global",muRef->isGlobalMuon());
    NtupleFactory_ -> FillFloat("muons_normalizedChi2",(muRef->globalTrack())->normalizedChi2());
    NtupleFactory_ -> FillInt  ("muons_numberOfValidTrackerHits",(muRef->globalTrack()->hitPattern()).numberOfValidTrackerHits());
    NtupleFactory_ -> FillInt  ("muons_numberOfValidMuonHits",(muRef->globalTrack()->hitPattern()).numberOfValidMuonHits());
    
  } // loop on muons
  
  //std::cout << "SimpleNtple::fillMuInfo::end" << std::endl;
}












///---- MET ----
void SimpleNtple::fillMetInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillMetInfo::begin" << std::endl;
  
  edm::Handle<reco::CaloMETCollection> MetHandle ;
  iEvent.getByLabel (MetTag_,MetHandle);
  
  edm::Handle<reco::CaloMETCollection> Type1MetHandle ;
  iEvent.getByLabel (Type1MetTag_,Type1MetHandle);
  
  edm::Handle<reco::PFMETCollection> PFMetHandle ;
  iEvent.getByLabel (PFMetTag_,PFMetHandle);
  
  edm::Handle<reco::METCollection> TcMetHandle ;
  iEvent.getByLabel (TcMetTag_,TcMetHandle);
  
  
  
  const reco::CaloMET* met = &(MetHandle->front());
  NtupleFactory_ -> Fill4V("met",met->p4());
  
  const reco::MET* type1Met = &(Type1MetHandle->front());
  NtupleFactory_ -> Fill4V("type1Met",type1Met->p4());
  
  const reco::MET* TcMet = &(TcMetHandle->front());
  NtupleFactory_ -> Fill4V("TcMet",TcMet->p4());
  
  const reco::PFMET* PFMet = &(PFMetHandle->front());
  NtupleFactory_ -> Fill4V("PFMet",PFMet->p4());
  
  //std::cout << "SimpleNtple::fillMetInfo::end" << std::endl;
}












///---- Jets ----
void SimpleNtple::fillJetInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillJetInfo::begin" << std::endl;
  
  edm::Handle<reco::CaloJetCollection> JetHandle ;
  iEvent.getByLabel (JetTag_,JetHandle);
  
  edm::Handle<reco::CaloJetCollection> JetHandle_forID ;
  iEvent.getByLabel (JetTag_forID_,JetHandle_forID);
  
  edm::Handle<edm::RefVector<reco::CaloJetCollection> > JetRefHandle;
  if(doJetRefCheck_)
   iEvent.getByLabel(JetRefTag_, JetRefHandle);
  
  edm::Handle<reco::JetIDValueMap> jetIDHandle;
  iEvent.getByLabel (jetIDTag_, jetIDHandle) ;
  
  
  
  // loop on jets
  for(unsigned int i=0; i<JetHandle->size(); ++i) 
  { 
    reco::CaloJetRef jetRef(JetHandle, i);
    
    // do the reference check
    bool isJetRefCheckOk = true;
    if(doJetRefCheck_)
      if(find(JetRefHandle -> begin(), JetRefHandle -> end(), jetRef) == JetRefHandle -> end())
        isJetRefCheckOk = false;
    
    if(!isJetRefCheckOk) continue;
    
    
    
    NtupleFactory_ -> Fill4V   ("jets",jetRef->p4());
    NtupleFactory_ -> FillFloat("jets_emEnergyFraction",jetRef->emEnergyFraction());
    NtupleFactory_ -> FillFloat("jets_etaetaMoment",jetRef->etaetaMoment());
    NtupleFactory_ -> FillFloat("jets_phiphiMoment",jetRef->phiphiMoment());
    NtupleFactory_ -> FillFloat("jets_etaphiMoment",jetRef->etaphiMoment());
    NtupleFactory_ -> FillFloat("jets_jetArea",jetRef->jetArea());
    
    
    
    float DRMin = 0.01;
    int jMin = -1;
    
    for(unsigned int j=0; j<JetHandle_forID->size(); ++j)
    {
      reco::CaloJetRef jetRef_forID(JetHandle_forID, j);
      
      float DRTemp = ROOT::Math::VectorUtil::DeltaR(jetRef->p4(), jetRef_forID->p4());
      if(DRTemp < DRMin)
      {
        DRMin = DRTemp;
        jMin = j;
      }
    }
    
    if(jMin != -1)
    {
      reco::CaloJetRef jetRef_match(JetHandle_forID, jMin);
      
      NtupleFactory_ -> FillFloat("jets_fHPD",((*jetIDHandle)[jetRef_match]).fHPD);
      NtupleFactory_ -> FillFloat("jets_fRBX",((*jetIDHandle)[jetRef_match]).fRBX);
      NtupleFactory_ -> FillFloat("jets_n90Hits",((*jetIDHandle)[jetRef_match]).n90Hits);
      NtupleFactory_ -> FillFloat("jets_nHCALTowers",((*jetIDHandle)[jetRef_match]).nHCALTowers);
      NtupleFactory_ -> FillFloat("jets_nECALTowers",((*jetIDHandle)[jetRef_match]).nECALTowers);
    }
    
    
    
    if(saveJetBTagging_)
      fillJetBTaggingInfo(iEvent, iESetup, jetRef->p4());
    
  } // loop on jets
  
  //std::cout << "SimpleNtple::fillJetInfo::end" << std::endl;
}






///---- PFJets ----
void SimpleNtple::fillPFJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillPFJetInfo::begin" << std::endl;
  
  edm::Handle<reco::PFJetCollection> JetHandle ;
  iEvent.getByLabel (JetTag_,JetHandle);
  
  edm::Handle<edm::RefVector<reco::PFJetCollection> > JetRefHandle;
  if(doJetRefCheck_)
    iEvent.getByLabel(JetRefTag_, JetRefHandle);
  
  
 
  // loop on jets
  for(unsigned int i=0; i<JetHandle->size(); ++i) 
  { 
    reco::PFJetRef jetRef(JetHandle, i);
    
    // do the reference check
    bool isJetRefCheckOk = true;
    if(doJetRefCheck_)
      if(find(JetRefHandle -> begin(), JetRefHandle -> end(), jetRef) == JetRefHandle -> end())
        isJetRefCheckOk = false;
    
    if(!isJetRefCheckOk) continue;
    
    
    
    NtupleFactory_ -> Fill4V   ("jets",jetRef->p4());
    NtupleFactory_ -> FillFloat("jets_emEnergyFraction",jetRef->neutralHadronEnergyFraction()); 
    NtupleFactory_ -> FillFloat("jets_chargedHadronEnergy",jetRef->chargedHadronEnergy()); 
    NtupleFactory_ -> FillFloat("jets_chargedHadronEnergyFraction",jetRef->chargedHadronEnergyFraction()); 
    NtupleFactory_ -> FillFloat("jets_neutralHadronEnergy",jetRef->neutralHadronEnergy()); 
    NtupleFactory_ -> FillFloat("jets_neutralHadronEnergyFraction",jetRef->neutralHadronEnergyFraction()); 
    NtupleFactory_ -> FillFloat("jets_chargedEmEnergy",jetRef->chargedEmEnergy()); 
    NtupleFactory_ -> FillFloat("jets_chargedEmEnergyFraction",jetRef->chargedEmEnergyFraction()); 
    NtupleFactory_ -> FillFloat("jets_chargedMuEnergy",jetRef->chargedMuEnergy()); 
    NtupleFactory_ -> FillFloat("jets_chargedMuEnergyFraction",jetRef->chargedMuEnergyFraction()); 
    NtupleFactory_ -> FillFloat("jets_neutralEmEnergy",jetRef->neutralEmEnergy()); 
    NtupleFactory_ -> FillFloat("jets_neutralEmEnergyFraction",jetRef->neutralEmEnergyFraction()); 
    NtupleFactory_ -> FillInt  ("jets_chargedMultiplicity",jetRef->chargedMultiplicity()); 
    NtupleFactory_ -> FillInt  ("jets_neutralMultiplicity",jetRef->neutralMultiplicity()); 
    NtupleFactory_ -> FillInt  ("jets_muonMultiplicity",jetRef->muonMultiplicity()); 
    
    if(saveJetBTagging_)
      fillJetBTaggingInfo(iEvent, iESetup, jetRef->p4());
    
  } // loop on jets
  
  //std::cout << "SimpleNtple::fillPFJetInfo::end" << std::endl;
}






///---- JetBTagging ----
void SimpleNtple::fillJetBTaggingInfo(const edm::Event & iEvent, const edm::EventSetup & iESetup,
                                      const ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<double> >& v1)
{
  //std::cout << "SimpleNtple::fillJetBTaggingInfo::begin" << std::endl;
  
  edm::Handle<reco::JetTagCollection> bTagHandle_trackCountingHighEff;
  iEvent.getByLabel("trackCountingHighEffBJetTags", bTagHandle_trackCountingHighEff);  
  
  edm::Handle<reco::JetTagCollection> bTagHandle_trackCountingHighPur;
  iEvent.getByLabel("trackCountingHighPurBJetTags", bTagHandle_trackCountingHighPur);  
  
  //edm::Handle<reco::JetTagCollection> bTagHandle_simpleSecondaryVertexHighEff;
  //iEvent.getByLabel("simpleSecondaryVertexHighEffBJetTags", bTagHandle_simpleSecondaryVertexHighEff);
  
  //edm::Handle<reco::JetTagCollection> bTagHandle_simpleSecondaryVertexHighPur;
  //iEvent.getByLabel("simpleSecondaryVertexHighPurBJetTags", bTagHandle_simpleSecondaryVertexHighPur);
  
  //edm::Handle<reco::JetTagCollection> bTagHandle_combinedSecondaryVertex;
  //iEvent.getByLabel("combinedSecondaryVertexBJetTags", bTagHandle_combinedSecondaryVertex);
  
  //edm::Handle<reco::JetTagCollection> bTagHandle_combinedSecondaryVertexMVA;
  //iEvent.getByLabel("combinedSecondaryVertexMVABJetTags", bTagHandle_combinedSecondaryVertexMVA);  
  
  //edm::Handle<reco::JetTagCollection> bTagHandle_jetProbability;
  //iEvent.getByLabel("jetProbabilityBJetTags", bTagHandle_jetProbability);
  
  //edm::Handle<reco::JetTagCollection> bTagHandle_jetBProbability;
  //iEvent.getByLabel("jetBProbabilityBJetTags", bTagHandle_jetBProbability);
  
  
  
  // trackCountingHighEff
  float DRMin = 0.01;
  int jMin = -1;
  
  for(unsigned int j = 0; j < bTagHandle_trackCountingHighEff->size(); ++j)
  {
    float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_trackCountingHighEff)[j].first->p4());
    if(DRTemp < DRMin)
    {
      DRMin = DRTemp;
      jMin = j;
    }
  }
    
  if(jMin != -1) NtupleFactory_->FillFloat("jets_trackCountingHighEffBJetTags", (*bTagHandle_trackCountingHighEff)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_trackCountingHighEffBJetTags", -999999.);
  
  
  
  // trackCountingHighPur
  DRMin = 0.01;
  jMin = -1;
  
  for(unsigned int j = 0; j < bTagHandle_trackCountingHighPur->size(); ++j)
  {
    float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_trackCountingHighPur)[j].first->p4());
    if(DRTemp < DRMin)
    {
      DRMin = DRTemp;
      jMin = j;
    }
  }
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_trackCountingHighPurBJetTags", (*bTagHandle_trackCountingHighPur)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_trackCountingHighPurBJetTags", -999999.);
  
  
  
  /*// simpleSecondaryVertexHighEff 
  DRMin = 0.01;
  jMin = -1;
  
  for(unsigned int j = 0; j < bTagHandle_simpleSecondaryVertexHighEff->size(); ++j)
  {
    float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_simpleSecondaryVertexHighEff)[j].first->p4());
    if(DRTemp < DRMin)
    {
      DRMin = DRTemp;
      jMin = j;
    }
  }
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_simpleSecondaryVertexHighEffBJetTags", (*bTagHandle_simpleSecondaryVertexHighEff)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_simpleSecondaryVertexHighEffBJetTags", -999999.);
  
  
  
  // simpleSecondaryVertexHighPur 
  DRMin = 0.01;
  jMin = -1;
  
  for(unsigned int j = 0; j < bTagHandle_simpleSecondaryVertexHighPur->size(); ++j)
  {
    float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_simpleSecondaryVertexHighPur)[j].first->p4());
    if(DRTemp < DRMin)
    {
      DRMin = DRTemp;
      jMin = j;
    }
  }
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_simpleSecondaryVertexHighPurBJetTags", (*bTagHandle_simpleSecondaryVertexHighPur)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_simpleSecondaryVertexHighPurBJetTags", -999999.);
  
  
  
  // combinedSecondaryVertex
  DRMin = 0.01;
  jMin = -1;
  
  for(unsigned int j = 0; j < bTagHandle_combinedSecondaryVertex->size(); ++j)
  {
    float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_combinedSecondaryVertex)[j].first->p4());
    if(DRTemp < DRMin)
    {
      DRMin = DRTemp;
      jMin = j;
    }
  }
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_combinedSecondaryVertexBJetTags", (*bTagHandle_combinedSecondaryVertex)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_combinedSecondaryVertexBJetTags", -999999.);
  
  
  
  // combinedSecondaryVertexMVA
  DRMin = 0.01;
  jMin = -1;
  
  for(unsigned int j = 0; j < bTagHandle_combinedSecondaryVertexMVA->size(); ++j)
  {
    float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_combinedSecondaryVertexMVA)[j].first->p4());
    if(DRTemp < DRMin)
    {
      DRMin = DRTemp;
      jMin = j;
    }
  }
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_combinedSecondaryVertexMVABJetTags", (*bTagHandle_combinedSecondaryVertexMVA)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_combinedSecondaryVertexMVABJetTags", -999999.);
  
  
  
  // jetProbability
  DRMin = 0.01;
  jMin = -1;
  
  for(unsigned int j = 0; j < bTagHandle_jetProbability->size(); ++j)
  {
    float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_jetProbability)[j].first->p4());
    if(DRTemp < DRMin)
    {
      DRMin = DRTemp;
      jMin = j;
    }
  }
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_jetProbabilityBJetTags", (*bTagHandle_jetProbability)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_jetProbabilityBJetTags", -999999.);
  
  
  
  // jetBProbability
  DRMin = 0.01;
  jMin = -1;
  
  for(unsigned int j = 0; j < bTagHandle_jetBProbability->size(); ++j)
  {
    float DRTemp = ROOT::Math::VectorUtil::DeltaR(v1, (*bTagHandle_jetBProbability)[j].first->p4());
    if(DRTemp < DRMin)
    {
      DRMin = DRTemp;
      jMin = j;
    }
  }
  
  if(jMin != -1) NtupleFactory_->FillFloat("jets_jetBProbabilityBJetTags", (*bTagHandle_jetBProbability)[jMin].second);
  else           NtupleFactory_->FillFloat("jets_jetBProbabilityBJetTags", -999999.);*/
  
  //std::cout << "SimpleNtple::fillJetBTaggingInfo::end" << std::endl;
}  












///---- GenMet ----
void SimpleNtple::fillGenMetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillGenMetInfo::begin" << std::endl;
  
  edm::Handle< reco::GenMETCollection > genMetHandle ;
  iEvent.getByLabel( genMetTag_, genMetHandle ) ;
  for (reco::GenMETCollection::const_iterator gMIt = genMetHandle->begin (); gMIt != genMetHandle->end (); ++gMIt ) 
  {
   math::XYZTLorentzVector myvect_XYZT(gMIt->px(),gMIt->py(),gMIt->pz(),gMIt->energy());
   NtupleFactory_->Fill4V("genMet",myvect_XYZT);
  }
  
  //std::cout << "SimpleNtple::fillGenMetInfo::end" << std::endl;
}






///---- GenJet ----

void SimpleNtple::fillGenJetInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillGenJetInfo::begin" << std::endl;
  
  edm::Handle<edm::View<reco::GenJet> > genJetHandle ;
  iEvent.getByLabel (genJetTag_,genJetHandle);
  for (edm::View<reco::GenJet>::const_iterator genJetIt = genJetHandle->begin (); genJetIt != genJetHandle->end (); ++genJetIt ) 
  { 
    NtupleFactory_->Fill4V("genJets",genJetIt->p4());
  }
  
  //std::cout << "SimpleNtple::fillGenJetInfo::end" << std::endl;
}






///---- MC ----

void SimpleNtple::fillMCPtHatInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillPtHatInfo::begin" << std::endl; 
  
  edm::Handle<edm::HepMCProduct> HepMCEvt;
  iEvent.getByLabel("generator", HepMCEvt);
  float ptHat = HepMCEvt -> GetEvent() -> event_scale();
  
  NtupleFactory_->FillFloat("mc_ptHat", ptHat);
  
  //std::cout << "SimpleNtple::fillPtHatInfo::end" << std::endl; 
}


void SimpleNtple::fillMCHiggsInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillMCHiggsInfo::begin" << std::endl; 
  
  NtupleFactory_->Fill4V("mc_H",mcAnalysis_ -> mcH()->p4());
  NtupleFactory_->FillFloat("mc_H_charge",mcAnalysis_ -> mcH()->charge());
  
  //std::cout << "SimpleNtple::fillMCHiggsInfo::end" << std::endl; 
}



void SimpleNtple::fillMCHiggsDecayInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillMCHiggsDecayInfo::begin" << std::endl; 

 bool isValid = mcAnalysis_ -> isValid();
  
 if( (eventType_ == 0) && (isValid == true) )
 {
  NtupleFactory_->Fill4V("mcV1",mcAnalysis_ -> mcV1()->p4());
  NtupleFactory_->FillFloat("mcV1_charge",mcAnalysis_ -> mcV1()->charge());
  NtupleFactory_->FillFloat("mcV1_pdgId",mcAnalysis_ -> mcV1()->pdgId());
  
  NtupleFactory_->Fill4V("mcV2",mcAnalysis_ -> mcV2()->p4());
  NtupleFactory_->FillFloat("mcV2_charge",mcAnalysis_ -> mcV2()->charge());
  NtupleFactory_->FillFloat("mcV2_pdgId",mcAnalysis_ -> mcV2()->pdgId());
     
  NtupleFactory_->Fill4V("mcF1_fromV1",mcAnalysis_ -> mcF1_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV1_charge",mcAnalysis_ -> mcF1_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV1_pdgId",mcAnalysis_ -> mcF1_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV1",mcAnalysis_ -> mcF2_fromV1()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV1_charge",mcAnalysis_ -> mcF2_fromV1()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV1_pdgId",mcAnalysis_ -> mcF2_fromV1()->pdgId());

  NtupleFactory_->Fill4V("mcF1_fromV2",mcAnalysis_ -> mcF1_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF1_fromV2_charge",mcAnalysis_ -> mcF1_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF1_fromV2_pdgId",mcAnalysis_ -> mcF1_fromV2()->pdgId());

  NtupleFactory_->Fill4V("mcF2_fromV2",mcAnalysis_ -> mcF2_fromV2()->p4());
  NtupleFactory_->FillFloat("mcF2_fromV2_charge",mcAnalysis_ -> mcF2_fromV2()->charge());
  NtupleFactory_->FillFloat("mcF2_fromV2_pdgId",mcAnalysis_ -> mcF2_fromV2()->pdgId());
    
  NtupleFactory_->Fill4V("mcQ1_tag",mcAnalysis_ -> mcQ1_tag()->p4());
  NtupleFactory_->FillFloat("mcQ1_tag_charge",mcAnalysis_ -> mcQ1_tag()->charge());
  NtupleFactory_->FillFloat("mcQ1_tag_pdgId",mcAnalysis_ -> mcQ1_tag()->pdgId());

  NtupleFactory_->Fill4V("mcQ2_tag",mcAnalysis_ -> mcQ2_tag()->p4());
  NtupleFactory_->FillFloat("mcQ2_tag_charge",mcAnalysis_ -> mcQ2_tag()->charge());
  NtupleFactory_->FillFloat("mcQ2_tag_pdgId",mcAnalysis_ -> mcQ2_tag()->pdgId());
 }
 
 //std::cout << "SimpleNtple::fillMCHiggsDecayInfo::end" << std::endl; 
}
 


void SimpleNtple::fillMCEleInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillMCEleInfo::begin" << std::endl; 
  
  for(unsigned int eleIt = 0; eleIt < (mcAnalysis_ -> GetMcE()).size(); ++eleIt)
  {
    NtupleFactory_->Fill4V("mc_ele", (mcAnalysis_ -> GetMcE()).at(eleIt)->p4());
    NtupleFactory_->FillFloat("mc_ele_charge", (mcAnalysis_ -> GetMcE()).at(eleIt)->charge());
  }
  
  //std::cout << "SimpleNtple::fillMCEleInfo::end" << std::endl; 
}



void SimpleNtple::fillMCMuInfo (const edm::Event & iEvent, const edm::EventSetup & iESetup) 
{
  //std::cout << "SimpleNtple::fillMCMuInfo::begin" << std::endl; 
  
  for(unsigned int muIt = 0; muIt < (mcAnalysis_ -> GetMcMu()).size(); ++muIt)
  {
    NtupleFactory_->Fill4V("mc_mu", (mcAnalysis_ -> GetMcMu()).at(muIt)->p4());
    NtupleFactory_->FillFloat("mc_mu_charge", (mcAnalysis_ -> GetMcMu()).at(muIt)->charge());
  }
  
  //std::cout << "SimpleNtple::fillMCMuInfo::end" << std::endl; 
}
