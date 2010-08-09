#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/Common/interface/AssociationMap.h"
#include "DataFormats/Common/interface/Wrapper.h"

#include <iostream>
 
namespace
{
  struct dictionary
  {
    edm::AssociationMap<edm::OneToValue<std::vector<reco::GsfElectron>, float> > m1;
    edm::Wrapper<edm::AssociationMap<edm::OneToValue<std::vector<reco::GsfElectron>, float> > > w1;
    
    edm::AssociationMap<edm::OneToValue<std::vector<reco::Muon>, float> > m2;
    edm::Wrapper<edm::AssociationMap<edm::OneToValue<std::vector<reco::Muon>, float> > > w2;
   };
}
