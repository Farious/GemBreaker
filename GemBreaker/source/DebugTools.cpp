#include "DebugTools.h"

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