#include "ParticipantsList.h"

ParticipantsList::ParticipantsList(PyObject* p1,
                                   PyObject* p2,
                                   PyObject* p3,
                                   PyObject* p4,
                                   PyObject* e1,
                                   PyObject* e2,
                                   PyObject* e3,
                                   PyObject* e4,
                                   int sz)
{
    participantsList = PyList_New(sz);
    this->sz = sz;
    if (p1)
    {
        if (PyList_SetItem(participantsList, 0, p1) < 0)
        {
            std::cout << "Error setting players";
            exit(1);
        }
    }
    if (p2)
    {
        if (PyList_SetItem(participantsList, 1, p2) < 0)
        {
            std::cout << "Error setting players";
            exit(1);
        }
    }
    if (p3)
    {
        if (PyList_SetItem(participantsList, 2, p3) < 0)
        {
            std::cout << "Error setting players";
            exit(1);
        }
    }
    if (p4)
    {
        if (PyList_SetItem(participantsList, 3, p4) < 0)
        {
            std::cout << "Error setting players";
            exit(1);
        }
    }
    if (e1)
    {
        if (PyList_SetItem(participantsList, 4, e1) < 0)
        {
            std::cout << "Error setting players";
            exit(1);
        }
    }
    if (e2)
    {
        if (PyList_SetItem(participantsList, 5, e2) < 0)
        {
            std::cout << "Error setting players";
            exit(1);
        }
    }
    if (e3)
    {
        if (PyList_SetItem(participantsList, 6, e3) < 0)
        {
            std::cout << "Error setting players";
            exit(1);
        }
    }
    if (e4)
    {
        if (PyList_SetItem(participantsList, 7, e4) < 0)
        {
            std::cout << "Error setting players";
            exit(1);
        }
    }
}

ParticipantsList::ParticipantsList(std::vector<PyObject*> objsVector)
{
    participantsList = PyList_New(objsVector.size());
    sz = objsVector.size();

    for (int i = 0; i < sz; i++)
    {
        if (objsVector[i])
        {
            if (PyList_SetItem(participantsList, i, objsVector[i]) < 0)
            {
                std::cout << "Error setting players";
                exit(1);
            }
        }
    }
}
                    

PyObject* ParticipantsList::operator[](int index)
{
    return PyList_GetItem(participantsList, index);
}

PyObject* ParticipantsList::getParticipants() const
{
    return participantsList;
}

PyObject* ParticipantsList::getPlayers() const
{
    return PyList_GetSlice(participantsList, 0, 4);
} 

PyObject* ParticipantsList::getEnemies() const
{
    return PyList_GetSlice(participantsList, 4, 8);
} 

int ParticipantsList::size() const
{
    return sz;
}