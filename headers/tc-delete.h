#ifndef __TC_DELETE_H__
	#define __TC_DELETE_H__

	#include "tc-task.h"
	#include "tc-init.h"
	#include "tc-directory.h"
	#include <stdio.h>

	void tc_delete(int argc,const char * argv[]);
	int _tc_askForConfirm();
#endif