from .. import BaseItem
import random

class better_hat(BaseItem.BaseItem):
    def __init__(self, name, Evasion, Defense, Agility, TempS, Swiftness, HealNum, StatEffect):
        super().__init__(self, name, Evasion, Defense, Agility, TempS, Swiftness, HealNum, StatEffect)

    name= "Yankees Cap"
    Defense= 1
