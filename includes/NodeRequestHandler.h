/*
 * NodeRequestHandler.h
 *
 *  Created on: Dec 20, 2013
 *      Author: denia
 */

#ifndef NODEREQUESTHANDLER_H
#define NODEREQUESTHANDLER_H

#include <jsoncpp/json/json.h>

#include "NodesManager.h"

#include "WebServer.h"

namespace WebServer {

class NodeRequestHandler  : public HTTPServer::BaseHTTPCallback
{
  private:
    ydleMaster::NodesManager *_nodesManager;

  public:
    NodeRequestHandler(ydleMaster::NodesManager *p);
    virtual ~NodeRequestHandler();
    int Run (const HTTPRequest *request, HTTPResponse *response);

  protected:
    Json::Value jsonReply;
    void SetResponse (const char *result, const char *msg) ;
};

} // namespace WebServer

#endif // NODEREQUESTHANDLER_H