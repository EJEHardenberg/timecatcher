#include "tc-task.h"
#include "tc-view.h"
#include "tc-directory.h"


void _tc_displayView(struct tc_task working_task,int verbose){
	int hoursWorked,secondsWorked,daysWorked,minutesWorked;
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
	"Total Time Worked: \t%i Days %i Hours %i Minutes and %i Seconds\n"
	"Last Time Updated: \t%s\n"
	"Task State: \t\t%s\n\n";

	/*Because endTime can be 0 and will be most of the time, take abs:*/
	daysWorked = abs(working_task.endTime - working_task.startTime - working_task.pauseTime)/86400;
	hoursWorked = (abs(working_task.endTime - working_task.startTime - working_task.pauseTime)-(daysWorked*86400))/3600;
	minutesWorked = (abs(working_task.endTime - working_task.startTime - working_task.pauseTime) - (hoursWorked*3600) - (daysWorked*86400))/60;
	secondsWorked = abs(working_task.endTime - working_task.startTime - working_task.pauseTime) - (minutesWorked*60) - (hoursWorked*3600) - (daysWorked*86400);

	strftime(taskStartedText,TC_MAX_BUFF/2,"%c",localtime(&working_task.startTime));

	if(working_task.state == TC_TASK_STARTED ) {
		/* The task is still in progress so: */
		working_task.endTime = time(0);
	}

	strftime(taskEndedText,TC_MAX_BUFF/2,"%c",localtime(&working_task.endTime));

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