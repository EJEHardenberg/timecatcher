#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wordexp.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

#define TC_MAX_BUFF 255
#define TC_DIR_PERM 0755

void init(){
	/* Create the .tc directory if it does not exist */
	const char * homePath;
	char tcdirectory[TC_MAX_BUFF];
	int success;
	DIR * dir;
	/* Time Variables */
	time_t rawtime;
	struct tm * timeinfo;
	char currentDate[TC_MAX_BUFF/2];
	char taskParentDirectory[TC_MAX_BUFF];



	/* Determine the home directory */
	homePath = getenv("HOME");
	
	if ( homePath == NULL ) {
		/* Determine the home directory a different way */
		wordexp_t exp_result;
		wordexp("~", &exp_result, 0);
		homePath = (exp_result.we_wordv[0]);
	}
	
	sprintf(tcdirectory,"%s/.tc",homePath);

	/* Determine if the directory exists or not */
	dir = opendir(tcdirectory);
	if(dir)
		closedir(dir);
	else if ( ENOENT == errno ) {
		/* The directory doesn't exist */
		success = mkdir(tcdirectory,TC_DIR_PERM);
		if ( success == -1 )
			printf("%s\n", "Could not create timecatcher directory in home folder");
	} else 
		printf("%s\n", "Could not create timecatcher directory in home folder");
	
	/* Create the directory for the current days tasks */
	rawtime = time(0);
	timeinfo = localtime (&rawtime);
	strftime(currentDate,80,"%Y%m%d",timeinfo);

	sprintf(taskParentDirectory,"%s/.tc/%s",homePath,currentDate);

	dir = opendir(taskParentDirectory);
	if ( dir )
		closedir(dir);
	else if ( ENOENT == errno ){
		/* The directory does not exists yet */
		success = mkdir(taskParentDirectory, TC_DIR_PERM);
		if ( success == -1 )
			printf("%s\n", "Could not create task directory for current date");
	} else
		printf("%s\n", "Could not create task directory for current date");


}

int main(int argc, char const *argv[]) {
	printf("argc: %i\n", argc);
	printf("%s\n", argv[0]);

	init();


	return 0;
}
