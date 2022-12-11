#include "../Characters/BaseCharacter.h"

bool isTeamAlive(const std::vector<BaseCharacter>& participants, bool enemy)
{
    int i = 0;
    if (enemy)
    {
        i = 4;
    }
    const int BOUNDARY = i + 4;

    for (; i < BOUNDARY; i++)
    {
        if (participants[i].isAlive())
        {
            return true;
        } 
    }
    
    return false;
}
