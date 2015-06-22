#ifndef _GAME_TABLE_H_
#define _GAME_TABLE_H_

// Common includes
#include <SDL.h>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <chrono>
#include <functional>

// GemBreaker includes
#include "SimpleTexture.h"
#include "SimpleTimer.h"
#include "DebugTools.h"
#include "Block.h"


#pragma region Block Definition

#pragma endregion Block Definition

// Defining the alias to help out readability, hash map on the coordinates
using Column = std::unordered_map < Uint32, Block* > ;
using Table = std::unordered_map < Uint32, Column* > ;
using uDist = std::uniform_int_distribution < Uint32 > ;

class GameTable
{
public:
    GameTable(SDL_Renderer* pRenderer);
    ~GameTable();

    // Resets and Init Game
    void Init();

#pragma region Table Generation Functions
    // Clears table
    void ClearTable();

    // Generate a new column
    Column* GenerateColumn(Uint32 col);

    // Generate new table
    void GenerateTable();

    // Add new column
    void AddColumn();
#pragma endregion Table Generation Functions

    // Render the table
    void Render(SDL_Renderer* renderer);

    // Calculate experience for next level
    void CalcNextLevelExperience();

    // Process a level up
    void LevelUp();

#pragma region Getters
    // Get current score
    const Uint32 GetScore();

    // Get current level
    const Uint32 GetLevel();

    // Get current experience in [0, 1[
    const Uint32 GetExperience();
#pragma endregion Getters

    // Event handler
    void HandleSDLMouseEvent(SDL_Event& event);

private:
    /*
      Game table's actual table.
      [Row][Col] -> Block*
      EndLimit ->  [0][*]
      ...
      InitLimit -> [initColNum][*]
      ...
      ColLimit ->  [colLimit][*]
      */
    Table* table = nullptr;
    Uint32 rowNum = 8;
    Uint32 initColNum = 6;
    Uint32 colLimit = 18;
    Uint32 minColNum = 3;
    Uint32 baseMaxBlockColor = 3;

    // Common block texture, used by each color
    SimpleTexture* blockTexture = nullptr;

    // Bomb texture
    SimpleTexture* bombTexture = nullptr;

    // Timer
    SimpleTimer* timer;

    // Table screen position (upper left corner)
    SDL_Point ulPos;

    // Simple random generator
    std::default_random_engine generator;
    uDist distributionForColumn;
    uDist distributionForBlockColor;
    uDist distributionForBlockType;
    std::function<Uint32()> randColNum;
    std::function<Uint32()> randBlockColor;
    std::function<Uint32()> randBlockType;

#pragma region Game Variables
    // Current score
    Uint32 score = 0;

    // Current experience
    Uint32 experience = 0;

    // Current level
    Uint32 level = 0;

    // Amount of time for the next column
    const float timeToNextCol = 5000.f; // 5 seconds

    // Amount of experience needed to level up
    // Base experience needed expression: 
    //     ((crntLvl + 1) ^ 2 ) * baseExpToLevelUp 
    const Uint32 baseExpToLevelUp = 10;
    Uint32 expToLevelUp = 10;

    // Has the game ended
    bool ended = false;
#pragma endregion Game Variables
};

#endif /* _GAME_TABLE_H_ */