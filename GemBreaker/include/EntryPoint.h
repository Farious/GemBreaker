#pragma once
// SDL2.0
#include <SDL.h>

// Standard IO
#include <stdio.h>

// Simple debugging tools
#include "DebugTools.h"

// Simple color holder in RGB Space
struct ColorRGB { uint8_t R; uint8_t G; uint8_t B; };

// Defining the possible game state so that we can process accordingly
enum class GameState :uint8_t { Init, Running, Paused, End };

class EntryPoint
{
public:
    //TODO - Screen dimensions
    const uint32_t ScreenWidth = 640;
    const uint32_t ScreenHeight = 480;

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

    // For event handling
    SDL_Event event;
#pragma endregion SDL Variables

#pragma region Game Variables
    GameState gameState = GameState::Init;
    uint8_t frameRate = 30;
#pragma endregion Game Variables

public:
	EntryPoint();
    ~EntryPoint();

#pragma region SDL Functions
    bool InitSDL();
    bool CreateWindow(const uint32_t W, const uint32_t H);
    bool CreateWindow();
    bool HandleSDLEvents();
    bool PaintWindowRGB(const ColorRGB color);
    bool OnQuit();
#pragma endregion SDL Functions

    bool MainLoop();
};
