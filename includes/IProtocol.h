/* 
 * File:   IScriptsManager.h
 * Author: Xylerk
 *
 * Created on 29 octobre 2015, 21:55
 */

#ifndef IPROTOCOL_H
#define IPROTOCOL_H

#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <sys/time.h>
#include <jsoncpp/json/json.h>

#include "IConnector.h"
#include "IFrame.h"
#include "INode.h"

#include "SettingsParser.h"
#include "YdleLog.h"
#include "ListPtr.h"
#include "WebServer.h"

namespace ydleMaster {

class IConnector;

class IProtocol
{
  public:
    virtual std::string Name () = 0 ;
    virtual std::string Class () = 0 ;
    virtual std::string Connector () = 0 ;

    virtual void Init (IConnector * connector) = 0 ;
    virtual void Start () = 0 ;

    virtual void Receive (uint8_t) = 0 ;
    virtual Json::Value onIHMRequest (const WebServer::HTTPRequest *) = 0 ;
    //~ virtual void Send (IFrame *) = 0 ;

    virtual std::string getDefaultNode () = 0 ;

  public:
    IProtocol () {} ;
    virtual ~IProtocol () {} ;

    typedef ListPtr<INode> NodeList ;
    void RegisterNode (INode * p) { this->_nodes.push_back (p); }
    NodeList & Nodes () ;

    template <class T>
    void SubscribeIHM (T* o, void (T::*func) (IFrame*))
    {
      _signalNewFrame.connect(bind(func, o,  _1));
    }

  protected:
    void NotifyIHM (IFrame *pFrame)
    {
      _signalNewFrame(pFrame);
    }
    boost::signals2::signal<void (IFrame*) > _signalNewFrame;
    IConnector * _connector ;

  private:
    NodeList	_nodes ;
} ;

} ; // namespace ydleMaster

#endif // IPROTOCOL_H