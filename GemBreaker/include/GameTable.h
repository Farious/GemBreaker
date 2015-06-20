#ifndef _GAME_TABLE_H_
#define _GAME_TABLE_H_

// Common includes
#include <SDL.h>
#include <unordered_map>

// GemBreaker includes
#include "SimpleTexture.h"
#include "SimpleTimer.h"
#include "DebugTools.h"


#pragma region Block Definition
enum class BlockColour :Uint8 { Red, Yellow, Green, Blue, Black, White };
enum class BlockType :Uint8 { Brick, Bomb, Multiplier };

// The idea is to use the same texture and only tint it accordingly
struct Block
{
    BlockColour color;
    BlockType type;
    Uint32 x;
    Uint32 y;
    Uint32 mult;
};
#pragma endregion Block Definition

// Defining the alias to help out readability, hash map on the coordinates
using Table = std::unordered_map < Uint32, std::unordered_map < Uint32, Block* > > ;

class GameTable
{
public:
    GameTable();
    ~GameTable();

private:
    /*
     * Game table's actual table.
     * [Row][Col] -> Block*
     */
    Table* table = nullptr;
    Uint32 rowNum = 8;
    Uint32 colNum = 8;
    Uint32 rowLimit = 12;
    
    // Common block texture, used by each color
    SimpleTexture* blockTexture = nullptr;

    // Bomb texture
    SimpleTexture* bombTexture = nullptr;

    // Timer
    SimpleTimer* timer;
};

#endif /* _GAME_TABLE_H_ */