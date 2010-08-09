#include "HiggsAnalysis/VBFHiggsToVV/plugins/VBFLeptonTipLipProducer.h"






VBFLeptonTipLipProducer::VBFLeptonTipLipProducer(const edm::ParameterSet& iConfig):
 m_srcVertices (iConfig.getParameter<edm::InputTag>("srcVertices")),
 m_srcElectrons(iConfig.getParameter<edm::InputTag>("srcElectrons")),
 m_srcMuons    (iConfig.getParameter<edm::InputTag>("srcMuons"))
{
  std::string alias;
  std::string name = "";
  
  
  // for electrons
  //produces<std::vector<float> >(alias = name + "EleLip").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "EleTip").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "EleSLip").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "EleSTip").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "EleTipSignificance").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "EleLipSignificance").setBranchAlias(alias);
  
  //produces<eleMap>(alias = name + "EleLipMap").setBranchAlias(alias);
  //produces<eleMap>(alias = name + "EleTipMap").setBranchAlias(alias);
  //produces<eleMap>(alias = name + "EleSLipMap").setBranchAlias(alias);
  //produces<eleMap>(alias = name + "EleSTipMap").setBranchAlias(alias);
  produces<eleMap>(alias = name + "EleTipSignificanceMap").setBranchAlias(alias);
  produces<eleMap>(alias = name + "EleLipSignificanceMap").setBranchAlias(alias);  
  
  // for muons
  //produces<std::vector<float> >(alias = name + "MuLip").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "MuTip").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "MuSLip").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "MuSTip").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "MuTipSignificance").setBranchAlias(alias);
  //produces<std::vector<float> >(alias = name + "MuLipSignificance").setBranchAlias(alias);
  
  //produces<muMap>(alias = name + "MuLipMap").setBranchAlias(alias);
  //produces<muMap>(alias = name + "MuTipMap").setBranchAlias(alias);
  //produces<muMap>(alias = name + "MuSLipMap").setBranchAlias(alias);
  //produces<muMap>(alias = name + "MuSTipMap").setBranchAlias(alias);
  produces<muMap>(alias = name + "MuTipSignificanceMap").setBranchAlias(alias);
  produces<muMap>(alias = name + "MuLipSignificanceMap").setBranchAlias(alias);    
}

// ----------------------------------------------------------------






VBFLeptonTipLipProducer::~VBFLeptonTipLipProducer()
{}

// ----------------------------------------------------------------






void VBFLeptonTipLipProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  //std::auto_ptr<std::vector<float> > EleTip(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > EleLip(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > EleSTip(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > EleSLip(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > EleTipSignificance(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > EleLipSignificance(new std::vector<float>);
  
  //std::auto_ptr<eleMap> EleTipMap(new eleMap);
  //std::auto_ptr<eleMap> EleLipMap(new eleMap);
  //std::auto_ptr<eleMap> EleSTipMap(new eleMap);
  //std::auto_ptr<eleMap> EleSLipMap(new eleMap);
  std::auto_ptr<eleMap> EleTipSignificanceMap(new eleMap);
  std::auto_ptr<eleMap> EleLipSignificanceMap(new eleMap);
  
  float eleTip, eleLip, eleSTip, eleSLip;
  
  
  
  //std::auto_ptr<std::vector<float> > MuTip(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > MuLip(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > MuSTip(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > MuSLip(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > MuTipSignificance(new std::vector<float>);
  //std::auto_ptr<std::vector<float> > MuLipSignificance(new std::vector<float>);
  
  //std::auto_ptr<muMap> MuTipMap(new muMap);
  //std::auto_ptr<muMap> MuLipMap(new muMap);
  //std::auto_ptr<muMap> MuSTipMap(new muMap);
  //std::auto_ptr<muMap> MuSLipMap(new muMap);
  std::auto_ptr<muMap> MuTipSignificanceMap(new muMap);
  std::auto_ptr<muMap> MuLipSignificanceMap(new muMap);
  
  float  muTip,  muLip,  muSTip,  muSLip;
  
  
  
  
    
  
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
  
  
  
  // propagator for electrons:
  const MultiTrajectoryStateTransform* eleMTSTransform = new MultiTrajectoryStateTransform;
  const GsfPropagatorAdapter* elePropagator = new GsfPropagatorAdapter(AnalyticalPropagator(B.product(), oppositeToMomentum)); 
  
  // propagator for muons:
  const TrajectoryStateTransform* muTSTransform = new TrajectoryStateTransform;
  Propagator* muPropagator = new AnalyticalPropagator(B.product(), oppositeToMomentum);                    
  
  
  
  
  
  
  //----------
  // Electrons
  //---------- 
  
  edm::Handle<reco::GsfElectronCollection> electrons;
  iEvent.getByLabel(m_srcElectrons, electrons);
  
  reco::GsfTrackRef eleTrack;
  
  unsigned int eleIndex = 0;
  for(reco::GsfElectronCollection::const_iterator eleIt = electrons -> begin();
      eleIt != electrons -> end(); ++eleIt)
  {
    eleTrack = eleIt -> get<reco::GsfTrackRef>();
    reco::GsfElectronRef eleRef(electrons, eleIndex);

    TrajectoryStateOnSurface eleInnerTSOS;
    
    if(!eleTrack.isNull())
      eleInnerTSOS = eleMTSTransform -> innerStateOnSurface(*eleTrack, *trackerGeometry.product(), B.product());
    

    // get initial TSOS
    if( (!eleInnerTSOS.isValid()) || (eleTrack.isNull()) )
    {    
      //EleTip -> push_back(-9999);
      //EleLip -> push_back(-9999);
      //EleSTip -> push_back(-9999);
      //EleSLip -> push_back(-9999);
      //EleTipSignificance -> push_back(-9999);
      //EleLipSignificance -> push_back(-9999);
      
      //EleTipMap -> insert(eleRef, -9999);
      //EleLipMap -> insert(eleRef, -9999);
      //EleSTipMap -> insert(eleRef, -9999);
      //EleSLipMap -> insert(eleRef, -9999);
      EleTipSignificanceMap -> insert(eleRef, -9999);
      EleLipSignificanceMap -> insert(eleRef, -9999);
    }
    
    else
    {
      // propagate the inner TSOS to PV
      TrajectoryStateOnSurface eleVtxTSOS = TransverseImpactPointExtrapolator(*elePropagator).extrapolate(eleInnerTSOS, PVPoint);
      
      
      // protection for eventual failing extrapolation
      if(!eleVtxTSOS.isValid())
      {		 
        //EleTip -> push_back(-9999);
        //EleLip -> push_back(-9999);
        //EleSTip -> push_back(-9999);
        //EleSLip -> push_back(-9999);
        //EleTipSignificance -> push_back(-9999);
        //EleLipSignificance -> push_back(-9999);
        
        //EleTipMap -> insert(eleRef, -9999);
        //EleLipMap -> insert(eleRef, -9999);
        //EleSTipMap -> insert(eleRef, -9999);
        //EleSLipMap -> insert(eleRef, -9999);
        EleTipSignificanceMap -> insert(eleRef, -9999);
        EleLipSignificanceMap -> insert(eleRef, -9999);
      }
      
      else
      {
        // get the distances (transverse & longitudinal) between extrapolated position and PV position
        GlobalPoint eleVtxPoint = eleVtxTSOS.globalPosition();
	GlobalVector eleVtxVector = eleVtxPoint - PVPoint; 
        
	eleTip = eleVtxVector.perp(); // positive by definition
	eleLip = eleVtxVector.z();    // signed by definition
	
        
        //--------------------------------
        // compute full error calculation:
        //--------------------------------
        
	// PV errors - diagonal terms
	AlgebraicSymMatrix33 PVErrMatrix;              
	PVErrMatrix(0,0) = PVErr.x()*PVErr.x(); 
	PVErrMatrix(1,1) = PVErr.y()*PVErr.y();
	PVErrMatrix(2,2) = PVErr.z()*PVErr.z();

	// electron vertex TSOS errors after projection - diagonal and off-diagonal terms:
	AlgebraicSymMatrix33 eleVtxTSOSErrMatrix = eleVtxTSOS.cartesianError().matrix().Sub<AlgebraicSymMatrix33>(0,0);
        
        // total error matrix
	AlgebraicSymMatrix33 totErrMatrix = PVErrMatrix + eleVtxTSOSErrMatrix;
        
	AlgebraicVector2 mjacobianTranV;
	mjacobianTranV[0] = eleVtxVector.x()/eleVtxVector.perp();	
	mjacobianTranV[1] = eleVtxVector.y()/eleVtxVector.perp();
        
        AlgebraicVector1 mjacobianLongV;
	mjacobianLongV[0] = 1.;	
        
	// Tip/Lip errors:
	eleSTip = sqrt(ROOT::Math::Similarity(totErrMatrix.Sub<AlgebraicSymMatrix22>(0,0), mjacobianTranV));
	eleSLip = sqrt(ROOT::Math::Similarity(totErrMatrix.Sub<AlgebraicSymMatrix11>(2,2), mjacobianLongV));
	
              
        
        // fill variables
	//EleTip -> push_back(eleTip);
	//EleLip -> push_back(eleLip);
	//EleSTip -> push_back(eleSTip);
	//EleSLip -> push_back(eleSLip);
	//EleTipSignificance -> push_back(eleTip/eleSTip);
	//EleLipSignificance -> push_back(eleLip/eleSLip);
        
        //EleTipMap -> insert(eleRef, eleTip);
        //EleLipMap -> insert(eleRef, eleLip);
        //EleSTipMap -> insert(eleRef, eleSTip);
        //EleSLipMap -> insert(eleRef, eleSLip);
        EleTipSignificanceMap -> insert(eleRef, eleTip/eleSTip);
        EleLipSignificanceMap -> insert(eleRef, eleLip/eleSLip);
                
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

  unsigned int muIndex = 0;  
  for(reco::MuonCollection::const_iterator muIt = muons -> begin();
      muIt != muons -> end(); ++muIt)
  {
    muTrack = muIt -> get<reco::TrackRef>();
    reco::MuonRef muRef(muons, muIndex);
    
    TrajectoryStateOnSurface muInnerTSOS;
    
    if(!muTrack.isNull())
      muInnerTSOS = muTSTransform -> innerStateOnSurface(*muTrack, *trackerGeometry.product(), B.product());
    

    // get initial TSOS (now protected against STA muons):
    if( (!muInnerTSOS.isValid()) || (muTrack.isNull()) )
    {    
      //MuTip -> push_back(-9999);
      //MuLip -> push_back(-9999);
      //MuSTip -> push_back(-9999);
      //MuSLip -> push_back(-9999);
      //MuTipSignificance -> push_back(-9999);
      //MuLipSignificance -> push_back(-9999);
      
      //MuTipMap -> insert(muRef, -9999);
      //MuLipMap -> insert(muRef, -9999);
      //MuSTipMap -> insert(muRef, -9999);
      //MuSLipMap -> insert(muRef, -9999);
      MuTipSignificanceMap -> insert(muRef, -9999);
      MuLipSignificanceMap -> insert(muRef, -9999);
    }
    
    else
    {
      // propagate the inner TSOS to PV
      TrajectoryStateOnSurface muVtxTSOS = TransverseImpactPointExtrapolator(*muPropagator).extrapolate(muInnerTSOS, PVPoint);
      
      
      // protection for eventual failing extrapolation
      if(!muVtxTSOS.isValid())
      {		 
        //MuTip -> push_back(-9999);
        //MuLip -> push_back(-9999);
        //MuSTip -> push_back(-9999);
        //MuSLip -> push_back(-9999);
        //MuTipSignificance -> push_back(-9999);
        //MuLipSignificance -> push_back(-9999);
        
        //MuTipMap -> insert(muRef, -9999);
        //MuLipMap -> insert(muRef, -9999);
        //MuSTipMap -> insert(muRef, -9999);
        //MuSLipMap -> insert(muRef, -9999);
        MuTipSignificanceMap -> insert(muRef, -9999);
        MuLipSignificanceMap -> insert(muRef, -9999);
      }
      
      else
      {
        // get the distances (transverse & longitudinal) between extrapolated position and PV position
        GlobalPoint muVtxPoint = muVtxTSOS.globalPosition();
	GlobalVector muVtxVector = muVtxPoint - PVPoint; 
        
	muTip = muVtxVector.perp(); // positive by definition
	muLip = muVtxVector.z();    // signed by definition
	
        
        //--------------------------------
        // compute full error calculation:
        //--------------------------------
        
	// PV errors - diagonal terms
	AlgebraicSymMatrix33 PVErrMatrix;              
	PVErrMatrix(0,0) = PVErr.x()*PVErr.x(); 
	PVErrMatrix(1,1) = PVErr.y()*PVErr.y();
	PVErrMatrix(2,2) = PVErr.z()*PVErr.z();

	// muon vertex TSOS errors after projection - diagonal and off-diagonal terms:
	AlgebraicSymMatrix33 muVtxTSOSErrMatrix = muVtxTSOS.cartesianError().matrix().Sub<AlgebraicSymMatrix33>(0,0);
        
        // total error matrix
	AlgebraicSymMatrix33 totErrMatrix = PVErrMatrix + muVtxTSOSErrMatrix;
        
	AlgebraicVector2 mjacobianTranV;
	mjacobianTranV[0] = muVtxVector.x()/muVtxVector.perp();	
	mjacobianTranV[1] = muVtxVector.y()/muVtxVector.perp();
        
        AlgebraicVector1 mjacobianLongV;
	mjacobianLongV[0] = 1.;	
        
	// Tip/Lip errors:
	muSTip = sqrt(ROOT::Math::Similarity(totErrMatrix.Sub<AlgebraicSymMatrix22>(0,0), mjacobianTranV));
	muSLip = sqrt(ROOT::Math::Similarity(totErrMatrix.Sub<AlgebraicSymMatrix11>(2,2), mjacobianLongV));
	
              
        
        // fill variables
	//MuTip -> push_back(muTip);
	//MuLip -> push_back(muLip);
	//MuSTip -> push_back(muSTip);
	//MuSLip -> push_back(muSLip);
	//MuTipSignificance -> push_back(muTip/muSTip);
	//MuLipSignificance -> push_back(muLip/muSLip);
        
        //MuTipMap -> insert(muRef, muTip);
        //MuLipMap -> insert(muRef, muLip);
        //MuSTipMap -> insert(muRef, muSTip);
        //MuSLipMap -> insert(muRef, muSLip);
        MuTipSignificanceMap -> insert(muRef, muTip/muSTip);
        MuLipSignificanceMap -> insert(muRef, muLip/muSLip);
        
      } // protection against failing propagation
      
    } // protection against non valid TSOS  
    
    ++muIndex;
    
  } // muon loop
  
  
  
  
  
  const std::string& name = "";

  //iEvent.put(EleTip,             name + "EleTip");
  //iEvent.put(EleLip,             name + "EleLip");
  //iEvent.put(EleSTip,            name + "EleSTip");
  //iEvent.put(EleSLip,            name + "EleSLip");
  //iEvent.put(EleTipSignificance, name + "EleTipSignificance");
  //iEvent.put(EleLipSignificance, name + "EleLipSignificance");
  
  //iEvent.put(EleTipMap, name + "EleTipMap");
  //iEvent.put(EleLipMap, name + "EleLipMap");
  //iEvent.put(EleSTipMap, name + "EleSTipMap");
  //iEvent.put(EleSLipMap, name + "EleSLipMap");
  iEvent.put(EleTipSignificanceMap, name + "EleTipSignificanceMap");
  iEvent.put(EleLipSignificanceMap, name + "EleLipSignificanceMap");  
  
  
  //iEvent.put(MuTip,             name + "MuTip");
  //iEvent.put(MuLip,             name + "MuLip");
  //iEvent.put(MuSTip,            name + "MuSTip");
  //iEvent.put(MuSLip,            name + "MuSLip");
  //iEvent.put(MuTipSignificance, name + "MuTipSignificance");
  //iEvent.put(MuLipSignificance, name + "MuLipSignificance");
  
  //iEvent.put(MuTipMap, name + "MuTipMap");
  //iEvent.put(MuLipMap, name + "MuLipMap");
  //iEvent.put(MuSTipMap, name + "MuSTipMap");
  //iEvent.put(MuSLipMap, name + "MuSLipMap");
  iEvent.put(MuTipSignificanceMap, name + "MuTipSignificanceMap");
  iEvent.put(MuLipSignificanceMap, name + "MuLipSignificanceMap");  
  
  
  delete eleMTSTransform;
  delete elePropagator; 

  delete muTSTransform;
  delete muPropagator;
  
}
