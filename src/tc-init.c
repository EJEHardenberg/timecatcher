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
#include "tc-init.h"


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
	if(_tc_args_flag_check(argc,argv,TC_HELP_LONG,TC_HELP_SHORT) == TRUE)
		_tc_display_usage(argv[1]);
}

void _tc_display_usage(const char * command){
	const char * general_usage;
	const char * view_usage;
	const char * start_usage;
	const char * add_info_usage;
	const char * finish_usage;
	const char * pause_usage;

	general_usage = ""
	"tcatch <command> [<args>]\n"
	"\n"
	"The tcatch commands are the following:\n"
	"\tstart		Start a new task\n"
	"\tadd-info 	Append information about the current task\n"
	"\tfinish		Finish a task that has been started\n"
	"\tview		View the current task or a list of all tasks\n"
	"\n"
	"See tcatch <command> --help for information on a specific command\n"
	"\n";

	view_usage = ""
	"tcatch view [--help | -h][ --all | -a][ <task name> ][--verbose | -v]\n"
	"\n"
	"Running view with no arguments will display the current tasks information\n"
	"If there is no current task, tcatch will let you know.\n"
	"To see this help dialog you can run view with the --help flag\n"
	"To view information on all tasks, use the --all flag and if you want to see\n"
	"information on a single specific task, use view <task name>\n"
	;
	start_usage = ""
	"tcatch start [--help | -h][--switch | -s ] <task name>\n"
	"\n"
	"To see this help text use --help or -h. \n"
	"To switch from the current task to the new one pass --switch or -s. \n"
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

	pause_usage = ""
	"tcatch pause[-h|--help]\n"
	"\n"
	"Pause the current task. If there is no current task then this command does\n"
	"nothing\n"
	"To See this help dialog pass the --help or -h flag"
	;

	if( command == NULL || strcasecmp(command, TC_HELP_COMMAND) == 0 )
		printf("%s", general_usage);
	else if( strcasecmp(command, TC_VIEW_COMMAND ) == 0) 
		printf("%s\n", view_usage);
	else if( strcasecmp(command, TC_START_COMMAND ) ==0 ) 
		printf("%s\n", start_usage);
	else if ( strcasecmp(command, TC_ADD_INFO_COMMAND ) == 0 ) 
		printf("%s\n", add_info_usage);
	else if ( strcasecmp(command, TC_FINISH_COMMAND ) == 0 ) 
		printf("%s\n", finish_usage);
	else if (strcasecmp(command, TC_PAUSE_COMMAND) == 0)
		printf("%s\n", pause_usage);
	else{
		fprintf(stderr,"%s\n\n", "Command not recognized, usage:");
		_tc_display_usage(NULL);
	}
}



const char * tc_init(char tcdirectory[]){
	/* Create the .tc directory if it does not exist */
	const char * homePath;
	char indexDirectory[TC_MAX_BUFF];
	int success;
	/* Time Variables */
	time_t rawtime;
	struct tm * timeinfo;
	char currentDate[TC_MAX_BUFF/2];
	char indexFilePath[TC_MAX_BUFF];
	char taskDirectory[TC_MAX_BUFF];
	


	homePath = _tc_getHomePath();
	sprintf(tcdirectory,"%s/.tc",homePath);

	/* Determine if the .tc directory exists or not */
	if ((success = _tc_directoryExists(tcdirectory)) == 0)
		success = mkdir(tcdirectory,TC_DIR_PERM);

	if (success == -1) {
		fprintf(stderr,"%s\n", "Problem creating .tc directory in home folder. Please check permissions");
		exit(1);
	}

	/* Create the index directory to store index files */
	sprintf(indexDirectory,"%s/.tc/%s",homePath,TC_INDEX_DIR);

	if ((success = _tc_directoryExists(indexDirectory)) == 0)
		success = mkdir(indexDirectory,TC_DIR_PERM);
	
	if (success == -1) {
		fprintf(stderr,"%s\n", "Could not create index directory. Please check permissions");
		exit(1);
	}

	/* 	Now that the directory is setup correctly check for the index file */
	rawtime = time(0);
	timeinfo = localtime (&rawtime);
	strftime(currentDate,80,"%Y%m%d",timeinfo);
	sprintf(indexFilePath,"%s/.tc/%s/%s.index",homePath,TC_INDEX_DIR,currentDate);
	if ( _tc_file_exists(indexFilePath) ){
		/* It exists so we'll have it later on to use */
		success = 1;
	}else{
		/* Create the index file */
		FILE *fp = fopen(indexFilePath, "wb");
		if (!fp) {
    		success = -1;
			fprintf(stderr,"%s\n", "Could not create index file. Please check permissions");
			exit(1);    		
    	} else {
    		fclose(fp);
    		success = 1;
    	}

	}

	/* Create the tasks directory */
	sprintf(taskDirectory,"%s/.tc/%s",homePath,TC_TASK_DIR);
	if (( success = _tc_directoryExists(taskDirectory)) == 0)
		success = mkdir(taskDirectory,TC_DIR_PERM);

	if (success == -1) {
		fprintf(stderr,"%s\n", "Could not create tasks directory. Please check permissions");
		exit(1);
	}

	return tcdirectory;

}
