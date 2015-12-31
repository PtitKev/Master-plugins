#include <dirent.h>
#include <fnmatch.h>
#include <iostream>

#include "Dir.h"
	
using namespace ydleMaster ;
using namespace std ;

void ydleMaster::ListFiles (const char * dir, const char * pattern, StringList & files)
{
	// open directory
	DIR *dp;
	struct dirent *dirp;
	if((dp = opendir(dir)) == NULL) {
		cout << "Error(" << errno << ") opening " << dir << endl;
		return ;
	}
	// while there is an entry
	while ((dirp = readdir(dp)) != NULL) {
		// is the entry a regular file ?
		if (dirp->d_type != DT_REG) continue ;
		char * name = dirp->d_name ;
		// ignore file not matching 'so' extension
		if ((pattern != NULL) && fnmatch(pattern, name, FNM_CASEFOLD) != 0) continue ;
		files.push_back(string(name));
	}
}




