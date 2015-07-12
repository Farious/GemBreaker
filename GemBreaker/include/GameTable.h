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
#include <map>


// GemBreaker includes
#include "SimpleTexture.h"
#include "SimpleTimer.h"
#include "DebugTools.h"
#include "Block.h"
#include "SimpleButton.h"
#include "SimpleConstants.h"

// Defining the alias to help out readability, hash map on the coordinates
using Column = std::map < Uint32, Block* > ;
using Table = std::map < Uint32, Column* > ;
using Link = std::vector < Block* > ;
using LinkList = std::vector < Link > ;
using uDist = std::uniform_int_distribution < Uint32 > ;

enum class GameTableState :Uint8 { Running, Paused, Ended, InputDisabled };

class GameTable
{
public:
    GameTable(SDL_Renderer* pRenderer, const Uint32 w, const Uint32 h);
    ~GameTable();

    // Resets and Init Game
    void Init();

    void Update();

    void AddNewColumn();

    // To be called after 
    void UpdateTable();

    // Move column
    void MoveColumnsLeft();

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

    // Tick the table
    void Tick(SDL_Renderer* renderer);

    void RenderRunning();

    void RenderPause();

    void RenderUserInterface(SDL_Renderer* renderer);

    void BackgroundRenderer(SDL_Renderer* renderer);

    // Render text at position
    void RenderText(const std::string& text, SDL_Color& color, SDL_Point& pos);

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

    void ProcessInputOnBlocks(SDL_Point& mousePos, SDL_Event& event);

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
    Uint32 windowW = 0;
    Uint32 windowH = 0;

    // Table background texture
    SimpleTexture* backgroundTexture = nullptr;

    // Common block texture, used by each color
    SimpleTexture* blockTexture = nullptr;
    SimpleTexture* blockTextureHighlighted = nullptr;

    // Bomb texture
    SimpleTexture* bombTexture = nullptr;

    // Text font and texture
    const std::string fontName = FONT_NAME;
    TTF_Font* gameFont = nullptr;
    SimpleTexture* textTexture = nullptr;

    // Buttons
    SimpleTexture* btnAddFaceTexture = nullptr;
    SimpleButton* btnAddColumn = nullptr;
    SimpleButton* btnPauseGame = nullptr;

    // Game Timer
    SDL_Rect timerRect;
    SDL_Color timerFillColor;
    SDL_Color timerBGColor;

    SimpleTimer* timer = nullptr;

    // Animation timer
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

    // Score rendering settings
    SDL_Color scoreColor;
    SDL_Point scoreULPos;
    const std::string scoreText = "Score : ";

    // Level rendering settings
    SDL_Point levelULPos;
    const std::string levelText = "Level : ";

    // Screen shake variable
    SDL_Point screenShakeULPos;

    // Paused variables
    SDL_Point pauseULPos;
    SDL_Texture* pauseTexture = nullptr;
    const std::string pauseText = "PAUSED";

    // Experience rendering settings
    SDL_Point expULPos;
    SDL_Rect expRect;
    const std::string expText = "experience";


#pragma region Game Variables
    // Current score
    Uint32 score = 0;

    // Current experience
    Uint32 experience = 0;

    // Current level
    Uint32 level = 0;

    // Amount of time for the next column and timer based points
    const float timerBonusPoints = 50.f;
    const float timeToNextCol = 3000.f; // 3 seconds

    // Amount of experience needed to level up
    // Base experience needed expression: 
    //     ((crntLvl + 1) * 3 ) * baseExpToLevelUp 
    const Uint32 baseExpToLevelUp = 300;
    Uint32 expToLevelUp = baseExpToLevelUp;

    // Has the game ended
    bool ended = false;
    bool isPaused = false;
#pragma endregion Game Variables

#pragma region Game State
    GameTableState state = GameTableState::Running;
    GameTableState prevState = GameTableState::Running;
    
public:
    void TogglePause();
    void Pause();
    void Resume();
    void LoseFocus();
    void GainFocus();
#pragma endregion Game State
};

#endif /* _GAME_TABLE_H_ */