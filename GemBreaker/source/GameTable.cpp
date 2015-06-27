#include "GameTable.h"

GameTable::GameTable(SDL_Renderer* pRenderer)
{
    // Game table initialization
    table = new Table();

    // Timer initialization
    timer = new SimpleTimer();
    animTimer = new SimpleTimer();

    // Upper Left position
    ulPos = new SDL_Point{ 50, 50 };

    // Table Rectangle delimiter
    tableDelimiter = new SDL_Rect{ ulPos->x, ulPos->y, colLimit * 40, rowNum * 40 };

    // Initialize randomization
    distributionForColumn = uDist(minColNum, rowNum);

    // At max baseMaxBlockColor colors at first
    distributionForBlockColor = uDist(0, baseMaxBlockColor);

    // At first only normal blocks
    distributionForBlockType = uDist(0, 0);

    // Random functions
    // Defining on the fly seed for the random engine
    auto seed = static_cast<Uint32>(std::chrono::system_clock::now().time_since_epoch().count());
    generator = std::default_random_engine(seed);

    // Binding for ease of use
    randColNum = std::bind(distributionForColumn, generator);
    randBlockColor = std::bind(distributionForBlockColor, generator);
    randBlockType = std::bind(distributionForBlockType, generator);

    // Loading block textures
    blockTexture = new SimpleTexture(pRenderer);
    blockTexture->LoadFromFileRGB("resources/textures/BackTile_05.png", SDL_FALSE, nullptr);

    blockTextureHighlighted = new SimpleTexture(pRenderer);
    blockTextureHighlighted->LoadFromFileRGB("resources/textures/BackTile_06.png", SDL_FALSE, nullptr);

    // Loading font
    gameFont = TTF_OpenFont(fontName.c_str(), 20);
    textTexture = new SimpleTexture(pRenderer);

    // Initialize Linked block lists
    blockLinks = LinkList();
    newLink = new Link();

    // Initialize all the game variables
    Init();
}


GameTable::~GameTable()
{
    // Cleaning textures
    delete blockTexture;
    delete bombTexture;
    delete blockTextureHighlighted;
    delete textTexture;
    blockTexture = nullptr;
    blockTextureHighlighted = nullptr;
    bombTexture = nullptr;
    textTexture = nullptr;

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
        RenderText(std::to_string(animTimer->getTicks()), SDL_Color{ 255, 255, 255, 255 }, SDL_Point{ 0, 200 });

        if (animTimer->getTicks() > animSeconds)
        {
            animTimer->stop();

            // Let's get back to normal
            state = GameTableState::Running;
        }
    }
}

void GameTable::Pause()
{
    state = GameTableState::Paused;
    LogMessage(LogLevel::Debug, "Pause");
}

void GameTable::Resume()
{
    state = GameTableState::Running;
    LogMessage(LogLevel::Debug, "Resume");
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

void GameTable::Render(SDL_Renderer* renderer)
{
    if (state != GameTableState::Paused )
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
                auto color = block->RetrieveColor(block->color);

                if (block->highlighted)
                {
                    blockTextureHighlighted->setColor(color);
                    blockTextureHighlighted->Render(block->rect);
                }
                else
                {
                    blockTexture->setColor(color);
                    blockTexture->Render(block->rect);
                }
            }
        }
    }

    Update();
    RenderText(std::to_string(score), SDL_Color{ 255, 255, 255, 255 }, SDL_Point{ 100, 0 });
}

void GameTable::RenderText(std::string text, SDL_Color color, SDL_Point pos)
{
    textTexture->LoadFromRenderedText(gameFont, text, color);
    textTexture->Render(pos.x, pos.y);
}

void GameTable::CalcNextLevelExperience()
{
    expToLevelUp = ((level + 1) ^ 2) * baseExpToLevelUp;
}

void GameTable::LevelUp()
{
    // Level Up
    level++;
    experience = 0;
    CalcNextLevelExperience();

    // This will take into account the level of the table
    distributionForBlockColor = uDist(0, std::min(static_cast<Uint32>(BlockColour::NumberOfValues), level));
    distributionForBlockType = uDist(0, std::min(static_cast<Uint32>(BlockType::NumberOfValues), level));

    // Update random functions
    randBlockColor = std::bind(distributionForBlockColor, generator);
    randBlockType = std::bind(distributionForBlockType, generator);
}

void GameTable::HandleSDLMouseEvent(SDL_Event& event)
{
    bool found = false;
    bool remove = false;
    Uint32 linkNR = 0;

    if (state != GameTableState::Running)
    {
        ClearBlocks(false);
        return;
    }

    auto mousePos = SDL_Point{ event.button.x, event.button.y };

    if (SDL_EnclosePoints(&mousePos, 1, tableDelimiter, nullptr) == SDL_FALSE)
    {
        return;
    }

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

    score += points * multiplier;
}

void GameTable::RemoveBlocksAndUpdateTable(Uint32 linkNr)
{
    for (auto blockPos : blockLinks[linkNr - 1])
    {
        table->at(blockPos->x)->erase(blockPos->y);
        delete blockPos;
    }

    ClearBlocks(true);
    CalcLinkedBlocks();

    // Begin destruction animation
    animTimer->start();
    state = GameTableState::InputDisabled;
}
