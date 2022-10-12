from . import BasePlayer

import random

class Bass(BasePlayer.BasePlayer):
    def __init__(self, name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier):
        super().__init__(name, hp, speed, hit, armor, itemModifier, speedModifier, dodgeModifier)
        self.moveSet = [self.attack]

