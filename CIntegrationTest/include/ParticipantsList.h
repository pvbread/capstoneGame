#pragma once

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <iostream>
#include <vector>


class ParticipantsList
{
public:
    ParticipantsList(PyObject* p1,
                     PyObject* p2,
                     PyObject* p3,
                     PyObject* p4,
                     PyObject* e1,
                     PyObject* e2,
                     PyObject* e3,
                     PyObject* e4,
                     int size
    );
    ParticipantsList(std::vector<PyObject*>);

    //overload
    PyObject* operator[](int index);
    PyObject* getParticipants() const;
    PyObject* getPlayers() const;
    PyObject* getEnemies() const;

    int size() const;

private:
    PyObject* participantsList;
    int sz;
};