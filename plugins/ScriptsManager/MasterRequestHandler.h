/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MasterRequestHandler.h
 * Author: Xylerk
 *
 * Created on 27 octobre 2015, 22:07
 */

#ifndef MASTERREQUESTHANDLER_H
#define MASTERREQUESTHANDLER_H

#include <jsoncpp/json/json.h>

#include "IScriptsManager.h"

#include "WebServer.h"

namespace WebServer {

class MasterRequestHandler : public HTTPServer::BaseHTTPCallback
{
  private:
    ydleMaster::IScriptsManager *_pScripts;        

  public:
    MasterRequestHandler(ydleMaster::IScriptsManager *p);
    virtual ~MasterRequestHandler();
    int Run(const HTTPRequest *request, HTTPResponse *response);

  protected:
    void ManageScript(const HTTPRequest *request, Json::Value &r, bool status);
    int ActivateScript(std::string script_file, int* result);
    int DesactivateScript(std::string script_file, int* result);
    void SetResponse(Json::Value &r, const char * result, const char * msg);
};

} // namespace WebServer

#endif // MASTERREQUESTHANDLER_H