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
carl = Carl("Carl", 100, 2, 3, 10, 3, 3, 3)
conehead = Conehead("Conehead", 100, 2, 3, 5, 3, 3, 6) 
bass = BaseCharacter("bassist", 100, 2, 3, 4, 3, 3, 6)

#this is going to be passed in to the combatSim program
#combatSym(playerChars, enemyChars)
playerCharacters = [bass]
enemyCharacters = [conehead, carl]

participants = playerCharacters + enemyCharacters

while isTeamAlive(playerCharacters) and isTeamAlive(enemyCharacters):
    #gets the new round order
    currentRoundOrder = setRoundTurns(participants)
    #debug print
    print(f"Current round order {currentRoundOrder}")
    
    for i in range(len(currentRoundOrder)):
        currentChar = currentRoundOrder[i]

        #get index to moveset (which is an array of action funciton) + targets
        actionAndTargets = currentChar.getActionAndTargets()
        #actionAndTargets = currentChar.getActionAndTargets("NeuralNetwork")
        #actionAndTargets = currentChar.getActionAndTargets("FuzzyLogic")

        #function takes in all the characters, and 
        #returns a copy of the character objects with the targets
        #having been affected by the action
        participants = carl.doAction(actionAndTargets[0], actionAndTargets[1], participants)

    
    #want remove participant from array if they die

