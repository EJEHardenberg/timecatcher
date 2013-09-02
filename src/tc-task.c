#include "tc-task.h"
#include "tc-directory.h"

void _find_current_task(struct tc_task * taskStruct){
	/*Returns an error code within the taskStruct to determine success or not*/
	const char * home;
	char currentTaskPath[TC_MAX_BUFF];

	home = _tc_getHomePath();
	sprintf(currentTaskPath,"%s/%s",home,TC_CURRENT_TASK);

	/* Check if the file exists or not */
	if( _tc_file_exists(currentTaskPath) ){
		/* If the file exists we should return information about it */
		taskStruct->state = TC_TASK_FOUND;
	}else{
		/* Return an error flag that there is no current task */
		taskStruct->state = TC_TASK_NOT_FOUND;
	}

}

/*void _tc_task_read(char const * taskName, struct tc_task structToFill){ */
	/* Attempt to fill the structure with data from the file */

	/* Scan the current directory for the task, if not found then look 
	 * into previous directories (get them in order and look backwards through 
	 * them)
	*/

/*}*/

void _tc_taskName_to_Hash(char * taskName, char  * fileHashName){
	unsigned char hash[SHA_DIGEST_LENGTH];
	char tempHashName[TC_MAX_BUFF];
	int looper;

	SHA1((const unsigned char*)taskName,strlen(taskName),hash);
	tempHashName[0] = '\0';
	for(looper = 0; looper < 20; ++looper)
		sprintf(tempHashName,"%s%02x",tempHashName,hash[looper]);
	tempHashName[looper] = '\0';
	
	strcpy(fileHashName,tempHashName);
}

void _tc_task_write(struct tc_task structToWrite, char taskParentDirectory[]){
	/* Write the task out in a useful format */

	/* The format of a task file name is as follows:
		<taskName sha-1>.seq and <taskName sha-1>.info

		The seq file format is as follows:
		<seq num> <state> <epoch time> [info hash for future]
		<seq num> <state> <epoch time> [info hash for future]
		<seq num> <state> <epoch time> [info hash for future]

		The info file is simply:
		Task Name \n
		[Raw text added through add-info]
		-- ideas for later implementation use info hash for future as info file
		name and then store individual info added to task at each sequence --
	*/
	char * fileHashName; /* hash is 20 characters .seq is 4 more \0 is 1 more*/
	fileHashName = malloc(25*sizeof(char));
	if( fileHashName == NULL ){
		fprintf(stderr, "%s\n", "Could not allocate memory for file hash string.");
		exit(1);
	}
	fileHashName[0] = '\0';

	printf("Working in: %s\n",taskParentDirectory );
	
	_tc_taskName_to_Hash(structToWrite.taskName,fileHashName);
	
	sprintf(fileHashName,"%s.seq",fileHashName);

	printf("%s\n", fileHashName);

	free(fileHashName);
	
}