/*
 * NodeRequestHandler.cpp
 *
 *  Created on: Dec 20, 2013
 *      Author: denia
 */

#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include "INode.h"

#include "Dir.h"
#include "LogDestination.h"
#include "NodeRequestHandler.h"
#include "SettingsParser.h"

using namespace ydleMaster ;

namespace WebServer {

NodeRequestHandler::NodeRequestHandler(ydleMaster::NodesManager *n) : _nodesManager(n)
{
}

NodeRequestHandler::~NodeRequestHandler()
{
}

int NodeRequestHandler::Run (const HTTPRequest *request, HTTPResponse *response)
{
	//~ response->SetContentType(HTTPServer::CONTENT_TYPE_JSON);
	Json::StyledWriter *writer = new Json::StyledWriter();
  std::string tmp;

	string target = request->GetParameter("target");

  if(target.length() > 0) {
    INode *pNode = _nodesManager->GetNode (atoi(target.c_str())) ;
    if (pNode == NULL) {
      tmp = "node " + target + " UNKNOWN";
      char *error = const_cast<char*>(tmp.c_str());
      SetResponse ("KO", error) ;
    } else {
      IProtocol *pProtocol = _nodesManager->GetProtocol (pNode->Protocol ()) ;
      if (pProtocol == NULL) {
        tmp = "protocol " + target + " UNKNOWN";
        char *error = const_cast<char*>(tmp.c_str());
        SetResponse ("KO", error) ;
      } else {
        jsonReply = pProtocol->onIHMRequest(request) ;
      }
    }
  } else {
    SetResponse ("KO", "Target missing") ;
  }

	response->Append(writer->write(jsonReply));

	return response->Send();
}

void NodeRequestHandler::SetResponse (const char *result, const char *message)
{
	if (result) jsonReply["result"] = result;
	if (message) jsonReply["message"] = message;
}

} // namespace WebServer