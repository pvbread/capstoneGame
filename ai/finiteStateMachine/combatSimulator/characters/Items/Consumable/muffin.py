from .. import BaseItem
import random

class muffin(BaseItem.BaseItem):
    def __init__(self, name, evasion, defense, agility, tempS, swiftness, healNum, statEffect):
        super().__init__(self, name, evasion, defense, agility, tempS, swiftness, healNum, statEffect)

        self.healNum=2