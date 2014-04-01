// Copyright 2014 by Martin Moene
//
// clue is based on ideas by Mark Nelson, see article at
// http://www.drdobbs.com/cpp/blundering-into-the-one-definition-rule/240166489
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define clue_LOG_TO_STRING
#define clue_LOG_EXPRESSION( sev, expr ) \
    clue_LOG_STRING_EXPRESSION( my_log, sev, expr )

#include "clue.hpp"
#include "lest_cpp03.hpp"

using namespace lest;
using namespace clue;

extern test_specification specification;

#define TEST( name ) lest_TEST( specification, name )

namespace {

strlog my_log;

TEST( "Macro LOG_ALERT() correctly records severity with user-provided string logger." )
{
    my_log.clear();
    LOG_ALERT( "" );
    EXPECT( my_log.severity() == clue_LOG_SEV_ALERT );
}

TEST( "Macro LOG_TO_STRING() correctly records text with user-provided string logger." )
{
    std::string text = "hello world";
    
    my_log.clear();
    LOG_ALERT( text );
    EXPECT( my_log.text() == text );
}

}
