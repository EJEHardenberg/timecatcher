#ifndef __TC_DIRECTORY_H__
	#define __TC_DIRECTORY_H__

	#define TC_MAX_BUFF 255
	#define TC_DIR_PERM 0755 
	#define TC_INDEX_DIR "indexes"
	#define TC_CURRENT_TASK "current"
	#define TC_TASK_DIR "tasks"	

	/* Prototypes */
	const char * _tc_getHomePath();
	int _tc_directoryExists(char * directoryToCheck);
	int _tc_file_exists(const char * filename);

#endif