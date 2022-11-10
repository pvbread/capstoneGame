#include <iostream>
#include "Characters/BaseCharacter.h"
#include "Characters/Players/Bass.h"
#include "Characters/Players/BasePlayer.h"
#include "Utility/setRoundTurns.h"

int main()
{
    //Bass Bass("Bassist", 30, 1, 3, 0, 3, 3, 3, false);

    //BasePlayers are instantiated as !enemy by default
    BasePlayer p1 = BasePlayer("P1", 30, 1, 1, 0, 3, 3, 3);
    Bass p2 = Bass("PFast", 30, 6, 3, 0, 3, 3, 3);
    Bass p3 = Bass("PFastish", 30, 5, 3, 0, 3, 3, 3);
    Bass p4 = Bass("PSlow", 30, 1, 3, 0, 3, 3, 3);
    BaseCharacter e1 = BaseCharacter("eFast", 30, 6, 3, 0, 3, 3, 3, true);
    BaseCharacter e2 = BaseCharacter("eSlow", 30, 0, 3, 0, 3, 3, 3, true);
    BaseCharacter e3 = BaseCharacter("eMid", 30, 2, 3, 0, 3, 3, 3, true);
    BaseCharacter e4 = BaseCharacter("eMid2", 30, 3, 3, 0, 3, 3, 3, true);
    std::vector<BaseCharacter> participants{e1,e2,e3,e4,p1,p2,p3,p4};
    for (auto el: participants)
    {
        std::cout << el.getName() << " ";
    }
    std::cout << std::endl;
    std::vector<BaseCharacter*> roundOrder;
    roundOrder = setRoundTurns(participants);

    for (auto el: roundOrder)
    {
        std::cout << el->getName() << " ";
    }
    std::cout << std::endl; 

    return 0;

}