#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <vector>
#include "GameCharacter.h"
#include "ParticipantsList.h"

class ParticipantsVector
{
public:
    ParticipantsVector(std::vector<GameCharacter> pv);
    ParticipantsList toPyList() const;
    void update(ParticipantsList pl);
    std::vector<GameCharacter> getParticipantsVector() const;
    void print();

private:
    std::vector<GameCharacter> participantsVector;
};