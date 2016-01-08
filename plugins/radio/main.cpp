
#include "radio.h"

using namespace ydleMaster ;

int LoadPlugins (Kernel & k)
{
  k.RegisterConnector (new radio) ;
  return 1 ;
}