from utility.setRoundTurns import setRoundTurns
from utility.isTeamAlive import isTeamAlive 
from characters.BaseCharacter import BaseCharacter
from characters.Conehead import Conehead
from characters.Carl import Carl

#name = className("print name", hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
carl = Carl("Carl", 100, 2, 3, 10, 3, 3, 3)
conehead = Conehead("Conehead", 100, 2, 3, 5, 3, 3, 6) 
bass = BaseCharacter("bassist", 100, 2, 3, 4, 3, 3, 6)

#carl.attack(conehead)


#this is going to be passed in to the combatSim program
#combatSym(playerChars, enemyChars)
playerCharacters = [bass]
enemyCharacters = [conehead, carl]

conehead.isAlive, carl.isAlive = False, False
alive = isTeamAlive(enemyCharacters)
print(f"enemyTeam isAlive?: {alive}")

participants = playerCharacters + enemyCharacters

#gets an array in a new order for the next
thisRoundTest = setRoundTurns(participants)

#test a setting of rounds
for char in thisRoundTest:
    print(char.name)

#while ()

