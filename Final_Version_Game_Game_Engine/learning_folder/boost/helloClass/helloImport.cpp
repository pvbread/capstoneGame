#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    Py_Initialize();
    PyObject* sysmodule = PyImport_ImportModule("sys");
    PyObject* syspath = PyObject_GetAttrString(sysmodule, "path");
    PyList_Append(syspath, PyUnicode_FromString("../"));
    PyObject* pName = PyUnicode_DecodeFSDefault("Bagel");
    PyObject* pModule = PyImport_Import(pName);
    PyObject* dict = PyModule_GetDict(pModule);
    PyObject* pythonClass = PyDict_GetItemString(dict, "Bagel");
    PyObject* bagel = PyObject_CallObject(pythonClass, nullptr);
    PyObject* isBagelDelicious = PyObject_CallMethod(bagel, "isDelicious", nullptr);

    bool parsedDelicious = PyBool_Check(isBagelDelicious);

    if (parsedDelicious)
        std::cout << "bagels are indeed delicious";
    else
        std::cout << "You're not getting your bagels from NY";

    Py_FinalizeEx();
   
    return 0;
}