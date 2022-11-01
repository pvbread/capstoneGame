#define PY_SSIZE_T_CLEAN

#include <Python.h>
#include <iostream>
#include "GameCharacter.h"
#include "ParticipantsList.h"
#include "ParticipantsVector.h"

void setRoundTurns(ParticipantsVector& unorderedParticipants, PyObject* setRoundTurnsFunc)
{
    PyObject* participantsArg = PyTuple_New(1);
    PyTuple_SetItem(participantsArg, 0, unorderedParticipants.toPyList().getParticipants());
    PyObject* ordereredRoundRaw = PyObject_CallObject(setRoundTurnsFunc, participantsArg);
    ParticipantsList ordereredRoundWrapper = ParticipantsList(ordereredRoundRaw);
    unorderedParticipants.update(ordereredRoundWrapper);
}

bool isTeamAlive(const ParticipantsVector& team)
{
    for (auto participant: team.getParticipantsVector())
    {
        if (participant.isAlive())
            return true;
    }
    return false;
}

int main(int argc, char const *argv[])
{
    Py_Initialize();
    PyObject* sysmodule = PyImport_ImportModule("sys");
    PyObject* syspath = PyObject_GetAttrString(sysmodule, "path");
    PyList_Append(syspath, PyUnicode_FromString("../PythonCode"));
    PyObject* pName = PyUnicode_DecodeFSDefault("Characters");
    PyObject* pModule = PyImport_Import(pName);

    if (!pModule)
    {
        std::cout << "Error loading module";
        exit(1);
    }
    PyObject* dict = PyModule_GetDict(pModule);

    GameCharacter carl = GameCharacter(dict, "Carl", "Carl", 100, 2, 6, 10, 1, 1, 1);
    GameCharacter conehead = GameCharacter(dict, "ConeHead", "ConeHead", 100, 2, 6, 10, 1, 1, 1);
    GameCharacter coneheadBeta = GameCharacter(dict, "ConeHead", "ConeHeadBeta", 100, 2, 6, 10, 1, 1, 1);
    GameCharacter coneheadAlpha = GameCharacter(dict, "ConeHead", "ConeHeadAlpha", 100, 2, 6, 10, 1, 1, 1);
    GameCharacter bass = GameCharacter(dict, "Bass", "Bassist", 100, 2, 6, 10, 1, 1, 1);
    GameCharacter drums = GameCharacter(dict, "Drums", "Drummer", 100, 2, 6, 10, 1, 1, 1);
    GameCharacter flute = GameCharacter(dict, "Flute", "Flutist", 100, 2, 6, 10, 1, 1, 1);
    GameCharacter conductor = GameCharacter(dict, "Conductor", "Maestro", 100, 2, 6, 10, 1, 1, 1);

    PyObject* setRoundName = PyUnicode_DecodeFSDefault("setRoundTurns");
    PyObject* setRoundTurnsModule = PyImport_Import(setRoundName);

    if (!setRoundTurnsModule)
    {
        std::cout << "Error loading setRoundTurns.py";
        exit(1);
    }


    
    std::vector<GameCharacter> pv {
        flute,
        conductor,
        drums,
        bass,
        conehead,
        coneheadAlpha,
        coneheadBeta,
        carl
    };

    ParticipantsVector participantsVector = ParticipantsVector(pv);

    ParticipantsList participantsList = ParticipantsList(
        flute.getChar(),
        drums.getChar(),
        conductor.getChar(),
        bass.getChar(),
        carl.getChar(),
        conehead.getChar(),
        coneheadAlpha.getChar(),
        coneheadBeta.getChar(),
        8
    );


    PyObject* setRoundTurnsFunc = PyObject_GetAttrString(setRoundTurnsModule, "setRoundTurns");
    ParticipantsVector roundOrder = pv;
    
    participantsVector.print();
    //participantsVector.update(participantsList);
    //participantsVector.print(); 
    setRoundTurns(roundOrder, setRoundTurnsFunc);
    roundOrder.print();
    std::cout << "one of the two teams is alive: " << isTeamAlive(participantsVector);

    /*
    while isTeamAlive(playerCharacters) and isTeamAlive(enemyCharacters):
    print(f"Round {roundNum}")
    #gets the new round order
    currentRoundOrder = setRoundTurns(participants)
    #debug print
    """
    print(f"\nCurrent round order {currentRoundOrder}\n")
    """
    printTurnOrder(currentRoundOrder)
    for i in range(len(currentRoundOrder)):
        currentChar = currentRoundOrder[i]
        
        if currentChar.isAlive == False:
            continue #formerly break #we could remove them from participants but may want to keep revive option
        
        sleep(2)
        print(f"{currentChar.name} turn")
        printCombatArray(participants)
        
        # enemy case scenario

        #get index to moveset (which is an array of action funciton) + targets
        if currentChar in enemyCharacters:
            actionAndTargets = currentChar.getActionAndTargets(playerCharacters,enemyCharacters,participants)
        #actionAndTargets = currentChar.getActionAndTargets("NeuralNetwork")
        #actionAndTargets = currentChar.getActionAndTargets("FuzzyLogic")

        #function takes in all the characters, and 
        #returns a copy of the character objects with the targets
        #having been affected by the action
            participants = currentChar.doAction(actionAndTargets[0], actionAndTargets[1], participants)
            print("\n")

        # player case scenario
        
        if currentChar in playerCharacters:
            print("Select action")
            print("0: attack")
            print("1: buff")
            print("2: debuff")
            print("3: move")
            action = int(input("Enter the number for the desired action: "))
            while(action < 0 or action > 3):
                action = int(input("Invalid command. Please enter the number for the desired action: "))
            charIndex = participants.index(currentChar)
            # if user selects attack, user will then choose the type of attack (for now it uses the relevant ranges as choices)
            if action == 0:  
                print("Your position is at", charIndex)
                print("Relevent attack ranges")
                print(currentChar.validMovesAndRanges)
                ranges = int(input("Enter the associated number for valid attack range: "))
                while(ranges < 0 or ranges > len(currentChar.validMovesAndRanges)-1):
                    ranges = int(input("Invalid command. Please enter the number for the desired attack range: "))
                validTargets = currentChar.getValidMoves(action, charIndex, playerCharacters, enemyCharacters, participants, ranges)
            # if user selects anything else, prints the valid targets, then the user will choose a specific target
            else: 
                validTargets = currentChar.getValidMoves(action, charIndex, playerCharacters, enemyCharacters, participants)
                for i in validTargets:
                    print(i,":",participants[i].name, end='. ')
                target = int(input("\nSelect target: "))
                while (target not in validTargets):
                    target = int(input("Invalid target. Select valid target: "))
                validTargets = [target]
            # commit player action
            participants = currentChar.doAction(action, validTargets, participants)
            print("\n")
            
             # checks if either team is not alive and breaks out of the game loop
        if isTeamAlive(playerCharacters) == False or isTeamAlive(enemyCharacters) == False:
            break

    printCombatArray(participants)

    print(f"End of round {roundNum}\n")
    roundNum += 1
    input("Press ENTER to continue\n")
    ## think about, how are we going to read in event input
    ## we're actually going to sit on a loop, doing nothing
    ## once legal input action is selected, we process
if isTeamAlive(playerCharacters):
    print("Victory")
if isTeamAlive(enemyCharacters):
    print("Defeat")
    */
   

    Py_FinalizeEx();
   
    return 0;
}