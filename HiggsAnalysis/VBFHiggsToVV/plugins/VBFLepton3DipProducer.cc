#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFLepton3DipProducer.h"






VBFLepton3DipProducer::VBFLepton3DipProducer(const edm::ParameterSet& iConfig):
 m_srcVertices (iConfig.getParameter<edm::InputTag>("srcVertices")),
 m_srcElectrons(iConfig.getParameter<edm::InputTag>("srcElectrons")),
 m_srcMuons    (iConfig.getParameter<edm::InputTag>("srcMuons"))
{
  std::string alias;
  std::string name = "";
  
  
  // for electrons
  //produces<std::vector<float> >(alias = name + "Ele3DipSignificance").setBranchAlias(alias);
  produces<eleMap>(alias = name + "Ele3DipSignificanceMap").setBranchAlias(alias);
  
  // for muons
  //produces<std::vector<float> >(alias = name + "Mu3DipSignificance").setBranchAlias(alias);
  produces<muMap>(alias = name + "Mu3DipSignificanceMap").setBranchAlias(alias);    
}

// ----------------------------------------------------------------






VBFLepton3DipProducer::~VBFLepton3DipProducer()
{}

// ----------------------------------------------------------------






void VBFLepton3DipProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //std::auto_ptr<std::vector<float> > Ele3DipSignificance(new std::vector<float>);
  std::auto_ptr<eleMap> Ele3DipSignificanceMap(new eleMap);
  
  //std::auto_ptr<std::vector<float> > Mu3DipSignificance(new std::vector<float>);
  std::auto_ptr<muMap> Mu3DipSignificanceMap(new muMap);
  
  
  
  
  
    
  //-----------------------------------------
  // Get vertex collection and select the PV 
  //----------------------------------------
  
  edm::Handle<reco::VertexCollection> vertexes;
  iEvent.getByLabel(m_srcVertices, vertexes);
  
  
  // select the primary vertex 
  reco::Vertex PV;
  bool PVfound = (vertexes -> size() != 0);
  
  if(PVfound)
  {
    // select the primary vertex as the one
    // with higest sum of (pt)^2 of tracks
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
  
  GlobalPoint PVPoint(PV.position().x(), PV.position().y(), PV.position().z());
  Basic3DVector<double> PVErr(PV.xError(), PV.yError(), PV.zError());
  
  
  
  
  
  
  
  //------------------------
  // Propagate leptons to PV
  //------------------------
  
  // get the magnetic field 
  edm::ESHandle<MagneticField> B;
  iSetup.get<IdealMagneticFieldRecord>().get(B);
  
  // get the tracker geometry
  edm::ESHandle<TrackerGeometry> trackerGeometry;
  iSetup.get<TrackerDigiGeometryRecord>().get(trackerGeometry);
  
  // get the track builder
  edm::ESHandle<TransientTrackBuilder> trackBuilder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", trackBuilder);
  
  
  
  
  
  
  //----------
  // Electrons
  //---------- 
  
  edm::Handle<reco::GsfElectronCollection> electrons;
  iEvent.getByLabel(m_srcElectrons, electrons);
  
  reco::GsfTrackRef eleTrack;
  reco::TransientTrack eleTransTrack;  

  unsigned int eleIndex = 0;
  for(reco::GsfElectronCollection::const_iterator eleIt = electrons -> begin();
      eleIt != electrons -> end(); ++eleIt)
  {
    eleTrack = eleIt -> get<reco::GsfTrackRef>();
    
    if(!eleTrack.isNull())
     eleTransTrack = trackBuilder -> build(eleTrack);
    
    TrajectoryStateOnSurface eleTSOS;
    
    if(!eleTrack.isNull())
     eleTSOS = eleTransTrack.stateOnSurface(PVPoint);

    reco::GsfElectronRef eleRef(electrons, eleIndex);
    
    
    // get initial TSOS
    if( (!eleTSOS.isValid()) || (eleTrack.isNull()) )
    {    
      //Ele3DipSignificance -> push_back(-9999);
      Ele3DipSignificanceMap -> insert(eleRef, -9999);
    }
    
    else
    {
      // get the 3D impact parameter
      std::pair<bool, Measurement1D> ele3DipPair = IPTools::signedImpactParameter3D(eleTransTrack, eleTSOS.globalDirection(), PV);
      
      
      if(!ele3DipPair.first)
      {		 
        //Ele3DipSignificance -> push_back(-9999);
        Ele3DipSignificanceMap -> insert(eleRef, -9999);
      }
      
      else
      {
        // fill variables
	//Ele3DipSignificance -> push_back(ele3DipPair.second.significance());
        Ele3DipSignificanceMap -> insert(eleRef, ele3DipPair.second.significance());
               
      } // protection against failing propagation
      
    } // protection against non valid TSOS  
    
    ++eleIndex;
    
  } // electron loop
  
  
  
  
  
  
  //------
  // Muons
  //------ 
  
  edm::Handle<reco::MuonCollection> muons;
  iEvent.getByLabel(m_srcMuons, muons);
  
  reco::TrackRef muTrack;
  reco::TransientTrack muTransTrack;
  
  unsigned int muIndex = 0;  
  for(reco::MuonCollection::const_iterator muIt = muons -> begin();
      muIt != muons -> end(); ++muIt)
  {
    muTrack = muIt -> get<reco::TrackRef>();
    
    if(!muTrack.isNull())
     muTransTrack = trackBuilder -> build(muTrack);  
    
    TrajectoryStateOnSurface muTSOS;
    
    if(!muTrack.isNull())
     muTSOS = muTransTrack.stateOnSurface(PVPoint);
     
    reco::MuonRef muRef(muons, muIndex);
    
    
    // get initial TSOS (now protected against STA muons):
    if( (!muTSOS.isValid()) || (muTrack.isNull()) )
    {    
      //Mu3DipSignificance -> push_back(-9999);
      Mu3DipSignificanceMap -> insert(muRef, -9999);
    }
    
    else
    {
      // get the 3D impact parameter
      std::pair<bool, Measurement1D> mu3DipPair = IPTools::signedImpactParameter3D(muTransTrack, muTSOS.globalDirection(), PV);
      
      
      
      // protection for eventual failing extrapolation
      if(!mu3DipPair.first)
      {		 
        //Mu3DipSignificance -> push_back(-9999);
        Mu3DipSignificanceMap -> insert(muRef, -9999);
      }
      
      else
      {
        // fill variables
	//Mu3DipSignificance -> push_back(mu3DipPair.second.significance());
        Mu3DipSignificanceMap -> insert(muRef, mu3DipPair.second.significance());
        
      } // protection against failing propagation
      
    } // protection against non valid TSOS  
    
    ++muIndex;
    
  } // muon loop
  
  
  
  
  
  
  const std::string& name = "";

  //iEvent.put(Ele3DipSignificance, name + "Ele3DipSignificance");
  iEvent.put(Ele3DipSignificanceMap, name + "Ele3DipSignificanceMap");
  
  //iEvent.put(Mu3DipSignificance, name + "Mu3DipSignificance");
  iEvent.put(Mu3DipSignificanceMap, name + "Mu3DipSignificanceMap");
  
}
