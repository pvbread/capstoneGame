#pragma once

#include "pch.h"
#include "TileType.h"
#include "GameEngine.h"
#include "TextureWrapper.h"
#include "Tile.h"
#include "BaseMenu.h"
#include "Timer.h"

class YourGame : public Archimedes
{
public:
    void runGameLoop() override;
    YourGame(Uint32 flags, 
             const char* title, 
             int x, int y, 
             int w, int h);
};