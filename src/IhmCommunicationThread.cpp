/*
 * IhmCommunicationThread.cpp
 *
 *  Created on: Jan 6, 2014
 *      Author: denia
 */

#include "curl/curl.h"
#include <iostream>
#include <thread>
#include <string.h>

#include "INode.h"
#include "IManager.h"

#include "LogDestination.h"
#include "IhmCommunicationThread.h"
#include "NodesManager.h"
#include "WebBrowser.h"

using namespace std ;
using namespace ydleMaster ;

IhmCommunicationThread::IhmCommunicationThread(Kernel *kernel) :
	_kernel(kernel)
{
	listcmd_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
}

IhmCommunicationThread::~IhmCommunicationThread()
{
}

int IhmCommunicationThread::SendFrame(IFrame &frame)
{
  char sztmp[255];
  sprintf(sztmp,"IHM: Data Hex: ");
  for (int a=0;a<frame.taille-1;a++)
    sprintf(sztmp,"%s 0x%02X", sztmp, frame.data[a]);
  YDLE_DEBUG << sztmp;

	int sender = (int) frame.sender;

  IManager *nm = _kernel->Manager ("nodes");

	INode *pNode = nm->GetNode (sender) ;
	if (pNode == NULL) {
		YDLE_DEBUG << "SendFrame : node " << sender << " UNKNOWN";
		return 0 ;
	}

  std::stringstream ihm;
	ihm << PARAM_STR("ihm_address") << ":" << PARAM_STR("ihm_port");

	std::stringstream path;
	path << PARAM_STR("ihm_base") << PARAM_STR("ihm_datas_post");

  memset(sztmp, 0, sizeof(sztmp));
  for (int a=0;a<frame.taille-1;a++)
    sprintf(sztmp,"%s0x%02X", sztmp, frame.data[a]);

  stringstream buf;
  buf << "sender=" << sender << "&value=" << sztmp << "\r\n" ;

  WebBrowser browser(ihm.str());
  browser.doPost(path.str(), buf.str());

	return 1;
}

void IhmCommunicationThread::AddFrameToList(IFrame *frame) {

	YDLE_DEBUG << "Frame added";

	IFrame tmp;

	memcpy(&tmp, frame, sizeof(IFrame));

	pthread_mutex_lock(&listcmd_mutex);
	ListCmd.push_back(tmp);
	pthread_mutex_unlock(&listcmd_mutex);
}

void IhmCommunicationThread::ThreadBegin()
{
	YDLE_INFO << "Start Communication thread";
	SetPauseMs (1000) ;
}

void IhmCommunicationThread::ThreadAction()
{
	pthread_mutex_lock(&listcmd_mutex);
	int size = ListCmd.size();
	pthread_mutex_unlock(&listcmd_mutex);

	if(size <= 0) {
		Pause () ;
		return ;
	}

	for(int i = 0; i < size; i++){

		pthread_mutex_lock(&listcmd_mutex);
		IFrame frame = ListCmd.front();
		ListCmd.pop_front();
		pthread_mutex_unlock(&listcmd_mutex);

		SendFrame(frame);
	}
	Pause () ;
}