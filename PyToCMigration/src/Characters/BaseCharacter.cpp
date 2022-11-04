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
    isAlive = true;
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
    if (actionType == "ATTACK")
    {

    }

    return {};
}

