#ifndef _GAME_TABLE_H_
#define _GAME_TABLE_H_

// Common includes
#include <SDL.h>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <chrono>
#include <functional>
#include <string>


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
using Link = std::vector < Block* > ;
using LinkList = std::vector < Link > ;
using uDist = std::uniform_int_distribution < Uint32 > ;

enum class GameTableState :Uint8 { Running, Paused, Ended, InputDisabled };

class GameTable
{
public:
    GameTable(SDL_Renderer* pRenderer);
    ~GameTable();

    // Resets and Init Game
    void Init();

    void Update();

#pragma region Table Generation Functions
    // Clears table
    void ClearTable();

    // Generate a new column
    Column *GenerateColumn(Uint32 col);

    // Generate new table
    void GenerateTable();

    // Add new column
    void AddColumn();
#pragma endregion Table Generation Functions

    // Clear Marked blocks
    void ClearBlocks(bool clearLink);

    // Calculate all linked blocks
    void CalcLinkedBlocks();

    // Destroy blocks
    void MarkBlocks(Block *block, bool highlight, bool destroy);

    // Destroy blocks
    void MarkBlocks(Block *block, Uint32 linkNr);

    // Render the table
    void Render(SDL_Renderer* renderer);

    // Render text at position
    void RenderText(std::string text, SDL_Color color, SDL_Point pos);

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

    // Given an linked group of blocks calculate the 
    // points it will give and update the score, level and
    // experience
    void GatherPoints(Uint32 linkNr);

    // Given an linked group of blocks destroy them from the table
    void RemoveBlocksAndUpdateTable(Uint32 linkNr);

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
    SimpleTexture* blockTextureHighlighted = nullptr;

    // Bomb texture
    SimpleTexture* bombTexture = nullptr;

    // Text font and texture
    const std::string fontName = "resources/OpenSans-Bold.ttf";
    TTF_Font *gameFont = nullptr;
    SimpleTexture* textTexture = nullptr;

    // Timer
    SimpleTimer* timer = nullptr;
    SimpleTimer* animTimer = nullptr;
    Uint32 animSeconds = static_cast<Uint32>(0.1f * 1000);

    // Table screen position (upper left corner)
    SDL_Point* ulPos = nullptr;
    SDL_Rect* tableDelimiter = nullptr;

    // Simple random generator
    std::default_random_engine generator;
    uDist distributionForColumn;
    uDist distributionForBlockColor;
    uDist distributionForBlockType;
    std::function<Uint32()> randColNum;
    std::function<Uint32()> randBlockColor;
    std::function<Uint32()> randBlockType;

    LinkList blockLinks;
    Link* newLink = nullptr;
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

#pragma region Game State
    GameTableState state = GameTableState::Running;

public:
    void Pause();
    void Resume();
#pragma endregion Game State
};

#endif /* _GAME_TABLE_H_ */