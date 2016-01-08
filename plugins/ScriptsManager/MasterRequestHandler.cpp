/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "LogDestination.h"
#include "MasterRequestHandler.h"
#include <cstring>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include "Dir.h"
#include "SettingsParser.h"

using namespace ydleMaster ;

namespace WebServer {
    #define NBSEND 1

MasterRequestHandler::MasterRequestHandler(IScriptsManager* m) : _pScripts(m)
{
}

MasterRequestHandler::~MasterRequestHandler()
{
	// TODO Auto-generated destructor stub
}


int MasterRequestHandler::Run(const HTTPRequest *request, HTTPResponse *response) {
	response->SetContentType(HTTPServer::CONTENT_TYPE_JSON);
	Json::StyledWriter *writer = new Json::StyledWriter();
	Json::Value root;

	string url = request->Url();

	if(std::count(url.begin(), url.end(), '/') > 1){
		// Split the URL
		const char *pch = std::strtok((char*)url.data(), "/" );
		pch = std::strtok(NULL, "/" );
		url = pch;
	}

	if(url.compare("activateScript") == 0){
		ManageScript (request, root, true) ;
        }
        else if (url.compare("desactivateScript") == 0){
		ManageScript (request, root, false) ;
        }
	else{
		SetResponse (root, "ko", "unknown action") ;
	}
		
	response->Append(writer->write(root));
	return response->Send();

}
void MasterRequestHandler::SetResponse (Json::Value & root, const char * result, const char * message)
{
	if (result) root["result"] = result;
	if (message) root["message"] = message;
	;
}

void MasterRequestHandler::ManageScript (const HTTPRequest *request, Json::Value & root, bool status)
{
	string script_file = request->GetParameter("script_file");
	//string sender = request->GetParameter("sender");

	if (script_file.length() == 0) {
		SetResponse (root, "ko", " parameter <target> is missing") ;
	//} else if (sender.length() == 0) {
	//	SetResponse (root, "ko", " parameter <sender> is missing") ;
	}else if (status == true){
		int result;
		ActivateScript (script_file.c_str(), &result);
		SetResponse (root, "ok", NULL) ;
	}else {
                int result;
		DesactivateScript (script_file.c_str(), &result);
		SetResponse (root, "ok", NULL) ;
        }
}



int MasterRequestHandler::ActivateScript(string script_file, int* result)
{
	YDLE_DEBUG << "enter in ActivateScript " << script_file  <<std::endl;

	string scripts_dir = PARAM_STR("lua-scripts.scripts_dir");
        string script_to_load = scripts_dir + "/" + script_file + ".lua" ;
        string running_scripts_dir = PARAM_STR("lua-scripts.running_scripts_dir");
        std::ifstream src( script_to_load.c_str() ,std::ios::binary);
        string script_loaded = running_scripts_dir + "/" + script_file + ".lua" ;
        std::ofstream dst( script_loaded.c_str() ,std::ios::binary);
        dst<<src.rdbuf();
        src.close();
        dst.close();
        
            _pScripts->LoadScript (script_file);

	*result = 1;
	return 1;
}
int MasterRequestHandler::DesactivateScript(string script_file, int* result)
{
	YDLE_DEBUG << "enter in DesactivateScript " << script_file  <<std::endl;

	string running_scripts_dir = PARAM_STR("lua-scripts.running_scripts_dir");
        string script_to_unload = running_scripts_dir + "/" + script_file + ".lua" ;
        std::remove(script_to_unload.c_str());
        
            _pScripts->UnloadScript (script_file);

	*result = 1;
	return 1;
}
 
} 
/* namespace WebServer */
        