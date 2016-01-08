
#include "Kernel.h"
#include "Float.h"

namespace ydleMaster {

class temp : public INode 
{
  public:
    virtual std::string Name () { return "Temp" ; }
    virtual std::string Class () { return "temp" ; }
    virtual std::string Protocol () { return "ydle" ; }
    virtual void Init () {} ;
    virtual void Start () {} ;
} ;

} ; // namespace ydleMaster

extern "C" int LoadPlugins (ydleMaster::Kernel &) ;