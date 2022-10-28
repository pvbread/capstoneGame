from utility.setRoundTurns import setRoundTurns
from utility.isTeamAlive import isTeamAlive 
from characters.BaseCharacter import BaseCharacter
from characters.Enemies.Conehead import Conehead
from characters.Enemies.Carl import Carl
from characters.Player.Drums import Drums
from characters.Player.Conductor import Conductor
from characters.Player.Flute import Flute
from characters.Player.Bass import Bass
# used sleep to delay by 1 second to print character's turn
from time import sleep # feel free to remove/comment out at line 116

'''
CombatSim aims to instantiate the proper game state, round states
and simulate rounds until the combat finishes
We can have the AI decision policy be added modularly.
'''

#name = className("print name", hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
carl = Carl(name ="Carl", hp= 30, speed=0, hit= 3, armor= 5,itemModifier= 3,speedModifier= 3,dodgeModifier= 1)
conehead = Conehead(name = "Conehead", hp=30, speed=3, hit=2, armor=1, itemModifier=3, speedModifier=3, dodgeModifier=5) 
coneheadBeta = Conehead(name = "ConeheadBeta", hp=30, speed=3, hit=2, armor=1, itemModifier=3, speedModifier=3, dodgeModifier=5) 
coneheadAlpha = Conehead(name = "ConeheadAlpha", hp=30, speed=3, hit=2, armor=1, itemModifier=3, speedModifier=3, dodgeModifier=5) 
bass = Bass(name="Bassist", hp=30, speed=1, hit=3, armor=0, itemModifier=3, speedModifier=3, dodgeModifier=3)
drum = Drums("Drunmmer", hp=30, speed=3, hit=3, armor=3, itemModifier=3, speedModifier=3, dodgeModifier=2)
flute = Flute("Flutist", hp=30, speed=4, hit=3, armor=2, itemModifier=3, speedModifier=3, dodgeModifier=4)
conductor = Conductor("Conductor", hp=30, speed=3, hit=3, armor=1, itemModifier=3, speedModifier=3, dodgeModifier=2)


#this is going to be passed in to the combatSim program
#combatSym(playerChars, enemyChars)
playerCharacters = [flute, conductor, drum, bass]
enemyCharacters = [conehead, coneheadBeta, coneheadAlpha, carl]

roundNum = 1

participants = playerCharacters + enemyCharacters

"""
# Testing Functions

moves = carl.getValidMoves(3,7, playerCharacters,enemyCharacters,participants) # consider making the input to be more user friendly



print(carl.name, " was ", carl.speedModifier)
carl.debuff(carl)
print(carl.name, " is speed", carl.speedModifier)
"""
#function to print out the array
def printCombatArray(participants):
    positionString =""
    for i in range(len(participants)):
        newBit = (f"[ {participants[i].name} ] ")
        positionString = positionString + newBit
    print("Character Positions:")
    print (positionString)
# function to print turn order
def printTurnOrder(currentRoundOrder):
    orderString = ""
    for i in range(len(currentRoundOrder)):
        if currentRoundOrder[i].isAlive:
            newBit = (f"{i+1,currentRoundOrder[i].name} ")
            orderString = orderString + newBit
    print("Turn Order:")
    print(orderString)
"""
# Testing Functions
print("\n")
printCombatArray(participants)

print("\n")
carl.move(coneheadAlpha, participants)
printCombatArray(participants)

print("\n")
carl.move(conehead, participants)
print("move is not concerned with targets being next to each other, that is the concren of checkValidMoves")
printCombatArray(participants)

print("\n")
carl.move(bass, participants)
print("move is not concerned with legality, that is the concren of checkValidMoves")
printCombatArray(participants)


"""

# game loop takes in user input for players' turn. Fuzzy logic not implemented yet, enemy decisions are random for now.  
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
