#ifndef _SIMPLE_TIMER_H
#define _SIMPLE_TIMER_H

// SDL2.0
#include <SDL2/SDL.h>

class SimpleTimer
{
public:
    SimpleTimer();

    // Basic timer functions
    void start();
    void stop();
    void pause();
    void unpause();

    // Timer's time
    Uint32 getTicks();

    // Timer's status
    bool IsStarted();
    bool IsPaused();

private:
    // The timer's initial tick value
    Uint32 startTicks = 0;

    // The timer's tick value as when paused
    Uint32 pausedTicks = 0;

    // Timer's status
    bool started = false;
    bool paused = false;
};

#endif /*_SIMPLE_TIMER_H*/
