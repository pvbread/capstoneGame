from BaseCharacter import BaseCharacter
import random

class Carl(BaseCharacter):
    def __init__(self, name, hp, speed, hit, itemModifier, speedModifier, dodgeModifier):
        super().__init__(name, hp, speed, hit, itemModifier, speedModifier, dodgeModifier)
        self.moveSet = []

    def attack(self, targetCharacter):
        roll = random.randint(1,6) + self.hit + self.itemModifier - targetCharacter.dodgeModifier
        if roll 