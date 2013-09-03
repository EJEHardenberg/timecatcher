#ifndef __TC_INIT_H__
	#define __TC_INIT_H__


	const char * tc_init(char taskParentDirectory[]);
	void _tc_display_usage(const char * command);
	void _tc_help_check(int argc, char const *argv[]);
	int _tc_args_flag_check(int argc, char const *argv[], char const * longFlag, char const * shortFlag);

	#define TC_VIEW_COMMAND "view"
	#define TC_HELP_LONG "--help"
	#define TC_HELP_SHORT "-h"
	#define TC_VIEW_ALL_SHORT "-a"
	#define TC_VIEW_ALL_LONG "--all"
	#define TC_HELP_COMMAND "help"
	#define TC_VERBOSE_SHORT "-v"
	#define TC_VERBOSE_LONG "--verbose"
	#define TC_ADD_INFO_COMMAND "add-info"
	#define TC_START_COMMAND "start"
	#define TC_FINISH_COMMAND "finish"
	#define TC_SWITCH_LONG "--switch"
	#define TC_SWITCH_SHORT "-s"
	#ifndef TRUE
		#define TRUE 1
	#endif
	#ifndef FALSE
		#define FALSE 0
	#endif

#endif