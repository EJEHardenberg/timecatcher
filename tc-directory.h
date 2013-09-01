#ifndef __TC_DIRECTORY_H__
	#define __TC_DIRECTORY_H__

	#define TC_MAX_BUFF 255
	#define TC_DIR_PERM 0755

	/* Prototypes */
	const char * _tc_getHomePath();
	int _tc_directoryExists(char * directoryToCheck);

#endif