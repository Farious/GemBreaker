#pragma once
// Standard IO
#include <stdio.h>

/*
 * Just a wrapper for debug messaging
*/
// Enable\Disable debug messages
#define GB_DEBUG 1

void LogMessage(const char* msg, ...)
{
    if (GB_DEBUG)
    {
        printf(msg);
    }
}

