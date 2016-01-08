
#include "Kernel.h"
#include "Float.h"

namespace ydleMaster {

class node : public INode 
{
  public:
    virtual std::string Name () { return "EXAMPLE NODE" ; }
    virtual std::string Class () { return "node" ; }
    virtual std::string Protocol () { return "protocol" ; }
    virtual void Init () {} ;
    virtual void Start () {} ;
} ;

} ; // namespace ydleMaster

extern "C" int LoadPlugins (ydleMaster::Kernel &) ;