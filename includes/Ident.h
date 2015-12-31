#ifndef IDENT_H
#define IDENT_H

#include <string>

class Ident
{
public:
	Ident (std::string n="") : mName(n) {}
	virtual std::string Name ()  { return mName ;}
	const char * NameStr ()  { return mName.c_str() ;}
private:
	std::string		mName ;
} ;

#endif // IDENT_H