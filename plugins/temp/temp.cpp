
#include "temp.h"
#include "DataAccess.h"

using namespace ydleMaster ;

int LoadPlugins (Kernel & k)
{
	k.RegisterNode (new temp) ;
	return 1 ;
}