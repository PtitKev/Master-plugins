#ifndef NODESMANAGER_H
#define NODESMANAGER_H

#include <list>
#include <string>

#include "Kernel.h"
#include "IManager.h"
#include "IProtocol.h"

namespace ydleMaster {

class NodesManager : public IManager
{
  public:
    std::string Name () { return "nodes" ; }
    void Init (Kernel * k) ;
    int SendCmd (int target, int sender, int param, int cmd) ;

    INode *	GetNode (IFrame *) ;
    INode *	GetNode (int) ;

  public:
    NodesManager () {}; 
    ~NodesManager () {}; 
    void SubscribeREST (WebServer::HTTPServer *) ; 

    typedef std::list<int> tNodesList ;
    static const int NB_NODES = 256 ;

    IProtocol *	GetProtocol (std::string name)
    {
      return _kernel.Protocol(name) ;
    } ;

  private:
    Kernel _kernel ;
    INode * _nodes[NB_NODES+1] ; // 2^8 maximum nodes
} ;

} ; // namespace ydleMaster

#endif // NODESMANAGER_H