#include "EntryPoint.h"


EntryPoint::EntryPoint()
{
}


EntryPoint::~EntryPoint()
{
}

bool EntryPoint::CreateWindow(const int W, const int H)
{
    if (!sdl_initialized)
    {
        // Let's Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return false;
        }
        else
        {
    }
}
