#ifndef _BLOCK_H_
#define _BLOCK_H_

// SDL Include
#include <SDL2/SDL.h>

// Our includes
#include "DebugTools.h"

enum class BlockColour :Uint8 { Red = 0, Yellow, Green, Blue, Black, White, NumberOfValues };
enum class BlockType :Uint8 { Brick = 0, Bomb, Multiplier, NumberOfValues };

// The idea is to use the same texture and only tint it accordingly
class Block
{
public:
    Block(BlockColour pColor, BlockType pType, Uint32 pX, Uint32 pY, Uint32 pPoints, Uint32 pMult, SDL_Point *pOffsetPos);

    BlockColour color;
    BlockType type;
    SDL_Rect rect;

    // Column position
    Uint32 x;

    // Row position
    Uint32 y;

    // Table Offset
    SDL_Point *offset;

    // Amount of points this block gives
    Uint32 points;

    // The multiplier associated with this block
    Uint32 mult;

    // Size of block
    Uint32 bSize = 40;

    // Is highlighted
    bool highlighted = false;
    // Marked to Destroy

    bool marked = false;

    // Link number, stores the link associated with this block.
    // 0 <- no link
    Uint32 link = 0;
private:
    // Update rect
    void Update();

public:
    // Calculate color
    SDL_Color RetrieveColor();

    // Move to another col and row
    void MoveTo(Uint32 col, Uint32 row);

    // Returns true if it is clicked
    bool IsHover(SDL_Point& mousePos);

    // Render function
    void Render();

    // Print function (for debugging)
    void Print();
};

#endif /* _BLOCK_H_ */
