clue &ndash; Collect and learn from uncovering evidence
=========================================================

A tiny header-only C++ logging framework (_in alpha stage_)

If your logging requirements are modest or you like to begin _light_, then `clue` may be for you.

`clue` is a C++03 header-only library to log messages with a severity and optional module identifier. Provided logging destinations are the console, the Windows debugger, the Windows event log and Unix syslog. You can also define your own logging destination. `clue` is based on an idea by Mark Nelson, presented in DrDobbs [1].

**Contents**  
- [Example usage](#example-usage)
- [Compile and run](#compile-and-run)
- [Synopsis](#synopsis)
- [Reported to work with](#reported-to-work-with)
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
The terminology in above messages is taken from the article by Matthew Wilson [Choosing severity levels](http://blog.pantheios.org/2010/10/choosing-severity-levels.html) on the blog of the Pantheios logging framework [2]. A description of these terms can be found in [3].

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
`clue` defines the following logging macros.  
**LOG_EMERGENCY(** _expr_ **)**  
**LOG_ALERT    (** _expr_ **)**  
**LOG_CRITICAL (** _expr_ **)**  
**LOG_ERROR    (** _expr_ **)**  
**LOG_WARNING  (** _expr_ **)**  
**LOG_NOTICE   (** _expr_ **)**  
**LOG_INFO     (** _expr_ **)**  
**LOG_DEBUG    (** _expr_ **)**  
Use these like `LOG_NOTICE( "Gear switched to: " << gear );`
          
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
-D<b>LOG_LEVEL</b>=LOG_SEV_DEBUG  
Define which logging statements will be included in the code via `LOG_LEVEL` before inclusion of `clue.hpp`.

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

-D<b>LOG_TO_SYSLOG</b>    
NTS:To be verified (Unix/Windows). 

If none of these is defined and you didn't define your own backend (see below), `clue` will select `console` for non-GUI builds, `debugger` for Windows GUI build (`_WINDOWS` is defined), and `syslog` on Unix (TBD).

### Define your own backend or logging destination
**clue_LOG_EXPRESSION(** *severity*, *expr* **)**  
`clue` allows to specify a backend to log to a destionation  of your choice. You do this by defining `clue_LOG_EXPRESSION` before inclusion of `clue.hpp`. For example:

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

### Other Macros
-D<b>clue_NO_TIME</b>  
Define this to omit the time from the loggedd message.

**LOG_LOGGED_LEVELS()**  
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
Return date and time as "*yyyy-mm-dd*&zwj;T&zwj;*hh:mm:ss*" [4], or empty string if `clue_NO_TIME` is defined. 

std::string **to_module_text(** std::string const & *module* **)**;  
Return ": _module_" or empty string if module itself is empty.

std::string **to_severity_text(** int const *severity* **)**;    
Return a string such as "Emergency", "Notice", etc. for the given severity.

std::string **to_severities_text(** int const *level*, std::string const *postfix* = "." **)**;  
Return a string enumerating the logged severities. For example `to_severities_text( LOG_SEV_CRITICAL );` yields "Emergency, Alert, Critical.".

#### When LOG_EVENTLOG is defined
int **to_eventlog_severity(** int *severity* **)**  
Return the eventlog severity for the given `clue` severity. Note that this isn't a one-to-one mapping.

#### When LOG_SYSLOG is defined
int **to_syslog_severity(** int *severity* **)**;  
Return the syslog severity for the given `clue` severity.


Reported to work with
---------------------
`clue` should work with any C++03 compiler. It has been reported to work with the following compilers:
- g++ 4.8.1
- Visual Studio 6, 2010, 2012


<!--
Variants of clue
----------------
-->

Notes and References
--------------------

[1] Mark Nelson [Blundering into the One Definition Rule](http://www.drdobbs.com/cpp/blundering-into-the-one-definition-rule/240166489). DrDobbs. 11 March 2014.

[2] Matthew Wilson. Pantheios Tips 'n' Tricks. [Choosing severity levels](http://blog.pantheios.org/2010/10/choosing-severity-levels.html). 30 October 2010.

[3] Matthew Wilson. [Quality Matters: The Worst Form of 'Error' Handling Except For All The Others](http://accu.org/index.php/journals/1681), section *A new vocabulary*. Overload, 18(98):28-32, August 2010.

[4] [ISO 8601](http://en.wikipedia.org/wiki/ISO_8601) format, but without timezone.
