from . import BaseCharacter
import random

class Carl(BaseCharacter.BaseCharacter):
    def __init__(self, name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier):
        super().__init__(name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
        self.moveSet = []

    def attack(self, targetCharacter):
        '''
        Input: Takes in targetCharacter object as output.
        Calculate an attack (dice roll + modifier *definition tbd)
        Output: return amount of damage (if miss, 0)
        '''
        roll = random.randint(1, 6)
        #if we hit
        if (roll + self.hit >= targetCharacter.armor):
            return self.hit
        #if we miss
        return 0
