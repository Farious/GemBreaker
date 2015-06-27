#ifndef _SIMPLE_BUTTON_H_
#define _SIMPLE_BUTTON_H_

#include <SDL.h>
#include "DebugTools.h"

// Our includes
#include "SimpleTexture.h"


class SimpleButton
{
public:
    SimpleButton(SDL_Renderer* pRenderer, SDL_Color color, Uint32 x, Uint32 y, Uint32 w, Uint32 h);
    ~SimpleButton();

    SimpleTexture* btnTexture = nullptr;
    SimpleTexture* btnTextureHighlighted = nullptr;
    SDL_Rect rect;
    SDL_Color color;
    SDL_Renderer* pRenderer = nullptr;

    bool highlighted = false;

    void Render();
};

#endif /*_SIMPLE_BUTTON_H_*/