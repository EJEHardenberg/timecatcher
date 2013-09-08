#ifndef __TC_TASK_H__
	#define __TC_TASK_H__
	#define TC_TASK_NOT_FOUND 2
	#define TC_TASK_FOUND 4
	#define TC_TASK_STARTED 8
	#define TC_TASK_FINISHED 16
	#define TC_TASK_PAUSED 32
	

	
	#include <time.h>
	#include <openssl/sha.h>
	#include <stdio.h>
	#include <string.h>
	#include <stdlib.h>

	struct tc_task {
		time_t startTime;
		time_t pauseTime;
		time_t endTime;
		int state;
		int seqNum;
		char * taskName;
		char * taskInfo;

	};

	void _tc_task_read(char const * taskName, struct tc_task * structToFill);
	void _tc_task_write(struct tc_task structToWrite, char taskParentDirectory[]);
	void _find_current_task(struct tc_task * taskStruct);
	char * _tc_stateToString(int state);
	void _tc_taskName_to_Hash(char * taskName, char  * fileHashName);
	void _resolve_taskName_from_args(int argc, char const *argv[],char * taskName);
	char *trim(char *str);

	#ifndef TRUE
		#define TRUE 1
	#endif
	#ifndef FALSE
		#define FALSE 0
	#endif

#endif