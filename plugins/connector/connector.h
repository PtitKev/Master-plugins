
#include "Kernel.h"
#include "Float.h"

namespace ydleMaster {

class connector : public IConnector
{
  public:
    virtual std::string Name () { return "EXAMPLE CONNECTOR" ; }
    virtual std::string Class () { return "connector" ; }
    virtual void Init () {} ;
    virtual void Start () {} ;
} ;

} ; // namespace ydleMaster

extern "C" int LoadPlugins (ydleMaster::Kernel &) ;