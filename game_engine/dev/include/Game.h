#pragma once

#include "pch.h"
#include "Screen.h"
#include "TileType.h"
#include "GameEngine.h"
#include "TextureWrapper.h"
#include "Tile.h"
#include "CharacterInMap.h"
#include "MapDebugController.h"
//#include "loadMedia.h"
#include "Screen.h"
#include "BaseMenu.h"

class EscapeFromCapstone : public Phoenix
{
public:
    void runGameLoop() override;
    EscapeFromCapstone(Uint32 flags, 
                       const char* title, 
                       int x, int y, 
                       int w, int h);
};