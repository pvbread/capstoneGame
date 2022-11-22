#pragma once

#include "pch.h"
#include "Screen.h"
#include "TileType.h"
#include "GameEngine.h"
#include "TextureWrapper.h"
#include "Tile.h"
#include "CharacterInMap.h"
#include "MapDebugController.h"
#include "Screen.h"
#include "BaseMenu.h"
#include "Game/Characters/BaseCharacter.h"
#include "Game/Characters/Players/BasePlayer.h"
#include "Game/Characters/Players/Bass.h"

/// @brief Where the game gets implemented
class DashDaCapo : public Archimedes
{
public:
    /// @brief Game loop override's the engine's loop
    void runGameLoop() override;
    DashDaCapo(Uint32 flags, 
                       const char* title, 
                       int x, int y, 
                       int w, int h);
};