#include "GameCharacter.h"


GameCharacter::GameCharacter(PyObject* moduleDict,
                             const char * className,
                             const char * charName, 
                             int hp, int speed, int hit, 
                             int armor, int itemModifier, 
                             int speedModifier, 
                             int dodgeModifier)
{
    PyObject* characterClass = PyDict_GetItemString(moduleDict, className);
    PyObject* args = PyTuple_New(8);
    PyObject* nameObj = PyUnicode_FromString(charName);
    PyObject* hpObj = PyLong_FromLong(hp);
    PyObject* speedObj = PyLong_FromLong(speed);
    PyObject* hitObj = PyLong_FromLong(hit);
    PyObject* armorObj = PyLong_FromLong(armor);
    PyObject* itemModifierObj = PyLong_FromLong(itemModifier);
    PyObject* speedModifierObj = PyLong_FromLong(speedModifier);
    PyObject* dodgeModifierObj = PyLong_FromLong(dodgeModifier);
    PyTuple_SetItem(args, 0, nameObj);
    PyTuple_SetItem(args, 1, hpObj);
    PyTuple_SetItem(args, 2, speedObj);
    PyTuple_SetItem(args, 3, hitObj);
    PyTuple_SetItem(args, 4, armorObj);
    PyTuple_SetItem(args, 5, itemModifierObj);
    PyTuple_SetItem(args, 6, speedModifierObj);
    PyTuple_SetItem(args, 7, dodgeModifierObj);
    
    this->characterInstance = PyObject_CallObject(characterClass, args);
    if (!characterInstance)
    {
        std::cout << "Error instantiating class";
    }
    else
    {
        Py_INCREF(characterInstance);
        //std::cout << "We got instatiation!";
        //PyObject* name = PyObject_GetAttrString(characterInstance, "name");
        //std::cout << PyUnicode_1BYTE_DATA(name); 
    }
    
    //TODO test this doActionFunc first before adding more
    PyObject* doActionFunc = PyObject_GetAttrString(characterClass, "doAction");
    
}

PyObject* GameCharacter::getChar() const
{
    return characterInstance;
}

unsigned char* GameCharacter::getName() const
{
    if (!characterInstance)
    {
        std::cout << "didn't get name";
        return NULL;
    }
    PyObject* name = PyObject_GetAttrString(characterInstance, "name");
    return PyUnicode_1BYTE_DATA(name); 
}

void GameCharacter::setChar(PyObject* newChar)
{
    characterInstance = newChar; 
}

bool GameCharacter::isAlive() const
{
    if (!characterInstance)
    {
        std::cout << "didn't exist";
        return false;
    }
    PyObject* lifeState = PyObject_GetAttrString(characterInstance, "isAlive");
    return PyObject_IsTrue(lifeState); 
}

//6 inputs
void GameCharacter::doAction(int moveIndex, 
                             int charIndex, 
                             ParticipantsList& participants
                            )
{
    //need move idx, int target, players, enemies, participants
    //verify the 6th argument
    PyObject* args = PyTuple_New(5);
    PyObject* moveIndexObj = PyLong_FromLong(moveIndex);
    PyObject* charIndexObj = PyLong_FromLong(charIndex);
    PyTuple_SetItem(args, 0, moveIndexObj);
    PyTuple_SetItem(args, 1, charIndexObj);
    PyTuple_SetItem(args, 2, participants.getPlayers());
    PyTuple_SetItem(args, 3, participants.getEnemies());
    PyTuple_SetItem(args, 4, participants.getParticipants());
    PyObject* testCall = PyObject_CallObject(doActionFunc, args);

}


std::vector<int> GameCharacter::getValidMoves() const
{
    return {1};
}