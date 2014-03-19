// example2.cpp

#define LOG_MODULE_NAME "Gearbox"
#include "clue.hpp"

int main()
{
    const int gear = 5;
    clue_LOG_NOTICE( "Gear switched to: " << gear );
}

// cl -nologo -W4 -EHsc -I.. example2.cpp && example2
// g++ -Wall -Wextra -Weffc++ -I.. -o example2.exe example2.cpp && example2
// g++ -Wall -Wextra -Weffc++ -I.. -DLOG_TO_SYSLOG -o example2.exe example2.cpp && example2
