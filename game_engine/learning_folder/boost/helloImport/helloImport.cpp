#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    Py_Initialize();
    PyObject* sysmodule = PyImport_ImportModule("sys");
    PyObject* syspath = PyObject_GetAttrString(sysmodule, "path");
    PyList_Append(syspath, PyUnicode_FromString("."));
    PyObject* pName = PyUnicode_DecodeFSDefault("multiply");
    PyObject* pModule = PyImport_Import(pName);
    
    PyObject* pArgs = PyTuple_New(2);
    PyObject* arg1 = PyLong_FromLong(3);
    PyObject* arg2 = PyLong_FromLong(4);
    PyTuple_SetItem(pArgs, 0, arg1);
    PyTuple_SetItem(pArgs, 1, arg2);
    PyObject* pFunc = PyObject_GetAttrString(pModule, "mult");

    PyObject* pVal = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pName);

    std::cout << PyLong_AsLong(pVal);

    if (Py_FinalizeEx() < 0) {
        return 120;
    }
    return 0;
}