#include "HiggsAnalysis/VBFHiggsToWWTolnujj/plugins/VBFElectronAmbiguityResolver.h"






VBFElectronAmbiguityResolver::VBFElectronAmbiguityResolver(const edm::ParameterSet& iConfig)
{}

// ------------------------------------------------------------






 VBFElectronAmbiguityResolver::~VBFElectronAmbiguityResolver()
{}

// ------------------------------------------------------------






void VBFElectronAmbiguityResolver::select(edm::Handle<collection> inputHandle,
                                          const edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  m_selected.clear ();
  
  //PG get the actual product
  dump(&m_selected, inputHandle);
  
  container::iterator ShorterEnd;
  
  sort(m_selected.begin(), m_selected.end(), TKeEoPSorting()); 
  ShorterEnd = unique(m_selected.begin(), m_selected.end(), equalTk()); 
  m_selected.erase(ShorterEnd, m_selected.end());
  
  sort(m_selected.begin(), m_selected.end(), SCEeEoPSorting()); 
  ShorterEnd = unique(m_selected.begin(), m_selected.end(), equalSC()); 
  m_selected.erase(ShorterEnd, m_selected.end());
  
  return;
}       

// ------------------------------------------------------------






void VBFElectronAmbiguityResolver::dump(container* output, const edm::Handle<collection>& input) 
{
 for(unsigned int ii = 0; ii != input->size(); ++ii)
 {
   edm::Ref<reco::GsfElectronCollection> electronRef(input, ii);
   output -> push_back(electronRef);
 }   
 
 return;
}
