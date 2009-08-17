#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "PhysicsTools/UtilAlgos/interface/ObjectSelector.h"



//define this as a plug-in
DEFINE_SEAL_MODULE () ;






//-------------------------------------------------
// VBFElectronAmbiguityResolver
//-------------------------------------------------

#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFElectronAmbiguityResolver.h"
typedef ObjectSelector<VBFElectronAmbiguityResolver> VBFElectronAmbiguityResolution;
DEFINE_ANOTHER_FWK_MODULE(VBFElectronAmbiguityResolution);




//-------------------------------------------------
// VBFNtuple
//-------------------------------------------------

//#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFNtuple_PFJets.h"
//DEFINE_FWK_MODULE(VBFNtuple_PFJets);
#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFNtupleCaloJets.h"
DEFINE_FWK_MODULE(VBFNtupleCaloJets);
