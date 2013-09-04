#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "tc-init.h"
#include "tc-directory.h"
#include "tc-task.h"
#include "tc-view.h"
#include "tc-start.h"


int main(int argc, char const *argv[]) {
	char tcHomeDirectory[TC_MAX_BUFF];
	char switchStringStorage[TC_MAX_BUFF];
	char const * recurseBuff[3];
	struct tc_task working_task;
	int i;
	char taskName[TC_MAX_BUFF];
	time_t rawtime;
	int verboseFlag,switchFlag;


	/* Make sure environment is proper */
	tc_init(tcHomeDirectory);
	taskName[0] = '\0';

	/* Initialize task struct*/
	working_task.taskName = malloc(TC_MAX_BUFF*sizeof(char));
	if(working_task.taskName == NULL){
		fprintf(stderr, "%s\n", "Could not allocate memory for task name in working struct. Exiting");
		exit(1);
	}
	working_task.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));
	if(working_task.taskInfo == NULL){
		fprintf(stderr, "%s\n", "Could not allocate memory for task information in working struct. Exiting");
		free(working_task.taskName);
		exit(1);	
	}
	working_task.state = TC_TASK_NOT_FOUND;
	working_task.seqNum = 0;
	working_task.startTime=working_task.pauseTime=working_task.endTime=0;
	

	/* Determine what we've been asked to do */
	if ( argc <= 1 ) {
		/* Called with no arguments. Display Usage */
		_tc_display_usage(NULL);

	}else if ( argc == 2 ){
		/* Called with just a command, beworking_tasksides view let it go to usage*/
		if ( strcasecmp( argv[1], TC_VIEW_COMMAND) == 0 ) {
			/* View is the only command (so far) that renders w/ no arguments */
			_tc_view_no_args(working_task);
		} else {
			_tc_display_usage(argv[1]);
		}

	}else{
		/* Called with command and arguments of some kind*/


		/* Check for the help flag */
		_tc_help_check(argc,argv);

		/* Check for verbose flag */
		verboseFlag = _tc_args_flag_check(argc,argv,TC_VERBOSE_LONG, TC_VERBOSE_SHORT);

		/* Check for start's switch flag */
		switchFlag = _tc_args_flag_check(argc,argv,TC_SWITCH_LONG,TC_SWITCH_SHORT);

		/* If we made it this far, then we can assume we need to resolve a task name */
		/*strcpy(taskName,argv[2]);*/
		for(i=2; i < argc; ++i)
			if(argv[i][0] != '-')
				sprintf(taskName,"%s %s",taskName,argv[i]);
			else
				i = argc; /*Ignore any flag value*/

		/* Strip front white space or ending white space */
		trim(taskName);

		/* No help requested try to parse the command*/
		if( strcasecmp( argv[1], TC_VIEW_COMMAND ) == 0 )
			_tc_view_with_args(working_task, verboseFlag, argc, argv, taskName);

		else if ( strcasecmp( argv[1], TC_START_COMMAND ) == 0 ) {
			
			if(switchFlag == FALSE) 
				_tc_start(working_task, taskName, tcHomeDirectory);
			else{	
				recurseBuff[0] = argv[0];
				recurseBuff[1] = argv[1];
				recurseBuff[2] = taskName;
				/* Remove current file */
				_tc_getCurrentTaskPath(switchStringStorage);
				/* If the current task is active (not paused) then pause it */
				_find_current_task(&working_task);
				if(working_task.state != TC_TASK_NOT_FOUND){

					if( working_task.state == TC_TASK_STARTED ){
						/* Pause the task */
						working_task.state = TC_TASK_PAUSED;
						/* Set the paused time */
						rawtime = time(0); 
						if(rawtime == -1){
							fprintf(stderr, "%s\n", "Could not determine time. Exiting");
							free(working_task.taskInfo);
							exit(1);
						}
						working_task.pauseTime = rawtime;
						/* Wwrite the task out to the file */
						_tc_task_write(working_task,tcHomeDirectory);
					}

					remove(switchStringStorage);
					/* Free anything */
					free(working_task.taskInfo);
					free(working_task.taskName);				
					main(3, recurseBuff);	
				}else{
					/* No current task anyway so just start a new task */
					_tc_start(working_task, taskName, tcHomeDirectory);
				}
				
			}

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

	if(working_task.taskInfo != NULL)
		free(working_task.taskInfo);
	if(working_task.taskName != NULL)
		free(working_task.taskName);
	return FALSE;
}
