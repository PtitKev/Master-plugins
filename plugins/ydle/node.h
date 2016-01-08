
#include "Kernel.h"
#include "Float.h"

namespace ydleMaster {

class node_ydle : public INode 
{
  public:
    std::string Name () { return "Node Ydle" ; }
    std::string Class () { return "node_ydle" ; }
    std::string Protocol () { return "ydle" ; }

    void Init () {} ;
    void Start () {} ;
} ;

} ; // namespace ydleMaster

extern "C" int LoadPlugins (ydleMaster::Kernel &) ;