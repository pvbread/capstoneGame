#include "ParticipantsVector.h"

ParticipantsVector::ParticipantsVector(std::vector<GameCharacter> pv)
{
    participantsVector = pv;
}

ParticipantsList ParticipantsVector::toPyList() const
{
    std::vector<PyObject*> newList(participantsVector.size());
    for (int i = 0; i < participantsVector.size(); i++)
    {
        newList[i] = participantsVector[i].getChar();
    }
    ParticipantsList convertedList = ParticipantsList(newList);
    return convertedList;
}

void ParticipantsVector::update(ParticipantsList pl)
{
    for (int i = 0; i < pl.size(); i++)
    {
        participantsVector[i].setChar(pl[i]);
    }
}

std::vector<GameCharacter> ParticipantsVector::getParticipantsVector() const
{
    return participantsVector;
}

void ParticipantsVector::print()
{
    for (auto participant: participantsVector)
    {
        std::cout << participant.getName() << " ";
    }
    std::cout << std::endl;
}

GameCharacter ParticipantsVector::operator[](int index)
{
    return participantsVector[index];
}