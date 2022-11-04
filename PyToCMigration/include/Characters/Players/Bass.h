#pragma once

#include "BasePlayer.h"

class Bass: public BasePlayer
{
public:
    Bass() = default;
    Bass(std::string name, int hp, int speed, 
                  int hit, int armor, int itemModifier, int speedModifier, 
                  int dodgeModifier, bool enemy
    );

private:
};