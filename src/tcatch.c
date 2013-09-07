#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "tc-init.h"
#include "tc-directory.h"
#include "tc-task.h"
#include "tc-view.h"
#include "tc-start.h"
#include "tc-info.h"


int main(int argc, char const *argv[]) {
	char tcHomeDirectory[TC_MAX_BUFF];
	char switchStringStorage[TC_MAX_BUFF];
	char const * recurseBuff[3];
	int i;
	char taskName[TC_MAX_BUFF];
	time_t rawtime;
	int switchFlag;
	struct tc_task currentTask; /* Used by finish*/
	

	/* Make sure environment is proper */
	tc_init(tcHomeDirectory);
	memset(taskName,0,TC_MAX_BUFF);
	taskName[0] = '\0';
	

	/* Determine what we've been asked to do */
	if ( argc <= 1 ) {
		/* Called with no arguments. Display Usage */
		_tc_display_usage(NULL);

	}else if ( argc == 2 ){
		/* Called with just a command, beworking_tasksides view let it go to usage*/
		if ( strcasecmp( argv[1], TC_VIEW_COMMAND) == 0 ) {
			/* View is the only command (so far) that renders w/ no arguments */
			tc_view(argc,argv);
		} else {
			_tc_display_usage(argv[1]);
		}

	}else{
		/* Called with command and arguments of some kind*/


		/* Check for the help flag */
		_tc_help_check(argc,argv);


		/* Check for start's switch flag */
		switchFlag = _tc_args_flag_check(argc,argv,TC_SWITCH_LONG,TC_SWITCH_SHORT);

		/* If we made it this far, then we can assume we need to resolve a task name */
		for(i=2; i < argc; ++i)
			if(argv[i][0] != '-')
				sprintf(taskName,"%s %s",taskName,argv[i]);
			else
				continue; /*Ignore any flag value*/

		/* Strip front white space or ending white space */
		trim(taskName);

		/* No help requested try to parse the command*/
		if( strcasecmp( argv[1], TC_VIEW_COMMAND ) == 0 ){
			tc_view(argc, argv);
		}else if ( strcasecmp( argv[1], TC_START_COMMAND ) == 0 ) {
			struct tc_task working_task;
	working_task.taskName = malloc(TC_MAX_BUFF*sizeof(char));
	working_task.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));
	working_task.state = -1;
	working_task.seqNum = 0;
			
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

					/* Cant switch to the current task */
					if(strcmp(working_task.taskName,taskName)==0){
						fprintf(stderr, "%s\n", "Already working on that task. No need to switch");
						free(working_task.taskInfo);
						free(working_task.taskName);
						exit(1);
					}

					if( working_task.state == TC_TASK_STARTED ){
						/* Pause the task */
						working_task.state = TC_TASK_PAUSED;
						/* Set the paused time */
						rawtime = time(0); 
						if(rawtime == -1){
							fprintf(stderr, "%s\n", "Could not determine time. Exiting");
							free(working_task.taskInfo);
							free(working_task.taskName);
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
			if(working_task.taskInfo != NULL)
		free(working_task.taskInfo);
	if(working_task.taskName != NULL)
		free(working_task.taskName);

		}else if (strcasecmp ( argv[1], TC_ADD_INFO_COMMAND ) == 0 )
			tc_addInfo(argc,argv);
		else if (strcasecmp( argv[1], TC_FINISH_COMMAND ) == 0 ) {
			struct tc_task working_task;
	working_task.taskName = malloc(TC_MAX_BUFF*sizeof(char));
	working_task.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));
	working_task.state = -1;
	working_task.seqNum = 0;
			/* Check if there's a task */
			free(working_task.taskName);
			_tc_task_read(taskName, &working_task);
			if(working_task.state == TC_TASK_NOT_FOUND){
				free(working_task.taskInfo);
				fprintf(stderr, "%s\n", "Could not find the task to finish");
				exit(1);
			}else if(working_task.state == TC_TASK_FINISHED){
				/* Already finished. */
				free(working_task.taskInfo);
				fprintf(stderr, "%s\n", "Task is already finished. To resume use the start -s command");
				exit(1);
			}
			/* Finish the task */
			working_task.state = TC_TASK_FINISHED;
			rawtime = time(0); 
			if(rawtime == -1){
				fprintf(stderr, "%s\n", "Could not determine time. Exiting");
				free(working_task.taskInfo);
				free(working_task.taskName);
				exit(1);
			}
			working_task.endTime = rawtime;

			/* Check the current task before the write.*/
			currentTask.taskName = malloc(TC_MAX_BUFF*sizeof(char));
			currentTask.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));
			_find_current_task(&currentTask);
			_tc_getCurrentTaskPath(switchStringStorage);

			/* Save the task */
			_tc_task_write(working_task, tcHomeDirectory);
			_tc_view_no_args(working_task);

			/* If this task was the same as the current task, remove the current file */
			if(strcmp(currentTask.taskName,working_task.taskName)==0)
				remove(switchStringStorage); /* Reusing switchStringStorage*/
			free(currentTask.taskName);
			free(currentTask.taskInfo);



			free(working_task.taskInfo);
			exit(1);
		}
	}

	return FALSE;
}
