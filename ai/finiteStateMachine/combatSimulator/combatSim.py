from utility.setRoundTurns import setRoundTurns
from utility.isTeamAlive import isTeamAlive 
from characters.BaseCharacter import BaseCharacter
from characters.Enemies.Conehead import Conehead
from characters.Enemies.Carl import Carl

'''
CombatSim aims to instantiate the proper game state, round states
and simulate rounds until the combat finishes
We can have the AI decision policy be added modularly.
'''

#name = className("print name", hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
carl = Carl(name ="Carl", hp= 50, speed=0, hit= 3, armor= 5,itemModifier= 3,speedModifier= 3,dodgeModifier= 1)
conehead = Conehead("Conehead", 50, 3, 2, 1, 3, 3, 5) 
bass = BaseCharacter("bassist", 50, 2, 3, 2, 3, 3, 6)

#this is going to be passed in to the combatSim program
#combatSym(playerChars, enemyChars)
playerCharacters = [bass]*4
enemyCharacters = [conehead, conehead, conehead, carl]

roundNum = 1

participants = playerCharacters + enemyCharacters
moves = carl.getValidMoves(3,7, playerCharacters,enemyCharacters,participants) # consider making the input to be more user friendly

print("Round Number: ", roundNum)
print(carl)
Carl.print_stats(carl)
print(carl.name, " has ", Carl.return_hp(carl))

"""
while isTeamAlive(playerCharacters) and isTeamAlive(enemyCharacters):
    print(f"Round {roundNum}")
    #gets the new round order
    currentRoundOrder = setRoundTurns(participants)
    #debug print
    print(f"Current round order {currentRoundOrder}")
    
    for i in range(len(currentRoundOrder)):
        currentChar = currentRoundOrder[i]
        if currentChar.isAlive == False:
            continue #formerly break #we could remove them from participants but may want to keep revive option

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
    roundNum += 1
    input("Press ENTER to continue\n")
    ## think about, how are we going to read in event input
    ## we're actually going to sit on a loop, doing nothing
    ## once legal input action is selected, we process
if isTeamAlive(playerCharacters):
    print("Victory")
if isTeamAlive(enemyCharacters):
    print("Defeat")
"""