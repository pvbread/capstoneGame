#include <iostream>
#include "Characters/BaseCharacter.h"
//#include "Characters/Players/Bass.h"
#include "Characters/Players/BasePlayer.h"

int main()
{
    //Bass Bass("Bassist", 30, 1, 3, 0, 3, 3, 3, false);
    
    //BasePlayers are instantiated as !enemy by default
    BasePlayer bass = BasePlayer("Bassist", 30, 1, 3, 0, 3, 3, 3);
    std::string name = bass.getName();
    std::cout << name;
    return 0;

}