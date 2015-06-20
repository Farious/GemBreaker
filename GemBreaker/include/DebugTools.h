#ifndef _DEBUG_TOOLS_H
#define _DEBUG_TOOLS_H

// Standard IO
#include <SDL.h>
#include <stdio.h>

/*
 * Just a wrapper for debug messaging
 */
// Enable\Disable debug messages
#define GB_DEBUG 1

// Enumerate that defines the logging level
enum class LogLevel :Uint8 { Debug, Warn, Error };

// Logs message with logging level, uses the same printf interface
extern void LogMessage(const LogLevel logLvl, const char* msg, ...);

#endif /*_DEBUG_TOOLS_H*/