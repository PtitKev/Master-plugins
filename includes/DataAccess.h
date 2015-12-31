#ifndef DATAACCESS_H
#define DATAACCESS_H

#include <map>
#include <string>

namespace ydleMaster {

class DataAccess : public std::map <std::string, double>
{
public:
	bool	Get(std::string n, double & v) ;
	static DataAccess data ;
};

} ; // namespace ydleMaster

#endif // DATAACCESS_H