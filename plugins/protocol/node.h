
#include "Kernel.h"
#include "Float.h"

namespace ydleMaster {

class node_protocol : public INode 
{
  public:
    virtual std::string Name () { return "Node Protocol" ; }
    virtual std::string Class () { return "node_protocol" ; }
    virtual std::string Protocol () { return "protocol" ; }
    virtual void Init () {} ;
    virtual void Start () {} ;
} ;

} ; // namespace ydleMaster

extern "C" int LoadPlugins (ydleMaster::Kernel &) ;