// example3a.cpp

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

// cl -nologo -W4 -EHsc -I.. example3a.cpp && example3a
// g++ -Wall -Wextra -Weffc++ -I.. -o example3a.exe example3a.cpp && example3a
