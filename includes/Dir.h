#ifndef DIR_H
#define DIR_H

#include <list>
#include <string>

namespace ydleMaster {

typedef std::list<std::string> StringList ;

void ListFiles (const char * dir, const char * pattern, StringList & files) ;

} ; // namespace ydleMaster

#endif // DIR_H