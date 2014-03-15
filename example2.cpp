// example2.cpp

#define clue_LOG_EXPRESSION( severity, expr ) \
    std::cout << clue::to_severity_text(severity) << ": " << expr

#include "clue.hpp"

int main()
{
    clue_LOG_NOTICE( "Hello" << " world" );
}

// cl -nologo -EHsc example2.cpp && example2
// g++ -Wall -Wextra -Weffc++ -o example2.exe example2.cpp && example2
