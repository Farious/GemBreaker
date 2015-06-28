#include "SimpleTexture.h"


SimpleTexture::SimpleTexture(SDL_Renderer* pRenderer)
{
    texture = nullptr;
    width = 0;
    height = 0;
    renderer = pRenderer;
}

SimpleTexture::~SimpleTexture()
{
    Free();
}

bool SimpleTexture::LoadFromFileRGB(const std::string& path, SDL_bool flag, const SDL_Color* keyColor)
{
    // Let's clean up first
    Free();

    // Temporary pointer to final texture
    SDL_Texture *newTexture = nullptr;

    // Loads the image at specified path
    SDL_Surface *loadedSurface = IMG_Load( path.c_str() );

    if (loadedSurface == nullptr)
    {
        LogMessage(LogLevel::Error, "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        Uint32 ret = 0;
        // Color key image
        if (flag == SDL_TRUE)
        {
            ret = SDL_SetColorKey(loadedSurface, flag, SDL_MapRGB(loadedSurface->format, keyColor->r, keyColor->g, keyColor->b));
        }
        
        if (ret != 0)
        {
            LogMessage(LogLevel::Error, "Unable to set color key! SDL_image Error: %s\n", IMG_GetError());
        }
        else
        {
            // Create texture from loaded surface
            newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

            if (newTexture == nullptr)
            {
                LogMessage(LogLevel::Error, "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
            }
            else
            {
                // Get image dimensions
                width = loadedSurface->w;
                height = loadedSurface->h;

                // Let's get rid of the surface
                SDL_FreeSurface(loadedSurface);

                // Setting the texture
                texture = newTexture;

                return texture != nullptr;
            }
        }
    }

    // Something went wrong
    return false;
}

bool SimpleTexture::LoadFromRenderedText(TTF_Font* font, const std::string& textureText, SDL_Color& textColor)
{
    //Get rid of preexisting texture 
    Free();

    //Render text surface 
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if (textSurface == nullptr)
    {
        LogMessage(LogLevel::Error, "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //Create texture from surface pixels 
        texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (texture == nullptr)
        {
            LogMessage(LogLevel::Error, "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //Get image dimensions 
            width = textSurface->w;
            height = textSurface->h;
        }

        //Get rid of old surface 
        SDL_FreeSurface(textSurface);
    }

    //Return success 
    return texture != nullptr;
}

void SimpleTexture::Free()
{
    // Check if texture exists
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);

        // Reseting values
        texture = nullptr;
        width = 0;
        height = 0;
    }
}

void SimpleTexture::setColor(SDL_Color& color)
{
    //Modulate texture 
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
}

void SimpleTexture::Render(int x, int y)
{
    SDL_Rect renderQuad{ x, y, width, height };
    SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
}

void SimpleTexture::Render(SDL_Rect& rect)
{
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

Uint32 SimpleTexture::getWidth()
{
    return width;
}

Uint32 SimpleTexture::getHeight()
{
    return height;
}
