import combatSim
from combatSim import carl
from combatSim import conehead
from combatSim import coneheadAlpha
from combatSim import coneheadBeta
from combatSim import bass
from combatSim import Carl
from combatSim import roundNum
from combatSim import participants

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
