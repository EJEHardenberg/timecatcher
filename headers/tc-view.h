#ifndef __TC_VIEW_H__
	#define __TC_VIEW_H__

	#include <stdlib.h>
	#include <stdio.h>

	void tc_view(int argc, char const *argv[]);
	void _tc_displayView(struct tc_task working_task,int verbose);
	void _tc_view_no_args(struct tc_task working_task);
	void _tc_view_with_args(struct tc_task working_task, int verboseFlag, int argc, char const *argv[], char * taskName);

#endif