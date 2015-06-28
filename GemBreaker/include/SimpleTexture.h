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
    bool LoadFromFileRGB(const std::string& path, SDL_bool flag, const SDL_Color* keyColor);

    // Creates image from font string 
    bool LoadFromRenderedText(TTF_Font* font, const std::string& textureText, SDL_Color& textColor);

    // Deallocates texture;
    void Free();

    // Set color modulation 
    void setColor(SDL_Color& color);

    // Set blending 
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation 
    void setAlpha(Uint8 alpha);

    // Render the texture at specified position
    void Render(int x, int y);

    // Render the texture in the specified rectangle
    void Render(SDL_Rect& rect);

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