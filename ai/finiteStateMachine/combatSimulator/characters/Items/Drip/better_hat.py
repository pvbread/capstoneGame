from .. import BaseItem
import random

class better_hat(BaseItem.BaseItem):
    def __init__(self, name, evasion, defense, agility, tempS, swiftness, healNum, statEffect):
        super().__init__(self, name, evasion, defense, agility, tempS, swiftness, healNum, statEffect)

        self.name= "Yankees Cap"
        self.defense= 1
