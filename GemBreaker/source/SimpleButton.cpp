#include "SimpleButton.h"


SimpleButton::SimpleButton(SDL_Renderer* pRenderer, SimpleTexture* pBtnTexture, SimpleTexture* pBtnHLTexture, SimpleTexture* pFaceTexture, SDL_Color pColor, Uint32 pX, Uint32 pY, Uint32 pW, Uint32 pH)
{
    rect = SDL_Rect{ pX, pY, pW, pH };
    color = SDL_Color(pColor);

    btnTexture = pBtnTexture;

    btnTextureHighlighted = pBtnHLTexture;

    btnFaceTexture = pFaceTexture;
}


SimpleButton::~SimpleButton()
{

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

    if (btnFaceTexture != nullptr)
    {
        btnFaceTexture->setColor(SDL_Color{ 0xAF, 0xAF, 0xAF, 0xFF });
        btnFaceTexture->Render(SDL_Rect{ rect.x, rect.y, rect.w - 1, rect.h - 1 });
    }
}
