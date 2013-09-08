#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "tc-init.h"
#include "tc-directory.h"
#include "tc-task.h"
#include "tc-view.h"
#include "tc-start.h"
#include "tc-info.h"
#include "tc-finish.h"

int main(int argc, char const *argv[]) {	
	/* Determine what we've been asked to do */
	if ( argc <= 1 ) {
		/* Called with no arguments. Display Usage */
		_tc_display_usage(NULL);

	}else if ( argc == 2 ){
		/* Called with just a command, beworking_tasksides view let it go to usage*/
		if ( strcasecmp( argv[1], TC_VIEW_COMMAND) == 0 ) {
			/* View is the only command (so far) that renders w/ no arguments */
			tc_view(argc,argv);
		} else {
			_tc_display_usage(argv[1]);
		}

	}else{/* Called with command and arguments of some kind*/

		/* Check for the help flag */
		_tc_help_check(argc,argv);

		/* No help requested try to parse the command*/
		if( strcasecmp( argv[1], TC_VIEW_COMMAND ) == 0 ){
			tc_view(argc, argv);
		}else if ( strcasecmp( argv[1], TC_START_COMMAND ) == 0 ) {
			tc_start(argc,argv);
		}else if (strcasecmp ( argv[1], TC_ADD_INFO_COMMAND ) == 0 )
			tc_addInfo(argc,argv);
		else if (strcasecmp( argv[1], TC_FINISH_COMMAND ) == 0 ) {
			tc_finish(argc,argv);
		}
	}

	return FALSE;
}
