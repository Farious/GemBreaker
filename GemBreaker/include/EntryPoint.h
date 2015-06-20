#pragma once
// SDL2.0
#include <SDL.h>

// Standard IO
#include <stdio.h>

// Simple debugging tools
#include "DebugTools.h"

// Simple color holder in RGB Space
struct ColorRGB
{
    int R;
    int G;
    int B;
};

class EntryPoint
{
public:
    //TODO - Screen dimensions
    const int ScreenWidth = 640;
    const int ScreenHeight = 480;

    // Window name
    const char* WindowName = "GemBreaker";

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

#pragma region SDL Functions
    bool InitSDL();
    bool CreateWindow(const int W, const int H);
    bool CreateWindow();
    bool PaintWindowRGB(const ColorRGB color);
    bool OnQuit();
#pragma endregion SDL Functions

    bool MainLoop();
};
