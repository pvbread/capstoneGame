#include "Game/Characters/BaseCharacter.h"

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
    if (decisionAlgo == "RANDOM")
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0,3);
        chosenMove = (ActionType)dist(gen);

        // try to find index of current character in vector (unsure if this works, needs testing)
        std::string key = name;
        auto itr = std::find_if(participants.begin(),
                            participants.end(),
                            [&key] (const BaseCharacter& obj)
                            {
                                return obj.getName()==key;
                            }
        );
        if (itr != participants.end())
        {
            auto participantsIndex = std::distance(participants.begin(),itr);
        }

        targets = getValidMoves(chosenMove, participantsIndex, participants);
    }

    return make_pair(chosenMove, targets);
}

std::vector<int> BaseCharacter::getValidMoves(ActionType actionType,
                                              int charIndex,
                                              const std::vector<BaseCharacter>& participants)
{
    std::vector<int> validMoves;

    switch(actionType)
    {
        case ATTACK:
        {
            if (!enemy)
            {
                for (auto position: movesAndRanges[ATTACK])
                {
                    int targetPosition = charIndex + position;
                    if (targetPosition < participants.size())
                        validMoves.push_back(targetPosition);
                }
            }
            else
            {
                //it's an enemy
                for (auto position: movesAndRanges[ATTACK])
                {
                    int targetPosition = charIndex - position;
                    if (targetPosition >= 0)
                        validMoves.push_back(targetPosition);
                }
            }
            break;
        }
        case BUFF: case DEBUFF:
        {
            //this covers both the BUFF and DEBUFF cases
            validMoves = getValidBuffTargets(actionType, participants);
            break;
        }
        case MOVE:
        {
            //SPECIAL CASE, it's like a buff but we want to filter out character index
            validMoves = getValidBuffTargets(BUFF, participants);
            std::vector<int>::iterator adjustments;
            int currCharIndex = charIndex; //needed for lambda to be happy (changed lambda conditions)
            adjustments = std::remove_if(validMoves.begin(),
                                    validMoves.end(),
                                    [currCharIndex] (int index) 
                                    {
                                        return index != currCharIndex + 1 || index != currCharIndex - 1;
                                    }
            );
            validMoves.erase(adjustments, validMoves.end());
            break;
        }
    }

    return validMoves;
}

//BUFFS, DEBUFFS, MOVES allowed on any living character on correct team
std::vector<int> BaseCharacter::getValidBuffTargets(ActionType typeOfBuff, 
                                                    const std::vector<BaseCharacter>& participants)
{
    std::vector<int> validMoves;
    const int TEAM_SIZE = 4;
    if ( (!enemy && typeOfBuff == BUFF) || (enemy && typeOfBuff == DEBUFF) )
    {
        for (int i = 0; i < TEAM_SIZE; i++)
        {
            if (participants[i].isAlive())
                validMoves.push_back(i);
        }
    }
    else
    {
        for (int i = 0; i < TEAM_SIZE; i++)
        {
            if (participants[i+4].isAlive())
                validMoves.push_back(i+4);
        }
    }
    return validMoves;
}

std::vector<BaseCharacter> BaseCharacter::doAction(ActionType actionType, 
                                                   std::vector<int> targets, 
                                                   std::vector<BaseCharacter> participants)
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
                        participants[target].changeLifeStatus(false);
                        participants[target].setHp(0); 
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
                participants[target].setSpeedModifier(newTargetSpeedMod);
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
    return participants;
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

int BaseCharacter::attack(BaseCharacter targetCharacter)
{
    const float DODGE_SCALE = 0.02;
    float dodgeChance = targetCharacter.getDodgeModifier() * DODGE_SCALE;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> floatDist(0,1);
    float roll = floatDist(gen);
    if (roll < dodgeChance)
    {
        return 0;
    }
    std::uniform_int_distribution<> intDist(1,4);
    int weaponRoll = intDist(gen);
    int damage = weaponRoll + hit;
    float REDUCTION_SCALE = 0.05;
    int reduction = int(damage * targetCharacter.getArmor() * REDUCTION_SCALE);
    damage -= reduction;
    if (damage < 0)
        return 0;
    return damage;
}

// returns the newHp of the targetCharacter after buff
int BaseCharacter::buff(BaseCharacter targetCharacter)
{
    if (targetCharacter.getHp() >= targetCharacter.getMaxHp())
        return targetCharacter.getHp();
    if (!targetCharacter.isAlive())
        return 0;
    // the last case is that char is alive and doesn't have full health
    int healAmount = ceil(float(targetCharacter.getMaxHp()) * 0.1);
    int newHp = targetCharacter.getHp() + healAmount;
    return newHp;
}

int BaseCharacter::debuff(BaseCharacter targetCharacter)
{
    int newSpeedMod = targetCharacter.getSpeedModifier() - 1;
    if (newSpeedMod < 0)
        return 0;
    return newSpeedMod;
}

std::vector<BaseCharacter> BaseCharacter::moveSpots(int charIndex, int targetIndex, std::vector<BaseCharacter> participants)
{
    if (
        ( participants[charIndex].isEnemy() && !participants[targetIndex].isEnemy() ) ||
        ( participants[targetIndex].isEnemy() && !participants[charIndex].isEnemy() )
       )
        return participants;
    std::swap(participants[charIndex], participants[targetIndex]);
    return participants;
}

std::string BaseCharacter::getName() const
{
    return name;
}

bool BaseCharacter::isAlive() const
{
    return alive;
}

int BaseCharacter::getHp() const
{
    return hp;
} 

int BaseCharacter::getMaxHp() const
{
    return maxHp;
}

int BaseCharacter::getHit() const
{
    return hit;
}

int BaseCharacter::getDodgeModifier() const
{
    return dodgeModifier;
}

int BaseCharacter::getArmor() const
{
    return armor;
}

int BaseCharacter::getSpeed() const
{
    return speed;
}

int BaseCharacter::getSpeedModifier() const
{
    return speedModifier;
}

bool BaseCharacter::isEnemy() const
{
    return enemy;
}

int BaseCharacter::getItemModifier() const
{
    return itemModifier;
}

int BaseCharacter::getParticipantsIndex() const
{
    return participantsIndex;
}

std::unordered_map<ActionType, std::vector<int>> BaseCharacter::getMovesAndRanges() const
{
    return movesAndRanges;
}

void BaseCharacter::setHp(int newHp)
{
    hp = newHp;
    if (hp <= 0)
        changeLifeStatus(false);
    if (!alive && hp > 0)
        changeLifeStatus(true);
}

void BaseCharacter::setName(const std::string& name)
{
    this->name = name;
}

void BaseCharacter::setHit(int hit)
{
    this->hit = hit;
}

void BaseCharacter::setArmor(int armor)
{
    this->armor = armor;
}

void BaseCharacter::setMaxHp(int hp)
{
    this->maxHp = hp;
}

void BaseCharacter::setDodgeModifier(int dodgeModifier)
{
    this->dodgeModifier = dodgeModifier;
}

void BaseCharacter::setItemModifier(int itemModifier)
{
    this->itemModifier = itemModifier;
}

void BaseCharacter::changeLifeStatus(bool alive)
{
    this->alive = alive;
}

void BaseCharacter::setSpeed(int speed)
{
    this->speed = speed;
}

void BaseCharacter::setSpeedModifier(int newSpeedMod)
{
    speedModifier = newSpeedMod;
}

void BaseCharacter::setAsPlayerOrEnemy(bool enemy)
{
    this->enemy = enemy;
}

void BaseCharacter::setNewParticipantsIndex(int newIndex)
{
    participantsIndex = newIndex;
}