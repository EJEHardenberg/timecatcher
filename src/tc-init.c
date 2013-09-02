#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <wordexp.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <strings.h>

#include "tc-directory.h"
#define TC_VIEW_COMMAND "view"
#define TC_HELP_COMMAND "help"
#define TC_ADD_INFO_COMMAND "add-info"
#define TC_START_COMMAND "start"
#define TC_FINISH_COMMAND "finish"


const char * tc_init(char taskParentDirectory[]);
void _tc_display_usage(const char * command);

int main(int argc, char const *argv[]) {
	char taskParentDirectory[TC_MAX_BUFF];

	if ( argc <= 1 ) {
		/* Called with no arguments. Display Usage */
		_tc_display_usage(NULL);
	}else if ( argc == 2 ){
		/* Called with just a command, besides view let it go to usage*/
		if ( strcasecmp(argv[1], TC_VIEW_COMMAND) == 0 ) {
			;
		} else {
			_tc_display_usage(argv[1]);
		}

	}else{
		/* Called with command and arguments of some kind*/
	}

	tc_init(taskParentDirectory);
	printf("%s\n",taskParentDirectory );

	/* Determine what we're asked to do */



	return 0;
}

void _tc_display_usage(const char * command){
	const char * general_usage;
	general_usage = ""
	"tcatch <command> [<args>]\n"
	"\n"
	"The tcatch commands are the following:\n"
	"\tstart		Start a new task\n"
	"\tadd-info 	Append information about the current task\n"
	"\tfinish		Finish a task that has been started\n"
	"\tview		View the current task or a list of all tasks\n"
	"\n"
	"See tcatch help <command> for information on a specific command\n"
	"\n";

	if( command == NULL )
		printf("%s", general_usage);
	else if( strcasecmp(command, TC_VIEW_COMMAND ) == 0) { 
		;
	}else if( strcasecmp(command, TC_START_COMMAND ) ==0 ) {
		;
	}else if ( strcasecmp(command, TC_ADD_INFO_COMMAND ) == 0 ) {
		;
	}else if ( strcasecmp(command, TC_FINISH_COMMAND ) == 0 ) {
		;
	}else{
		fprintf(stderr,"%s\n\n", "Command not recognized, usage:");
		_tc_display_usage(NULL);
	}
		

	/* Exit when we're done with displaying usage*/
	exit(1);
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