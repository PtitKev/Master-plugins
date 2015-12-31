/*
 * NodesManager.cpp
 *
 *  Created on: Jan 6, 2014
 *      Author: denia
 */

#include <iostream>

#include "NodesManager.h"
#include "NodeRequestHandler.h"
#include "LogDestination.h"
#include "WebBrowser.h"
#include "main.h"

using namespace ydleMaster ;
using namespace std ;

void NodesManager::Init (Kernel *k)
{
	std::stringstream ihm;
	std::stringstream path;

  std::string nodeProtocol;
  std::string nodeType;
  std::string nodeName;

  int nodeId;

	ihm << PARAM_STR("ihm_address") << ":" << PARAM_STR("ihm_port");
	path << PARAM_STR("ihm_base") << PARAM_STR("ihm_nodes_list");

	memset (_nodes, 0, sizeof (_nodes)) ;

  try {
    YDLE_INFO << "Request HUB for nodes list";
		WebBrowser browser(ihm.str());
		const Json::Value nodes = browser.doGet(path.str(), "");
    for ( int index = 0; (unsigned)index < nodes.size(); ++index ) {

      nodeId = nodes[index].get("id", 0).asInt();
      nodeProtocol = nodes[index].get("protocol", "").asString();
      nodeType = nodes[index].get("type", "default").asString();
      nodeName = nodes[index].get("name", "").asString();

			IProtocol *pProtocol = k->Protocol (nodeProtocol) ;
      if (pProtocol == NULL) {
        YDLE_WARN << "Node " << nodeId << " : protocol " << nodeProtocol << " not loaded";
        continue;
      }
      
			if (nodeType == "default") {
        nodeType = pProtocol->getDefaultNode();
      }

      INode *pNode = k->Node (nodeType) ;
      if (pNode == NULL) {
        YDLE_WARN << "Node " << nodeId << " : type " << nodeType << " not loaded";
        continue;
      }

      _nodes[nodeId] = pNode ;
      
      YDLE_INFO << nodeId << " : " << nodeProtocol <<
        " : " << nodeType << " : " << nodeName;
    }
    _kernel = *k;
  }
  catch (const runtime_error & e)  {
    YDLE_FATAL << "NodesManager::Init FAILED !!! : " << e.what();
    exit_handler(1);
  }
}

void NodesManager::SubscribeREST (WebServer::HTTPServer *server)
{
  WebServer::NodeRequestHandler *n = new WebServer::NodeRequestHandler(this);
  server->RegisterHandler("/node", n);
}

int NodesManager::SendCmd (int target, int sender, int param, int cmd)
{
	INode *pNode = GetNode(target) ;
	if (pNode == NULL) {
		YDLE_FATAL << "NodesManager::SendCmd FAILED, node " << target << " unknown !!!"  ;
	}

	IFrame frame ;

	frame.Dump("Dump from NodesManager::SendMsg") ;

	return 1 ;
}

INode * NodesManager::GetNode (IFrame *frame)
{
	return GetNode (frame->receptor) ;
}

INode * NodesManager::GetNode (int nodeId)
{
  YDLE_INFO << "NodesManager::GetNode " << nodeId;
	return _nodes[nodeId] ;
}