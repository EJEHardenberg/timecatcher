#include "tc-task.h"
#include "tc-directory.h"

void _find_current_task(struct tc_task * taskStruct){
	/*Returns an error code within the taskStruct to determine success or not*/
	char currentTaskPath[TC_MAX_BUFF];
	char tempBuffer[TC_MAX_BUFF];
	int storedTime;
	FILE * fp;

	_tc_getCurrentTaskPath(currentTaskPath);

	/* Check if the file exists or not */
	if( _tc_file_exists(currentTaskPath) ){
		/* the current task file is simply the name of the current task and its hash */
		fp = fopen(currentTaskPath, "r");
		if(!fp){
			fprintf(stderr, "%s\n", "Could not open current task file. Exiting");
			exit(1);
		}

		fgets(tempBuffer,TC_MAX_BUFF,fp);
		strcpy(taskStruct->taskName,tempBuffer);

		fgets(tempBuffer,TC_MAX_BUFF,fp); /* Read the hash, but ignore it. */

		/* If the file exists we should return information about it */
		fscanf(fp, "%i %i %i\n", &taskStruct->seqNum, &taskStruct->state, &storedTime);
		fclose(fp);
	}else{
		/* Return an error flag that there is no current task */
		taskStruct->state = TC_TASK_NOT_FOUND;
	}

}

void _tc_task_read(char const * taskName, struct tc_task * structToFill){ 
	/* Attempt to fill the structure with data from the file */

	char currentTaskPath[TC_MAX_BUFF];
	char taskHash[TC_MAX_BUFF];
	FILE * fp;

	_tc_getCurrentTaskPath(currentTaskPath);

	structToFill->taskName = (char *)taskName;

	if( _tc_file_exists(currentTaskPath) ){
		/* the current task file is simply the name of the current task and its hash */
		fp = fopen(currentTaskPath, "r");
		if(!fp){
			fprintf(stderr, "%s\n", "Could not open current task file. Exiting");
			exit(1);
		}

		fgets(structToFill->taskName,TC_MAX_BUFF,fp);
		fgets(taskHash, TC_MAX_BUFF, fp);

		fclose(fp);
		/* Read the sequence information for the sequence number and timing info and the last state*/


	}else{
		/* Return an error flag that there is no current task */
		structToFill->state = TC_TASK_NOT_FOUND;
	}	

	

}

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

void _tc_task_write(struct tc_task structToWrite, char tcHomeDirectory[]){
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
	char taskSequencePath[TC_MAX_BUFF]; 
	char taskInfoPath[TC_MAX_BUFF];
	char * fileHash;
	int timeToWrite;
	time_t rawtime;
	struct tm * timeinfo;
	char currentDate[TC_MAX_BUFF/2];
	char indexFilePath[TC_MAX_BUFF];
	FILE * fp;
	char currentTaskPath[TC_MAX_BUFF];

	fileHash = malloc(25*sizeof(char)); /* hash is 20 characters .seq is 4 more \0 is 1 more */
	if( fileHash == NULL ){
		fprintf(stderr, "%s\n", "Could not allocate memory for file hash string.");
		exit(1);
	}
	fileHash[0] = '\0';
	
	_tc_taskName_to_Hash(structToWrite.taskName,fileHash);
	
	sprintf(taskSequencePath,"%s/%s/%s.seq",tcHomeDirectory,TC_TASK_DIR,fileHash);
	sprintf(taskInfoPath,"%s/%s/%s.info",tcHomeDirectory,TC_TASK_DIR,fileHash);

	/* If the info file does not exist create it */
	if((_tc_file_exists(taskInfoPath)) == FALSE){
		fp = fopen(taskInfoPath, "w"); /* Note that w overwrites so no security hole here*/
		if (!fp) {    		
			fprintf(stderr,"%s\n", "Could not create task information file. Please check permissions");
			free(fileHash);
			exit(1);    		
    	} else {
    		/* Write out information to the file if there is any*/
    		fprintf(fp, "%s\n", structToWrite.taskName);
    		if(structToWrite.taskInfo != NULL)
    			fprintf(fp, "%s\n", structToWrite.taskInfo);
    		fflush(fp);
    		fclose(fp);
    	}
	}else{
		fp = fopen(taskInfoPath, "a+");
		if (!fp) {    		
			fprintf(stderr,"%s\n", "Could not create task sequence file. Please check permissions");
			free(fileHash);
			exit(1);    		
    	} else {
    		/* Write out information to the file if there is any*/
    		if(structToWrite.taskInfo != NULL)
    			fprintf(fp, "%s\n", structToWrite.taskInfo);
    		fflush(fp);
    		fclose(fp);
    	}
	}

	/*If the .seq file doesn't exist create it*/
	if((_tc_file_exists(taskSequencePath))==FALSE){
		fp = fopen(taskSequencePath, "w"); /* Note that w overwrites so no security hole here*/
		if (!fp) {
			fprintf(stderr,"%s\n", "Could not create task sequence file. Please check permissions");
			free(fileHash);
			exit(1);    		
		}
	}else{
		fp = fopen(taskSequencePath, "a+"); /* Note that w overwrites so no security hole here*/
		if (!fp) {
			fprintf(stderr,"%s\n", "Could not create task sequence file. Please check permissions");
			free(fileHash);
			exit(1);    		
		}
	}
    if(fp){
		/* Write out information to the sequence */    		
		switch( structToWrite.state ){
			case TC_TASK_PAUSED:
				timeToWrite = structToWrite.pauseTime;
				break;
			case TC_TASK_FINISHED:
				timeToWrite = structToWrite.endTime;
				break;
			case TC_TASK_STARTED:
			default:
				timeToWrite = structToWrite.startTime;
				break;

		}
		fprintf(fp, "%i %i %i\n", structToWrite.seqNum, structToWrite.state, timeToWrite);
		fflush(fp);
		fclose(fp);
    }

    /* Update the index file */
    rawtime = time(0); /*If you're starting a task at the crux of midnight then maybe this will be wrong*/
	timeinfo = localtime (&rawtime);
	strftime(currentDate,80,"%Y%m%d",timeinfo);
	sprintf(indexFilePath,"%s/%s/%s.index",tcHomeDirectory,TC_INDEX_DIR,currentDate);

	/* We don't need to check for the index file's existence because tc-init takes care of that */
	fp = fopen(indexFilePath, "a");
	if(!fp) {
		/* Weird permission problem? */
		free(fileHash);
		fprintf(stderr, "%s\n", "Could not open index file for appending. Exiting");
		exit(1);
	}

	/* Append information to the index file 
	 * This information is in the format: <task hash> <state>
	*/
	fprintf(fp, "%s %i\n", fileHash, structToWrite.state);
	fflush(fp);
	fclose(fp);

	/* If the structure was just started then it is our current task */
	_tc_getCurrentTaskPath(currentTaskPath);
	fp = fopen(currentTaskPath,"w");
	if(!fp) {
		/* Something went wrong */
		free(fileHash);
		fprintf(stderr, "%s\n", "Could not open current task file for writing. Exiting");
		exit(1);
	}
	fprintf(fp, "%s\n", structToWrite.taskName);
	fprintf(fp, "%s\n", fileHash);
	fprintf(fp, "%i %i %i\n", structToWrite.seqNum, structToWrite.state, timeToWrite);
	fflush(fp);
	fclose(fp);

	free(fileHash);
	
}