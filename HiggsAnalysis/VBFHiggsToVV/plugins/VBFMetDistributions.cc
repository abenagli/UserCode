#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMetDistributions.h"






VBFMetDistributions::VBFMetDistributions(const edm::ParameterSet& iConfig):
 m_srcGenMetNoNuBSM       (iConfig.getParameter<edm::InputTag>("srcGenMetNoNuBSM")),
 m_srcCaloMet             (iConfig.getParameter<edm::InputTag>("srcCaloMet")),
 m_srcMuonCorrectedCaloMet(iConfig.getParameter<edm::InputTag>("srcMuonCorrectedCaloMet")),
 m_srcPFMet               (iConfig.getParameter<edm::InputTag>("srcPFMet")),
 m_fileName (iConfig.getUntrackedParameter<std::string> ("fileName", std::string("VBFMetDistributions"))),
 m_verbosity(iConfig.getUntrackedParameter<bool>("verbosity", true)),
 m_eventType(iConfig.getUntrackedParameter<int>("eventType", 0)),
 m_eventId  (0)
{}






VBFMetDistributions::~VBFMetDistributions()
{}






void VBFMetDistributions::beginJob(const edm::EventSetup& iSetup)
{
  if(m_verbosity)
    std::cerr << "*** VBFMetDistributions::beginJob ***" << std::endl;
  
  
  
  // met histograms
  genMetNoNuBSMDistr = new TH1F("genMetNoNuBSMDistr", "genMetNoNuBSMDistr", 1000, 0., 500.);
  
  caloMetDistr = new TH1F("caloMetDistr", "caloMetDistr", 1000, 0., 500.);
  caloMetRes = new TH1F("caloMetRes", "caloMetRes", 1000, -250., 250.);
  
  muonCorrectedCaloMetDistr = new TH1F("muonCorrectedCaloMetDistr", "muonCorrectedCaloMetDistr", 1000, 0., 500.);
  muonCorrectedCaloMetRes = new TH1F("muonCorrectedCaloMetRes", "muonCorrectedCaloMetRes", 1000, -250., 250.);
  
  PFMetDistr = new TH1F("PFMetDistr", "PFMetDistr", 1000, 0., 500.);
  PFMetRes = new TH1F("PFMetRes", "PFMetRes", 1000, -250., 250.);
}






void VBFMetDistributions::endJob()
{
  if(m_verbosity)
    std::cerr << "*** VBFMetDistributions::endJob ***" << std::endl;
  
  
  
  // Create File
  m_outFile = new TFile(m_fileName.c_str() , "RECREATE");
  m_outFile -> cd();
  
  
  
  WriteNormalized(genMetNoNuBSMDistr);
  
  WriteNormalized(caloMetDistr);
  WriteNormalized(caloMetRes);
  
  WriteNormalized(muonCorrectedCaloMetDistr);
  WriteNormalized(muonCorrectedCaloMetRes);
  
  WriteNormalized(PFMetDistr);
  WriteNormalized(PFMetRes);
  
  
  
  m_outFile -> Close();
}






void VBFMetDistributions::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // set event id
  ++m_eventId;
  
  if( (m_eventId%100 == 0) && (m_verbosity == true) )
    std::cout << ">>>>>> VBFMetDistributions::Event number -----> " << std::fixed << std::setw(10) << m_eventId << std::endl;
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Get the collections
  // -----------------------------------------------------------------  

  edm::Handle<reco::GenMETCollection> genMetNoNuBSM;
  iEvent.getByLabel(m_srcGenMetNoNuBSM, genMetNoNuBSM);
  if( !(genMetNoNuBSM.isValid ()) )
  {
    std::cerr << ">>>>>> VBFMetDistributions::Warning: " << m_srcGenMetNoNuBSM << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::CaloMETCollection> caloMet;
  iEvent.getByLabel(m_srcCaloMet, caloMet);
  if( !(caloMet.isValid ()) )
  {
    std::cerr << ">>>>>> VBFMetDistributions::Warning: " << m_srcCaloMet << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::CaloMETCollection> muonCorrectedCaloMet;
  iEvent.getByLabel(m_srcMuonCorrectedCaloMet, muonCorrectedCaloMet);
  if( !(muonCorrectedCaloMet.isValid ()) )
  {
    std::cerr << ">>>>>> VBFMetDistributions::Warning: " << m_srcMuonCorrectedCaloMet << " not available" << std::endl;
    return;
  }
  
  edm::Handle<reco::PFMETCollection> PFMet;
  iEvent.getByLabel(m_srcPFMet, PFMet);
  if( !(PFMet.isValid ()) )
  {
    std::cerr << ">>>>>> VBFMetDistributions::Warning: " << m_srcPFMet << " not available" << std::endl;
    return;
  }
  
  
  
  
  
  
  // -----------------------------------------------------------------
  // Fill the histograms
  // -----------------------------------------------------------------  
  
  reco::GenMETCollection::const_iterator genMetNoNuBSMIt = genMetNoNuBSM -> begin();
  genMetNoNuBSMDistr -> Fill(genMetNoNuBSMIt -> et());

  reco::CaloMETCollection::const_iterator caloMetIt = caloMet -> begin();
  caloMetDistr -> Fill(caloMetIt -> et());
  caloMetRes   -> Fill(caloMetIt -> et() - genMetNoNuBSMIt -> et());
  
  reco::CaloMETCollection::const_iterator muonCorrectedCaloMetIt = muonCorrectedCaloMet -> begin();
  muonCorrectedCaloMetDistr -> Fill(muonCorrectedCaloMetIt -> et());
  muonCorrectedCaloMetRes   -> Fill(muonCorrectedCaloMetIt -> et() - genMetNoNuBSMIt -> et());

  reco::PFMETCollection::const_iterator PFMetIt = PFMet -> begin();
  PFMetDistr -> Fill(PFMetIt -> et());
  PFMetRes   -> Fill(PFMetIt -> et() - genMetNoNuBSMIt -> et());
  
}
