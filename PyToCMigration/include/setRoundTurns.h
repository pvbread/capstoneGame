# pragma once

#include "pch.h"
#include "Characters/BaseCharacter.h"

struct greater_prefer_ally
{   
    template<typename T, typename U>
    bool operator()(T const& lhs, U const& rhs) const
    {   
        if (lhs.first < rhs.first)
            return true;
        if (rhs.first < lhs.first)
            return false; 
        return lhs.second > rhs.second;
    }
};

std::vector<BaseCharacter> setRoundTurns(std::vector<BaseCharacter> characters)
{   
    
    std::vector<BaseCharacter> roundOrder;
    
    
    // map to keep track of what speed score corresponds to which character
    std::unordered_map<int, BaseCharacter> speedMap;

    std::priority_queue<std::pair<int, bool>, std::vector<std::pair<int, bool>>,
                    greater_prefer_ally> provisionalOrder;
    //std::priority_queue<int, std::vector<int>, std::greater<int>> provisionalOrder;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> intDist(1,6);

    for (int i = 0; i < characters.size(); i++) 
    {
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
    return roundOrder;

}