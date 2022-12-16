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

    stringArg = PyUnicode_FromString("ConeHead");
    PyTuple_SetItem(pArgs, 0, stringArg);
    
    PyObject* baseCharInstance2 = PyObject_CallObject(baseCharClass, pArgs);
    if (!baseCharInstance)
    {
        std::cout << "Error instantiating class";
    }

    stringArg = PyUnicode_FromString("ConeHeadBeta");
    PyTuple_SetItem(pArgs, 0, stringArg);
    
    PyObject* baseCharInstance3 = PyObject_CallObject(baseCharClass, pArgs);
    if (!baseCharInstance)
    {
        std::cout << "Error instantiating class";
    }

    stringArg = PyUnicode_FromString("ConeHeadCharlie");
    PyTuple_SetItem(pArgs, 0, stringArg);
    
    PyObject* baseCharInstance4 = PyObject_CallObject(baseCharClass, pArgs);
    if (!baseCharInstance)
    {
        std::cout << "Error instantiating class";
    }

    

    //Macro form of PyList_SetItem() without error checking. This is normally only used to fill in new lists where there is no previous content.

    PyObject* enemies = PyList_New(4);
    PyList_SET_ITEM(enemies, 0, baseCharInstance);
    PyList_SET_ITEM(enemies, 1, baseCharInstance2);
    PyList_SET_ITEM(enemies, 2, baseCharInstance3);
    PyList_SET_ITEM(enemies, 3, baseCharInstance4);

    //try to call a sort function from python
    //query each name on the list before and after

    for (int i = 0; i < 4; i++)
    {
        PyObject* listItem = PyList_GetItem(enemies, i);
        PyObject* name = PyObject_GetAttrString(listItem, "name");
        unsigned char* cppName = PyUnicode_1BYTE_DATA(name);
        std::cout << "char name is: " << cppName << std::endl;  
    }

    PyObject* sysmodule2 = PyImport_ImportModule("sys");
    PyObject* syspath2 = PyObject_GetAttrString(sysmodule2, "path");
    PyList_Append(syspath2, PyUnicode_FromString("../../utility/"));
    pName = PyUnicode_DecodeFSDefault("setRoundTurns");
    pModule = PyImport_Import(pName);

    if (!pModule)
    {
        std::cout << "Error loading module";
        exit(1);
    }
    
    PyObject* setRoundTurnsFunc = PyObject_GetAttrString(pModule, "setRoundTurns");

    if (!setRoundTurnsFunc)
    {
        std::cout << "Error loading function";
        exit(1);
    }

    PyObject* setRoundArgs = PyTuple_New(1);
    PyTuple_SetItem(setRoundArgs, 0, enemies);
    PyObject* orderedRound = PyObject_CallObject(setRoundTurnsFunc, setRoundArgs);
    
    for (int i = 0; i < 4; i++)
    {
        PyObject* listItem = PyList_GetItem(orderedRound, i);
        PyObject* name = PyObject_GetAttrString(listItem, "name");
        unsigned char* cppName = PyUnicode_1BYTE_DATA(name);
        std::cout << "char name is: " << cppName << std::endl;  
    }
    
   

    Py_FinalizeEx();
   
    return 0;
}