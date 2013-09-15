#include "tc-delete.h"

void tc_delete(int argc,const char * argv[]){
	struct tc_task working_task;
	struct tc_task currentTask; 
	char taskName[TC_MAX_BUFF];
	char tcHomeDirectory[TC_MAX_BUFF];
	char currentTaskPath[TC_MAX_BUFF];
	char taskSequencePath[TC_MAX_BUFF]; 
	char taskInfoPath[TC_MAX_BUFF];
	char * fileHash;
	int yaynay;
	
	tc_init(tcHomeDirectory);

	working_task.taskName = malloc(TC_MAX_BUFF*sizeof(char));
	working_task.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));

	_resolve_taskName_from_args(argc,argv,taskName);

	/* Check if there's a task */
	_tc_task_read(taskName, &working_task);

	if(working_task.state == TC_TASK_NOT_FOUND)
		fprintf(stderr, "%s\n", "Could not find the task to delete");
	else{
		/* Check the current task to see if it should be removed.*/
		currentTask.taskName = malloc(TC_MAX_BUFF*sizeof(char));
		currentTask.taskInfo = malloc(TC_MAX_BUFF*sizeof(char));

		_find_current_task(&currentTask);
		_tc_getCurrentTaskPath(currentTaskPath);

		/* If this task was the same as the current task, remove the current file */
		if( currentTask.state != TC_TASK_NOT_FOUND )
			if(strcmp(currentTask.taskName,working_task.taskName)==0)
				if (remove(currentTaskPath) == -1){
					/* Something went wrong */
					fprintf(stderr, "%s\n", "Could not remove the current task file. ");
				} 

		free(currentTask.taskName);
		free(currentTask.taskInfo);

		fileHash = malloc(25*sizeof(char)); /* hash is 20 characters .seq is 4 more \0 is 1 more */
		if( fileHash == NULL ){
			fprintf(stderr, "%s\n", "Could not allocate memory for file hash string.");
		}else{
			fileHash[0] = '\0';	
			/* Remove the task's sequence and info files*/
			_tc_taskName_to_Hash(working_task.taskName,fileHash);
	
			sprintf(taskSequencePath,"%s/%s/%s.seq",tcHomeDirectory,TC_TASK_DIR,fileHash);
			sprintf(taskInfoPath,"%s/%s/%s.info",tcHomeDirectory,TC_TASK_DIR,fileHash);

			/* Ask for confirmation */
			fprintf(stdout, "%s\n", "Deleting this task is permanent!");
			yaynay = _tc_askForConfirm();

			if( yaynay == TRUE ){ /* Remove the files */
				if(remove(taskSequencePath) == -1){
					fprintf(stderr, "%s\n", "Could not remove the sequence file for the task to be deleted.");
				}else{
					if(remove(taskInfoPath) == -1){
						fprintf(stderr, "%s\n", "Could not remove the info file for the task to be deleted.");
					}else{
						fprintf(stdout, "%s task has been removed.\n", working_task.taskName);
					}
				}
			}
		}
		free(fileHash);
	}

	free(working_task.taskInfo);
	free(working_task.taskName);
}

int _tc_askForConfirm(){
	char yn;

	fprintf(stdout, "Are you sure? [y|n]: " );
	yn = getc(stdin);
	while( yn != EOF && yn != 'y' && yn != 'n'){
		yn = getc(stdin);
		if(yn == '\n')
			fprintf(stderr, "\nEnter y or n: " );
	}

	if(yn=='y')
		return TRUE;
	if(yn=='n')
		return FALSE;

	/* throw the prompt down one*/
	fprintf(stderr, "\n");
	/* Err */
	return -1;

}