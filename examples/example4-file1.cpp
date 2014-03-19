// example4-file1.cpp

#define LOG_MODULE_NAME "Gearbox"
#include "clue.hpp"

extern void engine();

int main()
{
    engine();
    
    const int gear = 5;
    clue_LOG_NOTICE( "Gear switched to: " << gear );
}

// cl -nologo -W4 -EHsc -I.. example4-file1.cpp example4-file2.cpp && example4-file1
// g++ -Wall -Wextra -Weffc++ -I.. -o example4-file1.exe example4-file1.cpp example4-file2.cpp && example4-file1
