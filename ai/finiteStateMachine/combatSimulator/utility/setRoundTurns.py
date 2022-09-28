from queue import PriorityQueue
import random

#assumes there's a character class
def setRoundTurns(characters):
    '''
    Input: Takes an array (living characters) 
    Output: return an array of characters in the order for the next round
    '''
    roundOrder = []
    #map to keep track of which speed score corresponds to which character
    speedMap = {}
    provisionalOrder = PriorityQueue()
    for character in characters:
        totalSpeedScore = character.speed + character.speedModifier + random.randint(1,6)
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