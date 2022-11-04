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

    std::vector<int> getValidBuffTargets(std::string typeOfBuff, 
                                         const std::vector<BaseCharacter>& participants
    );

    void doAction(ActionType actionType, 
                  std::vector<int> targets, 
                  std::vector<BaseCharacter>& participants
    );

    void shiftDead(std::vector<BaseCharacter>& participants);

    int attack(BaseCharacter targetCharacter);
    int buff(BaseCharacter targetCharacter);
    int debuff(BaseCharacter targetCharacter);
    void moveSpots(int charIndex, int targetIndex, const std::vector<BaseCharacter>& participants);
    bool isAlive() const;
    int getHp() const;
    void setHp(int newHp);
    void changeLifeStatus();

private:
    std::string name;
    int hp;
    int maxHp; // do not currently have items that inc maxHp
    int speed;
    int hit;
    int armor;
    int itemModifier;
    int speedModifier;
    int dodgeModifier;
    bool enemy;
    bool alive;
    int participantsIndex;
    std::vector<std::vector<int>> validMovesAndRanges = {
        {1,2},
        {3,4},
        {5,6}
    };
};