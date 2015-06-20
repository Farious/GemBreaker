#include "EntryPoint.h"


EntryPoint::EntryPoint()
{
    sdl_initialized = false;
    window = nullptr;
    screenSurface = nullptr;
    gameState = GameState::Init;
}

EntryPoint::~EntryPoint()
{

}

bool EntryPoint::InitSDL()
{
    if (!sdl_initialized)
    {
        // Let's Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            LogMessage(LogLevel::Error, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        sdl_initialized = true;
        // Everything went as expected
        return true;
    }
    else
    {
        LogMessage(LogLevel::Warn, "SDL already initialized");
        return true;
    }

    return false; //This is not expected
}

bool EntryPoint::CreateWindow(const uint32_t W, const uint32_t H)
{
    if (sdl_initialized)
    {
        //Create window 
        window = SDL_CreateWindow(WindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, W, H, SDL_WINDOW_SHOWN);
        if (window == nullptr)
        {
            LogMessage(LogLevel::Error, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return false;
        }

        // Everything went as expected
        return true;
    }
    else
    {
        LogMessage(LogLevel::Error, "SDL not initialized.");
        return false;
    }

    return false; //This is not expected
}

bool EntryPoint::CreateWindow()
{
    return CreateWindow(ScreenWidth, ScreenHeight);
}

bool EntryPoint::HandleSDLEvents()
{
    /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
    /* more events on the event queue, our while loop will exit when */
    /* that occurs.                                                  */
    while (SDL_PollEvent(&event)){
        /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
        switch (event.type){
        
#pragma region Keyboard Events
        case SDL_KEYDOWN:
            LogMessage(LogLevel::Debug, "Key press detected\n");
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                gameState = GameState::End;
            }

            break;
        case SDL_KEYUP:
            LogMessage(LogLevel::Debug, "Key release detected\n");
            break;
#pragma endregion Keyboard Events

#pragma region Mouse Events
        case SDL_MOUSEBUTTONDOWN:
            LogMessage(LogLevel::Debug, "The mouse is at: %d x %d", event.button.x, event.button.y);
            break;
#pragma endregion Mouse Events

        case SDL_QUIT:
            gameState = GameState::End;
            break;
        default:
            break;
        }
    }

    return true;
}

bool EntryPoint::PaintWindowRGB(const ColorRGB color)
{
    if (sdl_initialized && window != nullptr)
    {
        //Get window surface 
        screenSurface = SDL_GetWindowSurface(window);

        //Fill the surface white 
        SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, color.R, color.G, color.B));

        //Update the surface 
        SDL_UpdateWindowSurface(window);
        return true;
    }
    else
    {
        LogMessage(LogLevel::Error, "SDL not initialized or window is NULL");
        return false;
    }
}

bool EntryPoint::OnQuit()
{
    //Destroy window 
    SDL_DestroyWindow(window);

    //Quit SDL subsystems 
    SDL_Quit();

    LogMessage(LogLevel::Debug, "Goodbye");
    return true;
}

bool EntryPoint::MainLoop()
{

    int i = 0;
    while (gameState !=  GameState::End)
    {
        HandleSDLEvents();

        i = (i + 1) % 255;
        ColorRGB color{ 255, i, 255 };
        PaintWindowRGB(color);

       

        SDL_Delay(100);
    }

    return true;
}

int main(int argc, char* args[])
{
    EntryPoint app;

    auto state = app.InitSDL();

    if (state)
    {
        app.CreateWindow();
        ColorRGB color{ 255, 0, 255 };
        app.PaintWindowRGB(color);

        app.MainLoop();

        //SDL_Delay(5000);

        app.OnQuit();
    }

    return true;
}