#ifndef _DEBUG_TOOLS_H
#define _DEBUG_TOOLS_H

// Standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

// Our includes
#include "SimpleConstants.h"

/*
 * Just a wrapper for debug messaging
 */
// Enumerate that defines the logging level
enum class LogLevel :Uint8 { Debug, Warn, Error };

// Logs message with logging level, uses the same printf interface
extern void LogMessage(const LogLevel logLvl, const char* msg, ...);

#endif /*_DEBUG_TOOLS_H*/
