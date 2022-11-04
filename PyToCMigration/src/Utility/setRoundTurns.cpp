#include "Characters/BaseCharacter.h"
#include "setRoundTurns.h"

std::vector<BaseCharacter> SetRoundTurns::setRoundTurns(std::vector<BaseCharacter> characters)
{
    

    for (int i = 0; i < characters.size(); i++) 
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> intDist(1,6);
        int initiativeRoll = intDist(gen);
        int totalSpeedScore = characters[i].getSpeed() + characters[i].getSpeedModifier() + initiativeRoll;

        if (characters[i].alive == true)
        {
            if (speedMap.find(totalSpeedScore) == speedMap.end())
            {
                speedMap[totalSpeedScore] = characters[i];
            }
        }
    }
}