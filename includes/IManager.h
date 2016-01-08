/*
 * IManager.h
 *
 *  Created on: Fev 24, 2014
 *      Author: fets
 */

#ifndef IMANAGER_H
#define IMANAGER_H

#include <string.h>

#include "INode.h"
#include "IFrame.h"

#include "WebServer.h"

namespace ydleMaster {

class Kernel;

class IManager
{
  public:
    virtual std::string Name () = 0 ;
    virtual void Init (Kernel * k) = 0;
    virtual void	SubscribeREST (WebServer::HTTPServer *) = 0 ;
    virtual int	SendCmd (int target, int sender, int param, int cmd) = 0 ;

  public:
    virtual INode *	GetNode (IFrame *) = 0;
    virtual INode *	GetNode (int) = 0;

    virtual ~IManager () {} ;
} ;

} ; // namespace ydleMaster

#endif // IMANAGER_H