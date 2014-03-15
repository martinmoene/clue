// example3.cpp

#define LOG_MODULE_NAME "Gearbox"
#include "clue.hpp"

int main()
{
    const int gear = 5;
    LOG_NOTICE( "Gear switched to: " << gear );
}

// cl -nologo -EHsc example3.cpp && example3
// g++ -Wall -Wextra -Weffc++ -o example3.exe example3.cpp && example3
