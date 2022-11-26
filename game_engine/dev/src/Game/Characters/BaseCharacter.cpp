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

/*
//copy constructor
BaseCharacter::BaseCharacter(const BaseCharacter& rhs)
{
    name = rhs.name;
    maxHp = rhs.maxHp; 
    speed = rhs.speed;
    hit = rhs.hit;
    armor = rhs.armor;
    dodgeModifier = rhs.dodgeModifier;
    enemy = rhs.enemy;
    speedModifier = rhs.speedModifier;
    hp = rhs.hp;
    alive = rhs.alive;
    participantsIndex = rhs.participantsIndex;
    itemModifier = rhs.itemModifier;
}

BaseCharacter& BaseCharacter::operator= (const BaseCharacter& rhs)
{
    BaseCharacter copy = rhs;
    std::swap(*this,copy);
    return *this;
}

BaseCharacter::BaseCharacter(BaseCharacter &&rhs)
{
    name = rhs.name;
    maxHp = rhs.maxHp; 
    speed = rhs.speed;
    hit = rhs.hit;
    armor = rhs.armor;
    dodgeModifier = rhs.dodgeModifier;
    enemy = rhs.enemy;
    speedModifier = rhs.speedModifier;
    hp = rhs.hp;
    alive = rhs.alive;
    participantsIndex = rhs.participantsIndex;
    itemModifier = rhs.itemModifier;
    rhs.name = "";
    rhs.maxHp = 0; 
    rhs.speed = 0;
    rhs.hit = 0;
    rhs.armor = 0;
    rhs.dodgeModifier = 0;
    rhs.enemy = 0;
    rhs.speedModifier = 0;
    rhs.hp = 0;
    rhs.alive = 0;
    rhs.participantsIndex = 0;
    rhs.itemModifier = 0;

}

BaseCharacter& BaseCharacter::operator= (BaseCharacter&& rhs)
{
    std::swap(name,rhs.name);
    std::swap(maxHp,rhs.maxHp);
    std::swap(speed,rhs.speed);
    std::swap(hit,rhs.hit);
    std::swap(armor,rhs.armor);
    std::swap(dodgeModifier,rhs.dodgeModifier);
    std::swap(enemy,rhs.enemy);
    std::swap(speedModifier,rhs.speedModifier);
    std::swap(hp,rhs.hp);
    std::swap(alive,rhs.alive);
    std::swap(participantsIndex,rhs.participantsIndex);
    std::swap(itemModifier,rhs.itemModifier);

    return *this;
}
*/
std::pair<ActionType, std::vector<std::vector<int>>> BaseCharacter::getActionAndTargets(const std::vector<BaseCharacter>& participants, 
                                                                            std::string decisionAlgo)
{
    ActionType chosenMove;
    std::vector<std::vector<int>> targets;
    if (decisionAlgo == "RANDOM")
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0,3);
        chosenMove = (ActionType)dist(gen);

        
        int participantsIndex = this->getParticipantsIndex();
        targets = getValidMoves(chosenMove, participantsIndex, participants);
    }

    return make_pair(chosenMove, targets);
}

std::vector<std::vector<int>> BaseCharacter::getValidMoves(ActionType actionType,
                                              int charIndex,
                                              const std::vector<BaseCharacter>& participants)
{
    std::vector<int> targets;
    std::vector<std::vector<int>> validMoves;

    switch(actionType)
    {
        case ATTACK:
        {   
            // change valid moves for attacks one character at a time
            const int TEAM_SIZE = 4;
            if (!enemy)
            {
                for (int i = 0; i < TEAM_SIZE; i++)
                    {
                        if (participants[i+4].isAlive())
                            validMoves.push_back({i+4});
                    }
                /*
                for (auto position: movesAndRanges[ATTACK])
                {
                    int targetPosition = charIndex + position;
                    if (targetPosition < participants.size())
                        targets = {targetPosition};
                    validMoves.push_back(targets);
                }
                */
            }
            else
            {
                for (int i = 0; i < TEAM_SIZE; i++)
                    {
                        if (participants[i].isAlive())
                            validMoves.push_back({i});
                    }
                //it's an enemy
                /*
                for (auto position: movesAndRanges[ATTACK])
                {
                    int targetPosition = charIndex - position;
                    if (targetPosition >= 0)
                        targets = {targetPosition};
                    validMoves.push_back(targets);
                }
                */
            }
            break;
        }
        case BUFF: case DEBUFF:
        {
            //this covers both the BUFF and DEBUFF cases
            targets = getValidBuffTargets(actionType, participants);
            for(int i = 0; i < targets.size(); i++)
            {
                std::vector<int> temp = {targets[i]};
                validMoves.push_back(temp);
            }
            break;
        }
        case MOVE:
        {
            const int TEAM_SIZE = 4;
            if (!enemy)
            {
                for (int i = 0; i < TEAM_SIZE; i++)
                    {
                        if (participants[i].isAlive() && i != charIndex)
                            validMoves.push_back({i});
                    }
            }
            else
            {
                for (int i = 0; i < TEAM_SIZE; i++)
                    {
                        if (participants[i+4].isAlive() && i+4 != charIndex)
                            validMoves.push_back({i+4});
                    }
            }

            // FOR SOME REASON THERE"S A SEGFAULT WITHIN THIS LAMBDA FUNCTION SOMETHING TO DO WITH THE ITERATORS
            /*
            //SPECIAL CASE, it's like a buff but we want to filter out character index
            targets = getValidBuffTargets(BUFF, participants);
            std::vector<int>::iterator adjustments;
            int currCharIndex = charIndex; //needed for lambda to be happy (changed lambda conditions)
            adjustments = std::remove_if(targets.begin(),
                                    targets.end(),
                                    [currCharIndex] (int index) 
                                    {
                                        return index != currCharIndex + 1 && index != currCharIndex - 1;
                                    }
            );
            targets.erase(adjustments, targets.end());
            for(int i; i < targets.size(); i++)
            {
                std::vector<int> temp = {targets[i]};
                validMoves.push_back(temp);
            }
            */
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
                                                   std::vector<int>& effectOfAction,
                                                   std::vector<int> targets, 
                                                   std::vector<BaseCharacter> participants)
{
    effectOfAction.clear();
    switch(actionType)
    {
        case ATTACK:
        {
        
            for (auto target: targets)
            {
                if (participants[target].isAlive())
                {
                    int hit = attack(participants[target]);
                    effectOfAction.push_back(hit);
                    int newHp = participants[target].getHp() - hit;
                    participants[target].setHp(newHp);
                    if (participants[target].getHp() <= 0)
                    {
                        participants[target].changeLifeStatus(false);
                        participants[target].setHp(0); 
                        participants = shiftDead(participants);
                    }
                }
            }

            
            break;
        }

        case BUFF:
        {
            for (auto target: targets)
            {
                int healAmount = buff(participants[target]);
                effectOfAction.push_back(healAmount);
                int newHp = participants[target].getHp() + healAmount;
                participants[target].setHp(newHp);
            }
            break;
        }

        case DEBUFF:
        {
            for (auto target: targets)
            {
                int newTargetSpeedMod = debuff(participants[target]);
                effectOfAction.push_back(newTargetSpeedMod); 
                participants[target].setSpeedModifier(newTargetSpeedMod);
            }
            break;
        }

        case MOVE:
        {
            int targetIndex = targets[0];
            participants = moveSpots(participantsIndex, targetIndex, participants);
            break;
        }
    }
    return participants;

}

std::vector<BaseCharacter> BaseCharacter::shiftDead(std::vector<BaseCharacter> participants)
{
    const int LIVE_BOUNDARY = 3;
    for (int i = 0; i < LIVE_BOUNDARY; i++)
    {
        for (int j = 0; j < LIVE_BOUNDARY; j++)
        {
            if (participants[j].isAlive() && !participants[j+1].isAlive())
            {
                std::swap(participants[j], participants[j+1]);
                // once swapped, update participant index
                participants[j].setNewParticipantsIndex(j);
                participants[j+1].setNewParticipantsIndex(j+1);
            }   
        }
    }
    for (int i = 0; i < LIVE_BOUNDARY; i++)
    {
        for (int j = 0; j < LIVE_BOUNDARY; j++)
        {
            if (participants[j+5].isAlive() && !participants[j+4].isAlive())
            {
                std::swap(participants[j+4],participants[j+5]);
                // once swapped, update participant index

                participants[j+4].setNewParticipantsIndex(j+4);
                participants[j+5].setNewParticipantsIndex(j+5);
            }   
        }
    }
    return participants;
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


// returns the hp to heal

int BaseCharacter::buff(BaseCharacter targetCharacter)
{
    if (targetCharacter.getHp() >= targetCharacter.getMaxHp())
        return 0;
    if (!targetCharacter.isAlive())
        return 0;
    // the last case is that char is alive and doesn't have full health
    int healAmount = ceil(float(targetCharacter.getMaxHp()) * 0.1);
    return healAmount;
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
    //BaseCharacter copy = participants[charIndex];
    //participants[charIndex] = std::move(participants[targetIndex]);
    //participants[targetIndex] = std::move(copy);
    std::swap(participants[charIndex],participants[targetIndex]);
    // once swapped, update participant index
    participants[charIndex].setNewParticipantsIndex(charIndex);
    participants[targetIndex].setNewParticipantsIndex(targetIndex);
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