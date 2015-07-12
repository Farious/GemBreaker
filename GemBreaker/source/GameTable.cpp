#include "GameTable.h"

GameTable::GameTable(SDL_Renderer* pRenderer, const Uint32 w, const Uint32 h)
{
    // Game window size
    windowW = w;
    windowH = h;

    // Game table initialization
    table = new Table();

    // Timer initialization
    Uint32 timerW = 100;
    Uint32 timerH = 10;
    Uint32 timerX = static_cast<Uint32>((windowW * 0.5) - (timerW * 0.5f));
    Uint32 timerY = 5;
    timerRect = SDL_Rect{ timerX, timerY, timerW, timerH };
    timerFillColor = SDL_Color{ 250, 104, 87, 255 };
    timerBGColor = SDL_Color{ 255, 207, 201, 255 };
    timer = new SimpleTimer();
    animTimer = new SimpleTimer();

    // Upper Left position
    ulPos = new SDL_Point{ 50, 300 };

    // Table Rectangle delimiter
    tableDelimiter = new SDL_Rect{ ulPos->x, ulPos->y - (rowNum - 1) * 40, colLimit * 40, rowNum * 40 };

    // Initialize randomization
    distributionForColumn = uDist(minColNum, rowNum);

    // At max baseMaxBlockColor colors at first
    distributionForBlockColor = uDist(0, baseMaxBlockColor);

    // At first only normal blocks
    distributionForBlockType = uDist(0, 0);

    // Random functions
    // Defining on the fly seed for the random engine
    auto seed = static_cast<Uint32>(std::chrono::system_clock::now().time_since_epoch().count());
    generator = std::default_random_engine(1);

    // Binding for ease of use
    randColNum = std::bind(distributionForColumn, generator);
    randBlockColor = std::bind(distributionForBlockColor, generator);
    randBlockType = std::bind(distributionForBlockType, generator);

    // Loading background textures
    backgroundTexture = new SimpleTexture(pRenderer);
    backgroundTexture->LoadFromFileRGB(TABLE_BG_TEX_NAME, SDL_FALSE, nullptr);

    // Loading block textures
    blockTexture = new SimpleTexture(pRenderer);
    blockTexture->LoadFromFileRGB(BLOCK_TEX_NAME, SDL_FALSE, nullptr);

    blockTextureHighlighted = new SimpleTexture(pRenderer);
    blockTextureHighlighted->LoadFromFileRGB(BLOCK_TEX_HL_NAME, SDL_FALSE, nullptr);

    // Loading font
    gameFont = TTF_OpenFont(fontName.c_str(), 20);
    textTexture = new SimpleTexture(pRenderer);

    // Creating buttons
    btnAddFaceTexture = new SimpleTexture(pRenderer);
    btnAddFaceTexture->LoadFromFileRGB(PLUS_TEX_HL_NAME, SDL_FALSE, nullptr);
    btnAddColumn = new SimpleButton(pRenderer, blockTexture, blockTextureHighlighted, btnAddFaceTexture, SDL_Color{ 255, 255, 255, 255 }, ulPos->x + ((colLimit - 1) * 40), ulPos->y + 40, 40, 40);
    btnPauseGame = new SimpleButton(pRenderer, blockTexture, blockTextureHighlighted, nullptr, SDL_Color{ 255, 255, 255, 255 }, ulPos->x - 40 - 5, ulPos->y - ((rowNum - 1) * 40), 40, 40);

    // Initialize Linked block lists
    blockLinks = LinkList();
    newLink = new Link();

    // Score, experience and pause rendering settings
    scoreColor = SDL_Color{ 0xFF, 0xE4, 0xD1, 0xFF };
    scoreULPos = SDL_Point{ 100, static_cast<Uint32>(windowH * 0.8f) };
    levelULPos = SDL_Point{ 100, static_cast<Uint32>(windowH * 0.8f) + 30 };

    // Calculate pause string width
    int* posW = new int();
    int* posH = new int();
    TTF_SizeText(gameFont, pauseText.c_str(), posW, posH);
    auto textSize = *posW;
    pauseULPos = SDL_Point{ static_cast<Uint32>((windowW - textSize) * 0.5f), static_cast<Uint32>(windowH * 0.5f - 100) };

    // Calculate experience string width
    TTF_SizeText(gameFont, expText.c_str(), posW, posH);
    textSize = *posW;
    expULPos = SDL_Point{ static_cast<Uint32>((windowW - textSize) * 0.5f), ulPos->y + 35 };
    expRect = SDL_Rect{ static_cast<Uint32>(windowW * 0.5f - 100), expULPos.y + *posH + 5, 200, 15 };
    delete posW;
    delete posH;

    // Initialize all the game variables
    Init();
}

GameTable::~GameTable()
{
    // Cleaning textures
    delete backgroundTexture;
    delete blockTexture;
    delete bombTexture;
    delete blockTextureHighlighted;
    delete textTexture;
    delete btnAddFaceTexture;
    backgroundTexture = nullptr;
    blockTexture = nullptr;
    blockTextureHighlighted = nullptr;
    bombTexture = nullptr;
    textTexture = nullptr;
    btnAddFaceTexture = nullptr;

    // Clear the font
    TTF_CloseFont(gameFont);
    gameFont = nullptr;

    // Clean the upper left position
    delete ulPos;
    ulPos = nullptr;

    // Clean timer
    delete timer;
    timer = nullptr;

    // Clean the Link pointer helper
    delete newLink;
    newLink = nullptr;

    // Destroy buttons
    delete btnAddColumn;
    delete btnPauseGame;
    btnAddColumn = nullptr;
    btnPauseGame = nullptr;

    // Clean table
    ClearTable();
    delete table;
    table = nullptr;
}

void GameTable::Init()
{
    ClearTable();

    score = 0;
    experience = 0;
    level = 0;
    expToLevelUp = baseExpToLevelUp;
    ended = false;

    // Generate the table
    GenerateTable();
    //UpdateTable();

    // Calculate all linked blocks
    CalcLinkedBlocks();

    // Reset timer
    timer->start();
}

void GameTable::Update()
{
    // We only do the animation update if the animation timer is ticking
    if (animTimer->IsStarted())
    {
        // SCREEN SHAKE
        screenShakeULPos = SDL_Point{ randColNum(), randColNum() };
        tableDelimiter->x = ulPos->x + screenShakeULPos.x;
        tableDelimiter->y = ulPos->y + screenShakeULPos.y - (rowNum - 1) * 40;

        // Let's check if it is time to stop animation
        if (animTimer->getTicks() > animSeconds)
        {
            animTimer->stop();
            timer->start();

            // Let's get back to normal
            state = GameTableState::Running;

            // Reposition table
            screenShakeULPos = SDL_Point{ 0, 0 };
            tableDelimiter->x = ulPos->x;
            tableDelimiter->y = ulPos->y - (rowNum - 1) * 40;
        }
    }

    if (timer->getTicks() > timeToNextCol)
    {
        if (table->size() == colLimit)
        {
            state = GameTableState::Ended;
        }
        else
        {
            timer->start();
            AddNewColumn();
        }
    }

    // Check game end state
    if (state == GameTableState::Ended)
    {
        timer->stop();
        table->clear();
    }
}

void GameTable::AddNewColumn()
{
    if (state == GameTableState::Running)
    {
        // Clear blocks linked state
        ClearBlocks(true);
        // Move blocks to the left
        MoveColumnsLeft();
        // Add the new column
        table->insert(std::make_pair(colLimit - 1, GenerateColumn(colLimit - 1)));
        // Recalculate linked state
        CalcLinkedBlocks();
    }
}

void GameTable::UpdateTable()
{
    Uint32 lastRow = 0, lastCol = colLimit - 1;
    Column colMap;
    bool shouldSwap = false;
    bool shouldMoveCol = false;

    // Let's traverse this in the reverse order
    for (auto rit = table->rbegin(); rit != table->rend(); ++rit)
    {
        auto col = rit->first;
        auto colRow = rit->second;

        shouldMoveCol = lastCol != col;
        shouldSwap = false;
        colMap.clear();
        lastRow = 0;
        // For each block in the current column
        for (auto& p2 : *colRow)
        {
            auto row = p2.first;
            auto block = p2.second;

            auto finalRow = (lastRow != row) ? lastRow : row;
            auto finalCol = (lastCol != col) ? lastCol : col;
            if (finalRow != row || finalCol != col)
            {
                // If there is a gap, then recreate the pair
                block->MoveTo(finalCol, finalRow);
                // Generate the pair to populate the column
                auto pair = std::make_pair(finalRow, block);

                // Insert pair
                colMap.insert(pair);
                shouldSwap = true;
            }
            else
            {
                // If there is no gap, then maintain the pair
                // Insert pair
                colMap.insert(p2);
            }

            lastRow++;
        }

        if (shouldSwap)
        {
            colRow->swap(colMap);
        }

        if (shouldMoveCol)
        {
            table->erase(col);
            table->insert(std::make_pair(lastCol, colRow));
        }

        lastCol--;
    }

    // Clean up
    colMap.clear();
}

void GameTable::MoveColumnsLeft()
{
    Table swapTable;

    if (table->size() < colLimit)
    {
        for (auto& p1 : *table)
        {
            auto col = p1.first;
            auto colRow = p1.second;

            if (col > 0)
            {
                for (auto& p2 : *colRow)
                {
                    auto row = p2.first;
                    auto block = p2.second;

                    block->MoveTo(col - 1, row);
                }

                auto pair = std::make_pair(col - 1, colRow);
                swapTable.insert(pair);
            }
        }

        table->swap(swapTable);
        swapTable.clear();
    }
}

void GameTable::TogglePause()
{
    if (isPaused)
    {
        isPaused = false;
        state = prevState;

        Resume();
    }
    else
    {
        isPaused = true;
        prevState = state;
        state = GameTableState::Paused;

        Pause();
    }
}

void GameTable::Pause()
{
    timer->pause();
    animTimer->pause();
    LogMessage(LogLevel::Debug, "Pause");
}

void GameTable::Resume()
{
    if (state == GameTableState::Running)
    {
        timer->unpause();
        animTimer->unpause();
    }
    LogMessage(LogLevel::Debug, "Resume");
}

void GameTable::LoseFocus()
{
    if (!isPaused)
    {
        prevState = state;
        state = GameTableState::Paused;

        Pause();
    }
    LogMessage(LogLevel::Debug, "Lost focus");
}

void GameTable::GainFocus()
{
    if (!isPaused)
    {
        state = prevState;

        Resume();
    }
    LogMessage(LogLevel::Debug, "Regained focus");
}

void GameTable::ClearTable()
{
    // For each row
    for (auto& p1 : *table)
    {
        auto col = p1.first;
        auto colRow = p1.second;

        // For each block in the current column
        for (auto& p2 : *colRow)
        {
            auto row = p2.first;
            auto block = p2.second;

            delete block;
        }

        // Clean up the entire column 
        // This is not necessary as the table->clear() 
        // should call the clear of every item (the delete)
        colRow->clear();
        delete colRow;
    }

    // Now let's clean up the hash
    table->clear();
}

Column* GameTable::GenerateColumn(Uint32 col)
{
    Column* iCol = new Column();

    // The number of blocks to create
    auto rows = rowNum;//randColNum();

    for (Uint32 row = 0; row < rows; row++)
    {
        // Generate new block
        auto bColour = static_cast<BlockColour>(randBlockColor());
        auto bType = static_cast<BlockType>(randBlockType());
        Block* block = new Block(bColour, bType, col, row, 1, 1, ulPos);

        // Generate the pair to populate the column
        auto pair = std::make_pair(row, block);

        // Insert pair
        iCol->insert(pair);
    }

    return iCol;
}

void GameTable::GenerateTable()
{
    for (Uint32 col = colLimit - initColNum; col < colLimit; col++)
    {
        Column* iCol = GenerateColumn(col);

        auto pair = std::make_pair(col, iCol);

        table->insert(pair);
    }
}

void GameTable::ClearBlocks(bool clearLink)
{
    for (auto& p1 : *table)
    {
        auto col = p1.first;
        auto colRow = p1.second;

        // For each block in the current column
        for (auto& p2 : *colRow)
        {
            auto row = p2.first;
            auto block = p2.second;
            block->highlighted = false;
            block->marked = false;

            if (clearLink)
            {
                block->link = 0;
            }
        }
    }
}

void GameTable::CalcLinkedBlocks()
{
    blockLinks.clear();
    Uint32 countLinks = 1;

    for (auto& p1 : *table)
    {
        auto col = p1.first;
        auto colRow = p1.second;

        // For each block in the current column
        for (auto& p2 : *colRow)
        {
            auto row = p2.first;
            auto block = p2.second;

            if (block->link == 0)
            {
                newLink->clear();
                MarkBlocks(block, countLinks);
                if (newLink->size() > 1)
                {
                    countLinks++;
                    blockLinks.push_back(Link(*newLink));
                }
                else
                {
                    block->link = 0;
                }
                newLink->clear();
            }
        }
    }
}

void GameTable::MarkBlocks(Block *block, bool highlight, bool destroy)
{
    auto linkNr = block->link;

    for (auto& p1 : *table)
    {
        auto col = p1.first;
        auto colRow = p1.second;

        // For each block in the current column
        for (auto& p2 : *colRow)
        {
            auto row = p2.first;
            auto block = p2.second;
            block->highlighted = false;
            block->marked = false;
        }
    }

    if (linkNr == 0)
    {
        // We are not going to do anything if it is a single block
        return;
    }
    else
    {
        auto blocks = blockLinks[linkNr - 1];

        for (auto iBlock : blocks)
        {
            if (destroy)
            {
                iBlock->marked = true;
            }
            else if (highlight)
            {
                iBlock->highlighted = true;
            }
        }
    }
}

void GameTable::MarkBlocks(Block *block, Uint32 linkNr)
{
    auto nI = block->x + 1;
    auto cI = block->x;
    auto pI = block->x - 1;

    auto nJ = block->y + 1;
    auto cJ = block->y;
    auto pJ = block->y - 1;

    block->link = linkNr;
    block->highlighted = false;
    block->marked = false;
    newLink->push_back(block);

    if (cI > 0)
    {
        auto nCol = table->find(pI);
        if (nCol != table->end())
        {

            auto nBlock = nCol->second->find(cJ);
            if (nBlock != nCol->second->end())
            {
                if (block->color == nBlock->second->color && nBlock->second->link == 0)
                {
                    MarkBlocks(nBlock->second, linkNr);
                }
            }
        }
    }

    if (nI < colLimit)
    {
        auto nCol = table->find(nI);
        if (nCol != table->end())
        {
            auto nBlock = nCol->second->find(cJ);
            if (nBlock != nCol->second->end())
            {
                if (block->color == nBlock->second->color && nBlock->second->link == 0)
                {
                    MarkBlocks(nBlock->second, linkNr);
                }
            }
        }
    }

    if (cJ > 0)
    {
        auto nCol = table->find(cI);
        if (nCol != table->end())
        {
            auto nBlock = nCol->second->find(pJ);
            if (nBlock != nCol->second->end())
            {
                if (block->color == nBlock->second->color && nBlock->second->link == 0)
                {
                    MarkBlocks(nBlock->second, linkNr);
                }
            }
        }
    }

    if (nJ < rowNum)
    {
        auto nCol = table->find(cI);
        if (nCol != table->end())
        {
            auto nBlock = nCol->second->find(nJ);
            if (nBlock != nCol->second->end())
            {
                if (block->color == nBlock->second->color && nBlock->second->link == 0)
                {
                    MarkBlocks(nBlock->second, linkNr);
                }
            }
        }
    }
}

void GameTable::Tick(SDL_Renderer* renderer)
{

    // Background Rendering
    BackgroundRenderer(renderer);

    // Only render the table's content when not paused
    switch (state)
    {
    case GameTableState::Running:
        RenderRunning();
        break;
    case GameTableState::Paused:
        RenderPause();
        break;
    case GameTableState::Ended:
        break;
    case GameTableState::InputDisabled:
        RenderRunning();
        break;
    default:
        break;
    }

    Update();

    // UI Drawing
    RenderUserInterface(renderer);
}

void GameTable::RenderRunning()
{
    for (auto& p1 : *table)
    {
        auto col = p1.first;
        auto colRow = p1.second;

        // For each block in the current column
        for (auto& p2 : *colRow)
        {
            auto row = p2.first;
            auto block = p2.second;
            auto color = block->RetrieveColor();

            if (block->highlighted)
            {
                blockTextureHighlighted->setColor(color);
                auto rect = block->rect;
                rect.x += screenShakeULPos.x;
                rect.y += screenShakeULPos.y;
                blockTextureHighlighted->Render(rect);
            }
            else
            {
                blockTexture->setColor(color);
                auto rect = block->rect;
                rect.x += screenShakeULPos.x;
                rect.y += screenShakeULPos.y;
                blockTexture->Render(rect);
            }
        }
    }
}

void GameTable::RenderPause()
{
    // Score drawing
    RenderText(pauseText, scoreColor, pauseULPos);
}

void GameTable::RenderUserInterface(SDL_Renderer* renderer)
{
    // Game timer drawing
    // Full background timer rectangle
    SDL_SetRenderDrawColor(renderer, timerBGColor.r, timerBGColor.g, timerBGColor.b, timerBGColor.a);
    SDL_RenderFillRect(renderer, &timerRect);

    // Amount of filled rect to render
    SDL_SetRenderDrawColor(renderer, timerFillColor.r, timerFillColor.g, timerFillColor.b, timerFillColor.a);
    auto amount = timer->getTicks() / timeToNextCol;
    auto fillRect = SDL_Rect{ timerRect.x, timerRect.y, static_cast<Uint32>(timerRect.w * amount), timerRect.h };
    SDL_RenderFillRect(renderer, &fillRect);

    // Score drawing
    RenderText(scoreText + std::to_string(score), scoreColor, scoreULPos);

    // Level drawing
    RenderText(levelText + std::to_string(level), scoreColor, levelULPos);

    // Experience drawing
    RenderText(expText, scoreColor, expULPos);

    // Full background Experience rectangle
    SDL_SetRenderDrawColor(renderer, timerBGColor.r, timerBGColor.g, timerBGColor.b, timerBGColor.a);
    SDL_RenderFillRect(renderer, &expRect);

    // Amount of filled rect to render - Experience
    SDL_SetRenderDrawColor(renderer, timerFillColor.r, timerFillColor.g, timerFillColor.b, timerFillColor.a);
    amount = static_cast<float>(experience) / static_cast<float>(expToLevelUp);
    fillRect = SDL_Rect{ expRect.x, expRect.y, static_cast<Uint32>(expRect.w * amount), expRect.h };
    SDL_RenderFillRect(renderer, &fillRect);

    btnAddColumn->Render();
    btnPauseGame->Render();
}

void GameTable::BackgroundRenderer(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0x24, 0x2B, 0x21, 0xFF);
    SDL_RenderFillRect(renderer, tableDelimiter);
    SDL_SetRenderDrawColor(renderer, 0x78, 0x3A, 0x0E, 0xFF);
    SDL_RenderDrawRect(renderer, tableDelimiter);
}

void GameTable::RenderText(const std::string& text, SDL_Color& color, SDL_Point& pos)
{
    textTexture->LoadFromRenderedText(gameFont, text, color);
    textTexture->Render(pos.x, pos.y);
}

void GameTable::CalcNextLevelExperience()
{
    expToLevelUp = ((level + 1) * 3) * baseExpToLevelUp;
}

void GameTable::LevelUp()
{
    // Level Up
    level++;
    experience -= expToLevelUp;
    CalcNextLevelExperience();

    /*
    // This will take into account the level of the table
    distributionForBlockColor = uDist(0, std::min(static_cast<Uint32>(BlockColour::NumberOfValues), level));
    distributionForBlockType = uDist(0, std::min(static_cast<Uint32>(BlockType::NumberOfValues), level));

    // Update random functions
    randBlockColor = std::bind(distributionForBlockColor, generator);
    randBlockType = std::bind(distributionForBlockType, generator);
    */
}

void GameTable::HandleSDLMouseEvent(SDL_Event& event)
{
    if (state != GameTableState::Running && state != GameTableState::Paused)
    {
        ClearBlocks(false);
        return;
    }

    // Clear button state
    btnAddColumn->highlighted = false;
    btnPauseGame->highlighted = false;

    // Mouse position
    auto mousePos = SDL_Point{ event.button.x, event.button.y };

    // Clear highlight state
    ClearBlocks(false);

    if (SDL_EnclosePoints(&mousePos, 1, tableDelimiter, nullptr) == SDL_TRUE)
    {
        // We are over the table
        if (state == GameTableState::Running)
        {
            ProcessInputOnBlocks(mousePos, event);
        }
    }
    else if (SDL_EnclosePoints(&mousePos, 1, &(btnAddColumn->rect), nullptr) == SDL_TRUE)
    {
        // We are over the add new column button
        btnAddColumn->highlighted = true;

        // We check if we can add a new column
        if (event.button.state == SDL_PRESSED && table->size() < colLimit)
        {
            // Add the new column
            AddNewColumn();
            // Restart the timer for the new column
            timer->start();
        }
    }
    else if (SDL_EnclosePoints(&mousePos, 1, &(btnPauseGame->rect), nullptr) == SDL_TRUE)
    {
        // We are over the pause button
        btnPauseGame->highlighted = true;
        if (event.button.state == SDL_PRESSED)
        {
            // Let's pause the game
            TogglePause();
        }
    }
}

void GameTable::ProcessInputOnBlocks(SDL_Point& mousePos, SDL_Event& event)
{
    bool found = false;
    bool remove = false;
    Uint32 linkNR = 0;
    for (auto& p1 : *table)
    {
        auto col = p1.first;
        auto colRow = p1.second;

        // For each block in the current column
        for (auto& p2 : *colRow)
        {
            auto row = p2.first;
            auto block = p2.second;
            found = block != nullptr && block->IsHover(mousePos);

            if (found)
            {
                if (event.button.state == SDL_PRESSED)
                {
                    MarkBlocks(block, true, true);
                    linkNR = block->link;
                    remove = true;
                }
                else
                {
                    MarkBlocks(block, true, false);
                }

                break;
            }
        }

        if (found)
        {
            break;
        }
    }

    if (!found)
    {
        ClearBlocks(false);
    }

    if (remove && linkNR > 0)
    {
        GatherPoints(linkNR);
        RemoveBlocksAndUpdateTable(linkNR);
    }
}

void GameTable::GatherPoints(Uint32 linkNr)
{
    Uint32 points = 0;
    Uint32 multiplier = 0;
    for (auto blockPos : blockLinks[linkNr - 1])
    {
        auto block = table->at(blockPos->x)->at(blockPos->y);
        points += block->points;
        multiplier += block->mult;
    }

    auto bonusPoints = static_cast<Uint32>(timerBonusPoints * timer->getTicks() / timeToNextCol);
    score += points * multiplier + bonusPoints;

    experience += points * multiplier + bonusPoints;
}

void GameTable::RemoveBlocksAndUpdateTable(Uint32 linkNr)
{
    // Let's check if we should level up
    if (experience > expToLevelUp)
    {
        LevelUp();
        ClearTable();
    }
    else
    {
        // Let's stop the timer
        timer->stop();

        for (auto blockPos : blockLinks[linkNr - 1])
        {
            auto col = table->at(blockPos->x);
            col->erase(blockPos->y);

            if (col->size() == 0)
            {
                table->erase(blockPos->x);
                delete col;
            }

            delete blockPos;
        }

        UpdateTable();

        ClearBlocks(true);
        CalcLinkedBlocks();

        // Begin destruction animation
        animTimer->start();
        state = GameTableState::InputDisabled;
    }
}
