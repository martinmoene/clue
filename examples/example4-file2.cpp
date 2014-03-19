// example4-fle2.cpp

#define LOG_MODULE_NAME "Engine"
#include "clue.hpp"

void engine()
{
    const int rpm = 3000;
    clue_LOG_NOTICE( "Rpm: " << rpm );
}
