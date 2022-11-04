#include "Characters/BaseCharacter.h"

BaseCharacter::BaseCharacter(std::string name, int hp, int speed, 
                             int hit, int armor, int itemModifier, int speedModifier, 
                             int dodgeModifier, bool enemy)
{
    this->name = name;
    this->hp = hp;
    maxHp = hp;
    this->speed = speed;
    this->hit = hit;
    this->armor = armor;
    this->itemModifier = itemModifier;
    this->speedModifier = speedModifier;
    this->dodgeModifier = dodgeModifier;
    this->enemy = enemy;
    alive = true;
}

std::pair<std::string, std::vector<int>> BaseCharacter::getActionAndTargets(const std::vector<BaseCharacter>& participants, 
                                                                            std::string decisionAlgo)
{
    std::string chosenMove;
    std::vector<int> targets;
    if (decisionAlgo == "")
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0,3);
        chosenMove = moveTypes[dist(gen)];
        targets = getValidMoves(chosenMove, participantsIndex, participants);
    }

    return make_pair(chosenMove, targets);
}

//still have to think about userInputRanges
std::vector<int> BaseCharacter::getValidMoves(std::string actionType,
                                              int charIndex,
                                              const std::vector<BaseCharacter>& participants)
{
    std::vector<int> validMoves;

    if (actionType == "ATTACK")
    {
        if (!enemy)
        {
            ;
        }

        return validMoves;
    }

    return {};
}

void BaseCharacter::doAction(std::string actionType, 
                             std::vector<int> targets, 
                             std::vector<BaseCharacter>& participants)
{
    if (actionType == "ATTACK")
    {
        for (auto target: targets)
        {
            if (participants[target].isAlive())
            {
                int hit = attack(participants[target]);
                int newHp = participants[target].getHp() - hit;
                participants[target].setHp(newHp);
                if (participants[target].getHp() <= 0)
                {
                    participants[target].changeLifeStatus();
                    shiftDead(participants);
                }
            }
        }
    }
    //TODO buff debuff move
}

void shiftDead(std::vector<BaseCharacter>& participants)
{
    const int LIVE_BOUNDARY = 3;
    for (int i = 0; i < LIVE_BOUNDARY; i++)
    {
        for (int j = 0; j < LIVE_BOUNDARY; j++)
        {
            if (participants[j].isAlive() && !participants[j+1].isAlive())
                std::swap(participants[j], participants[j+1]);
        }
    }
    for (int i = 0; i < LIVE_BOUNDARY; i++)
    {
        for (int j = 0; j < LIVE_BOUNDARY; j++)
        {
            if (participants[j+5].isAlive() && !participants[j+4].isAlive())
                std::swap(participants[j+5], participants[j+4]);
        }
    }
}

bool BaseCharacter::isAlive() const
{
    return alive;
}

int BaseCharacter::getHp() const
{
    return hp;
} 

void BaseCharacter::setHp(int newHp)
{
    hp = newHp;
}

void BaseCharacter::changeLifeStatus()
{
    alive = !alive;
}
