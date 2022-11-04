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

std::pair<ActionType, std::vector<int>> BaseCharacter::getActionAndTargets(const std::vector<BaseCharacter>& participants, 
                                                                            std::string decisionAlgo)
{
    ActionType chosenMove;
    std::vector<int> targets;
    if (decisionAlgo == "")
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0,3);
        chosenMove = (ActionType)dist(gen);
        targets = getValidMoves(chosenMove, participantsIndex, participants);
    }

    return make_pair(chosenMove, targets);
}

//still have to think about userInputRanges
std::vector<int> BaseCharacter::getValidMoves(ActionType actionType,
                                              int charIndex,
                                              const std::vector<BaseCharacter>& participants)
{
    std::vector<int> validMoves;

    if (actionType == ATTACK)
    {
        if (!enemy)
        {
            ;
        }

        return validMoves;
    }

    return {};
}

void BaseCharacter::doAction(ActionType actionType, 
                             std::vector<int> targets, 
                             std::vector<BaseCharacter>& participants)
{
    switch(actionType)
    {

        case ATTACK:
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
            break;
        }

        case BUFF:
        {
            for (auto target: targets)
            {
                int newTargetHp = buff(participants[target]);
                participants[target].setHp(newTargetHp);
            }
            break;
        }

        case DEBUFF:
        {
            for (auto target: targets)
            {
                int newTargetSpeedMod = debuff(participants[target]);
                participants[target].setSpeedMod(newTargetSpeedMod);
            }
            break;
        }

        case MOVE:
        {
            int targetIndex = targets[0];
            moveSpots(participantsIndex, targetIndex, participants);
            break;
        }
    }
}

void BaseCharacter::shiftDead(std::vector<BaseCharacter>& participants)
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

int BaseCharacter::attack(BaseCharacter targetCharacter){}
int BaseCharacter::buff(BaseCharacter targetCharacter){}
int BaseCharacter::debuff(BaseCharacter targetCharacter){}
void BaseCharacter::moveSpots(int charIndex, int targetIndex, const std::vector<BaseCharacter>& participants){}


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

void BaseCharacter::setSpeedMod(int newSpeedMod)
{
    speedModifier = newSpeedMod;
}