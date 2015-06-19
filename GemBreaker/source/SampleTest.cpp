#include <SDL.h> 
#include <stdio.h> 

//Screen dimension constants 
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class SampleApp
{
public:
	SampleApp();
	bool OnInit();
	bool OnQuit();

	int OnExecute();
private:
	//The window we'll be rendering to 
	SDL_Window* window = NULL; 

	//The surface contained by the window 
	SDL_Surface* screenSurface = NULL;
};

SampleApp::SampleApp()
{
}

bool SampleApp::OnInit()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0) 
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		//Create window 
		window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		}
		else 
		{ 
			//Get window surface 
			screenSurface = SDL_GetWindowSurface( window ); 
			//Fill the surface white 
			SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) ); 
			//Update the surface 
			SDL_UpdateWindowSurface( window ); 
			//Wait two seconds 
			SDL_Delay( 2000 ); 
		}
	}

	return true;
}

bool SampleApp::OnQuit()
{
	//Destroy window 
	SDL_DestroyWindow( window ); 

	//Quit SDL subsystems 
	SDL_Quit();

	return true;
}

int SampleApp::OnExecute()
{
	return OnInit();
}

int main1(int argc, char* args[])
{
	SampleApp app;

	auto state = app.OnExecute();
	
	if (state)
	{
		app.OnQuit();
	}
	else
	{
		// Something went very badly
		return false;
	}

	return true;
}