/*
 * IhmCommunicationThread.h
 *
 *  Created on: Jan 6, 2014
 *      Author: denia
 */

#ifndef IHMCOMMUNICATIONTHREAD_H
#define IHMCOMMUNICATIONTHREAD_H

#include <map>
#include <list>
#include <vector>

#include "INode.h"
#include "NodesManager.h"

#include "Kernel.h"
#include "Thread.h"

namespace ydleMaster {

class IhmCommunicationThread : public Thread
{
  public:
    IhmCommunicationThread(Kernel *);
    virtual ~IhmCommunicationThread();

    void AddFrameToList(IFrame *frame) ;
    int SendFrame(IFrame & frame);

  protected:
    virtual void ThreadBegin ();
    virtual void ThreadAction ();

  private:
    Kernel * _kernel ;
    IManager * _nodesManager ;
    std::list<IFrame> ListCmd ;
    pthread_mutex_t listcmd_mutex ;
};

} ; // namespace ydleMaster

#endif // IHMCOMMUNICATIONTHREAD_H