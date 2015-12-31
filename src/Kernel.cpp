/*
 * Kernel.cpp
 *
 *  Created on: Jan 6, 2014
 *      Author: denia
 */

#include "Kernel.h"
#include "DynLoad.h"
#include "Dir.h"
#include <stdexcept>
#include <iostream>
#include <dlfcn.h>

using namespace std ;
using namespace ydleMaster ;

Kernel::~Kernel ()
{
}

void	Kernel::LoadPlugins (std::string & dir)
{
	StringList files ;
	ListFiles (dir.c_str(), "*.so", files) ;
	for (StringList::iterator it = files.begin(); it != files.end(); ++it) {
		string full = dir + "/" + it->c_str() ;
		Plugin *p = new Plugin (full) ;
		if (p->Register(*this) ) {
			this->_loadedPlugins.insert(PluginMap::value_type(*it, p)) ;
      YDLE_INFO << "\t- " << it->c_str() << " : OK";
		} else {
			delete p ;
      YDLE_WARN << "\t- " << it->c_str() << " : KO";
		}
	}
}

Kernel::PluginMap & Kernel::Plugins ()
{
	return _loadedPlugins ;
}

/********************************************************************/

Kernel::ConnectorList & Kernel::Connectors ()
{
	return _connectors ;
}

Kernel::ProtocolList & Kernel::Protocols ()
{
	return _protocols ;
}

Kernel::NodeList & Kernel::Nodes ()
{
	return _nodes ;
}

Kernel::FeatureList & Kernel::Features ()
{
	return _features ;
}

Kernel::ManagerList & Kernel::Managers ()
{
	return _managers ;
}

/********************************************************************/

IConnector * Kernel::Connector (string name)
{
	for (ConnectorList::iterator it = _connectors.begin(); it != _connectors.end(); ++it) {
		IConnector *p = *it ;
		if (name == p->Class()) return p ;
	}
	return NULL ;
}

IProtocol * Kernel::Protocol (string name)
{
	for (ProtocolList::iterator it = _protocols.begin(); it != _protocols.end(); ++it) {
		IProtocol *p = *it ;
		if (name == p->Class()) return p ;
	}
	return NULL ;
}

INode * Kernel::Node (string name)
{
	for (NodeList::iterator it = _nodes.begin(); it != _nodes.end(); ++it) {
		INode *p = *it ;
		if (name == p->Class()) return p ;
	}
	return NULL ;
}

IFeature * Kernel::Feature (string name)
{
	for (FeatureList::iterator it = _features.begin(); it != _features.end(); ++it) {
		IFeature *p = *it ;
		if (name == p->Name()) return p ;
	}
	return NULL ;
}

IManager * Kernel::Manager (string name)
{
	for (ManagerList::iterator it = _managers.begin(); it != _managers.end(); ++it) {
		IManager *m = *it ;
		if (name == m->Name()) return m ;
	}
	return NULL ;
}

/********************************************************************/

Plugin::Plugin(string &path) : mDyn(path.c_str())
{
}

int Plugin::Register (Kernel &k)
{
		return mDyn.ExecFuncTpl<int, Kernel&>  ("LoadPlugins", (Kernel &)k) ;
}