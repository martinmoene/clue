// Copyright 2014 by Martin Moene
//
// clue is based on ideas by Mark Nelson, see article at
// http://www.drdobbs.com/cpp/blundering-into-the-one-definition-rule/240166489
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define clue_LOG_TO_STRING

#include "clue.hpp"
#include "lest_cpp03.hpp"

using namespace clue;

lest::tests & specification()
{
    static lest::tests tests;
    return tests;
}

#define CASE( name ) lest_CASE( specification(), name )

CASE( "Function is_true() to suppress warning \"expression has no effect\" acts as identity function." )
{
    EXPECT( false == is_true( false ) );
    EXPECT(  true == is_true( true  ) );
}

CASE( "Function text_or() gives text for non-empty text." )
{
    EXPECT( "text" == text_or( "text", "or_text" ) );
}

CASE( "Function text_or() gives or_text for empty text." )
{
    EXPECT( "or_text" == text_or( "", "or_text" ) );
}

CASE( "Function text_with_or() gives enclosed text for non-empty text." )
{
    EXPECT( "^text$" == text_with_or( "^", "text", "$", "or_text" ) );
}

CASE( "Function text_with_or() gives or_text for empty text." )
{
    EXPECT( "or_text" == text_with_or( "^", "", "$", "or_text" ) );
}

CASE( "Function to_module_text() prepends a colon to text." )
{
    EXPECT( ": Module" == to_module_text( "Module" ) );
}

CASE( "Function to_severity_text() return correct text for severity." )
{
    struct Table { 
        int severity; char const * text; 
    } 
    table[] = 
    {
        { clue_LOG_SEV_NONE     , clue_LOG_SEV_NONE_TEXT,       },
        { clue_LOG_SEV_EMERGENCY, clue_LOG_SEV_EMERGENCY_TEXT,  },
        { clue_LOG_SEV_ALERT    , clue_LOG_SEV_ALERT_TEXT,      },
        { clue_LOG_SEV_CRITICAL , clue_LOG_SEV_CRITICAL_TEXT,   },
        { clue_LOG_SEV_ERROR    , clue_LOG_SEV_ERROR_TEXT,      },
        { clue_LOG_SEV_WARNING  , clue_LOG_SEV_WARNING_TEXT,    },
        { clue_LOG_SEV_NOTICE   , clue_LOG_SEV_NOTICE_TEXT,     },
        { clue_LOG_SEV_INFO     , clue_LOG_SEV_INFO_TEXT,       },
        { clue_LOG_SEV_DEBUG    , clue_LOG_SEV_DEBUG_TEXT,      },
        { clue_LOG_SEV_MAX      , clue_LOG_SEV_DEBUG_TEXT,      },
        { 0, 0, },
    };
    
    for ( Table * pos = table; pos->text; ++pos )
    {
        EXPECT( pos->text == to_severity_text( pos->severity ) );
    }
}

CASE( "Function to_severities_text() with clue_LOG_SEV_NONE gives postfix dot only" )
{
    EXPECT( "." == to_severities_text( clue_LOG_SEV_NONE ) );
}

CASE( "Function to_severities_text() with clue_LOG_SEV_DEBUG gives all severities." )
{
    char const * const text = "Emergency, Alert, Critical, Error, Warning, Notice, Info, Debug.";
    EXPECT( text == to_severities_text( clue_LOG_SEV_DEBUG ) );
}

CASE( "Function to_severities_text() with clue_LOG_SEV_ALERT gives two severities." )
{
    char const * const text = "Emergency, Alert.";
    EXPECT( text == to_severities_text( clue_LOG_SEV_ALERT ) );
}

CASE( "Function to_severities_text() with non-default postfix ends correctly." )
{
    char const * const text = "Emergency, Alert$$";
    EXPECT( text == to_severities_text( clue_LOG_SEV_ALERT, "$$" ) );
}

CASE( "Macro LOG_ALERT() correctly records severity." )
{
    the_log().clear();
    LOG_ALERT( "" );
    EXPECT( the_log().severity() == clue_LOG_SEV_ALERT );
}

CASE( "Macro LOG_TO_STRING() correctly records text." )
{
    std::string text = "hello world";
    
    the_log().clear();
    LOG_ALERT( text );
    EXPECT( the_log().text() == text );
}

int main( int argc, char * argv[] )
{
    return lest::run( specification(), argc, argv );
}

// cl -nologo -W3 -EHsc -I.. test_clue.cpp test_clue_part2.cpp && test_clue
// cl -nologo -Wall -EHsc -I.. test_clue.cpp test_clue_part2.cpp && test_clue
// g++ -Wall -Wextra -Weffc++ -std=c++11 -I.. -o test_clue.exe test_clue.cpp test_clue_part2.cpp && test_clue
// g++ -Wall -Wextra -Weffc++ -std=c++03 -I.. -o test_clue.exe test_clue.cpp test_clue_part2.cpp && test_clue
