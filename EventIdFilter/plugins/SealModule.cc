#include <FWCore/PluginManager/interface/ModuleDef.h>



//define this as a plug-in
DEFINE_SEAL_MODULE();



#include "CommonTools/EventIdFilter/plugins/EventIdFilter.h"
DEFINE_ANOTHER_FWK_MODULE(EventIdFilter);
