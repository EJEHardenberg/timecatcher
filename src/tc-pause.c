#include "tc-init.h"
#include "tc-pause.h"
#include "tc-task.h"
#include "tc-directory.h"

#include <time.h>
#include <stdio.h>

void tc_pause(int argc, char const *argv[] ){
	time_t rawtime;
	struct tc_task working_task;
	
	char taskName[TC_MAX_BUFF];
	char tcHomeDirectory[TC_MAX_BUFF];
	char currentTaskPath[TC_MAX_BUFF];

	tc_init(tcHomeDirectory);
	_resolve_taskName_from_args(argc,argv,taskName);

	working_task.taskName = malloc(TC_MAX_BUFF*sizeof(char));
	working_task.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));

	/* If the current task is active (not paused) then pause it */
	_find_current_task(&working_task);
	
	if(working_task.state != TC_TASK_NOT_FOUND){
			/* Cant switch to the current task */
		if(strcmp(working_task.taskName,taskName)==0){
			fprintf(stderr, "%s\n", "No current task to pause.");
		}else{			
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
		
			printf("%s\n", "The current task has been paused.");

			/* Find the current file and remove it */
			_tc_getCurrentTaskPath(currentTaskPath);
			remove(currentTaskPath);

		}			
	}else{
		fprintf(stderr, "%s\n", "No current task to pause.\n");
	}
	free(working_task.taskInfo);
	free(working_task.taskName);
}
