import random
import math
from unicodedata import name


class BaseCharacter:
    def __init__(self, name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier):
        self.name = name
        self.hp = hp
        self.maxHp = hp     #we do not currently have items that increase max hp
        self.speed = speed
        self.hit = hit
        self.armor = armor
        self.itemModifier = itemModifier #Filler for encounters/item drop rate/ etc unsused at the moment
        self.speedModifier = speedModifier
        self.dodgeModifier = dodgeModifier
        self.isAlive = True
        self.moveSet = [self.attack, self.buff, self.debuff, self.move] #is it true everyone has these moves
        self.moveType = {0: 'attack', 1:'buff', 2:'debuff', 3:'move'} #Index from moveset maps to string that discribes the type
        #for example

        # open question is whether it's better to have absolute
        # numbers (i.e. enemies have negative numbers as attack ranges)
        # versus relative numbers and then we have to write a lot of conditionals
        
        '''
        self.moveset = [self.attack, self.move]
        self.validMoves =  {
                             0: [[5],[6],[7,8]], 
                             1: [1,2,3] #but you wouldn't be able to move into enemy territory
                            }
        #need to write this function getValidMoves(attack) -> i+1 or i+2
        '''
  
    def getActionAndTargets(self, players, enemies, participants, decision=None):
        '''
        Input: decision algorithm, if none, use random choice
        Output: a index to the moveset actions, and array of targets
        '''
        if decision == None:
            move = random.randint(0,len(self.moveType))
            charIndex = participants.index(self)
            targets = self.getValidMoves(move, charIndex, players, enemies, participants)

            
            #before we pick this action, we have to make sure, there's
            #a valid opponent for this action
            #if option == 0: # if random choice is to attack
            #if we choose an attack, we need to know what valid targets are for that attack
            #so we have multiple choices, but also within those, we can hit multiple enemies
            #so array of array<int> for valid targets for a given attack
            #but then we need a map of indexes (that refer to moves from moveset)
            #to said valid moves

            #so now we get a random legal move from the move "0"
            #and we check if there's an enemy there, if not we keep rolling
            #

            #put in to avoid attacking ally
            '''
                if self in players: # if player, choose enemies as targets
                    for character in enemies:
                        if character.isAlive == True:
                            targets.append(participants.index(character))
                if self in enemies: # if enemy, choose players as targets
                    for character in players:
                        if character.isAlive == True:
                            targets.append(participants.index(character))
                '''
            return [move, targets]

    def doAction(self, move, targets, participants):
        '''
        Input: index to moveset, targets, and participants object to affect
        Output: modified participants object
        '''
        # if the move chosen is to attack targets
        if move == 0:
            print(f"{self.name} chose attack")
            if len(targets)==0:
                print("No one to attack")
            for target in targets:
                hit = self.moveset[move](participants[target]) # call attack function
                participants[target].hp = participants[target].hp - hit
                if participants[target].hp <= 0:
                    participants[target].isAlive = False
                    print(f"{participants[target].name} is dead")
                    participants.remove(participants[target])               
        return participants

    def getValidMoves(self, move, charIndex, players, enemies, participants):
        '''
        Input: move is a moveset index to move
        Output: an array of array<int> of valid action indexes
        Example: getValidMoves(0) and 0 is index to attack
        Returns [1,2] for Carl, meaning Carl can attack from
        his position i: i-1, i-2 (because it's an enemy)
        [1,2,3,4,5,6,7,8] if carl is at position 5, he can attack
        validly 3 and 4 because 5-1 = 4 and 5-2 = 3
        '''
        #get move type 
        if self.moveType[move] == 'attack':
            if self in players:
                validMoves = []
                for position in self.validMovesAndRanges[move]:
                    if charIndex + position < len(participants):
                        validMoves.append(charIndex + position)
            else:
                #if self in enemies
                validMoves = []
                for position in self.validMovesAndRanges[move]:
                    if charIndex - position >= 0:
                        validMoves.append(charIndex - position)
                
            return validMoves 

        if self.moveType[move] == 'buff':  
            validMoves = self.getValidBuffTargets(players, participants, "buff")

        if self.moveType[move] == 'debuff':
            validMoves = self.getValidBuffTargets(players, participants, "debuff")

        if self.moveType[move] == 'move':
            validMoves = self.getValidBuffTargets(players, participants, "buff")
            # doesn't include its charIndex
            validMoves = list(filter(lambda i: i != charIndex, validMoves))
        return validMoves
        
    def getValidBuffTargets(self, players, participants, typeBuff):
        #  return [0,1,2,3]
        if (self in players and typeBuff == "buff") or (self not in players and typeBuff == "debuff"):
            validMoves = []
            for i in range(4):
                if participants[i].isAlive: 
                    validMoves.append(i) 
        #  return [4,5,6,7]
        else:
            #if self in enemies
            validMoves = []
            for i in range(4):
                if participants[i+4].isAlive:
                    validMoves.append(i+4) 
        return validMoves


    def attack(self, targetCharacter):
        '''
        Input: Takes in targetCharacter object as output.
        Calculate an attack (weapon dice roll + modifier - (target's armor *0.5)? tbd)
        Output: return amount of damage (if miss, 0)
        '''
        # if we miss
        dodgeChance = targetCharacter.dodgeModifier * 0.02
        roll = random.uniform(0, 1) # randomly pick a real number between 0 and 1
        if (roll <= dodgeChance):
            print(f"{targetCharacter.name} dodges attack" )
            return 0
        #we hit
        weaponRoll = random.randint(1,4) # need to add how to keep track of weapon (common weapon for now)
        damage = weaponRoll + self.hit 
        reduction = int(damage * (targetCharacter.armor * 0.05))
        damage = damage - reduction
        print(f"{targetCharacter.name} takes {damage} damage. Health is at {targetCharacter.hp - damage}")
        return damage

    def buff(self, targetCharacter): #Heal
        '''
        Input: Takes in targetCharacter object as output.
        Increase hp of character
        Print heal result in sentance
        Output: return new hp number
        '''
        #gonna have to figure out how to prevent overheal
        #Scenario 1: Target already at or surpasses max hp
        if (targetCharacter.hp >= targetCharacter.maxHp):
            print(f"{targetCharacter.name} can't Heal, their healthy.")
            return 0

        #Scenario 2: Target is at 0 hp
        elif (targetCharacter.isAlive == False):
            print(f"{targetCharacter.name} is out of the fight.")
            return 0

        #Scenario 3: Target is between 0-> maxHp not inclusive
        elif (targetCharacter.hp < targetCharacter.maxHp):
            #math %10 hp heal, rounded up
            healAmount = math.ceil(targetCharacter.maxHp * .1)
            targetCharacter.hp += healAmount
            print(f"{targetCharacter.name} heals {healAmount}. Health is at {targetCharacter.hp}.")
            return targetCharacter.hp

    def debuff(self, targetCharacter): #Slow
        '''
        Input: Takes in targetCharacter object as output.
        Decreases speedModifier of character
        Print debuff result in sentance
        Output: return new speed number
        '''
        #math decrease targetCharacter by 1
        targetCharacter.speedModifier -= 1
        print(f"{targetCharacter.name} speed slows by 1 tempo. Speed is at {(targetCharacter.speed + targetCharacter.speedModifier)}.")
        return targetCharacter.speedModifier

    def move(self, targetCharacters, participants): #Move forward 1
        # This is not the Conductors swap
        #This is A moving forward/backwards 1 step and Character B going to the slot A was at
        #[A][B][C][D] -> [B][A][C][D] and vice versa
        #dont need to check [3]vs[4] positions
        charA = participants.index(self)
        charB = participants.index(targetCharacters)
        participants[charA], participants[charB]= participants[charB], participants[charA]
        print(f"{self.name} has switched places with {targetCharacters.name}.")
        
        return participants

    ###### Important NOte #####
    

    #text utility functions

    
    def print_stats(self):
        print (self.name, " stats currently are:")
        print ("name = ", self.name)
        print ("hp = ", self.hp)
        print ("speed = ", self.speed)
        print ("hit = ", self.hit)
        print ("armor = ", self.armor)
        print ("itemModifier = ", self.itemModifier)
        print ("speedModifier = ", self.speedModifier)
        print ("dodgeModifier = ", self.dodgeModifier)

    def return_hp(BaseCharacter):
        return(BaseCharacter.hp)