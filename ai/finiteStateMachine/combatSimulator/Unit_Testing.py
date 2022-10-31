
from characters.Moves.Attack_moves import shove
from utility.setRoundTurns import setRoundTurns
from utility.isTeamAlive import isTeamAlive 
from characters.BaseCharacter import BaseCharacter
from characters.Enemies.Conehead import Conehead
from characters.Enemies.Carl import Carl
from characters.Enemies.Cone_Man import Cone_Man
from characters.Player.Bass import Bass
from characters.Player.Conductor import Conductor
from characters.Player.Drums import Drums
from characters.Player.Flute import Flute
from characters.Items.Weapons.better_flute import Better_Flute

#name = className("print name", hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
carl = Carl(name ="Carl", hp= 50, speed=0, hit= 3, armor= 5,itemModifier= 3,speedModifier= 3,dodgeModifier= 1)
conehead = Conehead("Conehead", 50, 3, 2, 1, 3, 3, 5) 
coneheadBeta = Conehead("ConeheadBeta", 30, 3, 2, 1, 3, 3, 5) 
coneheadAlpha = Conehead("ConeheadAlpha", 30, 3, 2, 1, 3, 3, 5) 
cone_man = Cone_Man("Cone Man", 10, 3, 2, 1, 3, 3, 5)

bass = Bass("Bassist", 7, 1, 3, 0, 0, 0, 0)
conductor = Conductor("Conductor", 5, 3, 2, 1, 0, 0, 0)
drums = Drums("Drums", 10, 3, 2, 3, 0, 0, 0)
flute = Flute("Flute", 5, 4, 4, 2, 0, 0, 0)



#this is going to be passed in to the combatSim program
#combatSym(playerChars, enemyChars)
playerCharacters = [flute, conductor, drums, bass]
enemyCharacters = [conehead, cone_man, coneheadAlpha, carl]

roundNum = 1

participants = playerCharacters + enemyCharacters

print("Round Number: ", roundNum)
print(carl)
Carl.print_stats(carl)
print(carl.name, " has ", Carl.return_hp(carl))

#basic funciton test
print(carl.name, " has taken damage. Blame Covid")
carl.hp -= 30
print(carl.name, " has ", Carl.return_hp(carl), " hp left. Lets heal them! Repair Function Deus Ex Alex")
carl.buff(carl)
print(carl.name, " has healed! Carl now has", Carl.return_hp(carl), " hp left. Thanks Alex!")
print("\n")
print(carl.name, " has slowed down. Blame Society")

print(carl.name, " was ", carl.speedModifier)
carl.debuff(carl)
print(carl.name, " is speed", carl.speedModifier)




#function to print out the array
def printCombatArray(partipants):
    positionString =""
    for i in range(len(participants)):
        newBit = (f"[ {participants[i].name} ] ")
        positionString = positionString + newBit
    print (positionString)

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

shove(bass, drums)

flute_test=Better_Flute(name="Flute of Bacon", tempS=0, swiftness=0, healNum=0, statEffect=0,rarity="common")
Better_Flute.weaponPrint(flute_test)