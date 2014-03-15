// example2.cpp

#define LOG_MODULE_NAME "Gearbox"
#include "clue.hpp"

int main()
{
    const int gear = 5;
    LOG_NOTICE( "Gear switched to: " << gear );
}

// cl -nologo -EHsc example2.cpp && example2
// g++ -Wall -Wextra -Weffc++ -o example2.exe example2.cpp && example2
