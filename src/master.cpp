/**
 * Ydle - v0.5 (http://www.ydle.fr)
 * Home automotion project lowcost and DIY, all questions and information on http://www.ydle.fr
 *
 * @package Master
 * @license CC by SA
 **/

#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <sstream>

#include "Master.h"
#include "RestLogDestination.h"
#include "NodesManager.h"
#include "Scheduler.h"
#include "WebServer.h"

using namespace std;
using namespace ydleMaster;

Master::Master (int argc, char **argv) 
{
  // Init config
  InitConfig (argc, argv);

	// Init loging
  InitLog () ;

	// Init REST Loging
  InitRestLog();

	// Init Plugins
	InitPlugins () ;

	// Init Connectors
	InitConnectors () ;
	// Init Protocols
	InitProtocols () ;
	// Init Nodes
	InitNodes () ;
	// Init Features
	InitFeatures () ;
	// Init Features
	InitManagers () ;

  // Init REST Server
  InitRestServer();

  //~ _nodesManager.Init (&_kernel) ;
	//~ _scriptsManager.Init (&_kernel) ;
}

/**********************************************/

// Parse command line and config file
void Master::InitConfig (int argc, char **argv) 
{
	_pSettings = SettingsParser::Instance() ;

	if(_pSettings->parseCommandLine(argc, argv) == 0){
      throw std::runtime_error("No parameters");
	}

	if(_pSettings->parseConfigFile() == 0){
		throw std::runtime_error("parse config file FAILED");
	}
}

// Init logging system
void Master::InitLog () 
{
	stderr_log = new ydleMaster::StdErrorLogDestination(ydleMaster::YDLE_LOG_DEBUG);
	stderr_log->setLevel((ydleMaster::log_level) PARAM_INT("logger.stderr.level"));

	ydleMaster::InitLogging(ydleMaster::YDLE_LOG_DEBUG, stderr_log);

	YDLE_INFO << "Login enable : " << PARAM_INT("logger.stderr.level");
}

void Master::InitRestLog()
{
	std::stringstream ihm;

	ihm << PARAM_STR("ihm_address") << ":" << PARAM_STR("ihm_port") << PARAM_STR("ihm_base");

	restLog = new ydleMaster::RestLogDestination(
    (ydleMaster::log_level) PARAM_INT("logger.rest.level"),
    ihm.str()
  );
	restLog->Init();

	ydleMaster::InitLogging(ydleMaster::YDLE_LOG_DEBUG, restLog);

	YDLE_INFO << "REST Login enable : " << PARAM_INT("logger.rest.level");
}

/**********************************************/

void Master::InitPlugins ()
{
	string dir = PARAM_STR("plugins.dir");
	YDLE_INFO << "Searching plugins into directory: " << dir;
	_kernel.LoadPlugins (dir) ;
}

void Master::InitConnectors ()
{
	Kernel::ConnectorList & l = _kernel.Connectors() ;
  YDLE_INFO << "InitConnectors : " << l.size() ;
	for (Kernel::ConnectorList::iterator it = l.begin(); it != l.end(); ++it) {
		IConnector *c = *it ;
		YDLE_INFO << "- Connector : " << c->Name().c_str() ;
		c->Init () ;
		c->Start () ;
	}
}

void Master::InitProtocols ()
{
	Kernel::ProtocolList & l = _kernel.Protocols() ;
  YDLE_INFO << "InitProtocols : " << l.size() ;
	for (Kernel::ProtocolList::iterator it = l.begin(); it != l.end(); ++it) {
		IProtocol *pt = *it ;
    IConnector *c = _kernel.Connector (pt->Connector().c_str());
    if (c == NULL) {
      YDLE_FATAL << "Unable to load " << pt->Name().c_str() << " : connector " << pt->Connector().c_str() << " not loaded";
      continue;
    }
    c->RegisterProtocol (pt);
		YDLE_INFO << "- Protocol : " << pt->Name().c_str() << " on connector " << c->Name().c_str();
		pt->Init (c) ;
		pt->Start () ;
	}
}

void Master::InitNodes ()
{
	Kernel::NodeList & l = _kernel.Nodes() ;
  YDLE_INFO << "InitNodes : " << l.size() ;
	for (Kernel::NodeList::iterator it = l.begin(); it != l.end(); ++it) {
		INode *n = *it ;
    IProtocol *pt = _kernel.Protocol (n->Protocol().c_str());
    if (pt == NULL) {
      YDLE_FATAL << "Unable to load " << n->Name().c_str() << " : protocol " << n->Protocol().c_str() << " not loaded";
      continue;
    }
    pt->RegisterNode (n);
		YDLE_INFO << "- Node : " << n->Name().c_str() << " on protocol " << pt->Name().c_str();
		n->Init () ;
		n->Start () ;
	}
}

void Master::InitFeatures ()
{
	Kernel::FeatureList & l = _kernel.Features() ;
  YDLE_INFO << "InitFeatures : " << l.size() ;
	for (Kernel::FeatureList::iterator it = l.begin(); it != l.end(); ++it) {
		IFeature *f = *it ;
		YDLE_INFO << "- Feature : " << f->Name().c_str() ;
		f->Init () ;
	}
}

void Master::InitManagers ()
{
	_kernel.RegisterManager (new NodesManager) ;
	Kernel::ManagerList & l = _kernel.Managers() ;
  YDLE_INFO << "InitManagers : " << l.size() ;
	for (Kernel::ManagerList::iterator it = l.begin(); it != l.end(); ++it) {
		IManager *m = *it ;
		YDLE_INFO << "- Manager : " << m->Name().c_str() ;
		m->Init (&_kernel) ;
	}
}

/**********************************************/

void Master::InitRestServer()
{
	YDLE_INFO << "Init REST Server";
	WebServer::HTTPServer::HTTPServerOptions options;
	options.port = PARAM_INT("port");

	// Launch webserver
	_server = new WebServer::HTTPServer(options);

  Kernel::ManagerList & l = _kernel.Managers() ;
	for (Kernel::ManagerList::iterator it = l.begin(); it != l.end(); ++it) {
		IManager *m = *it ;
		YDLE_INFO << "Manager : " << m->Name().c_str() ;
		m->SubscribeREST (_server) ;
	}

  _server->Init();
  _server->Run();
}

void Master::Start()
{
	StartFeatures () ;
	StartComm () ;
}

void	Master::StartFeatures ()
{
  YDLE_INFO << "Start Features" ;
	Kernel::FeatureList & l = _kernel.Features() ;
	for (Kernel::FeatureList::iterator it = l.begin(); it != l.end(); ++it) {
		IFeature *f = *it ;
		f->Start () ;
	}
}

void Master::StartComm()
{
  YDLE_INFO << "Subscribe IHM to protocols";

	_pCom = unique_ptr<IhmCommunicationThread>(new IhmCommunicationThread(&_kernel));

	Kernel::ProtocolList & l = _kernel.Protocols() ;
	for (Kernel::ProtocolList::iterator it = l.begin(); it != l.end(); ++it) {
		IProtocol *pt = *it ;
		YDLE_INFO << "Protocol : " << pt->Name().c_str() ;
		pt->SubscribeIHM (_pCom.get(), &IhmCommunicationThread::AddFrameToList) ;
	}
  
	_pCom->Start();
}