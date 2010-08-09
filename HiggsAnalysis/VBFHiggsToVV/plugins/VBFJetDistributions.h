#ifndef VBFJetDistributions_h
#define VBFJetDistributions_h

#include "HiggsAnalysis/VBFHiggsToVV/interface/MCDumperVBF.h"
#include "HiggsAnalysis/VBFHiggsToVV/interface/VBFUtils.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h" 
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Particle.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/METReco/interface/GenMET.h"
#include "DataFormats/METReco/interface/GenMETCollection.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"
// #include "SimDataFormats/HepMCProduct/interface/HepMCProduct.h" //---- CMSSW_2_X_Y
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h" //---- CMSSW_3_X_Y


#include <iostream>
#include <iomanip>

#include "TFile.h"
#include "TH1F.h"






template <class TCollection>
class VBFJetDistributions : public edm::EDAnalyzer
{
  typedef edm::Ref<TCollection> JetRef;
  
  
  
 public:
  
  //! ctor
  VBFJetDistributions(const edm::ParameterSet& iConfig);
  
  //! dtor
  ~VBFJetDistributions();
  
  void beginJob(const edm::EventSetup&);
  void endJob();
  
  void analyze(const edm::Event&, const edm::EventSetup&);
  
  
  
 private:
  
  edm::InputTag m_srcGenParticles;
  edm::InputTag m_srcJets;
  
  bool m_doRefCheck;
  edm::InputTag m_srcJetsRef;
  
  std::string m_fileName;
  bool m_verbosity;
  int m_eventType;
  int m_eventId;
  
  
  
  TFile* m_outFile;
  
  
  
  // jet histograms
  TH1F* recoJetNDistr;
  
  TH1F* recoJetPtDistr;
  TH1F* recoJetEtDistr;
  TH1F* recoJetEDistr;
  TH1F* recoJetEtaDistr;
  
  
  
  TH1F* etMin15RecoJetNDistr;
  
  TH1F* etMin15RecoJetPtDistr;
  TH1F* etMin15RecoJetEtDistr;
  TH1F* etMin15RecoJetEDistr;
  TH1F* etMin15RecoJetEtaDistr;
  
  
};






template <class TCollection>
VBFJetDistributions<TCollection>::VBFJetDistributions(const edm::ParameterSet& iConfig):
 m_srcGenParticles(iConfig.getParameter<edm::InputTag>("srcGenParticles")),
 m_srcJets        (iConfig.getParameter<edm::InputTag>("srcJets")),
 m_doRefCheck(iConfig.getParameter<bool>("doRefCheck")),
 m_srcJetsRef(iConfig.getParameter<edm::InputTag>("srcJetsRef")),
 m_fileName (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFJetDistributions"))),
 m_verbosity(iConfig.getUntrackedParameter<bool>("verbosity", true)),
 m_eventType(iConfig.getUntrackedParameter<int>("eventType", 0)),
 m_eventId  (0)
{}

// ----------------------------------------------------------------






template <class TCollection>
VBFJetDistributions<TCollection>::~VBFJetDistributions()
{}

// ----------------------------------------------------------------






template <class TCollection>
void VBFJetDistributions<TCollection>::beginJob(const edm::EventSetup& iSetup)
{
  // jet histograms
  recoJetNDistr = new TH1F("recoJetNDistr", "recoJetNDistr", 100, 0., 100.);
  
  recoJetPtDistr = new TH1F("recoJetPtDistr", "recoJetPtDistr", 1000, 0., 500.);
  recoJetEtDistr = new TH1F("recoJetEtDistr", "recoJetEtDistr", 1000, 0., 500.);
  recoJetEDistr  = new TH1F("recoJetEDistr",  "recoJetEDistr",  1000, 0., 500.);
  recoJetEtaDistr = new TH1F("recoJetEtaDistr", "recoJetEtaDistr", 1000, -5., 5.);
  
  
  
  etMin15RecoJetNDistr = new TH1F("etMin15recoJetNDistr", "recoJetNDistr", 100, 0., 100.);
  
  etMin15RecoJetPtDistr = new TH1F("etMin15RecoJetPtDistr", "etMin15RecoJetPtDistr", 1000, 0., 500.);
  etMin15RecoJetEtDistr = new TH1F("etMin15RecoJetEtDistr", "etMin15RecoJetEtDistr", 1000, 0., 500.);
  etMin15RecoJetEDistr  = new TH1F("etMin15RecoJetEDistr",  "etMin15RecoJetEDistr",  1000, 0., 500.);
  etMin15RecoJetEtaDistr = new TH1F("etMin15RecoJetEtaDistr", "etMin15RecoJetEtaDistr", 1000, -5., 5.);
  
}

// ----------------------------------------------------------------






template <class TCollection>
void VBFJetDistributions<TCollection>::endJob()
{
  if(m_verbosity)
    std::cerr << "*** VBFJetDistributions::endJob ***" << std::endl;
  
  
  
  // Create File
  m_outFile = new TFile(m_fileName.c_str() , "RECREATE");
  m_outFile -> cd();
  
  
  
  m_outFile -> mkdir("recoJetDistr");  
  m_outFile -> cd("recoJetDistr");  
  
  WriteNormalized(recoJetNDistr);
  
  WriteNormalized(recoJetPtDistr); 
  WriteNormalized(recoJetEtDistr); 
  WriteNormalized(recoJetEDistr); 
  WriteNormalized(recoJetEtaDistr); 
  
  m_outFile -> cd();
  
  
  
  m_outFile -> mkdir("etMin15RecoJetDistr");  
  m_outFile -> cd("etMin15RecoJetDistr");  
  
  WriteNormalized(etMin15RecoJetNDistr);
  
  WriteNormalized(etMin15RecoJetPtDistr); 
  WriteNormalized(etMin15RecoJetEtDistr); 
  WriteNormalized(etMin15RecoJetEDistr); 
  WriteNormalized(etMin15RecoJetEtaDistr); 
  
  m_outFile -> cd();

  
  
  m_outFile -> Close();
}






template <class TCollection>
void VBFJetDistributions<TCollection>::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // set event id
  ++m_eventId;
  
  if( (m_eventId%100 == 0) && (m_verbosity == true) )
    std::cout << ">>>>>> VBFJetDistributions::Event number -----> " << std::fixed << std::setw(10) << m_eventId << std::endl;
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Get the collections
  // -----------------------------------------------------------------  

  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(m_srcGenParticles, genParticles);
  if( !(genParticles.isValid ()) )
  {
    std::cerr << ">>>>>> VBFJetDistributions::Warning: " << m_srcGenParticles << " not available" << std::endl;
    return;
  }
  
  
  
  edm::Handle<TCollection> jets;
  iEvent.getByLabel(m_srcJets, jets);
  if( !(jets.isValid()) )
  {
    std::cerr << ">>>>>> VBFJetDistributions::Warning: " << m_srcJets << " not available" << std::endl;
    return;
  }
  
  edm::Handle< edm::RefVector<TCollection> > jetsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcJetsRef, jetsRef);
  
  
  
  
  
  
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Fill the histograms
  // -----------------------------------------------------------------  
  
  
  
  int recoJetN = 0;
  int etMin15RecoJetN = 0;
  
  std::vector<JetRef> recoJets;
  for(unsigned int ii = 0; ii < jets -> size(); ++ii)
  {
    // do the reference check
    bool isRefCheckOk = true;
    JetRef jetRef(jets, ii);
    if(m_doRefCheck)
      if(find(jetsRef -> begin(), jetsRef -> end(), jetRef) == jetsRef -> end())
        isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    ++recoJetN;
    recoJets.push_back(jetRef);
    
    
    
    // pt distribution
    recoJetPtDistr  -> Fill( jetRef -> pt() );
    recoJetEtDistr  -> Fill( jetRef -> et() );
    recoJetEDistr   -> Fill( jetRef -> energy() );
    recoJetEtaDistr -> Fill( jetRef -> eta() );
    
    
    
    if(jetRef -> et() < 15.) continue;
    
    ++etMin15RecoJetN;
    
    etMin15RecoJetPtDistr  -> Fill( jetRef -> pt() );
    etMin15RecoJetEtDistr  -> Fill( jetRef -> et() );
    etMin15RecoJetEDistr   -> Fill( jetRef -> energy() );
    etMin15RecoJetEtaDistr -> Fill( jetRef -> eta() );
  }
  
  recoJetNDistr  -> Fill( recoJetN );
  etMin15RecoJetNDistr  -> Fill( etMin15RecoJetN );
    
  
  
  
  
  
  // MCDumperVBF
  if(m_eventType != 0) return;
  MCDumperVBF mcAnalysis(genParticles, 0, false);
  bool isValid = mcAnalysis.isValid();
  if(!isValid) return;
  
  
  
}

#endif
