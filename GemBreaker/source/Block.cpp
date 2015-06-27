#include "Block.h"

Block::Block(BlockColour pColor, BlockType pType, Uint32 pX, Uint32 pY, Uint32 pPoints, Uint32 pMult, SDL_Point *pOffsetPos)
{
    color = pColor;
    type = pType;
    x = pX;
    y = pY;
    points = pPoints;
    mult = pMult;
    offset = pOffsetPos;

    Update();
}

void Block::Update()
{
    rect = SDL_Rect{ x * bSize + offset->x, y * bSize + offset->y, bSize, bSize };
}

SDL_Color Block::RetrieveColor(BlockColour color)
{
    switch (color)
    {
    case BlockColour::Red:
        return SDL_Color{ 255, 0, 0, 255 };
        break;
    case BlockColour::Yellow:
        return SDL_Color{ 255, 255, 0, 255 };
        break;
    case BlockColour::Green:
        return SDL_Color{ 0, 255, 0, 255 };
        break;
    case BlockColour::Blue:
        return SDL_Color{ 0, 0, 255, 255 };
        break;
    case BlockColour::Black:
        return SDL_Color{ 0, 0, 0, 255 };
        break;
    case BlockColour::White:
        return SDL_Color{ 255, 255, 255, 255 };
        break;
    case BlockColour::NumberOfValues:
    default:
        return SDL_Color{ 255, 255, 255, 255 };
        break;
    }
}

void Block::MoveTo(Uint32 col, Uint32 row)
{
    x = col;
    y = row;

    Update();
}

bool Block::IsHover(SDL_Point& mousePos)
{
    return SDL_EnclosePoints(&mousePos, 1, &rect, nullptr) == SDL_TRUE;
}

void Block::Render()
{

}

void Block::Print()
{
    LogMessage(LogLevel::Debug, "Block:: Color  : %d", color);
    LogMessage(LogLevel::Debug, "Block:: Type   : %d", type);
    LogMessage(LogLevel::Debug, "Block:: (x, y) : (%d, %d)", x, y);
    LogMessage(LogLevel::Debug, "Block:: Points : %d", points);
    LogMessage(LogLevel::Debug, "Block:: Mult.  : %d", mult);
}