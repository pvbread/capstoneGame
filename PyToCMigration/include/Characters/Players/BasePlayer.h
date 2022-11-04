#include "../../pch.h"
#include "../BaseCharacter.h"

class BasePlayer: public BaseCharacter 
{
public:
    std::unordered_map<int,std::vector<int>> validMovesAndRanges = {{0, {1, 2}}, {1, {3, 4}}, {2, {5, 6}}}; 
private:

};