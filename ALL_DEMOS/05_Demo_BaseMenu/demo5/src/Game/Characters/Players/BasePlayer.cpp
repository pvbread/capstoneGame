#include "Game/Characters/Players/BasePlayer.h"

BasePlayer::BasePlayer(std::string name, int hp, int speed, 
                       int hit, int armor, int itemModifier, 
                       int speedModifier, int dodgeModifier, bool enemy) :  
                                BaseCharacter(name, hp, speed, hit, armor, 
                                              itemModifier, speedModifier, 
                                              dodgeModifier, enemy) {}
