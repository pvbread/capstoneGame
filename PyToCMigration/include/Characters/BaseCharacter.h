#pragma once

#include "../pch.h"

//just in case we decide to do a moveset array, very unlikely
typedef void(*VoidFunctionPointer)();
typedef int(*IntFunctionPointer)();
typedef std::tuple<IntFunctionPointer, IntFunctionPointer, IntFunctionPointer, VoidFunctionPointer> MoveSet;

class BaseCharacter
{
public:
    BaseCharacter(std::string name, int hp, int maxHp, int speed, 
                  int hit, int armor, int itemModifier, int speedModifier, 
                  int dodgeModifier, bool enemy
    );
    std::pair<int, std::vector<int>> getActionAndTargets(const std::vector<BaseCharacter>& participants, 
                                                         std::string decisionAlgo = "");
    
    std::vector<BaseCharacter> doAction(std::string actionType, 
                                        std::vector<int> targets, 
                                        const std::vector<BaseCharacter>& participants
    );

    std::vector<int> getValidMoves(std::string actionType,
                                   int charIndex,
                                   const std::vector<BaseCharacter>& participants
    );

    std::vector<int> getValidBuffTargets(std::string typeOfBuff, 
                                         const std::vector<BaseCharacter>& participants
    );

    int attack(BaseCharacter targetCharacter);
    int buff(BaseCharacter targetCharacter);
    int debuff(BaseCharacter targetCharacter);
    void moveSpots(int charIndex, int targetIndex, const std::vector<BaseCharacter>& participants);

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
    bool isAlive;
};