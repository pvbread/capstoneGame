from .. import BaseItem
import random

class Muffin(BaseItem.BaseItem):
    def __init__(self, name, evasion, defense, agility, tempS, swiftness, healNum, statEffect):
        super().__init__(self, name, evasion, defense, agility, tempS, swiftness, healNum, statEffect)

        healNum=2