// cclue changes (c) 2020 by dbj@dbj.org -- https://dbj.org/license_dbj/

// choice of targets

// default if none is selected
// note: redirect std::cerr to file if required
// #define CLUE_LOG_TO_CONSOLE

// note: notice how visual studio is colouring the output
// #define CLUE_LOG_TO_DEBUGGER
 
// roadmap: event and category id to be used ? 
// #define CLUE_LOG_TO_EVENTLOG 

// #define CLUE_LOG_TO_STRING

// note: in here is a win syslog also used but just for windows builds
#define CLUE_LOG_TO_SYSLOG

// optional
// otherwise it is empty string
// #define CLUE_LOG_MODULE_NAME __FILE__

#include "../cclue.h"

int main(int argc, char** argv)
{
	CLUE_LOG_EMERGENCY << "design by contract violation: " << "irrecoverable, terminating...";
	CLUE_LOG_ALERT << "practically-unrecoverable condition: " << "need more memory; trying hard, likely failing...";
	CLUE_LOG_CRITICAL << "normative behaviour cannot be achieved: " << "severe error";
	CLUE_LOG_ERROR << "normative behaviour cannot be achieved! ";
	CLUE_LOG_WARNING << "you should be aware of: " << "e.g. disk 90% full, please free some";
	CLUE_LOG_NOTICE << "operating normal: " << "database connection achieved";

#if defined(__clang__) || defined(__GNUC__)
	CLUE_LOG_INFO << __VERSION__;
#endif

#ifdef	_MSC_FULL_VER
	CLUE_LOG_INFO << "_MSC_FULL_VER : " <<  _MSC_FULL_VER;
#endif // _MSC_FULL_VER

	CLUE_LOG_DEBUG << "tracking detailed information: " << "speed: " << 3.14 << " m/s";

	printf("\n%s\n", CLUE_STRING_LOG_TEXT); // empty string if no string logging
	return EXIT_SUCCESS;
}

/* EOF */