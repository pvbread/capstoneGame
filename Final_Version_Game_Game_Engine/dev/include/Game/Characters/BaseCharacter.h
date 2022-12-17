#pragma once

#include "pch.h"
#include "../Items/BaseItem.h"

//just in case we decide to do a moveset array, very unlikely
typedef void(*VoidFunctionPointer)();
typedef int(*IntFunctionPointer)();
typedef std::tuple<IntFunctionPointer, IntFunctionPointer, IntFunctionPointer, VoidFunctionPointer> MoveSet;

enum ActionType 
{
    ATTACK = 0,
    BUFF = 1,
    DEBUFF = 2,
    MOVE = 3
};

class BaseCharacter
{
public:
    BaseCharacter() = default;
    ~BaseCharacter() = default;
    BaseCharacter(std::string name, int hp, int speed, 
                  int hit, int armor, int itemModifier, int speedModifier, 
                  int dodgeModifier, bool enemy
    );

    /*
    //copy constructor
    BaseCharacter(const BaseCharacter& rhs);

    //copy assignment
    BaseCharacter& operator= (const BaseCharacter& rhs);

    //move constructor
    BaseCharacter(BaseCharacter&& rhs);

    //move assignment

    BaseCharacter& operator= (BaseCharacter && rhs);
    */
    std::pair<ActionType, std::vector<std::vector<int>>> getActionAndTargets(const std::vector<BaseCharacter>& participants, 
                                                                 std::string decisionAlgo = "RANDOM"
    );

    std::vector<std::vector<int>> getValidMoves(ActionType actionType,
                                   int charIndex,
                                   const std::vector<BaseCharacter>& participants
    );

    std::vector<int> getValidBuffTargets(ActionType typeOfBuff, 
                                         const std::vector<BaseCharacter>& participants
    );

    std::vector<BaseCharacter> doAction(ActionType actionType, 
                                        std::vector<int>& effectOfAction,
                                        std::vector<int> targets, 
                                        std::vector<BaseCharacter> participants
    );

    std::vector<BaseCharacter> shiftDead(std::vector<BaseCharacter> participants);

    int attack(BaseCharacter targetCharacter);
    int buff(BaseCharacter targetCharacter);
    int debuff(BaseCharacter targetCharacter);
    std::vector<BaseCharacter> moveSpots(int charIndex, int targetIndex, 
                                         std::vector<BaseCharacter> participants
    );

    friend std::vector<std::string> setRoundTurns(std::vector<BaseCharacter>& characters);
    friend bool isTeamAlive(const std::vector<BaseCharacter>& participants, bool enemy);
    

    std::string getName() const;
    bool isAlive() const;
    int getHp() const;
    int getMaxHp() const;
    int getHit() const;
    int getDodgeModifier() const;
    int getArmor() const;
    int getSpeed() const;
    int getSpeedModifier() const;
    int getItemModifier() const;
    int getParticipantsIndex() const;
    BaseItem getItem() const;
    std::unordered_map<ActionType, std::vector<int>> getMovesAndRanges() const;
    bool isEnemy() const;
    void setHp(int newHp);
    void setName(const std::string& name);
    void setHit(int hit);
    void setArmor(int armor);
    void setMaxHp(int hp);
    void setSpeed(int speed);
    void setDodgeModifier(int dodgeModifier);
    void setSpeedModifier(int newSpeedMod);
    void setItemModifier(int itemModifier);
    void changeLifeStatus(bool alive);
    void setAsPlayerOrEnemy(bool enemy);
    void setNewParticipantsIndex(int newIndex);
    void setItem(BaseItem newItem);

protected:
    std::string name;
    int maxHp; // do not currently have items that inc maxHp
    int speed;
    int hit;
    int armor;
    int dodgeModifier;
    bool enemy;
    int speedModifier;
    int hp;
    bool alive;
    int participantsIndex;
    int itemModifier;
    BaseItem itemEquipped;
    
    //example validMoves and ranges
    std::unordered_map<ActionType, std::vector<int>> movesAndRanges = {
        {ATTACK, {1,2,3,4,5,6,7}},
        {BUFF, {1}},
        {DEBUFF, {1}}
    };
};