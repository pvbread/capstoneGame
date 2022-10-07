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
        print(f"{character.name} got a initiative of {totalSpeedScore} and a speed stat of {character.speed + character.speedModifier}")
        if -totalSpeedScore not in speedMap:
            speedMap[-totalSpeedScore] = [character]
        else:
            #later have to account for player characters
            speedMap[-totalSpeedScore].append(character)
            # in case the initiaive is a tie. higher speed stat goes first. 
            for i in range(len(speedMap[-totalSpeedScore])):
                for j in range(len(speedMap[-totalSpeedScore]) - i - 1):
                    if (speedMap[-totalSpeedScore][j].speed + speedMap[-totalSpeedScore][j].speedModifier) > (speedMap[-totalSpeedScore][j+1].speed + speedMap[-totalSpeedScore][j+1].speedModifier):
                        speedMap[-totalSpeedScore][j], speedMap[-totalSpeedScore][j+1] = speedMap[-totalSpeedScore][j+1], speedMap[-totalSpeedScore][j]
                    # if speed is the same. lower health character goes first
                    elif (speedMap[-totalSpeedScore][j].speed + speedMap[-totalSpeedScore][j].speedModifier) == (speedMap[-totalSpeedScore][j+1].speed + speedMap[-totalSpeedScore][j+1].speedModifier):
                        if speedMap[-totalSpeedScore][j].hp < speedMap[-totalSpeedScore][j+1].hp:
                            speedMap[-totalSpeedScore][j], speedMap[-totalSpeedScore][j+1] = speedMap[-totalSpeedScore][j+1], speedMap[-totalSpeedScore][j]
                    
                
            
        #python doesn't provide a max heap or maxpq, so this is a pattern to do so
        provisionalOrder.put(-totalSpeedScore, totalSpeedScore)
    while not provisionalOrder.empty():
        speedKey = provisionalOrder.get()
        characterToAdd = speedMap[speedKey][-1]
        speedMap[speedKey].pop()  
        roundOrder.append(characterToAdd)      
    return roundOrder
