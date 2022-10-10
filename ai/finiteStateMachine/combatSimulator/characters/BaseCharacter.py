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
        self.moveset = [self.attack]
  
    def getActionAndTargets(self, players, enemies, participants, decision=None):
        '''
        Input: decision algorithm, if none, use random choice
        Output: a index to the moveset actions, and array of targets
        '''
        if decision == None:
            option = random.randint(0,0) # need to add other attacks/buff/debuff in range
            targets = []
            if option == 0: # if random choice is to attack
                if self in players: # if player, choose enemies as targets
                    for character in enemies:
                        if character.isAlive == True:
                            targets.append(participants.index(character))
                if self in enemies: # if enemy, choose players as targets
                    for character in players:
                        if character.isAlive == True:
                            targets.append(participants.index(character))
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
