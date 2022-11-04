# pragma once

#include "pch.h"

class SetRoundTurns
{
public: 

    std::vector<BaseCharacter> setRoundTurns(std::vector<BaseCharacter> characters);

    std::vector<BaseCharacter> roundOrder;
    // map to keep track of what speed score corresponds to which character
    std::unordered_map<int, BaseCharacter> speedMap;
    std::priority_queue<int, std::vector<int>, std::greater<int>> provisionalOrder;

private:

};