
#include "ydle.h"

using namespace ydleMaster ;

int LoadPlugins (Kernel & k)
{
	k.RegisterProtocol (new ydle) ;
	k.RegisterNode (new node_ydle) ;
	return 1 ;
}