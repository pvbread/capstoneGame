#include "Characters/Players/Bass.h"

Bass::Bass(std::string name, int hp, int speed, 
                             int hit, int armor, int itemModifier, 
                             int speedModifier, int dodgeModifier) :
                                    BasePlayer(name, hp, speed, hit, armor, 
                                               itemModifier, speedModifier, 
                                               dodgeModifier, enemy) {}
