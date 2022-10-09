from utility.setRoundTurns import setRoundTurns
from utility.isTeamAlive import isTeamAlive 
from characters.BaseCharacter import BaseCharacter
from characters.Conehead import Conehead
from characters.Carl import Carl

'''
CombatSim aims to instantiate the proper game state, round states
and simulate rounds until the combat finishes
We can have the AI decision policy be added modularly.
'''

#name = className("print name", hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
carl = Carl("Carl", 50, 0, 3, 5, 3, 3, 1)
conehead = Conehead("Conehead", 50, 3, 2, 1, 3, 3, 5) 
bass = BaseCharacter("bassist", 50, 2, 3, 2, 3, 3, 6)

#this is going to be passed in to the combatSim program
#combatSym(playerChars, enemyChars)
playerCharacters = [bass]
enemyCharacters = [conehead, carl]

roundNum = 1

participants = playerCharacters + enemyCharacters
while isTeamAlive(playerCharacters) and isTeamAlive(enemyCharacters):
    print(f"Round {roundNum}")
    #gets the new round order
    currentRoundOrder = setRoundTurns(participants)
    #debug print
    print(f"Current round order {currentRoundOrder}")
    
    for i in range(len(currentRoundOrder)):
        currentChar = currentRoundOrder[i]
        if currentChar.isAlive == False:
            break

        #get index to moveset (which is an array of action funciton) + targets
        actionAndTargets = currentChar.getActionAndTargets(playerCharacters,enemyCharacters,participants)
        #actionAndTargets = currentChar.getActionAndTargets("NeuralNetwork")
        #actionAndTargets = currentChar.getActionAndTargets("FuzzyLogic")

        #function takes in all the characters, and 
        #returns a copy of the character objects with the targets
        #having been affected by the action
        print(f"{currentChar.name} turn")
        participants = currentChar.doAction(actionAndTargets[0], actionAndTargets[1], participants)

        
    print(f"End of round {roundNum}\n")
    roundNum+=1
if isTeamAlive(playerCharacters):
    print("Victory")
if isTeamAlive(enemyCharacters):
    print("Defeat")
