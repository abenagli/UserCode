#include <FWCore/PluginManager/interface/ModuleDef.h>



#include "Egamma/LLRSkim/plugins/SkimAnalyzer.h"
DEFINE_FWK_MODULE(SkimAnalyzer);

#include "Egamma/LLRSkim/plugins/EventIdFilter.h"
DEFINE_FWK_MODULE(EventIdFilter);
