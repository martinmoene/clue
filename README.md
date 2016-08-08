clue &ndash; Collect and learn from uncovering evidence
=========================================================

A tiny header-only C++ logging framework (_in alpha stage_)

If your logging requirements are modest or you like to begin _light_, then `clue` may be for you.

`clue` is a C++03 header-only library to log messages with a severity and optional module identifier. Provided logging destinations are the console, the Windows debugger, the Windows event log, an in-memory log (string) and Unix syslog. You can also define your own logging destination. `clue` is based on an idea by Mark Nelson, presented in DrDobbs [1] and on ideas found in the CATCH test framework by Phil Nash [2].

**Contents**  
- [Example usage](#example-usage)
- [Compile and run](#compile-and-run)
- [Synopsis](#synopsis)
- [Dependencies](#dependencies)
- [Reported to work with](#reported-to-work-with)
- [Other logging libraries](#other-logging-libraries)
- [Notes and References](#notes-and-references)

<!--- [Variants of lest](#variants-of-lest) -->


Example usage
-------------
```
#include "clue.hpp"
int main()
{
    LOG_EMERGENCY( "design by contract violation: "           << "irrecoverable, terminating..." );
    LOG_ALERT    ( "practically-unrecoverable condition: "    << "need more memory; trying hard, likely failing..." );
    LOG_CRITICAL ( "normative behaviour cannot be achieved: " << "severe error" );
    LOG_ERROR    ( "normative behaviour cannot be achieved: " << "error" );
    LOG_WARNING  ( "you should be aware of: "                 << "e.g. disk 90% full, please free some" );
    LOG_NOTICE   ( "operating normal: "                       << "database connection achieved" );
    LOG_INFO     ( "monitoring health of the system: "        << "tracking more actions, not significantly degrading performance" );
    LOG_DEBUG    ( "tracking detailed information: "          << "speed: " << 3.14 << " m/s" );
}

```
The terminology in above messages is taken from the article by Matthew Wilson [Choosing severity levels](http://blog.pantheios.org/2010/10/choosing-severity-levels.html) on the blog of the Pantheios logging framework [3]. A description of these terms can be found in [4].


Compile and run
---------------
```
prompt> g++ -Wall -o example1.exe example1.cpp && example1
2014-03-14T23:01:33 Emergency: design by contract violation: irrecoverable, terminating...
2014-03-14T23:01:33     Alert: practically-unrecoverable condition: need more memory; trying hard, likely failing...
2014-03-14T23:01:33  Critical: normative behaviour cannot be achieved: severe error
2014-03-14T23:01:33     Error: normative behaviour cannot be achieved: error
2014-03-14T23:01:33   Warning: you should be aware of: e.g. disk 90% full, please free some
2014-03-14T23:01:33    Notice: operating normal: database connection achieved
2014-03-14T23:01:33      Info: monitoring health of the system: tracking more actions, not significantly degrading performance
2014-03-14T23:01:33     Debug: tracking detailed information: speed: 3.14 m/s
```


Synopsis
--------
### Unique macro names
-D<b>clue_NO_SHORT_NAMES</b>  
All macros of `clue` start with `clue_` to hopefully make them unique. Note however, that at default also macros without the leading `clue_` are available. Define `clue_NO_SHORT_NAMES` to omit the short variants.

### Logging macros
`clue` defines the following logging macros (see note below).  
**LOG_EMERGENCY(** _expr_ **)**  
**LOG_ALERT    (** _expr_ **)**  
**LOG_CRITICAL (** _expr_ **)**  
**LOG_ERROR    (** _expr_ **)**  
**LOG_WARNING  (** _expr_ **)**  
**LOG_NOTICE   (** _expr_ **)**  
**LOG_INFO     (** _expr_ **)**  
**LOG_DEBUG    (** _expr_ **)**  

Use these like `LOG_NOTICE( "Gear switched to: " << gear );`

Note: if `clue_LOG_TO_SYSLOG` or `LOG_TO_SYSLOG` is defined, the above short variants are not available as these would clash with the syslog severities.
          
### Log severities
`clue` defines the following severities.  
**LOG_SEV_NONE**   
**LOG_SEV_EMERGENCY**  
**LOG_SEV_ALERT**  
**LOG_SEV_CRITICAL**  
**LOG_SEV_ERROR**  
**LOG_SEV_WARNING**  
**LOG_SEV_NOTICE**  
**LOG_SEV_INFO**  
**LOG_SEV_DEBUG**  
**LOG_SEV_MAX**  

See also section _Logging level_ below.

### Loggging level
-D<b>LOG_LEVEL</b>=LOG_SEV_NOTICE  
Note: work in progress. Define the logging statements that are active via `LOG_LEVEL` before inclusion of `clue.hpp`. The idea is to make this runtime configurable.

-D<b>LOG_LEVEL_BUILD</b>=LOG_SEV_DEBUG  
Define which logging statements will be included in the code via `LOG_LEVEL_BUILD` before inclusion of `clue.hpp`. If you do not define `LOG_LEVEL_BUILD`, all logging statements are included. See also section Other macros, `clue_OMIT_UNUSED_LOG_EXPRESSIONS`

### Module name
-D<b>LOG_MODULE_NAME</b>=*name*  
Log messages may include a _module_ or _feature_ identifier right after the severity. You add such an identifer by defining `LOG_MODULE_NAME` before inclusion of `clue.hpp`. For example, a .cpp file may contain:

	#define LOG_MODULE_NAME "Gearbox"
	#include "clue.hpp"
	
	int main()
	{
	    const int gear = 5;
	    LOG_NOTICE( "Gear switched to: " << gear );
	}

Compile and run:

	prompt>g++ -Wall -o example2.exe example2.cpp && example2
	2014-03-15T17:29:11    Notice: Gearbox: Gear switched to: 5

### Logging destination control
-D<b>LOG\_TO\_...</b>  
To select a specific logging destination, define one of the following before inclusion of `clue.hpp`.

-D<b>LOG_TO_CONSOLE</b>  
Log to `std::clog`.

-D<b>LOG_TO_DEBUGGER</b>  
On Windows, log via `OutputDebugString()`. On Unix: TBD.
 
-D<b>LOG_TO_EVENTLOG</b>  
Windows only. Log via `ReportEvent()`. See source code in `to_eventlog_severity()` for mapping from clue (syslog) severity to event log severity. Note: you must link to `Advapi32.lib`.

-D<b>LOG_TO_STRING</b>    
Log to memory. This makes function `strlog & the_log()` available in namespace clue. Type `strlog` provides:
- void **clear()** - reset the severity to clue_LOG_SEV_NONE and clear text (as after construction),
- int **severity()** - the latest logged severity,
- std::string **text()** - the logged text since the latest clear().

Note: it's not safe to use `the_log()` from different threads.  
See also: [Define your own string logging object](#own_string_log_object).

-D<b>LOG_TO_SYSLOG</b>    
NTS:To be verified (Unix/Windows). 
See also syslog(3) [5].

If none of these is defined and you didn't define your own back-end (see below), `clue` will select `console` for non-GUI builds, `debugger` for Windows GUI build (`_WINDOWS` is defined), and `syslog` on Unix (TBD).

### Define your own back-end or logging destination
**clue_LOG_EXPRESSION(** *severity*, *expr* **)**  
`clue` allows to specify a back-end to log to a destination  of your choice. You do this by defining `clue_LOG_EXPRESSION` before inclusion of `clue.hpp`. For example:

	#define clue_LOG_EXPRESSION( severity, expr ) \
	    std::cout << clue::to_severity_text(severity) << ": " << expr
	
	#include "clue.hpp"
	
	int main()
	{
	    clue_LOG_NOTICE( "Hello" << " world" );
	}

Compile and run:

	prompt> g++ -Wall -o example3.exe example3.cpp && example3
	Notice: Hello world

<a name="own_string_log_object"></a>
### Define your own string logging object
**clue_LOG_STRING_EXPRESSION(** *log*, *severity*, *expr* **)**  
`clue` allows to specify your own string logging object. You do this by defining `clue_LOG_EXPRESSION`  in terms of `clue_LOG_STRING_EXPRESSION` before inclusion of `clue.hpp`. For example:

	#define clue_LOG_TO_STRING
	#define clue_LOG_EXPRESSION( severity, expr ) \
	    clue_LOG_STRING_EXPRESSION( my_log, severity, expr )
	
	#include "clue.hpp"
	#include <iostream>
		
	int main()
	{
	    clue::strlog my_log;
	    
	    clue_LOG_NOTICE( "Hello" << " world" );
	    
	    std::cout << "my_log.text(): " << my_log.text() << "\n";
	}

Compile and run:

	prompt> g++ -Wall -Wextra -Weffc++ -I.. -o example3a.exe example3a.cpp && example3a
	my_log.text(): Hello world

### Other Macros
-D<b>clue_NO_TIMESTAMP</b>  
Define this to omit the timestamp from the logged messages.

-D<b>clue_OMIT_UNUSED_LOG_EXPRESSIONS</b>  
Define this to omit the logging expressions with a severity lower than `LOG_LEVEL_BUILD`. The idea is that code that is inactivated by the chosen build log level normally is included in the code and compiled, but removed in the optimisation phase. For compilers that are not capable to do this, you may define `LOG_OMIT_UNUSED_LOG_EXPRESSIONS` to simplify these expressions to the equivalent of `do {} while(false)`.

**LOG_LOGGED_SEVERITIES()**  
Call this macro to issue a log message with severity LOG_SEV_NONE that enumerates the severities that are included in the build. For example as "[clue]: Emergency, Alert, Critical, Error, Warning, Notice, Info, Debug." (time omitted).

**LOG_EXPRESSION(** *severity*, *expr* **)**  
You can use it like `LOG_EXPRESSION( LOG_NOTICE, "Hello" << " world" )`.

### Namespace
namespace **clue** { }  
Types and functions are located in namespace clue.

### Utility functions
`clue` uses several functions that may also be useful to you.

#### General
std::string **text_or(** std::string const & *text*, std::string const & *or_text* **)**;  
Return *text* if non-empty, otherwise return *or_text*.

std::string **text_with_or(** std::string const & *prefix*, std::string const & *text*, std::string const & *postfix*, std::string const & *or_text* **)**;  
Return *text* enclosed in *prefix* and *postfix* if *text* is non-empty, otherwise return *or_text*.

std::string **now_text()**;  
Return date and time as "*yyyy-mm-dd*&zwj;T&zwj;*hh:mm:ss*" [6], or empty string if `clue_NO_TIMESTAMP` is defined. 

std::string **to_module_text(** std::string const & *module* **)**;  
Return ": _module_" or empty string if module itself is empty.

std::string **to_severity_text(** int const *severity* **)**;    
Return a string such as "Emergency", "Notice", etc. for the given severity.

std::string **to_severities_text(** int const *level*, std::string const & *postfix* = "." **)**;  
Return a string enumerating the logged severities. For example `to_severities_text( LOG_SEV_CRITICAL );` yields "Emergency, Alert, Critical.".

#### When LOG_EVENTLOG is defined
int **to_eventlog_severity(** int *severity* **)**  
Return the eventlog severity for the given `clue` severity. Note that this isn't a one-to-one mapping.

#### When LOG_SYSLOG is defined
int **to_syslog_severity(** int *severity* **)**;  
Return the syslog severity for the given `clue` severity.


Dependencies
------------
`clue` depends on the C++ standard library. For logging to the Windows eventlog it requires access to OutputDebugString(). For logging to the Unix system logger it requires openlog(), syslog() and closelog() declared in \<syslog.h\>.


Reported to work with
---------------------
`clue` should work with any C++03 compiler. It has been reported to work with the following compilers:
- g++ 4.6.2, g++ 4.6.3, g++ 4.7.2, g++ 4.8.1
- Visual C++ 6 (Visual Studio 6), VC10 (VS2010), VC11 (VS2012)


<!--
Variants of clue
----------------
-->

Other logging libraries
-----------------------
- [spdlog](https://github.com/gabime/spdlog) - Super fast C++ logging library.
- [mlog](https://github.com/zschoche/mlog) - Comfortable lightweight C++ logging library -- cross-platform, C++11.
- [Boost.Log](http://www.boost.org/doc/libs/1_55_0/libs/log/doc/html/) - C++ logging library -- simplicity, extensibility, performance.
- [Pantheios](http://www.pantheios.org/) - Pantheios is a C/C++ Diagnostic Logging API library, offering type-safety, efficiency, genericity and extensibility.


Notes and References
--------------------

[1] Mark Nelson [Blundering into the One Definition Rule](http://www.drdobbs.com/cpp/blundering-into-the-one-definition-rule/240166489). DrDobbs. 11 March 2014.

[2] Phil Nash. [CATCH, an automated test framework for C, C++ and Objective-C](http://builds.catch-lib.net/).

[3] Matthew Wilson. Pantheios Tips 'n' Tricks. [Choosing severity levels](http://blog.pantheios.org/2010/10/choosing-severity-levels.html). 30 October 2010.

[4] Matthew Wilson. [Quality Matters: The Worst Form of 'Error' Handling Except For All The Others](http://accu.org/index.php/journals/1681), section *A new vocabulary*. Overload, 18(98):28-32, August 2010.

[5] [syslog(3)](http://man7.org/linux/man-pages/man3/syslog.3.html)

[6] [ISO 8601](http://en.wikipedia.org/wiki/ISO_8601) format, but without timezone.

[![Build Status](https://travis-ci.org/martinmoene/clue.png?branch=master)](https://travis-ci.org/martinmoene/clue)
