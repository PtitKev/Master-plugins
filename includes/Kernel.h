#ifndef KERNEL_H
#define KERNEL_H

#include <list>
#include <map>
#include <string>

#include "IConnector.h"
#include "IProtocol.h"
#include "INode.h"
#include "IFeature.h"
#include "IManager.h"

#include "DynLoad.h"
#include "ListPtr.h"

namespace ydleMaster {

class Plugin
{
  public:
    Plugin (std::string &) ;
    virtual ~Plugin () {};
    int Register (Kernel &) ;
  private:
    DynLoad mDyn ;
} ;

class Kernel
{
  public:
    typedef std::map<std::string, Plugin*> PluginMap;

    typedef ListPtr<IConnector> ConnectorList ;
    typedef ListPtr<IProtocol> ProtocolList ;
    typedef ListPtr<INode> NodeList ;
    typedef ListPtr<IFeature> FeatureList ;
    typedef ListPtr<IManager> ManagerList ;

  public:
    virtual ~Kernel () ;

    PluginMap & Plugins () ;
    void	LoadPlugins (std::string & dir) ;

    void RegisterConnector (IConnector * p) { this->_connectors.push_back (p); }
    void RegisterProtocol (IProtocol * p) { this->_protocols.push_back (p); }
    void RegisterNode (INode * p) { this->_nodes.push_back (p); }
    void RegisterFeature (IFeature * p) { this->_features.push_back (p); }
    void RegisterManager (IManager * p) { this->_managers.push_back (p); }

    ConnectorList & Connectors () ;
    ProtocolList & Protocols () ;
    NodeList & Nodes () ;
    FeatureList & Features () ;
    ManagerList & Managers () ;

    IConnector * Connector (std::string name) ;
    IProtocol * Protocol (std::string name) ;
    INode * Node (std::string name) ;
    IFeature * Feature (std::string name) ;
    IManager * Manager (std::string name) ;
    
    typedef int	(*tPluginFonction) (Kernel & ) ;

  private:
    NodeList _nodes ;
    ConnectorList	_connectors ;
    ProtocolList	_protocols ;
    FeatureList _features ;
    ManagerList _managers ;

  private:
    PluginMap _loadedPlugins;
} ;

} ; // namespace ydleMaster

#endif // KERNEL_H