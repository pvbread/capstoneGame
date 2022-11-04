#pragma once

#include "../../pch.h"
#include "../BaseCharacter.h"

class BasePlayer: public BaseCharacter 
{
public:
    BasePlayer() = default;
    BasePlayer(std::string name, int hp, int speed, 
                  int hit, int armor, int itemModifier, int speedModifier, 
                  int dodgeModifier, bool enemy
    );

    std::unordered_map<int,std::vector<int>> validMovesAndRanges = {{0, {1, 2}}, {1, {3, 4}}, {2, {5, 6}}}; 
private:

};