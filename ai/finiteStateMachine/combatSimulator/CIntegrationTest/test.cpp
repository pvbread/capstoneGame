#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <iostream>

int main(int argc, char const *argv[])
{
    Py_Initialize();
    PyObject* sysmodule = PyImport_ImportModule("sys");
    PyObject* syspath = PyObject_GetAttrString(sysmodule, "path");
    PyList_Append(syspath, PyUnicode_FromString("../../characters/"));
    PyObject* pName = PyUnicode_DecodeFSDefault("BaseCharacter");
    PyObject* pModule = PyImport_Import(pName);

    if (!pModule)
    {
        std::cout << "Error loading module";
        exit(1);
    }
    PyObject* dict = PyModule_GetDict(pModule);
    PyObject* baseCharClass = PyDict_GetItemString(dict, "BaseCharacter");

    PyObject* pArgs = PyTuple_New(8);
    PyObject* stringArg = PyUnicode_FromString("Carl");
    PyObject* numArg1 = PyLong_FromLong(3);
    PyTuple_SetItem(pArgs, 0, stringArg);
    PyTuple_SetItem(pArgs, 1, numArg1);
    PyTuple_SetItem(pArgs, 2, numArg1);
    PyTuple_SetItem(pArgs, 3, numArg1);
    PyTuple_SetItem(pArgs, 4, numArg1);
    PyTuple_SetItem(pArgs, 5, numArg1);
    PyTuple_SetItem(pArgs, 6, numArg1);
    PyTuple_SetItem(pArgs, 7, numArg1);
    
    PyObject* baseCharInstance = PyObject_CallObject(baseCharClass, pArgs);
    if (!baseCharInstance)
    {
        std::cout << "Error instantiating class";
    }

    int hasHP = PyObject_HasAttrString(baseCharInstance, "name");
    if (hasHP)
    {
        std::cout << "we queried that BaseChar has name attr!" << std::endl;
    }

    PyObject* name = PyObject_GetAttrString(baseCharInstance, "name");
    unsigned char* cppName = PyUnicode_1BYTE_DATA(name); 

    std::cout << "HIS NAME IS " << cppName;
    
   

    Py_FinalizeEx();
   
    return 0;
}