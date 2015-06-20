#ifndef _SIMPLE_TEXTURE_H
#define _SIMPLE_TEXTURE_H

// SDL includes
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Standard includes
#include <fstream>

// GemBreaker includes
#include "DebugTools.h"

class SimpleTexture
{
public:
    SimpleTexture(SDL_Renderer* pRenderer);
    ~SimpleTexture();

    // Loads images from file
    bool LoadFromFile(std::string path, const SDL_Color& keyColor);

    // Creates image from font string 
    bool LoadFromRenderedText(TTF_Font* font, std::string textureText, SDL_Color textColor);

    // Deallocates texture;
    void Free();

    // Set color modulation 
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending 
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation 
    void setAlpha(Uint8 alpha);

    // Render the texture at specified position
    //, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE)
    void Render(int x, int y);

    // Image dimension
    Uint32 getWidth();
    Uint32 getHeight();

private:
    // The actual texture
    SDL_Texture *texture;

    // Image dimensions
    Uint32 width = 0;
    Uint32 height = 0;

    // Rendered to be used
    SDL_Renderer* renderer;
};

#endif /*_SIMPLE_TEXTURE_H*/