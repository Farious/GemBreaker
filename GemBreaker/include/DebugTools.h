#pragma once
// Standard IO
#include <stdio.h>

/*
 * Just a wrapper for debug messaging
 */
// Enable\Disable debug messages
#define GB_DEBUG 1

// Enumerate that defines the logging level
enum class LogLevel :uint8_t { Debug, Warn, Error };

// Logs message with logging level, uses the same printf interface
void LogMessage(const LogLevel logLvl, const char* msg, ...)
{
    if (GB_DEBUG)
    {
        va_list ap;
        va_start(ap, msg);

        switch (logLvl)
        {
        case LogLevel::Debug:
            printf("DEBUG:: ");
            break;
        case LogLevel::Warn:
            printf("WARN:: ");
            break;
        case LogLevel::Error:
            printf("ERROR:: ");
            break;
        default:
            break;
        }

        vprintf(msg, ap);
        printf("\n");
    }
}

