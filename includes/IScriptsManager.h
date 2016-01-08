/* 
 * File:   IScriptsManager.h
 * Author: Xylerk
 *
 * Created on 29 octobre 2015, 21:55
 */

#ifndef ISCRIPTSMANAGER_H
#define ISCRIPTSMANAGER_H

#include <string>

namespace ydleMaster {

class IScriptsManager
{
public:
	virtual ~IScriptsManager () {}
	virtual int	ReloadScript (std::string script_file) = 0 ;
	virtual int	LoadScript (std::string script_file) = 0 ;
	virtual int	UnloadScript (std::string script_file) = 0 ;
} ;

} ; // namespace ydleMaster

#endif // ISCRIPTSMANAGER_H