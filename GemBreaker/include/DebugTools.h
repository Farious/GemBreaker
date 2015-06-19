#pragma once
/*
 * Just a wrapper for debug messaging
*/
// Enable\Disable debug messages
#define GB_DEBUG 1

class DebugTools
{
public:

    DebugTools()
    {
    }

    ~DebugTools()
    {
    }

    void LogMessage(const string msg);
};

void DebugTools::LogMessage(const string msg)
{
    if (GB_DEBUG)
    {
        printf(msg);
    }
}

