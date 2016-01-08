
#include "protocol.h"
#include "node.h"
#include "DataAccess.h"

using namespace ydleMaster ;

int LoadPlugins (Kernel & k)
{
	k.RegisterProtocol (new protocol) ;
	k.RegisterNode (new node_protocol) ;
	return 1 ;
}