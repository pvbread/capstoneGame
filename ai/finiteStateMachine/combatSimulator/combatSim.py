import random
from queue import PriorityQueue
from Character import Character

#assumes there's a character class
def setRoundTurns(characters):
    '''
    Takes an array (living characters) and return an array of characters in the order for the next round
    '''
    roundOrder = []
    #map to keep track of which speed score corresponds to which character
    speedMap = {}
    provisionalOrder = PriorityQueue()
    for character in characters:
        totalSpeedScore = character.speed + character.speed_modifier + random.randint(1,6)
        print(f"{character.name} got a speed score of {totalSpeedScore}")
        if totalSpeedScore not in speedMap:
            speedMap[-totalSpeedScore] = [character] 
        else:
            #later have to account for player characters
            speedMap[totalSpeedScore].append(character)
        #python doesn't provide a max heap or maxpq, so this is a pattern to do so
        provisionalOrder.put(-totalSpeedScore, totalSpeedScore)
    while not provisionalOrder.empty():
        speedKey = provisionalOrder.get()
        characterToAdd = speedMap[speedKey][-1]
        speedMap[speedKey].pop()  
        roundOrder.append(characterToAdd)      
    return roundOrder

carl = Character("Carl", 100, 1, 0)
conehead = Character("Conehead", 50, 5, 1)
banana = Character("banana", 50, 3, 0)

participants = [carl, conehead, banana]

thisRoundTest = setRoundTurns(participants)

for char in thisRoundTest:
    print(char.name)


