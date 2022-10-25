from .. import BaseCharacter

import random

class BaseEnemy(BaseCharacter.BaseCharacter):
    def __init__(self, name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier):
        super().__init__(name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
        
        self.validMovesAndRanges = {0: [1,2], 1:[3,4], 2:[5,6]} #these have to be relative positions
        #(edit: added further ranges to make the combat sim rounds go quicker)

    def __str__(self) -> str:
        return super().__str__()

    