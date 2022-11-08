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
        return lhs.second.first > rhs.second.first;
    }
};

std::vector<BaseCharacter> setRoundTurns(std::vector<BaseCharacter> characters)
{   
    
    std::vector<BaseCharacter> roundOrder;
    // map to keep track of what speed score corresponds to which character
    std::unordered_map<int, std::vector<BaseCharacter>> speedMap;

    std::priority_queue<std::pair<int, std::pair<bool, BaseCharacter*>>, 
                        std::vector<std::pair<int, std::pair<bool, BaseCharacter*>>>,
                        greater_prefer_ally> newOrderQueue;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> intDist(1,6);

    for (int i = 0; i < characters.size(); i++) 
    {
        if (!characters[i].isAlive())
            continue;
        int initiativeRoll = intDist(gen);
        int totalSpeedScore = characters[i].getSpeed() + characters[i].getSpeedModifier() + initiativeRoll;
        //tuple of form (bool isEnemy, BaseChar*)
        auto enemyCharPtrTuple = std::make_pair(characters[i].isEnemy(), &characters[i]);
        auto speedScoreCharTuple = std::make_pair(totalSpeedScore, enemyCharPtrTuple); 
        newOrderQueue.push(speedScoreCharTuple);
    }

    while(!newOrderQueue.empty())
    {
        auto curr = newOrderQueue.top();
        roundOrder.push_back(*curr.second.second);
        newOrderQueue.pop();
    }

    return roundOrder;

}