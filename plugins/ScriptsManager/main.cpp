
#include "nodeManager.h"
#include "DataAccess.h"

using namespace ydleMaster ;

int LoadPlugins (Kernel & k)
{
	k.RegisterManager (new NodesManager) ;
	return 1 ;
}