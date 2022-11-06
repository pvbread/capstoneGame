#include <iostream>
#include "Characters/BaseCharacter.h"
#include "Characters/Players/Bass.h"
#include "Characters/Players/BasePlayer.h"

int main()
{
    //Bass Bass("Bassist", 30, 1, 3, 0, 3, 3, 3, false);

    //BasePlayers are instantiated as !enemy by default
    BasePlayer baseP = BasePlayer("BaseP", 30, 1, 3, 0, 3, 3, 3);
    Bass bass = Bass("Bass", 30, 1, 3, 0, 3, 3, 3);
    std::vector<BaseCharacter> participants{baseP, bass};
    for (auto el: participants)
        {
            std::cout << el.getName() << " ";
        }
    std::cout << std::endl;
    std::vector<BaseCharacter> res = baseP.moveSpots(0, 1, participants);
    for (auto el: res)
        {
            std::cout << el.getName() << " ";
        }
    std::cout << std::endl;

    return 0;

}