#This will be used when a character dies
#empty space will fill the far most left side of the player side
#empty space will fill the far most right side of the enemies side
#players/ enemies who die on opposite side of where space will be filled get moved closer towards one another
#players get shifted to the right & emptySpace's will get moved to left
#enemies get shifted to the left & emptySpace's will get moved to right 
#if player or enemie attempts to attack, buff or move on said space it doesnt allow
# 
#to check who wins or looses we need to write in combatSim.py if it sees 4 emptySpace characters on 
#   player side or enemie side the opposing side wins and end combat
#
#When character is dead and player tries to click on space with mouse it wont display anything and
#   wont take input. For keybord when selecting terget if it will go over emptySpace immeadetly SDLK_UP/ SDLK_DOWN

def shiftDead(participants):
    # shifting dead characters on player side (left side of the array)
    for i in range(3):
        for j in range(3):
            if participants[j].isAlive and participants[j+1].isAlive == False:
                participants[j], participants[j+1] = participants[j+1], participants[j]
    # shifting dead characters on the enemies side (right side of the array)
    for i in range(3):
        for j in range(3):
            if participants[j+5].isAlive and participants[j+4].isAlive == False:
                participants[j+4], participants[j+5] = participants[j+5], participants[j+4]
    return participants