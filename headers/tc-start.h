#ifndef __TC_START_H__
	#define __TC_START_H__
	
	#include "tc-task.h"
	void tc_start(int argc, char const *argv[] );

	
	void _tc_start(struct tc_task working_task, char * taskName, char * tcHomeDirectory );
	void _recurse(char * taskName, char const * argv[]);


#endif
