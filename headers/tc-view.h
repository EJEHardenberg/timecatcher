#ifndef __TC_VIEW_H__
	#define __TC_VIEW_H__

	#include <stdlib.h>
	#include <stdio.h>
	#include "tc-task.h"

	void tc_view(int argc, char const *argv[]);

	/*These are 'private' methods called by the subroutine of tc_view */
	void _tc_displayView(struct tc_task working_task,int verbose,int finishFlag);
	void _tc_view_no_args(struct tc_task working_task);
	void _tc_view_with_args(struct tc_task working_task, int verboseFlag, int argc, char const *argv[], char * taskName);
	int _getAllTasks(struct tc_task allTasks[]);
	void _tc_task_read_byHashPath(char const * taskHash, struct tc_task * structToFill);
#endif
