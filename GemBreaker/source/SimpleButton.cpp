#include "SimpleButton.h"


SimpleButton::SimpleButton(SDL_Renderer* pRenderer, SDL_Color pColor, Uint32 pX, Uint32 pY, Uint32 pW, Uint32 pH)
{
    rect = SDL_Rect{pX, pY, pW, pH};
    color = SDL_Color(pColor);

    btnTexture = new SimpleTexture(pRenderer);
    btnTexture->LoadFromFileRGB("resources/textures/BackTile_05.png", SDL_FALSE, nullptr);

    btnTextureHighlighted = new SimpleTexture(pRenderer);
    btnTextureHighlighted->LoadFromFileRGB("resources/textures/BackTile_06.png", SDL_FALSE, nullptr);
}


SimpleButton::~SimpleButton()
{
    delete btnTexture;
    delete btnTextureHighlighted;
    btnTexture = nullptr;
    btnTextureHighlighted = nullptr;
}

void SimpleButton::Render()
{
    if (highlighted)
    {
        btnTextureHighlighted->setColor(color);
        btnTextureHighlighted->Render(rect);
    }
    else
    {
        btnTexture->setColor(color);
        btnTexture->Render(rect);
    }
}
