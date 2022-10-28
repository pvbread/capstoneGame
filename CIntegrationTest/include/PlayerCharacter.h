#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "ParticipantsList.h"

class PlayerCharacter
{
//remember to inc ref and dec ref with constructors/destructors
    PlayerCharacter(std::string name, int hp, int speed, int hit, 
                    int armor, int itemModifier, int speedModifier, 
                    int dodgeModifier)
    {}

    ParticipantsList doAction(int moveIndex, 
                              std::vector<int> targetIdxs, 
                              ParticipantsList participants);
    //need do action

    //getValidMoves
    //getActionAndTargets
    //validMovesAndRanges

    //debug Log statements
private:
    PyObject* playerInstance;
};