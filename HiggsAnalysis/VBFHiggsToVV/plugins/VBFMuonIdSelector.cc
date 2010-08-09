#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFMuonIdSelector.h"

double m_normalizedChi2Max;
double m_numberOfValidTrackerHitsMin;
double m_numberOfValidMuonHitsMin;
double m_d0Max;




VBFMuonIdSelector::VBFMuonIdSelector(const edm::ParameterSet& iConfig):
  m_srcVertices                (iConfig.getParameter<edm::InputTag>("srcVertices")),
  m_normalizedChi2Max          (iConfig.getParameter<double>("normalizedChi2Max")),
  m_numberOfValidTrackerHitsMin(iConfig.getParameter<int>("numberOfValidTrackerHitsMin")),
  m_numberOfValidMuonHitsMin   (iConfig.getParameter<int>("numberOfValidMuonHitsMin")),
  m_d0Max                      (iConfig.getParameter<double>("d0Max")),
  m_doRefCheck (iConfig.getParameter<bool>("doRefCheck")),
  m_srcMuonsRef(iConfig.getParameter<edm::InputTag>("srcMuonsRef"))
{}

// ----------------------------------------------------------------






VBFMuonIdSelector::~VBFMuonIdSelector()
{}

// ----------------------------------------------------------------






void VBFMuonIdSelector::select(edm::Handle<collection> muons,
                               const edm::Event& iEvent,
                               const edm::EventSetup& iSetup)
{
  m_selected.clear();
  
  
  
  edm::Handle< edm::RefVector<collection> > muonsRef;
  if(m_doRefCheck)
    iEvent.getByLabel(m_srcMuonsRef, muonsRef);  
  
  edm::Handle<reco::VertexCollection> vertexes;
  iEvent.getByLabel(m_srcVertices, vertexes);  
  
  
  
  for(unsigned int i = 0; i < muons -> size(); ++i)
  {
    // do the reference check
    bool isRefCheckOk = true;
    muon muonRef(muons, i);
    if(m_doRefCheck)
      if(find(muonsRef -> begin(), muonsRef -> end(), muonRef) == muonsRef -> end())
	isRefCheckOk = false;

    if(!isRefCheckOk) continue;
    
    
    
    bool isMuIdOk = true;
    
    
    
    // select the primary vertex
    reco::Vertex PV;
    bool PVfound = (vertexes -> size() != 0);

    if(PVfound)
    {
      // select the primary vertex as the one with higest sum of (pt)^2 of tracks
      PrimaryVertexSorter PVSorter;
      std::vector<reco::Vertex> sortedVertices = PVSorter.sortedList( *(vertexes.product()) );
      PV = sortedVertices.front();
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

    
    // selections of GlobalMuonPromptTight
    if( !muonRef->isGlobalMuon() ) isMuIdOk = false;
    //if( muonRef->globalTrack()->normalizedChi2() > m_normalizedChi2Max ) isMuIdOk = false;
    //if( muonRef->globalTrack()->hitPattern().numberOfValidMuonHits() < m_numberOfValidMuonHitsMin) isMuIdOk = false;
    
    // further selections
    //if( muonRef->globalTrack()->hitPattern().numberOfValidTrackerHits() < m_numberOfValidTrackerHitsMin) isMuIdOk = false;
    //if( fabs(muonRef->innerTrack()->dxy(PVPoint)) > m_d0Max ) isMuIdOk = false;    
    
    // to remove punch-throughs that end in the first muon wheel
    //math::XYZPoint outerPosition = muonRef -> globalTrack() -> outerPosition();
    //float z = outerPosition.z();
    //float r = outerPosition.r();
    
    // barrel region
    //if( (abs(z) < 660) && (r > 400) && (r < 480) ) isMuIdOk = false;
    
    // endcap region
    //if( (abs(z) > 600) && (abs(z) < 650) && (r < 300) ) isMuIdOk = false;
    //if( (abs(z) > 680) && (abs(z) < 730) && (r < 480) ) isMuIdOk = false;
    
    
    
    if( (isMuIdOk == true ) )
      m_selected.push_back(muonRef);
  }
  
}
