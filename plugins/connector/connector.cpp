
#include "connector.h"
#include "DataAccess.h"

using namespace ydleMaster ;

int LoadPlugins (Kernel & k)
{
  k.RegisterConnector (new connector) ;
  return 1 ;
}