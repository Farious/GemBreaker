#pragma once
// SDL2.0
#include <SDL.h>

// Standard IO
#include <stdio.h>

class EntryPoint
{
public:
    //TODO - Screen dimensions
    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

private:
#pragma region SDL Variables
    // SDL Initialized
    bool sdl_initialized = false;

    // Rendering Window
    SDL_Window *window = nullptr;

    // Main surface
    SDL_Surface *screenSurface = nullptr;
#pragma endregion

public:
	EntryPoint();
    ~EntryPoint();

    bool CreateWindow(const int W, const int H);
};

