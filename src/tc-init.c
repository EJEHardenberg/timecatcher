#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wordexp.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

#include "tc-directory.h"

const char * tc_init(char taskParentDirectory[]);

int main(int argc, char const *argv[]) {
	char taskParentDirectory[TC_MAX_BUFF];
	
	printf("argc: %i\n", argc);
	printf("%s\n", argv[0]);

	if ( argc <= 1 ) {
		/* Called with no arguments. Display Usage */
	}

	tc_init(taskParentDirectory);
	printf("%s\n",taskParentDirectory );

	/* Determine what we're asked to do */


	return 0;
}



const char * tc_init(char taskParentDirectory[]){
	/* Create the .tc directory if it does not exist */
	const char * homePath;
	char tcdirectory[TC_MAX_BUFF];
	int success;
	/* Time Variables */
	time_t rawtime;
	struct tm * timeinfo;
	char currentDate[TC_MAX_BUFF/2];
	


	homePath = _tc_getHomePath();
	sprintf(tcdirectory,"%s/.tc",homePath);

	/* Determine if the .tc directory exists or not */
	if ((success = _tc_directoryExists(tcdirectory)) == 0)
		success = mkdir(tcdirectory,TC_DIR_PERM);

	if (success == -1) 
			fprintf(stderr,"%s\n", "Problem creating .tc directory in home folder. Please check permissions");

	if (success == -1)
		exit(1);

	/* Create the directory for the current days tasks */
	rawtime = time(0);
	timeinfo = localtime (&rawtime);
	strftime(currentDate,80,"%Y%m%d",timeinfo);
	sprintf(taskParentDirectory,"%s/.tc/%s",homePath,currentDate);

	if ((success = _tc_directoryExists(taskParentDirectory)) == 0)
		success = mkdir(taskParentDirectory,TC_DIR_PERM);
	
	if (success == -1) 
		fprintf(stderr,"%s\n", "Could not create task directory for current date. Please check permissions");

	/* 	Now that the directory is setup correctly Do one final check and 
		then return the path to the directory we'll be writing to for 
		the session.
	*/

	if (success == -1)
		exit(1);

	return taskParentDirectory;

}