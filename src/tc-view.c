#include "tc-task.h"
#include "tc-view.h"
#include "tc-directory.h"
#include "tc-init.h"

#include <dirent.h>

void tc_view(int argc, char const *argv[]){
	struct tc_task taskToView;
	char taskName[TC_MAX_BUFF];
	taskName[0] = '\0';

	_resolve_taskName_from_args(argc,argv,taskName);
	
	taskToView.taskName = malloc(TC_MAX_BUFF*sizeof(char));
	taskToView.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));
	if(argc == 2)
		_tc_view_no_args(taskToView);
	else
		_tc_view_with_args(
							taskToView,
							_tc_args_flag_check(argc,argv,TC_VERBOSE_LONG,TC_VERBOSE_SHORT),
							argc, argv,
							taskName
						);
	free(taskToView.taskName);
	free(taskToView.taskInfo);
}

void _tc_view_no_args(struct tc_task working_task){
	char taskName[TC_MAX_BUFF]; 
	taskName[0] = '\0';

	_find_current_task(&working_task);
	if(working_task.state == TC_TASK_NOT_FOUND){
		/* If we're working on a task then no. finish it first or pause it */
		fprintf(stderr, "\n%s\n", "No current task being worked on.");
		return;
	}
	strcpy(taskName,working_task.taskName);

	/* There is a working task so we should resolve it into information */
	_tc_task_read(taskName,&working_task);
	if(working_task.state == TC_TASK_NOT_FOUND){
		/* If we're working on a task then no. finish it first or pause it */
		fprintf(stderr, "%s\n", "Current task file exists, but could not resolve sequence file");
		return;
	}

	if( working_task.state == TC_TASK_FOUND ){
		/* Found, but not parsed correctly. */
		fprintf(stderr, "%s\n", "Current task file exists, but was corrupt");
		return;
	}

	_tc_displayView(working_task,FALSE,FALSE);
}

void _tc_view_with_args(struct tc_task working_task, int verboseFlag, int argc, char const *argv[], char * taskName){
	struct tc_task allTasks[TC_MAX_BUFF];
	int i;
	/* Check for all flag in any position*/
	if( _tc_args_flag_check(argc, argv, TC_VIEW_ALL_LONG, TC_VIEW_ALL_SHORT) == TRUE ){
		/* Show all tasks */
		
		/* Display each task */
		i=0;
		for (i = _getAllTasks(allTasks); i >= 0; --i){
			_tc_displayView(allTasks[i],verboseFlag,FALSE);
			free(allTasks[i].taskName);
			free(allTasks[i].taskInfo);
		}
		
	}else{
		if(strcmp(taskName,"")==0)
			_find_current_task(&working_task);
		else
			_tc_task_read(taskName,&working_task);
		
		if( working_task.state == TC_TASK_FOUND )
			fprintf(stderr, "%s\n", "Current task file exists, but was corrupt");
		else if( working_task.state == TC_TASK_NOT_FOUND )
			fprintf(stderr, "%s\n", "Could not find a current task to show.");
		else
			_tc_displayView(working_task,verboseFlag,FALSE);	
		
	}
}

int _getAllTasks(struct tc_task allTasks[]){
	DIR * dirPointer;
	struct dirent *dirEntry;
	char taskDir[TC_MAX_BUFF];
	char  * fileName;
	char * namePointer;
	int i;

	_tc_getTasksDir(taskDir);

	dirPointer= opendir(taskDir);
	if(dirPointer == NULL){
		fprintf(stderr, "%s\n", "Could not open task directory for file listing");
		return 0;
	}

	i=0;
	while((dirEntry = readdir(dirPointer)) != NULL && i < TC_MAX_BUFF){
		fileName = dirEntry->d_name;
		if((namePointer = strstr(fileName,".seq")) != NULL){
			/* We have a task to read about! */
			/* Get JUST the hash from the file */
			(*namePointer) = '\0'; /* Terminate the string at the end of the hash*/
			allTasks[i].taskName = malloc(TC_MAX_BUFF*sizeof(char));
			allTasks[i].taskInfo = malloc(TC_MAX_BUFF*sizeof(char));
			if(allTasks[i].taskName == NULL || allTasks[i].taskInfo == NULL)
				continue; /* If we can't get the memory then run away!*/
			allTasks[i].state = TC_TASK_NOT_FOUND;
			allTasks[i].seqNum = 0;
			_tc_task_read_byHashPath(fileName,&allTasks[i]);

			++i;
		}
	}
	closedir(dirPointer);
	return i-1;
}

/*finishFlag is to handle displaying the right state when finishing a task */
void _tc_displayView(struct tc_task working_task,int verbose, int finishFlag){
	time_t hoursWorked,secondsWorked,daysWorked,minutesWorked;
	char taskStartedText[TC_MAX_BUFF/2];
	char taskEndedText[TC_MAX_BUFF/2];
	char * shortView;
	char infoBuffer[TC_MAX_BUFF*2];
	FILE * fp_info;

	/* Display the information 
	 * (This is the short version! with a --verbose or something flag show info too.)
	*/
	shortView = ""
	"Task Name:\t\t%s\n"
	"Time Started: \t\t%s\n"
	"Total Time Worked: \t%ld Days %ld Hours %ld Minutes and %ld Seconds\n"
	"Last Time Updated: \t%s\n"
	"Task State: \t\t%s\n\n";


	
	/*Because endTime can be 0 and will be most of the time, take abs:*/
	daysWorked = abs( (time(0) - working_task.endTime)*( TC_TASK_STARTED == working_task.state ? 1 : 0) + working_task.pauseTime)/86400L;
	hoursWorked = (abs( (time(0) - working_task.endTime)*( TC_TASK_STARTED == working_task.state ? 1 : 0) + working_task.pauseTime)-(daysWorked*86400L))/3600L;
	minutesWorked = (abs( (time(0) - working_task.endTime)*( TC_TASK_STARTED == working_task.state ? 1 : 0) + working_task.pauseTime) - (hoursWorked*3600L) - (daysWorked*86400L))/60L;
	secondsWorked = abs( (time(0) - working_task.endTime)*( TC_TASK_STARTED == working_task.state ? 1 : 0) + working_task.pauseTime) - (minutesWorked*60L) - (hoursWorked*3600L) - (daysWorked*86400L);
	
	strftime(taskStartedText,TC_MAX_BUFF/2,"%c",localtime(&working_task.startTime));
	strftime(taskEndedText,TC_MAX_BUFF/2,"%c",localtime(&working_task.endTime));

	/* This is a bit of a hack, but hey if we were to have
	 * TC_TASK_FINISHED before this point then the calculation
	 * would be off. */
	if(finishFlag || finishFlag == TRUE){
		working_task.state = TC_TASK_FINISHED;
	}

	printf(shortView, working_task.taskName, taskStartedText, daysWorked,hoursWorked,minutesWorked,secondsWorked,taskEndedText,_tc_stateToString(working_task.state));
	if ( verbose == TRUE ) {
		/* Transfer file to output stream */
		fp_info = fopen(working_task.taskInfo,"r");
		if(!fp_info){ /* We rely on taskInfo being the stored path to the file */
			fprintf(stderr, "%s\n", "No information found for task. ");
			return;
		}
		fprintf(stdout, "Task Information: \n");
		/* This needs to be worked on! Should use fgets instead!*/
		while((fgets(infoBuffer,TC_MAX_BUFF*2,fp_info) != NULL)){
			fprintf(stdout, "%s", infoBuffer);
		}
		fprintf(stdout, "\n" );
		fclose(fp_info);
	}
}
