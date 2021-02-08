// example5.cpp - Log to file, include module name.

#define LOG_MODULE_NAME "Messenger"
#include "clue.hpp"
#include <iostream>

int main()
{
    clue_LOG_NOTICE( "Hello" << " world" );
}

// cl -nologo -W4 -EHsc -I.. -Dclue_LOG_TO_FILE=\"clue-log.txt\" example5.cpp && example5
// g++ -Wall -Wextra -Weffc++ -I.. -Dclue_LOG_TO_FILE=\"clue-log.txt\" -o example5.exe example5.cpp && example5
// clang-cl -Wall -Wextra -Weffc++ -I.. -Dclue_LOG_TO_FILE=\"clue-log.txt\" -o example5.exe example5.cpp && example5
