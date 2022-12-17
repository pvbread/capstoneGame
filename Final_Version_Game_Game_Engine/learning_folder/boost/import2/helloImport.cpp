#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    Py_Initialize();
    PyObject* sysmodule = PyImport_ImportModule("sys");
    PyObject* syspath = PyObject_GetAttrString(sysmodule, "path");
    PyList_Append(syspath, PyUnicode_FromString("../"));
    PyObject* pName = PyUnicode_DecodeFSDefault("multiply");
    PyObject* pModule = PyImport_Import(pName);
    PyObject* pName2 = PyUnicode_DecodeFSDefault("bar");
    PyObject* pModule2 = PyImport_Import(pName2);
    if (!pModule)
        std::cout << "Error loading mod" << std::endl;
    if (!pModule2)
        std::cout << "Error loading mod2" << std::endl;
   
    PyObject* pArgs = PyTuple_New(2);
    PyObject* arg1 = PyLong_FromLong(3);
    PyObject* arg2 = PyLong_FromLong(4);
    PyTuple_SetItem(pArgs, 0, arg1);
    PyTuple_SetItem(pArgs, 1, arg2);

    std:: cout << PyObject_HasAttrString(pModule2, "foo") << std::endl;
    PyObject* pFunc = PyObject_GetAttrString(pModule, "mult");

    PyObject* pVal = PyObject_CallObject(pFunc, pArgs);
    std::cout << PyLong_AsLong(pVal);

    
    

    Py_FinalizeEx();
   
    return 0;
}