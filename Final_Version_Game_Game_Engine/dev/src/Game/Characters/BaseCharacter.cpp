#include "Game/Characters/BaseCharacter.h"

// Constructor
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
    BaseItem noItem = BaseItem("", "", 0);
}

// For enemy AI, returns an action and its target list
std::pair<ActionType, std::vector<std::vector<int>>> BaseCharacter::getActionAndTargets(const std::vector<BaseCharacter>& participants, 
                                                                            std::string decisionAlgo)
{
    ActionType chosenMove;
    std::vector<std::vector<int>> targets;
    int participantsIndex = getParticipantsIndex();

    // Random decision
    if (decisionAlgo == "RANDOM")
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0,3);
        chosenMove = (ActionType)dist(gen);
        targets = getValidMoves(chosenMove, participantsIndex, participants);
        
    }

    // Basic logic
    if (decisionAlgo == "logic")
    {
        // Prioritize attack
        chosenMove = ATTACK;
        targets = getValidMoves(chosenMove, participantsIndex, participants);
        // Change target to specific target if target HP is low
        for (int i = 0; i < 4; i++)
        {
            if (hit + 6 > participants[i].getHp())
            {
                chosenMove = ATTACK;
                targets = {{i}};
            }
            break;
        }
        // Change action to Buff if ally HP is low
        for (int i = 4; i < participants.size(); i++)
        {
            if (participants[i].isAlive() && participants[i].getHp() < 5)
            {
                chosenMove = BUFF;
                targets = {{i}};
            }
            break;
        }
        // Change action to Debuff if target's speed is high
        for (int i = 0; i < 4; i++)
        {
            if (participants[i].getSpeedModifier() > 2)
            {
                chosenMove = DEBUFF;
                targets = {{i}};
            }
            break;
        }
        // Change action to Move if opponent's hit damage is higher than its own HP
        for (int i = 0; i < 4; i++)
        {
            if (participants[i].getHit() > hp)
            {
                chosenMove = MOVE;
                targets = getValidMoves(chosenMove, participantsIndex, participants);

            }
            break;
        }
        

    }

    return make_pair(chosenMove, targets);
}

// Get valid targets for each action, returns vector of valid targets for character to choose from
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
            // Valid moves for attacks one character at a time
            const int TEAM_SIZE = 4;
            if (!enemy)
            {
                for (int i = 0; i < TEAM_SIZE; i++)
                    {
                        if (participants[i+4].isAlive())
                            validMoves.push_back({i+4});
                    }
                
            }
            else
            {
                for (int i = 0; i < TEAM_SIZE; i++)
                    {
                        if (participants[i].isAlive())
                            validMoves.push_back({i});
                    }
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
            // If move is selected and teammates are dead
            if (validMoves.size()==0)
            {
                validMoves.push_back({charIndex});
                return validMoves;
            }

            
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

// Apply effects of the action the character chose, returns an updated combat participants vector
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
// Shift dead characters away from the middle of the combat, alive characters move closer to the middle
std::vector<BaseCharacter> BaseCharacter::shiftDead(std::vector<BaseCharacter> participants)
{
    const int LIVE_BOUNDARY = 3;
    // Shift dead for players
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
    // Shift dead for enemies
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
// Attack function returns amount of damage dealt
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

// Buff function, returns the hp to heal
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

// Debuff function, returns the new speed for character
int BaseCharacter::debuff(BaseCharacter targetCharacter)
{
    int newSpeedMod = targetCharacter.getSpeedModifier() - 1;
    if (newSpeedMod < 0)
        return 0;
    return newSpeedMod;
}

// Move function, returns updated combat participants vector
std::vector<BaseCharacter> BaseCharacter::moveSpots(int charIndex, int targetIndex, std::vector<BaseCharacter> participants)
{
   
    std::swap(participants[charIndex],participants[targetIndex]);
    // once swapped, update participant index
    participants[charIndex].setNewParticipantsIndex(charIndex);
    participants[targetIndex].setNewParticipantsIndex(targetIndex);
    return participants;
    
}

//////GETTER FUNCTIONS/////////
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

/////SETTER FUNCTIONS//////

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

BaseItem BaseCharacter::getItem() const
{
    return itemEquipped;
}

void BaseCharacter::setItem(BaseItem newItem)
{
    itemEquipped = newItem;
}