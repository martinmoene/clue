// example1.cpp

// -DLOG_TO_CONSOLE
// -DLOG_TO_DEBUGGER
// -DLOG_TO_EVENTLOG
// -DLOG_TO_SYSLOG
// -DLOG_LEVEL=LOG_SEV_NOTICE
// -DLOG_LEVEL_BUILD=LOG_SEV_INFO

//#define LOG_MODULE_NAME "Module"

#include "clue.hpp"

int main()
{
    // -DLOG_TO_SYSLOG requires use of clue_ prefix

    clue_LOG_LOGGED_SEVERITIES();

    clue_LOG_EXPRESSION( clue_LOG_SEV_NONE, "The log levels currently turned on are..." );
    clue_LOG_EMERGENCY ( "design by contract violation: "           << "irrecoverable, terminating..." );
    clue_LOG_ALERT     ( "practically-unrecoverable condition: "    << "need more memory; trying hard, likely failing..." );
    clue_LOG_CRITICAL  ( "normative behaviour cannot be achieved: " << "severe error" );
    clue_LOG_ERROR     ( "normative behaviour cannot be achieved: " << "error" );
    clue_LOG_WARNING   ( "you should be aware of: "                 << "e.g. disk 90% full, please free some" );
    clue_LOG_NOTICE    ( "operating normal: "                       << "database connection achieved" );
    clue_LOG_INFO      ( "monitoring health of the system: "        << "tracking more actions, not significantly degrading performance" );
    clue_LOG_DEBUG     ( "tracking detailed information: "          << "speed: " << 3.14 << " m/s" );

    clue_LOG_EXPRESSION( clue_LOG_SEV_NONE, "see Pantheios: The C++ Logging Library SweetSpot, http://www.pantheios.org/" );
}

// g++ -Wall -Wextra -Weffc++ -I.. -o example1.exe example1.cpp && example1

// cl -nologo -W4 -EHsc -I.. example1.cpp && example1
// cl -nologo -W4 -EHsc -I.. -D_WINDOWS example1.cpp && example1
// cl -nologo -W4 -EHsc -I.. -DLOG_TO_EVENTLOG example1.cpp Advapi32.lib && example1
                        
// cl -nologo -W4 -EHsc -I.. -DLOG_LEVEL_BUILD=3 example1.cpp && example1
// cl -nologo -W4 -EHsc -I.. -DLOG_LEVEL_BUILD=LOG_SEV_NOTICE example1.cpp && example1
// cl -nologo -W4 -EHsc -I.. -DLOG_LEVEL_BUILD=LOG_SEV_NOTICE -DLOG_TO_DEBUGGER example1.cpp && example1
