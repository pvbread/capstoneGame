#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "ParticipantsList.h"
#include <vector>

class GameCharacter
{
public:
//remember to inc ref and dec ref with constructors/destructors
    GameCharacter(PyObject* moduleDict,
                  const char * className,
                  const char * charName, 
                  int hp, int speed, int hit, 
                  int armor, int itemModifier, 
                  int speedModifier, 
                  int dodgeModifier
    );

    PyObject* getChar() const;
    unsigned char* getName() const;
    void setChar(PyObject* newChar);
    bool isAlive() const;
   
/*
    ParticipantsList doAction(int moveIndex, 
                              std::vector<int> targetIdxs, 
                              ParticipantsList participants);

    std::vector<int> getValidMoves() const{};
*/

//isAlive
    //getValidMoves
    //getActionAndTargets
    //validMovesAndRanges

    //debug Log statements
private:
    PyObject* characterInstance;
};