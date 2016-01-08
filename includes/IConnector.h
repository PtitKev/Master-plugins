/*
 * IConnector.h
 *
 *  Created on: Fev 24, 2014
 *      Author: fets
 */

#ifndef ICONNECTOR_H
#define ICONNECTOR_H

#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <sys/time.h>

#include "IProtocol.h"
#include "INode.h"

#include "SettingsParser.h"
#include "YdleLog.h"
#include "ListPtr.h"

namespace ydleMaster {

class IConnector
{
  public:
    virtual std::string Name ()  = 0 ;
    virtual std::string Class ()  = 0 ;

    virtual void Init () = 0 ;
    virtual void Start () = 0 ;

    virtual void Receive () = 0 ;
    virtual void Send (bool) = 0 ;

  public:
    IConnector () {} ;
    virtual ~IConnector () {} ;

    typedef ListPtr<IProtocol> ProtocolList ;
    void RegisterProtocol (IProtocol * p) { this->_protocols.push_back (p); }
    ProtocolList & Protocols () ;

    void Dispatch(uint8_t rx_value) {
      ProtocolList & l = _protocols ;
      for (ProtocolList::iterator it = l.begin(); it != l.end(); ++it) {
        IProtocol * pt = *it ;
        pt->Receive (rx_value) ;
      }
    }

  private:
    ProtocolList	_protocols ;
} ;

} ; // namespace ydleMaster

#endif // ICONNECTOR_H