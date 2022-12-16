# pragma once

#include "pch.h"
#include "../Characters/BaseCharacter.h"

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

// Set round order for character by using a priority queue, returns a vector of strings of the character names
std::vector<std::string> setRoundTurns(std::vector<BaseCharacter>& characters)
{   
    
    std::vector<std::string> roundOrder;
    std::priority_queue<std::pair<int, std::pair<bool, BaseCharacter*>>, 
                        std::vector<std::pair<int, std::pair<bool, BaseCharacter*>>>,
                        greater_prefer_ally> newOrderQueue;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> intDist(1,6);

    for (int i = 0; i < characters.size(); i++) 
    {
        // Skip if character is dead
        if (!characters[i].isAlive())
            continue;
        // Calculate initiative to determine round order
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
        auto temp = (&(*curr.second.second)); //insert pointer to character
        // Get name of character and push into the roundOrder vector
        std::string charName = temp->getName(); 
        roundOrder.push_back(charName);
        newOrderQueue.pop();
    }

    return roundOrder;

}