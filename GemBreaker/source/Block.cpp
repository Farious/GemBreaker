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
    rect = SDL_Rect{ offset->x + x * bSize, offset->y - y * bSize, bSize, bSize };
}

SDL_Color Block::RetrieveColor()
{
    switch (color)
    {
    case BlockColour::Red:
        return SDL_Color{ 0xD6, 0x51, 0x4F, 0xFF };
        break;
    case BlockColour::Yellow:
        return SDL_Color{ 0xD6, 0xAB, 0x4F, 0xFF };
        break;
    case BlockColour::Green:
        return SDL_Color{ 0x91, 0xD6, 0x4F, 0xFF };
        break;
    case BlockColour::Blue:
        return SDL_Color{ 0x4F, 0x7A, 0xD6, 0xFF };
        break;
    case BlockColour::Black:
        return SDL_Color{ 0x0, 0x0, 0x0, 0xFF };
        break;
    case BlockColour::White:
        return SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF };
        break;
    case BlockColour::NumberOfValues:
    default:
        return SDL_Color{ 0xFF, 0xFF, 0xFF, 0xFF };
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