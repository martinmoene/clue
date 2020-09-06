
# CCLUE == CLUE 2020

> Changes (c) 2020 by dbj@dbj.org
>
> LICENSE_DBJ --  https://dbj.org/license_dbj/

## Example

Choice of targets. 

Default if none is selected.Note: redirect `std::clog` to file if required.
```cpp
#define CLUE_LOG_TO_CONSOLE
```
Here is how you can [redirect std::clog to a file](https://stackoverflow.com/a/34619037).
```cpp
#include <iostream>
#include <fstream>
// default target is console
#include  <cclue.h>
int main()
{
   std::ofstream out("cclue_log.log");
   // Original rdbuf of clog. Do reset it back before exiting.
   auto old_rdbuf = std::clog.rdbuf();
   // Set the rdbuf of clog.
   std::clog.rdbuf(out.rdbuf());
   // use cclue
   CLUE_LOG_INFO << "CCLUE, CCLUE, CCLUE.\n";
   // Reset the rdbuf of clog.
   std::clog.rdbuf(old_rdbuf);
	   return 0;
}
```
Windows Debugger Target. Note: this will not compile on LINUX or MAC. 
```cpp
#define CLUE_LOG_TO_DEBUGGER
```
Notice how visual studio is colouring the output.

Windows event log.
```cpp
#define CLUE_LOG_TO_EVENTLOG 
```
Useful when there is no console and you have your own logging option.
```cpp
#define CLUE_LOG_TO_STRING
```
> You can not select more than one target at once !

Full example. Targeting the `syslog`.
```cpp
// note: win syslog implementation is included
#define CLUE_LOG_TO_SYSLOG

// optional
// default is empty string
// #define CLUE_LOG_MODULE_NAME __FILE__

#include <cclue.h>

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
```

## This is clue. But cleaned and simplified 

| file | explanation |
|------|-------------|
| `cclue.h` | The new **clue**. Number of macros dramatically lowered. The whole file is much simplified.
| `examples/example.cpp` | Contains the only relevant test and example. Tried and tested on WIN10 PRO machines. All targets are working. In there you will find all one needs to know to be able to use this tiny library.
| `syslog-win32/` | Windows version of syslog. See the readme for the original author and license. |

## Usage

As ever, best advice is to use this as a git submodule. 

NOTE: do not include anything but `cclue.h`.

Examples folder contains a Visual Studio 2019 solution.

## Portability

Tested on WIN10 PRO. Compiled using both cl and clang.

> **NOT** tested on Mac or Linux. Please let us know in "issues" of any problems.

## Roadmap 

There is indeed some kind of "roadmap". But this lib is so small and it seems fully functional and working quite well. Any addition would be a start on the path to something different. Thus we will stop here.

To adjust **cclue** for your needs please fork and change away. Please respect the licenses.

DBJ


---

Original repository

```
// Copyright 2014 by Martin Moene
//
// clue is based on ideas by Mark Nelson, see article at
// http://www.drdobbs.com/cpp/blundering-into-the-one-definition-rule/240166489
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
```
For the original repository please proceed [here](https://github.com/martinmoene/clue).