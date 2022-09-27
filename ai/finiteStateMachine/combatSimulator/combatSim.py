from setRoundTurns import setRoundTurns
from characters.BaseCharacter import BaseCharacter
from characters.Conehead import Conehead
from characters.Carl import Carl

carl = Carl("Carl", 100, 2, 3, 3, 3, 3)
conehead = Conehead("Conehead", 100, 2, 3, 3, 3, 6) 
banana = BaseCharacter("banana", 100, 2, 3, 3, 3, 6)

carl.attack(conehead)

#this is going to be passed in to the combatSim program
#combatSym(playerChars, enemyChars)
playerCharacters = [banana]
enemyCharacters = [conehead, carl]

participants = playerCharacters + enemyCharacters

thisRoundTest = setRoundTurns(participants)

#test a setting of rounds
for char in thisRoundTest:
    print(char.name)

#while ()

