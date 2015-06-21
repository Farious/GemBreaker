#ifndef _ENTRY_POINT_H
#define _ENTRY_POINT_H

// SDL2.0
#include <SDL.h>

// Standard IO
#include <stdio.h>
#include <string>

// Simple debugging tools
#include "DebugTools.h"

// Generic Classes
#include "SimpleTimer.h"
#include "SimpleTexture.h"

// Gem Breaker Includes
#include "GameTable.h"

using string = std::string;

// Defining the possible game state so that we can process accordingly
enum class GameState :Uint8 { Init, Running, Paused, End };

class EntryPoint
{
public:
    //TODO - Screen dimensions and frame rate
    const Uint32 ScreenWidth = 640;
    const Uint32 ScreenHeight = 480;
    const Uint32 ScreenFPS = 60; 
    const Uint32 ScreenTicksPerFrame = 1000 / ScreenFPS;

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

    //The window renderer 
    SDL_Renderer *renderer = nullptr;

    // For event handling
    SDL_Event event;
#pragma endregion SDL Variables

#pragma region Game Settings Variables
    GameState gameState = GameState::Init;
    SimpleTimer fpsTimer;
    SimpleTimer capTimer;

    // Frame rate variables
    Uint32 frameCount = 0;
    float avgFPS = 0;
    SimpleTexture* fpsTex = nullptr;
#pragma endregion Game Settings Variables

#pragma region Game Cosmetics
    string fontName = "resources/OpenSans-Regular.ttf";
    TTF_Font* font = nullptr;
#pragma endregion Game Cosmetics

    // The game table
    GameTable game;
public:
	EntryPoint();
    ~EntryPoint();

#pragma region SDL Functions
    bool InitSDL();
    bool CreateWindow(const Uint32 W, const Uint32 H);
    bool CreateWindow();
    bool HandleSDLEvents();
    bool PaintWindowRGB(const SDL_Color color);
    bool OnQuit();
#pragma endregion SDL Functions

#pragma region Tools
    void CalculateAverageFPS();
    void DrawFramerate();
#pragma endregion Tools

    bool MainLoop();
};

#endif /*_ENTRY_POINT_H*/