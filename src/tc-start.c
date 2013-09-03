#include "tc-start.h"
#include "tc-task.h"
#include "tc-directory.h"

#include <time.h>
#include <stdio.h>

void _tc_start(struct tc_task working_task, char * taskName, char * tcHomeDirectory ){
	time_t rawtime;
	/* Check if we are already working on a task */
	_find_current_task(&working_task);
	if(working_task.state != TC_TASK_NOT_FOUND){
		/* If we're working on a task then no. finish it first or pause it */
		fprintf(stderr, "\n%s\n", "There is already a task being worked on. ");
		fprintf(stderr, "%s\n\n", "Finish the current task first or switch tasks.");
		free(working_task.taskName);
		free(working_task.taskInfo);
		exit(1);
	}

	/* Create a task and store its information */			
	free(working_task.taskName);
	working_task.taskName = taskName;
	/* If we are making a task we're starting it */
	working_task.state = TC_TASK_STARTED;
	/* Set the start time */
	rawtime = time(0); 
	if(rawtime == -1){
		fprintf(stderr, "%s\n", "Could not determine time. Exiting");
		free(working_task.taskInfo);
		exit(1);
	}
	working_task.startTime = rawtime;
	_tc_task_write(working_task, tcHomeDirectory);
	
	fprintf(stdout, "Task: %s has been started.\n", working_task.taskName);
	free(working_task.taskInfo);
	exit(1); /* Exit now because we don't want to free again */
	/* Task write sets the new task as current automatically */
}