from . import BaseEnemy
import random

class Conehead(BaseEnemy.BaseEnemy):
    def __init__(self, name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier):
        super().__init__(name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
        self.moveSet = [self.attack]
    '''
    def attack(self, targetCharacter):
        damage = 4
        roll = random.randint(1,6) + self.hit + self.itemModifier - targetCharacter.dodgeModifier
        if roll > 0:
            return 4
        return 0
    '''
