from .. import BaseCharacter

import random

class BaseEnemy(BaseCharacter.BaseCharacter):
    def __init__(self, name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier):
        super().__init__(name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
        
        

        self.validMovesAndRanges = {0: [1,2], 1:[], 2:[]} #these have to be relative positions

    def __str__(self):
        return f"{self.name, self.hp, self.speed, self.hit, self.armor, self.itemModifier, self.speedModifier, self.dodgeModifier}"