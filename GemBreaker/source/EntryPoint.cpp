#include "EntryPoint.h"


EntryPoint::EntryPoint()
{
    sdl_initialized = false;
    window = nullptr;
    screenSurface = nullptr;
    game = nullptr;

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

        //Initialize PNG loading 
        int imgFlags = IMG_INIT_PNG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            LogMessage(LogLevel::Error, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            return false;
        }

        //Initialize SDL_ttf 
        if (TTF_Init() == -1)
        {
            LogMessage(LogLevel::Error, "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
            return false;
        }

        font = TTF_OpenFont(fontName.c_str(), 10);
        if (font == nullptr)
        {
            LogMessage(LogLevel::Error, "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
            return false;
        }

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

bool EntryPoint::CreateWindow(const Uint32 W, const Uint32 H)
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

        //Create renderer for window 
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr)
        {
            LogMessage(LogLevel::Error, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
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

void EntryPoint::Start()
{
    // Initializes SDL
    auto state = InitSDL();

    if (state)
    {
        CreateWindow();

        // Initializes GameTable
        game = new GameTable(renderer);

        MainLoop();

        OnQuit();
    }
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
            
            // Let's pass this event down to the GameTable
            game->HandleSDLMouseEvent(event);
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

bool EntryPoint::PaintWindowRGB(const SDL_Color color)
{
    if (sdl_initialized && window != nullptr)
    {
        //Get window surface 
        screenSurface = SDL_GetWindowSurface(window);

        //Fill the surface white 
        SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, color.r, color.g, color.b));

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
    // Destroy font
    delete fpsTex;
    fpsTex = nullptr;
    TTF_CloseFont(font);
    font = nullptr;

    // Destroy window 
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;
    renderer = nullptr;

    // Destroy the GameTable
    delete game;

    // Quit SDL subsystems 
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    LogMessage(LogLevel::Debug, "Goodbye");
    return true;
}

void EntryPoint::CalculateAverageFPS()
{
    avgFPS = frameCount / (fpsTimer.getTicks() / 1000.f);
    if (avgFPS > 2000000)
    {
        avgFPS = 0;
    }
}

void EntryPoint::DrawFramerate()
{
    fpsTex->LoadFromRenderedText(font, std::to_string(avgFPS), SDL_Color{ 255, 0, 0, 0 });
    fpsTex->Render(0, 0);
}

bool EntryPoint::MainLoop()
{
    // Let's start the FPS timer
    fpsTimer.start();

    // Setup FPS texture
    fpsTex = new SimpleTexture(renderer);

    // TODO - Remove this
    SimpleTexture bgTex(renderer);//../../External/KenneyNL/PNG/Balls/Black/
    auto rOk = bgTex.LoadFromFileRGB("ballBlack_10.png", SDL_FALSE, nullptr);

    if (!rOk)
    {
        gameState = GameState::End;
        return false;
    }
    // TODO - end remove this

    Uint32 i = 0;
    while (gameState != GameState::End)
    {
        // Frame rate timer cap
        capTimer.start();

        // Handling all SDL events
        HandleSDLEvents();

        // Clears the renderer
        SDL_RenderClear(renderer);

        // Update FrameRate counter
        CalculateAverageFPS();

        bgTex.Render(i++ % ScreenWidth, 0);

        game->Render(renderer);

        // FPS Drawing
        DrawFramerate();

        // Update frame
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderPresent(renderer);
        frameCount++;

        // If frame finished early 
        Uint32 frameTicks = capTimer.getTicks();
        if (frameTicks < ScreenTicksPerFrame)
        {
            //Wait remaining time 
            SDL_Delay(ScreenTicksPerFrame - frameTicks);
        }
    }

    return true;
}

int main(int argc, char* args[])
{
    EntryPoint app;

    app.Start();

    return true;
}