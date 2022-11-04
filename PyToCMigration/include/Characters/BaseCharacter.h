#pragma once

#include "../pch.h"

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
    BaseCharacter(std::string name, int hp, int speed, 
                  int hit, int armor, int itemModifier, int speedModifier, 
                  int dodgeModifier, bool enemy
    );

    std::pair<ActionType, std::vector<int>> getActionAndTargets(const std::vector<BaseCharacter>& participants, 
                                                                 std::string decisionAlgo = ""
    );

    std::vector<int> getValidMoves(ActionType actionType,
                                   int charIndex,
                                   const std::vector<BaseCharacter>& participants
    );

    std::vector<int> getValidBuffTargets(ActionType typeOfBuff, 
                                         const std::vector<BaseCharacter>& participants
    );

    std::vector<BaseCharacter> doAction(ActionType actionType, 
                                        std::vector<int> targets, 
                                        std::vector<BaseCharacter> participants
    );

    void shiftDead(std::vector<BaseCharacter>& participants);

    int attack(BaseCharacter targetCharacter);
    int buff(BaseCharacter targetCharacter);
    int debuff(BaseCharacter targetCharacter);
    std::vector<BaseCharacter> moveSpots(int charIndex, int targetIndex, 
                                         std::vector<BaseCharacter> participants
    );
    std::string getName() const;
    bool isAlive() const;
    int getHp() const;
    int getMaxHp() const;
    int getDodgeModifier() const;
    int getArmor() const;
    int getSpeed() const;
    int getSpeedModifier() const;
    void setHp(int newHp);
    void setSpeedModifier(int newSpeedMod);
    void changeLifeStatus();
    int speedModifier;
    int hp;
    bool alive;
    int participantsIndex;
    int itemModifier;


protected:
    std::string name;
    int maxHp; // do not currently have items that inc maxHp
    int speed;
    int hit;
    int armor;
    int dodgeModifier;
    bool enemy;
    
    //example validMoves and ranges
    std::unordered_map<ActionType, std::vector<int>> validMovesAndRanges = {
        {ATTACK, {2,4}},
        {BUFF, {2}},
        {DEBUFF, {5}}
    };
};