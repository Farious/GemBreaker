#include "GameTable.h"


GameTable::GameTable()
{
    // Game table initialization
    table = new Table();

    // Randomized init
    LogMessage(LogLevel::Debug, "%d", std::rand());
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
    table->clear();
}
