#ifndef DYNLOAD_H
#define DYNLOAD_H

#include <string>
#include <stdexcept>

typedef	void *	tDynLoadHandle ;
typedef	void *	tDynLoadFunc ;
typedef	const char * PCSTR ;

class DynLoad
{

public:
	DynLoad (PCSTR dll = NULL) ;
	virtual ~DynLoad () ;
	void	SetDllName (PCSTR dll) ;
	void *	ExecFunc (PCSTR funcName) ;
	tDynLoadFunc	GetSymbol (PCSTR funcName) ;

	template <typename tRet, typename tParam>
	tRet	ExecFuncTpl (PCSTR funcName, tParam param)
	{
		typedef tRet (* tPtrFunc) (tParam) ;
		tPtrFunc allocator = (tPtrFunc)GetSymbol (funcName) ;
		if (allocator == NULL) {
			std::string s ("ExecFuncTpl :") ;
			s += std::string(funcName) + " FAILED " ;
			throw std::runtime_error(s);
			return tRet(false) ;
		}
	    return allocator (param) ;
	}

protected:
  std::string		mDllName ;
	tDynLoadHandle 	mHandle ;
} ;

#endif // DYNLOAD_H