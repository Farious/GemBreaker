#include "GameTable.h"

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

GameTable::GameTable()
{
    // Game table initialization
    table = new Table();

    // Timer initialization
    timer = new SimpleTimer();

    // Upper Left position
    ulPos = vector2D{ 50, 50 };

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

    // Initialize all the game variables
    Init();
}


GameTable::~GameTable()
{
    // Cleaning textures
    delete blockTexture;
    delete bombTexture;
    blockTexture = nullptr;
    bombTexture = nullptr;

    // Clean timer
    delete timer;
    timer = nullptr;

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
    auto col = table->find(colLimit - 1);
    if (col != table->end())
    {
        auto row = col->second->find(0);

        if (row != col->second->end())
        {
            auto block = row->second;
            block->Print();
        }
    }

    // Reset timer
    timer->start();
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
    auto rows = randColNum();

    for (Uint32 row = 0; row < rows; row++)
    {
        // Generate new block
        auto bColour = static_cast<BlockColour>(randBlockColor());
        auto bType = static_cast<BlockType>(randBlockType());
        Block* block = new Block{ bColour, bType, col, row, 1, 1 };

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
