#include "SimpleTimer.h"


SimpleTimer::SimpleTimer()
{
    startTicks = 0;
    pausedTicks = 0;

    paused = false;
    started = false;
}

void SimpleTimer::start()
{
    // Setting clock state
    started = true;
    paused = false;

    // Set ticks
    startTicks = SDL_GetTicks();
    pausedTicks = 0;
}

void SimpleTimer::stop()
{
    // Setting clock state
    started = false;
    paused = false;

    // Clear ticks
    startTicks = 0;
    pausedTicks = 0;
}

void SimpleTimer::pause()
{
    // This must check if the timer is started and not paused
    if ( started && !paused)
    {
        paused = true;

        pausedTicks = SDL_GetTicks() - startTicks;
        startTicks = 0;
    }
}

void SimpleTimer::unpause()
{
    // This must check if the timer is started and paused
    if (started && paused)
    {
        paused = false;

        startTicks = SDL_GetTicks() - pausedTicks;
        pausedTicks = 0;
    }
}

Uint32 SimpleTimer::getTicks()
{
    // Timer's time
    Uint32 time = 0;

    // Check if timer is running
    if (started)
    {
        // If timer is paused just return the paused ticks
        if (paused)
        {
            time = pausedTicks;
        }
        else
        {
            time = SDL_GetTicks() - startTicks;
        }
    }

    return time;
}

bool SimpleTimer::IsStarted()
{
    return started;
}

bool SimpleTimer::IsPaused()
{
    return started && paused;
}
