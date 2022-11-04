#include "Characters/BaseCharacter.h"

bool isTeamAlive(std::vector<BaseCharacter> team )
{
    for (int i = 0; i < team.size(); i++)
    {
        if (team[i].alive == true)
        {
            return true;
        }
        
    }
    return false;
}