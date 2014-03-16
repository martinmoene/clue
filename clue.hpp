// Copyright 2014 by Martin Moene
//
// clue is based on ideas by Mark Nelson, see article at
// http://www.drdobbs.com/cpp/blundering-into-the-one-definition-rule/240166489
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef CLUE_CLUE_H_INCLUDED
#define CLUE_CLUE_H_INCLUDED

#include <iomanip>
#include <sstream>
#include <cassert>

#if defined( _MSC_VER )
# define clue_COMPILER_IS_MSVC
# if ( _MSC_VER >= 1200 ) && ( _MSC_VER < 1300 )
#  define clue_COMPILER_IS_MSVC6
# endif
#endif

#ifndef clue_NO_SHORT_NAMES

# ifdef  LOG_MODULE_NAME
#  define clue_LOG_MODULE_NAME  LOG_MODULE_NAME
# endif

# ifdef  LOG_LEVEL
#  define clue_LOG_LEVEL  LOG_LEVEL
# endif

# ifdef  LOG_TO_CONSOLE
#  define clue_LOG_TO_CONSOLE  LOG_TO_CONSOLE
# endif

# ifdef  LOG_TO_DEBUGGER
#  define clue_LOG_TO_DEBUGGER  LOG_TO_DEBUGGER
# endif

# ifdef  LOG_TO_DEBUGGER_WINDOWS
#  define clue_LOG_TO_DEBUGGER_WINDOWS  LOG_TO_DEBUGGER_WINDOWS
# endif

# ifdef  LOG_TO_EVENTLOG
#  define clue_LOG_TO_EVENTLOG  LOG_TO_EVENTLOG
# endif

# ifdef  LOG_TO_SYSLOG
#  define clue_LOG_TO_SYSLOG  LOG_TO_SYSLOG
# endif

# ifdef  LOG_EXPRESSION
#  define clue_LOG_EXPRESSION  LOG_EXPRESSIONLOG_TO_SYSLOG
# endif

#endif // clue_NO_SHORT_NAMES

// now we can determine if we must guess a destination:

#if !defined( clue_LOG_TO_CONSOLE  ) && \
    !defined( clue_LOG_TO_DEBUGGER ) && \
    !defined( clue_LOG_TO_EVENTLOG ) && \
    !defined( clue_LOG_TO_SYSLOG   )
# if defined( _WINDOWS )
#  define clue_LOG_TO_DEBUGGER
#elif defined( NTS_TO_BE_DETERMINED_UNIX )
#  define clue_LOG_TO_SYSLOG
# else
#  define clue_LOG_TO_CONSOLE
# endif
#endif

#if defined( clue_LOG_TO_CONSOLE  ) + \
    defined( clue_LOG_TO_DEBUGGER ) + \
    defined( clue_LOG_TO_EVENTLOG ) + \
    defined( clue_LOG_TO_SYSLOG   ) > 1
# error Please specify one, or none of [clue_]LOG_TO_CONSOLE, [clue_]LOG_TO_DEBUGGER [clue_]LOG_TO_EVENTLOG and [clue_]LOG_TO_SYSLOG
#endif

// NTS: add UNIX
#ifdef clue_LOG_TO_DEBUGGER
# define clue_LOG_TO_DEBUGGER_WINDOWS
#endif

#ifndef clue_NO_SHORT_NAMES

# define LOG_SEV_NONE       clue_LOG_SEV_NONE
# define LOG_SEV_EMERGENCY  clue_LOG_SEV_EMERGENCY
# define LOG_SEV_ALERT      clue_LOG_SEV_ALERT
# define LOG_SEV_CRITICAL   clue_LOG_SEV_CRITICAL
# define LOG_SEV_ERROR      clue_LOG_SEV_ERROR
# define LOG_SEV_WARNING    clue_LOG_SEV_WARNING
# define LOG_SEV_NOTICE     clue_LOG_SEV_NOTICE
# define LOG_SEV_INFO       clue_LOG_SEV_INFO
# define LOG_SEV_DEBUG      clue_LOG_SEV_DEBUG
# define LOG_SEV_MAX        clue_LOG_SEV_MAX

# ifndef clue_LOG_TO_SYSLOG
#  define LOG_EMERGENCY      clue_LOG_EMERGENCY
#  define LOG_ALERT          clue_LOG_ALERT
#  define LOG_CRITICAL       clue_LOG_CRITICAL
#  define LOG_ERROR          clue_LOG_ERROR
#  define LOG_WARNING        clue_LOG_WARNING
#  define LOG_NOTICE         clue_LOG_NOTICE
#  define LOG_INFO           clue_LOG_INFO
#  define LOG_DEBUG          clue_LOG_DEBUG
# endif

# define LOG_LOGGED_LEVELS   clue_LOG_LOGGED_LEVELS
# define LOG_EXPRESSION      clue_LOG_EXPRESSION

#endif // clue_NO_SHORT_NAMES

#ifdef clue_LOG_TO_CONSOLE
# include <iostream>
#endif

#ifdef clue_LOG_TO_DEBUGGER_WINDOWS
# include <windows.h>
#endif

#ifdef clue_LOG_TO_DEBUGGER_UNIX
# error log to debugger under Unix not implemented
#endif

#ifdef clue_LOG_TO_EVENTLOG
# include <windows.h>
#endif

#ifdef clue_LOG_TO_SYSLOG
# include <syslog.h>
#endif

const int clue_LOG_SEV_NONE      = -1;
const int clue_LOG_SEV_EMERGENCY = 0;
const int clue_LOG_SEV_ALERT     = 1;
const int clue_LOG_SEV_CRITICAL  = 2;
const int clue_LOG_SEV_ERROR     = 3;
const int clue_LOG_SEV_WARNING   = 4;
const int clue_LOG_SEV_NOTICE    = 5;
const int clue_LOG_SEV_INFO      = 6;
const int clue_LOG_SEV_DEBUG     = 7;
const int clue_LOG_SEV_MAX       = 7;

#ifndef clue_LOG_LEVEL
# define clue_LOG_LEVEL clue_LOG_SEV_DEBUG
#endif

#define clue_LOG_SEV_NONE_TEXT       "[clue]"
#define clue_LOG_SEV_EMERGENCY_TEXT  "Emergency"
#define clue_LOG_SEV_ALERT_TEXT      "Alert"
#define clue_LOG_SEV_CRITICAL_TEXT   "Critical"
#define clue_LOG_SEV_ERROR_TEXT      "Error"
#define clue_LOG_SEV_WARNING_TEXT    "Warning"
#define clue_LOG_SEV_NOTICE_TEXT     "Notice"
#define clue_LOG_SEV_INFO_TEXT       "Info"
#define clue_LOG_SEV_DEBUG_TEXT      "Debug"

#ifndef  clue_LOG_MODULE_NAME
# define clue_LOG_MODULE_NAME ""
#endif

#ifndef  clue_LOG_PREFIX_WIDTH
# define clue_LOG_PREFIX_WIDTH      sizeof( clue_LOG_SEV_EMERGENCY_TEXT )
#endif

#define clue_LOG_LOGGED_LEVELS() \
    clue_LOG_EXPRESSION( clue_LOG_SEV_NONE, clue::to_severities_text( clue_LOG_LEVEL ) )

#if clue_LOG_LEVEL >= clue_LOG_SEV_EMERGENCY
# define clue_LOG_EMERGENCY( expr ) clue_LOG_EXPRESSION( clue_LOG_SEV_EMERGENCY, expr )
#else
# define clue_LOG_EMERGENCY( expr )
#endif

#if clue_LOG_LEVEL >= clue_LOG_SEV_ALERT
# define clue_LOG_ALERT( expr ) clue_LOG_EXPRESSION( clue_LOG_SEV_ALERT, expr )
#else
# define clue_LOG_ALERT( expr )
#endif

#if clue_LOG_LEVEL >= clue_LOG_SEV_CRITICAL
# define clue_LOG_CRITICAL( expr ) clue_LOG_EXPRESSION( clue_LOG_SEV_CRITICAL, expr )
#else
# define clue_LOG_CRITICAL( expr )
#endif

#if clue_LOG_LEVEL >= clue_LOG_SEV_ERROR
# define clue_LOG_ERROR( expr ) clue_LOG_EXPRESSION( clue_LOG_SEV_ERROR, expr )
#else
# define clue_LOG_ERROR( expr )
#endif

#if clue_LOG_LEVEL >= clue_LOG_SEV_WARNING
# define clue_LOG_WARNING( expr ) clue_LOG_EXPRESSION( clue_LOG_SEV_WARNING, expr )
#else
# define clue_LOG_WARNING( expr )
#endif

#if clue_LOG_LEVEL >= clue_LOG_SEV_NOTICE
# define clue_LOG_NOTICE( expr ) clue_LOG_EXPRESSION( clue_LOG_SEV_NOTICE, expr )
#else
# define clue_LOG_NOTICE( expr )
#endif

#if clue_LOG_LEVEL >= clue_LOG_SEV_INFO
# define clue_LOG_INFO( expr ) clue_LOG_EXPRESSION( clue_LOG_SEV_INFO, expr )
#else
# define clue_LOG_INFO( expr )
#endif

#if clue_LOG_LEVEL >= clue_LOG_SEV_DEBUG  &&  !defined( NDEBUG )
# define clue_LOG_DEBUG( expr ) clue_LOG_EXPRESSION( clue_LOG_SEV_DEBUG, expr )
#else
# define clue_LOG_DEBUG( expr )
#endif

#if defined( clue_LOG_TO_CONSOLE ) && !defined( clue_LOG_EXPRESSION )
# define clue_LOG_EXPRESSION( severity, expr ) \
    std::clog << \
        clue::now_text() << std::setw( clue_LOG_PREFIX_WIDTH ) << \
        clue::to_severity_text(severity) << \
        clue::to_module_text(clue_LOG_MODULE_NAME) << ": " << expr << "\n"
#endif

#if defined( clue_LOG_TO_DEBUGGER_WINDOWS ) && !defined( clue_LOG_EXPRESSION )
# define clue_LOG_EXPRESSION( severity, expr ) \
    clue::windbg( severity, clue_LOG_PREFIX_WIDTH ).get() << \
        clue::to_module_text(clue_LOG_MODULE_NAME) << ": " << expr
#endif

#if defined( clue_LOG_TO_EVENTLOG ) && !defined( clue_LOG_EXPRESSION )
# define clue_LOG_EXPRESSION( severity, expr ) \
    clue::evtlog( severity ).get() << \
        clue_LOG_MODULE_NAME << ": " << expr
#endif

#if defined( clue_LOG_TO_SYSLOG ) && !defined( clue_LOG_EXPRESSION )
# define clue_LOG_EXPRESSION( severity, expr ) \
    clue::syslog( severity ).get() << \
        clue_LOG_MODULE_NAME << ": " << expr
#endif

namespace clue
{

inline std::string text_or( std::string const & text, std::string const & or_text )
{
    return text.length() ? text : or_text;
}

inline std::string text_with_or( std::string const & prefix, std::string const & text, std::string const & postfix, std::string const & or_text )
{
    return text.length() ? prefix + text + postfix: or_text;
}

inline std::string to_module_text( std::string const & module )
{
    return text_with_or( ": ", module, "", "" );
}

inline std::string to_severity_text( int const severity )
{
    assert( clue_LOG_SEV_NONE <= severity && severity <= clue_LOG_SEV_MAX && "invalid severity" );

    if ( severity == clue_LOG_SEV_NONE )
        return clue_LOG_SEV_NONE_TEXT;

    assert( LOG_SEV_EMERGENCY == 0 && "required by lookup table" );

    std::string const cont[] =
    {
        clue_LOG_SEV_EMERGENCY_TEXT,
        clue_LOG_SEV_ALERT_TEXT,
        clue_LOG_SEV_CRITICAL_TEXT,
        clue_LOG_SEV_ERROR_TEXT,
        clue_LOG_SEV_WARNING_TEXT,
        clue_LOG_SEV_NOTICE_TEXT,
        clue_LOG_SEV_INFO_TEXT,
        clue_LOG_SEV_DEBUG_TEXT,
    };
    return cont[ severity ];
}

inline std::string to_severities_text( int const level, std::string const postfix = ".", std::string const result = "" )
{
    if ( level < 0 )
        return result + postfix;

    return to_severities_text( level - 1, postfix, to_severity_text( level ) + (result.length() ? ", " : "") + result );
}

} // namespace clue

#ifdef clue_NO_TIMESTAMP

namespace clue {
inline std::string now_text() { return ""; }
}

#else // clue_NO_TIMESTAMP

# include <ctime>

# ifdef clue_COMPILER_IS_MSVC6
namespace std {
using ::time_t; using ::time; using ::strftime; using ::localtime;
}
# endif

namespace clue {

inline std::string now_text()
{
    char mbstr[100];
    const std::time_t now = std::time(NULL);

    // ISO  ISO 8601 date and time format: C++11: %FT%T
    if ( std::strftime( mbstr, 100, "%Y-%m-%dT%H:%M:%S", std::localtime( &now ) ) )
        return mbstr;
    else
        return "[time]";
}
} // namespace clue

#endif // clue_NO_TIMESTAMP

#ifdef clue_LOG_TO_DEBUGGER_WINDOWS

namespace clue
{

class windbg
{
    template<typename T>
    friend windbg & operator<<( windbg & stream, T const & that );

public:
    windbg( int const severity, int const severity_width )
    : stream()
    {
        stream << std::setw( severity_width ) << to_severity_text(severity);
    }

    windbg( windbg const & other )
    : stream()
    {
        stream << other.stream.rdbuf();
    }

//    windbg( windbg && other )
//    : stream( std::move( other ) ) {}

    ~windbg()
    {
        OutputDebugString( stream.str().c_str() );
    }
    
    windbg & get() 
    { 
        return *this; 
    }

private:
    std::ostringstream stream;
};

template<typename T>
windbg & operator<<( windbg & stream, T const & that )
{
    stream.stream << that;
    return stream;
}

} // namespace clue

#endif // clue_LOG_TO_DEBUGGER_WINDOWS

#ifdef clue_LOG_TO_EVENTLOG

namespace clue
{

inline const char * text_or( char const * const text, char const * const or_text )
{
    return strlen(text) ? text : or_text;
}

inline int to_eventlog_severity( int severity )
{
    assert( clue_LOG_SEV_NONE <= severity && severity <= clue_LOG_SEV_MAX && "invalid severity" );

    switch( severity )
    {
        case clue_LOG_SEV_NONE:      return EVENTLOG_INFORMATION_TYPE;
        case clue_LOG_SEV_EMERGENCY: return EVENTLOG_ERROR_TYPE;
        case clue_LOG_SEV_ALERT:     return EVENTLOG_ERROR_TYPE;
        case clue_LOG_SEV_CRITICAL:  return EVENTLOG_ERROR_TYPE;
        case clue_LOG_SEV_ERROR:     return EVENTLOG_ERROR_TYPE;
        case clue_LOG_SEV_WARNING:   return EVENTLOG_WARNING_TYPE;
        case clue_LOG_SEV_NOTICE:    return EVENTLOG_INFORMATION_TYPE;
        case clue_LOG_SEV_INFO:      return EVENTLOG_INFORMATION_TYPE;
        default:
        case clue_LOG_SEV_DEBUG:     return EVENTLOG_INFORMATION_TYPE;
    }
}

class evtlog
{
    template<typename T>
    friend evtlog & operator<<( evtlog & stream, T const & that );

public:
    evtlog( int const severity )
    : severity( severity )
    , stream() {}

    evtlog( evtlog const & other )
    : severity( other.severity )
    , stream()
    {
        stream << other.stream.rdbuf();
    }

//    evtlog( evtlog && other )
//    : stream( std::move( other ) ) {}

    ~evtlog()
    {
        // note string lifetime
        const std::string text = stream.str();
        const char *strings[]  = { text.c_str(), };

        const ::HANDLE hlog = ::RegisterEventSource(
            0, text_or( clue_LOG_MODULE_NAME, "[clue]" ) );

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

        ::DeregisterEventSource( hlog );
    }

    evtlog & get() 
    { 
        return *this; 
    }

private:
    const int severity;
    std::ostringstream stream;
};

template<typename T>
evtlog & operator<<( evtlog & stream, T const & that )
{
    stream.stream << that;
    return stream;
}

} // namespace clue

#endif // clue_LOG_TO_DEBUGGER_WINDOWS

#ifdef clue_LOG_TO_SYSLOG

namespace clue {

inline int to_syslog_severity( int severity )
{
    assert( clue_LOG_SEV_NONE <= severity && severity <= clue_LOG_SEV_MAX && "invalid severity" );

    switch( severity )
    {
        case clue_LOG_SEV_NONE:      return LOG_INFO;
        case clue_LOG_SEV_EMERGENCY: return LOG_EMERG;
        case clue_LOG_SEV_ALERT:     return LOG_ALERT;
        case clue_LOG_SEV_CRITICAL:  return LOG_CRIT;
        case clue_LOG_SEV_ERROR:     return LOG_ERR;
        case clue_LOG_SEV_WARNING:   return LOG_WARNING;
        case clue_LOG_SEV_NOTICE:    return LOG_NOTICE;
        case clue_LOG_SEV_INFO:      return LOG_INFO;
        default:
        case clue_LOG_SEV_DEBUG:     return LOG_DEBUG;
    }
}

class syslog
{
    template<typename T>
    friend syslog& operator<<( syslog& stream, const T& that );

public:
    syslog( int const severity )
    : severity( severity )
    , stream() {}

    syslog( syslog const & other )
    : severity( other.severity )
    , stream()
    {
        stream << other.stream.rdbuf();
    }

    ~syslog()
    {
        ::syslog( to_syslog_severity(severity), stream.str().c_str() );
    }

    syslog & get() 
    { 
        return *this; 
    }

private:
    const int severity;
    std::ostringstream stream;
};

template<typename T>
syslog & operator<<( syslog & stream, T const & that )
{
    stream.stream << that;
    return stream;
}

} // namespace clue

#endif //clue_LOG_TO_SYSLOG

#endif // CLUE_CLUE_H_INCLUDED
