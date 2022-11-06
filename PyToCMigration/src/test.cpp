#include <iostream>
#include "Characters/BaseCharacter.h"
#include "Characters/Players/Bass.h"
#include "Characters/Players/BasePlayer.h"

int main()
{
    //Bass Bass("Bassist", 30, 1, 3, 0, 3, 3, 3, false);

    //BasePlayers are instantiated as !enemy by default
    BasePlayer baseP = BasePlayer("BaseP", 30, 1, 3, 0, 3, 3, 3);
    std::string name = baseP.getName();
    std::cout << name << std::endl;
    Bass bass = Bass("Bassist", 30, 1, 3, 0, 3, 3, 3);
    name = bass.getName();
    std::cout << name << std::endl; 
    return 0;

}