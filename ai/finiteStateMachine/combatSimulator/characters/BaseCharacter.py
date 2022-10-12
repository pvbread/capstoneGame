import random


class BaseCharacter:
    def __init__(self, name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier):
        self.name = name
        self.hp = hp
        self.speed = speed
        self.hit = hit
        self.armor = armor
        self.itemModifier = itemModifier
        self.speedModifier = speedModifier
        self.dodgeModifier = dodgeModifier
        self.isAlive = True
        self.moveset = [self.attack, self.buff, self.debuff, self.move] #is it true everyone has these moves
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
            option = random.randint(0,0) # need to add other attacks/buff/debuff in range
            targets = []
            
            #before we pick this action, we have to make sure, there's
            #a valid opponent for this action
            if option == 0: # if random choice is to attack
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
            return [option, targets]

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
                        validMoves.append(participants[])#need to talk rules to define attack with range
            else:
                #if self in enemies
                validMoves = []
                for position in self.validMovesAndRanges[move]:
                    if charIndex - position >= 0:
                        validMoves.append(charIndex - position)
                
            return validMoves 

        if self.moveType[move] == 'buff':
            if self in players:
                validMoves = []
                for position in self.validMovesAndRanges[move]:
                    if charIndex < len(participants) - 4: 
                        validmoves.append()

        if self.moveType[move] == 'debuff':
            pass

        if self.moveType[move] == 'move':
            pass

        #check if it's a player
        if charIndex < 5:
            #check for type of move
                #if attack, sum the move distance
                #and check if there are existing enemies
            
                #if it's a move, you can add, subtract,
                #but need to keep it under < 5

                # if it's a buff??


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
        damage = int(weaponRoll + self.hit - (targetCharacter.armor * 0.5))
        print(f"{targetCharacter.name} takes {damage} damage. Health is at {targetCharacter.hp-damage}")
        return damage

    def buff(self, targetCharacter):
        pass

    def debuff(self, targetCharacter):
        pass

    def move(self, targetCharacters):
        pass