# pragma once

#include "pch.h"
#include "Characters/BaseCharacter.h"

std::vector<BaseCharacter> setRoundTurns(std::vector<BaseCharacter> characters)
{   
    
    std::vector<BaseCharacter> roundOrder;
    return roundOrder;
    /*
    // map to keep track of what speed score corresponds to which character
    std::unordered_map<int, BaseCharacter> speedMap;
    std::priority_queue<int, std::vector<int>, std::greater<int>> provisionalOrder;

    for (int i = 0; i < characters.size(); i++) 
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> intDist(1,6);
        int initiativeRoll = intDist(gen);
        int totalSpeedScore = characters[i].getSpeed() + characters[i].getSpeedModifier() + initiativeRoll;

        if (characters[i].isAlive())
        {
            if (speedMap.find(totalSpeedScore) == speedMap.end())
            {
                speedMap[totalSpeedScore] = characters[i];
            }
        }
    }
    */

}