from . import BaseCharacter
import random

class Carl(BaseCharacter.BaseCharacter):
    def __init__(self, name, hp, speed, hit, itemModifier, speedModifier, dodgeModifier):
        super().__init__(name, hp, speed, hit, itemModifier, speedModifier, dodgeModifier)
        self.moveSet = []

    def attack(self, targetCharacter):
        damage = 5
        print(f"enemy dodgeModifier is {targetCharacter.dodgeModifier}")
        roll = random.randint(1,6) + self.hit + self.itemModifier - targetCharacter.dodgeModifier
        if roll > 0:
            return 5
        return 0