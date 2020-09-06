//
// 2020 version of clue (c) 2020 by dbj@dbj.org -- https://dbj.org/license_dbj/
//

//
// Copyright 2014 by Martin Moene
//
// clue is based on ideas by Mark Nelson, see article at
// http://www.drdobbs.com/cpp/blundering-into-the-one-definition-rule/240166489
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLUE_CLUE_H_INCLUDED
#define CLUE_CLUE_H_INCLUDED

#ifdef __clang__
#pragma clang system_header
#endif

#include <iomanip>
#include <sstream>
#include <cassert>

// so that some other std lib might be used
// for example EASTL
// future target is to compile in CL /kernel mode
using cpp_string = std::string;

// also for when clang is packaged with Visual Studio
#if defined( _MSC_VER ) && ! defined(__clang__)
# define CLUE_COMPILER_IS_MSVC
#endif

#ifdef CLUE_COMPILER_IS_MSVC
# pragma warning( push )
# pragma warning( disable : 4996 ) // _CRT_SECURE_NO_WARNINGS
#endif

/* default to CLUE_LOG_TO_CONSOLE */

#if !defined( CLUE_LOG_TO_CONSOLE  ) && \
    !defined( CLUE_LOG_TO_DEBUGGER ) && \
    !defined( CLUE_LOG_TO_EVENTLOG ) && \
    !defined( CLUE_LOG_TO_STRING   ) && \
    !defined( CLUE_LOG_TO_SYSLOG   )
#  define CLUE_LOG_TO_CONSOLE
#endif

#if defined( CLUE_LOG_TO_CONSOLE  ) + \
    defined( CLUE_LOG_TO_DEBUGGER ) + \
    defined( CLUE_LOG_TO_EVENTLOG ) + \
    defined( CLUE_LOG_TO_STRING   ) + \
    defined( CLUE_LOG_TO_SYSLOG   ) > 1
# error Please specify *ONLY ONE, OR NONE*,  of CLUE_LOG_TO_CONSOLE, CLUE_LOG_TO_DEBUGGER CLUE_LOG_TO_EVENTLOG, CLUE_LOG_TO_STRING and CLUE_LOG_TO_SYSLOG
#endif

#ifndef CLUE_LOG_MODULE_NAME
#define CLUE_LOG_MODULE_NAME ""
#endif

// NTS: add UNIX
#ifdef CLUE_LOG_TO_DEBUGGER
# define CLUE_LOG_TO_DEBUGGER_WINDOWS
#endif

#ifdef CLUE_LOG_TO_DEBUGGER_WINDOWS
#ifndef _WIN32
# error log to debugger works only under Windows 
#endif
#endif

#ifdef CLUE_LOG_TO_DEBUGGER_WINDOWS
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI
#define NOKERNEL
# include <windows.h>
#endif

#ifdef CLUE_LOG_TO_EVENTLOG
#ifndef _WIN32
# error log to debugger works only under Windows 
#endif
#endif

#ifdef CLUE_LOG_TO_EVENTLOG
#define STRICT 1
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI
#define NOKERNEL
# include <windows.h>
#endif


#define CLUE_TITLE "[CLUE]"
#define CLUE_VERSION CLUE_TITLE " [" __TIMESTAMP__ "] "

namespace clue
{
	constexpr auto CLUE_DELIMITER = ":";

	enum class  SEVERITY {
		NONE = -1,
		EMERGENCY = 0,
		ALERT = 1,
		CRITICAL = 2,
		ERRORR = 3, /* ERROR is a name clash with syslog.h */
		WARNING = 4,
		NOTICE = 5,
		INFO = 6,
		DEBUG = 7
	};

	constexpr auto clue_log_level = SEVERITY::DEBUG;

	constexpr auto clue_log_level_build = SEVERITY::DEBUG;

	constexpr auto clue_log_prefix_width = sizeof("Emergency"); // the longest 

	constexpr inline bool is_true(bool const on) noexcept { return on; }

	inline auto clue_is_active_build = [](SEVERITY severity)  constexpr -> bool {
		return (severity <= clue_log_level_build);
	};

	inline auto clue_is_active = [](SEVERITY severity) constexpr -> bool {
		return (clue_is_active_build(severity));
	};

	inline cpp_string text_or(cpp_string const& text, cpp_string const& or_text) noexcept
	{
		return text.length() ? text : or_text;
	}

	inline cpp_string text_with_or(cpp_string const& prefix, cpp_string const& text, cpp_string const& postfix, cpp_string const& or_text) noexcept
	{
		return text.length() ? prefix + text + postfix : or_text;
	}

	inline cpp_string to_module_text(cpp_string const& module) noexcept
	{
		return text_with_or(": ", module, "", "");
	}

	inline cpp_string to_severity_text(SEVERITY severity) noexcept
	{
		switch (severity) {
		case 		SEVERITY::NONE:  return CLUE_TITLE ;
		case 		SEVERITY::EMERGENCY:  return "EMERGENCY";
		case 		SEVERITY::ALERT:  return "ALERT";
		case 		SEVERITY::CRITICAL:  return "CRITICAL";
		case 		SEVERITY::ERRORR:  return "ERROR";
		case 		SEVERITY::WARNING:  return "WARNING";
		case 		SEVERITY::NOTICE:  return "NOTICE";
		case 		SEVERITY::INFO:  return "INFO";
		case 		SEVERITY::DEBUG:  return "DEBUG";
		default: return "UNKNOWN";
		};
	}


} // namespace clue

#ifdef CLUE_NO_TIMESTAMP

namespace clue {
	inline cpp_string now_text() { return ""; }
}

#else // ! CLUE_NO_TIMESTAMP

# include <ctime>

namespace clue {

	inline cpp_string now_text()
	{
		char mbstr[100];
		const std::time_t now = std::time(NULL);

		// ISO  ISO 8601 date and time format: C++11: %FT%T
		if (std::strftime(mbstr, 100, "%Y-%m-%dT%H:%M:%S", std::localtime(&now)))
			return mbstr;
		else
			return "[time]";
	}
} // namespace clue

#endif // CLUE_NO_TIMESTAMP

#ifdef CLUE_LOG_TO_CONSOLE

# include <iostream>

namespace clue
{
	// nlike std::cerr/std::wcerr, these streams are not automatically flushed
	// and cout is not automatically tie()'d with these streams.
	class win_con_dbg
	{
		SEVERITY level_{};
	public:
		win_con_dbg(SEVERITY level)	noexcept : level_(level)  {
			// roadmap: colouring by severity 
			std::clog << "\n" << now_text() << CLUE_DELIMITER ;
			std::clog << (to_severity_text(level_).c_str()) << CLUE_DELIMITER ;
		}

		~win_con_dbg() noexcept {
			std::clog.flush();
		}

		template<typename T>
		win_con_dbg & operator<<(T const& that) noexcept
		{
			std::clog << " " << that ;
			return *this;
		}
	};

	inline auto target = [](SEVERITY level_ /* , auto module_name*/) noexcept {
		return clue::win_con_dbg{ level_ };
	};

#define CLUE_LOG_EMERGENCY  clue::target( clue::SEVERITY::EMERGENCY  )
#define	CLUE_LOG_ALERT  clue::target( clue::SEVERITY::ALERT ) 
#define	CLUE_LOG_CRITICAL  clue::target( clue::SEVERITY::CRITICAL ) 
#define	CLUE_LOG_ERROR  clue::target( clue::SEVERITY::ERRORR ) 
#define	CLUE_LOG_WARNING  clue::target( clue::SEVERITY::WARNING ) 
#define	CLUE_LOG_NOTICE  clue::target( clue::SEVERITY::NOTICE ) 
#define	CLUE_LOG_INFO  clue::target( clue::SEVERITY::INFO ) 
#define	CLUE_LOG_DEBUG  clue::target( clue::SEVERITY::DEBUG ) 

} // namespace clue
#endif // CLUE_LOG_TO_CONSOLE

#ifdef CLUE_LOG_TO_DEBUGGER_WINDOWS

namespace clue
{

	class windbg
	{
		SEVERITY level_;
		std::ostringstream stream;
	public:
		windbg(SEVERITY level) noexcept
			: level_(level), stream() {}

		~windbg() noexcept
		{
			OutputDebugString("\n"); // dbj added
			OutputDebugString(now_text().c_str()); // dbj added
			OutputDebugString(CLUE_DELIMITER);
			OutputDebugString(to_severity_text(level_).c_str()); // dbj added
			OutputDebugString(CLUE_DELIMITER);
			OutputDebugString(stream.str().c_str());
		}

		template<typename T>
		windbg& operator<<(T const& that) noexcept
		{
			stream << that;
			return *this;
		}
	};

	inline auto target = [](SEVERITY level_ /* , auto module_name*/) noexcept {
		return clue::windbg{ level_ };
	};

#define CLUE_LOG_EMERGENCY  clue::target( clue::SEVERITY::EMERGENCY  )
#define	CLUE_LOG_ALERT  clue::target( clue::SEVERITY::ALERT ) 
#define	CLUE_LOG_CRITICAL  clue::target( clue::SEVERITY::CRITICAL ) 
#define	CLUE_LOG_ERROR  clue::target( clue::SEVERITY::ERRORR ) 
#define	CLUE_LOG_WARNING  clue::target( clue::SEVERITY::WARNING ) 
#define	CLUE_LOG_NOTICE  clue::target( clue::SEVERITY::NOTICE ) 
#define	CLUE_LOG_INFO  clue::target( clue::SEVERITY::INFO ) 
#define	CLUE_LOG_DEBUG  clue::target( clue::SEVERITY::DEBUG ) 

} // namespace clue

#endif // CLUE_LOG_TO_DEBUGGER_WINDOWS

#ifdef CLUE_LOG_TO_EVENTLOG

#pragma  comment(lib, "Advapi32.lib" ) 

namespace clue
{

	inline int to_eventlog_severity(SEVERITY severity) noexcept
	{
		switch (severity)
		{
		case SEVERITY::NONE:      return EVENTLOG_INFORMATION_TYPE;
		case SEVERITY::EMERGENCY: return EVENTLOG_ERROR_TYPE;
		case SEVERITY::ALERT:     return EVENTLOG_ERROR_TYPE;
		case SEVERITY::CRITICAL:  return EVENTLOG_ERROR_TYPE;
		case SEVERITY::ERRORR:     return EVENTLOG_ERROR_TYPE;
		case SEVERITY::WARNING:   return EVENTLOG_WARNING_TYPE;
		case SEVERITY::NOTICE:    return EVENTLOG_INFORMATION_TYPE;
		case SEVERITY::INFO:      return EVENTLOG_INFORMATION_TYPE;
		default:
		case SEVERITY::DEBUG:     return EVENTLOG_INFORMATION_TYPE;
		}
	}

	constexpr auto WIN_EVENT_LOG_SOURCE_NAME = CLUE_TITLE ;

	class evtlog
	{
	public:
		evtlog(SEVERITY severity_, cpp_string const& module) noexcept
			: severity(severity_)
			, module(module)
			, stream() {}

		~evtlog() noexcept
		{
			// note string lifetime
			const cpp_string text = stream.str();
			const char* strings[] = { text.c_str(), };

			// TODO: error checking

			const ::HANDLE hlog = ::RegisterEventSource(
				0, text_or(module, WIN_EVENT_LOG_SOURCE_NAME).c_str());

			::ReportEvent(
				hlog          // HANDLE hEventLog,    // handle returned by RegisterEventSource
				, to_eventlog_severity(severity)   // WORD wType, // event type to log
				, 0           // WORD wCategory,      // event category
				, 0           // DWORD dwEventID,     // event identifier
				, 0           // PSID lpUserSid,      // user security identifier (optional)
				, 1           // WORD wNumStrings,    // number of strings to merge with message
				, 0           // DWORD dwDataSize,    // size of binary data, in bytes
				, strings     // LPCTSTR *lpStrings,  // array of strings to merge with message
				, 0           // LPVOID lpRawData     // address of binary data
			);

			::DeregisterEventSource(hlog);
		}

		template<typename T>
		evtlog& operator<<(T const& that) noexcept
		{
			stream << that;
			return *this;
		}

	private:
		SEVERITY severity;
		const cpp_string module;
		std::ostringstream stream;
	};

	inline auto target = [](SEVERITY severity, auto module_name) noexcept {
		return clue::evtlog{ severity , module_name };
	};

} // namespace clue

#define CLUE_LOG_EMERGENCY  clue::target( clue::SEVERITY::EMERGENCY, CLUE_LOG_MODULE_NAME  )
#define	CLUE_LOG_ALERT  clue::target( clue::SEVERITY::ALERT, CLUE_LOG_MODULE_NAME  ) 
#define	CLUE_LOG_CRITICAL  clue::target( clue::SEVERITY::CRITICAL, CLUE_LOG_MODULE_NAME  ) 
#define	CLUE_LOG_ERROR  clue::target( clue::SEVERITY::ERRORR, CLUE_LOG_MODULE_NAME  ) 
#define	CLUE_LOG_WARNING  clue::target( clue::SEVERITY::WARNING, CLUE_LOG_MODULE_NAME  ) 
#define	CLUE_LOG_NOTICE  clue::target( clue::SEVERITY::NOTICE, CLUE_LOG_MODULE_NAME  ) 
#define	CLUE_LOG_INFO  clue::target( clue::SEVERITY::INFO, CLUE_LOG_MODULE_NAME  ) 
#define	CLUE_LOG_DEBUG  clue::target( clue::SEVERITY::DEBUG, CLUE_LOG_MODULE_NAME  ) 

#endif // CLUE_LOG_TO_EVENTLOG

#ifdef CLUE_LOG_TO_STRING

namespace clue {

	class strlog final
	{
		SEVERITY severity_;
		std::ostringstream& stream;

	public:

		explicit strlog(std::ostringstream& stream_, SEVERITY severity) noexcept
			: stream(stream_), severity_(severity)
		{
			stream << "\n";
		}

		void clear(SEVERITY severity = SEVERITY::NONE) noexcept
		{
			severity_ = severity;
			stream.str(cpp_string());
		}

		void severity(SEVERITY sev) noexcept
		{
			severity_ = sev;
		}

		SEVERITY severity() const noexcept
		{
			return severity_;
		}

		cpp_string text() const noexcept
		{
			return to_severity_text(severity_).append(":").append(stream.str());
		}

		template<typename T>
		strlog& operator<<(T const& that) noexcept
		{
			stream << that;
			return *this;
		}

	};

	inline strlog the_log(SEVERITY sev_ = SEVERITY::NONE) noexcept
	{
		static std::ostringstream stream_;
			return strlog (stream_, sev_);;
	}

	inline auto& target = [](SEVERITY severity /* , auto module_name*/) {
		return the_log(severity);
	};

#define CLUE_LOG_EMERGENCY  clue::target( clue::SEVERITY::EMERGENCY  )
#define	CLUE_LOG_ALERT  clue::target( clue::SEVERITY::ALERT ) 
#define	CLUE_LOG_CRITICAL  clue::target( clue::SEVERITY::CRITICAL ) 
#define	CLUE_LOG_ERROR  clue::target( clue::SEVERITY::ERRORR ) 
#define	CLUE_LOG_WARNING  clue::target( clue::SEVERITY::WARNING ) 
#define	CLUE_LOG_NOTICE  clue::target( clue::SEVERITY::NOTICE ) 
#define	CLUE_LOG_INFO  clue::target( clue::SEVERITY::INFO ) 
#define	CLUE_LOG_DEBUG  clue::target( clue::SEVERITY::DEBUG ) 

} // namespace clue

#endif // CLUE_LOG_TO_STRING

#ifdef CLUE_LOG_TO_STRING
#define CLUE_STRING_LOG_TEXT clue::the_log().text().c_str()
#else
#define CLUE_STRING_LOG_TEXT ""
#endif

#ifdef CLUE_LOG_TO_SYSLOG

#ifdef _WIN32
# include "syslog-win32/syslog.h"
# include "syslog-win32/syslogc.c"
#include <winsock2.h>
// #include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else  // ! _WIN32
#include <syslog.h>
#endif // ! _WIN32


namespace clue {

	inline int to_syslog_severity(SEVERITY severity) noexcept
	{
		switch (severity)
		{
		case SEVERITY::NONE:      return LOG_INFO;
		case SEVERITY::EMERGENCY: return LOG_EMERG;
		case SEVERITY::ALERT:     return LOG_ALERT;
		case SEVERITY::CRITICAL:  return LOG_CRIT;
		case SEVERITY::ERRORR:     return LOG_ERR;
		case SEVERITY::WARNING:   return LOG_WARNING;
		case SEVERITY::NOTICE:    return LOG_NOTICE;
		case SEVERITY::INFO:      return LOG_INFO;
		default:
		case SEVERITY::DEBUG:     return LOG_DEBUG;
		}
	}

	class syslog
	{
	public:
		explicit syslog(SEVERITY severity) noexcept
			: severity(severity)
			, stream() {}

		~syslog()
		{
			// emit: program-name[pid]:
			::openlog(NULL, LOG_PID, LOG_USER);
			::syslog(to_syslog_severity(severity), "%s", stream.str().c_str());
			::closelog();
		}

		template<typename T>
		syslog& operator<<(T const& that) noexcept
		{
			stream << that;
			return *this;
		}

	private:
		SEVERITY severity;
		std::ostringstream stream;
	};

	inline auto target = [](SEVERITY severity /*, auto module_name*/) noexcept {

		static bool init_syslog_done = false;

		// TODO: user define url for init_syslog
		if (init_syslog_done == false) {
			init_syslog("localhost");
			init_syslog_done = true;
		}

		return syslog(severity);
	};

} // namespace clue

#define CLUE_LOG_EMERGENCY  clue::target( clue::SEVERITY::EMERGENCY  )
#define	CLUE_LOG_ALERT  clue::target( clue::SEVERITY::ALERT ) 
#define	CLUE_LOG_CRITICAL  clue::target( clue::SEVERITY::CRITICAL ) 
#define	CLUE_LOG_ERROR  clue::target( clue::SEVERITY::ERRORR ) 
#define	CLUE_LOG_WARNING  clue::target( clue::SEVERITY::WARNING ) 
#define	CLUE_LOG_NOTICE  clue::target( clue::SEVERITY::NOTICE ) 
#define	CLUE_LOG_INFO  clue::target( clue::SEVERITY::INFO ) 
#define	CLUE_LOG_DEBUG  clue::target( clue::SEVERITY::DEBUG ) 

#endif //CLUE_LOG_TO_SYSLOG

#ifdef CLUE_COMPILER_IS_MSVC
# pragma warning( pop ) 
#endif

// TODO: undef all that can be undef'd
#undef CLUE_DELIMITER
#undef CLUE_COMPILER_IS_MSVC

#endif // CLUE_CLUE_H_INCLUDED
