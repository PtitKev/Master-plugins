
#include "Kernel.h"
#include "Float.h"

namespace ydleMaster {

class protocol : public IProtocol 
{
  public:
    virtual std::string Name () { return "EXAMPLE PROTOCOL" ; }
    virtual std::string Class () { return "protocol" ; }
    virtual std::string Connector () { return "connector" ; }
    virtual void Init () {} ;
    virtual void Start () {} ;

    virtual std::string getDefaultNode () { return "node_protocol" ; }
} ;

} ; // namespace ydleMaster

extern "C" int LoadPlugins (ydleMaster::Kernel &) ;