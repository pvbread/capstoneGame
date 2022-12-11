#pragma once

#include "pch.h"
#include "../BaseCharacter.h"

class BasePlayer: public BaseCharacter 
{
public:
    BasePlayer(std::string name, int hp, int speed, 
                  int hit, int armor, int itemModifier, int speedModifier, 
                  int dodgeModifier, bool enemy = false
    );
 
private:

};