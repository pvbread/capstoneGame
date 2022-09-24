import random
from enum import Enum
import sys
from copy import deepcopy

class Direction(Enum):
    LEFT = 1
    UP = 2
    RIGHT = 3
    DOWN = 4

step = {
    Direction.LEFT : (-1, 0),
    Direction.UP : (0, -1),
    Direction.RIGHT : (1, 0),
    Direction.DOWN : (0, 1)
}

newDir = random.randint(1,4)

rows, cols = (27, 27)
room = [[0] * cols] * rows

def printRoom(room):
    for row in range(len(room)):
        print(room[row])

print(step[Direction(newDir)])
print(Direction(newDir).name)
#printRoom(room)

pos = (0, 0)

    

#def drawDrunkWalk(room):
#    return travel(0, 0, room)

def drawDrunkWalk(room):
    r, c = 0, 0
    counter = 0
    doNotEnter = deepcopy(room)
    while True:
        room[r][c] = 1 
        validMove = False
        while not validMove:
            newDir = random.randint(1,4)
            print(Direction(newDir).name)
            newStep = step[Direction(newDir)]
            #print(newStep)
            newR = r + newStep[0]
            newC = c + newStep[1]
            counter += 1
            if (counter > 50):
                return room
            if newR < 0 or newC < 1 or newR > len(room)-1 or newC >len(room[0])-1 or room[newR][newC]:
                continue
            r, c = newR, newC
            print(r, c)
            validMove = True
            #printRoom(room)
        if r == len(room)-1 and c == len(room[0])-1:
            return room
        
    

room = drawDrunkWalk(room)
printRoom(room)
