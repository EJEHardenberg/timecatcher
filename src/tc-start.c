#include "tc-init.h"
#include "tc-start.h"
#include "tc-task.h"
#include "tc-directory.h"

#include <time.h>
#include <stdio.h>



void tc_start(int argc, char const *argv[] ){
	time_t rawtime;
	struct tc_task working_task;
	
	char taskName[TC_MAX_BUFF];
	char tcHomeDirectory[TC_MAX_BUFF];
	char switchStringStorage[TC_MAX_BUFF];

	tc_init(tcHomeDirectory);
	_resolve_taskName_from_args(argc,argv,taskName);

	working_task.taskName = malloc(TC_MAX_BUFF*sizeof(char));
	working_task.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));

	/* Check for start's switch flag */			
	if(_tc_args_flag_check(argc,argv,TC_SWITCH_LONG,TC_SWITCH_SHORT) == FALSE) 
		_tc_start(working_task, taskName, tcHomeDirectory);
	else{	
		
		/* If the current task is active (not paused) then pause it */
		_find_current_task(&working_task);
		if(working_task.state != TC_TASK_NOT_FOUND){

			/* Cant switch to the current task */
			if(strcmp(working_task.taskName,taskName)==0){
				fprintf(stderr, "%s\n", "Already working on that task. No need to switch");
			}else{			
				if( working_task.state == TC_TASK_STARTED ){
					/* Pause the task */
					working_task.state = TC_TASK_PAUSED;
					/* Set the paused time */
					rawtime = time(0); 
					if(rawtime == -1){
						fprintf(stderr, "%s\n", "Could not determine time and pause task. Exiting");
						free(working_task.taskInfo); 
						free(working_task.taskName);
						exit(1); /* This or a goto. hm.*/
					}
					working_task.pauseTime = rawtime;
					/* Wwrite the task out to the file */
					_tc_task_write(working_task,tcHomeDirectory);
				}

				/* Find the current file and remove it */
				_tc_getCurrentTaskPath(switchStringStorage);
				remove(switchStringStorage);

				/* Start the new task (with evil black magic)*/
				_recurse(taskName,argv);	
			}
		}else{
			/* No current task anyway so just start a new task */
			_tc_start(working_task, taskName, tcHomeDirectory);
		}
			
	}
	
	free(working_task.taskInfo);
	free(working_task.taskName);

}

void _recurse(char * taskName, char const * argv[]){
	char const * recurseBuff[3];

	recurseBuff[0] = argv[0];
	recurseBuff[1] = argv[1];
	recurseBuff[2] = taskName;

	main(3, recurseBuff);
}

void _tc_start(struct tc_task working_task, char * taskName, char * tcHomeDirectory ){
	time_t rawtime;
	char fileExistsCheck[TC_MAX_BUFF];
	char taskHash[TC_MAX_BUFF];
	struct tc_task existsTask;
	fileExistsCheck[0] = '\0';
	taskHash[0] = '\0';

	/* Check if we are already working on a task */
	_find_current_task(&working_task);
	if(working_task.state != TC_TASK_NOT_FOUND){
		/* If we're working on a task then no. finish it first or pause it */
		fprintf(stderr, "\n%s\n", "There is already a task being worked on. ");
		fprintf(stderr, "%s\n\n", "Finish the current task first or switch tasks.");
		return;
	}

	_tc_taskName_to_Hash(taskName,taskHash);
	sprintf(fileExistsCheck,"%s/%s/%s.seq",tcHomeDirectory,TC_TASK_DIR,taskHash);

	if(_tc_file_exists(fileExistsCheck)){
		existsTask.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));
		existsTask.taskName = malloc(TC_MAX_BUFF*sizeof(char));

		_tc_task_read(taskName,&existsTask);
		existsTask.state = TC_TASK_STARTED;
		_tc_task_write(existsTask ,tcHomeDirectory);

		fprintf(stdout, "Resuming task: %s\n",taskName);
		free(existsTask.taskName);
		free(existsTask.taskInfo);
		return;
	}

	/* Create a task and store its information */			
	strcpy(working_task.taskName,taskName);

	/* If we are making a task we're starting it */
	working_task.state = TC_TASK_STARTED;
	working_task.seqNum = 0;

	/* Set the start time */
	rawtime = time(0); 
	if(rawtime == -1){
		fprintf(stderr, "%s\n", "Could not determine time. Exiting");
		free(working_task.taskName);
		free(working_task.taskInfo);
		exit(1);
	}
	working_task.startTime = rawtime;

	/* The Task info should be nothing.*/
	(*working_task.taskInfo) = '\0';

	_tc_task_write(working_task, tcHomeDirectory);
	
	fprintf(stdout, "Task: %s has been started.\n", working_task.taskName);

}