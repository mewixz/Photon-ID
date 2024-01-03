#include "GaudiKernel/DeclareFactoryEntries.h"
#include "DerivationFrameworkCNNPID/PhotonID.h"

using namespace DerivationFramework;
 
DECLARE_TOOL_FACTORY( PhotonID )

DECLARE_FACTORY_ENTRIES( DerivationFrameworkCNNPID ) {
   DECLARE_TOOL( PhotonID )
  
}
 
