#include "tc-directory.h"
#include "tc-view.h"
#include "tc-task.h"
#include "tc-init.h"

#include <stdio.h>

void tc_finish(int argc, char const * argv[]){
	struct tc_task working_task;
	struct tc_task currentTask; 
	char taskName[TC_MAX_BUFF];
	char tcHomeDirectory[TC_MAX_BUFF];
	char currentTaskPath[TC_MAX_BUFF];
	time_t rawtime;

	tc_init(tcHomeDirectory);

	working_task.taskName = malloc(TC_MAX_BUFF*sizeof(char));
	working_task.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));

	_resolve_taskName_from_args(argc,argv,taskName);

	/* Check if there's a task */
	_tc_task_read(taskName, &working_task);

	if(working_task.state == TC_TASK_NOT_FOUND)
		fprintf(stderr, "%s\n", "Could not find the task to finish");
	else if(working_task.state == TC_TASK_FINISHED)
		fprintf(stderr, "%s\n", "Task is already finished. To resume use the start -s command");
	else{
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
		_tc_getCurrentTaskPath(currentTaskPath);

		/* Save the task */
		_tc_task_write(working_task, tcHomeDirectory);
		_tc_view_no_args(working_task);

		/* If this task was the same as the current task, remove the current file */
		if(strcmp(currentTask.taskName,working_task.taskName)==0)
			remove(currentTaskPath); 

		free(currentTask.taskName);
		free(currentTask.taskInfo);
	}


	free(working_task.taskInfo);
	free(working_task.taskName);
}