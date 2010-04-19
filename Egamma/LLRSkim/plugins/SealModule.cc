#include <FWCore/PluginManager/interface/ModuleDef.h>



//define this as a plug-in
DEFINE_SEAL_MODULE();



#include "Egamma/LLRSkim/plugins/SkimAnalyzer.h"
DEFINE_ANOTHER_FWK_MODULE(SkimAnalyzer);

#include "Egamma/LLRSkim/plugins/EventIdFilter.h"
DEFINE_ANOTHER_FWK_MODULE(EventIdFilter);
