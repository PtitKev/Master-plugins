#ifndef THREAD_H
#define THREAD_H

#include <thread>
#include <time.h>

#include "Ident.h"

namespace ydleMaster {

class Thread : public Ident {
public:
	Thread (std::string n = "???") ;
	virtual ~Thread () ;

	void	Start() ;
	void	Stop() ;
	void	Pause() ;
	void	SetPauseMs (uint32_t val) ;
protected:
	virtual void ThreadBegin () {}
	virtual void ThreadAction () ;
	virtual void ThreadEnd () {}
	virtual void ThreadMain () ;
protected:
	std::thread *thread_t;
	bool running;
	struct timespec valPause ;
} ;

} ; // namespace ydleMaster

#endif // THREAD_H