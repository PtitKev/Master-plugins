
#include "node.h"
#include "DataAccess.h"

using namespace ydleMaster ;

int LoadPlugins (Kernel & k)
{
	k.RegisterNode (new node) ;
	return 1 ;
}