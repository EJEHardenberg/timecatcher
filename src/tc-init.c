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
#define TC_HELP_LONG "--help"
#define TC_HELP_SHORT "-h"
#define TC_VIEW_ALL_SHORT "-a"
#define TC_VIEW_ALL_LONG "--all"
#define TC_HELP_COMMAND "help"
#define TC_ADD_INFO_COMMAND "add-info"
#define TC_START_COMMAND "start"
#define TC_FINISH_COMMAND "finish"
#define TRUE 1
#define FALSE 0


const char * tc_init(char taskParentDirectory[]);
void _tc_display_usage(const char * command);
void _tc_help_check(int argc, char const *argv[]);
int _tc_args_flag_check(int argc, char const *argv[], char const * longFlag, char const * shortFlag);

int main(int argc, char const *argv[]) {
	char taskParentDirectory[TC_MAX_BUFF];

	/* Make sure environment is proper */
	tc_init(taskParentDirectory);
	printf("Working in: %s\n",taskParentDirectory );

	/* Determine what we've been asked to do */
	if ( argc <= 1 ) {
		/* Called with no arguments. Display Usage */
		_tc_display_usage(NULL);
	}else if ( argc == 2 ){
		/* Called with just a command, besides view let it go to usage*/
		if ( strcasecmp( argv[1], TC_VIEW_COMMAND) == 0 ) {
			;
		} else {
			_tc_display_usage(argv[1]);
		}

	}else{
		/* Called with command and arguments of some kind*/

		/* Check for the help flag */
		_tc_help_check(argc,argv);

		/* No help requested try to parse the command*/
		if( strcasecmp( argv[1], TC_VIEW_COMMAND ) == 0 ) {
			/* Check for all flag in any position*/
			if( _tc_args_flag_check(argc, argv, TC_VIEW_ALL_LONG, TC_VIEW_ALL_SHORT) == TRUE ){
				; /* Show all tasks */
				exit(1);
			}
			/* If we made it this far, then we can assume we need to resolve a task name */


		}else if ( strcasecmp( argv[1], TC_START_COMMAND ) == 0 ) {
			/* Check if we are already working on a task */

			/* Create a task and store its information */
		}else if (strcasecmp ( argv[1], TC_ADD_INFO_COMMAND ) == 0 ) {
			/* Check if there is a current task */

			/* Retrieve the current task */

			/* Add information to the task */

			/* Save the task */
		}else if (strcasecmp( argv[1], TC_FINISH_COMMAND ) == 0 ) {
			/* Check if there's a current task */

			/* Retrieve the current task */

			/* Finish the task */

			/* Save the task */
		}
	}

	return FALSE;
}

int _tc_args_flag_check(int argc, char const *argv[], char const * longFlag, char const * shortFlag){
	int counter;
	for(counter = 0; counter < argc; ++counter) {
		/* Check for a --help or -h flag */
		if(argv[counter][0] == '-'){
			if( strcasecmp( argv[counter], shortFlag ) == 0 || strcasecmp( argv[counter], longFlag ) == 0 ) {
				/* Flag found */
				return TRUE;
			}
		}
	}
	return FALSE;
}

void _tc_help_check(int argc, char const *argv[]){
	int counter;
	for(counter = 0; counter < argc; ++counter) {
		/* Check for a --help or -h flag */
		if(argv[counter][0] == '-'){
			if( strcasecmp( argv[counter], TC_HELP_SHORT ) == 0 || strcasecmp( argv[counter], TC_HELP_LONG ) == 0 ) {
				_tc_display_usage(argv[1]);
			}
		}
	}
}

void _tc_display_usage(const char * command){
	const char * general_usage;
	const char * view_usage;
	const char * start_usage;
	const char * add_info_usage;
	const char * finish_usage;

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

	view_usage = ""
	"tcatch view [--help | -h][ --all | -a][ <task name> ]\n"
	"\n"
	"Running view with no arguments will display the current tasks information\n"
	"If there is no current task, tcatch will let you know.\n"
	"To see this help dialog you can run view with the --help flag\n"
	"To view information on all tasks, use the --all flag and if you want to see\n"
	"information on a single specific task, use view <task name>\n"
	;
	start_usage = ""
	"tcatch start [--help | -h] <task name>\n"
	"\n"
	"To see this help text use --help or -h. \n"
	"To create a new task to be worked on simple use tcatch start and then the \n"
	"task name\n";
	add_info_usage = ""
	"tcatch add-info [--help | -h] <information>\n"
	"\n"
	"The add-info command appends a given information string to the current task.\n"
	"If there is no current task, this will fail. You can see this help text with\n"
	"--help or -h.\n"
	;
	finish_usage = ""
	"tcatch finish <task name>\n"
	"\n"
	"Finish a task identified by <task name>. Finishing a task marks the task as \n"
	"complete and creates the neccesary information to calculate the total time \n"
	"spent on the task itself. This information can be seen later with the view \n"
	"command. \n"
	"To See this help dialog pass the --help or -h flag.\n"
	;

	if( command == NULL || strcasecmp(command, TC_HELP_COMMAND) == 0 )
		printf("%s", general_usage);
	else if( strcasecmp(command, TC_VIEW_COMMAND ) == 0) { 
		printf("%s\n", view_usage);
	}else if( strcasecmp(command, TC_START_COMMAND ) ==0 ) {
		printf("%s\n", start_usage);
	}else if ( strcasecmp(command, TC_ADD_INFO_COMMAND ) == 0 ) {
		printf("%s\n", add_info_usage);
	}else if ( strcasecmp(command, TC_FINISH_COMMAND ) == 0 ) {
		printf("%s\n", finish_usage);
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
