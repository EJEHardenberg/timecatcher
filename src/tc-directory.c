#include <stdlib.h>
#include <wordexp.h>
#include <dirent.h>
#include <errno.h>

#include "tc-directory.h"

const char * _tc_getHomePath(){
	const char * homePath;
	/* Determine the home directory */
	homePath = getenv("HOME");
	
	if ( homePath == NULL ) {
		/* Determine the home directory a different way */
		wordexp_t exp_result;
		wordexp("~", &exp_result, 0);
		homePath = (exp_result.we_wordv[0]);
	}

	return homePath;
}

int _tc_directoryExists(char * directoryToCheck){
	DIR * dir;
	int success;

	dir = opendir(directoryToCheck);
	if ( dir ) {
		closedir(dir);
		success = 1;
	} else if ( ENOENT == errno ) 
		success = 0; /* The directory doesn't exist */
	else 
		success = -1;/* Something went wrong with opening it */

	return success;
	
}