#include "tc-info.h"
#include "tc-task.h"
#include "tc-init.h"
#include "tc-directory.h"

#include <stdio.h>

void tc_addInfo(int argc, char const *argv[]){
	char tcHomeDirectory[TC_MAX_BUFF];
	struct tc_task working_task;
	char taskInfo[TC_MAX_BUFF];
	taskInfo[0] = '\0';

	tc_init(tcHomeDirectory);
	
	working_task.taskName = malloc(TC_MAX_BUFF*sizeof(char));
	working_task.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));
	
	_find_current_task(&working_task);
	if(working_task.state == TC_TASK_NOT_FOUND){
			fprintf(stderr, "%s\n", "No current task to add information to.");
	}else{
		/* Open up the info file for writing */
		if(_tc_file_exists(working_task.taskInfo) == FALSE)
			fprintf(stderr, "%s\n", "Could not find information file for current task.");
		else {/* Add information to the task */

			/* The cli is the task information, so grab it like a name */
			_resolve_taskName_from_args(argc,argv,taskInfo);

			strcpy(working_task.taskInfo,taskInfo); 
			_tc_task_write(working_task,tcHomeDirectory);

			fprintf(stdout, "%s\n", "Wrote information to current task.");
		}
		
	}
	
	free(working_task.taskName);
	free(working_task.taskInfo);

}